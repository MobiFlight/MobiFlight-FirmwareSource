// 
// MFAnalog.h
//
// (C) MobiFlight Project 2022
// 

#pragma once

#include "config.h"
#include <Arduino.h>

#define ADC_MAX_AVERAGE      8 // must be 2^n
#define ADC_MAX_AVERAGE_LOG2 3 // please calculate LOG2(ADC_MAX_AVERAGE)

extern "C" {
// callback functions
typedef void (*analogEventHandler)(int, uint8_t, const char *);
};

class MFAnalog
{
public:
    static constexpr uint8_t getType(void) { return kTypeAnalogInput; }
    static void              attachHandler(analogEventHandler newHandler) { _handler = newHandler; }

    MFAnalog(void){};

    void attach(uint8_t pin, uint8_t sensitivity = 2, const char *name = "Analog Input");
    void detach(void){}; // Stub required for emulated polymorphism
    void reset(uint8_t action) { (void)action; };
    void update();
    void updateAverage();
    void powerSave(uint8_t){}; // dummy stub - see IODevice.h

private:
    static analogEventHandler _handler;

    const char               *_name;
    uint8_t                   _pin;

    int                       _lastValue;
    uint8_t                   _sensitivity;

    uint16_t                  ADC_Buffer[ADC_MAX_AVERAGE] = {0}; // Buffer for all values from each channel
    uint16_t                  ADC_Average_Total           = 0;   // sum of sampled values, must be divided by ADC_MAX_AVERAGE to get actual value
    volatile uint8_t          ADC_Average_Pointer         = 0;   // points to the actual position in ADC_BUFFER
    uint32_t                  _lastReadBuffer;
};
