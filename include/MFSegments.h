// MFSegments.h
//
/// \mainpage MF Button module for MobiFlight Framework
/// \par Revision History
/// \version 1.0 Initial release
/// \version 1.1 Required functions from LedControl library are included
/// \author  Sebastian Moebius (mobiflight@moebiuz.de) DO NOT CONTACT THE AUTHOR DIRECTLY: USE THE LISTS
// Copyright (C) 2013-2014 Sebastian Moebius

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

#ifndef MFSegments_h
#define MFSegments_h

#if ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#include <wiring.h>
#endif

// #include <LedControl.h>
#include <avr/pgmspace.h>

//the opcodes for the MAX7221 and MAX7219
#define OP_NOOP   0
#define OP_DIGIT0 1
#define OP_DIGIT1 2
#define OP_DIGIT2 3
#define OP_DIGIT3 4
#define OP_DIGIT4 5
#define OP_DIGIT5 6
#define OP_DIGIT6 7
#define OP_DIGIT7 8
#define OP_DECODEMODE  9
#define OP_INTENSITY   10
#define OP_SCANLIMIT   11
#define OP_SHUTDOWN    12
#define OP_DISPLAYTEST 15

/*
 * Segments to be switched on for characters and digits on
 * 7-Segment Displays
 * B01234567  
 *
 *   1
 * 6   2
 *   7
 * 5   3
 *   4
 */
const static byte charTable[128] = {
    B01111110,B00110000,B01101101,B01111001,B00110011,B01011011,B01011111,B01110000,
    B01111111,B01111011,B01110111,B00011111,B00001101,B00111101,B01001111,B01000111,
    B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
    B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
 // (space)  ,!        ,"        ,#        ,$        ,%        ,&        ,'
    B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
 // (        ,)        ,*        ,+        ,,        ,-        ,.        ,/
    B00000000,B00000000,B00000000,B00000000,B10000000,B00000001,B10000000,B00100101,
 // 0        ,1        ,2        ,3        ,4        ,5        ,6        ,7
    B01111110,B00110000,B01101101,B01111001,B00110011,B01011011,B01011111,B01110000,
 // 8        ,9        ,:        ,;        ,<        ,=        ,>        ,?
    B01111111,B01111011,B00000000,B00000000,B00000000,B00001001,B00000000,B00000000,
 // @        ,A        ,B        ,C        ,D        ,E        ,F        ,G  
    B00000000,B01110111,B01111111,B01001110,B01111110,B01001111,B01000111,B01011111,
 // H        ,I        ,J        ,K        ,L        ,M        ,N        ,O
    B00110111,B00110000,B01111100,B00000000,B00001110,B00000000,B01110110,B01111110,
 // P        ,Q        ,R        ,S        ,T        ,U        ,V        ,W
    B01100111,B01111110,B01110111,B01011011,B01000110,B00111110,B00000000,B00000000,
 // X        ,Y        ,Z        ,[        ,\        ,]        ,^        ,_
    B00000000,B00110011,B01101101,B01001110,B00010011,B01111000,B00000000,B00001000,
 // `        ,a        ,b        ,c        ,d        ,e        ,f        ,g
    B00000000,B01111101,B00011111,B00001101,B00111101,B01001111,B01000111,B01111011,
 // h        ,i        ,j        ,k        ,l        ,m        ,n        ,o
    B00110111,B00110000,B00111000,B00000000,B00000110,B00000000,B00010101,B00011101,
 // p        ,q        ,r        ,s        ,t        ,u        ,v        ,w
    B01100111,B01110011,B00000101,B01011011,B00001111,B00011100,B00000000,B00000000,
 // x        ,y        ,z        ,{        ,|        ,}        ,~        ,(delete)
    B00000000,B00100111,B01101101,B01001110,B00000000,B01111000,B00000000,B00000000
};

/////////////////////////////////////////////////////////////////////
/// \class MFSegments MFSegments.h <MFSegments.h>
class MFSegments
{
public:
    MFSegments();
    void display(byte module, char *string, byte points, byte mask, bool convertPoints = false);
    void attach(int dataPin, int csPin, int clkPin, byte moduleCount, byte brightness);
    void detach();
    void test();
    void powerSavingMode(bool state);
    void setBrightness(byte module, byte value);

private:
    void shutdown(int addr, bool b);
    void setScanLimit(int addr, int limit);
    void clearDisplay(int addr);
    void setIntensity(int addr, int intensity);
    void setDigit(int addr, int digit, byte value, boolean dp);
    void setChar(int addr, int digit, char value, boolean dp);
    void spiTransfer(int addr, volatile byte opcode, volatile byte data);
//    LedControl *_ledControl;
    bool _initialized;
    byte _moduleCount;
    /* The array for shifting the data to the devices */
    byte spidata[16];
    /* Data is shifted out of this pin*/
    int SPI_MOSI;
    /* The clock is signaled on this pin */
    int SPI_CLK;
    /* This one is driven LOW for chip selectzion */
    int SPI_CS;
    /* The maximum number of devices we use */
    int maxDevices;
};
#endif

