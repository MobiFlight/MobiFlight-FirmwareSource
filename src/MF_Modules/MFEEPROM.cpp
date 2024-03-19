//
// MFEEPROM.cpp
//
// (C) MobiFlight Project 2022
//

#include <Arduino.h>
#include "MFEEPROM.h"
#include "MFBoards.h"

MFEEPROM::MFEEPROM() {}

void MFEEPROM::init(void)
{
#if defined(ARDUINO_ARCH_RP2040)
    EEPROM.begin(4096);
#endif
    _eepromLength = EEPROM.length();
}

uint16_t MFEEPROM::get_length(void)
{
    return _eepromLength;
}

uint8_t MFEEPROM::read_byte(uint16_t adr)
{
    if (adr >= _eepromLength) return 0;
    return EEPROM.read(adr);
}

bool MFEEPROM::write_byte(uint16_t adr, const uint8_t data)
{
    if (adr >= _eepromLength) return false;
    EEPROM.write(adr, data);
#if defined(ARDUINO_ARCH_RP2040) && defined(USE_2ND_CORE)
    // #########################################################################
    // Communication with Core1
    // see https://raspberrypi.github.io/pico-sdk-doxygen/group__multicore__fifo.html
    // #########################################################################
    multicore_fifo_push_blocking(CORE1_CMD_STOP);
    multicore_lockout_start_blocking();
#endif
#if defined(ARDUINO_ARCH_RP2040)
        EEPROM.commit();
#endif
#if defined(ARDUINO_ARCH_RP2040) && defined(USE_2ND_CORE)
    multicore_lockout_end_blocking();
#endif
    return true;
}

// MFEEPROM.cpp