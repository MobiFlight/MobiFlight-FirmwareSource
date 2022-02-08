// MFSegments.h
//
/// \mainpage MF Button module for MobiFlight Framework
/// \par Revision History
/// \version 1.0 Initial release
/// \author  Sebastian Moebius (mobiflight@moebiuz.de) DO NOT CONTACT THE AUTHOR DIRECTLY: USE THE LISTS
// Copyright (C) 2013-2014 Sebastian Moebius

#pragma once

#include <Arduino.h>
#include <LedControl.h>

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
    LedControl  _ledControl;
    byte        _moduleCount;
};
