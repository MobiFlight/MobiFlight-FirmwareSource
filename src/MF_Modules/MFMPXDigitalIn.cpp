// MFMPXDigitalIn.cpp
//
// Copyright (C) 2021

#include "MFMPXDigitalIn.h"
#include "MFMultiplex.h"
#include "mobiflight.h"

MFMultiplex * MFMPXDigitalIn::_MPX;

MPXDigitalInEvent MFMPXDigitalIn::_inputHandler = NULL;


MFMPXDigitalIn::MFMPXDigitalIn(void)
{
    _MPX = NULL;
    _name = "MPXDigIn";
    _flags = 0x00;
    clear();
}

MFMPXDigitalIn::MFMPXDigitalIn(MFMultiplex *MPX, const char *name)
: _name(name)
{
    if(MPX) _MPX = MPX;
    _flags = 0x00;
    clear();
}

void MFMPXDigitalIn::setMPX(MFMultiplex *MPX)
{
    _MPX = MPX;
}

// Registers a new MPX input block and configures the driver pins
void MFMPXDigitalIn::attach(uint8_t dataPin, bool halfSize, char const *name)
{
    if(!_MPX) return;
    _dataPin    = dataPin;
    _name       = name;
    _flags      = 0x00; 
    if(halfSize) bitSet(_flags, MPX_HALFSIZE);
    pinMode(_dataPin, INPUT);
    bitSet(_flags, MPX_INITED);
}

void MFMPXDigitalIn::detach()
{
    pinMode(_dataPin, INPUT_PULLUP);
    bitClear(_flags, MPX_INITED);
}

// Reads the values from the attached modules, compares them to the previously
// read values, and calls the registered event handler for any inputs that
// changed from the previously read state.
void MFMPXDigitalIn::update()
{
    poll(true, bitRead(_flags, MPX_LAZY));
}

// Helper function for update() and retrigger()
void MFMPXDigitalIn::poll(bool detect, bool isLazy)
{
    if(!_MPX) return;

    // Meaning of "Lazy mode" flag
    // ===========================
    //
    // Lazy mode ON:
    // MPX selector is set externally, normally at main loop level
    // (incremented sequentially at each pass) 
    // Individual modules work in one of two ways:
    // 1. they must have an associate channel number (which may also be "any"),
    //    and only execute if that matches the current channel;
    // 2. account for current channel number in their internal working 
    //    (e.g. for digital inputs, "place input bit in position #n").
    //
    // Lazy mode OFF (default):
    // Every block using the multiplexer sets its own selector value (or span of values).
    // MPX selector can have any value upon entry; it is saved and restored before exit.
    //
    // Each block can use its preferred mode, and blocks of both types can co-exist.

    uint8_t  selMax = (bitRead(_flags, MPX_HALFSIZE) ? 16 : 8);;

    if(!isLazy) {

        uint16_t    currentState = 0x0000;
        _MPX->saveChannel();
        for (uint8_t sel = selMax; sel > 0; sel--)
        {
            _MPX->setChannel(sel-1);
            currentState |= (digitalRead(_dataPin) ? 1 : 0);
            currentState <<= 1;
        }
        _MPX->restoreChannel(); // tidy up

        if(_lastState != currentState)
        {
            if(detect) detectChanges(_lastState, currentState);
            _lastState = currentState;
        }

    } else {

        bool     chVal = (digitalRead(_dataPin) ? true : false);
        uint8_t  ch    = _MPX->getChannel() % selMax;
        uint16_t msk   = (0x0001<<ch);
        
        if(((_lastState & msk)!=0) != chVal) trigger(ch, chVal);

        if(chVal) {
            _lastState |= msk;
        } else {
            _lastState &= ~msk;
        }
    }
}

// Detects changes between the current state and the previously saved state
void MFMPXDigitalIn::detectChanges(uint16_t lastState, uint16_t currentState)
{
    if(!_MPX) return;
    uint8_t     selMax = (bitRead(_flags, MPX_HALFSIZE) ? 16 : 8);
    uint16_t    diff   = lastState ^ currentState;
    for (uint8_t i = 0; i < selMax; i++)
    {
        if (diff & 0x0001) {
            trigger(i, ((currentState & 0x0001)!=0));
        }
        diff >>= 1;
        currentState >>= 1;
    }
}

// Reads the current state for all connected modules then fires
// 'release' events for every 'off' input, followed by
// 'press' events for every 'on' input.
// (Remember that 'off' inputs actually have physical status '1')
void MFMPXDigitalIn::retrigger()
{
    // The current state for all attached modules is stored,
    // so future update() calls will work off whatever was read by the
    // retrigger flow.
    poll(false, false);    // just read, do not retrigger

    // Pass 1/2: Trigger all the 'off' inputs (released buttons) first
    detectChanges(0x0000, _lastState);

    // Pass 2/2: Trigger all the 'on' inputs (pressed buttons)
    detectChanges(0xFFFF, _lastState);
}

// Triggers the event handler for the associated input channel
void MFMPXDigitalIn::trigger(uint8_t channel, bool state)
{
    if(!_MPX) return;
    if(!_inputHandler) return;
    (*_inputHandler)((state ? MPXDigitalInOnRelease : MPXDigitalInOnPress), channel, _name);
}

// Attaches a new event handler for the specified event.
void MFMPXDigitalIn::attachHandler(MPXDigitalInEvent newHandler)
{
  _inputHandler = newHandler;
}

// Clears the internal state
void MFMPXDigitalIn::clear()
{
    _lastState = 0;
}

void MFMPXDigitalIn::setLazyMode(bool mode)
{
    if(mode) {
        bitSet(_flags, MPX_LAZY);
    } else {
        bitClear(_flags, MPX_LAZY);
    }
}
