#pragma once

#include "Arduino.h"

/* **********************************************************************************
    These defines are required to differ between multiple classes within
    MFCustomDevice.cpp (see example in this file)
    If you have only one class, these defines are not required as you have
    not to differ.
********************************************************************************** */
#define MY_CUSTOM_TYPE_1 "MyCustomType1"
// This define should be in the second class, it's only here to show how to handle it
#define MY_CUSTOM_TYPE_2 "MyCustomType2"
class MyCustomClass
{
public:
    MyCustomClass(uint8_t Pin1, uint8_t Pin2);
    void begin();
    void attach(uint16_t Pin3, char *init);
    void detach();
    void set(int8_t messageID, char *setPoint);
    void update();

private:
    bool    _initialised;
    uint8_t _pin1, _pin2, _pin3;
};