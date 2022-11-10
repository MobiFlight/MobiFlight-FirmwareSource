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
    MFDigInMux *digInMux[MAX_DIGIN_MUX];
    uint8_t     digInMuxRegistered = 0;

    void handlerOnDigInMux(uint8_t eventId, uint8_t channel, uint8_t deviceID)
    {
        cmdMessenger.sendCmdStart(kDigInMuxChange);
        cmdMessenger.sendCmdArg(deviceID);
        cmdMessenger.sendCmdArg(channel);
        cmdMessenger.sendCmdArg(eventId);
        cmdMessenger.sendCmdEnd();
    };

    void Add(uint8_t dataPin, uint8_t nRegs)
    {
        if (digInMuxRegistered == MAX_DIGIN_MUX)
            return;
        MFDigInMux *dip;
        dip                          = new (allocateMemory(sizeof(MFDigInMux))) MFDigInMux(&MUX);
        digInMux[digInMuxRegistered] = dip;
        dip->attach(dataPin, (nRegs == 1), digInMuxRegistered);
        dip->clear();
        // MFDigInMux::setMux(&MUX);
        MFDigInMux::attachHandler(handlerOnDigInMux);
        digInMuxRegistered++;

#ifdef DEBUG2MSG
        cmdMessenger.sendCmd(kDebug, F("Added digital input MUX"));
#endif
    }

    void Clear()
    {
        for (uint8_t i = 0; i < digInMuxRegistered; i++) {
            digInMux[digInMuxRegistered]->detach();
        }
        digInMuxRegistered = 0;
#ifdef DEBUG2CMDMESSENGER
        cmdMessenger.sendCmd(kDebug, F("Cleared dig. input Muxes"));
#endif
    }

    void read()
    {
        for (uint8_t i = 0; i < digInMuxRegistered; i++) {
            digInMux[i]->update();
        }
    }

    void OnTrigger()
    {
        for (uint8_t i = 0; i < digInMuxRegistered; i++) {
            digInMux[i]->retrigger();
        }
    }
} // namespace

// DigInMux.cpp