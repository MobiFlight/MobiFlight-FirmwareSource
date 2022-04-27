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
    uint16_t get_length(void);
    bool     read_block(uint16_t addr, char data[], uint16_t len);
    bool     write_block(uint16_t addr, char data[], uint16_t len);
    char     read_char(uint16_t adr);
    bool     write_byte(uint16_t adr, char data);

    bool     read_block(char data[], uint16_t len) { return (read_block(_pos, data, len) ? (_pos += len, true) : false); }
    bool     write_block(char data[], uint16_t len) { return (write_block(_pos, data, len) ? (_pos += len, true) : false); }
    char     read_char(void) { return (read_char(_pos) ? (_pos++, true) : false); }
    bool     write_byte(char data) { return (write_byte(_pos, data) ? (_pos++, true) : false); }

    bool     setPosition(uint16_t pos = 0);

private:
    uint16_t _pos;
};

// MFEEPROM.h