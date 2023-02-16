//
// MFFastIO.h
//
// (C) MobiFlight Project 2022
//

#pragma once

#include <Arduino.h>

#ifdef ARDUINO_ARCH_RP2040
typedef uint32_t FASTIO_Port_t;
#else
typedef uint8_t FASTIO_Port_t;
#endif

inline void digitalWriteFast(volatile FASTIO_Port_t *PinPort, FASTIO_Port_t PinMask, uint8_t value)
{
    if (value)
        *PinPort |= PinMask;
    else
        *PinPort &= ~PinMask;
}

inline uint8_t digitalReadFast(volatile FASTIO_Port_t *PinPort, FASTIO_Port_t PinMask)
{
    if (*PinPort & PinMask) return HIGH;
    return LOW;
}
