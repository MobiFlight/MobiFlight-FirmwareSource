//
// MFButton.cpp
//
// (C) MobiFlight Project 2022
//

#include "MFButton.h"

buttonEvent MFButton::_handler = NULL;

MFButton::MFButton(uint8_t pin, const char *name)
{
#ifdef USE_FAST_IO
    _pinPort = portInputRegister(digitalPinToPort(pin));
    _pinMask = digitalPinToBitMask(pin);
#endif
    _pin   = pin;
    _name  = name;
    pinMode(pin, INPUT_PULLUP);    // set pin to input
    _state = digitalRead(pin);     // initialize on actual status
}

void MFButton::update()
{
#ifdef USE_FAST_IO
    uint8_t newState = digitalReadFast(_pinPort, _pinMask);
#else
    uint8_t newState = (uint8_t)digitalRead(_pin);
#endif
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
    if (_handler && _state == LOW) {
        (*_handler)(btnOnPress, _pin, _name);
    }
}

void MFButton::triggerOnRelease()
{
    if (_handler && _state == HIGH) {
        (*_handler)(btnOnRelease, _pin, _name);
    }
}

void MFButton::attachHandler(buttonEvent newHandler)
{
    _handler = newHandler;
}

// MFButton.cpp