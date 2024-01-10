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
    uint8_t   maxOutputs        = 0;

    bool setupArray(uint16_t count)
    {
        if (!FitInMemory(sizeof(MFOutput) * count))
            return false;
        outputs    = new (allocateMemory(sizeof(MFOutput) * count)) MFOutput;
        maxOutputs = count;
        return true;
    }

    void Add(uint8_t pin)
    {
        if (outputsRegistered == maxOutputs)
            return;
        outputs[outputsRegistered] = MFOutput();
        outputs[outputsRegistered].attach(pin);
        outputsRegistered++;
#ifdef DEBUG2CMDMESSENGER
        cmdMessenger.sendCmdStart(kDebug);
        cmdMessenger.sendCmdArg(F("Added output with "));
        cmdMessenger.sendCmdArg(sizeof(MFOutput));
        cmdMessenger.sendCmdArg(F(" byte"));
        cmdMessenger.sendCmdEnd();
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
        if (state == 0xFF)
            digitalWrite(pin, MF_HIGH);
        else if (state == 0x00)
            digitalWrite(pin, MF_LOW);
        else
            analogWrite(pin, state);
    }

    void PowerSave(bool state)
    {
        for (uint8_t i = 0; i < outputsRegistered; ++i) {
            outputs[i].powerSavingMode(state);
        }
    }
} // namespace

// Output.cpp
