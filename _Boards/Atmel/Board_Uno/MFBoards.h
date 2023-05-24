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

#define MAX_OUTPUTS         18
#define MAX_BUTTONS         18
#define MAX_LEDSEGMENTS     1
#define MAX_ENCODERS        9
#define MAX_STEPPERS        2
#define MAX_MFSERVOS        2
#define MAX_MFLCD_I2C       2
#define MAX_ANALOG_INPUTS   6
#define MAX_OUTPUT_SHIFTERS 2
#define MAX_INPUT_SHIFTERS  2
#define MAX_DIGIN_MUX       3

#define MOBIFLIGHT_TYPE     "MobiFlight Uno"
#define MOBIFLIGHT_SERIAL   "0987654321"
#define MOBIFLIGHT_NAME     "MobiFlight Uno"
#define EEPROM_SIZE         1024 // EEPROMSizeUno
#define MEM_LEN_CONFIG      286  // max. size for config which wil be stored in EEPROM, must be less than (MEM_OFFSET_NAME + EEPROM_SIZE - (MEM_LEN_NAME + MEM_LEN_SERIAL))
#define MF_MAX_DEVICEMEM    300  // max. memory size for devices


#endif

// MFBoards.h