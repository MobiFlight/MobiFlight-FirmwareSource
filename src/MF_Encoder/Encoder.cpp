#include <Arduino.h>
#include "MFEncoder.h"
#include "allocateMem.h"
#include "mobiflight.h"
#include "commandmessenger.h"
#include "Encoder.h"
#include "MFBoards.h"

namespace Encoder
{
MFEncoder *encoders[MAX_ENCODERS];
uint8_t encodersRegistered = 0;

void handlerOnEncoder(uint8_t eventId, uint8_t pin, const char *name)
{
    cmdMessenger.sendCmdStart(kEncoderChange);
    cmdMessenger.sendCmdArg(name);
    cmdMessenger.sendCmdArg(eventId);
    cmdMessenger.sendCmdEnd();
};

void Add(uint8_t pin1 = 1, uint8_t pin2 = 2, uint8_t encoder_type = 0, char const *name = "Encoder")
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
    encoders[encodersRegistered]->attachHandler(handlerOnEncoder);
    encodersRegistered++;
#ifdef DEBUG2CMDMESSENGER
    cmdMessenger.sendCmd(kStatus, F("Added encoder"));
#endif
}

void Clear()
{
    encodersRegistered = 0;
#ifdef DEBUG2CMDMESSENGER
    cmdMessenger.sendCmd(kStatus, F("Cleared encoders"));
#endif
}

void read()
{
    for (int i = 0; i != encodersRegistered; i++)
    {
        encoders[i]->update();
    }
}
}       // end of namespace encoder
