//
// Analog.h
//
// (C) MobiFlight Project 2022
//

#pragma once

#include <stdint.h>
namespace Analog
{
    void Add(uint8_t pin, uint8_t sensitivity);
    void Clear();
    void read();
    void readAverage();
    void OnTrigger();
}

// Analog.h