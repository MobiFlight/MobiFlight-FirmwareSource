// MFMultiplex.cpp
//
// Copyright (C) 2021

#include "MFMultiplex.h"
#include "mobiflight.h"

MFMultiplex::MFMultiplex(void)
{
    _flags = 0x00;
}

// Registers a new MPX input block and configures the driver pins
void MFMultiplex::
attach( uint8_t Sel0Pin, uint8_t Sel1Pin, uint8_t Sel2Pin, uint8_t Sel3Pin)
{
    _selPin[0]  = Sel0Pin;
    _selPin[1]  = Sel1Pin;
    _selPin[2]  = Sel2Pin;
    _selPin[3]  = Sel3Pin;
    _flags      = 0x00;

    for(uint8_t i=0; i<4; i++) pinMode(_selPin[i], OUTPUT);
    _flags |= MPX_INITED;
}

#if 0
// Useless uber-optimized version, for test purposes only
void MFMultiplex::setChannelOpt(uint8_t mode)
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
void MFMultiplex::setChannel(uint8_t value)
{
    if(value > 15) return;
    _channel = value;
    for(uint8_t i=0; i<4; i++) {
        digitalWrite(_selPin[i], (value & 0x01));
        value >>= 1;
    }
}

uint8_t MFMultiplex::getChannel(void)
{
    return _channel;
}

uint8_t MFMultiplex::nextChannel(void)
{
    setChannel((++_channel)%16);
    return _channel;
}

void MFMultiplex::detach()
{
    for(uint8_t i=0; i<4; i++) pinMode(_selPin[i], INPUT_PULLUP);
    _flags &= (~MPX_INITED);
}
