//
// Servos.h
//
// (C) MobiFlight Project 2022
//

#pragma once

namespace Servos
{
    void setupArray(uint16_t count);
    void Add(int pin);
    void Clear();
    void OnSet();
    void update();
}

// Servos.h