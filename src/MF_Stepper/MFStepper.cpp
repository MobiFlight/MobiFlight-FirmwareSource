//
// MFStepper.cpp
//
// (C) MobiFlight Project 2022
//

#include "mobiflight.h"
#include "MFStepper.h"
#include "Stepper.h"

MFStepper::MFStepper()
{
    _initialized = false;
}

void MFStepper::attach(uint8_t pin1, uint8_t pin2, uint8_t pin3, uint8_t pin4, uint8_t btnPin5, uint8_t typeID, int8_t backlash, bool deactivateOutput)
{
    if (!FitInMemory(sizeof(AccelStepper))) {
        // Error Message to Connector
        cmdMessenger.sendCmd(kStatus, F("MFStepper does not fit in Memory"));
        return;
    }
    switch (typeID) {
    case Stepper::B28BYJ_OLD:
        // init new stepper in full 4 wire mode as before
        _stepper = new (allocateMemory(sizeof(AccelStepper))) AccelStepper(AccelStepper::FULL4WIRE, pin4, pin2, pin1, pin3);
        break;
    case Stepper::B28BYJ_NEW:
        // init new stepper in full 4 wire mode
        _stepper = new (allocateMemory(sizeof(AccelStepper))) AccelStepper(AccelStepper::HALF4WIRE, pin4, pin2, pin1, pin3);
        break;
    case Stepper::X27:
        // init new stepper in full 4 wire mode
        _stepper = new (allocateMemory(sizeof(AccelStepper))) AccelStepper(AccelStepper::HALF4WIRE, pin4, pin2, pin1, pin3);
        break;
    case Stepper::DRIVER:
        // init new stepper in driver mode
        _stepper = new (allocateMemory(sizeof(AccelStepper))) AccelStepper(AccelStepper::DRIVER, pin4, pin2, pin1, pin3);
        break;
    default:
        _initialized = false;
        return;
        break;
    }
    _zeroPin      = btnPin5;
    _zeroPinState = HIGH;

    if (_zeroPin) {
        pinMode(_zeroPin, INPUT_PULLUP); // set pin to input
    }

    _backlash         = backlash;
    _deactivateOutput = deactivateOutput;
    _initialized      = true;
    _resetting        = false;
}

void MFStepper::detach()
{
    _initialized = false;
}

void MFStepper::moveTo(long newPosition)
{
    _resetting = false;
    if (_targetPos != newPosition) {
        if (_stepper->currentPosition() < _targetPos && _stepper->currentPosition() > newPosition) // moving in CW direction AND a change of direction
            newPosition -= _backlash;
        if (_stepper->currentPosition() > _targetPos && _stepper->currentPosition() < newPosition) // moving in CCW direction AND a change of direction
            newPosition += _backlash;
        _targetPos = newPosition;
        if (_deactivateOutput)
            _stepper->enableOutputs();
        _stepper->moveTo(newPosition);
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
    if (_stepper->currentPosition() == _targetPos && _deactivateOutput) {
        _stepper->disableOutputs();
    }
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

void MFStepper::setMaxSpeed(uint16_t speed)
{
    _stepper->setMaxSpeed(speed);
}

void MFStepper::setAcceleration(uint16_t acceleration)
{
    _stepper->setAcceleration(acceleration);
}

// MFStepper.cpp
