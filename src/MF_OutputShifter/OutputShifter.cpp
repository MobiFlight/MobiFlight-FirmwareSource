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

    /*
    Order of commands:
    1) kSetShiftRegisterPins
    2) which module (chain of shift registers)
    3) set or reset pin
    4) number of 8bit Register
    5) bitmask of which pins to be set/unset
    First byte of bit mask must be MSB
    e.g. 27,0,1,3,[0x12345678][0x87654321][0x12348765];

    in cpp it would be:
    cmdMessenger.sendCmdStart (kSetShiftRegisterPins);
    cmdMessenger.sendCmdArg<uint8_t>((uint8_t)module);
    cmdMessenger.sendCmdArg<uint8_t>((uint8_t)value);
    cmdMessenger.sendCmdArg<uint8_t>((uint8_t)number_of_submodules);
    for (uint8_t i = number_of_submodules - 1; i != 0; i--) {
        cmdMessenger.sendCmdBinArg<uint8_t>((uint8_t)value[i]);
    }
    cmdMessenger.sendCmdEnd ();
    */
    void OnSet()
    {
        int     module                      = cmdMessenger.readInt16Arg(); // which chain of shifter
        int     value                       = cmdMessenger.readInt16Arg(); // set or reset pin
        int     number_of_submodules        = cmdMessenger.readInt16Arg(); // number of 8bit Register
        uint8_t _pins[number_of_submodules] = {0};                         // pin mask

        for (uint8_t i = number_of_submodules - 1; i != 0; i--) {
            _pins[i] = cmdMessenger.readBinArg<uint8_t>(); // read in data
        }
        outputShifter[module].setPins(_pins, value); // call set function of class
    }
    /*
        void OnSet()
        {

            int   module = cmdMessenger.readInt16Arg();
            char *pins   = cmdMessenger.readStringArg();
            int   value  = cmdMessenger.readInt16Arg();
            outputShifter[module].setPins(pins, value);
        }
    */
    void PowerSave(bool state)
    {
        for (uint8_t i = 0; i < outputShifterRegistered; ++i) {
            outputShifter[i].powerSavingMode(state);
        }
    }
} // namespace

// OutputShifter.cpp
