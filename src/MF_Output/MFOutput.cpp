//
// Output.cpp
//
// (C) MobiFlight Project 2022
//

#include "MFOutput.h"

MFOutput::MFOutput() {}

void MFOutput::init(uint8_t pin)
{
    _pin   = pin;
    _value = false;
    pinMode(_pin, OUTPUT);
    set(_value);
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
