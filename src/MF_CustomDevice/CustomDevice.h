#pragma once

#if defined(ARDUINO_ARCH_RP2040) && defined(USE_2ND_CORE)
// #########################################################################
// For Communication with Core1
// https://www.raspberrypi.com/documentation/pico-sdk/high_level.html#pico_multicore
// #########################################################################
#define CORE1_CMD_SEND   (1 << 0)
#endif

namespace CustomDevice
{
    bool setupArray(uint16_t count);
    void Add(uint16_t adrPin, uint16_t adrType, uint16_t adrConfig);
    void Clear();
    void update();
    void OnSet();
    void PowerSave(bool state);
#if defined(USE_2ND_CORE)
    void init_2ndCore();
#endif
}