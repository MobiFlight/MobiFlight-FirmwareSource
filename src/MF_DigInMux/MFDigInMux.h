//
// MFDigInMux.h
//
// (C) MobiFlight Project 2022
//

#pragma once

#include <Arduino.h>
#include "MFMuxDriver.h"

extern "C" {
typedef void (*MuxDigInEvent)(byte, uint8_t, const char *);
};

enum {
    MuxDigInOnPress,
    MuxDigInOnRelease,
};

class MFDigInMux
{
public:
    enum { MUX_MODE_FAST = 0,
           MUX_MODE_LAZY = 1,
    };

    MFDigInMux(void);
    MFDigInMux(MFMuxDriver *MUX, const char *name);
    static void setMux(MFMuxDriver *MUX);
    static void attachHandler(MuxDigInEvent newHandler);

    void        attach(uint8_t dataPin, bool halfSize, char const *name);
    void        detach();
    void        clear();
    void        retrigger();
    void        update();
    void        setLazyMode(bool mode);
    uint16_t    getValues(void) { return _lastState; }

private:
    enum { MUX_INITED   = 0,
           MUX_HALFSIZE = 1,
           MUX_LAZY     = 2,
    };

    enum { DONT_TRIGGER = 0,
           DO_TRIGGER   = 1 };

    static MFMuxDriver  *_MUX;
    static MuxDigInEvent _inputHandler;

    const char          *_name;
    uint8_t              _dataPin; // Data pin - MUX common, input to AVR
    uint8_t              _flags;
    uint16_t             _lastState;

    void                 poll(bool detect, bool lazy);
    void                 detectChanges(uint16_t lastState, uint16_t currentState);
    void                 trigger(uint8_t channel, bool state);
};

// MFDigInMux.h