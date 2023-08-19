//
// Button.h
//
// (C) MobiFlight Project 2022
//

#pragma once

#include <stdint.h>

namespace Button
{
    void Add(uint8_t pin, char const *name = "Button");
    void setupArray(uint16_t count);
    void Clear();
    void read();
    void OnTrigger();
}

// Button.h