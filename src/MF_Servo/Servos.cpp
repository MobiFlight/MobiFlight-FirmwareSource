//
// Servos.cpp
//
// (C) MobiFlight Project 2022
//

#include <Arduino.h>
#include "mobiflight.h"
#include "MFServo.h"

namespace Servos
{
    DEFINE_VT_STUBS(MFServo); // see IODevice.h

    void Add(uint8_t pin)
    {
        MFServo *MFS;

        Stowage.AddItem(&MFS);

        if (MFS) {
            MFS->attach(pin, true);
#ifdef DEBUG2CMDMESSENGER
            cmdMessenger.sendCmd(kDebug, F("Added Servo"));
        } else {
            cmdMessenger.sendCmd(kDebug, F("Servo: Memory full"));
#endif
        }
    }

    void OnSet(void)
    {
        MFServo *MFS;
        int      nServo = cmdMessenger.readInt16Arg();
        int      value  = cmdMessenger.readInt16Arg();
        MFS             = (MFServo *)(Stowage.getNth((uint8_t)nServo, kTypeServo));
        if (MFS) {
            MFS->setval(value);
            setLastCommandMillis();
        }
    }
} // namespace

// Servos.cpp
