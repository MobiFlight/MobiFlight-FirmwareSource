//
// MFMuxDriver.h
//
// (C) MobiFlight Project 2022
//

#pragma once

#include <Arduino.h>
#ifdef USE_FAST_IO
#include "MFFastIO.h"
#endif

extern "C" {
typedef void (*MuxDigInEvent)(byte, uint8_t, const char *);
};

class MFMuxDriver
{
public:
    MFMuxDriver(void);
    void attach(uint8_t Sel0Pin, uint8_t Sel1Pin, uint8_t Sel2Pin, uint8_t Sel3Pin);
    void detach();

    // void setChannelOpt(uint8_t mode);
    void    setChannel(uint8_t value);
    uint8_t getChannel(void);
    uint8_t nextChannel(void);
    void    saveChannel(void); // Not reentrant - one level only
    void    restoreChannel(void);

private:
    enum { MUX_INITED = 0,
    };

#ifdef USE_FAST_IO
    volatile FASTIO_Port_t *_selPinPort[4]; // Selector pin ports and mask; 0 is LSb
    FASTIO_Port_t           _selPinMask[4];
#endif
    uint8_t _selPin[4]; // Selector pins; 0 is LSb
    uint8_t _flags;
    uint8_t _channel;
    uint8_t _savedChannel;
};

// MFMuxDriver.h
