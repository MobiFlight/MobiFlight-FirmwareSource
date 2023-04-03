//
// MFPfcMatrix.cpp
//
// (C) MobiFlight Project 2022
//
#include "mobiflight.h"
#include "MFPfcMatrix.h"
#include "MFOutput.h"

MFPfcMatrix::MFPfcMatrix()
{
    _initialized = false;
}

void MFPfcMatrix::display(byte module, char *string, byte points, uint16_t mask, bool convertPoints)
{
    byte pos = 0;
    for (uint8_t i = 0; i < 10; i++) {
        if (((1 << i) & mask) == 0)
            continue;

        if (string[pos] == ' ')
            buffer[i] = 0;
        else {
            byte mapping = min((byte)string[pos] - 48, 9);
            buffer[i]    = digitMapping[mapping];
        }

        if (((1 << pos) & points) > 0)
            buffer[i] |= 0b10000000;
        pos++;
    }
}

void MFPfcMatrix::setSegment(uint8_t digit, uint8_t segment, bool state)
{
    if (state)
        buffer[digit] |= segment;
    else
        buffer[digit] &= ~segment;
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
    _digits[0] = new MFOutput(digit0);
    _digits[1] = new MFOutput(digit1);
    _digits[2] = new MFOutput(digit2);
    _digits[3] = new MFOutput(digit3);
    _digits[4] = new MFOutput(digit4);
    _digits[5] = new MFOutput(digit5);
    _digits[6] = new MFOutput(digit6);
    _digits[7] = new MFOutput(digit7);
    _digits[8] = new MFOutput(digit8);
    _digits[9] = new MFOutput(digit9);

    for (int i = 0; i < 10; i++)
        _digits[i]->set(255);

    _initialized = true;
}

void MFPfcMatrix::detach()
{
    _outputShifter->detach();
    _initialized = false;
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
    if (!_initialized) return;

    for (uint8_t i = 0; i < 10; i++) {
        if (i > 0)
            _digits[i - 1]->set(255);
        else
            _digits[9]->set(255);
        _outputShifter->setBuffer(0, buffer[i]);
        _digits[i]->set(0);
        delayMicroseconds(500);
    }
}

// MFPfcMatrix.cpp
