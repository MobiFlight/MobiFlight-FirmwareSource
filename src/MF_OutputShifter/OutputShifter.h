#pragma once

namespace OutputShifter
{
void Add(uint8_t latchPin, uint8_t clockPin, uint8_t dataPin, uint8_t modules);
void Clear();
//void OnInit();        // this is defined but not used!?
void OnSet();
}
