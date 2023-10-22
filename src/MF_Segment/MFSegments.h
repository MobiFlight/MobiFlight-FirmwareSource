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
    void display(uint8_t module, char *string, uint8_t points, uint8_t mask, bool convertPoints = false);
    void attach(uint8_t type, uint8_t dataPin, uint8_t csPin, uint8_t clkPin, uint8_t moduleCount, uint8_t brightness);
    void detach();
    void test();
    void powerSavingMode(bool state);
    void setBrightness(uint8_t module, uint8_t value);

private:
    LedControl _ledControl;
    uint8_t       _moduleCount;
};

// MFSegments.h
