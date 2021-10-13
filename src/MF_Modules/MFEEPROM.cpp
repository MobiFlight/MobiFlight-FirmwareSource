#include <Arduino.h>
#include "MFEEPROM.h"
#include <EEPROM.h>

uint16_t eepromLength = 0;

void eeprom_init(void) {
    eepromLength = EEPROM.length();
    return;
}

void eeprom_read_block(uint16_t adr, char data[], uint16_t len) {
    if (adr + len >= eepromLength) return;
    for (uint16_t i = 0; i<len; i++) {
        EEPROM.get(adr + i,data[i]);
    }
}

void eeprom_write_block (uint16_t adr, char data[], uint16_t len) {
    if (adr + len >= eepromLength) return;
    for (uint16_t i = 0; i<len; i++) {
        EEPROM.put(adr + i,data[i]);
    }
}

char eeprom_read_char(uint16_t adr) {
    if (adr >= eepromLength) return 0;
    return EEPROM.read(adr);
}

void eeprom_write_char (uint16_t adr, char data) {
    if (adr >= eepromLength) return;
    EEPROM.put(adr, data);
}
