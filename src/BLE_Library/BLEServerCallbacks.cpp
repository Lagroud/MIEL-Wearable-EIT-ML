/**
 * @file BLEServerCallbacks.cpp
 * @brief Implémentation de la classe MyBLEServerCallbacks pour gérer les callbacks du serveur BLE.
 */

#include "BLEServerCallbacks.h"
#include "BLECommunication.h"
#include "EIT_Interface_Display.h"


/**
 * @brief Méthode appelée lorsqu'un appareil se connecte au serveur BLE.
 * @param pServer Pointeur vers le serveur BLE.
 */
void MyBLEServerCallbacks::onConnect(NimBLEServer* pServer) {
    BLECommunication::getInstance()->setDeviceConnected(true);
    if(page == 3){
        //drawConfigScreen(rtc.getDate().month, rtc.getDate().date, rtc.getDate().year, rtc.getTime().hours, rtc.getTime().minutes);
    }
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
