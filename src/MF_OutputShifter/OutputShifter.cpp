//
// OutputShifter.cpp
//
// (C) MobiFlight Project 2022
//

#include "commandmessenger.h"
#include "allocateMem.h"
#include "MFOutputShifter.h"
#include "OutputShifter.h"

namespace OutputShifter
{
    MFOutputShifter *outputShifter;
    uint8_t          outputShifterRegistered = 0;
    uint8_t          maxOutputShifter        = 0;

    bool setupArray(uint16_t count)
    {
        if (!count) return true;
        outputShifter = static_cast<MFOutputShifter *>(MF_ALLOC_TYPE(MFOutputShifter, count));
        if (!outputShifter) return false;

        maxOutputShifter = count;
        return true;
    }

    void Add(uint8_t latchPin, uint8_t clockPin, uint8_t dataPin, uint8_t modules)
    {
        if (outputShifterRegistered == maxOutputShifter)
            return;

        new (&outputShifter[outputShifterRegistered]) MFOutputShifter();
        if (!outputShifter[outputShifterRegistered].attach(latchPin, clockPin, dataPin, modules)) {
            cmdMessenger.sendCmd(kStatus, F("OutputShifter array does not fit into Memory"));
            return;
        }
        outputShifterRegistered++;

#ifdef DEBUG2CMDMESSENGER
        cmdMessenger.sendCmd(kDebug, F("Added Output Shifter"));
#endif
    }

    void Clear()
    {
        for (uint8_t i = 0; i < outputShifterRegistered; i++) {
            outputShifter[i].detach();
        }

        outputShifterRegistered = 0;
#ifdef DEBUG2CMDMESSENGER
        cmdMessenger.sendCmd(kDebug, F("Cleared Output Shifter"));
#endif
    }

    void OnSet()
    {
        int     module                      = cmdMessenger.readInt16Arg();
        int     number_of_submodules        = cmdMessenger.readInt16Arg();
        int     value                       = cmdMessenger.readInt16Arg();
        uint8_t _pins[number_of_submodules] = {0};

        for (uint8_t i = number_of_submodules; i != 0; i--) {
            _pins[i-1] = (uint8_t)cmdMessenger.readInt16Arg();
        }

        outputShifter[module].setPins(_pins, value);
    }

    void PowerSave(bool state)
    {
        for (uint8_t i = 0; i < outputShifterRegistered; ++i) {
            outputShifter[i].powerSavingMode(state);
        }
    }
} // namespace

// OutputShifter.cpp
