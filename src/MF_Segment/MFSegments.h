//
// MFSegments.h
//
// (C) MobiFlight Project 2022
//

#pragma once

#include <Arduino.h>
#include <LedControl.h>

class MFSegments
{
public:
    MFSegments();
    void display(byte module, char *string, byte points, byte mask, bool convertPoints = false);
    void attach(int dataPin, int csPin, int clkPin, byte moduleCount, byte brightness);
    void detach();
    void test();
    void powerSavingMode(bool state);
    void setBrightness(byte module, byte value);

private:
    LedControl _ledControl;
    byte       _moduleCount;
};

// MFSegments.h
