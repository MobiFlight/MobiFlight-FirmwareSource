#pragma once

#include <Arduino.h>

uint8_t shiftInData(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder);
void shiftOutData(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t val);
