//
// Servos.h
//
// (C) MobiFlight Project 2022
//

#pragma once

#include <stdint.h>

namespace Servos
{
    DECLARE_VT_STUBS;   // see IODevice.h

    void Add(uint8_t pin);
    void OnSet(void);
}

// Servos.h
