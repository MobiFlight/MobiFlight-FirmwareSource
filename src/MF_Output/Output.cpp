//
// Output.cpp
//
// (C) MobiFlight Project 2022
//

#include "mobiflight.h"
#include "MFOutput.h"
#include "Output.h"

namespace Output
{
    MFOutput *outputs;
    uint8_t   outputsRegistered = 0;

    void setupArray(uint16_t count)
    {
        if (count == 0) return;

        // ToDo: how to handle exceeding device memory!!
        if (!FitInMemory(sizeof(MFOutput) * count)) {
            cmdMessenger.sendCmd(kStatus, F("Output does not fit in Memory"));
            return;
        }
        outputs = new (allocateMemory(sizeof(MFOutput) * count)) MFOutput;
    }

    void Add(uint8_t pin)
    {
        outputs[outputsRegistered] = MFOutput(pin);
        outputsRegistered++;
#ifdef DEBUG2CMDMESSENGER
        cmdMessenger.sendCmd(kDebug, F("Added output"));
#endif
    }

    void Clear()
    {
        outputsRegistered = 0;
#ifdef DEBUG2CMDMESSENGER
        cmdMessenger.sendCmd(kDebug, F("Cleared outputs"));
#endif
    }

    void OnSet()
    {
        // Read led state argument, interpret string as boolean
        int pin   = cmdMessenger.readInt16Arg();
        int state = cmdMessenger.readInt16Arg();
        // Set led
        analogWrite(pin, state); // why does the UI sends the pin number and not the x.th output number like other devices?
                                 //  output[pin].set(state);      // once this is changed uncomment this
        setLastCommandMillis();
    }

    void PowerSave(bool state)
    {
        for (uint8_t i = 0; i < outputsRegistered; ++i) {
            outputs[i].powerSavingMode(state);
        }
    }
} // namespace

// Output.cpp
