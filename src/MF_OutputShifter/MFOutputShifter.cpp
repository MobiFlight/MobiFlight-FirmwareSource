//
// MFOutputShifter.cpp
//
// (C) MobiFlight Project 2022
//

#include "MFOutputShifter.h"
#include "mobiflight.h"

MFOutputShifter::MFOutputShifter()
{
    _moduleCount = 0;
}

void MFOutputShifter::attach(uint8_t latchPin, uint8_t clockPin, uint8_t dataPin, uint8_t moduleCount)
{
    _latchPin = latchPin;
    _clockPin = clockPin;
    _dataPin = dataPin;
    _moduleCount = moduleCount;

    pinMode(_latchPin, OUTPUT);
    pinMode(_clockPin, OUTPUT);
    pinMode(_dataPin, OUTPUT);

    clear();
}

void MFOutputShifter::detach(void)
{
    if (_moduleCount)
        clear();
    _moduleCount = 0;
}

void MFOutputShifter::reset(uint8_t action)
{
    if (ONRESET_DEFAULT == action)
        clear();
}

void MFOutputShifter::update(void)
{
    digitalWrite(_latchPin, LOW);
    for (uint8_t i = _moduleCount; i > 0; i--) {
        shiftOut(_dataPin, _clockPin, MSBFIRST, _outputBuffer[i - 1]); // LSBFIRST, MSBFIRST,
    }
    digitalWrite(_latchPin, HIGH);
}

void MFOutputShifter::setPin(uint8_t pin, uint8_t value, uint8_t refresh)
{
    if (_moduleCount) {
        uint8_t idx = (pin & 0xF8) >> 3;
        uint8_t msk = (0x01 << (pin & 0x07));
        if(idx < _moduleCount) return;
        if (value > 0) {
            _outputBuffer[idx] |= msk;
        } else {
            _outputBuffer[idx] &= ~msk;
        }
        if (refresh) {
            update();
        }
    }
}

void MFOutputShifter::setPins(char *pins, uint8_t value)
{
    if (_moduleCount == 0) return;
    char *pinTokens = strtok(pins, "|");
    while (pinTokens != 0) {
        uint8_t num = (uint8_t)atoi(pinTokens);
        setPin(num, value, 0);
        pinTokens = strtok(0, "|");
    }
    update();
}

void MFOutputShifter::clear(void)
{
    if (_moduleCount == 0) return;
    for (uint8_t i = 0; i < _moduleCount; i++) {
        _outputBuffer[i] = 0;
    }
    update();
}

void MFOutputShifter::test(void)
{
    for (uint8_t b = 0; b < _moduleCount * 8; b++) {
        uint8_t idx = (b & 0xF8) >> 3;
        uint8_t msk = (0x01 << (b & 0x07));
        _outputBuffer[idx] = msk;
        if (msk == 0x01 && idx != 0) {
            _outputBuffer[idx - 1] = 0x00;
        }
        update();
        delay(50);

        _outputBuffer[_moduleCount - 1] = 0;
        update();
    }
}

void MFOutputShifter::powerSave(uint8_t state)
{
    clear();
}

// MFOutputShifter.cpp