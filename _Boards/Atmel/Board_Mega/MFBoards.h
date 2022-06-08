// 
// MFBoards.h (Mega)
// 

#pragma once

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
#define MF_MUX_SUPPORT       1
#endif
#ifndef MF_DIGIN_MUX_SUPPORT
#define MF_MUX_SUPPORT       1
#define MF_DIGIN_MUX_SUPPORT 1
#endif

#ifdef TESTING
// Current values for Mega
//#define MODULE_MAX_PINS 69
#define MAX_OUTPUTS 40
#define MAX_BUTTONS 68
#define MAX_LEDSEGMENTS 4
#define MAX_ENCODERS 20
#define MAX_STEPPERS 10
#define MAX_MFSERVOS 10
#define MAX_MFLCD_I2C 2
#define MAX_ANALOG_INPUTS 16
#define MAX_OUTPUT_SHIFTERS 4
#define MAX_INPUT_SHIFTERS 4

// Current values for Uno
//#define MODULE_MAX_PINS 13
// #define MAX_OUTPUTS 8
// #define MAX_BUTTONS 8
// #define MAX_LEDSEGMENTS 1
// #define MAX_ENCODERS 3
// #define MAX_STEPPERS 2
// #define MAX_MFSERVOS 2
// #define MAX_MFLCD_I2C 2
// #define MAX_ANALOG_INPUTS 3
// #define MAX_OUTPUT_SHIFTERS 2
// #define MAX_INPUT_SHIFTERS 2

// Current values for ProMicro
//#define MODULE_MAX_PINS 21
// #define MAX_OUTPUTS 18
// #define MAX_BUTTONS 18
// #define MAX_LEDSEGMENTS 1
// #define MAX_ENCODERS 5
// #define MAX_STEPPERS 3
// #define MAX_MFSERVOS 3
// #define MAX_MFLCD_I2C 2
// #define MAX_ANALOG_INPUTS 5
// #define MAX_OUTPUT_SHIFTERS 2
// #define MAX_INPUT_SHIFTERS 2

#endif

#define STEPS 64
#define STEPPER_SPEED 400 // 300 already worked, 467, too?
#define STEPPER_ACCEL 800

#define MOBIFLIGHT_TYPE         "MobiFlight Mega"
#define MOBIFLIGHT_SERIAL       "1234567890"
#define MOBIFLIGHT_NAME         "MobiFlight Mega"
#define FLASH_SIZE              262144  // For reference only, not used    
#define RAM_SIZE                8192    // For reference only, not used    
#define EEPROM_SIZE             4096    // EEPROMSizeMega
#define MEMLEN_CONFIG_MAX       1496    // max. size for config which wil be stored in EEPROM (?)
#define MEMLEN_OBJ_BUFFER       1500 //3350    // size of device object buffer (RAM)
#define MEMLEN_NAME_BUFFER      1000 //2000    // size of nameBuffer (RAM) - contains only device names

#define RANDOM_SEED_INPUT A0

// MFBoards.h (Mega)
