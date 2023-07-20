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
    MFCustomDevice(uint8_t pin1, uint8_t pin2, uint8_t pin3, uint8_t pin4, uint8_t pin5, uint8_t pin6, char *customName, char *configuration);
    void detach();
    void update();
    void set(uint8_t messageID, char *setPoint);

private:
    bool            _initialized = false;
    MyCustomDevice *_mydevice;
};
