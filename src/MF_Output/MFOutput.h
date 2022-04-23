//
// MFOutput.h
//
// (C) MobiFlight Project 2022
//

#pragma once

#include <Arduino.h>
#include "config.h"

class MFOutput  
{
public:
    static constexpr uint8_t  getType(void) { return kTypeOutput; }

    MFOutput(void);
    
    void    attach(uint8_t pin);
    void    detach(void);
    void    reset(uint8_t action);
    void    setval(uint8_t value);
    void    update(void) {};  // Stub Required for emulated polymorphism
    void    powerSave(uint8_t state);

private:
    uint8_t       _pin;
    uint8_t       _value;
};

// MFOutput.h
