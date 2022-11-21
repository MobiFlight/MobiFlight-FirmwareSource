// 
// Stepper.h
// 
// (C) MobiFlight Project 2022
//

#pragma once

namespace Stepper
{
    void Add(uint8_t pin1, uint8_t pin2, uint8_t pin3, uint8_t pin4, uint8_t btnPin1, uint8_t type = 0, int8_t backlash = 0, bool deactivateOutput = false);
    // type == 0 -> FULL4WIRE
    // type == 1 -> HALF4WIRE
    // type == 2 -> DRIVER, only pin1 and pin2 needed
    // backlash  -> number of steps which will be added after changing direction
    // deactivateOutput -> if true the stepper outputs will be deactivated
    void Clear();
    void OnSet();
    void OnReset();
    void OnSetZero();
    void update();
}

// Stepper.h