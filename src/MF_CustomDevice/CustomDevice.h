#pragma once

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