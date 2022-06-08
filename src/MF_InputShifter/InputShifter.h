//
// InputShifter.h
//
// (C) MobiFlight Project 2022
//

#pragma once

#include <stdint.h>
namespace InputShifter
{
    DECLARE_VT_STUBS;   // see IODevice.h
 
    void Add(uint8_t latchPin, uint8_t clockPin, uint8_t dataPin, uint8_t modules = 1, char const *name = "InputShifter");
}

// InputShifter.h
