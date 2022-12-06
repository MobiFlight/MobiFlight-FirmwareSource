//
// MFStepper.cpp
//
// (C) MobiFlight Project 2022
//

#include "mobiflight.h"
#include "MFStepper.h"

enum {          // enumeration for type
    B28BYJ_OLD, // init Stepper in FULL4WIRE for backwards compatibility
    B28BYJ_NEW, // init Stepper in HALF4WIRE
    X27,        // init Stepper in HALF4WIRE
    DRIVER      // init Stepper in DRIVER
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

void MFStepper::attach(uint8_t pin1, uint8_t pin2, uint8_t pin3, uint8_t pin4, uint8_t btnPin5, uint8_t typeID, int8_t backlash, bool deactivateOutput)
{
    if (!FitInMemory(sizeof(AccelStepper))) {
        // Error Message to Connector
        cmdMessenger.sendCmd(kStatus, F("MFStepper does not fit in Memory"));
        return;
    }
    _type             = typeID;
    uint16_t maxSpeed = 0;
    uint16_t Accel    = 0;

    switch (_type) {
    case B28BYJ_OLD:
        // init B28BYJ stepper in full 4 wire mode as before
        _type    = AccelStepper::FULL4WIRE;
        maxSpeed = STEPPER_SPEED_B28BYJ_OLD;
        Accel    = STEPPER_ACCEL_B28BYJ_OLD;
        if (pin1 == pin3 && pin2 == pin4) // for backwards compatibility
            _type = AccelStepper::DRIVER;
        break;
    case B28BYJ_NEW:
        // init B28BYJ stepper in half 4 wire mode as new standard
        _type    = AccelStepper::HALF4WIRE;
        maxSpeed = STEPPER_SPEED_B28BYJ;
        Accel    = STEPPER_ACCEL_B28BYJ;
        break;
    case X27:
        // init new X27 stepper in half 4 wire mode
        _type    = AccelStepper::HALF4WIRE;
        maxSpeed = STEPPER_SPEED_X27;
        Accel    = STEPPER_ACCEL_X27;
        break;
    case DRIVER:
        // init stepper in driver mode
        _type    = AccelStepper::DRIVER;
        maxSpeed = STEPPER_SPEED_DRIVER;
        Accel    = STEPPER_ACCEL_X27;
        break;
    default:
        _initialized = false;
        return;
        break;
    }
    _stepper = new (allocateMemory(sizeof(AccelStepper))) AccelStepper(_type, pin4, pin2, pin1, pin3);
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
    uint16_t deltaSpeed;
    uint16_t deltaAccel;
    _resetting = false;

    if (_targetPos != newPosition) {
        if (_inMove == MOVE_CW && newPosition < _stepper->currentPosition()) // moving in CW direction AND a change of direction
            newPosition -= _backlash;
        if (_inMove == MOVE_CCW && newPosition > _stepper->currentPosition()) // moving in CCW direction AND a change of direction
            newPosition += _backlash;
        if (newPosition > _targetPos)
            _inMove = MOVE_CW;
        else
            _inMove = MOVE_CCW;
        if (_deactivateOutput && _inMove == STOP)
            _stepper->enableOutputs();
        _stepper->moveTo(newPosition);
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
