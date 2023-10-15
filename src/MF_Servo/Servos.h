//
// Servos.h
//
// (C) MobiFlight Project 2022
//

#pragma once

namespace Servos
{
    bool setupArray(uint16_t count);
    void Add(int pin);
    void Clear();
    void OnSet();
    void update();
}

// Servos.h