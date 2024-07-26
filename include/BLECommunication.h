/**
 * @file BLE_COMMUNICATION_H
 * @brief Déclaration de la classe BLECommunication pour la gestion de la communication Bluetooth Low Energy (BLE).
 */

#ifndef BLE_COMMUNICATION_H
#define BLE_COMMUNICATION_H

#include <NimBLEDevice.h>
#include <M5Unified.h>
#include <BLEConfig.h>
#include <BLEServerCallbacks.h>


/**
 * @class BLECommunication
 * @brief Gère la communication BLE pour les appareils utilisant NimBLE et M5Unified.
 */
class BLECommunication
{
public:
    /**
     * @brief Définit la caractéristique de transmission (Tx).
     * @param characteristic Pointeur vers la caractéristique de transmission.
     */
    void setTxCharacteristic(NimBLECharacteristic *characteristic);
    
    /**
     * @brief Obtient la caractéristique de transmission (Tx).
     * @return Pointeur vers la caractéristique de transmission.
     */
    NimBLECharacteristic *getTxCharacteristic() const;

    /**
     * @brief Constructeur par défaut de la classe BLECommunication.
     */
    BLECommunication();
    
    void startBLEServer();

    void stopBLEServer();
    
    /**
     * @brief Envoie des données CSV au client.
     * @param str La chaîne de caractères à envoyer.
     * @param impedanceData Tableau contenant les données d'impédance à envoyer.
     */
    void sendCsvDataToClient(const String &str, double impedanceData[8][8]);

    /**
     * @brief Envoie le marqueur de fin de CSV au client.
     */
    void sendCsvEndMarker();
    
    /**
     * @brief Définit l'état de la connexion de l'appareil.
     * @param connected État de la connexion (true si connecté, false sinon).
     */
    void setDeviceConnected(bool connected);
    
    /**
     * @brief Vérifie si l'appareil est connecté.
     * @return true si l'appareil est connecté, false sinon.
     */
    bool isDeviceConnected() const;

    /**
     * @brief Vérifie si le serveur BLE est démarré.
     * @return true si le serveur est démarré, false sinon.
     */
    bool isStarted() const;


    /**
     * @brief Affiche l'adresse MAC du module BLE sur le port série, à utiliser une fois pour le paramètrer dans le programme python.
     */
    void serialPrintBLEMacAddress();

    /**
     * @brief Supprime le constructeur de copie pour empêcher la copie de l'objet.
     * @param other L'objet à copier (non utilisé).
     */
    BLECommunication(BLECommunication &other) = delete;
    
    /**
     * @brief Supprime l'opérateur d'affectation pour empêcher la copie de l'objet.
     * @param other L'objet à affecter (non utilisé).
     */
    void operator=(const BLECommunication &) = delete;

    /**
     * @brief Obtient l'instance unique de la classe BLECommunication.
     * @return Pointeur vers l'instance unique de la classe BLECommunication.
     */
    static BLECommunication *getInstance();

private:
    static BLECommunication* instance; /*!< Instance unique de la classe BLECommunication. */
    bool deviceConnected; /*!< État de la connexion de l'appareil. */
    NimBLEServer *pServer; /*!< Pointeur vers le serveur BLE. */
    bool started; /*!< État de démarrage du serveur BLE. */
    NimBLECharacteristic *pTxCharacteristic; /*!< Pointeur vers la caractéristique de transmission. */
};

#endif // BLE_COMMUNICATION_H
