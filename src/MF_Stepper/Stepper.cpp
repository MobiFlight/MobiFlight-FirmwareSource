//
// Stepper.cpp
//
// (C) MobiFlight Project 2022
//

#include <Arduino.h>
#include "mobiflight.h"
#include "MFStepper.h"

namespace Stepper
{
    DEFINE_VT_STUBS(MFStepper);   // see IODevice.h

    void Add(uint8_t pin1, uint8_t pin2, uint8_t pin3, uint8_t pin4, uint8_t zeroPin)
    {
        MFStepper *MFS;

        Stowage.AddItem(&MFS);

        if(MFS) {
            MFS->attach(pin1, pin2, pin3, pin4, zeroPin);

            MFS->setMaxSpeed(STEPPER_SPEED);        //TODO move to constructor?
            MFS->setAcceleration(STEPPER_ACCEL);    //TODO move to constructor?
            if (zeroPin > 0)                        //TODO move to constructor?
            {
                MFS->reset(ONRESET_DEFAULT);
            }
#ifdef DEBUG2MSG
            cmdMessenger.sendCmd(kStatus, F("Added Stepper"));
        } else {
            cmdMessenger.sendCmd(kStatus, F("Stepper: Memory full"));
#endif
        }
    }

    enum {
        NONE = 0,
        SET,
        RESET, 
        ZERO,
    };
    
    void _stepperEvent(uint8_t evt) 
    {
        MFStepper *MFS;
        int nStepper = cmdMessenger.readInt16Arg();
        int value;
        if(SET == evt) {
            value  = cmdMessenger.readInt32Arg();
        }        
        MFS = (MFStepper *)(Stowage.getNth((uint8_t)nStepper, kTypeStepper));
        if(MFS) {
            if(SET == evt) {
                MFS->setval(value);
            } else
            if(RESET == evt) {
                MFS->reset(ONRESET_DEFAULT);
            } else
            if(ZERO == evt) {
                MFS->setZero();
            } else
            {
                evt = NONE;
            }
            if(NONE != evt) setLastCommandMillis();
        }
    }

    void OnSet(void)
    {
        _stepperEvent(SET);
    }

    void OnReset(void)
    {
        _stepperEvent(RESET);
    }
    
    void OnSetZero(void)
    {
        _stepperEvent(ZERO);
    }
    

}   // namespace

// Stepper.cpp
