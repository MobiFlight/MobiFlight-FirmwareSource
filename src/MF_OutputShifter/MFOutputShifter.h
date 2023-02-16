//
// MFOutputShifter.h
//
// (C) MobiFlight Project 2022
//

#pragma once

#include <Arduino.h>
#ifdef USE_FAST_IO
#include "MFFastIO.h"
#endif

// Maximum number of shifters allowed on an individual chain. While this is currently set to 4
// there is no technical limit in the code for how many can be chained. It is constrained only
// by available memory (one byte required per chip) and the time it takes to push all the bits out.
#define MAX_CHAINED_OUTPUT_SHIFTERS 4

class MFOutputShifter
{
public:
    MFOutputShifter();
    void setPin(uint8_t pin, uint8_t value, uint8_t refresh = 1);
    void setPins(char *pins, uint8_t value);
    void attach(uint8_t latchPin, uint8_t clockPin, uint8_t dataPin, uint8_t moduleCount);
    void detach();
    void clear();
    void test();
    void updateShiftRegister();

private:
#ifdef USE_FAST_IO
    volatile FASTIO_Port_t *_latchPinPort;
    FASTIO_Port_t           _latchPinMask;
    volatile FASTIO_Port_t *_clockPinPort;
    FASTIO_Port_t           _clockPinMask;
    volatile FASTIO_Port_t *_dataPinPort;
    FASTIO_Port_t           _dataPinMask;
#else
    uint8_t _latchPin;
    uint8_t _clockPin;
    uint8_t _dataPin;
#endif
    uint8_t _moduleCount; // Number of 8 bit modules in series. For a shift register with 16 bit one needs to select 2 modules a 8......
    uint8_t _outputBuffer[MAX_CHAINED_OUTPUT_SHIFTERS];
    bool    _initialized = false;
};

// MFOutputShifter.h
