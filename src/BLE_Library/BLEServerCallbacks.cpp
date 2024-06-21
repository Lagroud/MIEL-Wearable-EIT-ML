#include "BLEServerCallbacks.h"
#include "BLECommunication.h"

void MyBLEServerCallbacks::onConnect(NimBLEServer* pServer) {
    BLECommunication::getInstance()->setDeviceConnected(true);
    BLECommunication::getInstance()->setFirstConnect(true);
    M5.Lcd.println("Device connected");
}

void MyBLEServerCallbacks::onDisconnect(NimBLEServer* pServer) {
    BLECommunication::getInstance()->setDeviceConnected(false);
    M5.Lcd.clear();
    M5.Lcd.println("Device disconnected");
}