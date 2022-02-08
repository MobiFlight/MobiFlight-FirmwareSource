#pragma once

namespace InputShifter
{
void Add(uint8_t latchPin, uint8_t clockPin, uint8_t dataPin, uint8_t modules, char const *name);
void Clear();
void read();
void OnTrigger();
}
