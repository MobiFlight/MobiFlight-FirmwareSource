//
// Analog.h
//
// (C) MobiFlight Project 2022
//

#pragma once

#include <stdint.h>
namespace Analog
{
    void Add(uint8_t pin, char const *name = "AnalogInput", uint8_t sensitivity = 3);
    void Clear();
    void read();
    void readAverage();
    void OnTrigger();
}

// Analog.h