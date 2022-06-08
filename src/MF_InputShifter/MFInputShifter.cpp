//
// MFInputShifter.cpp
//
// (C) MobiFlight Project 2022
//

#include "MFInputShifter.h"
#include "mobiflight.h"

inputShifterEventHandler MFInputShifter::_handler = NULL;

MFInputShifter::MFInputShifter(void)
{
    _moduleCount = 0;
    clearLastState();
}

// Registers a new input shifter and configures the clock, data and latch pins as well
// as the number of modules to read from.
void MFInputShifter::attach(uint8_t latchPin, uint8_t clockPin, uint8_t dataPin, uint8_t moduleCount, const char *name)
{
    _latchPin    = latchPin;
    _clockPin    = clockPin;
    _dataPin     = dataPin;
    _name        = name;
    _moduleCount = moduleCount;

    pinMode(_latchPin, OUTPUT);
    pinMode(_clockPin, OUTPUT);
    pinMode(_dataPin, INPUT);
}

// Reads the values from the attached modules, compares them to the previously
// read values, and calls the registered event handler for any inputs that
// changed from the previously read state.
void MFInputShifter::update()
{
    update(true);
}

// Reads the values from the attached modules, compares them to the previously
// read values, and (if doTrigger == true) calls the registered event handler 
// for any inputs that changed from the previously read state.
void MFInputShifter::update(uint8_t doTrigger)
{
    if(0 == _moduleCount) return;
    digitalWrite(_clockPin, HIGH); // Preset clock to retrieve first bit
    digitalWrite(_latchPin, HIGH); // Disable input latching and enable shifting

    // Multiple chained modules are handled one at a time. As shiftIn() keeps getting
    // called it will pull in the data from each chained module.
    for (uint8_t i = 0; i < _moduleCount; i++) {
        uint8_t currentState;

        currentState = shiftIn(_dataPin, _clockPin, MSBFIRST);

        // If an input changed on the current module from the last time it was read
        // then hand it off to figure out which bits specifically changed.
        if (doTrigger && currentState != _lastState[i]) {
            detectChanges(_lastState[i], currentState, i);
        _lastState[i] = currentState;
        }
    }

    digitalWrite(_latchPin, LOW); // disable shifting and enable input latching
}

// Detects changes between the current state and the previously saved state
// of a byte's worth of input.
void MFInputShifter::detectChanges(uint8_t lastState, uint8_t currentState, uint8_t module)
{
    uint8_t diff = lastState ^ currentState;
    for (uint8_t i = 0; i < 8; i++) {
        // If last and current input state for the bit are different
        // then the input changed and the handler for the bit needs to fire
        if (diff & 0x01) {
            // When triggering event the pin is the actual pin on the chip, offset by 8 bits for each
            // module beyond the first that it's on. The state of the trigger is the bit currently
            // in position 0 of currentState.
            trigger(i + (module * 8), ((currentState & 0x01)!=0));
        }

        diff >>= 1;
        currentState >>= 1;
    }
}

void MFInputShifter::reset(uint8_t action)
{
    // Handle retrigger logic according to:
    // https://github.com/MobiFlight/MobiFlight-Connector/issues/497
    // and  https://github.com/MobiFlight/MobiFlight-Connector/pull/502.
    
    // uint8_t pin;
    uint8_t state;

    if (NULL == _handler)   return;
    if (_moduleCount == 0)  return;

    if(action == ONRESET_RELEASE) {
        update(false);
    }

    for (int module = 0; module < _moduleCount; module++) {
        state = _lastState[module];

        if(action == ONRESET_RELEASE) {
            // Pass 1/2: Trigger all the 'off' inputs (released buttons) first
            detectChanges(0x00, state, module);  
        } else 
        if(action == ONRESET_PRESS) {
            // Pass 2/2: Trigger all the 'on' inputs (pressed buttons)
            detectChanges(0xFF, state, module);  
        }
    }
}

// Triggers the event handler for the associated input shift register pin,
// if a handler is registered.
void MFInputShifter::trigger(uint8_t pin, bool state)
{
    if (_handler) {
        (*_handler)((state ? inputShifterOnPress : inputShifterOnRelease), pin, _name);
    }
}

void MFInputShifter::detach(void)
{
    _moduleCount = 0;
}

// Clears the internal state of the shifter, including all received bits
// and the timestamp for the last time the data was read.
// Sets the last recorded state of every bit on every shifter to 0.
void MFInputShifter::clearLastState()
{
    for (uint8_t i = 0; i < MAX_CHAINED_INPUT_SHIFTERS; i++) {
        _lastState[i] = 0;
    }
}

// MFInputShifter.cpp
