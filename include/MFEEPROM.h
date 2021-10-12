#ifndef MFEEPROM_H
#define MFEEPROM_H

void eeprom_init(void);
void eeprom_read_block(uint16_t addr, char data[], uint16_t len);
void eeprom_write_block (uint16_t addr, char data[], uint16_t len);
char eeprom_read_char(uint16_t addr);
void eeprom_write_char (uint16_t addr, char data);


#endif