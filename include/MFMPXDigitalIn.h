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
    enum { MPX_MODE_FAST = 0, MPX_MODE_LAZY = 1, };

    MFMPXDigitalIn(void);
    MFMPXDigitalIn(MFMultiplex *MPX, const char *name);
    static void setMPX(MFMultiplex *MPX);
    static void attachHandler(MPXDigitalInEvent newHandler);

    void attach(uint8_t dataPin, bool halfSize, char const *name);
    void detach();
    void clear();
    void retrigger();
    void update();
    void setLazyMode(bool mode);
    uint16_t getValues(void) { return _lastState; }

private:

    enum { MPX_INITED = 0, MPX_HALFSIZE = 1, MPX_LAZY = 2, };

    static MFMultiplex *_MPX;
    static MPXDigitalInEvent _inputHandler;
    
    const char *  _name;
    uint8_t       _dataPin;     // Data pin - MPX common, input to AVR
    uint8_t       _flags;
    uint16_t      _lastState;

    void poll(bool detect, bool lazy);
    void detectChanges(uint16_t lastState, uint16_t currentState);
    void trigger(uint8_t channel, bool state);

};
#endif
