#pragma once

#include <Arduino.h>
#include "MyCustomDevice.h"

enum {
    MY_CUSTOM_DEVICE_1 = 0,
    MY_CUSTOM_DEVICE_2
};
class MFCustomDevice
{
public:
    MFCustomDevice(uint16_t adrPin, uint16_t adrType, uint16_t adrConfig);
    void detach();
    void update();
    void set(uint8_t messageID, char *setPoint);

private:
    bool            getStringFromEEPROM(uint16_t addreeprom, char *buffer);
    bool            _initialized = false;
    MyCustomDevice *_mydevice;
    uint8_t         _pin1, _pin2, _pin3;
    uint8_t         _customType;
};
