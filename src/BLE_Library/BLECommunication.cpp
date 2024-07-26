/**
 * @file BLECommunication.cpp
 * @brief Implémentation de la classe BLECommunication pour la gestion de la communication Bluetooth Low Energy (BLE).
 */

#include "BLECommunication.h"
#include "BLEConfig.h"
#include <WiFi.h>

/**
 * @brief Instance unique de la classe BLECommunication.
 */
BLECommunication *BLECommunication::instance = nullptr;

/**
 * @brief Constructeur par défaut de la classe BLECommunication.
 */
BLECommunication::BLECommunication()
    : deviceConnected(false), pTxCharacteristic(nullptr), started(false)
{
    NimBLEDevice::init("ESP32_BLE");
    pServer = NimBLEDevice::createServer();
    if (!pServer)
    {
        Serial.println("Error creating BLE server");
        return;
    }

    pServer->setCallbacks(new MyBLEServerCallbacks());
    NimBLEService *pService = pServer->createService(UUID_SERVICE);
    if (!pService)
    {
        Serial.println("Failed to create BLE service");
        return;
    }

    NimBLECharacteristic *pTxCharacteristic = pService->createCharacteristic(
        UUID_TX,
        NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::NOTIFY);
    if (!pTxCharacteristic)
    {
        Serial.println("Failed to create TX characteristic");
        return;
    }

    setTxCharacteristic(pTxCharacteristic);

    pService->start();
    started = pServer->getAdvertising()->start();
    Serial.println("BLE server started");
}

/**
 * @brief Obtient l'instance unique de la classe BLECommunication.
 * @return Pointeur vers l'instance unique de la classe BLECommunication.
 */
BLECommunication *BLECommunication::getInstance()
{
    if (instance == nullptr)
    {
        instance = new BLECommunication();
    }
    return instance;
}

/**
 * @brief Définit la caractéristique de transmission (Tx).
 * @param characteristic Pointeur vers la caractéristique de transmission.
 */
void BLECommunication::setTxCharacteristic(NimBLECharacteristic *characteristic)
{
    pTxCharacteristic = characteristic;
}

/**
 * @brief Obtient la caractéristique de transmission (Tx).
 * @return Pointeur vers la caractéristique de transmission.
 */
NimBLECharacteristic *BLECommunication::getTxCharacteristic() const
{
    return pTxCharacteristic;
}

/**
 * @brief Envoie des données CSV au client.
 * @param str La chaîne de caractères à envoyer.
 * @param impedanceData Tableau contenant les données d'impédance à envoyer.
 */
void BLECommunication::sendCsvDataToClient(const String &str, double impedanceData[8][8])
{
    if (deviceConnected && pTxCharacteristic)
    {
        String dataLine = "";
        for (int i = 0; i < 8; i++)
        {
            for (int j = i + 1; j < 8; j++)
            {
                String data = String(impedanceData[i][j]);
                dataLine += data + ";";
            }
        }
        dataLine += str + "\n";
        const int chunkSize = 20; // Limitation matérielle protocole BLE 20 bytes par bloc de données
        int dataLength = dataLine.length();
        int offset = 0;

        while (offset < dataLength)
        {
            int bytesToSend = min(chunkSize, dataLength - offset);
            String chunk = dataLine.substring(offset, offset + bytesToSend);
            pTxCharacteristic->setValue(chunk);
            pTxCharacteristic->notify();
            delay(10);
            offset += bytesToSend;
        }
    }
}

/**
 * @brief Envoie le marqueur de fin de CSV au client.
 */
void BLECommunication::sendCsvEndMarker()
{
    if (deviceConnected && pTxCharacteristic)
    {
        String endMarker = "<END>";
        pTxCharacteristic->setValue(endMarker);
        pTxCharacteristic->notify();
    }
}

/**
 * @brief Définit l'état de la connexion de l'appareil.
 * @param connected État de la connexion (true si connecté, false sinon).
 */
void BLECommunication::setDeviceConnected(bool connected)
{
    deviceConnected = connected;
}

/**
 * @brief Vérifie si l'appareil est connecté.
 * @return true si l'appareil est connecté, false sinon.
 */
bool BLECommunication::isDeviceConnected() const
{
    return deviceConnected;
}

/**
 * @brief Affiche l'adresse MAC Bluetooth sur le port série.
 */
void BLECommunication::serialPrintBLEMacAddress()
{
    uint8_t baseMac[6];
    esp_read_mac(baseMac, ESP_MAC_BT);
    Serial.print("Bluetooth MAC: ");
    for (int i = 0; i < 5; i++)
    {
        Serial.printf("%02X:", baseMac[i]);
    }
    Serial.printf("%02X\n", baseMac[5]);
}
