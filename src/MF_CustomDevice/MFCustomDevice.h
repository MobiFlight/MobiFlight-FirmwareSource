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
    MFCustomDevice(char *customPins, char *customType, char *configuration);
    void detach();
    void update();
    void set(uint8_t messageID, char *setPoint);

private:
    bool            _initialized = false;
    MyCustomDevice *_mydevice;
    uint8_t _pin1, _pin2, _pin3;
};
