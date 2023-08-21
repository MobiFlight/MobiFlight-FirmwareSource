//
// Servos.cpp
//
// (C) MobiFlight Project 2022
//

#include "mobiflight.h"
#include "MFServo.h"
#include "Servos.h"

namespace Servos
{
    MFServo *servos;
    uint8_t  servosRegistered = 0;

    void setupArray(uint16_t count)
    {
        if (count) return;

        // ToDo: how to handle exceeding device memory!!
        if (!FitInMemory(sizeof(MFServo) * count)) {
            cmdMessenger.sendCmd(kStatus, F("Servo does not fit in Memory!"));
            return;
        }
        servos = new (allocateMemory(sizeof(MFServo) * count)) MFServo;
    }

    void Add(int pin)
    {
        servos[servosRegistered] = MFServo();
        servos[servosRegistered].attach(pin, true);
        servosRegistered++;
#ifdef DEBUG2CMDMESSENGER
        cmdMessenger.sendCmd(kDebug, F("Added servos"));
#endif
    }

    void Clear()
    {
        for (uint8_t i = 0; i < servosRegistered; i++) {
            servos[i].detach();
        }
        servosRegistered = 0;
#ifdef DEBUG2CMDMESSENGER
        cmdMessenger.sendCmd(kDebug, F("Cleared servos"));
#endif
    }

    void OnSet()
    {
        int servo    = cmdMessenger.readInt16Arg();
        int newValue = cmdMessenger.readInt16Arg();
        if (servo >= servosRegistered)
            return;
        servos[servo].moveTo(newValue);
        setLastCommandMillis();
    }

    void update()
    {
        for (uint8_t i = 0; i < servosRegistered; i++) {
            servos[i].update();
        }
    }
} // namespace

// Servos.cpp
