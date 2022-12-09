//
// MFStepper.cpp
//
// (C) MobiFlight Project 2022
//

#include "mobiflight.h"
#include "MFStepper.h"

enum {          // enumeration for stepper mode
    FULL4WIRE,
    HALF4WIRE,
    DRIVER
};

enum {
    MOVE_CCW = -1,
    STOP,
    MOVE_CW
};

MFStepper::MFStepper()
{
    _initialized = false;
}

void MFStepper::attach(uint8_t pin1, uint8_t pin2, uint8_t pin3, uint8_t pin4, uint8_t btnPin5, uint8_t mode, int8_t backlash, bool deactivateOutput)
{
    if (!FitInMemory(sizeof(AccelStepper))) {
        // Error Message to Connector
        cmdMessenger.sendCmd(kStatus, F("MFStepper does not fit in Memory"));
        return;
    }
    _mode             = mode;
    uint16_t maxSpeed = 0;
    uint16_t Accel    = 0;

    switch (_mode) {
    case FULL4WIRE:
        // init B28BYJ stepper in full 4 wire mode as before
        maxSpeed = STEPPER_SPEED;
        Accel    = STEPPER_ACCEL;
        if (pin1 == pin3 && pin2 == pin4) // for backwards compatibility
            _stepper = new (allocateMemory(sizeof(AccelStepper))) AccelStepper(AccelStepper::DRIVER, pin4, pin2, pin1, pin3);
        else
            _stepper = new (allocateMemory(sizeof(AccelStepper))) AccelStepper(AccelStepper::FULL4WIRE, pin4, pin2, pin1, pin3);
        break;
    case HALF4WIRE:
        // init stepper in half 4 wire mode as new standard
        _stepper = new (allocateMemory(sizeof(AccelStepper))) AccelStepper(AccelStepper::HALF4WIRE, pin4, pin2, pin1, pin3);
        maxSpeed = STEPPER_SPEED;
        Accel    = STEPPER_ACCEL;
        break;
    case DRIVER:
        // init stepper in driver mode
        _stepper = new (allocateMemory(sizeof(AccelStepper))) AccelStepper(AccelStepper::DRIVER, pin1, pin2);
        maxSpeed = STEPPER_SPEED;
        Accel    = STEPPER_ACCEL;
        break;
    default:
        _initialized = false;
        return;
        break;
    }

    _stepper->setMaxSpeed(maxSpeed);
    _stepper->setAcceleration(Accel);

    _zeroPin      = btnPin5;
    _zeroPinState = HIGH;

    if (_zeroPin) {
        pinMode(_zeroPin, INPUT_PULLUP);
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
    /*
        if (_inMove == MOVE_CW && newPosition < _stepper->currentPosition()) // moving in CW direction AND a change of direction
            newPosition -= _backlash;
        if (_inMove == MOVE_CCW && newPosition > _stepper->currentPosition()) // moving in CCW direction AND a change of direction
            newPosition += _backlash;
        if (newPosition > _targetPos)
            _inMove = MOVE_CW;
        else
            _inMove = MOVE_CCW;
    */
        if (_deactivateOutput && _inMove == STOP)
            _stepper->enableOutputs();
        _stepper->moveTo(newPosition);
        _targetPos = newPosition;
        _inMove = 1; //must also be changed with reworking of backlash considering, just to have a working function w/o backlash
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
        _inMove = STOP;
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
