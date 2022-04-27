//
// MFEEPROM.cpp
//
// (C) MobiFlight Project 2022
//

// WARNING: This code is based on Arduino / Atmel AVR libraries (although itself not AVR-specific).
// When compiling for other platforms, check that the available libraries are compatible.

#include <Arduino.h>
#include "MFEEPROM.h"
#include <EEPROM.h>

MFEEPROM::MFEEPROM() {}

uint16_t MFEEPROM::get_length(void)
{
    return EEPROM.length();
}

bool MFEEPROM::read_block(uint16_t adr, char data[], uint16_t len)
{
    // If length is exceeded, return only the legitimate part
    for (uint16_t i = 0; i < len && adr < EEPROM.length(); i++, adr++) {
        data[i] = read_char(adr);
    }
    return (adr < EEPROM.length());
}

bool MFEEPROM::write_block(uint16_t adr, char data[], uint16_t len)
{
    // If length is exceeded, do not write anything
    if (adr + len >= EEPROM.length()) return false;
    for (uint16_t i = 0; i < len; i++, adr++) {
        EEPROM.put(adr, data[i]);
    }
    return true;
}

char MFEEPROM::read_char(uint16_t adr)
{
    if (adr >= EEPROM.length()) return 0;
    return EEPROM.read(adr);
}

bool MFEEPROM::write_byte(uint16_t adr, char data)
{
    if (adr >= EEPROM.length()) return false;
    EEPROM.put(adr, data);
    return true;
}

bool MFEEPROM::setPosition(uint16_t pos)
{
    return ((pos < EEPROM.length()) ? ((_pos = pos), true) : false);
}

// MFEEPROM.cpp
