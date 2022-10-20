//
// Servos.h
//
// (C) MobiFlight Project 2022
//

#pragma once

namespace Servos
{
    void    Add(int pin);
    void    Clear();
    void    OnSet();
    int16_t getActualValue(uint8_t channel);
    void    update();
}

// Servos.h