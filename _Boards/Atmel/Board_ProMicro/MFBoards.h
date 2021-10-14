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

// ALL          26892 (93.8%) / 1713 (66.9%)
// No Segments  25148 (87.7%) / 1580 (61.7%)
// No LCDs      25308 (88.3%) / 1678 (65.5%)
// NO Steppers  21892 (76.4%) / 1675 (65.4%)
// No Servos    25302 (88.2%) / 1620 (63.3%)

#define MODULE_MAX_PINS 21
#define MAX_OUTPUTS 10
#define MAX_BUTTONS 16
#define MAX_LEDSEGMENTS 1
#define MAX_ENCODERS 4
#define MAX_STEPPERS 2
#define MAX_MFSERVOS 2
#define MAX_MFLCD_I2C 2
#define MAX_ANALOG_INPUTS 2
#define MAX_SHIFTERS 4

#define STEPS 64
#define STEPPER_SPEED 400 // 300 already worked, 467, too?
#define STEPPER_ACCEL 800

#define MOBIFLIGHT_TYPE "MobiFlight Micro"
#define MOBIFLIGHT_SERIAL "0987654321"
#define MOBIFLIGHT_NAME "MobiFlight Micro"
#define EEPROM_SIZE EEPROMSizeMicro
#define MEMLEN_CONFIG 256

#endif
