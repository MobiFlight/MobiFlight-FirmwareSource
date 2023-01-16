//
// Stepper.h
//
// (C) MobiFlight Project 2022
//

#pragma once

namespace Stepper
{
    uint8_t Add(uint8_t pin1, uint8_t pin2, uint8_t pin3, uint8_t pin4, uint8_t btnPin1, uint8_t mode = 0, int8_t backlash = 0, bool deactivateOutput = false);
    void    Clear();
    void    OnSet();
    void    OnSetRelative();
    void    SetRelative(uint8_t _stepper, int16_t _pos);
    void    setMaxSpeed(uint8_t _stepper, uint16_t _maxspeed);
    void    setAcceleration(uint8_t _stepper, uint16_t _acceleration);
    void    OnReset();
    void    OnSetZero();
    void    update();
    void    OnSetSpeedAccel();
    void    PowerSave(bool state);
}

// Stepper.h