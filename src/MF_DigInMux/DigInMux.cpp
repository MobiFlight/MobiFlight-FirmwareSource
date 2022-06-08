//
// DigInMux.cpp
//
// (C) MobiFlight Project 2022
//

#include <Arduino.h>
#include "mobiflight.h"
#include "MFDigInMux.h"
#include "MFMuxDriver.h"

namespace DigInMux
{
    DEFINE_VT_STUBS(MFDigInMux);   // see IODevice.h

    void OnChange(uint8_t eventId, uint8_t pin, const char *name)
    {
        cmdMessenger.sendCmdStart(kDigInMuxChange);
        cmdMessenger.sendCmdArg(name);
        cmdMessenger.sendCmdArg(pin);
        cmdMessenger.sendCmdArg(eventId);
        cmdMessenger.sendCmdEnd();
    };

    void Add(uint8_t dataPin, uint8_t nRegs, char const *name, bool mode)
    {
        MFDigInMux *MFI;

        Stowage.AddItem(&MFI);

        if(MFI) {
            MFI->attach(dataPin, (nRegs==1), name);
            MFI->setLazyMode(mode==MFDigInMux::MuxModeLazy);
            MFDigInMux::setMux(&MUX);
            MFDigInMux::attachHandler(OnChange);
#ifdef DEBUG2MSG
            cmdMessenger.sendCmd(kStatus, F("Added DigInMux"));
        } else {
            cmdMessenger.sendCmd(kStatus, F("DigInMux: Memory full"));
#endif
        }
    }

}

// DigInMux.cpp
