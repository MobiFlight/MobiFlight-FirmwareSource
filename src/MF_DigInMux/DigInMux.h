//
// DigInMux.h
//
// (C) MobiFlight Project 2022
//

#pragma once
#include <stdint.h>
#include "MFDigInMux.h"

namespace DigInMux
{
    void Add(uint8_t dataPin, uint8_t nRegs, char const *name = "DigInMux");
    void Clear();
    void read();
    void OnTrigger();
}

// DigInMux.h