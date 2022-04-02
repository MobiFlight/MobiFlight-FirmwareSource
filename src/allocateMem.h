//
// allocatemem.h
//
// (C) MobiFlight Project 2022
//

#pragma once

#include <new>

extern char deviceBuffer[];

char       *allocateMemory(uint8_t size);
void        ClearMemory();
uint16_t    GetAvailableMemory();
bool        FitInMemory(uint8_t size);

// allocatemem.h
