/**
 * Includes Core Arduino functionality 
 **/
char foo;
#if ARDUINO < 100
#include <WProgram.h>
#else
#include <Arduino.h>
#endif

#include "mobiflight.h"
#include <MFBoards.h>
#include "allocateMem.h"

// The build version comes from an environment variable
#define STRINGIZER(arg) #arg
#define STR_VALUE(arg) STRINGIZER(arg)
#define VERSION STR_VALUE(BUILD_VERSION)

//#define DEBUG 1

#include "MFEEPROM.h"

#if MF_SEGMENT_SUPPORT == 1
#include <MFSegments.h>
#endif

#include <MFButton.h>

#if MF_INPUT_SHIFTER_SUPPORT == 1
#include <MFInputShifter.h>
#endif

#if MF_STEPPER_SUPPORT == 1
#include <AccelStepper.h>
#include <MFStepper.h>
#endif

#if MF_SERVO_SUPPORT == 1
#include <Servo.h>
#include <MFServo.h>
#endif

#include <MFOutput.h>

#if MF_LCD_SUPPORT == 1
#include <LiquidCrystal_I2C.h>
#include <MFLCDDisplay.h>
#endif

#if MF_ANALOG_SUPPORT == 1
#include <MFAnalog.h>
#endif

#if MF_OUTPUT_SHIFTER_SUPPORT == 1
#include <MFOutputShifter.h>
#endif

const uint8_t MEM_OFFSET_NAME = 0;
const uint8_t MEM_LEN_NAME = 48;
const uint8_t MEM_OFFSET_SERIAL = MEM_OFFSET_NAME + MEM_LEN_NAME;
const uint8_t MEM_LEN_SERIAL = 11;
const uint8_t MEM_OFFSET_CONFIG = MEM_OFFSET_NAME + MEM_LEN_NAME + MEM_LEN_SERIAL;

uint32_t lastButtonUpdate = 0;
uint32_t lastEncoderUpdate = 0;

#if MF_ANALOG_SUPPORT == 1
uint32_t lastAnalogAverage = 0;
uint32_t lastAnalogRead = 0;
#endif

#if MF_SERVO_SUPPORT == 1
uint32_t lastServoUpdate = 0;
#endif

#if MF_INPUT_SHIFTER_SUPPORT == 1
uint32_t lastInputShifterUpdate = 0;
#endif

char serial[MEM_LEN_SERIAL] = MOBIFLIGHT_SERIAL;
char name[MEM_LEN_NAME] = MOBIFLIGHT_NAME;
const int MEM_LEN_CONFIG = MEMLEN_CONFIG;

char nameBuffer[MEMLEN_CONFIG_BUFFER] = "";

uint16_t configLength = 0;
boolean configActivated = false;

bool powerSavingMode = false;
const unsigned long POWER_SAVING_TIME = 60 * 15; // in seconds

CmdMessenger cmdMessenger = CmdMessenger(Serial);
unsigned long lastCommand;

MFEEPROM MFeeprom;

MFOutput *outputs[MAX_OUTPUTS];
uint8_t outputsRegistered = 0;

MFButton *buttons[MAX_BUTTONS];
uint8_t buttonsRegistered = 0;

#if MF_SEGMENT_SUPPORT == 1
MFSegments ledSegments[MAX_LEDSEGMENTS];
uint8_t ledSegmentsRegistered = 0;
#endif

MFEncoder *encoders[MAX_ENCODERS];
uint8_t encodersRegistered = 0;

#if MF_STEPPER_SUPPORT == 1
MFStepper steppers[MAX_STEPPERS]; //
uint8_t steppersRegistered = 0;
#endif

#if MF_SERVO_SUPPORT == 1
MFServo servos[MAX_MFSERVOS];
uint8_t servosRegistered = 0;
#endif

#if MF_LCD_SUPPORT == 1
MFLCDDisplay lcd_I2C[MAX_MFLCD_I2C];
uint8_t lcd_12cRegistered = 0;
#endif

#if MF_ANALOG_SUPPORT == 1
MFAnalog *analog[MAX_ANALOG_INPUTS];
uint8_t analogRegistered = 0;
#endif

#if MF_OUTPUT_SHIFTER_SUPPORT == 1
MFOutputShifter *outputShifters[MAX_OUTPUT_SHIFTERS];
uint8_t outputShifterRegistered = 0;
#endif

#if MF_INPUT_SHIFTER_SUPPORT == 1
MFInputShifter *inputShifters[MAX_INPUT_SHIFTERS];
uint8_t inputShiftersRegistered = 0;
#endif

// Callbacks define on which received commands we take action
void attachCommandCallbacks()
{
  // Attach callback methods
  cmdMessenger.attach(OnUnknownCommand);

#if MF_SEGMENT_SUPPORT == 1
  cmdMessenger.attach(kInitModule, OnInitModule);
  cmdMessenger.attach(kSetModule, OnSetModule);
  cmdMessenger.attach(kSetModuleBrightness, OnSetModuleBrightness);
#endif

  cmdMessenger.attach(kSetPin, OnSetPin);

#if MF_STEPPER_SUPPORT == 1
  cmdMessenger.attach(kSetStepper, OnSetStepper);
#endif

#if MF_SERVO_SUPPORT == 1
  cmdMessenger.attach(kSetServo, OnSetServo);
#endif

  cmdMessenger.attach(kGetInfo, OnGetInfo);
  cmdMessenger.attach(kGetConfig, OnGetConfig);
  cmdMessenger.attach(kSetConfig, OnSetConfig);                 // 3rd step, uploading new config block wise
  cmdMessenger.attach(kResetConfig, OnResetConfig);             // 2nd step for uploading a new config
  cmdMessenger.attach(kSaveConfig, OnSaveConfig);               // 4th step, not really required anymore, config is stored directly to EEPROM, changes in UI required as feedback is required for now
  cmdMessenger.attach(kActivateConfig, OnActivateConfig);       // 5th step, reading config and activate
  cmdMessenger.attach(kSetName, OnSetName);                     // 1st step, write name
  cmdMessenger.attach(kGenNewSerial, OnGenNewSerial);

#if MF_STEPPER_SUPPORT == 1
  cmdMessenger.attach(kResetStepper, OnResetStepper);
  cmdMessenger.attach(kSetZeroStepper, OnSetZeroStepper);
#endif

  cmdMessenger.attach(kTrigger, OnTrigger);
//  cmdMessenger.attach(kResetBoard, OnResetBoard);               // why is this command coming from the UI additional to OnActivateConfig?

#if MF_LCD_SUPPORT == 1
  cmdMessenger.attach(kSetLcdDisplayI2C, OnSetLcdDisplayI2C);
#endif

#if MF_OUTPUT_SHIFTER_SUPPORT
  cmdMessenger.attach(kSetShiftRegisterPins, OnSetOutputShifterPins);
#endif

#ifdef DEBUG
  cmdMessenger.sendCmd(kStatus, F("Attached callbacks"));
#endif

}

// Callbacks that define what commands we issue upon internal events
void attachEventCallbacks()
{
  MFButton::attachHandler(handlerOnButton);
  MFEncoder::attachHandler(handlerOnEncoder);
#if MF_ANALOG_SUPPORT == 1
  MFAnalog::attachHandler(handlerOnAnalogChange);
#endif
}

void ResetBoard()
{
  MFeeprom.init();
  nameBuffer[0] = '\0';
  generateSerial(false);
  lastCommand = millis();
  loadConfig();
  _restoreName();
}
/*
void OnResetBoard()
{
  resetConfig();        // was part of loadConfig(), but not needed on initial start up
  ResetBoard();
}
*/
// Setup function
void setup()
{
  Serial.begin(115200);
  attachCommandCallbacks();
  attachEventCallbacks();
  cmdMessenger.printLfCr();
  ResetBoard();

// Time Gap between Inputs, do not read at the same loop
  lastButtonUpdate = millis();
  lastEncoderUpdate = millis() + 2;

#if MF_ANALOG_SUPPORT == 1
  lastAnalogAverage = millis() + 4;
  lastAnalogRead = millis() + 4;
#endif

#if MF_SERVO_SUPPORT == 1
  lastServoUpdate = millis();
#endif

#if MF_INPUT_SHIFTER_SUPPORT == 1
  lastInputShifterUpdate = millis() + 6;
#endif
}

void generateSerial(bool force)
{
  MFeeprom.read_block(MEM_OFFSET_SERIAL, serial, MEM_LEN_SERIAL);
  if (!force && serial[0] == 'S' && serial[1] == 'N')
    return;
  randomSeed(analogRead(RANDOM_SEED_INPUT));
  sprintf(serial, "SN-%03x-", (unsigned int)random(4095));
  sprintf(&serial[7], "%03x", (unsigned int)random(4095));
  MFeeprom.write_block(MEM_OFFSET_SERIAL, serial, MEM_LEN_SERIAL);
  if (!force)
    MFeeprom.write_byte(MEM_OFFSET_CONFIG, 0x00); // First byte of config to 0x00 to ensure to start 1st time with empty config, but not if forced from the connector to generate a new one
}

// reads the EEPRROM until NULL termination and returns the number of characters incl. NULL termination, starting from given address
bool readConfigLength()
{
  char temp = 0;
  uint16_t addreeprom = MEM_OFFSET_CONFIG;
  uint16_t length = MFeeprom.get_length();
  configLength = 0;
  do
  {
    temp = MFeeprom.read_char(addreeprom++);
    configLength++;
    if (addreeprom > length)                                      // abort if EEPROM size will be exceeded
    {
      cmdMessenger.sendCmd(kStatus, F("Loading config failed"));  // text or "-1" like config upload?
      return false;
    }
  }
  while (temp != 0x00);                                           // reads until NULL
  configLength--;
  return true;
}

void loadConfig()
{
#ifdef DEBUG
  cmdMessenger.sendCmd(kStatus, F("Restored config"));
  cmdMessenger.sendCmd(kStatus, nameBuffer);
#endif
  if (readConfigLength())
  {
    readConfig();
    _activateConfig();
  }
}

void SetPowerSavingMode(bool state)
{
  // disable the lights ;)
  powerSavingMode = state;

#if MF_SEGMENT_SUPPORT == 1
  PowerSaveLedSegment(state);
#endif

#ifdef DEBUG
  if (state)
    cmdMessenger.sendCmd(kStatus, F("On"));
  else
    cmdMessenger.sendCmd(kStatus, F("Off"));
#endif
  //PowerSaveOutputs(state);
}

void updatePowerSaving()
{
  if (!powerSavingMode && ((millis() - lastCommand) > (POWER_SAVING_TIME * 1000)))
  {
    // enable power saving
    SetPowerSavingMode(true);
  }
  else if (powerSavingMode && ((millis() - lastCommand) < (POWER_SAVING_TIME * 1000)))
  {
    // disable power saving
    SetPowerSavingMode(false);
  }
}

// Loop function
void loop()
{
  // Process incoming serial data, and perform callbacks
  cmdMessenger.feedinSerialData();
  updatePowerSaving();

  // if config has been reset
  // and still is not activated
  // do not perform updates
  // to prevent mangling input for config (shared buffers)
  if (!configActivated)
    return;

  readButtons();
  readEncoder();

#if MF_INPUT_SHIFTER_SUPPORT == 1
  readInputShifters();
#endif

#if MF_ANALOG_SUPPORT == 1
  readAnalog();
#endif

  // segments do not need update
#if MF_STEPPER_SUPPORT == 1
  updateSteppers();
#endif

#if MF_SERVO_SUPPORT == 1
  updateServos();
#endif
}

//// OUTPUT /////
void AddOutput(uint8_t pin = 1, char const *name = "Output")
{
  if (outputsRegistered == MAX_OUTPUTS)
    return;
  if (!FitInMemory(sizeof(MFOutput)))
  {
    // Error Message to Connector
    cmdMessenger.sendCmd(kStatus, F("Output does not fit in Memory"));
    return;
  }
  outputs[outputsRegistered] = new (allocateMemory(sizeof(MFOutput))) MFOutput(pin);
  outputsRegistered++;
#ifdef DEBUG
  cmdMessenger.sendCmd(kStatus, F("Added output"));
#endif
}

void ClearOutputs()
{
  outputsRegistered = 0;
#ifdef DEBUG
  cmdMessenger.sendCmd(kStatus, F("Cleared outputs"));
#endif
}

//// BUTTONS /////
void AddButton(uint8_t pin = 1, char const *name = "Button")
{
  if (buttonsRegistered == MAX_BUTTONS)
    return;
  if (!FitInMemory(sizeof(MFButton)))
  {
    // Error Message to Connector
    cmdMessenger.sendCmd(kStatus, F("Button does not fit in Memory"));
    return;
  }
  buttons[buttonsRegistered] = new (allocateMemory(sizeof(MFButton))) MFButton(pin, name);
  buttonsRegistered++;
#ifdef DEBUG
  cmdMessenger.sendCmd(kStatus, F("Added button ") /* + name */);
#endif
}

void ClearButtons()
{
  buttonsRegistered = 0;
#ifdef DEBUG
  cmdMessenger.sendCmd(kStatus, F("Cleared buttons"));
#endif
}

//// ENCODERS /////
void AddEncoder(uint8_t pin1 = 1, uint8_t pin2 = 2, uint8_t encoder_type = 0, char const *name = "Encoder")
{
  if (encodersRegistered == MAX_ENCODERS)
    return;

  if (!FitInMemory(sizeof(MFEncoder)))
  {
    // Error Message to Connector
    cmdMessenger.sendCmd(kStatus, F("Encoders does not fit in Memory"));
    return;
  }
  encoders[encodersRegistered] = new (allocateMemory(sizeof(MFEncoder))) MFEncoder;
  encoders[encodersRegistered]->attach(pin1, pin2, encoder_type, name);
  encodersRegistered++;
#ifdef DEBUG
  cmdMessenger.sendCmd(kStatus, F("Added encoder"));
#endif
}

void ClearEncoders()
{
  encodersRegistered = 0;
#ifdef DEBUG
  cmdMessenger.sendCmd(kStatus, F("Cleared encoders"));
#endif
}

#if MF_INPUT_SHIFTER_SUPPORT == 1
//// INPUT SHIFT REGISTER /////
void AddInputShifter(uint8_t latchPin, uint8_t clockPin, uint8_t dataPin, uint8_t modules, char const *name = "Shifter")
{
  if (inputShiftersRegistered == MAX_INPUT_SHIFTERS)
    return;
  if (!FitInMemory(sizeof(MFInputShifter)))
  {
    // Error Message to Connector
    cmdMessenger.sendCmd(kStatus, F("InputShifter does not fit in Memory"));
    return;
  }
  inputShifters[inputShiftersRegistered] = new (allocateMemory(sizeof(MFInputShifter))) MFInputShifter;
  inputShifters[inputShiftersRegistered]->attach(latchPin, clockPin, dataPin, modules, name);
  inputShifters[inputShiftersRegistered]->clear();
  inputShiftersRegistered++;
#ifdef DEBUG
  cmdMessenger.sendCmd(kStatus, F("Added input shifter"));
#endif
}

void ClearInputShifters()
{
  for (int i = 0; i < inputShiftersRegistered; i++)
  {
    inputShifters[i]->detach();
  }
  inputShiftersRegistered = 0;
#ifdef DEBUG
  cmdMessenger.sendCmd(kStatus, F("Cleared input shifter"));
#endif
}
#endif

//// OUTPUTS /////

#if MF_SEGMENT_SUPPORT == 1
//// SEGMENTS /////
void AddLedSegment(int dataPin, int csPin, int clkPin, int numDevices, int brightness)
{
  if (ledSegmentsRegistered == MAX_LEDSEGMENTS)
    return;

  ledSegments[ledSegmentsRegistered].attach(dataPin, csPin, clkPin, numDevices, brightness); // lc is our object
  ledSegmentsRegistered++;
#ifdef DEBUG
  cmdMessenger.sendCmd(kStatus, F("Added Led Segment"));
#endif
}

void ClearLedSegments()
{
  for (int i = 0; i != ledSegmentsRegistered; i++)
  {
    ledSegments[i].detach();
  }
  ledSegmentsRegistered = 0;
#ifdef DEBUG
  cmdMessenger.sendCmd(kStatus, F("Cleared segments"));
#endif
}

void PowerSaveLedSegment(bool state)
{
  for (int i = 0; i != ledSegmentsRegistered; ++i)
  {
    ledSegments[i].powerSavingMode(state);
  }

  for (int i = 0; i != outputsRegistered; ++i)
  {
    outputs[i]->powerSavingMode(state);
  }
}
#endif

#if MF_STEPPER_SUPPORT == 1
//// STEPPER ////
void AddStepper(int pin1, int pin2, int pin3, int pin4, int btnPin1)
{
  if (steppersRegistered == MAX_STEPPERS)
    return;
  steppers[steppersRegistered].attach(pin1, pin2, pin3, pin4, btnPin1);
  steppers[steppersRegistered].setMaxSpeed(STEPPER_SPEED);
  steppers[steppersRegistered].setAcceleration(STEPPER_ACCEL);
  // autoreset is not released yet
  if (btnPin1 > 0)
  {
    // this triggers the auto reset if we need to reset
    steppers[steppersRegistered].reset();
  }

  // all set
  steppersRegistered++;

#ifdef DEBUG
  cmdMessenger.sendCmd(kStatus, F("Added stepper"));
#endif
}

void ClearSteppers()
{
  for (int i = 0; i != steppersRegistered; i++)
  {
    steppers[i].detach();
  }
  steppersRegistered = 0;
#ifdef DEBUG
  cmdMessenger.sendCmd(kStatus, F("Cleared steppers"));
#endif
}
#endif

#if MF_SERVO_SUPPORT == 1
//// SERVOS /////
void AddServo(int pin)
{
  if (servosRegistered == MAX_MFSERVOS)
    return;
  servos[servosRegistered].attach(pin, true);
  servosRegistered++;
}

void ClearServos()
{
  for (int i = 0; i != servosRegistered; i++)
  {
    servos[i].detach();
  }
  servosRegistered = 0;
#ifdef DEBUG
  cmdMessenger.sendCmd(kStatus, F("Cleared servos"));
#endif
}
#endif

#if MF_LCD_SUPPORT == 1
//// LCD Display /////
void AddLcdDisplay(uint8_t address = 0x24, uint8_t cols = 16, uint8_t lines = 2)
{
  if (lcd_12cRegistered == MAX_MFLCD_I2C)
    return;
  lcd_I2C[lcd_12cRegistered].attach(address, cols, lines);
  lcd_12cRegistered++;
#ifdef DEBUG
  cmdMessenger.sendCmd(kStatus, F("Added lcdDisplay"));
#endif
}

void ClearLcdDisplays()
{
  for (int i = 0; i != lcd_12cRegistered; i++)
  {
    lcd_I2C[i].detach();
  }
  lcd_12cRegistered = 0;
#ifdef DEBUG
  cmdMessenger.sendCmd(kStatus, F("Cleared lcdDisplays"));
#endif
}
#endif

#if MF_ANALOG_SUPPORT == 1

void AddAnalog(uint8_t pin = 1, char const *name = "AnalogInput", uint8_t sensitivity = 3)
{
  if (analogRegistered == MAX_ANALOG_INPUTS)
    return;
  if (!FitInMemory(sizeof(MFAnalog)))
  {
    // Error Message to Connector
    cmdMessenger.sendCmd(kStatus, F("AnalogIn does not fit in Memory"));
    return;
  }
  analog[analogRegistered] = new (allocateMemory(sizeof(MFAnalog))) MFAnalog(pin, name, sensitivity);
  analogRegistered++;
#ifdef DEBUG
  cmdMessenger.sendCmd(kStatus, F("Added analog device "));
#endif
}

void ClearAnalog()
{
  analogRegistered = 0;
#ifdef DEBUG
  cmdMessenger.sendCmd(kStatus, F("Cleared analog devices"));
#endif
}

#endif

#if MF_OUTPUT_SHIFTER_SUPPORT == 1
//// SHIFT REGISTER /////
void AddShifter(uint8_t latchPin, uint8_t clockPin, uint8_t dataPin, uint8_t modules)
{
  if (outputShifterRegistered == MAX_OUTPUT_SHIFTERS)
    return;
  if (!FitInMemory(sizeof(MFOutputShifter)))
  {
    // Error Message to Connector
    cmdMessenger.sendCmd(kStatus, F("OutputShifter does not fit in Memory"));
    return;
  }
  outputShifters[outputShifterRegistered] = new (allocateMemory(sizeof(MFOutputShifter))) MFOutputShifter;
  outputShifters[outputShifterRegistered]->attach(latchPin, clockPin, dataPin, modules);
  outputShifters[outputShifterRegistered]->clear();
  outputShifterRegistered++;

#ifdef DEBUG
  cmdMessenger.sendCmd(kStatus, F("Added Output Shifter"));
#endif
}

void ClearOutputShifters()
{
  for (int i = 0; i != outputShifterRegistered; i++)
  {
    outputShifters[i]->detach();
  }

  outputShifterRegistered = 0;
#ifdef DEBUG
  cmdMessenger.sendCmd(kStatus, F("Cleared Output Shifter"));
#endif
}
#endif

//// EVENT HANDLER /////
void handlerOnButton(uint8_t eventId, uint8_t pin, const char *name)
{
  cmdMessenger.sendCmdStart(kButtonChange);
  cmdMessenger.sendCmdArg(name);
  cmdMessenger.sendCmdArg(eventId);
  cmdMessenger.sendCmdEnd();
};

//// EVENT HANDLER /////
void handlerOnEncoder(uint8_t eventId, uint8_t pin, const char *name)
{
  cmdMessenger.sendCmdStart(kEncoderChange);
  cmdMessenger.sendCmdArg(name);
  cmdMessenger.sendCmdArg(eventId);
  cmdMessenger.sendCmdEnd();
};

#if MF_INPUT_SHIFTER_SUPPORT == 1
//// EVENT HANDLER /////
void handlerInputShifterOnChange(uint8_t eventId, uint8_t pin, const char *name)
{
  cmdMessenger.sendCmdStart(kInputShifterChange);
  cmdMessenger.sendCmdArg(name);
  cmdMessenger.sendCmdArg(pin);
  cmdMessenger.sendCmdArg(eventId);
  cmdMessenger.sendCmdEnd();
};
#endif

//// EVENT HANDLER /////
void handlerOnAnalogChange(int value, uint8_t pin, const char *name)
{
  cmdMessenger.sendCmdStart(kAnalogChange);
  cmdMessenger.sendCmdArg(name);
  cmdMessenger.sendCmdArg(value);
  cmdMessenger.sendCmdEnd();
};

/**
 ** config stuff
 **/
void OnSetConfig()
{
#ifdef DEBUG
  cmdMessenger.sendCmd(kStatus, F("Setting config start"));
#endif
  lastCommand = millis();
  char *cfg = cmdMessenger.readStringArg();
  uint8_t cfgLen = strlen(cfg);

  if (configLength + cfgLen + 1 < MEM_LEN_CONFIG)
  {
    MFeeprom.write_block(MEM_OFFSET_CONFIG + configLength, cfg, cfgLen+1);    // save the received config string including the terminatung NULL (+1) to EEPROM
    configLength += cfgLen;
    cmdMessenger.sendCmd(kStatus, configLength);
  }
  else
    cmdMessenger.sendCmd(kStatus, -1);                             // last successfull saving block is already NULL terminated, nothing more todo
#ifdef DEBUG
  cmdMessenger.sendCmd(kStatus, F("Setting config end"));
#endif
}

void resetConfig()
{
  ClearButtons();
  ClearEncoders();
  ClearOutputs();

#if MF_SEGMENT_SUPPORT == 1
  ClearLedSegments();
#endif

#if MF_SERVO_SUPPORT == 1
  ClearServos();
#endif

#if MF_STEPPER_SUPPORT == 1
  ClearSteppers();
#endif

#if MF_LCD_SUPPORT == 1
  ClearLcdDisplays();
#endif

#if MF_ANALOG_SUPPORT == 1
  ClearAnalog();
#endif

#if MF_OUTPUT_SHIFTER_SUPPORT == 1
  ClearOutputShifters();
#endif

#if MF_INPUT_SHIFTER_SUPPORT == 1
  ClearInputShifters();
#endif

  ClearMemory();
  configLength = 0;
  configActivated = false;
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
uint8_t readUintFromEEPROM (volatile uint16_t *addreeprom) {
  char params[4] = {0};                                           // max 3 (255) digits NULL terminated
  uint8_t counter = 0;
  do
  {
    params[counter++] = MFeeprom.read_char((*addreeprom)++);      // read character from eeprom and locate next buffer and eeprom location
  }
  while ( params[counter-1] != '.' && counter < sizeof(params));  // reads until limiter '.' and for safety reason not more then size of params[]
  params[counter-1] = 0x00;                                       // replace '.' by NULL to terminate the string
  return atoi(params);
}

// reads a string from EEPROM at given address which is ':' terminated and saves it in the nameBuffer
// once the nameBuffer is not needed anymore, just read until the ":" termination -> see function below
bool readNameFromEEPROM(uint16_t *addreeprom, char* buffer, uint16_t *addrbuffer)   {
  char temp = 0;
  do
  {
    temp = MFeeprom.read_char((*addreeprom)++);                   // read the first character
      buffer[(*addrbuffer)++] = temp;                             // save character and locate next buffer position
      if (*addrbuffer >= MEMLEN_CONFIG_BUFFER) {                  // nameBuffer will be exceeded
        return false;                                             // abort copying from EEPROM to nameBuffer
      }
  } while (temp != ':');                                          // reads until limiter ':' and locates the next free buffer position
  buffer[(*addrbuffer)-1] = 0x00;                                 // replace ':' by NULL, terminates the string
  return true;
}

// reads the EEPRROM until end of command which ':' terminated
bool readEndCommandFromEEPROM(uint16_t *addreeprom)   {
  char temp = 0;
  uint16_t length = MFeeprom.get_length();
  do
  {
    temp = MFeeprom.read_char((*addreeprom)++);
    if (*addreeprom > length)                                     // abort if EEPROM size will be exceeded
      return false;
  }
  while (temp != ':');                                            // reads until limiter ':'
  return true;
}

void readConfig()
{
  if (configLength == 0)                                          // do nothing if no config is available   
    return;
  uint16_t addreeprom = MEM_OFFSET_CONFIG;                        // define first memory location where config is saved in EEPROM
  uint16_t addrbuffer = 0;                                        // and start with first memory location from nameBuffer
  char params[6] = "";
  char command = readUintFromEEPROM(&addreeprom);                 // read the first value from EEPROM, it's a device definition
  bool copy_success = true;                                       // will be set to false if copying input names to nameBuffer exceeds array dimensions
                                                                  // not required anymore when pins instead of names are transferred to the UI

  if (command == 0)                                               // just to be sure, configLength should also be 0
    return;

  do                                                              // go through the EEPROM until it is NULL terminated
  {
    switch (command)
    {
    case kTypeButton:
      params[0] = readUintFromEEPROM(&addreeprom);                // get the Pin number
      AddButton(params[0], &nameBuffer[addrbuffer]);              // MUST be before readNameFromEEPROM because readNameFromEEPROM returns the pointer for the NEXT Name
      copy_success = readNameFromEEPROM(&addreeprom, nameBuffer, &addrbuffer); // copy the NULL terminated name to to nameBuffer and set to next free memory location
      break;

    case kTypeOutput:
      params[0] = readUintFromEEPROM(&addreeprom);                // get the Pin number
      AddOutput(params[0]);
      copy_success = readEndCommandFromEEPROM(&addreeprom);       // check EEPROM until end of name
      break;

#if MF_SEGMENT_SUPPORT == 1
    case kTypeLedSegment:
      params[0] = readUintFromEEPROM(&addreeprom);                // get the Pin Data number
      params[1] = readUintFromEEPROM(&addreeprom);                // get the Pin CS number
      params[2] = readUintFromEEPROM(&addreeprom);                // get the Pin CLK number
      params[3] = readUintFromEEPROM(&addreeprom);                // get the brightness
      params[4] = readUintFromEEPROM(&addreeprom);                // get the number of modules
      AddLedSegment(params[0], params[1], params[2], params[4], params[3]);
      copy_success = readEndCommandFromEEPROM(&addreeprom);       // check EEPROM until end of name
      break;
#endif

#if MF_STEPPER_SUPPORT == 1
    case kTypeStepperDeprecated:
      // this is for backwards compatibility
      params[0] = readUintFromEEPROM(&addreeprom);                // get the Pin1 number
      params[1] = readUintFromEEPROM(&addreeprom);                // get the Pin2 number
      params[2] = readUintFromEEPROM(&addreeprom);                // get the Pin3 number
      params[3] = readUintFromEEPROM(&addreeprom);                // get the Pin4 number
      params[4] = readUintFromEEPROM(&addreeprom);                // get the Button number
      AddStepper(params[0], params[1], params[2], params[3], 0);
      copy_success = readEndCommandFromEEPROM(&addreeprom);       // check EEPROM until end of name
      break;
#endif

#if MF_STEPPER_SUPPORT == 1
    case kTypeStepper:
      params[0] = readUintFromEEPROM(&addreeprom);                // get the Pin1 number
      params[1] = readUintFromEEPROM(&addreeprom);                // get the Pin2 number
      params[2] = readUintFromEEPROM(&addreeprom);                // get the Pin3 number
      params[3] = readUintFromEEPROM(&addreeprom);                // get the Pin4 number
      params[4] = readUintFromEEPROM(&addreeprom);                // get the Button number
      AddStepper(params[0], params[1], params[2], params[3], params[4]);
      copy_success = readEndCommandFromEEPROM(&addreeprom);       // check EEPROM until end of name
      break;
#endif

#if MF_SERVO_SUPPORT == 1
    case kTypeServo:
      params[0] = readUintFromEEPROM(&addreeprom);                // get the Pin number
      AddServo(params[0]);
      copy_success = readEndCommandFromEEPROM(&addreeprom);       // check EEPROM until end of name
      break;
#endif

    case kTypeEncoderSingleDetent:
      params[0] = readUintFromEEPROM(&addreeprom);                // get the Pin1 number
      params[1] = readUintFromEEPROM(&addreeprom);                // get the Pin2 number
      AddEncoder(params[0], params[1], 0, &nameBuffer[addrbuffer]);             // MUST be before readNameFromEEPROM because readNameFromEEPROM returns the pointer for the NEXT Name
      copy_success = readNameFromEEPROM(&addreeprom, nameBuffer, &addrbuffer);  // copy the NULL terminated name to it and get the next free memory location
      break;

    case kTypeEncoder:
      params[0] = readUintFromEEPROM(&addreeprom);                // get the Pin1 number
      params[1] = readUintFromEEPROM(&addreeprom);                // get the Pin2 number
      params[2] = readUintFromEEPROM(&addreeprom);                // get the type
      AddEncoder(params[0], params[1], params[2], &nameBuffer[addrbuffer]);     // MUST be before readNameFromEEPROM because readNameFromEEPROM returns the pointer for the NEXT Name
      copy_success = readNameFromEEPROM(&addreeprom, nameBuffer, &addrbuffer);  // copy the NULL terminated name to to nameBuffer and set to next free memory location
      break;

#if MF_LCD_SUPPORT == 1
    case kTypeLcdDisplayI2C:
      params[0] = readUintFromEEPROM(&addreeprom);                // get the address
      params[1] = readUintFromEEPROM(&addreeprom);                // get the columns
      params[2] = readUintFromEEPROM(&addreeprom);                // get the lines
      AddLcdDisplay(params[0], params[1], params[2]);
      copy_success = readEndCommandFromEEPROM(&addreeprom);       // check EEPROM until end of name
      break;
#endif

#if MF_ANALOG_SUPPORT == 1
    case kTypeAnalogInput:
      params[0] = readUintFromEEPROM(&addreeprom);                // get the pin number
      params[1] = readUintFromEEPROM(&addreeprom);                // get the sensitivity
      AddAnalog(params[0], &nameBuffer[addrbuffer], params[1]);   // MUST be before readNameFromEEPROM because readNameFromEEPROM returns the pointer for the NEXT Name
      copy_success = readNameFromEEPROM(&addreeprom, nameBuffer, &addrbuffer);  // copy the NULL terminated name to to nameBuffer and set to next free memory location
      break;
#endif

#if MF_OUTPUT_SHIFTER_SUPPORT == 1
    case kShiftRegister:
      params[0] = readUintFromEEPROM(&addreeprom);                // get the latch Pin
      params[1] = readUintFromEEPROM(&addreeprom);                // get the clock Pin
      params[2] = readUintFromEEPROM(&addreeprom);                // get the data Pin
      params[3] = readUintFromEEPROM(&addreeprom);                // get the number of daisy chained modules
      AddShifter(params[0], params[1], params[2], params[3]);
      copy_success = readEndCommandFromEEPROM(&addreeprom);       // check EEPROM until end of name
      break;
#endif

#if MF_INPUT_SHIFTER_SUPPORT == 1
    case kTypeInputShifter:
      params[0] = readUintFromEEPROM(&addreeprom);                // get the latch Pin
      params[1] = readUintFromEEPROM(&addreeprom);                // get the clock Pin
      params[2] = readUintFromEEPROM(&addreeprom);                // get the data Pin
      params[3] = readUintFromEEPROM(&addreeprom);                // get the number of daisy chained modules
      AddInputShifter(params[0], params[1], params[2], params[3], &nameBuffer[addrbuffer]);
      copy_success = readNameFromEEPROM(&addreeprom, nameBuffer, &addrbuffer);  // copy the NULL terminated name to to nameBuffer and set to next free memory location
      break;
#endif

    default:
      copy_success = readEndCommandFromEEPROM(&addreeprom);       // check EEPROM until end of name
    }
    command = readUintFromEEPROM(&addreeprom);
  } while (command && copy_success);
  if (!copy_success) {                                            // too much/long names for input devices
    nameBuffer[MEMLEN_CONFIG_BUFFER-1] = 0x00;                    // terminate the last copied (part of) string with 0x00
  }
}

// Called when a received command has no attached function
void OnUnknownCommand()
{
  lastCommand = millis();
  cmdMessenger.sendCmd(kStatus, F("n/a"));
}

void OnGetInfo()
{
  lastCommand = millis();
  cmdMessenger.sendCmdStart(kInfo);
  cmdMessenger.sendCmdArg(F(MOBIFLIGHT_TYPE));
  cmdMessenger.sendCmdArg(name);
  cmdMessenger.sendCmdArg(serial);
  cmdMessenger.sendCmdArg(F(VERSION));
  cmdMessenger.sendCmdEnd();
}

void OnGetConfig()
{
  lastCommand = millis();
  cmdMessenger.sendCmdStart(kInfo);
  if (configLength > 0)
  {
    cmdMessenger.sendCmdArg(MFeeprom.read_char(MEM_OFFSET_CONFIG));
    for (uint16_t i = 1; i < configLength; i++)
    {
      cmdMessenger.sendArg(MFeeprom.read_char(MEM_OFFSET_CONFIG + i));
    }
  }
  cmdMessenger.sendCmdEnd();
}

// Callback function that sets led on or off
void OnSetPin()
{
  // Read led state argument, interpret string as boolean
  int pin = cmdMessenger.readInt16Arg();
  int state = cmdMessenger.readInt16Arg();
  // Set led
  analogWrite(pin, state);        // why does the UI sends the pin number and not the x.th output number like other devices?
//  outputs[pin]->set(state);
  lastCommand = millis();
}

#if MF_SEGMENT_SUPPORT == 1
void OnInitModule()
{
  int module = cmdMessenger.readInt16Arg();
  int subModule = cmdMessenger.readInt16Arg();
  int brightness = cmdMessenger.readInt16Arg();
  ledSegments[module].setBrightness(subModule, brightness);
  lastCommand = millis();
}

void OnSetModule()
{
  int module = cmdMessenger.readInt16Arg();
  int subModule = cmdMessenger.readInt16Arg();
  char *value = cmdMessenger.readStringArg();
  uint8_t points = (uint8_t)cmdMessenger.readInt16Arg();
  uint8_t mask = (uint8_t)cmdMessenger.readInt16Arg();
  ledSegments[module].display(subModule, value, points, mask);
  lastCommand = millis();
}

void OnSetModuleBrightness()
{
  int module = cmdMessenger.readInt16Arg();
  int subModule = cmdMessenger.readInt16Arg();
  int brightness = cmdMessenger.readInt16Arg();
  ledSegments[module].setBrightness(subModule, brightness);
  lastCommand = millis();
}

#endif

#if MF_OUTPUT_SHIFTER_SUPPORT == 1
void OnInitOutputShifter()
{
  int module = cmdMessenger.readInt16Arg();
  outputShifters[module]->clear();
  lastCommand = millis();
}

void OnSetOutputShifterPins()
{

  int module = cmdMessenger.readInt16Arg();
  char *pins = cmdMessenger.readStringArg();
  int value = cmdMessenger.readInt16Arg();
  outputShifters[module]->setPins(pins, value);
  lastCommand = millis();
}
#endif

#if MF_INPUT_SHIFTER_SUPPORT == 1
void OnInitInputShiftRegister()
{
  int module = cmdMessenger.readInt16Arg();
  inputShifters[module]->clear();
  lastCommand = millis();
}
#endif

#if MF_STEPPER_SUPPORT == 1
void OnSetStepper()
{
  int stepper = cmdMessenger.readInt16Arg();
  long newPos = cmdMessenger.readInt32Arg();

  if (stepper >= steppersRegistered)
    return;
  steppers[stepper].moveTo(newPos);
  lastCommand = millis();
}

void OnResetStepper()
{
  int stepper = cmdMessenger.readInt16Arg();

  if (stepper >= steppersRegistered)
    return;
  steppers[stepper].reset();
  lastCommand = millis();
}

void OnSetZeroStepper()
{
  int stepper = cmdMessenger.readInt16Arg();

  if (stepper >= steppersRegistered)
    return;
  steppers[stepper].setZero();
  lastCommand = millis();
}

void updateSteppers()
{
  for (int i = 0; i != steppersRegistered; i++)
  {
    steppers[i].update();
  }
}
#endif

#if MF_SERVO_SUPPORT == 1
void OnSetServo()
{
  int servo = cmdMessenger.readInt16Arg();
  int newValue = cmdMessenger.readInt16Arg();
  if (servo >= servosRegistered)
    return;
  servos[servo].moveTo(newValue);
  lastCommand = millis();
}

void updateServos()
{
  if (millis() - lastServoUpdate <= MF_SERVO_DELAY_MS)
    return;
  lastServoUpdate = millis();

  for (int i = 0; i != servosRegistered; i++)
  {
    servos[i].update();
  }
}
#endif

#if MF_LCD_SUPPORT == 1
void OnSetLcdDisplayI2C()
{
  int address = cmdMessenger.readInt16Arg();
  char *output = cmdMessenger.readStringArg();
  lcd_I2C[address].display(output);
  lastCommand = millis();
}
#endif

void readButtons()
{
  if (millis() - lastButtonUpdate < MF_BUTTON_DEBOUNCE_MS)
    return;
  lastButtonUpdate = millis();
  for (int i = 0; i != buttonsRegistered; i++)
  {
    buttons[i]->update();
  }
}

void readEncoder()
{
  if (millis() - lastEncoderUpdate < 1)
    return;
  lastEncoderUpdate = millis();
  for (int i = 0; i != encodersRegistered; i++)
  {
    encoders[i]->update();
  }
}

#if MF_INPUT_SHIFTER_SUPPORT == 1
void readInputShifters()
{
  if (millis() - lastInputShifterUpdate <= MF_BUTTON_DEBOUNCE_MS)
    return;
  lastInputShifterUpdate = millis();

  for (int i = 0; i != inputShiftersRegistered; i++)
  {
    inputShifters[i]->update();
  }
}
#endif

#if MF_ANALOG_SUPPORT == 1
void readAnalog()
{
  if (millis() - lastAnalogAverage > MF_ANALOGAVERAGE_DELAY_MS - 1)
  {
    for (int i = 0; i != analogRegistered; i++)
    {
      analog[i]->readBuffer();
    }
    lastAnalogAverage = millis();
  }
  if (millis() - lastAnalogRead < MF_ANALOGREAD_DELAY_MS)
    return;
  lastAnalogRead = millis();
  for (int i = 0; i != analogRegistered; i++)
  {
    analog[i]->update();
  }
}
#endif

void OnGenNewSerial()
{
  generateSerial(true);
//  cmdMessenger.sendCmdStart(kInfo);
//  cmdMessenger.sendCmdArg(serial);
//  cmdMessenger.sendCmdEnd();
  cmdMessenger.sendCmd(kInfo,serial);
}

void OnSetName()
{
  char *cfg = cmdMessenger.readStringArg();
  memcpy(name, cfg, MEM_LEN_NAME);
  _storeName();
  cmdMessenger.sendCmdStart(kStatus);
  cmdMessenger.sendCmdArg(name);
  cmdMessenger.sendCmdEnd();
}

void _storeName()
{
  char prefix[] = "#";
  MFeeprom.write_block(MEM_OFFSET_NAME, prefix, 1);
  MFeeprom.write_block(MEM_OFFSET_NAME + 1, name, MEM_LEN_NAME - 1);
}

void _restoreName()
{
  char testHasName[1] = "";
  MFeeprom.read_block(MEM_OFFSET_NAME, testHasName, 1);
  if (testHasName[0] != '#')
    return;

  MFeeprom.read_block(MEM_OFFSET_NAME + 1, name, MEM_LEN_NAME - 1);
}

void OnTrigger()
{
  // Trigger all button release events first...
  for (int i = 0; i != buttonsRegistered; i++)
  {
    buttons[i]->triggerOnRelease();
  }
  // ... then trigger all the press events
  for (int i = 0; i != buttonsRegistered; i++)
  {
    buttons[i]->triggerOnPress();
  }

// Retrigger all the input shifters. This automatically sends
// the release events first followed by press events.
#if MF_INPUT_SHIFTER_SUPPORT == 1
  for (int i = 0; i != inputShiftersRegistered; i++)
  {
    inputShifters[i]->retrigger();
  }
#endif
}
