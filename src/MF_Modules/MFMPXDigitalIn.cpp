// MFMPXDigitalIn.cpp
//
// Copyright (C) 2021

#include "MFMPXDigitalIn.h"
#include "mobiflight.h"

MPXDigitalInEvent MFMPXDigitalIn::_inputHandler = NULL;

MFMPXDigitalIn::MFMPXDigitalIn(const char *name)
: _name(name)
{
    clearLastState();
    _flags = 0x00;
}

// Registers a new MPX input block and configures the driver pins
void MFMPXDigitalIn::
attach( uint8_t Sel0Pin, uint8_t Sel1Pin, uint8_t Sel2Pin, uint8_t Sel3Pin, 
        uint8_t dataPin, bool halfSize, char const *name)
{
    _selPin[0]  = Sel0Pin;
    _selPin[1]  = Sel1Pin;
    _selPin[2]  = Sel2Pin;
    _selPin[3]  = Sel3Pin;
    _dataPin    = dataPin;
    _name       = name;
    _flags      = (halfSize ? MPX_HALFSIZE : 0x00);

    for(uint8_t i=0; i<4; i++) pinMode(_selPin[i], OUTPUT);
    pinMode(_dataPin, INPUT);
    _flags |= MPX_INITED;
}

#if 0
// Useless uber-optimized version, for test purposes only
void MFMPXDigitalIn::setSelectorOpt(uint8_t mode)
{
    uint8_t mask = 0x0F;
    uint8_t currSel;
    if(0 == mode) {
        _flags &= 0xF0;
        currSel = 0;
    } else {
        currSel = _flags & 0x0F;
        if(currSel == 15) return;
        mask = currSel;
        mask ^= ++currSel;
        _flags = ((_flags & 0xF0) | currSel);
    }
    for(uint8_t i=0; i<4; i++) {
        if(mask & 0x01) digitalWrite(_selPin[i], (currSel & 0x01));
        mask    >>= 1;
        currSel >>= 1;
    }
    
}
#endif

// Sets the driver lines to select the specified channel
void MFMPXDigitalIn::setSelector(uint8_t value)
{
    for(uint8_t i=0; i<4; i++) {
        digitalWrite(_selPin[i], (value & 0x01));
        value >>= 1;
    }
}

// Reads the values from the attached modules, compares them to the previously
// read values, and calls the registered event handler for any inputs that
// changed from the previously read state.
void MFMPXDigitalIn::update()
{
    poll(true);
}

// Helper function for update() and retrigger()
void MFMPXDigitalIn::poll(bool detect)
{
    uint8_t selMax = ((_flags & MPX_HALFSIZE) ? 16 : 8);
    uint16_t currentState = 0x0000;

    for (uint8_t sel = selMax; sel > 0; sel--)
    {
        setSelector(sel-1);
        currentState |= (digitalRead(_dataPin) ? 1 : 0);
        currentState <<= 1;
    }
    if(_lastState != currentState)
    {
        if(detect) detectChanges(_lastState, currentState);
        _lastState = currentState;
    }
}

// Detects changes between the current state and the previously saved state
void MFMPXDigitalIn::detectChanges(uint16_t lastState, uint16_t currentState)
{
    uint8_t     selMax = ((_flags & MPX_HALFSIZE) ? 16 : 8);
    uint16_t    diff   = lastState ^ currentState;
    for (uint8_t i = 0; i < selMax; i++)
    {
        if (diff & 0x0001) {
            trigger(i, currentState & 0x0001);
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
    poll(false);    // just read, do not retrigger

    // Pass 1/2: Trigger all the 'off' inputs (released buttons) first
    detectChanges(0x0000, _lastState);

    // Pass 2/2: Trigger all the 'on' inputs (pressed buttons)
    detectChanges(0xFFFF, _lastState);
}

// Triggers the event handler for the associated input channel
void MFMPXDigitalIn::trigger(uint8_t channel, bool state)
{
    if(!_inputHandler) return;
    (*_inputHandler)((state ? MPXDigitalInOnRelease : MPXDigitalInOnPress), channel, _name);
}

// Attaches a new event handler for the specified event.
void MFMPXDigitalIn::attachHandler(MPXDigitalInEvent newHandler)
{
  _inputHandler = newHandler;
}

void MFMPXDigitalIn::detach()
{
    _flags &= (~MPX_INITED);
}

// Clears the internal state
void MFMPXDigitalIn::clear()
{
    clearLastState();
}

// Sets the last recorded state of every bit on every shifter to 0.
void MFMPXDigitalIn::clearLastState()
{
    _lastState = 0;
}