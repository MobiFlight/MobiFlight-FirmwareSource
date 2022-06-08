//
// LCDDisplay.h
//
// (C) MobiFlight Project 2022
//

#pragma once

#include <stdint.h>

namespace LCDDisplay
{
    DECLARE_VT_STUBS;   // see IODevice.h
   
    void Add(uint8_t address = 0x24, uint8_t cols = 16, uint8_t lines = 2);
    void OnSet(void);

}

// LCDDisplay.h
