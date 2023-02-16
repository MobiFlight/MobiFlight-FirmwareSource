//
// MFButton.h
//
// (C) MobiFlight Project 2022
//

#pragma once

#include <Arduino.h>
#ifdef USE_FAST_IO
#include "MFFastIO.h"
#endif

extern "C" {
// callback functions always follow the signature: void cmd(void);
typedef void (*buttonEvent)(byte, uint8_t, const char *);
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
    MFButton(uint8_t pin = 1, const char *name = "Button");
    static void attachHandler(buttonEvent newHandler);
    void        update();
    void        trigger(uint8_t state);
    void        triggerOnPress();
    void        triggerOnRelease();

private:
    const char *_name;
#ifdef USE_FAST_IO
    volatile FASTIO_Port_t *_pinPort;
    FASTIO_Port_t _pinMask;
#endif
    uint8_t     _pin;
    static buttonEvent _handler;
    bool               _state;
};

// MFButton.h