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
    MFServo *servos[MAX_MFSERVOS];
    uint8_t  servosRegistered = 0;

    void     Add(int pin)
    {
        if (servosRegistered == MAX_MFSERVOS)
            return;

        if (!FitInMemory(sizeof(MFServo))) {
            // Error Message to Connector
            cmdMessenger.sendCmd(kStatus, F("Servo does not fit in Memory!"));
            return;
        }
        servos[servosRegistered] = new (allocateMemory(sizeof(MFServo))) MFServo;
        servos[servosRegistered]->attach(pin, true);
        servosRegistered++;
#ifdef DEBUG2CMDMESSENGER
        cmdMessenger.sendCmd(kDebug, F("Added servos"));
#endif
    }

    void Clear()
    {
        for (uint8_t i = 0; i < servosRegistered; i++) {
            servos[i]->detach();
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
        servos[servo]->moveTo(newValue);
        setLastCommandMillis();
    }

    void update()
    {
        for (uint8_t i = 0; i < servosRegistered; i++) {
            servos[i]->update();
        }
    }
} // namespace

// Servos.cpp
