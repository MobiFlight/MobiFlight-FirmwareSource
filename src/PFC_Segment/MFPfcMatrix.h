//
// MFPfcMatrix.h
//
// (C) MobiFlight Project 2022
//

#pragma once

#include <Arduino.h>
#include "MFOutputShifter.h"
#include "MFOutput.h"

const static byte digitMapping[10] = {
    //  0         ,1         ,2         ,3         ,4         ,5         ,6         ,7
    0b00111111, 0b00000110, 0b01011011, 0b01001111, 0b01100110, 0b01101101, 0b01111101, 0b00000111,
    //  8         ,9
    0b01111111, 0b01101111};

class MFPfcMatrix
{
public:
    MFPfcMatrix();
    void display(byte module, char *string, byte points, uint16_t mask, bool convertPoints = false);
    void setSegment(uint8_t digit, uint8_t segment, bool state);
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
    bool             _initialized;
    MFOutputShifter *_outputShifter;
    uint8_t          buffer[10] = {
        digitMapping[1],
        digitMapping[2],
        digitMapping[3],
        digitMapping[4],
        digitMapping[5],
        digitMapping[6],
        digitMapping[7],
        digitMapping[8],
        0b101101,
        0b101101};
    MFOutput *_digits[10];
};

// MFPfcMatrix.h
