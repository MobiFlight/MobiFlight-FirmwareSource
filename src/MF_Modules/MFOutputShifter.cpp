// MFOutputShifter.cpp
//
// Copyright (C) 2021

#include "MFOutputShifter.h"

MFOutputShifter::MFOutputShifter()
{
  _initialized = false;
}

void MFOutputShifter::setPin(uint8_t pin, uint8_t value, uint8_t refresh)
{
  if (!_initialized) return;

  uint8_t idx = (pin & 0xF8)>>3;
  uint8_t msk = (0x01 << (pin & 0x07));
  //if(idx < _moduleCount) return;
  if (value > 0) {
    _outputBuffer[idx] |= msk;  
  } else {
    _outputBuffer[idx] &= ~msk;  
  }    
  if(refresh) updateShiftRegister();
}

void MFOutputShifter::setPins(char* pins, uint8_t value)
{
  if (!_initialized) return;

  char* pinTokens = strtok(pins, "|");
  while (pinTokens != 0) {
    uint8_t num = (uint8_t)atoi(pinTokens);
    setPin(num, value, 0);
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
  for (uint8_t i = 0; i < _moduleCount; i++) {   
    _outputBuffer[i] = 0;
  }
  updateShiftRegister();  
}


void MFOutputShifter::_testbar(uint8_t mode)
{
  for (uint8_t b = 0; b < _moduleCount * 8; b++) {   
    uint8_t idx = (b & 0xF8)>>3;
    uint8_t msk = (0x01 << (b & 0x07));
    if(mode==0) {   // Set n-th output ON
        _outputBuffer[idx] = msk;
        if(msk == 0x01 && idx != 0) _outputBuffer[idx-1] = 0x00;
    } else 
    if(mode==1) {   // Set all outputs ON incrementally
        _outputBuffer[idx] |= msk;
    } else 
    if(mode==2) {   // Set all outputs OFF incrementally
        _outputBuffer[idx] &= ~msk;
    } 
    updateShiftRegister();
    delay(50);
  }
  if(mode==0) {
    _outputBuffer[_moduleCount-1] = 0;
    updateShiftRegister();
  }
}

void MFOutputShifter::test() 
{
    // Two sequences to choose from:
    
    // 1. Span all outputs, one at a time 
    _testbar(0);
    
    // 2. All outputs incrementally ON, then all OFF
    // (when driving LEDs, or heavier loads, beware of power drain!)
    //_testbar(1);
    //_testbar(2);
}


void MFOutputShifter::updateShiftRegister()
{
   digitalWrite(_latchPin, LOW);
   for (uint8_t i = _moduleCount; i>0 ; i--) {
      shiftOut(_dataPin, _clockPin, MSBFIRST, _outputBuffer[i]); //LSBFIRST, MSBFIRST,
   }    
   digitalWrite(_latchPin, HIGH);
}
