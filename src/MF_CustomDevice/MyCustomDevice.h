#pragma once

#include "Arduino.h"
#include "MyCustomDevice.h"

/* **********************************************************************************
    This define will be reported back to the UI
    If a custom devive will be added within the UI, only these devices
    will be listed which match this definition.
********************************************************************************** */
#define MY_CUSTOM_TYPE_1  "MyCustomType1"
#define MY_CUSTOM_TYPE_2  "MyCustomType2"
class MyCustomDevice
{
public:
    MyCustomDevice(uint8_t Pin1, uint8_t Pin2);
    void begin();
    void attach(uint16_t Pin3, char *init);
    void detach();
    void set(uint8_t messageID, char *setPoint);

private:
    bool     _initialised;
    uint8_t  _pin1;
    uint8_t  _pin2;
    uint16_t _pin3;
};