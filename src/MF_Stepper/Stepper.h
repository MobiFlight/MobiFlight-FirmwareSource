//
// Stepper.h
//
// (C) MobiFlight Project 2022
//

#pragma once

namespace Stepper
{
    void Add(int pin1, int pin2, int pin3, int pin4, int btnPin1);
    void Clear();
    void OnSet();
    void OnSetRelative();
    void SetRelative(uint8_t _stepper, int16_t _pos);
    void setMaxSpeed(uint8_t _stepper, uint16_t _maxspeed);
    void setAcceleration(uint8_t _stepper, uint16_t _acceleration);
    void OnReset();
    void OnSetZero();
    void update();
}

// Stepper.h