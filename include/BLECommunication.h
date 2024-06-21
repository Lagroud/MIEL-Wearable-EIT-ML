#ifndef BLE_COMMUNICATION_H
#define BLE_COMMUNICATION_H

#include <NimBLEDevice.h>
#include <M5Unified.h>

class BLECommunication
{
public:
    void setTxCharacteristic(NimBLECharacteristic *characteristic);
    NimBLECharacteristic *getTxCharacteristic() const;

    BLECommunication();
    
    void handleBLEConnection();
    void sendInitialMessage();
    void sendStringToClient(const std::string &str);

    void setDeviceConnected(bool connected);
    bool isDeviceConnected() const;
    void setFirstConnect(bool first);
    bool isFirstConnect() const;
    void setWaitingForClientResponse(bool waiting);
    bool isWaitingForClientResponse() const;

    BLECommunication(BLECommunication &other) = delete;
    void operator=(const BLECommunication &) = delete;

    static BLECommunication *getInstance();

private:
    static BLECommunication* instance;
    bool deviceConnected;
    bool firstConnect;
    bool waitingForClientResponse;
    unsigned long lastNotifyTime;
    const unsigned long notifyInterval;
    NimBLECharacteristic *pTxCharacteristic;
};

#endif // BLE_COMMUNICATION_H