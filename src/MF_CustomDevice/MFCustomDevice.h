#pragma once

#include <Arduino.h>
#include "MyCustomDevice.h"

extern "C" {
// callback functions
typedef void (*CustomDeviceEvent)(uint8_t, const char *);
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
};
