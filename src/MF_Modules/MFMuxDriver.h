//
// MFMuxDriver.h
//
// (C) MobiFlight Project 2022
//

#pragma once

#include <Arduino.h>

extern "C" {
typedef void (*MuxDigInEvent)(byte, uint8_t, uint8_t);
};

class MFMuxDriver
{
public:
    MFMuxDriver(void);
    void    attach(uint8_t Sel0Pin, uint8_t Sel1Pin, uint8_t Sel2Pin, uint8_t Sel3Pin);
    void    detach();

    // void setChannelOpt(uint8_t mode);
    void    setChannel(uint8_t value);
    uint8_t getChannel(void);
    uint8_t nextChannel(void);
    void    saveChannel(void); // Not reentrant - one level only
    void    restoreChannel(void);

private:
    enum { MUX_INITED = 0,
    };

    uint8_t _selPin[4]; // Selector pins; 0 is LSb
    uint8_t _flags;
    uint8_t _channel;
    uint8_t _savedChannel;
};

// MFMuxDriver.h
