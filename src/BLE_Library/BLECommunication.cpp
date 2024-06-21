#include "BLECommunication.h"
#include "BLEConfig.h"

BLECommunication* BLECommunication::instance = nullptr;

BLECommunication::BLECommunication() 
    : deviceConnected(false), firstConnect(false), waitingForClientResponse(false), lastNotifyTime(0), notifyInterval(1000), pTxCharacteristic(nullptr) {}

BLECommunication* BLECommunication::getInstance() {
    if (instance == nullptr) {
        instance = new BLECommunication();
    }
    return instance;
}

void BLECommunication::setTxCharacteristic(NimBLECharacteristic* characteristic) {
    pTxCharacteristic = characteristic;
}

NimBLECharacteristic* BLECommunication::getTxCharacteristic() const {
    return pTxCharacteristic;
}

void BLECommunication::sendStringToClient(const std::string& str) {
    if (deviceConnected && pTxCharacteristic) {
        pTxCharacteristic->setValue(str);
        pTxCharacteristic->notify();
        M5.Lcd.println("Sent: " + String(str.c_str()));
    } else {
        M5.Lcd.println("No device connected or characteristic not set to send the string");
    }
}

void BLECommunication::sendInitialMessage() {
    sendStringToClient("Welcome to the ESP32 chat!");
}

void BLECommunication::handleBLEConnection() {
    if (deviceConnected && firstConnect) {
        
        firstConnect = false;
        delay(3000);
        sendInitialMessage();
    }

    unsigned long currentTime = millis();
    if (deviceConnected && !waitingForClientResponse && pTxCharacteristic) {
        if (currentTime - lastNotifyTime >= notifyInterval) {
            std::string value = "Hello World " + std::to_string(currentTime / 1000);
            sendStringToClient(value);
            lastNotifyTime = currentTime;
            waitingForClientResponse = true;
        }
    }
}

void BLECommunication::setDeviceConnected(bool connected) {
    deviceConnected = connected;
}

bool BLECommunication::isDeviceConnected() const {
    return deviceConnected;
}

void BLECommunication::setFirstConnect(bool first) {
    firstConnect = first;
}

bool BLECommunication::isFirstConnect() const {
    return firstConnect;
}

void BLECommunication::setWaitingForClientResponse(bool waiting) {
    waitingForClientResponse = waiting;
}

bool BLECommunication::isWaitingForClientResponse() const {
    return waitingForClientResponse;
}
