//
// MFOutput.h
//
// (C) MobiFlight Project 2022
//

#pragma once

#include <Arduino.h>

class MFOutput
{
public:
    MFOutput(uint8_t pin = 1);
    void set(uint8_t value);
    void powerSavingMode(bool state);

private:
    uint8_t _pin;
    uint8_t _value;
};

// MFOutput.h
