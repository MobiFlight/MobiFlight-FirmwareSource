//
// MFStepper.h
//
// (C) MobiFlight Project 2022
//

#pragma once

#include <Arduino.h>
#include <AccelStepper.h>

class MFStepper
{
#define STEPPER_SPEED_B28BYJ_OLD       400
#define STEPPER_ACCEL_B28BYJ_OLD       800
#define STEPPER_DELTA_SPEED_B28BYJ_OLD 10
#define STEPPER_DELTA_ACCEL_B28BYJ_OLD 10
#define STEPPER_SPEED_B28BYJ           1600
#define STEPPER_ACCEL_B28BYJ           4000
#define STEPPER_DELTA_SPEED_B28BYJ     2
#define STEPPER_DELTA_ACCEL_B28BYJ     4
#define STEPPER_SPEED_X27              1300
#define STEPPER_ACCEL_X27              4000
#define STEPPER_DELTA_SPEED_X27        2
#define STEPPER_DELTA_ACCEL_X27        4
#define STEPPER_SPEED_DRIVER           400
#define STEPPER_ACCEL_DRIVER           800
#define STEPPER_DELTA_SPEED_DRIVER     2
#define STEPPER_DELTA_ACCEL_DRIVER     4

public:
    MFStepper();
    void    attach(uint8_t pin1 = 1, uint8_t pin2 = 2, uint8_t pin3 = 3, uint8_t pin4 = 4, uint8_t btnPin1 = 0, uint8_t type = 0, int8_t backlash = 0, bool deactivateOutput = false);
    void    detach();
    void    update();
    void    reset();
    void    moveTo(long absolute);
    void    setMaxSpeed(uint16_t speed);
    void    setAcceleration(uint16_t acceleration);
    void    setZero();
    uint8_t getZeroPin();

private:
    bool          _initialized;
    bool          _resetting;
    AccelStepper *_stepper;
    uint8_t       _zeroPin;
    uint8_t       _zeroPinState;
    long          _targetPos;
    uint8_t       _backlash;
    bool          _deactivateOutput;
    int8_t        _inMove;
    uint8_t       _type;

    void checkZeroPin(void);
    void setZeroInReset(void);
};

// MFStepper.h
