//
// MFStepper.cpp
//
// (C) MobiFlight Project 2022
//

#include "mobiflight.h"
#include "MFStepper.h"

enum { // enumeration for stepper mode
    FULL4WIRE,
    HALF4WIRE,
    DRIVER
};

enum {
    MOVE_CCW,
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
    uint16_t maxSpeed = 0;
    uint16_t Accel    = 0;

    switch (mode) {
    case FULL4WIRE:
        maxSpeed = 100; // STEPPER_SPEED;
        Accel    = STEPPER_ACCEL;
        if (pin1 == pin3 && pin2 == pin4) // for backwards compatibility
            _stepper = new (allocateMemory(sizeof(AccelStepper))) AccelStepper(AccelStepper::DRIVER, pin4, pin2, pin1, pin3);
        else
            _stepper = new (allocateMemory(sizeof(AccelStepper))) AccelStepper(AccelStepper::FULL4WIRE, pin4, pin2, pin1, pin3);
        break;
    case HALF4WIRE:
        _stepper = new (allocateMemory(sizeof(AccelStepper))) AccelStepper(AccelStepper::HALF4WIRE, pin4, pin2, pin1, pin3);
        maxSpeed = STEPPER_SPEED;
        Accel    = STEPPER_ACCEL;
        break;
    case DRIVER:
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

    _backlash         = 40;
    _deactivateOutput = deactivateOutput;
    _initialized      = true;
    _resetting        = false;
    _isStopped        = true;
    _inMove           = MOVE_CW;
}

void MFStepper::detach()
{
    _initialized = false;
}

void MFStepper::moveTo(long newPosition)
{
    _resetting           = false;
    long currentPosition = _stepper->currentPosition();

    if (_targetPos != newPosition) {
        if (_deactivateOutput && _isStopped) {
            _stepper->enableOutputs();
            _isStopped = false;
        }
/*
        if (_inMove == MOVE_CW) {
            if (newPosition > min(currentPosition, _targetPos))
                _inMove = MOVE_CW;
            else
                _inMove = MOVE_CCW;
        } else {
            if (newPosition > max(currentPosition, _targetPos))
                _inMove = MOVE_CW;
            else
                _inMove = MOVE_CCW;
        }
*/
/*
        if (_inMove == MOVE_CW) {
    // this if clause is the same as above but not using min() finction
            if (newPosition > currentPosition || newPosition > _targetPos)
                _inMove = MOVE_CW;
            else
                _inMove = MOVE_CCW;
        } else {
    // this if clause is the same as above but not using max() finction
            if (newPosition < currentPosition || newPosition < _targetPos)
                _inMove = MOVE_CCW;
            else
                _inMove = MOVE_CW;
        }
*/
/*
        if (_inMove == MOVE_CW) {
    // only the else from above is required as only in this case _inMove is changed
    // so it can be transferred in this if clause
            if (newPosition < currentPosition && newPosition < _targetPos)
                _inMove = MOVE_CCW;
        } else {
    // only the else from above is required as only in this case _inMove is changed
    // so it can be transferred in this if clause
            if (newPosition > currentPosition && newPosition > _targetPos)
                _inMove = MOVE_CW;
        }
*/
    // And now the first if clause is moved into the second if clause
        if (_inMove == MOVE_CW && newPosition < currentPosition && newPosition < _targetPos)
            _inMove = MOVE_CCW;
    // And now the first if clause is moved into the second if clause
        if (_inMove == MOVE_CCW && newPosition > currentPosition && newPosition > _targetPos)
            _inMove = MOVE_CW;

        Serial.print("Alte Position: ");
        Serial.println(_targetPos);
        Serial.print("Current Position: ");
        Serial.println(currentPosition);
        Serial.print("Neue Position: ");
        Serial.println(newPosition);
        Serial.print("Fahre zu Position: ");
        Serial.println(newPosition + _backlash * _inMove);
        Serial.print("Richtung ist: ");
        Serial.println(_inMove);
        Serial.println("----------------------------");

        _stepper->moveTo(newPosition + _backlash * _inMove);
        _targetPos = newPosition;
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
    if (_stepper->currentPosition() == (_targetPos + _backlash * _inMove) && _deactivateOutput) {
        _stepper->disableOutputs();
        _isStopped = true;
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
