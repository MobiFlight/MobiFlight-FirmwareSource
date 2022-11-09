//
// Encoder.h
//
// (C) MobiFlight Project 2022
//

#pragma once
#include <stdint.h>

namespace Encoder
{
    void Add(uint8_t pin1 = 1, uint8_t pin2 = 2, uint8_t encoder_type = 0);
    void Clear();
    void read();
}

// Encoder.h
