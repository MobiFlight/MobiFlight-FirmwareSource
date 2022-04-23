//
// mobiflight.h
//
// (C) MobiFlight Project 2022
// 

#pragma once

#include <Arduino.h>
#include "MFBoards.h"
#include "IODevice.h"
#include "stowManager.h"
#include "commandmessenger.h"

extern CmdMessenger cmdMessenger;
extern StowManager  Stowage;

//=======================================
// Includes for INPUT devices
//=======================================

#ifdef MF_MUX_SUPPORT
#include "MFMuxDriver.h"
extern MFMuxDriver  MUX;
#endif

#include "Button.h"
#include "Encoder.h"
#if MF_ANALOG_SUPPORT == 1
#include "Analog.h"
#endif
#if MF_INPUT_SHIFTER_SUPPORT == 1
#include "InputShifter.h"
#endif
#if MF_DIGIN_MUX_SUPPORT == 1
#include "DigInMux.h"
#endif

//=======================================
// Includes for OUTPUT devices
//=======================================

#include "Output.h"
#if MF_OUTPUT_SHIFTER_SUPPORT == 1
#include "OutputShifter.h"
#endif
#if MF_SEGMENT_SUPPORT == 1
#include "LedSegment.h"
#endif
#if MF_STEPPER_SUPPORT == 1
#include "Stepper.h"
#endif
#if MF_SERVO_SUPPORT == 1
#include "Servos.h"
#endif
#if MF_LCD_SUPPORT == 1
#include "LCDDisplay.h"
#endif


void wipeDevices(void);     //TODO: GCC
void resetDevices(void);    //TODO: GCC

// mobiflight.h
