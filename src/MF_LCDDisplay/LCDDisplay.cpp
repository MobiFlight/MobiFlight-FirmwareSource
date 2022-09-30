//
// LCDDisplay.cpp
//
// (C) MobiFlight Project 2022
//

#include <Arduino.h>
#include "mobiflight.h"
#include "MFLCDDisplay.h"

namespace LCDDisplay
{
    DEFINE_VT_STUBS(MFLCDDisplay); // see IODevice.h

    void Add(uint8_t I2Caddress, uint8_t cols, uint8_t lines)
    {
        MFLCDDisplay *MFL;
        Stowage.AddItem(&MFL);

        if (MFL) {
            MFL->attach(I2Caddress, cols, lines);
#ifdef DEBUG2CMDMESSENGER
            cmdMessenger.sendCmd(kDebug, F("Added LCD display"));
        } else {
            cmdMessenger.sendCmd(kDebug, F("LCD display: Memory full"));
#endif
        }
    }

    void OnSet(void)
    {
        MFLCDDisplay *MFL;
        int           nLCD   = cmdMessenger.readInt16Arg();
        char         *output = cmdMessenger.readStringArg();
        cmdMessenger.unescape(output);
        // MFS = static_cast<MFSegments *>(Stowage.getNth(module, kTypeLedSegment));
        MFL = (MFLCDDisplay *)(Stowage.getNth((uint8_t)nLCD, kTypeLcdDisplayI2C));
        if (MFL) {
            MFL->setval(output);
            setLastCommandMillis();
        }
    }
} // namespace

// LCDDisplay.cpp
