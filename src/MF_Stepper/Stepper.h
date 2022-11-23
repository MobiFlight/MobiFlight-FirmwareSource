//
// Stepper.h
//
// (C) MobiFlight Project 2022
//

#pragma once

namespace Stepper
{
    enum {          // enumeration for typeID
        B28BYJ_OLD, // init Stepper in FULL4WIRE as before
        B28BYJ_NEW, // init Stepper in HALF4WIRE, set maxSpeed/Accel to ???
        X27,        // init Stepper in HALF4WIRE, set maxSpeed/Accel to ???
        DRIVER      // init Stepper in DRIVER, only pin1 and pin2 needed
    };

    void Add(uint8_t pin1, uint8_t pin2, uint8_t pin3, uint8_t pin4, uint8_t btnPin1, uint8_t type = 0, int8_t backlash = 0, bool deactivateOutput = false);
    void Clear();
    void OnSet();
    void OnReset();
    void OnSetZero();
    void update();
    void OnSetSpeedAccel();
}

// Stepper.h