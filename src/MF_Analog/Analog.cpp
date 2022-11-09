//
// Analog.cpp
//
// (C) MobiFlight Project 2022
//

#include "mobiflight.h"
#include "MFAnalog.h"
#include "Analog.h"

#if MF_ANALOG_SUPPORT == 1
namespace Analog
{
    MFAnalog *analog[MAX_ANALOG_INPUTS];
    uint8_t   analogRegistered = 0;

    void handlerOnAnalogChange(int16_t value, uint8_t deviceID)
    {
        cmdMessenger.sendCmdStart(kAnalogChange);
        cmdMessenger.sendCmdArg(deviceID);
        cmdMessenger.sendCmdArg(value);
        cmdMessenger.sendCmdEnd();
    };

    void Add(uint8_t pin, uint8_t sensitivity)
    {
        if (analogRegistered == MAX_ANALOG_INPUTS)
            return;

        if (!FitInMemory(sizeof(MFAnalog))) {
            // Error Message to Connector
            cmdMessenger.sendCmd(kStatus, F("AnalogIn does not fit in Memory"));
            return;
        }
        analog[analogRegistered] = new (allocateMemory(sizeof(MFAnalog))) MFAnalog(pin, analogRegistered, sensitivity);
        MFAnalog::attachHandler(handlerOnAnalogChange);
        analogRegistered++;
#ifdef DEBUG2CMDMESSENGER
        cmdMessenger.sendCmd(kDebug, F("Added analog device "));
#endif
    }

    void Clear(void)
    {
        analogRegistered = 0;
#ifdef DEBUG2CMDMESSENGER
        cmdMessenger.sendCmd(kDebug, F("Cleared analog devices"));
#endif
    }

    void read(void)
    {
        for (uint8_t i = 0; i < analogRegistered; i++) {
            analog[i]->update();
        }
    }

    void readAverage(void)
    {
        for (uint8_t i = 0; i < analogRegistered; i++) {
            analog[i]->readBuffer();
        }
    }

    void OnTrigger()
    {
        // Scan and transit current values
        for (uint8_t i = 0; i < analogRegistered; i++) {
            analog[i]->retrigger();
        }
    }

} // namespace Analog
#endif

// Analog.cpp
