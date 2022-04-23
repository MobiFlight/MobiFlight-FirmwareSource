//
// IOdevice.cpp
//
// (C) MobiFlight Project 2022
// 

#include "IOdevice.h"
#include "mobiflight.h"

// Helper #defines for VTable definition

#define VT_ENTRY(devType) { &devType::Update, &devType::Detach, &devType::Reset, &devType::powerSave, }

// Following #defines are used to "neuter" the VTable entries for non-implemented device types.
//
//Note: #ifdef's could be used directly for corresponding entries in the table definition, e.g.
// #if MF_SEGMENT_SUPPORT !=1
//     { LEDsegments::Update, LEDsegments::Detach, LEDsegments::Reset, LEDsegments::powerSave, },
// #else
//     { NULL,  NULL,  NULL  },
// #endif
// However, that would be less readable and more likely to disrupt table integrity in case of modifications.

#define VT_NULL_ENTRY \
    void Update(void *){}; \
    void Detach(void *){}; \
    void Reset(void *, uint8_t){}; \
    void powerSave(void *, uint8_t){};

namespace blank { VT_NULL_ENTRY; }

#if MF_ANALOG_SUPPORT !=1
namespace Analog { VT_NULL_ENTRY; }
#endif
#if MF_INPUT_SHIFTER_SUPPORT !=1
namespace InputShifter { VT_NULL_ENTRY; }
#endif
#if MF_DIGIN_MUX_SUPPORT !=1
namespace DigINMux { VT_NULL_ENTRY; }
#endif
#if MF_SEGMENT_SUPPORT !=1
namespace LedSegment { VT_NULL_ENTRY; }
#endif
#if MF_LCD_SUPPORT !=1
namespace LCDDisplay { VT_NULL_ENTRY; }
#endif
#if MF_STEPPER_SUPPORT !=1
namespace Stepper { VT_NULL_ENTRY; }
#endif
#if MF_SERVO_SUPPORT !=1
namespace Servos { VT_NULL_ENTRY; }
#endif
#if MF_OUTPUT_SHIFTER_SUPPORT !=1
namespace OutputShifter { VT_NULL_ENTRY; }
#endif


// ================================= 
//  Definition of the virtual table 
// ================================= 

// IMPORTANT:
// The position of each device entry MUST correspond to the device code ("kTypeXxxx")
// as defined in config.h!

#ifndef __AVR__
#define PROGMEM
#endif

funSet_t const VTable[] PROGMEM = 
{
    VT_ENTRY(blank),        // 0  kTypeNotSet
    VT_ENTRY(Button),       // 1  kTypeButton
    VT_ENTRY(Encoder),      // 2  kTypeEncoderSingleDetent
    VT_ENTRY(Output),       // 3  kTypeOutput
    VT_ENTRY(LedSegment),   // 4  kTypeLedSegment
    VT_ENTRY(Stepper),      // 5  kTypeStepperDeprecated
    VT_ENTRY(Servos),       // 6  kTypeServo
    VT_ENTRY(LCDDisplay),   // 7  kTypeLcdDisplayI2C
    VT_ENTRY(Encoder),      // 8  kTypeEncoder
    VT_ENTRY(Stepper),      // 9  kTypeStepper
    VT_ENTRY(OutputShifter),// 10 kTypeOutShiftReg
    VT_ENTRY(Analog),       // 11 kTypeAnalogInput
    VT_ENTRY(InputShifter), // 12 kTypeInShiftReg
    VT_ENTRY(blank),        // 13 kTypeMuxDriver
    VT_ENTRY(DigInMux),     // 14 kTypeDigInMux
};

// _IODevice.h
