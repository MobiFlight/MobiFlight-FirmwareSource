//
// Button.h
//
// (C) MobiFlight Project 2022
//

#pragma once

#include <stdint.h>

namespace Button
{
    uint8_t Add(uint8_t pin, char const *name = "Button");
    void    Clear();
    void    read();
    void    OnTrigger();
}

// Button.h