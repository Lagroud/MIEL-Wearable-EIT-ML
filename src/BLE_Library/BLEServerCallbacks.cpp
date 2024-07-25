#include "BLEServerCallbacks.h"
#include "BLECommunication.h"

void MyBLEServerCallbacks::onConnect(NimBLEServer* pServer) {
    BLECommunication::getInstance()->setDeviceConnected(true);
    Serial.println("Device connected");
}

void MyBLEServerCallbacks::onDisconnect(NimBLEServer* pServer) {
    BLECommunication::getInstance()->setDeviceConnected(false);
    Serial.println("Device disconnected");
}