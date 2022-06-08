//
// Stepper.h
//
// (C) MobiFlight Project 2022
//

#pragma once
#include <stdint.h>

namespace Stepper
{
    DECLARE_VT_STUBS;   // see IODevice.h

    void Add(uint8_t pin1, uint8_t pin2, uint8_t pin3, uint8_t pin4, uint8_t zeroPin);
    void OnSet(void);
    void OnReset(void);
    void OnSetZero(void);
}

// Stepper.h
