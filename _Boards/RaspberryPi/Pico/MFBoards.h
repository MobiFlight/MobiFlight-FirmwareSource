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

#define MAX_OUTPUTS         26
#define MAX_BUTTONS         26
#define MAX_LEDSEGMENTS     4
#define MAX_ENCODERS        8
#define MAX_STEPPERS        6
#define MAX_MFSERVOS        8
#define MAX_MFLCD_I2C       2
#define MAX_ANALOG_INPUTS   3
#define MAX_OUTPUT_SHIFTERS 4
#define MAX_INPUT_SHIFTERS  4
#define MAX_DIGIN_MUX       4
#define MAX_CUSTOM_DEVICES  2

#define STEPS               64
#define STEPPER_SPEED       400     // 300 already worked, 467, too?
#define STEPPER_ACCEL       800

#define MOBIFLIGHT_TYPE         "MobiFlight RaspiPico"
#define MOBIFLIGHT_SERIAL       "0987654321"
#define MOBIFLIGHT_NAME         "MobiFlight RaspiPico"
#define EEPROM_SIZE             4096    // EEPROMSizeRaspberryPico
#define MEMLEN_CONFIG           1496    // MUST be less than EEPROM_SIZE!! MEM_OFFSET_CONFIG + MEM_LEN_CONFIG <= EEPROM_SIZE, see: eeprom_write_block (MEM_OFFSET_CONFIG, configBuffer, MEM_LEN_CONFIG);
#define MEMLEN_NAMES_BUFFER     1000    // max. size for configBuffer, contains only names from inputs
#define MF_MAX_DEVICEMEM        2000    // max. memory size for devices

#endif
