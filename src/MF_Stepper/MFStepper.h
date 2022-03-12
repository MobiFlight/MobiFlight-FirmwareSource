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
public:
    MFStepper();
    void    attach(uint8_t pin1 = 1, uint8_t pin2 = 2, uint8_t pin3 = 3, uint8_t pin4 = 4, uint8_t btnPin1 = 0);
    void    detach();
    void    update();
    void    reset();
    void    moveTo(long absolute);
    void    setMaxSpeed(float speed);
    void    setAcceleration(float acceleration);
    void    setZero();
    uint8_t getZeroPin();

private:
    bool          _initialized;
    bool          _resetting;
    AccelStepper *_stepper;
    uint8_t       _zeroPin;
    uint8_t       _zeroPinState;
    long          _targetPos;

    void          checkZeroPin(void);
    void          setZeroInReset(void);
};

// MFStepper.h
