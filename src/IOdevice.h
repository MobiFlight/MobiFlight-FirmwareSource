//
// IOdevice.h
//
// (C) MobiFlight Project 2022
// 

// Purpose of this code
// 
// In order to achieve uniform treatment of all device objects in the main processing code, 
// a terse and straightforward solution would have been subclassing the individual MFxxx device classes
// from a base "MFIODevice" class with virtual methods.
// This however would have entailed two overhead sources:
// - additional 2 bytes of RAM for EACH object for the VTable pointer: thhis would have been tolerable
//   (minimum 1 byte is required for the identification of type anyway, see also below)
// - a non-negligible amount of RAM for the VTables themselves (order of 100+ bytes).
//   This is a known inconvenience of AVR-GCC: although VTables have no reason at all to reside in RAM, 
//   GCC is VonNeumann-centric, therefore it does not correctly deal with AVR's Harvard architecture.
//   
// Using the language-enforced polymorphism has therefore been deemed excessive for the limited
// available resources; instead, a "custom" polymorphism has been implemented by storing a 1-byte
// type code with each object and managing the virtual tables explicitly.
//
// For MCUs with different architectures (STM32, ESP32...) this problem may not exist; however, 
// the same code, albeit not strictly necessary, can be used.
// The only part of the code that needs to be changed is the macros that access the VTable memory
// (DeviceUpdate, DeviceDetach etc); and the VTable ceclaration as PROGMEM; these are reverted to
// regular memory access / allocation in case the compilation platform is not AVR. 

// 
// This file contains the required helper data structures and definitions for the implementation above, 
// with some instructions on the required features of device objects.

#pragma once

#include <stdint.h>
#include <Arduino.h>
#include "config.h"

// Arguments of reset
enum {
    ONRESET_PRESS, 
    ONRESET_RELEASE, 
    ONRESET_DEFAULT = ONRESET_PRESS, 
};

// Mandatory "Virtual" methods
// =========================== 
//
// Each MFxxx device class is required to define following methods:
//    void update(void);              // Fetch values (I)/ Update internal state (O)
//    void detach(void);              // Cleanup resources when deallocated
//    void reset(uint8_t action);     // Clear the module state. For inputs: does NOT read current actual state!
//    void powerSave(uint8_t mode);   // Set internal status according to power saving state
// Names are not required to be the same (though they better be); >>> signatures are. <<<
//
// See important notes about the reset() method further below.

// Recommended regular methods
// =========================== 
//
// These methods are not required for the VTable implementation;
// they are called explicitly (usually only from the DeviceXxx.cpp file), 
// and each MFDeviceXxx.* could define the corresponding methods with their own signature.
// This is a recommendation of a uniform naming for better clarity.
//
// static void attachHandler(...);  // (input dev. only) When an input event occurs, manage it
// void attach(...);                // Assign connections after object creation
// void setval(...);                // (output dev. only) Send new values to the device

// Stub functions for "virtual" methods
// ==================================== 
//
// Following functions must be defined for all devices to allow accessing the object methods
// from pointers in the VTable.
// Being this an applicative requirement, they can be conveniently placed in the <device>.cpp / <device>.h files
// (the .h should NOT be included if the device is not required, e.g. (MF_<device>_SUPPORT != 1))
// There is a convenient macro for these definitions - see further below.
//
//    namespace DeviceXxx {
//        void Update(void *p)                { static_cast<DeviceXxx *>(p)->update(); }
//        void Detach(void *p)                { static_cast<DeviceXxx *>(p)->detach(); }
//        void Reset(void *p, uint8_t i)      { static_cast<DeviceXxx *>(p)->reset(i); }
//        void powerSave(void *p, uint8_t i)  { static_cast<DeviceXxx *>(p)->powerSave(i); }
//    }
//
// These functions will be inserted in the VTable as  
// { DeviceXxx::Update, DeviceXxx::Detach, DeviceXxx::Reset, DeviceXxx::powerSave }
//
// Note: Above functions could be also implemented as class static functions:
// class MFDeviceXxx {
//      ...
//      static void Update(void *p)           { static_cast<MFDeviceXxx *>(p)->update(); }
//      ...
// }

// Notes about the reset() method.
// ==================================== 
// 
// The reset method has slightly different meanings for input and output devices.
//
// For INPUTS, it has to first update the current state, and then generate a retrigger sequence
// according to the logic described in 
//   https://github.com/MobiFlight/MobiFlight-Connector/issues/497 and 
//   https://github.com/MobiFlight/MobiFlight-Connector/pull/502.
// This requires firing all Release events first, followed by all Press events.
// (IMPORTANT: these trigger events are generated on CURRENT STATUS, not ON CHANGE!)
// In order to accomplish this, the method has to be called twice:
//   - the first time with parameter "RELEASE", whereby (for every device) only inputs in the Released state
//     will trigger an event;
//   - the first time with parameter "PRESS", whereby only inputs in the Released state
//     will trigger an event.
// 
// For OUTPUTS, it will clear the internal state of all outputs (whatever this means for any
//   given device) and send/activate that status.
// 
// Since the method must be called twice (because of inputs), in order to avoid unnecessary 
// double operations:   
// - for INPUTS, only the call with argument "RELEASE", which is made first, performs an update
//   (peripheral poll / data refresh);
// - for OUTPUTS, only the call with argument "PRESS" (which is also defined as default)
//   will be at all effective.


// Helper macros to declare/define function stubs (to be used in "DeviceXxx.h/.cpp" files under the proper namespace scope)
#define DECLARE_VT_STUBS \
    void Update(void *p);  \
    void Detach(void *p);  \
    void Reset(void *p, uint8_t i);  \
    void powerSave(void *p, uint8_t i);

#define DEFINE_VT_STUBS(devClass) \
    void Update(void *p)                { static_cast<devClass *>(p)->update(); };  \
    void Detach(void *p)                { static_cast<devClass *>(p)->detach(); };  \
    void Reset(void *p, uint8_t i)      { static_cast<devClass *>(p)->reset(i); };  \
    void powerSave(void *p, uint8_t i)  { static_cast<devClass *>(p)->powerSave(i); }

// Declarations required for the virtual table
typedef void(*funNoArg_t)(void *);
typedef void(*funByteArg_t)(void *, uint8_t);

typedef struct  {
    funNoArg_t   _update;
    funNoArg_t   _detach;
    funByteArg_t _reset;
    funByteArg_t _powerSave;
} funSet_t;

// Defines required to access the VT in PROGMEM (for AVR)
#ifndef __AVR__
#define pgm_read_word_near
#endif
#define DeviceUpdate(n, ob)          ((funNoArg_t)  pgm_read_word_near(VTable + n))(ob)
#define DeviceDetach(n, ob)          ((funNoArg_t)  pgm_read_word_near(VTable + n + 1*sizeof(void *)))(ob)
#define DeviceReset(n, ob, arg)      ((funByteArg_t)pgm_read_word_near(VTable + n + 2*sizeof(void *)))(ob, arg)
#define DevicePowerSave(n, ob, arg)  ((funByteArg_t)pgm_read_word_near(VTable + n + 3*sizeof(void *)))(ob, arg)

extern funSet_t const VTable[] PROGMEM;

// IODevice.h
