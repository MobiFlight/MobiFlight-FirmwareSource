//
// MFOutput.cpp
//
// (C) MobiFlight Project 2022
//

#include "MFOutput.h"
#include "mobiflight.h"

MFOutput::MFOutput(void)
{
    _pin = 0xFF;
    _value = false;
}

void MFOutput::attach(uint8_t pin)
{
    _pin = pin;
    pinMode(_pin, OUTPUT);
    setval(false);
}

void MFOutput::detach(void)
{
    pinMode(_pin, INPUT);
    _pin = 0xFF;
}

void MFOutput::reset(uint8_t action)
{
    if(ONRESET_DEFAULT == action) {
        setval(0);
    }
}

void MFOutput::setval(uint8_t value)
{
    _value = value;
    analogWrite(_pin, _value);
}

void MFOutput::powerSave(uint8_t state)
{
    if (state) {
        analogWrite(_pin, 0);
    } else {
        setval(_value);
    }
}

// MFoutput.cpp