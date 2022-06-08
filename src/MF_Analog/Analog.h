//
// Analog.h
//
// (C) MobiFlight Project 2022
//

#pragma once

#include <stdint.h>

namespace Analog
{
    DECLARE_VT_STUBS;   // see IODevice.h
 
    void Add(uint8_t pin, uint8_t sensitivity = 3, char const *name = "AnalogInput");
    void UpdateAverage(void);
}

// Analog.h
