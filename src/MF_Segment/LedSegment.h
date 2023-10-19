//
// LedSegment.h
//
// (C) MobiFlight Project 2022
//

#pragma once

namespace LedSegment
{
    void Add(uint8_t dataPin, uint8_t csPin, uint8_t clkPin, uint8_t numDevices, uint8_t brightness);
    void Clear();
    void PowerSave(bool state);
    void OnInitModule();
    void OnSetModule();
    void OnSetModuleBrightness();
}

// LedSegment.h
