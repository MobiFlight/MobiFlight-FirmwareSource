//
// MFServo.h
//
// (C) MobiFlight Project 2022
//

#pragma once

#include <Arduino.h>
#include <Servo.h>
#include "config.h"

class MFServo  
{
public:
    static constexpr uint8_t  getType(void) { return kTypeServo; }

    MFServo();		

    void    attach(uint8_t pin = 1, bool enable = true);
    void    detach(void);
    void    reset(uint8_t action);
    void    setval(int absolute);
    void    update(void);
    void    powerSave(uint8_t state);

    void    setExternalRange(int min, int max);
    void    setInternalRange(int min, int max);

private:
    uint8_t _pin;
    int		_mapRange[4];
    bool    _initialized;
    Servo   _servo;
    long    _targetPos;
    long    _currentPos;    
    int     speed;
};

// MFServo.h
