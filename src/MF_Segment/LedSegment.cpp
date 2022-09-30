//
// LedSegment.cpp
//
// (C) MobiFlight Project 2022
//

#include <Arduino.h>
#include "mobiflight.h"
#include "MFSegments.h"

extern CmdMessenger cmdMessenger;
extern StowManager  Stowage;

namespace LedSegment
{
    DEFINE_VT_STUBS(MFSegments); // see IODevice.h

    void Add(uint8_t dataPin, uint8_t csPin, uint8_t clkPin, uint8_t numDevices, uint8_t brightness)
    {
        MFSegments *MFS;

        Stowage.AddItem(&MFS);

        if (MFS) {
            MFS->attach(dataPin, csPin, clkPin, numDevices, brightness);
#ifdef DEBUG2CMDMESSENGER
            cmdMessenger.sendCmd(kDebug, F("Added LEDSegment"));
        } else {
            cmdMessenger.sendCmd(kDebug, F("LEDSegment: Memory full"));
#endif
        }
    }

    void OnSetBrightness(void)
    {
        MFSegments *MFS;
        int         module     = cmdMessenger.readInt16Arg();
        int         subModule  = cmdMessenger.readInt16Arg();
        int         brightness = cmdMessenger.readInt16Arg();
        MFS                    = (MFSegments *)(Stowage.getNth((uint8_t)module, kTypeLedSegment));
        if (MFS) {
            MFS->setBrightness(subModule, brightness);
            setLastCommandMillis();
        }
    }

    void OnInit(void)
    {
        OnSetBrightness(); // Same function
    }

    void OnSet(void)
    {
        MFSegments *MFS;
        int         module    = cmdMessenger.readInt16Arg();
        int         subModule = cmdMessenger.readInt16Arg();
        char       *value     = cmdMessenger.readStringArg();
        uint8_t     points    = (uint8_t)cmdMessenger.readInt16Arg();
        uint8_t     mask      = (uint8_t)cmdMessenger.readInt16Arg();
        // MFS = static_cast<MFSegments *>(Stowage.getNth(module, kTypeLedSegment));
        MFS = (MFSegments *)(Stowage.getNth((uint8_t)module, kTypeLedSegment));
        if (MFS) {
            MFS->setval(subModule, value, points, mask);
            setLastCommandMillis();
        }
    }

} // namespace

// LedSegment.cpp
