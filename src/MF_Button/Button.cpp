//
// Button.cpp
//
// (C) MobiFlight Project 2022
//

#include "mobiflight.h"
#include "MFButton.h"
#include "Button.h"

namespace Button
{
    MFButton *buttons[MAX_BUTTONS];
    uint8_t   buttonsRegistered = 0;

    void      handlerOnButton(uint8_t eventId, uint8_t pin, const char *name)
    {
        cmdMessenger.sendCmdStart(kButtonChange);
        cmdMessenger.sendCmdArg(name);
        cmdMessenger.sendCmdArg(eventId);
        cmdMessenger.sendCmdEnd();
    };

    void Add(uint8_t pin, char const *name)
    {
        if (buttonsRegistered == MAX_BUTTONS)
            return;

        if (!FitInMemory(sizeof(MFButton))) {
            // Error Message to Connector
            cmdMessenger.sendCmd(kStatus, F("Button does not fit in Memory"));
            return;
        }
        buttons[buttonsRegistered] = new (allocateMemory(sizeof(MFButton))) MFButton(pin, name);
        MFButton::attachHandler(handlerOnButton);
        buttonsRegistered++;
#ifdef DEBUG2CMDMESSENGER
        cmdMessenger.sendCmd(kDebug, F("Added button ") /* + name */);
#endif
    }

    void Clear(void)
    {
        buttonsRegistered = 0;
#ifdef DEBUG2CMDMESSENGER
        cmdMessenger.sendCmd(kDebug, F("Cleared buttons"));
#endif
    }

    void read(void)
    {
        for (uint8_t i = 0; i < buttonsRegistered; i++) {
            buttons[i]->update();
        }
    }

    void OnTrigger(void)
    {
        // Trigger all button release events first...
        for (uint8_t i = 0; i < buttonsRegistered; i++) {
            buttons[i]->triggerOnRelease();
        }
        // ... then trigger all the press events
        for (uint8_t i = 0; i < buttonsRegistered; i++) {
            buttons[i]->triggerOnPress();
        }
    }
} // namespace button

// Button.cpp
