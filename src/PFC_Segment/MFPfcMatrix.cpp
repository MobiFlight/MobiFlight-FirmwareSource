//
// MFPfcMatrix.cpp
//
// (C) MobiFlight Project 2022
//
#include "mobiflight.h"
#include "MFPfcMatrix.h"
#include "MFOutput.h"

const static byte digitMapping[10] = {
    //  0         ,1         ,2         ,3         ,4         ,5         ,6         ,7
    0b01111110, 0b00110000, 0b01101101, 0b01111001, 0b00110011, 0b01011011, 0b01011111, 0b01110000,
    //  8         ,9
    0b01111111, 0b01111011};

MFPfcMatrix::MFPfcMatrix()
{
}

void MFPfcMatrix::display(byte module, char *string, byte points, uint16_t mask, bool convertPoints)
{
    byte digit = 10;
    byte pos   = 0;
    for (uint8_t i = 0; i < 10; i++) {
        digit--;
        if (((1 << digit) & mask) == 0)
            continue;
        buffer[digit] = digitMapping[atol(&string[pos])];
        //, ((1 << digit) & points));
        pos++;
    }
}

void MFPfcMatrix::displaySegment(uint8_t digit, uint8_t segment)
{
}

void MFPfcMatrix::setBrightness(byte module, byte value)
{
    // nothing todo
}

void MFPfcMatrix::attach(int dataPin, int csPin, int clkPin,
                         uint8_t digit0, uint8_t digit1, uint8_t digit2, uint8_t digit3,
                         uint8_t digit4, uint8_t digit5, uint8_t digit6, uint8_t digit7,
                         uint8_t digit8, uint8_t digit9)
{
    _outputShifter = new (allocateMemory(sizeof(MFOutputShifter))) MFOutputShifter;
    _outputShifter->attach(dataPin, csPin, clkPin, 1); // lc is our object
    _digits[0] = new MFOutput(digit1);
    _digits[1] = new MFOutput(digit1);
    _digits[2] = new MFOutput(digit2);
    _digits[3] = new MFOutput(digit3);
    _digits[4] = new MFOutput(digit4);
    _digits[5] = new MFOutput(digit5);
    _digits[6] = new MFOutput(digit6);
    _digits[7] = new MFOutput(digit7);
    _digits[8] = new MFOutput(digit8);
    _digits[9] = new MFOutput(digit9);
}

void MFPfcMatrix::detach()
{
    _outputShifter->detach();
}

void MFPfcMatrix::powerSavingMode(bool state)
{
    // nothing todo
}

void MFPfcMatrix::test()
{
    // tbd
}

void MFPfcMatrix::update()
{
    for (uint8_t i = 0; i < 10; i++) {
        _digits[(i - 1) % 10]->set(255);
        _outputShifter->setBuffer(0, buffer[i]);
        _digits[i]->set(0);
    }
}

// MFPfcMatrix.cpp
