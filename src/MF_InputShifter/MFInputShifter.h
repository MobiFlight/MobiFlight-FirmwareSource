//
// MFInputShifter.h
//
// (C) MobiFlight Project 2022
//

#pragma once

#include <Arduino.h>

// Maximum number of shifters allowed on an individual chain. While this is currently set to 4
// there is no technical limit in the code for how many can be chained. It is constrained only
// by available memory (one byte required per chip) and the time it takes to read all the bits in.
#define MAX_CHAINED_INPUT_SHIFTERS 4

extern "C" {
typedef void (*inputShifterEvent)(byte, uint8_t, const char *);
};

enum {
    inputShifterOnPress,
    inputShifterOnRelease,
};

class MFInputShifter
{
public:
    MFInputShifter(const char *name = "InputShifter");
    void        attach(uint8_t latchPin, uint8_t clockPin, uint8_t dataPin, uint8_t moduleCount, const char *name);
    static void attachHandler(inputShifterEvent newHandler);
    void        clear();
    void        detach();
    void        retrigger();
    void        update();

private:
    enum { DONT_TRIGGER = 0,
           DO_TRIGGER   = 1 };

    const char       *_name;
    volatile uint8_t *_latchPinPort;
    uint8_t           _latchPinMask;
    volatile uint8_t *_clockPinPort;
    uint8_t           _clockPinMask;
    volatile uint8_t *_dataPinPort;
    uint8_t           _dataPinMask;
    uint8_t           _moduleCount; // Number of 8 bit modules in series.
    bool              _initialized                           = false;
    uint8_t           _lastState[MAX_CHAINED_INPUT_SHIFTERS] = {0};

    void                     poll(uint8_t doTrigger);
    void                     detectChanges(uint8_t lastState, uint8_t currentState, uint8_t module);
    void                     trigger(uint8_t pin, bool state);
    inline void              Pin_HIGH(volatile uint8_t *PinPort, uint8_t PinMask);
    inline void              Pin_LOW(volatile uint8_t *PinPort, uint8_t PinMask);
    inline void              latchPin_HIGH(void);
    inline void              latchPin_LOW(void);
    inline void              clockPin_HIGH(void);
    inline void              clockPin_LOW(void);
    inline uint8_t           dataPin_READ(void);
    static inputShifterEvent _inputHandler;
};

// MFInputShifter.h
