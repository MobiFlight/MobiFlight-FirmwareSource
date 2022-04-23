//
// MFEncoder.h
//
// (C) MobiFlight Project 2022
//
// based on the RotaryEncoder library by Matthias Hertel (c) 2014
//

#pragma once

#include <Arduino.h>
#include "config.h"

extern "C"
{
  typedef void (*encoderEventHandler) (uint8_t, uint8_t, const char *);
};

// this prevents the internal position overflow.
#define MF_ENC_MAX 8000 

// this defines the delta value limit for triggering onFast
#define MF_ENC_FAST_LIMIT 50 // 50

class MFEncoder  {
public:
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
        // Example: An encoder with 1 detent per quadrature cycle has a useful resolution of
        // 1/4 of the number of pulses so we can apply a simple bit shift of 2 to 
        // determine the effective position of the encoder.
        uint8_t resolutionShift;
    } encoderType;

    static constexpr uint8_t  getType(void) { return kTypeEncoder; }
    static void     attachHandler(encoderEventHandler newHandler) { _handler = newHandler; }

    MFEncoder();
    void    attach(uint8_t pin1, uint8_t pin2, uint8_t TypeEncoder, const char * name = "Encoder");
    void    detach(void) {};  // Stub required for emulated polymorphism
    void    reset(uint8_t action) { (void)action; };
    void    update();
    void    powerSave(uint8_t){}; // dummy stub - see IODevice.h

    void    tick(void);     // call this function every some milliseconds or by using an interrupt for handling state changes of the rotary encoder.
    int16_t getPosition();                      // retrieve the current position
    void    setPosition(int16_t newPosition);   // adjust the current position
    
private:
    static encoderEventHandler  _handler;
    static const int8_t         KnobDir[];
    static const encoderType    EncoderTypes[];

    uint8_t                   _pin1;              
    uint8_t                   _pin2;
    bool                      _initialized;
    const char *              _name;
    int16_t                   _pos;
    uint8_t                   _TypeEncoder;
    uint8_t                   _detentCounter;
    uint8_t                   _encoderType;
    uint8_t                   _oldState;
    int16_t                   _position;            // Internal position (4 times _positionExt)
    int16_t                   _positionExt;         // External position
    uint32_t                  _positionTime;        // time last position change was detected
    uint32_t                  _positionTimePrev;    // time previous position change was detected
    uint32_t                  _lastFastDec;

};
// MFEncoder.h