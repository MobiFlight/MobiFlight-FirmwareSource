#pragma once

#include <Arduino.h>

extern "C" {
// callback functions
typedef void (*CustomDeviceEvent)(uint8_t, const char *);
};

class MFCustomDevice
{
public:
    MFCustomDevice(uint8_t init1, uint8_t init2, uint8_t init3, const char *initParameter, uint16_t init4);
    void        detach();
    static void attachHandler(CustomDeviceEvent handler);
    void        update();
    void        set(uint8_t messageID, char *setPoint);

private:
    static CustomDeviceEvent _handler;
    bool                     _initialized = false;
};
