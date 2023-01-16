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
#ifndef MF_CUSTOMDEVICE_SUPPORT
#define MF_CUSTOMDEVICE_SUPPORT 1
#endif

#define MAX_OUTPUTS         40
#define MAX_BUTTONS         68
#define MAX_LEDSEGMENTS     4
#define MAX_ENCODERS        20
#define MAX_STEPPERS        10
#define MAX_MFSERVOS        10
#define MAX_MFLCD_I2C       2
#define MAX_ANALOG_INPUTS   16
#define MAX_OUTPUT_SHIFTERS 4
#define MAX_INPUT_SHIFTERS  4
#define MAX_DIGIN_MUX       4
#define MAX_CUSTOM_DEVICES  2

#define MOBIFLIGHT_TYPE     "MobiFlight Mega"
#define MOBIFLIGHT_SERIAL   "1234567890"
#define MOBIFLIGHT_NAME     "MobiFlight Mega"
#define EEPROM_SIZE         4096 // EEPROMSizeMega
#define MEMLEN_CONFIG       1496 // max. size for config which wil be stored in EEPROM
#define MEMLEN_NAMES_BUFFER 1000 // max. size for configBuffer, contains only names from inputs
#define MF_MAX_DEVICEMEM    1500 // max. memory size for devices


#endif

// MFBoards.h