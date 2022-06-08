// 
// MFButton.h
//
// (C) MobiFlight Project 2022
//

#pragma once

#include <Arduino.h>
#include "config.h"

extern "C" {
// callback functions always follow the signature: void cmd(void);
typedef void (*buttonEventHandler)(byte, uint8_t, const char *);
};

enum {
    btnOnPress,
    btnOnRelease,
};

class MFButton
{
public:
    static constexpr uint8_t getType(void) { return kTypeButton; }
    static void              attachHandler(buttonEventHandler newHandler) { _handler = newHandler; };

    MFButton(void);

    void attach(uint8_t pin, const char *name = "Button");
    void detach(void){}; // Stub required for emulated polymorphism
    void reset(uint8_t action);
    void update();
    void powerSave(uint8_t){}; // dummy stub - see IODevice.h

    void trigger(uint8_t state); // could be private

private:
    static buttonEventHandler _handler;

    const char               *_name;
    uint8_t                   _pin;
    bool                      _state;
};
// MFButton.h