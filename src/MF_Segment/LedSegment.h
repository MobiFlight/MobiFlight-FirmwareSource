//
// LedSegment.h
//
// (C) MobiFlight Project 2022
//

#pragma once

#include <stdint.h>

namespace LedSegment
{
    DECLARE_VT_STUBS;   // see IODevice.h

    void Add(uint8_t dataPin, uint8_t csPin, uint8_t clkPin, uint8_t numDevices, uint8_t brightness);
    void OnInit(void);
    void OnSet(void);
    void OnSetBrightness(void);
}

// LedSegment.h
