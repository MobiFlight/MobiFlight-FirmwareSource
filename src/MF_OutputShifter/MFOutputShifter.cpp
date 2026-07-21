//
// MFOutputShifter.cpp
//
// (C) MobiFlight Project 2022
//

#include "MFOutputShifter.h"
#include "MFShiftData.h"
#include "allocateMem.h"

MFOutputShifter::MFOutputShifter()
{
    _initialized = false;
}

void MFOutputShifter::setPins(uint8_t *pins, uint8_t value)
{
    if (!_initialized) return;

    for (uint8_t i = 0; i < _moduleCount; i++) {
        if (value != MF_LOW)
            _lastState[i] |= pins[i];
        else
            _lastState[i] &= ~pins[i];
    }
    update();
}

bool MFOutputShifter::attach(uint8_t latchPin, uint8_t clockPin, uint8_t dataPin, uint8_t moduleCount)
{
    _initialized = true;
    _latchPin    = latchPin;
    _clockPin    = clockPin;
    _dataPin     = dataPin;
    _moduleCount = moduleCount;

    pinMode(_latchPin, OUTPUT);
    pinMode(_clockPin, OUTPUT);
    pinMode(_dataPin, OUTPUT);

    _lastState = static_cast<uint8_t*>(MF_ALLOC_BYTES(_moduleCount));
    if (!_lastState) return false;

    clear();
    return true;
}

void MFOutputShifter::detach()
{
    _initialized = false;
}

void MFOutputShifter::clear()
{
    for (uint8_t i = 0; i < _moduleCount; i++) {
        _lastState[i] = 0xFF * MF_LOW;
    }
    update();
}

void MFOutputShifter::update()
{
    digitalWrite(_latchPin, LOW);
    for (uint8_t i = _moduleCount; i > 0; i--) {
        shiftOutData(_dataPin, _clockPin, MSBFIRST, _lastState[i - 1]); // LSBFIRST, MSBFIRST,
    }
    digitalWrite(_latchPin, HIGH);
}

void MFOutputShifter::powerSavingMode(bool state)
{
    if (state) {
        digitalWrite(_latchPin, LOW);
        for (uint8_t i = _moduleCount; i > 0; i--) {
            shiftOutData(_dataPin, _clockPin, MSBFIRST, 0xFF * MF_LOW);
        }
        digitalWrite(_latchPin, HIGH);
    } else {
        update();
    }
}

// MFOutputShifter.cpp
