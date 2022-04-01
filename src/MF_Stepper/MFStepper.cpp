//
// MFStepper.cpp
//
// (C) MobiFlight Project 2022
//

#include "mobiflight.h"
#include "MFStepper.h"

MFStepper::MFStepper()
{
    _initialized = false;
}

void MFStepper::attach(uint8_t pin1, uint8_t pin2, uint8_t pin3, uint8_t pin4, uint8_t btnPin5)
{
    if (!FitInMemory(sizeof(AccelStepper))) {
        // Error Message to Connector
        cmdMessenger.sendCmd(kStatus, F("MFStepper does not fit in Memory"));
        return;
    }
    if (pin2 == pin4 && pin1 == pin3) // if pin1/2 are identical to pin3/4
    {                                 // init new stepper with external driver (step and direction)
        _stepper = new (allocateMemory(sizeof(AccelStepper))) AccelStepper(AccelStepper::DRIVER, pin1, pin2);
    } else { // otherwise init new stepper in full 4 wire mode
        _stepper = new (allocateMemory(sizeof(AccelStepper))) AccelStepper(AccelStepper::FULL4WIRE, pin4, pin2, pin1, pin3);
    }
    _zeroPin      = btnPin5;
    _zeroPinState = HIGH;

    if (_zeroPin) {
        pinMode(_zeroPin, INPUT_PULLUP); // set pin to input
    }
    _initialized = true;
    _resetting   = false;
}

void MFStepper::detach()
{
    _initialized = false;
}

void MFStepper::moveTo(long absolute)
{
    _resetting = false;
    if (_targetPos != absolute) {
        _targetPos = absolute;
        _stepper->moveTo(absolute);
    }
}

uint8_t MFStepper::getZeroPin()
{
    return _zeroPin;
}

void MFStepper::setZero()
{
    _stepper->setCurrentPosition(0);
}

void MFStepper::setZeroInReset()
{
    if (_resetting) {
        _stepper->setCurrentPosition(0);
        _resetting = false;
    }
}

void MFStepper::checkZeroPin()
{
    uint8_t newState = (uint8_t)digitalRead(_zeroPin);
    if (newState != _zeroPinState) {
        _zeroPinState = newState;
        if (_zeroPinState == LOW) setZeroInReset();
    }
}

void MFStepper::update()
{
    _stepper->run();
    checkZeroPin();
}

void MFStepper::reset()
{
    // we are not a auto reset stepper if this pin is 0
    if (_zeroPin == 0)
        return;

    // if we are already resetting ignore next reset command
    if (_resetting)
        return;

    // flag that we are resetting
    _resetting = true;

    // tell stepper to move counter clockwise for a long while
    _stepper->moveTo(-100000);
}

void MFStepper::setMaxSpeed(float speed)
{
    _stepper->setMaxSpeed(speed);
}

void MFStepper::setAcceleration(float acceleration)
{
    _stepper->setAcceleration(acceleration);
}

// MFStepper.cpp
