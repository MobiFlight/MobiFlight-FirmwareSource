//
// MFLCDDisplay.cpp
//
// (C) MobiFlight Project 2022
//

#include "MFLCDDisplay.h"
#include "mobiflight.h"

MFLCDDisplay::MFLCDDisplay() : _address(0xFF) 
{}

void MFLCDDisplay::attach(byte I2Caddress, byte cols, byte lines)
{
    if (0xFF == I2Caddress ) return;
    _address = I2Caddress;
    _cols = cols;
    _lines = lines;
    _lcdDisplay.init((uint8_t)I2Caddress, (uint8_t)cols, (uint8_t)lines);
    _lcdDisplay.backlight();
    Wire.setClock(400000);
    test();
}

void MFLCDDisplay::detach()
{
    if (_address != 0xFF) {
        _lcdDisplay.clear();
        _lcdDisplay.noBacklight();
        _address = 0xFF;
    }
}

void MFLCDDisplay::setval(const char *string)
{
    if (0xFF != _address) {
        for (uint8_t line = 0; line != _lines; line++) {
            _lcdDisplay.setCursor(0, line);
            _lcdDisplay.writeString(&string[line * _cols], _cols);
        }
    }
}

void MFLCDDisplay::reset(uint8_t action)
{
    if (ONRESET_DEFAULT == action) {
        test();
    }
}

void MFLCDDisplay::powerSave(uint8_t state)
{
    if (state)
        _lcdDisplay.noBacklight();
    else
        _lcdDisplay.backlight();
}

void MFLCDDisplay::test()
{
    if (0xFF != _address) {
        uint8_t preLines = 0;
        _lcdDisplay.clear();

        if (_lines > 2) {
            preLines = (_lines / 2) - 1; // floor needs much Flash and for integer it's the same
        }

        _printCentered("MF LCD", preLines++);
        if (_lines > 1) {
            _printCentered("Test OK", preLines++);
        }

        _lcdDisplay.setCursor(0, 0);
    }
}

void MFLCDDisplay::_printCentered(const char *str, uint8_t line)
{
  uint8_t startCol = 0;
  uint8_t printChar = _cols;
  
  if (_cols > strlen(str)) {
    startCol = (_cols - strlen(str)) / 2;
    printChar = strlen(str);
  }

  _lcdDisplay.setCursor(startCol, line);
  
  for (uint8_t i = 0; i < printChar; i++) {
    _lcdDisplay.write(str[i]);
  }
}

// MFLCDDisplay.cpp
