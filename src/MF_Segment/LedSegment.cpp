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
    MFSegments *ledSegments[MAX_LEDSEGMENTS];
    uint8_t     ledSegmentsRegistered = 0;

    void        Add(int dataPin, int csPin, int clkPin, int numDevices, int brightness)
    {
        if (ledSegmentsRegistered == MAX_LEDSEGMENTS)
            return;

        if (!FitInMemory(sizeof(MFSegments))) {
            // Error Message to Connector
            cmdMessenger.sendCmd(kStatus, F("7Segment does not fit in Memory!"));
            return;
        }
        ledSegments[ledSegmentsRegistered] = new (allocateMemory(sizeof(MFSegments))) MFSegments;
        ledSegments[ledSegmentsRegistered]->attach(dataPin, csPin, clkPin, numDevices, brightness); // lc is our object
        ledSegmentsRegistered++;
#ifdef DEBUG2CMDMESSENGER
        cmdMessenger.sendCmd(kDebug, F("Added Led Segment"));
#endif
    }

    void Clear()
    {
        for (uint8_t i = 0; i < ledSegmentsRegistered; i++) {
            ledSegments[i]->detach();
        }
        ledSegmentsRegistered = 0;
#ifdef DEBUG2CMDMESSENGER
        cmdMessenger.sendCmd(kDebug, F("Cleared segments"));
#endif
    }

    void PowerSave(bool state)
    {
        for (uint8_t i = 0; i < ledSegmentsRegistered; ++i) {
            ledSegments[i]->powerSavingMode(state);
        }
    }

    void OnInitModule()
    {
        int module     = cmdMessenger.readInt16Arg();
        int subModule  = cmdMessenger.readInt16Arg();
        int brightness = cmdMessenger.readInt16Arg();
        ledSegments[module]->setBrightness(subModule, brightness);
        setLastCommandMillis();
    }

    void OnSetModule()
    {
        int     module    = cmdMessenger.readInt16Arg();
        int     subModule = cmdMessenger.readInt16Arg();
        char   *value     = cmdMessenger.readStringArg();
        uint8_t points    = (uint8_t)cmdMessenger.readInt16Arg();
        uint8_t mask      = (uint8_t)cmdMessenger.readInt16Arg();
        ledSegments[module]->display(subModule, value, points, mask);
        setLastCommandMillis();
    }

    void OnSetModuleBrightness()
    {
        int module     = cmdMessenger.readInt16Arg();
        int subModule  = cmdMessenger.readInt16Arg();
        int brightness = cmdMessenger.readInt16Arg();
        ledSegments[module]->setBrightness(subModule, brightness);
        setLastCommandMillis();
    }
} // namespace

// LedSegment.cpp
