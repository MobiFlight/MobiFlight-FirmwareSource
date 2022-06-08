//
// Analog.cpp
// 
// (C) MobiFlight Project 2022
//
#include <Arduino.h>
#include "mobiflight.h"
#include "MFAnalog.h"
namespace Analog
{
    DEFINE_VT_STUBS(MFAnalog);   // see IODevice.h

    void OnChange(int value, uint8_t pin, const char *name)
    {
        cmdMessenger.sendCmdStart(kAnalogChange);
        cmdMessenger.sendCmdArg(name);
        cmdMessenger.sendCmdArg(value);
        cmdMessenger.sendCmdEnd();
    };

    void Add(uint8_t pin, uint8_t sensitivity, char const *name)
    {
        MFAnalog *MFA;

        Stowage.AddItem(&MFA);
        
        if(MFA) {
            MFA->attach(pin, sensitivity, name);
            MFAnalog::attachHandler(OnChange);
#ifdef DEBUG2MSG
            cmdMessenger.sendCmd(kStatus, F("Added Analog"));
        } else {
            cmdMessenger.sendCmd(kStatus, F("Analog: Memory full"));
#endif
        }
    }

    void UpdateAverage(void)
    {
        MFAnalog *ain;

        Stowage.reset();
        while((ain = (MFAnalog *)Stowage.getNext(kTypeAnalogInput)) != NULL) {
            ain->updateAverage();
        }
    }
} // namespace

// Analog.cpp
