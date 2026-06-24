#include "MFShiftData.h"

uint8_t shiftInData(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder)
{
    uint8_t value = 0;
    uint8_t i;

    for (i = 0; i < 8; ++i) {
        if (bitOrder == LSBFIRST)
            value |= digitalRead(dataPin) << i;
        else
            value |= digitalRead(dataPin) << (7 - i);
        digitalWrite(clockPin, HIGH);
        digitalWrite(clockPin, LOW);
#if !defined(ARDUINO_ARCH_AVR) && !defined(ARDUINO_ARCH_RP2040)
        delayMicroseconds(1);
#endif
    }
    return value;
}

void shiftOutData(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t val)
{
    uint8_t i;

    for (i = 0; i < 8; i++)  {
        if (bitOrder == LSBFIRST) {
            digitalWrite(dataPin, val & 1);
            val >>= 1;
        } else {    
            digitalWrite(dataPin, (val & 128) != 0);
            val <<= 1;
        }
            
        digitalWrite(clockPin, HIGH);
        digitalWrite(clockPin, LOW);
#if !defined(ARDUINO_ARCH_AVR) && !defined(ARDUINO_ARCH_RP2040)
        delayMicroseconds(1);
#endif
    }
}
