#include <Arduino.h>
#include "MFEEPROM.h"
#include <EEPROM.h>


void eeprom_init(void) {
    return;
}

void eeprom_read_block(uint16_t adr, char data[], uint16_t len) {
    for (uint16_t i = 0; i<len; i++) {
        EEPROM.get(adr + i,data[i]);
    }
}

void eeprom_write_block (uint16_t adr, char data[], uint16_t len) {
    for (uint16_t i = 0; i<len; i++) {
        EEPROM.put(adr + i,data[i]);
    }
}

char eeprom_read_char(uint16_t addr) {
    return EEPROM.read(addr);
}

void eeprom_write_char (uint16_t addr, char data) {
    EEPROM.put(addr, data);
}
