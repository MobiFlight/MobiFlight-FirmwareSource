//
// mobiflight.gcc
//
// (C) MobiFlight Project 2022
// 


//#define DEBUG2MSG 1

//#define TESTING

#include <Arduino.h>
//#include "allocateMem.h"
#include "MFBoards.h"
#include "MFEEPROM.h"
#include "config.h"
#include "mobiflight.h"

#define MF_BUTTON_DEBOUNCE_MS 10     // time between updating the buttons
#define MF_ENCODER_DEBOUNCE_MS 1     // time between encoder updates
#define MF_INSHIFTER_POLL_MS 10      // time between input shift reg updates
#define MF_INMUX_POLL_MS 10          // time between dig input mux updates
#define MF_SERVO_DELAY_MS 5          // time between servo updates
#define MF_ANALOGAVERAGE_DELAY_MS 10 // time between updating the analog average calculation
#define MF_ANALOGREAD_DELAY_MS 50    // time between sending analog values

bool powerSavingMode = false;
const unsigned long POWER_SAVING_TIME = 60 * 15; // in seconds

uint32_t lastButtonUpdate = 0;
uint32_t lastEncoderUpdate = 0;
// ==================================================
//   Polling interval counters
// ==================================================

typedef struct {
    uint32_t Buttons = 0;
    uint32_t Encoders = 0;
#if MF_SERVO_SUPPORT == 1
    uint32_t Servos = 0;
#endif
#if MF_ANALOG_SUPPORT == 1
    uint32_t AnalogAverage = 0;
    uint32_t Analog = 0;
#endif
#if MF_INPUT_SHIFTER_SUPPORT == 1
    uint32_t InputShifters = 0;
#endif
#if MF_DIGIN_MUX_SUPPORT == 1
    uint32_t DigInMux = 0;
#endif
} lastUpdate_t;

lastUpdate_t lastUpdate;

void initPollIntervals(void)
{
    // Init Time Gap between Inputs, do not read at the same loop
    lastUpdate.Buttons = millis();
    lastUpdate.Encoders = millis();
#if MF_SERVO_SUPPORT == 1
    lastUpdate.Servos = millis() + 2;
#endif
#if MF_ANALOG_SUPPORT == 1
    lastUpdate.AnalogAverage = millis() + 4;
    lastUpdate.Analog = millis() + 4;
#endif
#if MF_INPUT_SHIFTER_SUPPORT == 1
    lastUpdate.InputShifters = millis() + 6;
#endif
#if MF_DIGIN_MUX_SUPPORT == 1
    lastUpdate.DigInMux = millis() + 8;
#endif

}

static void updateDevices(uint8_t);

void timedUpdate(uint8_t typ, uint32_t *tim, uint32_t intv)
{
    if (millis() - (*tim) >= (intv)) { 
        *tim = millis();              
        updateDevices(typ);           
    }                                 
}


// ************************************************************
//  General I/O handling functions
// ************************************************************


void resetDevices(void)
{
    uint8_t  typ;
    uint8_t* dev;

    // Trigger all release events first for inputs, does nothing for outputs
    Stowage.reset();
    while((dev = Stowage.getNext())) {        
        DeviceReset(typ, dev, ONRESET_RELEASE);
    };


    // ...then trigger all the press events for inputs, and clear outputs
    Stowage.reset();
    while((dev = Stowage.getNext())) {        
        DeviceReset(typ, dev, ONRESET_PRESS);
    };
    setLastCommandMillis();
}

void updateDevices(uint8_t typ = StowManager::TypeALL)
{
    uint8_t* dev;

    Stowage.reset();
    while((dev = Stowage.getNext())) {        
        DeviceUpdate(typ, dev);

    };
    setLastCommandMillis();
}

void setPowerSave(uint8_t mode, uint8_t typ = StowManager::TypeALL)
{
    uint8_t* dev;
    Stowage.reset();
    while((dev = Stowage.getNext())) {        
        DevicePowerSave(typ, dev, mode);
    };
    setLastCommandMillis();
}

void wipeDevices(void)
{
    // Reset device storage (this will do all devices)
    uint8_t* dev;
    uint8_t  typ;
    
    Stowage.reset();
    while((dev = Stowage.getNext())) {        
        DeviceDetach(typ, dev);
    };
    Stowage.wipe();
}

// ************************************************************
// Power save management
// ************************************************************
void setPowerSavingMode(bool state)
{
    // disable the lights ;)
    powerSavingMode = state;

    setPowerSave(state);

    #ifdef DEBUG2MSG
    if (state) {
        cmdMessenger.sendCmd(kStatus, F("On"));
    } else {
        cmdMessenger.sendCmd(kStatus, F("Off"));
    }
    #endif
}

void updatePowerSaving()
{
    if (!powerSavingMode && ((millis() - getLastCommandMillis()) > (POWER_SAVING_TIME * 1000))) {
        setPowerSavingMode(true);
    } else if (powerSavingMode && ((millis() - getLastCommandMillis()) < (POWER_SAVING_TIME * 1000))) {
        setPowerSavingMode(false);
    }
}

// ************************************************************
// TEST FUNCTIONS
// ************************************************************

void printItemSize(void)
{
#ifdef TESTING

#define TST_WRITE(s, d)                     \
    {                                       \
        sprintf(buf, fmt, s, d);            \
        cmdMessenger.sendCmd(kStatus, buf); \
        delay(100);                         \
    }

    char buf[40];
    const char *fmt = "Actual size of each <%s>: %d bytes";
    // The actual size in the buffer of each device is as follows:
    // <n> bytes for the "bare" class attributes
    // + 1 bytes for buffer link pointer (increment to next element in buffer)
    // + 1 byte  for type code (allowing to cast the object to the correct type)
    // First component is included in the value returned by sizeof(),
    // other 2 component are added explicitly below
    TST_WRITE("Output", sizeof(MFOutput) + 2);
    TST_WRITE("Button", sizeof(MFButton) + 2);
    TST_WRITE("Encoder", sizeof(MFEncoder) + 2);
    TST_WRITE("AnalogIn", sizeof(MFAnalog) + 2);
    TST_WRITE("InShiftReg", sizeof(MFInputShifter) + 2);
    TST_WRITE("LEDsegment", sizeof(MFSegments) + 2);
    TST_WRITE("Stepper", sizeof(MFStepper) + 2);
    TST_WRITE("Servo", sizeof(MFServo) + 2);
    TST_WRITE("OutShiftReg", sizeof(MFOutputShifter) + 2);
    TST_WRITE("LCDdisplay", sizeof(MFLCDDisplay) + 2);
#endif
}

void printReport(uint8_t nItems, const char *itemName)
{
#ifdef TESTING
    char buf[40];
    sprintf(buf, "Added %d %s", nItems, itemName);
    cmdMessenger.sendCmd(kStatus, buf);
    sprintf(buf, " used %d bytes, remaining %d",
            Stowage.getUsedSize(),
            Stowage.getFreeSize());
    cmdMessenger.sendCmd(kStatus, buf);
    delay(1000);
#endif
}

void setupData(void)
{
#ifdef TESTING
    uint8_t i;
    delay(1000);

    printItemSize();

    Stowage.wipe();
    printReport(0, "-none-");

    // #define MAX_OUTPUTS 40
    for (i = 0; i < MAX_OUTPUTS; i++)
        AddOutput(1);
    printReport(MAX_OUTPUTS, "Outputs");

    // #define MAX_BUTTONS 68
    for (i = 0; i < MAX_BUTTONS; i++)
        AddButton(1);
    printReport(MAX_BUTTONS, "Buttons");

    // #define MAX_ENCODERS 20
    for (i = 0; i < MAX_ENCODERS; i++)
        AddEncoder(1, 2, 1);
    printReport(MAX_ENCODERS, "Encoders");

    // #define MAX_ANALOG_INPUTS 16
    for (i = 0; i < MAX_ANALOG_INPUTS; i++)
        AddAnalog(1);
    printReport(MAX_ANALOG_INPUTS, "AnalogIns");

    // #define MAX_INPUT_SHIFTERS 4
    for (i = 0; i < MAX_INPUT_SHIFTERS; i++)
        AddInputShiftReg(1, 2, 3, 2);
    printReport(MAX_INPUT_SHIFTERS, "InputShiftRegs");

    // #define MAX_LEDSEGMENTS 4
    for (i = 0; i < MAX_LEDSEGMENTS; i++)
        AddLedSegment(1, 2, 3, 2, 15);
    printReport(MAX_LEDSEGMENTS, "LEDSegments");

    // #define MAX_STEPPERS 10
    for (i = 0; i < MAX_STEPPERS; i++)
        AddStepper(1, 2, 3, 4, 5);
    printReport(MAX_STEPPERS, "Steppers");

    // #define MAX_MFSERVOS 10
    for (i = 0; i < MAX_MFSERVOS; i++)
        AddServo(1);
    printReport(MAX_MFSERVOS, "Servos");

    // #define MAX_OUTPUT_SHIFTERS 4
    for (i = 0; i < MAX_OUTPUT_SHIFTERS; i++)
        AddOutShiftReg(1, 2, 3, 2);
    printReport(MAX_OUTPUT_SHIFTERS, "OutShiftRegs");

    // #define MAX_MFLCD_I2C 2
    for (i = 0; i < MAX_MFLCD_I2C; i++)
        AddLcdDisplay(1, 40, 2);
    printReport(MAX_MFLCD_I2C, "LCD displays");

#endif
}


// ************************************************************
// Reset Board
// ************************************************************
void resetBoard()
{
    generateSerial(false);
    setLastCommandMillis();
    loadConfig();
}


// ************************************************************
// Setup
// ************************************************************
void setup()
{
    Serial.begin(115200);
#if MF_DIGIN_MUX_SUPPORT == 1
    MFDigInMux::setMux(&MUX);
#endif
    attachCommandCallbacks();
    //attachEventCallbacks();

    cmdMessenger.printLfCr();
    resetBoard();
    initPollIntervals();
    resetDevices();

#ifdef TESTING
    while (1) {
        setupData();
        delay(60000);
    }
#endif
}

// ************************************************************
// Loop function
// ************************************************************
void loop()
{
    // Process incoming serial data, and perform callbacks
    cmdMessenger.feedinSerialData();
    updatePowerSaving();

    // if config has been reset and still is not activated
    // do not perform updates
    // to prevent mangling input for config (shared buffers)
    if (getStatusConfig()) {

        MUX.nextChannel();

        timedUpdate(kTypeButton, &lastUpdate.Buttons, MF_BUTTON_DEBOUNCE_MS);

        timedUpdate(kTypeEncoder, &lastUpdate.Encoders, MF_ENCODER_DEBOUNCE_MS);

#if MF_STEPPER_SUPPORT == 1
        updateDevices(kTypeStepper);
#endif
#if MF_SERVO_SUPPORT == 1
        timedUpdate(kTypeServo, &lastUpdate.Servos, MF_SERVO_DELAY_MS);
#endif
#if MF_ANALOG_SUPPORT == 1
        timedUpdate(kTypeAnalogInput, &lastUpdate.Analog, MF_ANALOGREAD_DELAY_MS);
        if (millis() - lastUpdate.AnalogAverage >= MF_ANALOGAVERAGE_DELAY_MS) {
            lastUpdate.AnalogAverage = millis();
            Analog::UpdateAverage();
        }
#endif
#if MF_INPUT_SHIFTER_SUPPORT == 1
        timedUpdate(kTypeInShiftReg, &lastUpdate.InputShifters, MF_INSHIFTER_POLL_MS);
#endif

#if MF_DIGIN_MUX_SUPPORT == 1
        timedUpdate(kTypeDigInMux, &lastUpdate.DigInMux, MF_INMUX_POLL_MS);
#endif
        // lcds, outputs, outputshifters, segments do not need update
    }
}

// mobiflight.cpp