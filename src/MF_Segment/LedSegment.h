//
// LedSegment.h
//
// (C) MobiFlight Project 2022
//

#pragma once

namespace LedSegment
{
    uint8_t Add(int dataPin, int csPin, int clkPin, int numDevices, int brightness);
    void    Clear();
    void    PowerSave(bool state);
    void    OnInitModule();
    void    OnSetModule();
    void    OnSetModuleBrightness();
}

// LedSegment.h
