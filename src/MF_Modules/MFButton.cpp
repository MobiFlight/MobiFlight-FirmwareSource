// MFButton.cpp
//
// Copyright (C) 2013-2014

#include "MFButton.h"

MFButton::MFButton(uint8_t pin, const char * name)
{   
  _pin  = pin;
  _name = name;
  _state = 1;
  pinMode(_pin, INPUT_PULLUP);     // set pin to input
}

void MFButton::update()
{
    uint8_t newState = (uint8_t) digitalRead(_pin);
    if (newState!=_state) {     
      _state = newState;
      trigger();      
    }
}

void MFButton::trigger()
{
      triggerOnRelease();
      triggerOnPress();
}

void MFButton::triggerOnPress()
{
      if (_state==LOW && _handlerList[btnOnPress]!= NULL) {
        (*_handlerList[btnOnPress])(btnOnPress, _pin, _name);
      }
}

void MFButton::triggerOnRelease()
{
      if (_state==HIGH && _handlerList[btnOnRelease] != NULL) {
        (*_handlerList[btnOnRelease])(btnOnRelease, _pin, _name);
      }
}

void MFButton::attachHandler(byte eventId, buttonEvent newHandler)
{
  _handlerList[eventId] = newHandler;
}