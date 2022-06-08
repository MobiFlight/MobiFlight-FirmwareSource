// 
// MFButton.cpp
//
// (C) MobiFlight Project 2022
// 

#include "MFButton.h"
#include "mobiflight.h"

buttonEventHandler   MFButton::_handler = NULL;

MFButton::MFButton(void)
{ 
    attach(0xFF, "Button");
}

void MFButton::attach(uint8_t pin, const char * name)
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
      trigger(_state);      
    }
}

void MFButton::trigger(uint8_t state)
{
    if(_handler) {
        (*_handler)((state == LOW ? btnOnPress : btnOnRelease), _pin, _name);
    }
}

void MFButton::reset(uint8_t action)
{
    // Handle retrigger logic according to:
    // https://github.com/MobiFlight/MobiFlight-Connector/issues/497
    // and  https://github.com/MobiFlight/MobiFlight-Connector/pull/502.
    
    if(_handler) {
        if (action == ONRESET_PRESS && _state == LOW) {
            (*_handler)(btnOnPress,   _pin, _name);
        } else 
        if (action == ONRESET_RELEASE && _state != LOW) {
            (*_handler)(btnOnRelease, _pin, _name);
        }
    }
}

// MFButton.cpp
