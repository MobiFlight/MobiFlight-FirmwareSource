#include <Arduino.h>
#include "mobiflight.h"
#include "commandmessenger.h"
#include "MFBoards.h"
#include "config.h"
#include "Button.h"
#include "Encoder.h"
#if MF_ANALOG_SUPPORT == 1
#include "Analog.h"
#endif
#if MF_INPUT_SHIFTER_SUPPORT == 1
#include "InputShifter.h"
#endif  
#include "Output.h"
#if MF_SEGMENT_SUPPORT == 1
#include "LedSegment.h"
#endif
#if MF_STEPPER_SUPPORT == 1
#include "Stepper.h"
#endif
#if MF_SERVO_SUPPORT == 1
#include "Servos.h"
#endif
#if MF_OUTPUT_SHIFTER_SUPPORT == 1
#include "OutputShifter.h"
#endif

#define MF_BUTTON_DEBOUNCE_MS         10    // time between updating the buttons
#define MF_SERVO_DELAY_MS             5     // time between servo updates
#define MF_ANALOGAVERAGE_DELAY_MS     10    // time between updating the analog average calculation
#define MF_ANALOGREAD_DELAY_MS        50    // time between sending analog values
#define MF_ENCODER_DEBOUNCE_MS        1     // time between updating encoders

bool powerSavingMode = false;
const unsigned long POWER_SAVING_TIME = 60 * 15; // in seconds
uint32_t lastButtonUpdate= 0;
uint32_t lastEncoderUpdate = 0;
#if MF_SERVO_SUPPORT == 1
uint32_t lastServoUpdate = 0;
#endif
#if MF_ANALOG_SUPPORT == 1
uint32_t lastAnalogAverage = 0;
uint32_t lastAnalogRead = 0;
#endif
#if MF_INPUT_SHIFTER_SUPPORT == 1
uint32_t lastInputShifterUpdate = 0;
#endif


// ************************************************************
// Power saving
// ************************************************************
void SetPowerSavingMode(bool state)
{
  // disable the lights ;)
  powerSavingMode = state;
  Output::PowerSave(state);
#if MF_SEGMENT_SUPPORT == 1
  LedSegment::PowerSave(state);
#endif

#ifdef DEBUG2CMDMESSENGER
  if (state)
    cmdMessenger.sendCmd(kStatus, F("On"));
  else
    cmdMessenger.sendCmd(kStatus, F("Off"));
#endif
}

void updatePowerSaving()
{
  if (!powerSavingMode && ((millis() - getLastCommandMillis()) > (POWER_SAVING_TIME * 1000)))
  {
    // enable power saving
    SetPowerSavingMode(true);
  }
  else if (powerSavingMode && ((millis() - getLastCommandMillis()) < (POWER_SAVING_TIME * 1000)))
  {
    // disable power saving
    SetPowerSavingMode(false);
  }
}

// ************************************************************
// Reset Board
// ************************************************************
void ResetBoard()
{
  generateSerial(false);
  setLastCommandMillis();
  loadConfig();
  _restoreName();
}

// ************************************************************
// Setup
// ************************************************************
void setup()
{
  Serial.begin(115200);
  attachCommandCallbacks();
  cmdMessenger.printLfCr();
  ResetBoard();

// Time Gap between Inputs, do not read at the same loop
  lastButtonUpdate = millis() + 0;
  lastEncoderUpdate = millis();           // encoders will be updated every 1ms

#if MF_SERVO_SUPPORT == 1
  lastServoUpdate = millis() + 2;
#endif

#if MF_ANALOG_SUPPORT == 1
  lastAnalogAverage = millis() + 4;
  lastAnalogRead = millis() + 4;
#endif

#if MF_INPUT_SHIFTER_SUPPORT == 1
  lastInputShifterUpdate = millis() + 6;
#endif

#if MF_SERVO_SUPPORT == 1
  lastServoUpdate = millis() + 8;
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
  if (getStatusConfig())
  {
    if (millis() - lastButtonUpdate >= MF_BUTTON_DEBOUNCE_MS)
    {
      lastButtonUpdate = millis();
      Button::read();
    }
    if (millis() - lastEncoderUpdate >= MF_ENCODER_DEBOUNCE_MS)
    {
      lastEncoderUpdate = millis();
      Encoder::read();
    }
#if MF_STEPPER_SUPPORT == 1
    Stepper::update();
#endif
#if MF_SERVO_SUPPORT == 1
    if (millis() - lastServoUpdate >= MF_SERVO_DELAY_MS)
    {
      lastServoUpdate = millis();
      Servos::update();
    }
#endif
#if MF_ANALOG_SUPPORT == 1
    if (millis() - lastAnalogRead >= MF_ANALOGREAD_DELAY_MS)
    {
      lastAnalogRead = millis();
      Analog::read();
    }
    if (millis() - lastAnalogAverage >= MF_ANALOGAVERAGE_DELAY_MS)
    {
      lastAnalogAverage = millis();
      Analog::readAverage();
    }
#endif
#if MF_INPUT_SHIFTER_SUPPORT == 1
    if (millis() - lastInputShifterUpdate >= MF_ENCODER_DEBOUNCE_MS)
    {
      lastInputShifterUpdate = millis();
      InputShifter::read();
    }
#endif
    // lcds, outputs, outputshifters, segments do not need update
  }
}
