//
// Stepper.h
//
// (C) MobiFlight Project 2022
//

#pragma once

#define STEPPER_SPEED_B28BYJ 400
#define STEPPER_ACCEL_B28BYJ 800

#define STEPPER_SPEED_X27 1600
#define STEPPER_ACCEL_X27 4000

#define STEPPER_SPEED_DRIVER 400
#define STEPPER_ACCEL_DRIVER 800
namespace Stepper
{
    enum {          // enumeration for typeID
        B28BYJ_OLD, // init Stepper in FULL4WIRE for backwards compatibility
        B28BYJ_NEW, // init Stepper in HALF4WIRE
        X27,        // init Stepper in HALF4WIRE
        DRIVER      // init Stepper in DRIVER
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