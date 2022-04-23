//
// Button.h
//
// (C) MobiFlight Project 2022
//

#pragma once

#include <stdint.h>
namespace Button
{
    DECLARE_VT_STUBS;   // see IODevice.h

    void Add(uint8_t pin, char const *name = "Button");
}

// Button.h
