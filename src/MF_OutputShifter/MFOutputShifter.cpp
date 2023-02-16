//
// MFOutputShifter.cpp
//
// (C) MobiFlight Project 2022
//

#include "MFOutputShifter.h"

MFOutputShifter::MFOutputShifter()
{
    _initialized = false;
}

void MFOutputShifter::setPin(uint8_t pin, uint8_t value, uint8_t refresh)
{
    if (!_initialized) return;

    uint8_t idx = (pin & 0xF8) >> 3;
    uint8_t msk = (0x01 << (pin & 0x07));

    if (value > 0) {
        _outputBuffer[idx] |= msk;
    } else {
        _outputBuffer[idx] &= ~msk;
    }
    if (refresh) updateShiftRegister();
}

void MFOutputShifter::setPins(char *pins, uint8_t value)
{
    if (!_initialized) return;

    char *pinTokens = strtok(pins, "|");
    while (pinTokens != 0) {
        uint8_t num = (uint8_t)atoi(pinTokens);
        setPin(num, value, 0);
        pinTokens = strtok(0, "|");
    }
    updateShiftRegister();
}

void MFOutputShifter::attach(uint8_t latchPin, uint8_t clockPin, uint8_t dataPin, uint8_t moduleCount)
{
    _initialized  = true;
#ifdef USE_FAST_IO
    _latchPin.Port = portOutputRegister(digitalPinToPort(latchPin));
    _latchPin.Mask = digitalPinToBitMask(latchPin);
    _clockPin.Port = portOutputRegister(digitalPinToPort(clockPin));
    _clockPin.Mask = digitalPinToBitMask(clockPin);
    _dataPin.Port  = portInputRegister(digitalPinToPort(dataPin));
    _dataPin.Mask  = digitalPinToBitMask(dataPin);
#else
    _latchPin    = latchPin;
    _clockPin    = clockPin;
    _dataPin     = dataPin;
#endif
    _moduleCount  = moduleCount;

    pinMode(latchPin, OUTPUT);
    pinMode(clockPin, OUTPUT);
    pinMode(dataPin, OUTPUT);

    clear();
}

void MFOutputShifter::detach()
{
    if (!_initialized) return;
    _initialized = false;
}

void MFOutputShifter::clear()
{
    // Set everything to 0
    for (uint8_t i = 0; i < _moduleCount; i++) {
        _outputBuffer[i] = 0;
    }
    updateShiftRegister();
}

void MFOutputShifter::test()
{
    for (uint8_t b = 0; b < _moduleCount * 8; b++) {
        uint8_t idx        = (b & 0xF8) >> 3;
        uint8_t msk        = (0x01 << (b & 0x07));
        _outputBuffer[idx] = msk;
        if (msk == 0x01 && idx != 0) _outputBuffer[idx - 1] = 0x00;
        updateShiftRegister();
        delay(50);

        _outputBuffer[_moduleCount - 1] = 0;
        updateShiftRegister();
    }
}

void MFOutputShifter::updateShiftRegister()
{
    DIGITALWRITE(_latchPin, LOW);
    for (uint8_t i = _moduleCount; i > 0; i--) {
        for (int8_t j = 7; j >= 0; j--) {
            DIGITALWRITE(_dataPin, (_outputBuffer[i - 1] & (1 << (j))));
            DIGITALWRITE(_clockPin, HIGH);
            DIGITALWRITE(_clockPin, LOW);
        }
    }
    DIGITALWRITE(_latchPin, HIGH);
}

// MFOutputShifter.cpp
