// MFSegments.cpp
//
// Copyright (C) 2013-2014

#include "MFSegments.h"

MFSegments::MFSegments()
{
  _initialized = false;
}

void MFSegments::display(byte module, char *string, byte points, byte mask, bool convertPoints)
{
  if (!_initialized)
    return;

  String str = String(string);
  byte digit = 8;
  byte pos = 0;
  for (int i = 0; i != 8; i++)
  {
    digit--;
    if (((1 << digit) & mask) == 0)
      continue;
#ifdef USE_POINTER
    _ledControl->setChar(module, digit, str.charAt(pos), ((1 << digit) & points));
#else
    _ledControl.setChar(module, digit, str.charAt(pos), ((1 << digit) & points));
#endif
    pos++;
  }
}

void MFSegments::setBrightness(byte module, byte value)
{
  if (!_initialized)
    return;
  if (module < _moduleCount)
  {
#ifdef USE_POINTER
    _ledControl->setIntensity(module, value);
#else
    _ledControl.setIntensity(module, value);
#endif
  }
}

void MFSegments::attach(int dataPin, int csPin, int clkPin, int moduleCount, byte brightness)
{
#ifdef USE_POINTER
  _ledControl = new LedControl();
  _ledControl->begin(dataPin, clkPin, csPin, moduleCount);
#else
  _ledControl.begin(dataPin, clkPin, csPin, moduleCount);
#endif
  _initialized = true;
  _moduleCount = moduleCount;
  for (int i = 0; i != _moduleCount; ++i)
  {
    setBrightness(i, brightness);
#ifdef USE_POINTER
    _ledControl->shutdown(i, false);
    _ledControl->clearDisplay(i);
#else
    _ledControl.shutdown(i, false);
    _ledControl.clearDisplay(i);
#endif
  }
}

void MFSegments::detach()
{
  if (!_initialized)
    return;
#ifdef USE_POINTER
  delete _ledControl;
#endif
  _initialized = false;
}

void MFSegments::powerSavingMode(bool state)
{
  for (byte i = 0; i != _moduleCount; ++i)
  {
#ifdef USE_POINTER
    _ledControl->shutdown(i, state);
#else
    _ledControl.shutdown(i, state);
#endif
  }
}

void MFSegments::test()
{
  if (!_initialized)
    return;
  byte _delay = 10;
  byte module = 0;
  byte digit = 0;

  for (digit = 0; digit < 8; digit++)
  {
    for (module = 0; module != _moduleCount; ++module)
    {
#ifdef USE_POINTER
      _ledControl->setDigit(module, digit, 8, 1);
#else
      _ledControl.setDigit(module, digit, 8, 1);
#endif
    }
    delay(_delay);
  }

  for (digit = 0; digit < 8; digit++)
  {
    for (module = 0; module != _moduleCount; ++module)
    {
#ifdef USE_POINTER
      _ledControl->setChar(module, 7 - digit, '-', false);
#else
      _ledControl.setChar(module, 7 - digit, '-', false);
#endif
    }
    delay(_delay);
  }

  for (digit = 0; digit < 8; digit++)
  {
    for (module = 0; module != _moduleCount; ++module)
    {
#ifdef USE_POINTER
      _ledControl->setChar(module, 7 - digit, ' ', false);
#else
      _ledControl.setChar(module, 7 - digit, ' ', false);
#endif
    }
    delay(_delay);
  }
}
