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
    MFOutput();
    void attach(uint8_t pin);
    void set(uint8_t value);
    void powerSavingMode(bool state);

private:
    uint8_t _pin;
    uint8_t _value;
};

// MFOutput.h
