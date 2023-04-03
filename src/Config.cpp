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
#include "ArduinoUniqueID.h"

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
#include "PfcMatrix.h"

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

char      serial[3 + UniqueIDsize * 2 + 1] = MOBIFLIGHT_SERIAL; // 3 characters for "SN-", UniqueID as HEX String, terminating NULL
char      name[MEM_LEN_NAME]               = MOBIFLIGHT_NAME;
const int MEM_LEN_CONFIG                   = MEMLEN_CONFIG;
char      nameBuffer[MEM_LEN_CONFIG]       = "";
uint16_t  configLength                     = 0;
boolean   configActivated                  = false;

void resetConfig();
void readConfig();
void _activateConfig();

// ************************************************************
// configBuffer handling
// ************************************************************
// reads the EEPROM until NUL terminator and returns the number of characters incl. terminator, starting from given address
bool readConfigLength()
{
    return true;
}

void loadConfig()
{
#ifdef DEBUG2CMDMESSENGER
    cmdMessenger.sendCmd(kDebug, F("Load config"));
#endif
    if (readConfigLength()) {
        readConfig();
        _activateConfig();
    }
}

void OnSetConfig()
{
#ifdef DEBUG2CMDMESSENGER
    cmdMessenger.sendCmd(kDebug, F("Setting config start"));
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
    cmdMessenger.sendCmd(kDebug, F("Setting config end"));
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
#endif
#if MF_DIGIN_MUX_SUPPORT == 1
    DigInMux::Clear();
#endif
    configLength    = 0;
    configActivated = false;
    ClearMemory();
}

void OnResetConfig()
{
    resetConfig();
    cmdMessenger.sendCmd(kStatus, F("OK"));
}

void OnSaveConfig()
{
    cmdMessenger.sendCmd(kConfigSaved, F("OK"));
    //  Uncomment the if{} part to reset and load the config via serial terminal for testing w/o the GUI
    //    1: Type "13" to reset the config
    //    2: Type "14" to get the config length
    //    3: Type "16" to load the config
    /*
        if (readConfigLength())
        {
            readConfig();
            _activateConfig();
        }
    */
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
        params[counter++] = MFeeprom.read_byte((*addreeprom)++);      // read character from eeprom and locate next buffer and eeprom location
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
        temp                    = MFeeprom.read_byte((*addreeprom)++); // read the first character
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
        temp = MFeeprom.read_byte((*addreeprom)++);
        if (*addreeprom > length) // abort if EEPROM size will be exceeded
            return false;
    } while (temp != ':'); // reads until limiter ':'
    return true;
}

void readConfig()
{
    PfcMatrix::Add(2, 3, 4, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31);
    Encoder::Add(34, 35, 2, "Encoder Outer Knob");
    Encoder::Add(37, 36, 2, "Encoder Inner Knob");
    Button::Add(40, "RMI Nav1/2");
    Button::Add(41, "Switch RMT");
    Button::Add(42, "Switch FREQ");
    Button::Add(43, "Switch GS/T");
    Button::Add(44, "Switch N1");
    Button::Add(45, "Switch N2");
}

void OnGetConfig()
{
    // const char * display1 = "4.2.3.4.15.1.Display 1:4.2.3.4.15.1.Display 2:";
    const char *display1 = "\
4.2.3.4.15.1.DME Display 1 (3 digits):\
4.2.3.4.15.1.DME Display 2 (5 digits):\
3.2.NM Status:\
3.3.KTS Status:\
3.4.MHZ Status:\
3.5.Min Status:\
1.40.RMI Nav1/2:\
1.41.Switch RMT:\
1.42.Switch FREQ:\
1.43.Switch GS/T:\
1.44.Switch N1:\
1.45.Switch N2:\
8.34.35.2.Encoder Outer Knob:\
8.36.37.2.Encoder Inner Knob:\
";
    setLastCommandMillis();
    cmdMessenger.sendCmdStart(kInfo);
    if (true) {
        uint16_t configLength = strlen(display1);
        cmdMessenger.sendCmdArg((char)display1[0]);
        for (uint16_t i = 1; i < configLength; i++) {
            cmdMessenger.sendArg((char)display1[i]);
        }
    }
    cmdMessenger.sendCmdEnd();
}

void OnGetInfo()
{
    // read the serial number and generate if 1st start up, was before in ResetBoard()
    // moved to this position as the time to generate a serial number in ResetBoard() is always the same
    // OnGetInfo() is called from the connector and the time is very likely always different
    // Therefore millis() can be used for randomSeed
    generateSerial(false);
    setLastCommandMillis();
    cmdMessenger.sendCmdStart(kInfo);
    cmdMessenger.sendCmdArg(F(MOBIFLIGHT_TYPE));
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
void generateRandomSerial()
{
    randomSeed(millis());
    sprintf(serial, "SN-%03x-%03x", (unsigned int)random(4095), (unsigned int)random(4095));
    MFeeprom.write_block(MEM_OFFSET_SERIAL, serial, MEM_LEN_SERIAL);
}

void generateUniqueSerial()
{
    sprintf(serial, "SN-");
    for (size_t i = 0; i < UniqueIDsize; i++) {
        sprintf(&serial[3 + i * 2], "%02X", UniqueID[i]);
    }
}

void generateSerial(bool force)
{
    if (force) {
        // A serial number is forced to generate
        // generate a serial number acc. the old style also for the Pico
        generateRandomSerial();
        return;
    }

    // A serial number according old style is already generated and saved to the eeprom
    if (MFeeprom.read_byte(MEM_OFFSET_SERIAL) == 'S' && MFeeprom.read_byte(MEM_OFFSET_SERIAL + 1) == 'N') {
        MFeeprom.read_block(MEM_OFFSET_SERIAL, serial, MEM_LEN_SERIAL);
        return;
    }

    // A uniqueID is already generated and saved to the eeprom
    if (MFeeprom.read_byte(MEM_OFFSET_SERIAL) == 'I' && MFeeprom.read_byte(MEM_OFFSET_SERIAL + 1) == 'D') {
        generateUniqueSerial();
        return;
    }

    // Coming here no UniqueID and no serial number is available, so it's the first start up of a board
#if defined(ARDUINO_ARCH_AVR)
    // Generate a serial number for AVR's
    // To have not always the same starting point for the random generator, millis() are
    // used as starting point. It is very unlikely that the time between flashing the firmware
    // and getting the command to send the info's to the connector is always the same.
    generateRandomSerial();
#elif defined(ARDUINO_ARCH_RP2040)
    // Read the uniqueID for Pico's and use it as serial number
    generateUniqueSerial();
    // mark this in the eeprom that a UniqueID is used on first start up for Pico's
    MFeeprom.write_block(MEM_OFFSET_SERIAL, "ID", 2);
#endif
    // Set first byte of config to 0x00 to ensure empty config on 1st start up
    // Otherwise the complete length of the config will be send with 0xFF (empty EEPROM)
    MFeeprom.write_byte(MEM_OFFSET_CONFIG, 0x00);
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
    MFeeprom.write_byte(MEM_OFFSET_NAME, '#');
    MFeeprom.write_block(MEM_OFFSET_NAME + 1, name, MEM_LEN_NAME - 1);
}

void restoreName()
{
    if (MFeeprom.read_byte(MEM_OFFSET_NAME) != '#')
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
