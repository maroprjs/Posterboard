/*
 * Multimeter.cpp
 *
 *  Created on: 22 Feb 2020
 *      Author: maro
 */

#include "Multimeter.h"

Multimeter::Multimeter(sensingPin_t currentPin,sensingPin_t voltPin, current_t* current,  voltage_t* volt) {
	_voltPin = voltPin;
	_currentPin = currentPin;
	_voltADSample = 0;
	_currentADSample = 0;
	_voltPtr = volt;
	_currentPtr = current;
	_sampleWindow = DEFAULT_SAMPLE_WINDOW;
	_voltSampleCount = 0;
	_currSampleCount = 0;
	_voltSampleAverage = 0;
	_currSampleAverage = 0;
	_startMillis = 0;
	_voltSampleTotalSum = 0;
	_currSampleTotalSum = 0;



}

void Multimeter::setup(){
  _startMillis = millis();
}

void Multimeter::loop(){

    if(millis() - _startMillis < _sampleWindow)
    {

        _currentADSample = analogRead(_currentPin);
        _voltADSample = analogRead(_voltPin);
        _currSampleTotalSum += _currentADSample;
        _currSampleCount++;
        _voltSampleTotalSum += _voltADSample;
        _voltSampleCount++;
        //_startMillis = millis();
     }
     else{
    	   //Serial.println(_currSampleTotalSum);
          _currSampleAverage = _currSampleTotalSum / _currSampleCount;
          _voltSampleAverage = _voltSampleTotalSum / _voltSampleCount;
          _currSampleTotalSum = 0;
          _currSampleCount = 0;
          _voltSampleTotalSum = 0;
          _voltSampleCount = 0;
          *_voltPtr = _voltSampleAverage; // - VOLT_RESOLUTION * _voltSampleAverage;
          *_currentPtr = _currSampleAverage; //CURR_RESOLUTION * _currSampleAverage;
	      _startMillis= millis();  // Start of sample window
     }

}

Multimeter::~Multimeter() {
	// TODO Auto-generated destructor stub
}

