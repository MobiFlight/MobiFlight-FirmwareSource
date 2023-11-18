//
// OutputShifter.cpp
//
// (C) MobiFlight Project 2022
//

#include "mobiflight.h"
#include "MFOutputShifter.h"
#include "OutputShifter.h"

namespace OutputShifter
{
    MFOutputShifter *outputShifters;
    uint8_t          outputShifterRegistered = 0;
    uint8_t          maxOutputShifter        = 0;

    bool setupArray(uint16_t count)
    {
        if (!FitInMemory(sizeof(MFOutputShifter) * count))
            return false;
        outputShifters   = new (allocateMemory(sizeof(MFOutputShifter) * count)) MFOutputShifter;
        maxOutputShifter = count;
        return true;
    }

    void Add(uint8_t latchPin, uint8_t clockPin, uint8_t dataPin, uint8_t modules)
    {
        if (outputShifterRegistered == maxOutputShifter)
            return;
        outputShifters[outputShifterRegistered] = MFOutputShifter();
        outputShifters[outputShifterRegistered].attach(latchPin, clockPin, dataPin, modules);
        outputShifters[outputShifterRegistered].clear();
        outputShifterRegistered++;

#ifdef DEBUG2CMDMESSENGER
        cmdMessenger.sendCmdStart(kDebug);
        cmdMessenger.sendCmdArg(F("Added Output Shifter with "));
        cmdMessenger.sendCmdArg(sizeof(MFOutputShifter));
        cmdMessenger.sendCmdArg(F(" byte"));
        cmdMessenger.sendCmdEnd();
#endif
    }

    void Clear()
    {
        for (uint8_t i = 0; i < outputShifterRegistered; i++) {
            outputShifters[i].detach();
        }

        outputShifterRegistered = 0;
#ifdef DEBUG2CMDMESSENGER
        cmdMessenger.sendCmd(kDebug, F("Cleared Output Shifter"));
#endif
    }

    void OnInit() // not used anywhere!?
    {
        int module = cmdMessenger.readInt16Arg();
        outputShifters[module].clear();
    }

    void OnSet()
    {

        int   module = cmdMessenger.readInt16Arg();
        char *pins   = cmdMessenger.readStringArg();
        int   value  = cmdMessenger.readInt16Arg();
        outputShifters[module].setPins(pins, value);
    }
} // namespace

// OutputShifter.cpp
