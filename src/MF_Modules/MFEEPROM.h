//
// MFEEPROM.h
//
// (C) MobiFlight Project 2022
//

#pragma once

#include <stdint.h>

class MFEEPROM
{

public:
    MFEEPROM();
    void     init(void);
    uint16_t get_length(void);
    bool     read_block(uint16_t addr, char data[], uint16_t len);
    bool     write_block(uint16_t addr, char data[], uint16_t len);
    char     read_char(uint16_t adr);
    bool     write_byte(uint16_t adr, char data);

private:
    uint16_t _eepromLength = 0;
};

// MFEEPROM.h