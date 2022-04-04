//
// config.cpp
//
// (C) MobiFlight Project 2022
//

#include "mobiflight.h"
#include "MFEEPROM.h"

#include "Button.h"
#include "Encoder.h"
#include "Output.h"

#if MF_ANALOG_SUPPORT == 1
#include "Analog.h"
#endif
#if MF_INPUT_SHIFTER_SUPPORT == 1
#include "InputShifter.h"
#endif
#if MF_SEGMENT_SUPPORT == 1
#include "LedSegment.h"
#endif
#if MF_STEPPER_SUPPORT == 1
#include "Stepper.h"
#endif
#if MF_SERVO_SUPPORT == 1
#include "Servos.h"
#endif
#if MF_LCD_SUPPORT == 1
#include "LCDDisplay.h"
#endif
#if MF_OUTPUT_SHIFTER_SUPPORT == 1
#include "OutputShifter.h"
#endif
#if MF_MUX_SUPPORT == 1
#include "MFMuxDriver.h"
#endif
#if MF_DIGIN_MUX_SUPPORT == 1
#include "DigInMux.h"
#endif

// The build version comes from an environment variable
#define STRINGIZER(arg) #arg
#define STR_VALUE(arg)  STRINGIZER(arg)
#define VERSION         STR_VALUE(BUILD_VERSION)
MFEEPROM MFeeprom;

#if MF_MUX_SUPPORT == 1
extern MFMuxDriver MUX;
#endif

const uint8_t MEM_OFFSET_NAME   = 0;
const uint8_t MEM_LEN_NAME      = 48;
const uint8_t MEM_OFFSET_SERIAL = MEM_OFFSET_NAME + MEM_LEN_NAME;
const uint8_t MEM_LEN_SERIAL    = 11;
const uint8_t MEM_OFFSET_CONFIG = MEM_OFFSET_NAME + MEM_LEN_NAME + MEM_LEN_SERIAL;

const char type[sizeof(MOBIFLIGHT_TYPE)] = MOBIFLIGHT_TYPE;
char       serial[MEM_LEN_SERIAL]        = MOBIFLIGHT_SERIAL;
char       name[MEM_LEN_NAME]            = MOBIFLIGHT_NAME;
const int  MEM_LEN_CONFIG                = MEMLEN_CONFIG;
char       nameBuffer[MEM_LEN_CONFIG]    = "";
uint16_t   configLength                  = 0;
boolean    configActivated               = false;

void resetConfig();
void readConfig();
void _activateConfig();

// ************************************************************
// configBuffer handling
// ************************************************************
// reads the EEPROM until NUL terminator and returns the number of characters incl. terminator, starting from given address
bool readConfigLength()
{
    char     temp       = 0;
    uint16_t addreeprom = MEM_OFFSET_CONFIG;
    uint16_t length     = MFeeprom.get_length();
    configLength        = 0;
    do {
        temp = MFeeprom.read_char(addreeprom++);
        configLength++;
        if (addreeprom > length) // abort if EEPROM size will be exceeded
        {
            cmdMessenger.sendCmd(kStatus, F("Loading config failed")); // text or "-1" like config upload?
            return false;
        }
    } while (temp != 0x00); // reads until NULL
    configLength--;
    return true;
}

void loadConfig()
{
#ifdef DEBUG2CMDMESSENGER
    cmdMessenger.sendCmd(kStatus, F("Load config"));
#endif
    if (readConfigLength()) {
        readConfig();
        _activateConfig();
    }
}

void OnSetConfig()
{
#ifdef DEBUG2CMDMESSENGER
    cmdMessenger.sendCmd(kStatus, F("Setting config start"));
#endif
    setLastCommandMillis();
    char   *cfg    = cmdMessenger.readStringArg();
    uint8_t cfgLen = strlen(cfg);

    if (configLength + cfgLen + 1 < MEM_LEN_CONFIG) {
        MFeeprom.write_block(MEM_OFFSET_CONFIG + configLength, cfg, cfgLen + 1); // save the received config string including the terminatung NULL (+1) to EEPROM
        configLength += cfgLen;
        cmdMessenger.sendCmd(kStatus, configLength);
    } else
        cmdMessenger.sendCmd(kStatus, -1); // last successfull saving block is already NULL terminated, nothing more todo
#ifdef DEBUG2CMDMESSENGER
    cmdMessenger.sendCmd(kStatus, F("Setting config end"));
#endif
}

void resetConfig()
{
    Button::Clear();
    Encoder::Clear();
    Output::Clear();
#if MF_SEGMENT_SUPPORT == 1
    LedSegment::Clear();
#endif
#if MF_SERVO_SUPPORT == 1
    Servos::Clear();
#endif
#if MF_STEPPER_SUPPORT == 1
    Stepper::Clear();
#endif
#if MF_LCD_SUPPORT == 1
    LCDDisplay::Clear();
#endif
#if MF_ANALOG_SUPPORT == 1
    Analog::Clear();
#endif
#if MF_OUTPUT_SHIFTER_SUPPORT == 1
    OutputShifter::Clear();
#endif
#if MF_INPUT_SHIFTER_SUPPORT == 1
    InputShifter::Clear();
    configLength    = 0;
    configActivated = false;
#endif
}

void OnResetConfig()
{
    resetConfig();
    cmdMessenger.sendCmd(kStatus, F("OK"));
}

void OnSaveConfig()
{
    cmdMessenger.sendCmd(kConfigSaved, F("OK"));
}

void OnActivateConfig()
{
    readConfig();
    _activateConfig();
}

void _activateConfig()
{
    configActivated = true;
    cmdMessenger.sendCmd(kConfigActivated, F("OK"));
}

// reads an ascii value which is '.' terminated from EEPROM and returns it's value
uint8_t readUintFromEEPROM(volatile uint16_t *addreeprom)
{
    char    params[4] = {0}; // max 3 (255) digits NULL terminated
    uint8_t counter   = 0;
    do {
        params[counter++] = MFeeprom.read_char((*addreeprom)++);      // read character from eeprom and locate next buffer and eeprom location
    } while (params[counter - 1] != '.' && counter < sizeof(params)); // reads until limiter '.' and for safety reason not more then size of params[]
    params[counter - 1] = 0x00;                                       // replace '.' by NULL to terminate the string
    return atoi(params);
}

// reads a string from EEPROM at given address which is ':' terminated and saves it in the nameBuffer
// once the nameBuffer is not needed anymore, just read until the ":" termination -> see function below
bool readNameFromEEPROM(uint16_t *addreeprom, char *buffer, uint16_t *addrbuffer)
{
    char temp = 0;
    do {
        temp                    = MFeeprom.read_char((*addreeprom)++); // read the first character
        buffer[(*addrbuffer)++] = temp;                                // save character and locate next buffer position
        if (*addrbuffer >= MEMLEN_NAMES_BUFFER) {                      // nameBuffer will be exceeded
            return false;                                              // abort copying from EEPROM to nameBuffer
        }
    } while (temp != ':');            // reads until limiter ':' and locates the next free buffer position
    buffer[(*addrbuffer) - 1] = 0x00; // replace ':' by NULL, terminates the string
    return true;
}

// reads the EEPRROM until end of command which ':' terminated
bool readEndCommandFromEEPROM(uint16_t *addreeprom)
{
    char     temp   = 0;
    uint16_t length = MFeeprom.get_length();
    do {
        temp = MFeeprom.read_char((*addreeprom)++);
        if (*addreeprom > length) // abort if EEPROM size will be exceeded
            return false;
    } while (temp != ':'); // reads until limiter ':'
    return true;
}

void readConfig()
{
    if (configLength == 0) // do nothing if no config is available
        return;
    uint16_t addreeprom   = MEM_OFFSET_CONFIG; // define first memory location where config is saved in EEPROM
    uint16_t addrbuffer   = 0;                 // and start with first memory location from nameBuffer
    char     params[6]    = "";
    char     command      = readUintFromEEPROM(&addreeprom); // read the first value from EEPROM, it's a device definition
    bool     copy_success = true;                            // will be set to false if copying input names to nameBuffer exceeds array dimensions
                                                             // not required anymore when pins instead of names are transferred to the UI

    if (command == 0) // just to be sure, configLength should also be 0
        return;

    do // go through the EEPROM until it is NULL terminated
    {
        switch (command) {
        case kTypeButton:
            params[0] = readUintFromEEPROM(&addreeprom);                             // Pin number
            Button::Add(params[0], &nameBuffer[addrbuffer]);                         // MUST be before readNameFromEEPROM because readNameFromEEPROM returns the pointer for the NEXT Name
            copy_success = readNameFromEEPROM(&addreeprom, nameBuffer, &addrbuffer); // copy the NULL terminated name to nameBuffer and set to next free memory location
            break;

        case kTypeOutput:
            params[0] = readUintFromEEPROM(&addreeprom); // Pin number
            Output::Add(params[0]);
            copy_success = readEndCommandFromEEPROM(&addreeprom); // check EEPROM until end of name
            break;

#if MF_SEGMENT_SUPPORT == 1
        case kTypeLedSegment:
            params[0] = readUintFromEEPROM(&addreeprom); // Pin Data number
            params[1] = readUintFromEEPROM(&addreeprom); // Pin CS number
            params[2] = readUintFromEEPROM(&addreeprom); // Pin CLK number
            params[3] = readUintFromEEPROM(&addreeprom); // brightness
            params[4] = readUintFromEEPROM(&addreeprom); // number of modules
            LedSegment::Add(params[0], params[1], params[2], params[4], params[3]);
            copy_success = readEndCommandFromEEPROM(&addreeprom); // check EEPROM until end of name
            break;
#endif

#if MF_STEPPER_SUPPORT == 1
        case kTypeStepperDeprecated:
            // this is for backwards compatibility
            params[0] = readUintFromEEPROM(&addreeprom); // Pin1 number
            params[1] = readUintFromEEPROM(&addreeprom); // Pin2 number
            params[2] = readUintFromEEPROM(&addreeprom); // Pin3 number
            params[3] = readUintFromEEPROM(&addreeprom); // Pin4 number
            params[4] = readUintFromEEPROM(&addreeprom); // Button number
            Stepper::Add(params[0], params[1], params[2], params[3], 0);
            copy_success = readEndCommandFromEEPROM(&addreeprom); // check EEPROM until end of name
            break;
#endif

#if MF_STEPPER_SUPPORT == 1
        case kTypeStepper:
            params[0] = readUintFromEEPROM(&addreeprom); // Pin1 number
            params[1] = readUintFromEEPROM(&addreeprom); // Pin2 number
            params[2] = readUintFromEEPROM(&addreeprom); // Pin3 number
            params[3] = readUintFromEEPROM(&addreeprom); // Pin4 number
            params[4] = readUintFromEEPROM(&addreeprom); // Button number
            Stepper::Add(params[0], params[1], params[2], params[3], params[4]);
            copy_success = readEndCommandFromEEPROM(&addreeprom); // check EEPROM until end of name
            break;
#endif

#if MF_SERVO_SUPPORT == 1
        case kTypeServo:
            params[0] = readUintFromEEPROM(&addreeprom); // Pin number
            Servos::Add(params[0]);
            copy_success = readEndCommandFromEEPROM(&addreeprom); // check EEPROM until end of name
            break;
#endif

        case kTypeEncoderSingleDetent:
            params[0] = readUintFromEEPROM(&addreeprom);                             // Pin1 number
            params[1] = readUintFromEEPROM(&addreeprom);                             // Pin2 number
            Encoder::Add(params[0], params[1], 0, &nameBuffer[addrbuffer]);          // MUST be before readNameFromEEPROM because readNameFromEEPROM returns the pointer for the NEXT Name
            copy_success = readNameFromEEPROM(&addreeprom, nameBuffer, &addrbuffer); // copy the NULL terminated name to nameBuffer and set to next free memory location
                                                                                     //    copy_success = readEndCommandFromEEPROM(&addreeprom);       // once the nameBuffer is not required anymore uncomment this line and delete the line before
            break;

        case kTypeEncoder:
            params[0] = readUintFromEEPROM(&addreeprom);                             // Pin1 number
            params[1] = readUintFromEEPROM(&addreeprom);                             // Pin2 number
            params[2] = readUintFromEEPROM(&addreeprom);                             // type
            Encoder::Add(params[0], params[1], params[2], &nameBuffer[addrbuffer]);  // MUST be before readNameFromEEPROM because readNameFromEEPROM returns the pointer for the NEXT Name
            copy_success = readNameFromEEPROM(&addreeprom, nameBuffer, &addrbuffer); // copy the NULL terminated name to to nameBuffer and set to next free memory location
                                                                                     //    copy_success = readEndCommandFromEEPROM(&addreeprom);       // once the nameBuffer is not required anymore uncomment this line and delete the line before
            break;

#if MF_LCD_SUPPORT == 1
        case kTypeLcdDisplayI2C:
            params[0] = readUintFromEEPROM(&addreeprom); // address
            params[1] = readUintFromEEPROM(&addreeprom); // columns
            params[2] = readUintFromEEPROM(&addreeprom); // lines
            LCDDisplay::Add(params[0], params[1], params[2]);
            copy_success = readEndCommandFromEEPROM(&addreeprom); // check EEPROM until end of name
            break;
#endif

#if MF_ANALOG_SUPPORT == 1
        case kTypeAnalogInput:
            params[0] = readUintFromEEPROM(&addreeprom);                             // pin number
            params[1] = readUintFromEEPROM(&addreeprom);                             // sensitivity
            Analog::Add(params[0], &nameBuffer[addrbuffer], params[1]);              // MUST be before readNameFromEEPROM because readNameFromEEPROM returns the pointer for the NEXT Name
            copy_success = readNameFromEEPROM(&addreeprom, nameBuffer, &addrbuffer); // copy the NULL terminated name to to nameBuffer and set to next free memory location
                                                                                     //    copy_success = readEndCommandFromEEPROM(&addreeprom);       // once the nameBuffer is not required anymore uncomment this line and delete the line before
            break;
#endif

#if MF_OUTPUT_SHIFTER_SUPPORT == 1
        case kTypeOutputShifter:
            params[0] = readUintFromEEPROM(&addreeprom); // latch Pin
            params[1] = readUintFromEEPROM(&addreeprom); // clock Pin
            params[2] = readUintFromEEPROM(&addreeprom); // data Pin
            params[3] = readUintFromEEPROM(&addreeprom); // number of daisy chained modules
            OutputShifter::Add(params[0], params[1], params[2], params[3]);
            copy_success = readEndCommandFromEEPROM(&addreeprom); // check EEPROM until end of name
            break;
#endif

#if MF_INPUT_SHIFTER_SUPPORT == 1
        case kTypeInputShifter:
            params[0] = readUintFromEEPROM(&addreeprom); // latch Pin
            params[1] = readUintFromEEPROM(&addreeprom); // clock Pin
            params[2] = readUintFromEEPROM(&addreeprom); // data Pin
            params[3] = readUintFromEEPROM(&addreeprom); // number of daisy chained modules
            InputShifter::Add(params[0], params[1], params[2], params[3], &nameBuffer[addrbuffer]);
            copy_success = readNameFromEEPROM(&addreeprom, nameBuffer, &addrbuffer); // copy the NULL terminated name to to nameBuffer and set to next free memory location
                                                                                     //    copy_success = readEndCommandFromEEPROM(&addreeprom);       // once the nameBuffer is not required anymore uncomment this line and delete the line before
            break;
#endif

#if MF_MUX_SUPPORT == 1
            // No longer a separate config command for the mux driver
            // case kTypeMuxDriver:
            //   params[0] = strtok_r(NULL, ".", &p); // Sel0 pin
            //   params[1] = strtok_r(NULL, ".", &p); // Sel1 pin
            //   params[2] = strtok_r(NULL, ".", &p); // Sel2 pin
            //   params[3] = strtok_r(NULL, ":", &p); // Sel3 pin
            //   AddMultiplexer(atoi(params[0]), atoi(params[1]), atoi(params[2]), atoi(params[3]));
            //   break;
#endif

#if MF_DIGIN_MUX_SUPPORT == 1
        case kTypeDigInMux:
            params[0] = readUintFromEEPROM(&addreeprom); // data pin
            // Mux driver section
            // Repeated commands do not define more objects, but change the only existing one
            // therefore beware that all DigInMux configuration commands are consistent!
            params[1] = readUintFromEEPROM(&addreeprom); // Sel0 pin
            params[2] = readUintFromEEPROM(&addreeprom); // Sel1 pin
            params[3] = readUintFromEEPROM(&addreeprom); // Sel2 pin
            params[4] = readUintFromEEPROM(&addreeprom); // Sel3 pin
            MUX.attach(params[1], params[2], params[3], params[4]);
            params[5] = readUintFromEEPROM(&addreeprom); // 8-bit registers (1-2)
            DigInMux::Add(params[0], params[5], &nameBuffer[addrbuffer]);
            copy_success = readNameFromEEPROM(&addreeprom, nameBuffer, &addrbuffer);

            // cmdMessenger.sendCmd(kDebug, F("Mux loaded"));
            break;
#endif

        default:
            copy_success = readEndCommandFromEEPROM(&addreeprom); // check EEPROM until end of name
        }
        command = readUintFromEEPROM(&addreeprom);
    } while (command && copy_success);
    if (!copy_success) {                            // too much/long names for input devices
        nameBuffer[MEMLEN_NAMES_BUFFER - 1] = 0x00; // terminate the last copied (part of) string with 0x00
        cmdMessenger.sendCmd(kStatus, F("Failure on reading config"));
    }
}

void OnGetConfig()
{
    setLastCommandMillis();
    cmdMessenger.sendCmdStart(kInfo);
    if (configLength > 0) {
        cmdMessenger.sendCmdArg(MFeeprom.read_char(MEM_OFFSET_CONFIG));
        for (uint16_t i = 1; i < configLength; i++) {
            cmdMessenger.sendArg(MFeeprom.read_char(MEM_OFFSET_CONFIG + i));
        }
    }
    cmdMessenger.sendCmdEnd();
}

void OnGetInfo()
{
    setLastCommandMillis();
    cmdMessenger.sendCmdStart(kInfo);
    cmdMessenger.sendCmdArg(type);
    cmdMessenger.sendCmdArg(name);
    cmdMessenger.sendCmdArg(serial);
    cmdMessenger.sendCmdArg(VERSION);
    cmdMessenger.sendCmdEnd();
}

bool getStatusConfig()
{
    return configActivated;
}

// ************************************************************
// serial number handling
// ************************************************************
void generateSerial(bool force)
{
    MFeeprom.read_block(MEM_OFFSET_SERIAL, serial, MEM_LEN_SERIAL);
    if (!force && serial[0] == 'S' && serial[1] == 'N')
        return;
    randomSeed(analogRead(RANDOM_SEED_INPUT));
    sprintf(serial, "SN-%03x-", (unsigned int)random(4095));
    sprintf(&serial[7], "%03x", (unsigned int)random(4095));
    MFeeprom.write_block(MEM_OFFSET_SERIAL, serial, MEM_LEN_SERIAL);
    if (!force) {
        MFeeprom.write_byte(MEM_OFFSET_CONFIG, 0x00); // First byte of config to 0x00 to ensure to start 1st time with empty config, but not if forced from the connector to generate a new one
    }
}

void OnGenNewSerial()
{
    generateSerial(true);
    cmdMessenger.sendCmd(kInfo, serial);
}

// ************************************************************
// Naming handling
// ************************************************************
void storeName()
{
    char prefix[] = "#";
    MFeeprom.write_block(MEM_OFFSET_NAME, prefix, 1);
    MFeeprom.write_block(MEM_OFFSET_NAME + 1, name, MEM_LEN_NAME - 1);
}

void restoreName()
{
    char testHasName[1] = "";
    MFeeprom.read_block(MEM_OFFSET_NAME, testHasName, 1);
    if (testHasName[0] != '#')
        return;

    MFeeprom.read_block(MEM_OFFSET_NAME + 1, name, MEM_LEN_NAME - 1);
}

void OnSetName()
{
    char *cfg = cmdMessenger.readStringArg();
    memcpy(name, cfg, MEM_LEN_NAME);
    storeName();
    cmdMessenger.sendCmd(kStatus, name);
}

// config.cpp
