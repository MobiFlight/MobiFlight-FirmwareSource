//
// MFBoards.h (Arduino Uno/Nano)
//
// (C) MobiFlight Project 2022
//

#ifndef MFBoardUno_h
#define MFBoardUno_h

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
#define MAX_OUTPUTS         18
#endif
#ifndef MAX_BUTTONS
#define MAX_BUTTONS         18
#endif
#ifndef MAX_LEDSEGMENTS
#define MAX_LEDSEGMENTS     1
#endif
#ifndef MAX_ENCODERS
#define MAX_ENCODERS        9
#endif
#ifndef MAX_STEPPERS
#define MAX_STEPPERS        2
#endif
#ifndef MAX_MFSERVOS
#define MAX_MFSERVOS        2
#endif
#ifndef MAX_MFLCD_I2C
#define MAX_MFLCD_I2C       2
#endif
#ifndef MAX_ANALOG_INPUTS
#define MAX_ANALOG_INPUTS   6
#endif
#ifndef MAX_OUTPUT_SHIFTERS
#define MAX_OUTPUT_SHIFTERS 2
#endif
#ifndef MAX_INPUT_SHIFTERS
#define MAX_INPUT_SHIFTERS  2
#endif
#ifndef MAX_DIGIN_MUX
#define MAX_DIGIN_MUX       3
#endif

#ifndef MOBIFLIGHT_TYPE
#define MOBIFLIGHT_TYPE     "MobiFlight Uno"
#endif
#ifndef MOBIFLIGHT_SERIAL
#define MOBIFLIGHT_SERIAL   "0987654321"
#endif
#ifndef MOBIFLIGHT_NAME
#define MOBIFLIGHT_NAME     "MobiFlight Uno"
#endif
#ifndef EEPROM_SIZE
#define EEPROM_SIZE         1024 // EEPROMSizeUno
#endif
#ifndef MEMLEN_CONFIG
#define MEMLEN_CONFIG       286  // max. size for config which wil be stored in EEPROM
#endif
#ifndef MEMLEN_NAMES_BUFFER
#define MEMLEN_NAMES_BUFFER 220  // max. size for configBuffer, contains only names from inputs
#endif
#ifndef MF_MAX_DEVICEMEM
#define MF_MAX_DEVICEMEM    300  // max. memory size for devices
#endif

#endif

// MFBoards.h