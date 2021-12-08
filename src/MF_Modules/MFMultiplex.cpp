// MFMultiplex.cpp
//
// Copyright (C) 2021

#include "MFMultiplex.h"
#include "mobiflight.h"

MFMultiplex::MFMultiplex(void)
{
    _flags = 0x00;
    for(uint8_t i=0; i<4; i++) {
        _selPin[i] = 0xFF;
    }
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
    bitSet(_flags, MPX_INITED);
    
    setChannel(0);
}

void MFMultiplex::detach()
{
    for(uint8_t i=0; i<4; i++) {
        if(_selPin[i] == 0xFF) continue;
        pinMode(_selPin[i], INPUT_PULLUP);
        _selPin[i] = 0xFF;
    }
    bitClear(_flags, MPX_INITED);
}

/// \brief Sets the driver lines to select the specified channel
void MFMultiplex::setChannel(uint8_t value)
{
    if(!bitRead(_flags, MPX_INITED)) return;
    if(value > 15) return;
    _channel = value;
    for(uint8_t i=0; i<4; i++) {
        digitalWrite(_selPin[i], (value & 0x01));
        value >>= 1;
    }
}

/// \brief Returns currently selected channel
uint8_t MFMultiplex::getChannel(void)
{
    return _channel;
}

/// \brief  Increments current channel, wraps around to 0
uint8_t MFMultiplex::nextChannel(void)
{
    setChannel((++_channel)%16);
    return _channel;
}

/// \brief  Temporarily stores current channel for later retrieval
void MFMultiplex::saveChannel(void)
{
    _savedChannel = _channel;
}

/// \brief  Restored previously stored channel
void MFMultiplex::restoreChannel(void)
{
    setChannel(_savedChannel);
}

