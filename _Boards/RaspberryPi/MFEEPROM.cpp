#include <Arduino.h>
#include "MFBoards.h"
#include "MFEEPROM.h"
#include <EEPROM.h>

MFEEPROM::MFEEPROM() {}

void MFEEPROM::get_length(void)
{
  eepromLength = EEPROM.length();
}

void MFEEPROM::read_block(uint16_t adr, char data[], uint16_t len)
{
  for (uint16_t i = 0; i < len; i++)
  {
    data[i] = read_char(adr + i);
  }
}

void MFEEPROM::init()
{
  EEPROM.begin(EEPROM_SIZE);
  eepromLength = EEPROM.length();
}

void MFEEPROM::write_block(uint16_t adr, char data[], uint16_t len)
{
  if (adr + len >= eepromLength)
    return;
  for (uint16_t i = 0; i < len; i++)
  {
    EEPROM.put(adr + i, data[i]);
  }
  EEPROM.commit();
}

char MFEEPROM::read_char(uint16_t adr)
{
  if (adr >= eepromLength)
    return 0;
  return EEPROM.read(adr);
}

void MFEEPROM::write_byte(uint16_t adr, char data)
{
  if (adr >= eepromLength)
    return;
  EEPROM.put(adr, data);
  EEPROM.commit();
}