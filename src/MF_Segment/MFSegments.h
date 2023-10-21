//
// MFSegments.h
//
// (C) MobiFlight Project 2022
//

#pragma once

#include <Arduino.h>
#include <LedControl_dual.h>

class MFSegments
{
public:
    MFSegments();
    void display(byte module, char *string, byte points, byte mask, bool convertPoints = false);
    void attach(byte type, int dataPin, int csPin, int clkPin, byte moduleCount, byte brightness);
    void detach();
    void test();
    void powerSavingMode(bool state);
    void setBrightness(uint8_t module, uint8_t value);

private:
    LedControl _ledControl;
    byte       _moduleCount;
};

// MFSegments.h
