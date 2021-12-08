// MFMPXDigitalIn.h
 
#ifndef MFMPXDigitalIn_h
#define MFMPXDigitalIn_h

#if ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#include <wiring.h>
#endif
#include "MFMultiplex.h"

extern "C"
{
    typedef void (*MPXDigitalInEvent)(byte, uint8_t, const char *);
};

enum
{
    MPXDigitalInOnPress,
    MPXDigitalInOnRelease,
};

/////////////////////////////////////////////////////////////////////
/// \class MFMPXDigitalIn MFMPXDigitalIn.h <MFMPXDigitalIn.h>
class MFMPXDigitalIn
{
public:
    MFMPXDigitalIn(MFMultiplex *MPX = NULL, const char *name = "MPXDigIn");
    void attach(uint8_t Sel0Pin, uint8_t Sel1Pin, uint8_t Sel2Pin, uint8_t Sel3Pin, 
                uint8_t dataPin, bool halfSize, char const *name);
    static void attachHandler(MPXDigitalInEvent newHandler);
    void clear();
    void detach();
    void retrigger();
    void update();

private:

    enum { MPX_INITED = 0x80, MPX_HALFSIZE = 0x40, };

    static MFMultiplex *_MPX;
    static MPXDigitalInEvent _inputHandler;
    
    const char *  _name;
    uint8_t       _dataPin;     // Data pin - MPX common, input to AVR
    uint8_t       _flags;
    uint16_t      _lastState;

    void poll(bool detect);
    void detectChanges(uint16_t lastState, uint16_t currentState);
    void trigger(uint8_t channel, bool state);
    void clearLastState();
    

};
#endif
