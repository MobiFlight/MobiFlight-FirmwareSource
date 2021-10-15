/*
 * MFEncoder.h
 *
 * Created: 2021/09/07
 * Author: Ralf Kull
 * 
 * This is based on Peter Danneggers work
 * see 	http://www.mikrocontroller.net/articles/Drehgeber
 * 		https://www.mikrocontroller.net/topic/209118#new
 * 		https://www.mikrocontroller.net/topic/drehgeber-auslesen?page=2#1568686
 */ 

#ifndef MFEncoder_h
#define MFEncoder_h

#include <Arduino.h>
#include <MFBoards.h>

#define USE_ACCELERATION_MILLIS       // choose one! from these both for acceleration method
//#define USE_ACCELERATION_TICKS

// ----------------------------------------------------------------------------
// Acceleration configuration (for 1000Hz calls to ::readInput())
//
#define ENC_ACCEL_TOP       250
#define ENC_ACCEL_INC        10
#define ENC_ACCEL_DEC         1 
#define MF_ENC_FAST_LIMIT    75
#define FAST_LIMIT_MILLIS    25             // less than this value it will be switched to fast mode (delta *= MF_ENC_FAST_LIMIT + 5)

enum
{
  encLeft,
  encLeftFast,
  encRight,
  encRightFast
};

extern "C"
{
  typedef void (*encoderEvent) (uint8_t, uint8_t, const char *);
};


class MFEncoder
{
private:
    uint8_t         _pin1, _pin2;                   // Arduino pins used for the encoder.
    uint8_t         _encoder_Steps = 1;             // Encoder steps per detent, can be 1,2 or 4
    uint8_t         _encoder_Last = 0;
    int8_t          _encoder_Delta = 0;
    encoderEvent    _handlerList[4];
    const char *    _name;
    bool            _initialized = false;
#ifdef USE_ACCELERATION_TICKS
    uint16_t         _acceleration = 0;
#endif
#ifdef USE_ACCELERATION_MILLIS
    unsigned long _positionTime;                    // time last position change was detected
    unsigned long _positionTimePrev;                // time previous position change was detected
#endif

public:
    // Constructor
    MFEncoder(uint8_t pin1 = 1, uint8_t pin2 = 2, uint8_t encoder_type = 0, const char * name = "Encoder");
    // void attach(uint8_t pin1, uint8_t pin2, uint8_t encoderType, const char * name = "Encoder");
    void update();
    void attachHandler(uint8_t eventId, encoderEvent newHandler);
    // call this function every some milliseconds or by using an interrupt for handling state changes of the rotary encoder
    void readInput(void);
    // retrieve the current position
    int16_t getPosition();
};

#endif
