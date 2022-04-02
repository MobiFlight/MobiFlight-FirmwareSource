//
// MFSegments.cpp
//
// (C) MobiFlight Project 2022
//

#include "MFSegments.h"

MFSegments::MFSegments()
{
    _moduleCount = 0;
}

void MFSegments::display(byte module, char *string, byte points, byte mask, bool convertPoints)
{
    if (_moduleCount == 0)
        return;
    byte digit = 8;
    byte pos   = 0;
    for (uint8_t i = 0; i < 8; i++) {
        digit--;
        if (((1 << digit) & mask) == 0)
            continue;
        _ledControl.setChar(module, digit, string[pos], ((1 << digit) & points));
        pos++;
    }
}

void MFSegments::setBrightness(byte module, byte value)
{
    if (_moduleCount == 0)
        return;
    if (module < _moduleCount) {
        if (value) {
            _ledControl.setIntensity(module, value - 1);
            _ledControl.shutdown(module, false);
        } else {
            _ledControl.shutdown(module, true);
        }
    }
}

void MFSegments::attach(int dataPin, int csPin, int clkPin, byte moduleCount, byte brightness)
{
    _ledControl.begin(dataPin, clkPin, csPin, moduleCount);
    _moduleCount = moduleCount;
    for (uint8_t i = 0; i < _moduleCount; ++i) {
        setBrightness(i, brightness);
        _ledControl.shutdown(i, false);
        _ledControl.clearDisplay(i);
    }
}

void MFSegments::detach()
{
    _moduleCount = 0;
}

void MFSegments::powerSavingMode(bool state)
{
    for (byte i = 0; i != _moduleCount; ++i) {
        _ledControl.shutdown(i, state);
    }
}

void MFSegments::test()
{
    if (_moduleCount == 0)
        return;
    byte _delay = 10;
    byte module = 0;
    byte digit  = 0;

    for (digit = 0; digit < 8; digit++) {
        for (module = 0; module != _moduleCount; ++module) {
            _ledControl.setDigit(module, digit, 8, 1);
        }
        delay(_delay);
    }

    for (digit = 0; digit < 8; digit++) {
        for (module = 0; module != _moduleCount; ++module) {
            _ledControl.setChar(module, 7 - digit, '-', false);
        }
        delay(_delay);
    }

    for (digit = 0; digit < 8; digit++) {
        for (module = 0; module != _moduleCount; ++module) {
            _ledControl.setChar(module, 7 - digit, ' ', false);
        }
        delay(_delay);
    }
}

// MFSegments.cpp
