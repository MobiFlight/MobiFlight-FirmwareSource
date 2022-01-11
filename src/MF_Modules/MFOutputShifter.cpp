// MFOutputShifter.cpp
//
// Copyright (C) 2021

#include "MFOutputShifter.h"

MFOutputShifter::MFOutputShifter()
{
  _initialized = false;
}

void MFOutputShifter::setPin(uint8_t pin, int value)
{
  if (!_initialized) return;

  if (value > 0) {
    bitSet(_output, pin);  
  } else {
    bitClear(_output, pin);
  }  
  updateShiftRegister();
}

void MFOutputShifter::setPins(char* pins, int value)
{
  if (!_initialized) return;

  char* pinTokens = strtok(pins, "|");
  while (pinTokens != 0) {
    int registerNum = atoi(pinTokens);

    if (value > 0) {
      bitSet(_output, registerNum);  
    } else {
      bitClear(_output, registerNum);
    }    
    pinTokens = strtok(0, "|");
  }
  updateShiftRegister();
}

void MFOutputShifter::attach(uint8_t latchPin, uint8_t clockPin, uint8_t dataPin, uint8_t moduleCount)
{
  _initialized = true;
  _latchPin = latchPin;
  _clockPin = clockPin;
  _dataPin = dataPin;
  _moduleCount = moduleCount;

  pinMode(_latchPin, OUTPUT);
  pinMode(_clockPin, OUTPUT);  
  pinMode(_dataPin, OUTPUT);

  clear();
}

void MFOutputShifter::detach()
{
  if (!_initialized) return;
  _initialized = false;
}

void MFOutputShifter::clear() 
{
  // Set everything to 0
  _output = 0;
  updateShiftRegister();  
}

void MFOutputShifter::test() 
{
  _output = 0;
  updateShiftRegister();

  for (unsigned long i = 0; i < _moduleCount * 8; i++)
  {   
    bitSet(_output, i);
    updateShiftRegister();
    delay(50);
  }
  
  for (unsigned long i = 0; i < _moduleCount * 8; i++)
  {
    bitClear(_output, i);
    updateShiftRegister();
    delay(50);
  }
}

void MFOutputShifter::updateShiftRegister()
{
   digitalWrite(_latchPin, LOW);
   for (uint8_t i = _moduleCount; i>0 ; i--) {
      shiftOut(_dataPin, _clockPin, MSBFIRST, (_output >> ((i-1)*8))); //LSBFIRST, MSBFIRST,
   }    
   digitalWrite(_latchPin, HIGH);
}
