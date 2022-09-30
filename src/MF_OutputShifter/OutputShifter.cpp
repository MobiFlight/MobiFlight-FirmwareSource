//
// OutputShifter.cpp
//
// (C) MobiFlight Project 2022
//

#include <Arduino.h>
#include "mobiflight.h"
#include "MFOutputShifter.h"

namespace OutputShifter
{
    DEFINE_VT_STUBS(MFOutputShifter); // see IODevice.h

    void Add(uint8_t latchPin, uint8_t clockPin, uint8_t dataPin, uint8_t modules)
    {
        MFOutputShifter *MFS;

        Stowage.AddItem(&MFS);

        if (MFS) {
            MFS->attach(latchPin, clockPin, dataPin, modules);
#ifdef DEBUG2CMDMESSENGER
            cmdMessenger.sendCmd(kDebug, F("Added OutputShifter"));
        } else {
            cmdMessenger.sendCmd(kDebug, F("OutputShifter: Memory full"));
#endif
        }
    }

    void OnInit(void)
    {
        MFOutputShifter *MFS;
        uint8_t          module = cmdMessenger.readInt16Arg();
        MFS                     = (MFOutputShifter *)(Stowage.getNth((uint8_t)module, kTypeOutShiftReg));
        if (MFS) {
            MFS->clear();
            setLastCommandMillis();
        }
    }

    void OnSet(void)
    {
        MFOutputShifter *MFS;
        int              module = cmdMessenger.readInt16Arg();
        char            *pins   = cmdMessenger.readStringArg();
        int              value  = cmdMessenger.readInt16Arg();
        MFS                     = (MFOutputShifter *)(Stowage.getNth((uint8_t)module, kTypeOutShiftReg));
        if (MFS) {
            MFS->setPins(pins, value);
            setLastCommandMillis();
        }
    }
} // namespace

// OutputShifter.cpp
