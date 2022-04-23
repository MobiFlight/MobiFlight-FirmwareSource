//
// commandMessenger.cpp
//
// (C) MobiFlight Project 2022
//

#include <Arduino.h>
#include "commandmessenger.h"
#include "config.h"
#include "mobiflight.h"


CmdMessenger cmdMessenger = CmdMessenger(Serial);
unsigned long lastCommand;

// Prototypes for forward declarations
static void OnTrigger();
static void OnUnknownCommand();

// Callbacks define on which received commands we take action
void attachCommandCallbacks()
{
    // Attach callback methods
    cmdMessenger.attach(OnUnknownCommand);

#if MF_SEGMENT_SUPPORT == 1
    cmdMessenger.attach(kInitModule, LedSegment::OnInit);
    cmdMessenger.attach(kSetModule, LedSegment::OnSet);
    cmdMessenger.attach(kSetModuleBrightness, LedSegment::OnSetBrightness);
#endif

    cmdMessenger.attach(kSetPin, Output::OnSet);

#if MF_STEPPER_SUPPORT == 1
    cmdMessenger.attach(kSetStepper, Stepper::OnSet);
    cmdMessenger.attach(kResetStepper, Stepper::OnReset);
    cmdMessenger.attach(kSetZeroStepper, Stepper::OnSetZero);
#endif

#if MF_SERVO_SUPPORT == 1
    cmdMessenger.attach(kSetServo, Servos::OnSet);
#endif

  cmdMessenger.attach(kGetInfo, OnGetInfo);
  cmdMessenger.attach(kGetConfig, OnGetConfig);
  cmdMessenger.attach(kSetConfig, OnSetConfig);
  cmdMessenger.attach(kResetConfig, OnResetConfig);
  cmdMessenger.attach(kSaveConfig, OnSaveConfig);
  cmdMessenger.attach(kActivateConfig, OnActivateConfig);
  cmdMessenger.attach(kSetName, OnSetName);
  cmdMessenger.attach(kGenNewSerial, OnGenNewSerial);
  cmdMessenger.attach(kTrigger, OnTrigger);

#if MF_LCD_SUPPORT == 1
  cmdMessenger.attach(kSetLcdDisplayI2C, LCDDisplay::OnSet);
#endif

#if MF_OUTPUT_SHIFTER_SUPPORT
  cmdMessenger.attach(kSetShiftRegisterPins, OutputShifter::OnSet);
#endif

#ifdef DEBUG2MSG
    cmdMessenger.sendCmd(kStatus, F("Attached callbacks"));
#endif
}

// Called when a received command has no attached function
void OnUnknownCommand()
{
    lastCommand = millis();
    cmdMessenger.sendCmd(kStatus, F("n/a"));
}

uint32_t getLastCommandMillis() 
{
  return lastCommand;
}

void setLastCommandMillis() 
{
    lastCommand = millis();
}

void setLastCommandMillis(uint32_t time) 
{
    lastCommand = time;
}

void OnTrigger()
{
    //   Button::OnTrigger();
    // #if MF_INPUT_SHIFTER_SUPPORT == 1
    //   InputShifter::OnTrigger();
    // #endif
    resetDevices();
}

// commandMessenger.cpp
