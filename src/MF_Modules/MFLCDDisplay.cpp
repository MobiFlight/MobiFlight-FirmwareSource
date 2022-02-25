// MFSegments.cpp
//
// Copyright (C) 2013-2014

#include "MFLCDDisplay.h"
#include "allocateMem.h"
#include "mobiflight.h"

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
  if (!FitInMemory(sizeof(LiquidCrystal_I2C)))
	{
		// Error Message to Connector
    cmdMessenger.sendCmdStart(kDebug);
    cmdMessenger.sendCmdArg(F("LCD does not fit in Memory!"));
    cmdMessenger.sendCmdEnd();
		return;
	}
  _lcdDisplay = new (allocateMemory(sizeof(LiquidCrystal_I2C))) LiquidCrystal_I2C;
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
    preLines = (_lines / 2) - 1;    // floor needs much Flash and for integer it's the same
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
  if (_cols > strlen(str))
  {
    for (byte c = 0; c != ((_cols - strlen(str)) / 2); c++)
    {
      _lcdDisplay->print(F(" "));
    }
    _lcdDisplay->print(str);
    for (byte c = 0; c != ((_cols - strlen(str)) / 2); c++)
    {
      _lcdDisplay->print(F(" "));
    }
  } else
  {
    _lcdDisplay->print(str);
  }
}