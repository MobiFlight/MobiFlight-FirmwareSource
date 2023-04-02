//
// MFPfcMatrix.h
//
// (C) MobiFlight Project 2022
//

#pragma once

#include <Arduino.h>
#include "MFOutputShifter.h"
#include "MFOutput.h"

class MFPfcMatrix
{
public:
    MFPfcMatrix();
    void display(byte module, char *string, byte points, uint16_t mask, bool convertPoints = false);
    void displaySegment(uint8_t digit, uint8_t segment);
    void attach(int dataPin, int csPin, int clkPin,
                uint8_t digit0, uint8_t digit1, uint8_t digit2, uint8_t digit3,
                uint8_t digit4, uint8_t digit5, uint8_t digit6, uint8_t digit7,
                uint8_t digit8, uint8_t digit9);
    void detach();
    void test();
    void update();
    void powerSavingMode(bool state);
    void setBrightness(byte module, byte value);

private:
    MFOutputShifter *_outputShifter;
    uint8_t          buffer[10];
    MFOutput        *_digits[10];
};

// MFPfcMatrix.h
