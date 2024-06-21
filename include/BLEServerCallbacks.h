#ifndef BLE_SERVER_CALLBACKS_H
#define BLE_SERVER_CALLBACKS_H

#include <NimBLEDevice.h>
#include <M5Unified.h>

class MyBLEServerCallbacks : public NimBLEServerCallbacks {
    void onConnect(NimBLEServer* pServer) override;
    void onDisconnect(NimBLEServer* pServer) override;
};

#endif // BLE_SERVER_CALLBACKS_H
