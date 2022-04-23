//
// Encoder.h
//
// (C) MobiFlight Project 2022
//

#pragma once

#include <stdint.h>

namespace Encoder
{
    DECLARE_VT_STUBS;   // see IODevice.h

    void Add(uint8_t pin1, uint8_t pin2, uint8_t encoder_type = 0, char const *name = "Encoder");
}

// Encoder.h
