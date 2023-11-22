//
// Output.cpp
//
// (C) MobiFlight Project 2022
//

#include "MFOutput.h"

MFOutput::MFOutput()
{
    _value = false;
}

void MFOutput::attach(uint8_t pin)
{
    _pin   = pin;
#if !defined(ARDUINO_ARCH_RP2040)
    pinMode(_pin, OUTPUT);
    set(_value);
#endif
}

void MFOutput::set(uint8_t value)
{
    _value = value;
    analogWrite(_pin, _value);
}

void MFOutput::powerSavingMode(bool state)
{
    if (state)
        set(0);
    else
        set(_value);
}

// Output.cpp
