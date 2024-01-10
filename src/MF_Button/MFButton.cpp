//
// MFButton.cpp
//
// (C) MobiFlight Project 2022
//
#include "MFButton.h"

buttonEvent MFButton::_handler = NULL;

MFButton::MFButton()
{
    _initialized = false;
}

void MFButton::attach(uint8_t pin, uint8_t deviceID)
{
    _pin      = pin;
    _deviceID = deviceID;
    pinMode(_pin, INPUT_PULLUP); // set pin to input
    _state = digitalRead(_pin);  // initialize on actual status
    _initialized = true;
}

void MFButton::update()
{
    if (!_initialized)
        return;
    uint8_t newState = (uint8_t)digitalRead(_pin);
    if (newState != _state) {
        _state = newState;
        trigger(_state);
    }
}

void MFButton::trigger(uint8_t state)
{
    (state == LOW) ? triggerOnPress() : triggerOnRelease();
}

void MFButton::triggerOnPress()
{
    if (!_initialized)
        return;
    if (_handler && _state == LOW) {
        (*_handler)(btnOnPress, _deviceID);
    }
}

void MFButton::triggerOnRelease()
{
    if (!_initialized)
        return;
    if (_handler && _state == HIGH) {
        (*_handler)(btnOnRelease, _deviceID);
    }
}

void MFButton::attachHandler(buttonEvent newHandler)
{
    _handler = newHandler;
}

// MFButton.cpp