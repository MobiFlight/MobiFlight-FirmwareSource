#pragma once

namespace Button
{
void Add(uint8_t pin, char const *name);
void Clear();
void read();
void OnTrigger();
}
