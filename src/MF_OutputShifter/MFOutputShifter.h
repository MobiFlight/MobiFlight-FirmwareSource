//
// MFOutputShifter.h
//
// (C) MobiFlight Project 2022
//

#pragma once

#include <Arduino.h>
#include "config.h"


// Maximum number of shifters allowed on an individual chain. While this is currently set to 4
// there is no technical limit in the code for how many can be chained. It is constrained only
// by available memory (one byte required per chip) and the time it takes to push all the bits out.
#define MAX_CHAINED_OUTPUT_SHIFTERS 4


class MFOutputShifter  {
public:
    static constexpr uint8_t  getType(void) { return kTypeOutShiftReg; }

    MFOutputShifter(void);

    void    attach(uint8_t latchPin, uint8_t clockPin, uint8_t dataPin, uint8_t moduleCount);
    void    detach(void);
    void    reset(uint8_t action);
    void    update(void);
    void    powerSave(uint8_t state);

    void    setPin(uint8_t pin, uint8_t value, uint8_t refresh = 1);
    void    setPins(char* pins, uint8_t value);
    void    clear(void);
    void    test(void);
    
private:
    uint8_t _latchPin;	    // Latch pin
    uint8_t _clockPin;	    // Clock pin
    uint8_t _dataPin;	    // Data/SI pin
    uint8_t _moduleCount;   // Number of 8 bit modules in series. For a shift register with 16 bit one needs to select 2 modules a 8......
    uint8_t _outputBuffer[MAX_CHAINED_OUTPUT_SHIFTERS];
};

// MFOutputShifter.h
