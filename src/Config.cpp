// 
// config.cpp
//
// (C) MobiFlight Project 2022
// 

#include <Arduino.h>
#include "mobiflight.h"
#include "config.h"
#include "MFEEPROM.h"

// Macros used to extract the build version from an environment variable
#define STRINGIZER(arg) #arg
#define STR_VALUE(arg)  STRINGIZER(arg)
#define VERSION         STR_VALUE(BUILD_VERSION)

MFEEPROM            MFeeprom;


// Initialize buffer for objects
uint8_t       stowBuf[MEMLEN_OBJ_BUFFER];
StowManager   Stowage(stowBuf, MEMLEN_OBJ_BUFFER);

const uint8_t   EEP_OFFSET_NAME   = 0;
const uint8_t   MEM_LEN_NAME      = 48;
const uint8_t   EEP_OFFSET_SERIAL = EEP_OFFSET_NAME + MEM_LEN_NAME;
const uint8_t   MEM_LEN_SERIAL    = 11;
const uint8_t   EEP_OFFSET_CONFIG = EEP_OFFSET_NAME + MEM_LEN_NAME + MEM_LEN_SERIAL;
const uint16_t  EEP_LEN_CONFIG    = MEMLEN_CONFIG_MAX; //EEPROM_SIZE - EEP_OFFSET_CONFIG;


struct {
    boolean     activated;
    uint16_t    length;
    const char  type[sizeof(MOBIFLIGHT_TYPE)];
    char        name[MEM_LEN_NAME];
    char        serial[MEM_LEN_SERIAL];
    char        nameBuffer[MEMLEN_NAME_BUFFER];
} config = {
    false,
    0,
    MOBIFLIGHT_TYPE,
    MOBIFLIGHT_NAME,
    MOBIFLIGHT_SERIAL,
    ""
};

static bool readConfigLength(void);
static void readConfig(void);
static void activateConfig(void);
static void restoreName(void);

void resetConfig(void)
{
    wipeDevices();
    config.length    = 0;
    config.activated = false;
    config.nameBuffer[0] = '\0';
}

// ************************************************************
// configBuffer handling
// ************************************************************
// reads the EEPROM until NULL termination and returns the number of characters incl. NULL termination, starting from given address
bool readConfigLength(void)
{
    char     temp       = 0;
    uint16_t addreeprom = EEP_OFFSET_CONFIG;
    uint16_t length     = MFeeprom.get_length();
    config.length        = 0;
    do {
        temp = MFeeprom.read_char(addreeprom++);
        config.length++;
        if (addreeprom > length) // abort if EEPROM size will be exceeded
        {
            cmdMessenger.sendCmd(kStatus, F("Loading config failed")); // text or "-1" like config upload?
            return false;
        }
    } while (temp != 0x00); // reads until NULL
    config.length--;
    return true;
}

void loadConfig(void)
{
#ifdef DEBUG2MSG
    cmdMessenger.sendCmd(kStatus, F("Load config"));
#endif
    if (readConfigLength()) {
        readConfig();
        activateConfig();
    }
    restoreName();
}

void OnSetConfig(void)
{
#ifdef DEBUG2MSG
    cmdMessenger.sendCmd(kStatus, F("Setting config start"));
#endif
    setLastCommandMillis();
    char   *cfg    = cmdMessenger.readStringArg();
    uint8_t cfgLen = strlen(cfg);

    if (config.length + cfgLen + 1 < EEP_LEN_CONFIG) {
        MFeeprom.write_block(EEP_OFFSET_CONFIG + config.length, cfg, cfgLen + 1);
        config.length += cfgLen;
        cmdMessenger.sendCmd(kStatus, config.length);
    } else
        cmdMessenger.sendCmd(kStatus, -1); // last successfull saving block is already NULL terminated, nothing more todo
#ifdef DEBUG2MSG
    cmdMessenger.sendCmd(kStatus, F("Setting config end"));
#endif
}

void OnResetConfig(void)
{
    resetConfig();
    cmdMessenger.sendCmd(kStatus, F("OK"));
}

void OnSaveConfig(void)
{
    //_storeConfig();
    cmdMessenger.sendCmd(kConfigSaved, F("OK"));
}

void OnActivateConfig(void)
{
    readConfig();
    activateConfig();
}

void activateConfig(void)
{
    config.activated = true;
    cmdMessenger.sendCmd(kConfigActivated, F("OK"));
}

void resetEEPROMpointer(uint16_t pos = 0){
    MFeeprom.setPosition(pos);
}


// Read from EEPROM a '.' terminated ASCII number and return its value
uint8_t readUintFromEEPROM(void)
{
    char    params[4] = {0};    // max 3 (255) digits, NUL terminated
    uint8_t counter   = 0;
    do {
        params[counter++] = MFeeprom.read_char();      // read character from eeprom and locate next buffer and eeprom location
    } while (params[counter - 1] != '.' && counter < sizeof(params)); // reads until limiter '.' and for safety reason not more then size of params[]
    params[counter - 1] = 0x00;                                       // replace '.' by NULL to terminate the string
    return atoi(params);
}

// Reads the ':' terminated command tail from EEPROM at given address
// If a buffer is specified, the caracters read are stored there and terminated by NUL.
// Note:
// when the nameBuffer will be no longer needed, just remove the parts 
// under the "if(dest)" clauses (and return "true")

bool readRecordTailFromEEPROM(char **dest = NULL, char *cap = (char *)0xFFFF)
{
    char temp = 0;
    do {
        temp = MFeeprom.read_char();     // read the first character
        if(dest) {
            *((*dest)++) = temp;
            //TODO: this function should be buffer-agnostic -
            // the specific constant for buffer size should not be hardcoded!
            if (*dest >= cap) break;   // nameBuffer full: stop copying
        }
    } while (temp != ':');            // reads until limiter ':' and locates the next free buffer position
    if(dest) {
        *((*dest)--) = 0x00; // replace ':' by NULL, terminates the string
    }
    return (*dest >= cap);
}

#if MF_MUX_SUPPORT == 1
void SetMultiplexer(uint8_t Sel0Pin, uint8_t Sel1Pin, uint8_t Sel2Pin, uint8_t Sel3Pin)
{
    MUX.attach(Sel0Pin, Sel1Pin, Sel2Pin, Sel3Pin);
    #ifdef DEBUG2MSG
    cmdMessenger.sendCmd(kStatus, F("Added multiplexer"));
    #endif
}
#endif

void readConfig()
{
    if (config.length == 0) // do nothing if no config is available
        return;
    //uint16_t bufferIdx   = 0;                   // and start with first memory location from nameBuffer
    char     *nameBufPtr  = config.nameBuffer;  // Current position in name buffer
    char     *topOfBuf    = config.nameBuffer + MEMLEN_NAME_BUFFER;  // Max available position (+ 1) in name buffer
    char     params[6]    = "";
    char     devCode      = 0;                  // Code for device type             
    bool     copySuccess  = true;               // will be set to false if copying input names to nameBuffer exceeds array dimensions
                                                // (no longer required when pins will be transferred to the UI instead of names)

    resetEEPROMpointer(EEP_OFFSET_CONFIG);      // Start from first memory location where config is saved in EEPROM

    devCode = readUintFromEEPROM(); // read the first value from EEPROM, it's a device definition
    if (devCode == 0) // just to be sure, config.length should also be 0
        return;

    do // go through the EEPROM until it is NULL terminated
    {
        switch (devCode) {
        case kTypeButton:
            params[0] = readUintFromEEPROM();                       // Pin
            Button::Add(params[0], nameBufPtr);  // MUST be before readNameFromEEPROM because readNameFromEEPROM returns the pointer for the NEXT Name
            copySuccess = readRecordTailFromEEPROM(&nameBufPtr, topOfBuf); // copy the NUL terminated name to to nameBuffer and set to next free memory location
            break;

        case kTypeOutput:
            params[0] = readUintFromEEPROM(); // Pin
            Output::Add(params[0]);
            copySuccess = readRecordTailFromEEPROM(); // check EEPROM until end of name
            break;

        #if MF_SEGMENT_SUPPORT == 1
        case kTypeLedSegment:
            params[0] = readUintFromEEPROM(); // Pin Data
            params[1] = readUintFromEEPROM(); // Pin CS
            params[2] = readUintFromEEPROM(); // Pin CLK
            params[3] = readUintFromEEPROM(); // brightness
            params[4] = readUintFromEEPROM(); // numModules
            LedSegment::Add(params[0], params[1], params[2], params[4], params[3]);
            copySuccess = readRecordTailFromEEPROM(); // check EEPROM until end of name
            break;
        #endif

        #if MF_STEPPER_SUPPORT == 1
        case kTypeStepperDeprecated:
            // this is for backwards compatibility
            params[0] = readUintFromEEPROM(); // Pin1
            params[1] = readUintFromEEPROM(); // Pin2
            params[2] = readUintFromEEPROM(); // Pin3
            params[3] = readUintFromEEPROM(); // Pin4
            params[4] = readUintFromEEPROM(); // Button
            Stepper::Add(params[0], params[1], params[2], params[3], 0);
            copySuccess = readRecordTailFromEEPROM(); // check EEPROM until end of name
            break;
        #endif

        #if MF_STEPPER_SUPPORT == 1
        case kTypeStepper:
            params[0] = readUintFromEEPROM(); // Pin1
            params[1] = readUintFromEEPROM(); // Pin2
            params[2] = readUintFromEEPROM(); // Pin3
            params[3] = readUintFromEEPROM(); // Pin4
            params[4] = readUintFromEEPROM(); // Button
            Stepper::Add(params[0], params[1], params[2], params[3], params[4]);
            copySuccess = readRecordTailFromEEPROM(); // check EEPROM until end of name
            break;
        #endif

        #if MF_SERVO_SUPPORT == 1
        case kTypeServo:
            params[0] = readUintFromEEPROM(); // Pin
            Servos::Add(params[0]);
            copySuccess = readRecordTailFromEEPROM(); // check EEPROM until end of name
            break;
        #endif

        case kTypeEncoderSingleDetent:
            params[0] = readUintFromEEPROM();                                    // Pin1
            params[1] = readUintFromEEPROM();                                    // Pin2
            Encoder::Add(params[0], params[1], 0, nameBufPtr);            // MUST be before readNameFromEEPROM because readNameFromEEPROM returns the pointer for the NEXT Name
            copySuccess = readRecordTailFromEEPROM(&nameBufPtr, topOfBuf);
            break;

        case kTypeEncoder:
            params[0] = readUintFromEEPROM();                                    // Pin1
            params[1] = readUintFromEEPROM();                                    // Pin2
            params[2] = readUintFromEEPROM();                                    // type
            Encoder::Add(params[0], params[1], params[2], nameBufPtr);    // MUST be before readNameFromEEPROM because readNameFromEEPROM returns the pointer for the NEXT Name
            copySuccess = readRecordTailFromEEPROM(&nameBufPtr, topOfBuf); // copy the NULL terminated name to to nameBuffer and set to next free memory location
             // copy the NULL terminated name to to nameBuffer and set to next free memory location
//          copySuccess = readEndCommandFromEEPROM();                    // once the nameBuffer is not required anymore uncomment this line and delete the line before
            break;

        #if MF_LCD_SUPPORT == 1
        case kTypeLcdDisplayI2C:
            params[0] = readUintFromEEPROM(); // address
            params[1] = readUintFromEEPROM(); // columns
            params[2] = readUintFromEEPROM(); // lines
            LCDDisplay::Add(params[0], params[1], params[2]);
            copySuccess = readRecordTailFromEEPROM();
            break;
        #endif

        #if MF_ANALOG_SUPPORT == 1
        case kTypeAnalogInput:
            params[0] = readUintFromEEPROM(); // pin
            params[1] = readUintFromEEPROM(); // sensitivity
            Analog::Add(params[0], params[1], nameBufPtr);              // MUST be before readNameFromEEPROM because readNameFromEEPROM returns the pointer for the NEXT Name
            copySuccess = readRecordTailFromEEPROM(&nameBufPtr, topOfBuf); // copy the NULL terminated name to to nameBuffer and set to next free memory location
//          copySuccess = readEndCommandFromEEPROM();                      // once the nameBuffer is not required anymore uncomment this line and delete the line before
            break;
        #endif

        #if MF_OUTPUT_SHIFTER_SUPPORT == 1
        case kTypeOutShiftReg:
            params[0] = readUintFromEEPROM(); // latch Pin
            params[1] = readUintFromEEPROM(); // clock Pin
            params[2] = readUintFromEEPROM(); // data Pin
            params[3] = readUintFromEEPROM(); // number of daisy chained modules
            OutputShifter::Add(params[0], params[1], params[2], params[3]);
            copySuccess = readRecordTailFromEEPROM(); // check EEPROM until end of name
            break;
        #endif

        #if MF_INPUT_SHIFTER_SUPPORT == 1
        case kTypeInShiftReg:
            params[0] = readUintFromEEPROM(); // latch Pin
            params[1] = readUintFromEEPROM(); // clock Pin
            params[2] = readUintFromEEPROM(); // data Pin
            params[3] = readUintFromEEPROM(); // number of daisy chained modules
            InputShifter::Add(params[0], params[1], params[2], params[3], nameBufPtr);
            copySuccess = readRecordTailFromEEPROM(&nameBufPtr, topOfBuf); // copy the NULL terminated name to to nameBuffer and set to next free memory location
            break;
        #endif

        #if MF_MUX_SUPPORT == 1
        // No longer a separate config command for the mux driver
        // case kTypeMuxDriver:
        //   // Repeated commands do not define more objects, but change the only existing one
        //   params[0] = strtok_r(NULL, ".", &p); // Sel0 pin
        //   params[1] = strtok_r(NULL, ".", &p); // Sel1 pin
        //   params[2] = strtok_r(NULL, ".", &p); // Sel2 pin
        //   params[3] = strtok_r(NULL, ":", &p); // Sel3 pin
        //   AddMultiplexer(atoi(params[0]), atoi(params[1]), atoi(params[2]), atoi(params[3]));
        //   break;
        #endif

        #if MF_DIGIN_MUX_SUPPORT == 1
        case kTypeDigInMux:
            params[0] = readUintFromEEPROM(); // data pin
            // Mux driver section
            // Repeated commands do not define more objects, but change the only existing one
            params[1] = readUintFromEEPROM(); // Sel0 pin
            params[2] = readUintFromEEPROM(); // Sel1 pin
            params[3] = readUintFromEEPROM(); // Sel2 pin
            params[4] = readUintFromEEPROM(); // Sel3 pin
            SetMultiplexer(params[1], params[2], params[3], params[4]);
            params[5] = readUintFromEEPROM(); // 8-bit registers (1-2)
            DigInMux::Add(params[0], params[5], nameBufPtr);
            copySuccess = readRecordTailFromEEPROM(&nameBufPtr, topOfBuf);
            break;
        #endif

        default:
            // read to the end of the current command which is apparently not understood
            copySuccess = readRecordTailFromEEPROM(); // check EEPROM until end of name
        }
    
        devCode = readUintFromEEPROM();     // Read dev code of next entry
    
    } while (devCode && copySuccess);
    
    // No longer required: now readRecordTailFromEEPROM() always terminates last string regardless if valid 
    // if (!copySuccess) {                                    // too much/long names for input devices
    //     *(--nameBufPtr)  = 0x00;
    //     // Was:
    //     // config.nameBuffer[MEMLEN_NAME_BUFFER - 1] = 0x00; // terminate the last copied (part of) string with 0x00
    // }
}

void OnGetConfig()
{
    setLastCommandMillis();
    cmdMessenger.sendCmdStart(kInfo);
    cmdMessenger.sendCmdArg(MFeeprom.read_char(EEP_OFFSET_CONFIG));
    for (uint16_t i = 1; i < config.length; i++) {
        cmdMessenger.sendArg(MFeeprom.read_char(EEP_OFFSET_CONFIG + i));
    }
    cmdMessenger.sendCmdEnd();
}

void OnGetInfo()
{
    setLastCommandMillis();
    cmdMessenger.sendCmdStart(kInfo);
    cmdMessenger.sendCmdArg(config.type);
    cmdMessenger.sendCmdArg(config.name);
    cmdMessenger.sendCmdArg(config.serial);
    cmdMessenger.sendCmdArg(VERSION);
    cmdMessenger.sendCmdEnd();
}

bool getStatusConfig()
{
    return config.activated;
}

// ************************************************************
// serial number handling
// ************************************************************
void generateSerial(bool force)
{
    MFeeprom.read_block(EEP_OFFSET_SERIAL, config.serial, MEM_LEN_SERIAL);
    if (!force && config.serial[0] == 'S' && config.serial[1] == 'N')
        return;
    randomSeed(analogRead(RANDOM_SEED_INPUT));
    sprintf(config.serial, "SN-%03x-", (unsigned int)random(4095));
    sprintf(&config.serial[7], "%03x", (unsigned int)random(4095));
    MFeeprom.write_block(EEP_OFFSET_SERIAL, config.serial, MEM_LEN_SERIAL);
    if (!force) {
        MFeeprom.write_byte(EEP_OFFSET_CONFIG, 0x00); // First byte of config to 0x00 to ensure to start 1st time with empty config, but not if forced from the connector to generate a new one
    }
}

void OnGenNewSerial()
{
    generateSerial(true);
    cmdMessenger.sendCmd(kInfo, config.serial);
}

// ************************************************************
// Naming handling
// ************************************************************
void _storeName()
{
    char prefix[] = "#";
    MFeeprom.write_block(EEP_OFFSET_NAME, prefix, 1);
    MFeeprom.write_block(EEP_OFFSET_NAME + 1, config.name, MEM_LEN_NAME - 1);
}

void restoreName()
{
    char testHasName[1] = "";
    MFeeprom.read_block(EEP_OFFSET_NAME, testHasName, 1);
    if (testHasName[0] != '#')
        return;

    MFeeprom.read_block(EEP_OFFSET_NAME + 1, config.name, MEM_LEN_NAME - 1);
}

void OnSetName()
{
    char *cfg = cmdMessenger.readStringArg();
    memcpy(config.name, cfg, MEM_LEN_NAME);
    _storeName();
    cmdMessenger.sendCmd(kStatus, config.name);
}

// config.cpp