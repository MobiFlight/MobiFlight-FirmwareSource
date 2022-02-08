// MFEEPROM.h
//
/// \mainpage MF MFEEPROM module for MobiFlight Framework
/// \par Revision History
/// \version 1.0 Initial release
// Copyright (C) 2021

#pragma once

class MFEEPROM
{
	  
  public:
    MFEEPROM();
    uint16_t get_length(void);
    void read_block(uint16_t addr, char data[], uint16_t len);
    void write_block (uint16_t addr, char data[], uint16_t len);
    char read_char(uint16_t adr);
    void write_byte (uint16_t adr, char data);

private:
    uint16_t eepromLength = 0;

};
