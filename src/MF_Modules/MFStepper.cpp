// MFStepper.cpp
//
// Copyright (C) 2013-2014

#include "MFBoards.h"
#include "MFStepper.h"
#include "MFButton.h"
#include "allocateMem.h"
#include "mobiflight.h"

uint8_t MFStepper_stepperCount = 0;
MFStepper *MFStepper_steppers[MAX_STEPPERS];

MFStepper::MFStepper()
{
  _initialized = false;
}

void MFStepper::attach(uint8_t pin1, uint8_t pin2, uint8_t pin3, uint8_t pin4, uint8_t btnPin5)
{
  if (!FitInMemory(sizeof(AccelStepper)))
  {
    // Error Message to Connector
		cmdMessenger.sendCmdStart(kDebug);
		cmdMessenger.sendCmdArg(F("Stepper does not fit in Memory"));
		cmdMessenger.sendCmdEnd();
    return;
  }
  _stepper = new (allocateMemory(sizeof(AccelStepper))) AccelStepper;
  _stepper->init(AccelStepper::FULL4WIRE, pin4, pin2, pin1, pin3);
  _zeroPin=btnPin5;
  _zeroPinState=HIGH;

  if(_zeroPin) {
    pinMode(_zeroPin, INPUT_PULLUP);     // set pin to input
  }  
  _resetting = false;
}
void MFStepper::detach()
{
  if (!_initialized)
    return;
  _initialized = false;
}


void addStepper(MFStepper *stepper)
{
  MFStepper_steppers[MFStepper_stepperCount] = stepper;
  MFStepper_stepperCount++;
}

void MFStepper::moveTo(long absolute)
{
  _resetting = false;
  if (_targetPos != absolute)
  {
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
  if (_resetting)
  {
    _stepper->setCurrentPosition(0);
    _resetting = false;
  }
}

void MFStepper::checkZeroPin()
{
    uint8_t newState = (uint8_t) digitalRead(_zeroPin);
    if (newState!=_zeroPinState) {     
      _zeroPinState = newState;
      if(_zeroPinState == LOW) setZeroInReset();      
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
