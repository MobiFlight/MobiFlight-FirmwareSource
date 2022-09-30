//
// Output.cpp
//
// (C) MobiFlight Project 2022
//

#include <Arduino.h>
#include "mobiflight.h"
#include "MFOutput.h"

namespace Output
{
    DEFINE_VT_STUBS(MFOutput); // see IODevice.h

    void Add(uint8_t pin)
    {
        MFOutput *MFO;

        Stowage.AddItem(&MFO);

        if (MFO) {
            MFO->attach(pin);

#ifdef DEBUG2CMDMESSENGER
            cmdMessenger.sendCmd(kDebug, F("Added Output"));
        } else {
            cmdMessenger.sendCmd(kDebug, F("MFoutput: Memory full"));
#endif
        }
    }

    void OnSet(void)
    {
        // MFOutput *MFO;
        uint8_t nOutput = cmdMessenger.readInt16Arg();
        uint8_t state   = cmdMessenger.readInt16Arg(); // interpret string as boolean

        // MFO = (MFOutput *)(Stowage.getNth((uint8_t)nOutput, kTypeOutput));

        // CURRENTLY INCORRECT:
        // Here, "Pin" is the actual output pin, NOT the object index!!!
        // Patch:
        // Build a dummy object just for the purpose of setting the value
        MFOutput MFOut;
        MFOut.attach(nOutput);

        // Once the situation is rectified, uncomment statements below:
        // if(MFO) {
        // MFO->setval(state);       //analogWrite(pin, state);
        MFOut.setval(state);
        setLastCommandMillis();
        // }
    }
} // namespace

// Output.cpp
