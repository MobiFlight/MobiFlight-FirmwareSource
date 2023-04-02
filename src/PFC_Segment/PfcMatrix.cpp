//
// PfcMatrix.cpp
//
// (C) MobiFlight Project 2022
//

#include "mobiflight.h"
#include "PfcMatrix.h"
#include "MFOutputShifter.h"
#include "MFOutput.h"
#include "MFPfcMatrix.h"

namespace PfcMatrix
{
    MFPfcMatrix *pfcMatrix;
    char         display1[] = "123";
    char         display2[] = "12345";
    bool         outputNM;
    bool         outputKTS;
    bool         outputMHz;
    bool         outputMin;

    void Add(int dataPin, int csPin, int clkPin,
             uint8_t digit1, uint8_t digit2, uint8_t digit3, uint8_t digit4,
             uint8_t digit5, uint8_t digit6, uint8_t digit7, uint8_t digit8,
             uint8_t digit9, uint8_t digit10)
    {
        if (!FitInMemory(sizeof(MFPfcMatrix))) {
            // Error Message to Connector
            cmdMessenger.sendCmd(kStatus, F("MFPfcMatrix does not fit in Memory!"));
            return;
        }

        pfcMatrix = new (allocateMemory(sizeof(MFPfcMatrix))) MFPfcMatrix;
        pfcMatrix->attach(dataPin, csPin, clkPin, digit1, digit2, digit3, digit4, digit5, digit6, digit7, digit8, digit9, digit10);

#ifdef DEBUG2CMDMESSENGER
        cmdMessenger.sendCmd(kDebug, F("Added Led Segment"));
#endif
    }

    void Clear()
    {
        pfcMatrix->detach();
#ifdef DEBUG2CMDMESSENGER
        cmdMessenger.sendCmd(kDebug, F("Cleared segments"));
#endif
    }

    void PowerSave(bool state)
    {
        // todo
    }

    void OnSetDisplay()
    {
        int module = cmdMessenger.readInt16Arg();
        // we have to read submodule but we don't use it
        cmdMessenger.readInt16Arg();
        char   *value  = cmdMessenger.readStringArg();
        uint8_t points = (uint8_t)cmdMessenger.readInt16Arg();
        uint8_t mask   = (uint8_t)cmdMessenger.readInt16Arg();

        if (module == 0) {
            // we want to use the first display
            pfcMatrix->display(0, value, 0, 0b1110000000);
        }
        if (module == 1) {
            // we want to use the second display
            pfcMatrix->display(0, value, 0, 0b0001111100);
        }

        setLastCommandMillis();
    }

    void OnSet()
    {
        // Read led state argument, interpret string as boolean
        int pin   = cmdMessenger.readInt16Arg();
        int state = cmdMessenger.readInt16Arg();
        // Set led
        analogWrite(pin, state); // why does the UI sends the pin number and not the x.th output number like other devices?
                                 //  output[pin]->set(state);      // once this is changed uncomment this
        setLastCommandMillis();
    }

    void update()
    {
        pfcMatrix->update();
    }
} // namespace

// PfcMatrix.cpp
