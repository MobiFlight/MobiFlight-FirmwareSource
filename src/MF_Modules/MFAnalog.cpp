// MFSegments.cpp
//
// Copyright (C) 2021

#include "MFAnalog.h"

MFAnalog::MFAnalog(uint8_t pin, analogEvent callback, const char * name, uint8_t sensitivity)
{   
  _sensitivity = sensitivity;  
  _pin  = pin;
  _name = name;
  _lastValue = 0;
  _last = millis();
  _handler = callback; 
  pinMode(_pin, INPUT_PULLUP);     // set pin to input. Could use OUTPUT for analog, but shows the intention :-)
  analogRead(_pin); // turn on pullup resistors
}

void MFAnalog::update()
{
/* *****************************************************************************
*   There is a pull request to transfer the millis() to the mobiflight.cpp
*   this will save some memory and would be also valid for tick();
********************************************************************************/
    uint32_t now = millis();
    if (now-_lastTick > 10) {
      tick();
      _lastTick = millis();
    }
    if (now-_last <= 50) return; // Analog is too spammy on the protocol to MF otherwise.
/* *************************************************************************** */

    int newValue = ADC_Average_Buffer/ADC_MAX_AVERAGE;
    _last = now;
    if (abs(newValue - _lastValue) >= _sensitivity) {
      _lastValue = newValue;
       if (_handler!= NULL) {
        (*_handler)(_lastValue, _pin, _name);
      }      
    }
}

void MFAnalog::tick(){                                      // read ADC and calculate floating average, call it every ~10ms
  ADC_Average_Buffer -= ADC_Buffer[(ADC_Average_Pointer)];  // subtract oldest value to save the newest value
  ADC_Buffer[ADC_Average_Pointer] = analogRead(_pin);       // store read in, must be subtracted in next loop
  ADC_Average_Buffer += ADC_Buffer[ADC_Average_Pointer];    // add read in for floating average
  ADC_Average_Pointer++;                                    // prepare for next loop
  ADC_Average_Pointer &= (ADC_MAX_AVERAGE-1);               // limit max. values for floating average
}

