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
#define STEPPER_SPEED 400
#define STEPPER_ACCEL 800

public:
    MFStepper();
    void    attach(uint8_t pin1 = 1, uint8_t pin2 = 2, uint8_t pin3 = 3, uint8_t pin4 = 4, uint8_t btnPin1 = 0, uint8_t mode = 0, int8_t backlash = 0, bool deactivateOutput = false);
    void    detach();
    void    update();
    void    reset();
    void    moveTo(long absolute);
    void    setMaxSpeed(uint16_t speed);
    void    setAcceleration(uint16_t acceleration);
    void    setZero();
    void    powerSavingMode(bool state);

private:
    bool          _initialized;
    bool          _resetting;
    AccelStepper *_stepper;
    uint8_t       _zeroPin;
    uint8_t       _zeroPinState;
    long          _targetPos;
    uint8_t       _backlash;
    bool          _deactivateOutput;
    bool          _inMove;
    bool          _isStopped;

    void checkZeroPin(void);
    void setZeroInReset(void);
};

// MFStepper.h
