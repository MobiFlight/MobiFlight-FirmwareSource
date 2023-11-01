//
// MFBoards.h (Arduino Mega)
//
// (C) MobiFlight Project 2022
//

#ifndef MFBoardMega_h
#define MFBoardMega_h

#ifndef MF_SEGMENT_SUPPORT
#define MF_SEGMENT_SUPPORT 1
#endif
#ifndef MF_LCD_SUPPORT
#define MF_LCD_SUPPORT 1
#endif
#ifndef MF_STEPPER_SUPPORT
#define MF_STEPPER_SUPPORT 1
#endif
#ifndef MF_SERVO_SUPPORT
#define MF_SERVO_SUPPORT 1
#endif
#ifndef MF_ANALOG_SUPPORT
#define MF_ANALOG_SUPPORT 1
#endif
#ifndef MF_OUTPUT_SHIFTER_SUPPORT
#define MF_OUTPUT_SHIFTER_SUPPORT 1
#endif
#ifndef MF_INPUT_SHIFTER_SUPPORT
#define MF_INPUT_SHIFTER_SUPPORT 1
#endif
#ifndef MF_MUX_SUPPORT
#define MF_MUX_SUPPORT 1
#endif
#ifndef MF_DIGIN_MUX_SUPPORT
#define MF_MUX_SUPPORT       1
#define MF_DIGIN_MUX_SUPPORT 1
#endif

#ifndef MAX_OUTPUTS
#define MAX_OUTPUTS         40
#endif
#ifndef MAX_BUTTONS
#define MAX_BUTTONS         68
#endif
#ifndef MAX_LEDSEGMENTS
#define MAX_LEDSEGMENTS     4
#endif
#ifndef MAX_ENCODERS
#define MAX_ENCODERS        20
#endif
#ifndef MAX_STEPPERS
#define MAX_STEPPERS        10
#endif
#ifndef MAX_MFSERVOS
#define MAX_MFSERVOS        10
#endif
#ifndef MAX_MFLCD_I2C
#define MAX_MFLCD_I2C       2
#endif
#ifndef MAX_ANALOG_INPUTS
#define MAX_ANALOG_INPUTS   16
#endif
#ifndef MAX_OUTPUT_SHIFTERS
#define MAX_OUTPUT_SHIFTERS 4
#endif
#ifndef MAX_INPUT_SHIFTERS
#define MAX_INPUT_SHIFTERS  4
#endif
#ifndef MAX_DIGIN_MUX
#define MAX_DIGIN_MUX       4
#endif
#ifndef MAX_CUSTOM_DEVICES
#define MAX_CUSTOM_DEVICES  5
#endif

#ifndef MOBIFLIGHT_TYPE
#define MOBIFLIGHT_TYPE     "MobiFlight Mega"
#endif
#ifndef MOBIFLIGHT_SERIAL
#define MOBIFLIGHT_SERIAL   "1234567890"
#endif
#ifndef MOBIFLIGHT_NAME
#define MOBIFLIGHT_NAME     "MobiFlight Mega"
#endif
#ifndef MEMLEN_CONFIG
#define MEMLEN_CONFIG       1496 // max. size for config which wil be stored in EEPROM
#endif
#ifndef MEMLEN_NAMES_BUFFER
#define MEMLEN_NAMES_BUFFER 1000 // max. size for configBuffer, contains only names from inputs
#endif
#ifndef MF_MAX_DEVICEMEM
#define MF_MAX_DEVICEMEM    1500 // max. memory size for devices
#endif

#endif

// MFBoards.h