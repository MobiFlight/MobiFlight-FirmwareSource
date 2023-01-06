//
// allocatemem.h
//
// (C) MobiFlight Project 2022
//

#pragma once

#include <new>

#if defined (ARDUINO_ARCH_AVR)
uint8_t     *allocateMemory(uint8_t size);
#else
std::size_t    *allocateMemory(uint8_t size);
#endif

void        ClearMemory();
uint16_t    GetAvailableMemory();
bool        FitInMemory(uint8_t size);

// allocatemem.h
