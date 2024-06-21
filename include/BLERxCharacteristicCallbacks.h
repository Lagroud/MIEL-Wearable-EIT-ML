#ifndef BLE_RX_CHARACTERISTIC_CALLBACKS_H
#define BLE_RX_CHARACTERISTIC_CALLBACKS_H

#include <NimBLEDevice.h>
#include <M5Unified.h>

class BLERxCharacteristicCallbacks : public NimBLECharacteristicCallbacks {
    void onWrite(NimBLECharacteristic* pCharacteristic) override;
};

#endif // BLE_RX_CHARACTERISTIC_CALLBACKS_H
