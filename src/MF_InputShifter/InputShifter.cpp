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
    uint8_t         maxInputShiffter        = 0;

    void handlerInputShifterOnChange(uint8_t eventId, uint8_t pin, const char *name)
    {
        cmdMessenger.sendCmdStart(kInputShifterChange);
        cmdMessenger.sendCmdArg(name);
        cmdMessenger.sendCmdArg(pin);
        cmdMessenger.sendCmdArg(eventId);
        cmdMessenger.sendCmdEnd();
    };

    bool setupArray(uint16_t count)
    {
        if (!FitInMemory(sizeof(MFInputShifter) * count))
            return false;
        inputShifters    = new (allocateMemory(sizeof(MFInputShifter) * count)) MFInputShifter;
        maxInputShiffter = count;
        return true;
    }

    void Add(uint8_t latchPin, uint8_t clockPin, uint8_t dataPin, uint8_t modules, char const *name)
    {
        if (inputShiftersRegistered == maxInputShiffter)
            return;
        inputShifters[inputShiftersRegistered] = MFInputShifter();
        inputShifters[inputShiftersRegistered].attach(latchPin, clockPin, dataPin, modules, name);
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

    void OnInit() // not used anywhere!?
    {
        int module = cmdMessenger.readInt16Arg();
        inputShifters[module].clear();
    }

} // namespace

// InputShifter.cpp
