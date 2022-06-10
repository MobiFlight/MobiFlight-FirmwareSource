//
// LCDDisplay.cpp
//
// (C) MobiFlight Project 2022
//

#include "mobiflight.h"
#include "MFLCDDisplay.h"
#include "LCDDisplay.h"

namespace LCDDisplay
{
    MFLCDDisplay *lcd_I2C[MAX_MFLCD_I2C];
    uint8_t       lcd_12cRegistered = 0;

    void Add(uint8_t address, uint8_t cols, uint8_t lines)
    {
        if (lcd_12cRegistered == MAX_MFLCD_I2C)
            return;

        if (!FitInMemory(sizeof(MFLCDDisplay))) {
            // Error Message to Connector
            cmdMessenger.sendCmd(kStatus, F("LCD does not fit in Memory!"));
            return;
        }
        lcd_I2C[lcd_12cRegistered] = new (allocateMemory(sizeof(MFLCDDisplay))) MFLCDDisplay;
        lcd_I2C[lcd_12cRegistered]->attach(address, cols, lines);
        lcd_12cRegistered++;
#ifdef DEBUG2CMDMESSENGER
        cmdMessenger.sendCmd(kDebug, F("Added lcdDisplay"));
#endif
    }

    void Clear()
    {
        for (uint8_t i = 0; i < lcd_12cRegistered; i++) {
            lcd_I2C[i]->detach();
        }
        lcd_12cRegistered = 0;
#ifdef DEBUG2CMDMESSENGER
        cmdMessenger.sendCmd(kDebug, F("Cleared lcdDisplays"));
#endif
    }

    void OnSet()
    {
        int   address = cmdMessenger.readInt16Arg();
        char *output  = cmdMessenger.readStringArg();
        cmdMessenger.unescape(output);
        lcd_I2C[address]->display(output);
        setLastCommandMillis();
    }
} // namespace

// LCDDisplay.cpp
