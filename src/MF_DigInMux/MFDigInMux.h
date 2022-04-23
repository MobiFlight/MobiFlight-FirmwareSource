//
// MFDigInMux.h
//
// (C) MobiFlight Project 2022
//

#pragma once

#include "IOdevice.h"
#include "MFMuxDriver.h"
#include <Arduino.h>

extern "C" {
typedef void (*MuxDigInEvent)(byte, uint8_t, const char *);
};

class MFDigInMux
{
public:
    enum {
        MuxDigInOnPress,
        MuxDigInOnRelease,
    };

    enum {
        MuxModeFast = 0,
        MuxModeLazy = 1,
    };

    static constexpr uint8_t getType(void) { return kTypeDigInMux; }
    static void              attachHandler(MuxDigInEvent newHandler) { _handler = newHandler; }
    static void              setMux(MFMuxDriver *MUX);

    MFDigInMux(void);

    void     attach(uint8_t dataPin, bool halfSize, char const *name);
    void     detach(void);
    void     reset(uint8_t action);
    void     update(void);
    void     powerSave(uint8_t){}; // dummy stub - see IODevice.h

    void     setLazyMode(bool mode);
    uint16_t getValues(void) { return _lastState; }

private:
    enum {
        MUX_INITED   = 0,
        MUX_HALFSIZE = 1,
        MUX_LAZY     = 2,
    };

    static MFMuxDriver  *_MUX;
    static MuxDigInEvent _handler;

    const char          *_name;
    uint8_t              _dataPin; // Data pin - MUX common, input to AVR
    uint8_t              _flags;
    uint16_t             _lastState;

    void                 poll(bool detect, bool lazy);
    void                 detectChanges(uint16_t lastState, uint16_t currentState);
    void                 trigger(uint8_t channel, bool state);
};
// MFDigInMux.h
