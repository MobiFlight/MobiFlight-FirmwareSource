//
// MFDigInMux.cpp
//
// (C) MobiFlight Project 2022
//

#include "mobiflight.h"
#include "MFDigInMux.h"
#include "MFMuxDriver.h"

MFMuxDriver  *MFDigInMux::_MUX;

MuxDigInEvent MFDigInMux::_inputHandler = NULL;

MFDigInMux::MFDigInMux(void)
{
    _MUX   = NULL;
    _name  = "MUXDigIn";
    _flags = 0x00;
    setLazyMode(MUX_MODE_FAST);
    clear();
}

MFDigInMux::MFDigInMux(MFMuxDriver *MUX, const char *name)
    : _name(name)
{
    if (MUX) _MUX = MUX;
    _flags = 0x00;
    setLazyMode(MUX_MODE_FAST);
    clear();
}

void MFDigInMux::setMux(MFMuxDriver *MUX)
{
    _MUX = MUX;
}

// Registers a new MUX input block and configures the driver pins
void MFDigInMux::attach(uint8_t dataPin, bool halfSize, char const *name)
{
    // if(!_MUX) return;     // no need to check, the object can be set up in advance before the MUX is configured
    _dataPin = dataPin;
    _name    = name;
    _flags   = 0x00;
    if (halfSize) bitSet(_flags, MUX_HALFSIZE);
    pinMode(_dataPin, INPUT_PULLUP);
    bitSet(_flags, MUX_INITED);

    // Initialize all inputs with current status
    poll(DONT_TRIGGER, bitRead(_flags, MUX_LAZY));

}

void MFDigInMux::detach()
{
    if (bitRead(_flags, MUX_INITED)) {
        pinMode(_dataPin, INPUT_PULLUP);
        bitClear(_flags, MUX_INITED);
    }
}

// Reads the values from the attached modules, compares them to the previously
// read values, and calls the registered event handler for any inputs that
// changed from the previously read state.
void MFDigInMux::update()
{
    poll(DO_TRIGGER, bitRead(_flags, MUX_LAZY));
}

// Helper function for update() and retrigger()
void MFDigInMux::poll(bool doTrigger, bool isLazy)
{
    if (!_MUX) return;

    // Meaning of "Lazy mode" flag
    // ===========================
    //
    // Lazy mode ON:
    // MUX selector is set externally, normally at main loop level
    // (incremented sequentially at each pass)
    // Individual modules work in one of two ways:
    // 1. they must have an associate channel number (which may also be "any"),
    //    and only execute if that matches the current channel;
    // 2. account for current channel number in their internal working
    //    (e.g. for digital inputs, "place input bit in position #n").
    //
    // Lazy mode OFF (default):
    // Every block using the multiplexer sets its own selector value (or span of values).
    // MUX selector can have any value upon entry; it is saved and restored before exit.
    //
    // Each block can use its preferred mode, and blocks of both types can co-exist.

    uint8_t selMax = (bitRead(_flags, MUX_HALFSIZE) ? 8 : 16);

    if (!isLazy) {

        // "Fast" read:
        // scan all inputs right away

        uint16_t         currentState = 0x0000;
        volatile uint8_t pinVal;

        _MUX->saveChannel();
        for (uint8_t sel = selMax; sel > 0; sel--) {
            _MUX->setChannel(sel - 1);

            // Allow the output to settle from voltage transients:
            // transients towards 0 (GND) are negligible, but transients towards 1 (Vcc)
            // require a pullup to charge parasitic capacities.
            // These are examples of delay times measured for 0->1 transitions with different pull-ups:
            // integrated PU -> 1.4us
            // external, 10k -> 400ns
            // external, 4k7 -> 250ns
            // A digitalRead() takes about 5us, therefore even the integrated pullup should be sufficient;
            // for added safety, we perform one more (useless) digitalRead().
            // NB An external pullup (10k or 4k7) is recommended anyway for better interference immunity.

            pinVal = digitalRead(_dataPin);
            pinVal = digitalRead(_dataPin);
            // delayMicroseconds(5);  // This is overkill
            currentState <<= 1;
            currentState |= (pinVal ? 1 : 0);
        }
        _MUX->restoreChannel(); // tidy up

        if (_lastState != currentState) {
            if (doTrigger) detectChanges(_lastState, currentState);
            _lastState = currentState;
        }

    } else {

        // "Lazy" read:
        // read one more channel every time the method is invoked
        // (the corresponding event, if any, is generated immediately).
        // Relies on the MuxDriver to be set externally by the caller
        // (typically incremented at every main loop iteration).

        bool     chVal = (digitalRead(_dataPin) ? true : false);
        uint8_t  ch    = _MUX->getChannel() % selMax;
        uint16_t msk   = (0x0001 << ch);

        if (((_lastState & msk) != 0) != chVal) trigger(ch, chVal);

        if (chVal) {
            _lastState |= msk;
        } else {
            _lastState &= ~msk;
        }
    }
}

// Detects changes between the current state and the previously saved state
void MFDigInMux::detectChanges(uint16_t lastState, uint16_t currentState)
{
    if (!_MUX) return;
    uint8_t  selMax = (bitRead(_flags, MUX_HALFSIZE) ? 8 : 16);
    uint16_t diff   = lastState ^ currentState;
    for (uint8_t i = 0; i < selMax; i++) {
        if (diff & 0x0001) {
            trigger(i, ((currentState & 0x0001) != 0));
        }
        diff >>= 1;
        currentState >>= 1;
    }
}

// Reads the current state for all connected modules then fires
// 'release' events for every 'off' input, followed by
// 'press' events for every 'on' input.
// (Remember that 'off' inputs actually have physical status '1')
void MFDigInMux::retrigger()
{
    // The current state for all attached modules is stored,
    // so future update() calls will work off whatever was read by the
    // retrigger flow.
    poll(DONT_TRIGGER, false); // just read, do not retrigger

    // Pass 1/2: Trigger all the 'off' inputs (released buttons) first
    detectChanges(0x0000, _lastState);

    // Pass 2/2: Trigger all the 'on' inputs (pressed buttons)
    detectChanges(0xFFFF, _lastState);
}

// Triggers the event handler for the associated input channel
void MFDigInMux::trigger(uint8_t channel, bool state)
{
    if (!_MUX) return;
    if (!_inputHandler) return;
    (*_inputHandler)((state ? MuxDigInOnRelease : MuxDigInOnPress), channel, _name);
}

// Attaches a new event handler for the specified event.
void MFDigInMux::attachHandler(MuxDigInEvent newHandler)
{
    _inputHandler = newHandler;
}

// Clears the internal state
void MFDigInMux::clear()
{
    _lastState = 0;
}

void MFDigInMux::setLazyMode(bool mode)
{
    if (mode) {
        bitSet(_flags, MUX_LAZY);
    } else {
        bitClear(_flags, MUX_LAZY);
    }
}

// MFDigInMux.cpp