//
// MFEncoder.h
//
// (C) MobiFlight Project 2022
//

// Based on the RotaryEncoder library from Matthias Hertel, see below
// -----
// RotaryEncoder.cpp - Library for using rotary encoders.
// This class is implemented for use with the Arduino environment.
// Copyright (c) by Matthias Hertel, http://www.mathertel.de
// This work is licensed under a BSD style license. See http://www.mathertel.de/License.aspx
// More information on: http://www.mathertel.de/Arduino
// -----
// 18.01.2014 created by Matthias Hertel
// -----

#pragma once

#include <Arduino.h>

extern "C" {
typedef void (*encoderEvent)(uint8_t, uint8_t, const char *);
};

// this prevents the internal position overflow.
#define MF_ENC_MAX 8000

// this defines the delta value limit for triggering onFast
#define MF_ENC_FAST_LIMIT 50 // 50

enum {
    encLeft,
    encLeftFast,
    encRight,
    encRightFast,
};

typedef struct {
    // Detent positions in the quadrature (by value, not position)
    bool    detents[4];

    // Bit shift to apply given the detent resolution of this encoder.
    //
    // Example: An encoder with 1 detent per quadrature cycle has a useful resolution of
    // 1/4 of the number of pulses so we can apply a simple bit shift of 2 to
    // determine the effective position of the encoder.
    uint8_t resolutionShift;
} encoderType;

class MFEncoder
{
public:
    MFEncoder();
    static void attachHandler(encoderEvent newHandler);
    void        attach(uint8_t pin1, uint8_t pin2, uint8_t TypeEncoder, const char *name = "Encoder");
    void        update();
    // call this function every some milliseconds or by using an interrupt for handling state changes of the rotary encoder.
    void        tick(void);
    // retrieve the current position
    int16_t     getPosition();
    // adjust the current position
    void        setPosition(int16_t newPosition);

private:
    static encoderEvent _handler;
    uint8_t             _pin1;
    uint8_t             _pin2;
    bool                _initialized;
    const char         *_name;
    int16_t             _pos;
    uint8_t             _TypeEncoder;
    uint8_t             _detentCounter;
    encoderType         _encoderType;
    int8_t              _oldState;
    int16_t             _position;         // Internal position (4 times _positionExt)
    int16_t             _positionExt;      // External position
    uint32_t            _positionTime;     // time last position change was detected
    uint32_t            _positionTimePrev; // time previous position change was detected
    uint32_t            _lastFastDec;
};

// MFEncoder.h