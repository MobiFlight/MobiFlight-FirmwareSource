/*
 * MFEncoder.cpp
 *
 * Created: 2021/09/07
 * Author: Ralf Kull
 * 
 * This is based on Peter Danneggers work
 * see 	http://www.mikrocontroller.net/articles/Drehgeber
 * 		https://www.mikrocontroller.net/topic/209118#new
 * 		https://www.mikrocontroller.net/topic/drehgeber-auslesen?page=2#1568686
 */

#include "MFEncoder.h"

const uint8_t encoderTypes[] {4,4,2,2,1};
// EncoderType 0: 1 step/cycle - detent on [11]
// EncoderType 1: 1 step/cycle - detent on [00]
// EncoderType 2: 2 step/cycle - detent on [00] / [11]
// EncoderType 3: 2 step/cycle - detent on [10] / [01]
// EncoderType 4: 4 step/cycle - detent on [00] / [10] / [11] / [01]

MFEncoder::MFEncoder( uint8_t pin1, uint8_t pin2, uint8_t encoder_type, const char * name)
{
  	_pin1 = pin1;
  	_pin2 = pin2;
  	_encoder_Steps = encoderTypes[encoder_type];
	_name = name;
	pinMode(_pin1, INPUT_PULLUP);							// Encoder Port as Input and Activate Pull up's
	pinMode(_pin2, INPUT_PULLUP);							// Encoder Port as Input and Activate Pull up's
	if (digitalRead(_pin1)) _encoder_Last = 3;				// convert gray to binary
	if (digitalRead(_pin2)) _encoder_Last ^= 1;				// convert gray to binary
	if (encoder_type == 3) _encoder_Delta = 0;				// encoder starts at [10] / [01]
	else _encoder_Delta = (_encoder_Last&1?0:1);			// encoder starts at [00] / [11]
	_initialized = true;
}

void MFEncoder::readInput( void ) {							// must be called every ~1ms, otherwise acceleration has to be adjusted
	int8_t encoder_new=0, encoder_diff=0;
	if (_acceleration >= ENC_ACCEL_DEC) _acceleration -= ENC_ACCEL_DEC;
	else _acceleration = 0;
	if (digitalRead(_pin1)) encoder_new = 3;				// convert gray to binary
	if (digitalRead(_pin2)) encoder_new ^= 1;				// convert gray to binary
	encoder_diff = _encoder_Last - encoder_new;				// difference last - new
	if( encoder_diff & 1 ) {								// bit 0 = value (1/0)
		_acceleration += ENC_ACCEL_INC;						// consider acceleration
		_encoder_Last = encoder_new;						// store new as next last
		_encoder_Delta += (encoder_diff & 2) - 1;			// bit 1 = direction (+/-)
	}
}

int16_t MFEncoder::getPosition( void ) {
	int16_t delta=0;
	noInterrupts();
	delta = _encoder_Delta;
	_encoder_Delta = delta & (_encoder_Steps - 1);			// set Delta Position to "Zero" (must not be really zero, getPosition() could be within one detent of encoder -> not set "0")
	interrupts();
	delta = delta >> (_encoder_Steps/2);
	if (delta < 0) delta -= _acceleration;
	else if (delta > 0) delta += _acceleration;
	return delta;
}

void MFEncoder::update()
{
  if (!_initialized) return;
  readInput();
  int16_t position = getPosition();
  if (position == 0) { return;}    // nothing happened 
  if (abs(position) < MF_ENC_FAST_LIMIT) {
    // slow turn detected
    if (position<0 && _handlerList[encLeft]!= NULL) {
        (*_handlerList[encLeft])(encLeft, _pin1, _name);
    } else if(_handlerList[encRight]!= NULL) {
        (*_handlerList[encRight])(encRight, _pin2, _name);
    }
  } else {
    // fast turn detected
    if (position<0 && _handlerList[encLeftFast]!= NULL) {
        (*_handlerList[encLeftFast])(encLeftFast,  _pin1, _name);
    } else if(_handlerList[encRightFast]!= NULL) {
        (*_handlerList[encRightFast])(encRightFast, _pin2, _name);
    }
  }
}

void MFEncoder::attachHandler(uint8_t eventId, encoderEvent newHandler)
{
  _handlerList[eventId] = newHandler;
}
