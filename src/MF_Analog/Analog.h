//
// Analog.h
//
// (C) MobiFlight Project 2022
//

#pragma once

#include <stdint.h>
namespace Analog
{
    uint8_t Add(uint8_t pin, char const *name = "AnalogInput", uint8_t sensitivity = 3);
    void    Clear();
    void    read();
    int16_t getActualValue(uint8_t channel);
    void    readAverage();
    void    OnTrigger();
}

// Analog.h