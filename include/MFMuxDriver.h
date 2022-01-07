// MFMuxDriver.h
 
#ifndef MFMultiplex_h
#define MFMultiplex_h

#if ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#include <wiring.h>
#endif

extern "C"
{
    typedef void (*MuxDigInEvent)(byte, uint8_t, const char *);
};

/////////////////////////////////////////////////////////////////////
/// \class MFMuxDriver MFMuxDriver.h <MFMuxDriver.h>
/// \brief  Implements a (system-level) multiplex selector
///          
class MFMuxDriver
{
public:
    
    MFMuxDriver(void);
    void attach(uint8_t Sel0Pin, uint8_t Sel1Pin, uint8_t Sel2Pin, uint8_t Sel3Pin);
    void detach();

    //void setChannelOpt(uint8_t mode);
    void    setChannel(uint8_t value);
    uint8_t getChannel(void);
    uint8_t nextChannel(void);
    void    saveChannel(void);      // Not reentrant - one level only
    void    restoreChannel(void);

private:

    enum { MUX_INITED = 0, };

    uint8_t       _selPin[4];   // Selector pins; 0 is LSb
    uint8_t       _flags;
    uint8_t       _channel;
    uint8_t       _savedChannel;
};
#endif
