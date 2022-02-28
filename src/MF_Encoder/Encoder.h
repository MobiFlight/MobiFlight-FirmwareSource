#pragma once

namespace Encoder
{
void Add(uint8_t pin1, uint8_t pin2, uint8_t encoder_type, char const *name);
void Clear();
void read();
}
