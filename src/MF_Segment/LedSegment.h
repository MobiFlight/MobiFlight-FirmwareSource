#pragma once

namespace LedSegment
{
void Add(int dataPin, int csPin, int clkPin, int numDevices, int brightness);
void Clear();
void PowerSave(bool state);
void OnInitModule();
void OnSetModule();
void OnSetModuleBrightness();
}
