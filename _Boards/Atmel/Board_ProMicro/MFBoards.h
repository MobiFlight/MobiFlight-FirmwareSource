//
// MFBoards.h (Arduino ProMicro)
//
// (C) MobiFlight Project 2022
//

#ifndef MFBoardMicro_h
#define MFBoardMicro_h

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

#define MAX_OUTPUTS         18
#define MAX_BUTTONS         18
#define MAX_LEDSEGMENTS     1
#define MAX_ENCODERS        9
#define MAX_STEPPERS        3
#define MAX_MFSERVOS        3
#define MAX_MFLCD_I2C       2
#define MAX_ANALOG_INPUTS   9
#define MAX_OUTPUT_SHIFTERS 2
#define MAX_INPUT_SHIFTERS  2
#define MAX_DIGIN_MUX       3
#define MAX_CUSTOM_DEVICES  2

#define MOBIFLIGHT_TYPE     "MobiFlight Micro"
#define MOBIFLIGHT_SERIAL   "0987654321"
#define MOBIFLIGHT_NAME     "MobiFlight Micro"
#define EEPROM_SIZE         1024 // EEPROMSizeMicro
#define MEMLEN_CONFIG       440  // max. size for config which wil be stored in EEPROM
#define MEMLEN_NAMES_BUFFER 350  // max. size for configBuffer, contains only names from inputs
#define MF_MAX_DEVICEMEM    400  // max. memory size for devices


#endif

// MFBoards.h
