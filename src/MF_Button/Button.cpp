//
// Button.cpp
//
// (C) MobiFlight Project 2022
//

#include <Arduino.h>
#include "mobiflight.h"
#include "MFButton.h"

extern CmdMessenger cmdMessenger;
extern StowManager  Stowage;

namespace Button
{
    DEFINE_VT_STUBS(MFButton); // see IODevice.h

    void OnChange(uint8_t eventId, uint8_t pin, const char *name)
    {
        cmdMessenger.sendCmdStart(kButtonChange);
        cmdMessenger.sendCmdArg(name);
        cmdMessenger.sendCmdArg(eventId);
        cmdMessenger.sendCmdEnd();
    };

    void Add(uint8_t pin, char const *name)
    {
        MFButton *MFB;
        Stowage.AddItem(&MFB);
        // Non-templated alternative would be:
        // MFB = (MFButton *)Stowage.add(sizeof(MFButton));
        // if(MFB) new ((void *)MFB) MFButton;

        if (MFB) {
            MFB->attach(pin, name);
            MFButton::attachHandler(OnChange);
#ifdef DEBUG2CMDMESSENGER
            cmdMessenger.sendCmd(kDebug, F("Added Button"));
        } else {
            cmdMessenger.sendCmd(kDebug, F("Button: Memory full"));
#endif
        }
    }
} // namespace

// Button.cpp