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
    MFLCDDisplay *lcd_I2C;
    uint8_t       lcd_12cRegistered = 0;

    void setupArray(uint16_t count)
    {
        if (count == 0) return;

        // ToDo: how to handle exceeding device memory!!
        if (!FitInMemory(sizeof(MFLCDDisplay) * count)) {
            cmdMessenger.sendCmd(kStatus, F("LCD does not fit in Memory!"));
            return;
        }
        lcd_I2C = new (allocateMemory(sizeof(MFLCDDisplay) * count)) MFLCDDisplay;
    }

    void Add(uint8_t address, uint8_t cols, uint8_t lines)
    {
        lcd_I2C[lcd_12cRegistered] = MFLCDDisplay();
        lcd_I2C[lcd_12cRegistered].attach(address, cols, lines);
        lcd_12cRegistered++;
#ifdef DEBUG2CMDMESSENGER
        cmdMessenger.sendCmd(kDebug, F("Added lcdDisplay"));
#endif
    }

    void Clear()
    {
        for (uint8_t i = 0; i < lcd_12cRegistered; i++) {
            lcd_I2C[i].detach();
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
        lcd_I2C[address].display(output);
        setLastCommandMillis();
    }
} // namespace

// LCDDisplay.cpp
