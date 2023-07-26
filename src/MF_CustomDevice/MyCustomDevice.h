#pragma once

#include "Arduino.h"
#include "MyCustomDevice.h"

class MyCustomDevice
{
public:
    MyCustomDevice();
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