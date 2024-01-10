//
// DigInMux.cpp
//
// (C) MobiFlight Project 2022
//

#include "mobiflight.h"
#include "MFDigInMux.h"
#include "MFMuxDriver.h"

extern MFMuxDriver MUX;

namespace DigInMux
{
    MFDigInMux *digInMux;
    uint8_t     digInMuxRegistered = 0;
    uint8_t     maxDigInMux        = 0;

    void handlerOnDigInMux(uint8_t eventId, uint8_t channel, uint8_t deviceID)
    {
        cmdMessenger.sendCmdStart(kDigInMuxChange);
        cmdMessenger.sendCmdArg(deviceID);
        cmdMessenger.sendCmdArg(channel);
        cmdMessenger.sendCmdArg(eventId);
        cmdMessenger.sendCmdEnd();
    };

    bool setupArray(uint16_t count)
    {
        if (!FitInMemory(sizeof(MFDigInMux) * count))
            return false;
        digInMux    = new (allocateMemory(sizeof(MFDigInMux) * count)) MFDigInMux;
        maxDigInMux = count;
        return true;
    }

    void Add(uint8_t dataPin, uint8_t nRegs)
    {
        if (digInMuxRegistered == maxDigInMux)
            return;
        digInMux[digInMuxRegistered] = MFDigInMux(&MUX);
        digInMux[digInMuxRegistered].attach(dataPin, (nRegs == 1), digInMuxRegistered);
        MFDigInMux::attachHandler(handlerOnDigInMux);
        digInMuxRegistered++;

#ifdef DEBUG2CMDMESSENGER
        cmdMessenger.sendCmd(kDebug, F("Added digital input MUX"));
#endif
    }

    void Clear()
    {
        for (uint8_t i = 0; i < digInMuxRegistered; i++) {
            digInMux[i].detach();
        }
        digInMuxRegistered = 0;
#ifdef DEBUG2CMDMESSENGER
        cmdMessenger.sendCmd(kDebug, F("Cleared dig. input Muxes"));
#endif
    }

    void read()
    {
        for (uint8_t i = 0; i < digInMuxRegistered; i++) {
            digInMux[i].update();
        }
    }

    void OnTrigger()
    {
        for (uint8_t i = 0; i < digInMuxRegistered; i++) {
            digInMux[i].retrigger();
        }
    }
} // namespace

// DigInMux.cpp