//
// MFInputShifter.cpp
//
// (C) MobiFlight Project 2022
//

#include "MFInputShifter.h"

inputShifterEvent MFInputShifter::_inputHandler = NULL;

MFInputShifter::MFInputShifter(const char *name)
{
    _initialized = false;
    _name        = name;
}


inline void MFInputShifter::Pin_HIGH(volatile uint8_t *PinPort, uint8_t PinMask)
{
    *PinPort |= PinMask;
}
inline void MFInputShifter::Pin_LOW(volatile uint8_t *PinPort, uint8_t PinMask)
{
    *PinPort &= ~PinMask;
}

inline void MFInputShifter::latchPin_HIGH(void)
{
    *_latchPinPort |= _latchPinMask;
}
inline void MFInputShifter::latchPin_LOW(void)
{
    *_latchPinPort &= ~_latchPinMask;
}

inline void MFInputShifter::clockPin_HIGH(void)
{
    *_clockPinPort |= _clockPinMask;
}
inline void MFInputShifter::clockPin_LOW(void)
{
    *_clockPinPort &= ~_clockPinMask;
}

inline uint8_t MFInputShifter::dataPin_READ(void)
{
    if (*_dataPinPort & _dataPinMask) return HIGH;
    return LOW;
}

// Registers a new input shifter and configures the clock, data and latch pins as well
// as the number of modules to read from.
void MFInputShifter::attach(uint8_t latchPin, uint8_t clockPin, uint8_t dataPin, uint8_t moduleCount, const char *name)
{
    _latchPinPort = portOutputRegister(digitalPinToPort(latchPin));
    _latchPinMask = digitalPinToBitMask(latchPin);
    _clockPinPort = portOutputRegister(digitalPinToPort(clockPin));
    _clockPinMask = digitalPinToBitMask(clockPin);
    _dataPinPort  = portInputRegister(digitalPinToPort(dataPin));
    _dataPinMask  = digitalPinToBitMask(dataPin);
    _name         = name;
    _moduleCount  = moduleCount;

    pinMode(latchPin, OUTPUT);
    pinMode(clockPin, OUTPUT);
    pinMode(dataPin, INPUT);
    _initialized = true;

    // And now initialize all buttons with the actual status
    poll(DONT_TRIGGER);
}

// Reads the values from the attached modules, compares them to the previously
// read values, and calls the registered event handler for any inputs that
// changed from the previously read state.
void MFInputShifter::update()
{
    poll(DO_TRIGGER);
}

void MFInputShifter::poll(uint8_t doTrigger)
{
uint32_t millisStop = 0;
Serial.println("Start reading InputShifters 100 times");
uint32_t millisStart = millis();
for (uint8_t test = 0; test < 100; test++) {
    //clockPin_HIGH(); // Preset clock to retrieve first bit
    Pin_HIGH(_clockPinPort, _clockPinMask);
    //latchPin_HIGH(); // Disable input latching and enable shifting
    Pin_HIGH(_latchPinPort, _latchPinMask);
    // Multiple chained modules are handled one at a time. As shiftIn() keeps getting
    // called it will pull in the data from each chained module.
    for (uint8_t module = 0; module < _moduleCount; module++) {
        uint8_t currentState = 0;
        for (uint8_t i = 0; i < 8; ++i) {
            clockPin_HIGH();
            currentState |= dataPin_READ() << i;
            clockPin_LOW();
        }
        // If an input changed on the current module from the last time it was read
        // then hand it off to figure out which bits specifically changed.
        if (currentState != _lastState[module]) {
            if (doTrigger) detectChanges(_lastState[module], currentState, module);
            _lastState[module] = currentState;
        }
    }
    //latchPin_LOW();
    Pin_LOW(_latchPinPort, _latchPinMask);
}
millisStop = millis() - millisStart;
Serial.print("Stop reading InputShifters 100 times, took:"); Serial.print(millisStop); Serial.println("ms");
}

// Detects changes between the current state and the previously saved state
// of a byte's worth of input.
void MFInputShifter::detectChanges(uint8_t lastState, uint8_t currentState, uint8_t module)
{
    for (uint8_t i = 0; i < 8; i++) {
        // If last and current input state for the bit are different
        // then the input changed and the handler for the bit needs to fire
        if ((lastState & 1) ^ (currentState & 1)) {
            // When triggering event the pin is the actual pin on the chip offset by 8 bits for each
            // module beyond the first that it's on. The state of the trigger is the bit currently
            // in position 0 of currentState.
            trigger(i + (module * 8), currentState & 1);
        }

        lastState    = lastState >> 1;
        currentState = currentState >> 1;
    }
}

// Reads the current state for all connected modules then fires
// release events for every released button followed by
// press events for every pressed button.
void MFInputShifter::retrigger()
{
    uint8_t state;

    poll(DONT_TRIGGER);

    // Trigger all the released buttons
    for (int module = 0; module < _moduleCount; module++) {
        state = _lastState[module];
        for (uint8_t i = 0; i < 8; i++) {
            // Only trigger if the button is in the off position
            if (state & 1) {
                trigger(i + (module * 8), HIGH);
            }
            state = state >> 1;
        }
    }

    // Trigger all the pressed buttons
    for (int module = 0; module < _moduleCount; module++) {
        state = _lastState[module];
        for (uint8_t i = 0; i < 8; i++) {
            // Only trigger if the button is in the on position
            if (!(state & 1)) {
                trigger(i + (module * 8), LOW);
            }

            state = state >> 1;
        }
    }
}

// Triggers the event handler for the associated input shift register pin,
// if a handler is registered.
void MFInputShifter::trigger(uint8_t pin, bool state)
{
    if (!_inputHandler) return;
    (*_inputHandler)((state == LOW ? inputShifterOnPress : inputShifterOnRelease), pin, _name);
}

// Attaches a new event handler for the specified event.
void MFInputShifter::attachHandler(inputShifterEvent newHandler)
{
    _inputHandler = newHandler;
}

void MFInputShifter::detach()
{
    _initialized = false;
}

// MFInputShifter.cpp
