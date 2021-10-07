// MFSegments.cpp
//
// Copyright (C) 2013-2021
//
/*
The functions to control the MAX7219/7221 are copied from
LedControl.h - A library for controling Leds with a MAX7219/MAX7221
Copyright (c) 2007-2015 Eberhard Fahle

Permission is hereby granted, free of charge, to any person
obtaining a copy of this software and associated documentation
files (the "Software"), to deal in the Software without
restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following
conditions:

This permission notice shall be included in all copies or 
substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.
*/

#include "MFSegments.h"
#include <avr/pgmspace.h>

MFSegments::MFSegments()
{
  _initialized = false;
}

void MFSegments::display(byte module, char *string, byte points, byte mask, bool convertPoints)
{
  if (!_initialized)
    return;

  byte digit = 8;
  byte pos = 0;
  for (int i = 0; i != 8; i++)
  {
    digit--;
    if (((1 << digit) & mask) == 0)
      continue;
    setChar(module, digit, string[pos], ((1 << digit) & points));
    pos++;
  }
}

void MFSegments::setBrightness(byte module, byte value)
{
  if (!_initialized)
    return;
  if (module < _moduleCount)
  {
    setIntensity(module, value);
  }
}

void MFSegments::attach(int dataPin, int csPin, int clkPin, byte moduleCount, byte brightness)
{
  _initialized = true;
  _moduleCount = moduleCount;

  SPI_MOSI=dataPin;
  SPI_CLK=clkPin;
  SPI_CS=csPin;
  if(_moduleCount<=0 || _moduleCount>8 )
      _moduleCount=8;
  maxDevices=_moduleCount;
  pinMode(SPI_MOSI,OUTPUT);
  pinMode(SPI_CLK,OUTPUT);
  pinMode(SPI_CS,OUTPUT);
  digitalWrite(SPI_CS,HIGH);
  SPI_MOSI=dataPin;
  for(int i=0;i<maxDevices;i++) {
      spiTransfer(i,OP_DISPLAYTEST,0);
      //scanlimit is set to max on startup
      setScanLimit(i,7);
      //decode is done in source
      spiTransfer(i,OP_DECODEMODE,0);
      clearDisplay(i);
      //we go into shutdown-mode on startup
      shutdown(i,true);
  }

  for (int i = 0; i != _moduleCount; ++i)
  {
    setBrightness(i, brightness);
    shutdown(i, false);
    clearDisplay(i);
  }
}

void MFSegments::detach()
{
  _initialized = false;
}

void MFSegments::powerSavingMode(bool state)
{
  for (byte i = 0; i != _moduleCount; ++i)
  {
    shutdown(i, state);
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
      setDigit(module, digit, 8, 1);
    }
    delay(_delay);
  }

  for (digit = 0; digit < 8; digit++)
  {
    for (module = 0; module != _moduleCount; ++module)
    {
      setChar(module, 7 - digit, '-', false);
    }
    delay(_delay);
  }

  for (digit = 0; digit < 8; digit++)
  {
    for (module = 0; module != _moduleCount; ++module)
    {
      setChar(module, 7 - digit, ' ', false);
    }
    delay(_delay);
  }
}

// --------------------------------------------------------------------------
void MFSegments::shutdown(int addr, bool b) {
    if(addr<0 || addr>=maxDevices)
        return;
    if(b)
        spiTransfer(addr, OP_SHUTDOWN,0);
    else
        spiTransfer(addr, OP_SHUTDOWN,1);
}

void MFSegments::setScanLimit(int addr, int limit) {
    if(addr<0 || addr>=maxDevices)
        return;
    if(limit>=0 && limit<8)
        spiTransfer(addr, OP_SCANLIMIT,limit);
}

void MFSegments::clearDisplay(int addr) {
    if(addr<0 || addr>=maxDevices)
        return;
    for(int i=0;i<8;i++) {
        spiTransfer(addr, i+1,0);
    }
}

void MFSegments::setIntensity(int addr, int intensity) {
    if(addr<0 || addr>=maxDevices)
        return;
    if(intensity>=0 && intensity<16)	
        spiTransfer(addr, OP_INTENSITY,intensity);
}

void MFSegments::setDigit(int addr, int digit, byte value, boolean dp) {
    byte v;

    if(addr<0 || addr>=maxDevices)
        return;
    if(digit<0 || digit>7 || value>15)
        return;
    v=pgm_read_byte_near(charTable + value); 
    if(dp)
        v|=B10000000;
    spiTransfer(addr, digit+1,v);
}

void MFSegments::setChar(int addr, int digit, char value, boolean dp) {
    byte index,v;

    if(addr<0 || addr>=maxDevices)
        return;
    if(digit<0 || digit>7)
        return;
    index=(byte)value;
    if(index >127) {
        //no defined beyond index 127, so we use the space char
        index=32;
    }
    v=pgm_read_byte_near(charTable + index); 
    if(dp)
        v|=B10000000;
    spiTransfer(addr, digit+1,v);
}

void MFSegments::spiTransfer(int addr, volatile byte opcode, volatile byte data) {
    //Create an array with the data to shift out
    int offset=addr*2;
    int maxbytes=maxDevices*2;

    for(int i=0;i<maxbytes;i++)
        spidata[i]=(byte)0;
    //put our device data into the array
    spidata[offset+1]=opcode;
    spidata[offset]=data;
    //enable the line 
    digitalWrite(SPI_CS,LOW);
    //Now shift out the data 
    for(int i=maxbytes;i>0;i--)
        shiftOut(SPI_MOSI,SPI_CLK,MSBFIRST,spidata[i-1]);
    //latch the data onto the display
    digitalWrite(SPI_CS,HIGH);
}
