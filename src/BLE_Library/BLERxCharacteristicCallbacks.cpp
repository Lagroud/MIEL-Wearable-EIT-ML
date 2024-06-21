#include "BLERxCharacteristicCallbacks.h"
#include "BLECommunication.h"

void BLERxCharacteristicCallbacks::onWrite(NimBLECharacteristic* pCharacteristic) {
    if (pCharacteristic) {
        std::string rxValue = pCharacteristic->getValue();
        M5.Lcd.println("Received from client: " + String(rxValue.c_str()));
        BLECommunication::getInstance()->setWaitingForClientResponse(false);
    } else {
        Serial.println("Error: Null characteristic in onWrite callback");
    }
}