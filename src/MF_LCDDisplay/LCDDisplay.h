//
// LCDDisplay.h
//
// (C) MobiFlight Project 2022
//

#pragma once
#include <stdint.h>

namespace LCDDisplay
{
    uint8_t Add(uint8_t address = 0x24, uint8_t cols = 16, uint8_t lines = 2);
    void    Clear();
    void    OnSet();
}

// LCDDisplay.h
