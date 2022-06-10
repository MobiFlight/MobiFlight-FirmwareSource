//
// Stepper.cpp
//
// (C) MobiFlight Project 2022
//

#include "mobiflight.h"
#include "MFStepper.h"
#include "Stepper.h"

namespace Stepper
{
    MFStepper *steppers[MAX_STEPPERS];
    uint8_t    steppersRegistered = 0;

    void Add(int pin1, int pin2, int pin3, int pin4, int btnPin1)
    {
        if (steppersRegistered == MAX_STEPPERS)
            return;

        if (!FitInMemory(sizeof(MFStepper))) {
            // Error Message to Connector
            cmdMessenger.sendCmd(kStatus, F("Stepper does not fit in Memory!"));
            return;
        }
        steppers[steppersRegistered] = new (allocateMemory(sizeof(MFStepper))) MFStepper;
        steppers[steppersRegistered]->attach(pin1, pin2, pin3, pin4, btnPin1);
        steppers[steppersRegistered]->setMaxSpeed(STEPPER_SPEED);
        steppers[steppersRegistered]->setAcceleration(STEPPER_ACCEL);
        // autoreset is not released yet
        if (btnPin1 > 0) {
            // this triggers the auto reset if we need to reset
            steppers[steppersRegistered]->reset();
        }

        // all set
        steppersRegistered++;

#ifdef DEBUG2CMDMESSENGER
        cmdMessenger.sendCmd(kDebug, F("Added stepper"));
#endif
    }

    void Clear()
    {
        for (uint8_t i = 0; i < steppersRegistered; i++) {
            steppers[i]->detach();
        }
        steppersRegistered = 0;
#ifdef DEBUG2CMDMESSENGER
        cmdMessenger.sendCmd(kDebug, F("Cleared steppers"));
#endif
    }

    void OnSet()
    {
        int  stepper = cmdMessenger.readInt16Arg();
        long newPos  = cmdMessenger.readInt32Arg();

        if (stepper >= steppersRegistered)
            return;
        steppers[stepper]->moveTo(newPos);
        setLastCommandMillis();
    }

    void OnReset()
    {
        int stepper = cmdMessenger.readInt16Arg();

        if (stepper >= steppersRegistered)
            return;
        steppers[stepper]->reset();
        setLastCommandMillis();
    }

    void OnSetZero()
    {
        int stepper = cmdMessenger.readInt16Arg();

        if (stepper >= steppersRegistered)
            return;
        steppers[stepper]->setZero();
        setLastCommandMillis();
    }

    void update()
    {
        for (uint8_t i = 0; i < steppersRegistered; i++) {
            steppers[i]->update();
        }
    }
} // namespace

// Stepper.cpp
