//
// Encoder.cpp
//
// (C) MobiFlight Project 2022
//

#include "mobiflight.h"
#include "MFEncoder.h"
#include "Encoder.h"

namespace Encoder
{
    MFEncoder *encoders;
    uint8_t    encodersRegistered = 0;
    uint8_t    maxEncoders        = 0;

    void handlerOnEncoder(uint8_t eventId, uint8_t deviceID)
    {
        cmdMessenger.sendCmdStart(kEncoderChange);
        cmdMessenger.sendCmdArg(deviceID);
        cmdMessenger.sendCmdArg(eventId);
        cmdMessenger.sendCmdEnd();
    };

    bool setupArray(uint16_t count)
    {
        if (!FitInMemory(sizeof(MFEncoder) * count))
            return false;
        encoders    = new (allocateMemory(sizeof(MFEncoder) * count)) MFEncoder;
        maxEncoders = count;
        return true;
    }

    void Add(uint8_t pin1, uint8_t pin2, uint8_t encoder_type)
    {
        if (encodersRegistered == maxEncoders)
            return;
        encoders[encodersRegistered] = MFEncoder();
        encoders[encodersRegistered].attach(pin1, pin2, encoder_type, encodersRegistered);
        MFEncoder::attachHandler(handlerOnEncoder);
        encodersRegistered++;
#ifdef DEBUG2CMDMESSENGER
        cmdMessenger.sendCmd(kDebug, F("Added encoder"));
#endif
    }

    void Clear()
    {
        encodersRegistered = 0;
#ifdef DEBUG2CMDMESSENGER
        cmdMessenger.sendCmd(kDebug, F("Cleared encoders"));
#endif
    }

    void read()
    {
        for (uint8_t i = 0; i < encodersRegistered; i++) {
            encoders[i].update();
        }
    }
} // namespace encoder

// Encoder.cpp
