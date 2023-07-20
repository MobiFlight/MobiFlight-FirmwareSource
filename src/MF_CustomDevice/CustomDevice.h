#pragma once

namespace CustomDevice
{
    void Add(uint8_t Pin1, uint8_t Pin2, uint8_t Pin3, uint8_t Pin4, uint8_t Pin5, uint8_t Pin6, char *customName, char *configuration);
    void Clear();
    void update();
    void OnSet();
}
