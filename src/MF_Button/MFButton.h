//
// MFButton.h
//
// (C) MobiFlight Project 2022
//

#pragma once

#include <Arduino.h>

extern "C" {
// callback functions always follow the signature: void cmd(void);
typedef void (*buttonEvent)(uint8_t, const char *);
};

enum {
    btnOnPress,
    btnOnRelease,
};

/////////////////////////////////////////////////////////////////////
/// \class MFButton MFButton.h <MFButton.h>
class MFButton
{
public:
    MFButton();
    static void attachHandler(buttonEvent newHandler);
    void        attach(uint8_t pin, const char *name);
    void        update();
    void        trigger(uint8_t state);
    void        triggerOnPress();
    void        triggerOnRelease();
    const char *_name;
    uint8_t     _pin;

private:
    static buttonEvent _handler;
    bool               _state;
    bool               _initialized;
};

// MFButton.h