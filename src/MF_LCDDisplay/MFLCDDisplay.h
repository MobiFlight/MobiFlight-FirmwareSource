//
// MFLCDDisplay.h
//
// (C) MobiFlight Project 2022
//

#pragma once

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include "config.h"

class MFLCDDisplay  
{
public:
    static constexpr uint8_t  getType(void) { return kTypeLcdDisplayI2C; }

    MFLCDDisplay(void);
    
    void    attach(byte address, byte cols, byte lines);
    void    detach(void);
    void    reset(uint8_t action);
    void    setval(const char *string);
    void    update(void) {};  // Stub Required for emulated polymorphism
    void    powerSave(uint8_t state);
    void    test();

private:
    
    LiquidCrystal_I2C _lcdDisplay;
    byte _address;
    byte _cols;
    byte _lines;

    void _printCentered(const char *str, uint8_t line);
};

// MFLCDDisplay.h
