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
    MFStepper *steppers;
    uint8_t    steppersRegistered = 0;
    uint8_t    maxSteppers        = 0;

    bool setupArray(uint16_t count)
    {
        if (!FitInMemory(sizeof(MFStepper) * count))
            return false;
        steppers    = new (allocateMemory(sizeof(MFStepper) * count)) MFStepper;
        maxSteppers = count;
        return true;
    }

    void Add(uint8_t pin1, uint8_t pin2, uint8_t pin3, uint8_t pin4, uint8_t btnPin1, uint8_t mode, int8_t backlash, bool deactivateOutput)
    {
        if (steppersRegistered == maxSteppers)
            return;
        steppers[steppersRegistered] = MFStepper();
        steppers[steppersRegistered].attach(pin1, pin2, pin3, pin4, btnPin1, mode, backlash, deactivateOutput);

        if (btnPin1 > 0) {
            // this triggers the auto reset if we need to reset
            steppers[steppersRegistered].reset();
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
            steppers[i].detach();
        }
        steppersRegistered = 0;
#ifdef DEBUG2CMDMESSENGER
        cmdMessenger.sendCmd(kDebug, F("Cleared steppers"));
#endif
    }

    void OnSet()
    {
        uint8_t stepper = (uint8_t)cmdMessenger.readInt16Arg();
        long    newPos  = cmdMessenger.readInt32Arg();

        if (stepper >= steppersRegistered)
            return;
        steppers[stepper].moveTo(newPos);
        setLastCommandMillis();
    }

    void OnReset()
    {
        uint8_t stepper = (uint8_t)cmdMessenger.readInt16Arg();

        if (stepper >= steppersRegistered)
            return;
        steppers[stepper].reset();
        setLastCommandMillis();
    }

    void OnSetZero()
    {
        uint8_t stepper = (uint8_t)cmdMessenger.readInt16Arg();

        if (stepper >= steppersRegistered)
            return;
        steppers[stepper].setZero();
        setLastCommandMillis();
    }

    void OnSetSpeedAccel()
    {
        uint8_t  stepper  = (uint8_t)cmdMessenger.readInt16Arg();
        uint16_t maxSpeed = cmdMessenger.readInt16Arg();
        uint16_t maxAccel = cmdMessenger.readInt16Arg();

        if (stepper >= steppersRegistered)
            return;
        steppers[stepper].setMaxSpeed(maxSpeed);
        steppers[stepper].setAcceleration(maxAccel);
    }

    void update()
    {
        for (uint8_t i = 0; i < steppersRegistered; i++) {
            steppers[i].update();
        }
    }

    void PowerSave(bool state)
    {
        for (uint8_t i = 0; i < steppersRegistered; ++i) {
            steppers[i].powerSavingMode(state);
        }
    }

} // namespace

// Stepper.cpp
