// MFSegments.h
//
/// \mainpage MF LED Segment module for MobiFlight Framework
/// \par Revision History
/// \version 1.0 Initial release
/// \author  Sebastian Moebius (mobiflight@moebiuz.de) DO NOT CONTACT THE AUTHOR DIRECTLY: USE THE LISTS
// Copyright (C) 2013-2014 Sebastian Moebius

#ifndef MFSegments_h
#define MFSegments_h

#if ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#include <wiring.h>
#endif

#include <MFBoards.h>

#if MF_SEGMENT_SUPPORT == 1
#include <LedControl.h>
#include <ht16k33.h>
#include <TM1637Display.h>
#include "asciifont-pinout12.h"
#endif

/* An enumeration of the supported types of LED digit displays */
enum eSegmentType
{
    kSegmentType_undefined = 0,
    kSegmentType_7seg = 7,
    kSegmentType_14seg = 14,
    kSegmentType_16seg = 16,
    kSegmentType_16x8matrix = 128,
};

/* An enumeration of the supported multi-segment and dot-matrix drivers */
enum eSegmentDriver
{
    kSegmentDriver_undefined = 0,
    kSegmentDriver_MAX1719, // SPI
    kSegmentDriver_MAX1721, // SPI
    kSegmentDriver_HT16K33, // I2C
    kSegmentDriver_TM1637   // pseudo I2C
};

/////////////////////////////////////////////////////////////////////
/// \class MFSegments MFSegments.h <MFSegments.h>
class MFSegments
{
public:
    MFSegments();
    void display(byte module, char *string, byte points, byte mask, bool convertPoints = false);
    void attach(int dataPin, int csPin, int clkPin, byte moduleCount, byte brightness, byte segmentType, byte segmentDriver);
    void detach();
    void test();
    void powerSavingMode(bool state);
    void setBrightness(byte module, byte value);

private:
    LedControl *_ledControl;    // Maxim MAX7219/7221 7 segment driver (modules chain together)
    HT16K33 *_ht16k33[HT16K33_MAX_DEVICES];   // Holtek HT16K33 16x8 segment drivers (modules have different i2c addresses)
    TM1637Display *_tm1637;
    bool _initialized;
    byte _moduleCount;
    eSegmentType _segmentType = kSegmentType_undefined;
    eSegmentDriver _segmentDriver = kSegmentDriver_undefined;
};
#endif