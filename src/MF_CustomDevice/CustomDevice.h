#pragma once

#if defined(ARDUINO_ARCH_RP2040) && defined(USE_2ND_CORE)
// #########################################################################
// For Communication with Core1
// see https://raspberrypi.github.io/pico-sdk-doxygen/group__multicore__fifo.html
// https://www.raspberrypi.com/documentation/pico-sdk/high_level.html#pico_multicore
// #########################################################################
#define CORE1_CMD        (1 << 31)
#define CORE1_DATA       0

#define CORE1_CMD_STOP   (1 << 0)
#define CORE1_CMD_SEND   (1 << 1)

#define CORE1_DATA_DEVICE      (1 << 30)
#define CORE1_DATA_MESSAGE_ID  (1 << 29)
#define CORE1_DATA_PAYLOAD     (1 << 28)
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