//
// InputShifter.cpp
//
// (C) MobiFlight Project 2022
//

#include "mobiflight.h"
#include "MFInputShifter.h"
#include "InputShifter.h"

namespace InputShifter
{
    MFInputShifter *inputShifters;
    uint8_t         inputShiftersRegistered = 0;
    uint8_t         maxInputShifter        = 0;

    void handlerInputShifterOnChange(uint8_t eventId, uint8_t pin, uint8_t deviceID)
    {
        cmdMessenger.sendCmdStart(kInputShifterChange);
        cmdMessenger.sendCmdArg(deviceID);
        cmdMessenger.sendCmdArg(pin);
        cmdMessenger.sendCmdArg(eventId);
        cmdMessenger.sendCmdEnd();
    };

    bool setupArray(uint16_t count)
    {
        if (!FitInMemory(sizeof(MFInputShifter) * count))
            return false;
        inputShifters    = new (allocateMemory(sizeof(MFInputShifter) * count)) MFInputShifter;
        maxInputShifter = count;
        return true;
    }

    void Add(uint8_t latchPin, uint8_t clockPin, uint8_t dataPin, uint8_t modules)
    {
        if (inputShiftersRegistered == maxInputShifter)
            return;
        inputShifters[inputShiftersRegistered] = MFInputShifter();
        if (!inputShifters[inputShiftersRegistered].attach(latchPin, clockPin, dataPin, modules, inputShiftersRegistered))
        {
            cmdMessenger.sendCmd(kStatus, F("InputShifter array does not fit into Memory"));
            return;
        }
        MFInputShifter::attachHandler(handlerInputShifterOnChange);
        inputShiftersRegistered++;
#ifdef DEBUG2CMDMESSENGER
        cmdMessenger.sendCmd(kDebug, F("Added input shifter"));
#endif
    }

    void Clear()
    {
        for (uint8_t i = 0; i < inputShiftersRegistered; i++) {
            inputShifters[i].detach();
        }
        inputShiftersRegistered = 0;
#ifdef DEBUG2CMDMESSENGER
        cmdMessenger.sendCmd(kDebug, F("Cleared input shifter"));
#endif
    }

    void read()
    {
        for (uint8_t i = 0; i < inputShiftersRegistered; i++) {
            inputShifters[i].update();
        }
    }

    void OnTrigger()
    {
        // Retrigger all the input shifters. This automatically sends
        // the release events first followed by press events.
        for (uint8_t i = 0; i < inputShiftersRegistered; i++) {
            inputShifters[i].retrigger();
        }
    }

} // namespace

// InputShifter.cpp
