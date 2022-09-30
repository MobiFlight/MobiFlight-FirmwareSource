//
// InputShifter.cpp
//
// (C) MobiFlight Project 2022
//

#include <Arduino.h>
#include "mobiflight.h"
#include "MFInputShifter.h"

namespace InputShifter
{

    DEFINE_VT_STUBS(MFInputShifter); // see IODevice.h

    void OnChange(uint8_t eventId, uint8_t pin, const char *name)
    {
        cmdMessenger.sendCmdStart(kInputShifterChange);
        cmdMessenger.sendCmdArg(name);
        cmdMessenger.sendCmdArg(pin);
        cmdMessenger.sendCmdArg(eventId);
        cmdMessenger.sendCmdEnd();
    };

    void Add(uint8_t latchPin, uint8_t clockPin, uint8_t dataPin, uint8_t nModules, char const *name)
    {
        MFInputShifter *MFI;

        Stowage.AddItem(&MFI);

        if (MFI) {
            MFI->attach(latchPin, clockPin, dataPin, nModules, name);
            MFInputShifter::attachHandler(OnChange);
#ifdef DEBUG2CMDMESSENGER
            cmdMessenger.sendCmd(kDebug, F("Added InShiftReg"));
        } else {
            cmdMessenger.sendCmd(kDebug, F("InShiftReg: Memory full"));
#endif
        }
    }
} // namespace

// InputShifter.cpp
