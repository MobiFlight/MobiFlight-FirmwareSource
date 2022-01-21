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
#ifndef MF_SHIFTER_SUPPORT
#define MF_SHIFTER_SUPPORT 1
#endif
#ifndef MF_INPUT_SHIFTER_SUPPORT
#define MF_INPUT_SHIFTER_SUPPORT 1
#endif

//#define MODULE_MAX_PINS 21
#define MAX_OUTPUTS 18
#define MAX_BUTTONS 18
#define MAX_LEDSEGMENTS 1
#define MAX_ENCODERS 5
#define MAX_STEPPERS 3
#define MAX_MFSERVOS 3
#define MAX_MFLCD_I2C 2
#define MAX_ANALOG_INPUTS 5
#define MAX_SHIFTERS 2
#define MAX_INPUT_SHIFTERS 2

#define STEPS 64
#define STEPPER_SPEED 400 // 300 already worked, 467, too?
#define STEPPER_ACCEL 800

#define MOBIFLIGHT_TYPE "MobiFlight Micro"
#define MOBIFLIGHT_SERIAL "0987654321"
#define MOBIFLIGHT_NAME "MobiFlight Micro"
#define EEPROM_SIZE 1024 // EEPROMSizeMicro
#define MEMLEN_CONFIG 440
#define MEMLEN_CONFIG_BUFFER    230     // max. size for configBuffer, contains only names from inputs
#define MF_MAX_DEVICEMEM        500     // max. memory size for devices

#endif
