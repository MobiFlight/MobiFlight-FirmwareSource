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
  _handler = callback; 
  pinMode(_pin, INPUT_PULLUP);     // set pin to input. Could use OUTPUT for analog, but shows the intention :-)
  analogRead(_pin); // turn on pullup resistors
}

void MFAnalog::update()
{
    int newValue = ADC_Average_Buffer/ADC_MAX_AVERAGE;
    if (abs(newValue - _lastValue) >= _sensitivity) {
      _lastValue = newValue;
       if (_handler!= NULL) {
        (*_handler)(_lastValue, _pin, _name);
      }      
    }
}

void MFAnalog::tick(void){                                  // read ADC and calculate floating average, call it every ~10ms
  uint16_t ADC_temp = 0;                                    // floating average must be divided by 8 in return function
  ADC_Average_Buffer -=ADC_Buffer[(ADC_Average_Pointer)];   // subtract oldest value to save the newest value
  ADC_temp = analogRead(_pin);                              // store value read in
  ADC_Buffer[ADC_Average_Pointer] = ADC_temp;               // store read in, must be subtracted in next loop
  ADC_Average_Buffer += ADC_temp;                           // add read in for floating average
  ADC_Average_Pointer++;                                    // prepare for next loop
  ADC_Average_Pointer &= (ADC_MAX_AVERAGE-1);               // limit max. values for floating average
}

