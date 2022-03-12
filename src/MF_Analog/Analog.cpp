#include <Arduino.h>
#include "MFAnalog.h"
#include "allocateMem.h"
#include "mobiflight.h"
#include "commandmessenger.h"
#include "Analog.h"
#include "MFBoards.h"

#if MF_ANALOG_SUPPORT == 1
namespace Analog
{
MFAnalog *analog[MAX_ANALOG_INPUTS];
uint8_t analogRegistered = 0;

void handlerOnAnalogChange(int value, uint8_t pin, const char *name)
{
    cmdMessenger.sendCmdStart(kAnalogChange);
    cmdMessenger.sendCmdArg(name);
    cmdMessenger.sendCmdArg(value);
    cmdMessenger.sendCmdEnd();
};

void Add(uint8_t pin = 1, char const *name = "AnalogInput", uint8_t sensitivity = 3)
{
    if (analogRegistered == MAX_ANALOG_INPUTS)
        return;

    if (!FitInMemory(sizeof(MFAnalog)))
    {
        // Error Message to Connector
        cmdMessenger.sendCmd(kStatus, F("AnalogIn does not fit in Memory"));
        return;
    }
    analog[analogRegistered] = new (allocateMemory(sizeof(MFAnalog))) MFAnalog(pin, name, sensitivity);
    analog[analogRegistered]->attachHandler(handlerOnAnalogChange);
    analogRegistered++;
    #ifdef DEBUG2CMDMESSENGER
    cmdMessenger.sendCmd(kStatus, F("Added analog device "));
    #endif
}

void Clear()
{
    analogRegistered = 0;
    #ifdef DEBUG2CMDMESSENGER
    cmdMessenger.sendCmd(kStatus, F("Cleared analog devices"));
    #endif
}

void read()
{
    for (int i = 0; i != analogRegistered; i++)
    {
        analog[i]->update();
    }
}

void readAverage()
{
    for (int i = 0; i != analogRegistered; i++)
    {
    analog[i]->readBuffer();
    }
}
}       // end of namespace Analog
#endif
