//
// commandMessenger.h
//
// (C) MobiFlight Project 2022
//

#pragma once

#include <CmdMessenger.h>

// This is the list of recognized commands. These can be commands that can either be sent or received.
// In order to receive, attach a callback function to these events
//
// If you increase this list, make sure to check that the MAXCALLBACKS value
// in CmdMessenger.h is set appropriately
enum {
    kInitModule,           // 0
    kSetModule,            // 1
    kSetPin,               // 2
    kSetStepper,           // 3
    kSetServo,             // 4
    kStatus,               // 5, Command to report status
    kEncoderChange,        // 6
    kButtonChange,         // 7
    kStepperChange,        // 8
    kGetInfo,              // 9
    kInfo,                 // 10
    kSetConfig,            // 11
    kGetConfig,            // 12
    kResetConfig,          // 13
    kSaveConfig,           // 14
    kConfigSaved,          // 15
    kActivateConfig,       // 16
    kConfigActivated,      // 17
    kSetPowerSavingMode,   // 18
    kSetName,              // 19
    kGenNewSerial,         // 20
    kResetStepper,         // 21
    kSetZeroStepper,       // 22
    kTrigger,              // 23
    kResetBoard,           // 24
    kSetLcdDisplayI2C,     // 25
    kSetModuleBrightness,  // 26
    kSetShiftRegisterPins, // 27
    kAnalogChange,         // 28
    kInputShifterChange,   // 29
    kDigInMuxChange,       // 30
    kDebug = 0xFF          // 255 -> for Debug print later, changes in UI are required
};

void        attachCommandCallbacks(void);
uint32_t    getLastCommandMillis(void);
void        setLastCommandMillis(void);

extern CmdMessenger     cmdMessenger;
//? extern unsigned long    lastCommand;

// commandMessenger.h
