/**
 * @file BLEServerCallbacks.cpp
 * @brief Implémentation de la classe MyBLEServerCallbacks pour gérer les callbacks du serveur BLE.
 */

#include "BLEServerCallbacks.h"
#include "BLECommunication.h"

/**
 * @brief Méthode appelée lorsqu'un appareil se connecte au serveur BLE.
 * @param pServer Pointeur vers le serveur BLE.
 */
void MyBLEServerCallbacks::onConnect(NimBLEServer* pServer) {
    BLECommunication::getInstance()->setDeviceConnected(true);
    Serial.println("Device connected");
}

/**
 * @brief Méthode appelée lorsqu'un appareil se déconnecte du serveur BLE.
 * @param pServer Pointeur vers le serveur BLE.
 */
void MyBLEServerCallbacks::onDisconnect(NimBLEServer* pServer) {
    BLECommunication::getInstance()->setDeviceConnected(false);
    Serial.println("Device disconnected");
}
