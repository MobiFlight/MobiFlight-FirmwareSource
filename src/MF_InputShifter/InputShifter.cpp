#include <Arduino.h>
#include "MFInputShifter.h"
#include "allocateMem.h"
#include "mobiflight.h"
#include "commandmessenger.h"
#include "InputShifter.h"
#include "MFBoards.h"

namespace InputShifter
{
MFInputShifter *inputShifters[MAX_INPUT_SHIFTERS];
uint8_t inputShiftersRegistered = 0;

void handlerInputShifterOnChange(uint8_t eventId, uint8_t pin, const char *name)
{
    cmdMessenger.sendCmdStart(kInputShifterChange);
    cmdMessenger.sendCmdArg(name);
    cmdMessenger.sendCmdArg(pin);
    cmdMessenger.sendCmdArg(eventId);
    cmdMessenger.sendCmdEnd();
};

void Add(uint8_t latchPin, uint8_t clockPin, uint8_t dataPin, uint8_t modules, char const *name = "Shifter")
{
    if (inputShiftersRegistered == MAX_INPUT_SHIFTERS)
        return;
    if (!FitInMemory(sizeof(MFInputShifter)))
    {
        // Error Message to Connector
        cmdMessenger.sendCmd(kStatus, F("InputShifter does not fit in Memory"));
        return;
    }
    inputShifters[inputShiftersRegistered] = new (allocateMemory(sizeof(MFInputShifter))) MFInputShifter;
    inputShifters[inputShiftersRegistered]->attach(latchPin, clockPin, dataPin, modules, name);
    inputShifters[inputShiftersRegistered]->clear();
    inputShifters[inputShiftersRegistered]->attachHandler(handlerInputShifterOnChange);
    inputShiftersRegistered++;
    #ifdef DEBUG2CMDMESSENGER
    cmdMessenger.sendCmd(kStatus, F("Added input shifter"));
    #endif
}

void Clear()
{
    for (int i = 0; i < inputShiftersRegistered; i++)
    {
        inputShifters[i]->detach();
    }
    inputShiftersRegistered = 0;
    #ifdef DEBUG2CMDMESSENGER
    cmdMessenger.sendCmd(kStatus, F("Cleared input shifter"));
    #endif
}

void read()
{
    for (int i = 0; i != inputShiftersRegistered; i++)
    {
        inputShifters[i]->update();
    }
}

void OnTrigger()
{
    // Retrigger all the input shifters. This automatically sends
    // the release events first followed by press events.
    for (int i = 0; i != inputShiftersRegistered; i++)
    {
        inputShifters[i]->retrigger();
    }
    setLastCommandMillis();
}

void OnInit()     // not used anywhere!?
{
  int module = cmdMessenger.readInt16Arg();
  inputShifters[module]->clear();
  setLastCommandMillis();
}

}       // end of namespace InputShifter
