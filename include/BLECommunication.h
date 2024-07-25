#ifndef BLE_COMMUNICATION_H
#define BLE_COMMUNICATION_H

#include <NimBLEDevice.h>
#include <M5Unified.h>
#include <BLEConfig.h>
#include <BLEServerCallbacks.h>
#include <BLERxCharacteristicCallbacks.h>

class BLECommunication
{
public:
    void setTxCharacteristic(NimBLECharacteristic *characteristic);
    NimBLECharacteristic *getTxCharacteristic() const;

    BLECommunication();
    
    void sendInitialMessage();
    void sendStringToClient(const std::string &str);
    void sendCsvDataToClient(const String &str,  double impedanceData[8][8]);


    void sendCsvEndMarker();
    void setDeviceConnected(bool connected);
    bool isDeviceConnected() const;

    void setWaitingForClientResponse(bool waiting);
    bool isWaitingForClientResponse() const;

    void serialPrintBLEMacAddress();

    BLECommunication(BLECommunication &other) = delete;
    void operator=(const BLECommunication &) = delete;

    static BLECommunication *getInstance();

private:
    static BLECommunication* instance;
    bool deviceConnected;
    NimBLEServer *pServer;
    bool started;
    bool waitingForClientResponse;
    unsigned long lastNotifyTime;
    const unsigned long notifyInterval;
    NimBLECharacteristic *pTxCharacteristic;
};

#endif // BLE_COMMUNICATION_H