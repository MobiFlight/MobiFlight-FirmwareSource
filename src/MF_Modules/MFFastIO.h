//
// MFFastIO.h
//
// (C) MobiFlight Project 2022
//

#pragma once

#include <Arduino.h>

inline void digitalWriteFast(volatile uint8_t *PinPort, uint8_t PinMask, uint8_t value)
{
    if (value)
        *PinPort |= PinMask;
    else
        *PinPort &= ~PinMask;
}

inline uint8_t digitalReadFast(volatile uint8_t *PinPort, uint8_t PinMask)
{
    if (*PinPort & PinMask) return HIGH;
    return LOW;
}
