// MFSegments.cpp
//
// Copyright (C) 2013-2014

#include "MFLCDDisplay.h"

MFLCDDisplay::MFLCDDisplay()
{
  _initialized = false;
}

void MFLCDDisplay::display(const char *string)
{
  if (!_initialized)
    return;
  for (uint8_t line = 0; line != _lines; line++)
  {
    _lcdDisplay->setCursor(0, line);
    _lcdDisplay->writeString(&string[line*_cols], _cols);
  }
}

void MFLCDDisplay::attach(byte address, byte cols, byte lines)
{
  _address = address;
  _cols = cols;
  _lines = lines;
  _lcdDisplay = new LiquidCrystal_I2C();
  _initialized = true;
  _lcdDisplay->init((uint8_t)address, (uint8_t)cols, (uint8_t)lines);
  _lcdDisplay->backlight();
  Wire.setClock(400000);
  test();
}

void MFLCDDisplay::detach()
{
  if (!_initialized)
    return;
  delete _lcdDisplay;
  _initialized = false;
}

void MFLCDDisplay::powerSavingMode(bool state)
{
  if (state)
    _lcdDisplay->noBacklight();
  else
    _lcdDisplay->backlight();
}

void MFLCDDisplay::test()
{
  if (!_initialized)
    return;
  uint8_t preLines = 0;
  _lcdDisplay->clear();

  if (_lines > 2)
  {
    preLines = floor(_lines / 2) - 1;
  }

  _printCentered("MF wishes", preLines++);
  if (_lines > 1)
  {
    _printCentered("Happy Holidays!!", preLines++);
  }

  _lcdDisplay->setCursor(0, 0);
}

void MFLCDDisplay::_printCentered(const char *str, uint8_t line)
{
  _lcdDisplay->setCursor(0, line);
  for (byte c = 0; c != ((_cols - strlen(str)) / 2); c++)
  {
    _lcdDisplay->print(F(" "));
  }
  _lcdDisplay->print(str);
  for (byte c = 0; c != ((_cols - strlen(str)) / 2); c++)
  {
    _lcdDisplay->print(F(" "));
  }
}