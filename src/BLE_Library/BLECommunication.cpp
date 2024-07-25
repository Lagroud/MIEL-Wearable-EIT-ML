#include "BLECommunication.h"
#include "BLEConfig.h"
#include <WiFi.h>

BLECommunication *BLECommunication::instance = nullptr;

BLECommunication::BLECommunication()
    : deviceConnected(false), waitingForClientResponse(false), lastNotifyTime(0), notifyInterval(1000), pTxCharacteristic(nullptr), started(false)
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

BLECommunication *BLECommunication::getInstance()
{
    if (instance == nullptr)
    {
        instance = new BLECommunication();
    }
    return instance;
}

void BLECommunication::setTxCharacteristic(NimBLECharacteristic *characteristic)
{
    pTxCharacteristic = characteristic;
}

NimBLECharacteristic *BLECommunication::getTxCharacteristic() const
{
    return pTxCharacteristic;
}

void BLECommunication::sendStringToClient(const std::string &str)
{
    if (deviceConnected && pTxCharacteristic)
    {
        pTxCharacteristic->setValue(str);
        pTxCharacteristic->notify();
        M5.Lcd.println("Sent: " + String(str.c_str()));
    }
    else
    {
        M5.Lcd.println("No device connected or characteristic not set to send the string");
    }
}

void BLECommunication::sendCsvDataToClient(const String &str, double impedanceData[8][8])
{
    if (deviceConnected && pTxCharacteristic)
    {
        // Construct data line
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
        // Maximum chunk size for BLE transmission (20 bytes is common, but verify for your device)
        const int chunkSize = 20;
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


void BLECommunication::sendInitialMessage()
{
    sendStringToClient("Welcome to the ESP32 chat!");
}

void BLECommunication::sendCsvEndMarker()
{
    if (deviceConnected && pTxCharacteristic)
    {
        String endMarker = "<END>";
        pTxCharacteristic->setValue(endMarker);
        pTxCharacteristic->notify();
    }
}

void BLECommunication::setDeviceConnected(bool connected)
{
    deviceConnected = connected;
}

bool BLECommunication::isDeviceConnected() const
{
    return deviceConnected;
}

void BLECommunication::setWaitingForClientResponse(bool waiting)
{
    waitingForClientResponse = waiting;
}

bool BLECommunication::isWaitingForClientResponse() const
{
    return waitingForClientResponse;
}

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
