//
// Output.h
//
// (C) MobiFlight Project 2022
//

#pragma once
#include <stdint.h>

namespace Output
{
    uint8_t Add(uint8_t pin = 1);
    void    Clear();
    void    OnSet();
    void    PowerSave(bool state);
}

// Output.h
