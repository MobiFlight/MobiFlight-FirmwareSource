//
// LedSegment.cpp
//
// (C) MobiFlight Project 2022
//

#include "mobiflight.h"
#include "MFSegments.h"
#include "LedSegment.h"

namespace LedSegment
{
    MFSegments *ledSegments;
    uint8_t     ledSegmentsRegistered  = 0;
    uint8_t     ledSegmentsRegistereds = 0;

    bool setupArray(uint16_t count)
    {
        if (!FitInMemory(sizeof(MFSegments) * count))
            return false;
        ledSegments            = new (allocateMemory(sizeof(MFSegments) * count)) MFSegments;
        ledSegmentsRegistereds = count;
        return true;
    }

    void Add(int dataPin, int csPin, int clkPin, int numDevices, int brightness)
    {
        if (ledSegmentsRegistered == ledSegmentsRegistereds)
            return;
        ledSegments[ledSegmentsRegistered] = MFSegments();
        ledSegments[ledSegmentsRegistered].attach(dataPin, csPin, clkPin, numDevices, brightness); // lc is our object
        ledSegmentsRegistered++;
#ifdef DEBUG2CMDMESSENGER
        cmdMessenger.sendCmd(kDebug, F("Added Led Segment"));
#endif
    }

    void Clear()
    {
        for (uint8_t i = 0; i < ledSegmentsRegistered; i++) {
            ledSegments[i].detach();
        }
        ledSegmentsRegistered = 0;
#ifdef DEBUG2CMDMESSENGER
        cmdMessenger.sendCmd(kDebug, F("Cleared segments"));
#endif
    }

    void PowerSave(bool state)
    {
        for (uint8_t i = 0; i < ledSegmentsRegistered; ++i) {
            ledSegments[i].powerSavingMode(state);
        }
    }

    void OnInitModule()
    {
        int module     = cmdMessenger.readInt16Arg();
        int subModule  = cmdMessenger.readInt16Arg();
        int brightness = cmdMessenger.readInt16Arg();
        ledSegments[module].setBrightness(subModule, brightness);
        setLastCommandMillis();
    }

    void OnSetModule()
    {
        int     module    = cmdMessenger.readInt16Arg();
        int     subModule = cmdMessenger.readInt16Arg();
        char   *value     = cmdMessenger.readStringArg();
        uint8_t points    = (uint8_t)cmdMessenger.readInt16Arg();
        uint8_t mask      = (uint8_t)cmdMessenger.readInt16Arg();
        ledSegments[module].display(subModule, value, points, mask);
        setLastCommandMillis();
    }

    void OnSetModuleBrightness()
    {
        int module     = cmdMessenger.readInt16Arg();
        int subModule  = cmdMessenger.readInt16Arg();
        int brightness = cmdMessenger.readInt16Arg();
        ledSegments[module].setBrightness(subModule, brightness);
        setLastCommandMillis();
    }
} // namespace

// LedSegment.cpp
