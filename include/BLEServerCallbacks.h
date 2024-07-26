/**
 * @file BLE_SERVER_CALLBACKS_H
 * @brief Déclaration de la classe MyBLEServerCallbacks pour gérer les callbacks du serveur BLE.
 */

#ifndef BLE_SERVER_CALLBACKS_H
#define BLE_SERVER_CALLBACKS_H

#include <NimBLEDevice.h>
#include <M5Unified.h>


/**
 * @class MyBLEServerCallbacks
 * @brief Classe pour gérer les callbacks du serveur BLE.
 */
class MyBLEServerCallbacks : public NimBLEServerCallbacks {
    /**
     * @brief Méthode appelée lorsqu'un appareil se connecte au serveur BLE.
     * @param pServer Pointeur vers le serveur BLE.
     */
    void onConnect(NimBLEServer* pServer) override;

    /**
     * @brief Méthode appelée lorsqu'un appareil se déconnecte du serveur BLE.
     * @param pServer Pointeur vers le serveur BLE.
     */
    void onDisconnect(NimBLEServer* pServer) override;
};

#endif // BLE_SERVER_CALLBACKS_H
