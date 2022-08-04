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
    MFOutputShifter *outputShifters[MAX_OUTPUT_SHIFTERS];
    uint8_t          outputShifterRegistered = 0;

    void             Add(uint8_t latchPin, uint8_t clockPin, uint8_t dataPin, uint8_t modules)
    {
        if (outputShifterRegistered == MAX_OUTPUT_SHIFTERS)
            return;
        if (!FitInMemory(sizeof(MFOutputShifter))) {
            // Error Message to Connector
            cmdMessenger.sendCmd(kStatus, F("OutputShifter does not fit in Memory"));
            return;
        }
        outputShifters[outputShifterRegistered] = new (allocateMemory(sizeof(MFOutputShifter))) MFOutputShifter;
        outputShifters[outputShifterRegistered]->attach(latchPin, clockPin, dataPin, modules);
        outputShifters[outputShifterRegistered]->clear();
        outputShifterRegistered++;

#ifdef DEBUG2CMDMESSENGER
        cmdMessenger.sendCmd(kDebug, F("Added Output Shifter"));
#endif
    }

    void Clear()
    {
        for (uint8_t i = 0; i < outputShifterRegistered; i++) {
            outputShifters[i]->detach();
        }

        outputShifterRegistered = 0;
#ifdef DEBUG2CMDMESSENGER
        cmdMessenger.sendCmd(kDebug, F("Cleared Output Shifter"));
#endif
    }

    void OnInit() // not used anywhere!?
    {
        int module = cmdMessenger.readInt16Arg();
        outputShifters[module]->clear();
        setLastCommandMillis();
    }

    void OnSet()
    {

        int   module = cmdMessenger.readInt16Arg();
        char *pins   = cmdMessenger.readStringArg();
        int   value  = cmdMessenger.readInt16Arg();
        outputShifters[module]->setPins(pins, value);
        setLastCommandMillis();
    }
} // namespace

// OutputShifter.cpp
