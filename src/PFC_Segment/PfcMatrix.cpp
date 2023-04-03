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
#include "MFButton.h"

namespace PfcMatrix
{
    MFPfcMatrix *pfcMatrix;
    MFButton    *buttons[6];
    char         display1[] = "123";
    char         display2[] = "12345";
    bool         outputNM;
    bool         outputKTS;
    bool         outputMHz;
    bool         outputMin;

    const uint8_t NM  = 2;
    const uint8_t KTS = 3;
    const uint8_t MHz = 4;
    const uint8_t Min = 5;

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
        cmdMessenger.sendCmd(kDebug, F("Added PfcMatrix"));
#endif
    }

    void Clear()
    {
        pfcMatrix->detach();
#ifdef DEBUG2CMDMESSENGER
        cmdMessenger.sendCmd(kDebug, F("Cleared PfcMatrix"));
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
        char    *value  = cmdMessenger.readStringArg();
        uint8_t  points = (uint8_t)cmdMessenger.readInt16Arg();
        uint16_t mask   = (uint16_t)cmdMessenger.readInt16Arg();

        if (module == 0) {
            // we want to use the first display
            pfcMatrix->display(0, value, points, mask);
        }
        if (module == 1) {
            // we want to use the second display
            pfcMatrix->display(0, value, points, mask << 3);
        }

        setLastCommandMillis();
    }

    void OnSet()
    {
        // Read led state argument, interpret string as boolean
        int pin   = cmdMessenger.readInt16Arg();
        int state = cmdMessenger.readInt16Arg();

        byte segments = 0;
        byte digit    = 8;

        if (pin != NM && pin != MHz && pin != KTS && pin != Min)
            return;

        switch (pin) {
        case NM:
        case MHz:
            segments = 0b00001001;
            break;

        case KTS:
        case Min:
            segments = 0b00100100;
            break;
        }

        if (pin == MHz || pin == Min)
            digit = 9;

        pfcMatrix->setSegment(digit, segments, state);

        setLastCommandMillis();
    }

    void update()
    {
        if (pfcMatrix == nullptr) return;

        pfcMatrix->update();
    }
} // namespace

// PfcMatrix.cpp
