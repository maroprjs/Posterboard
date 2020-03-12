/*
 * Windmill.cpp
 *
 *  Created on: 17 Feb 2020
 *      Author: maro
 */

#include "Windmill.h"

Windmill::Windmill(driverPin_t motorPin, CRGB* crgb, windspeed_t* wind) {
	_motorPin = motorPin;
	_crgb = crgb;
	_wind = wind;
	_on_off_state = OFF;
	_measurementInterval =  DEFAULT_MEASUREMENT_INTERVAL;
	_elapsedTime = 0;
	_ledStepCountMax = LED_FADING_TIME / LED_MAX_BRIGHTNESS;
	_ledStepCounter = 0;
	_lastLEDUpdate = 0;
	_statusHasChanged = true;
	_auto = ON;
}


void Windmill::setup(void){
   pinMode(_motorPin, OUTPUT);
   turnOff();
   _elapsedTime = millis();
}

void Windmill::loop(void){
	if (_auto == ON){
		if (millis() >= (_elapsedTime + _measurementInterval)){
			if ((*_wind >= WIND_THRESHOLD_ON) && (_on_off_state == OFF)){//needs to be called only when off
				turnOn();
			};
			if ((*_wind <= WIND_THRESHOLD_OFF) && (_on_off_state == ON)){//needs to be called only when on
				turnOff();
			};
			_elapsedTime = millis();
		};
	} else{
		if (millis() >= (_elapsedTime + _measurementInterval)){
		    _elapsedTime = millis();
		};
	};
	if (_on_off_state == ON) ledAnimation();

}

void Windmill::setAutomaticModusOn(void){
	_statusHasChanged = true;
	_auto = ON;
}

void Windmill::setAutomaticModusOff(void){
	_statusHasChanged = true;
	_auto = OFF;
}

void Windmill::ledAnimation(void){

		if (millis() >= (_lastLEDUpdate + LED_UPDATE_INTERVAL)){
    	    if ((_ledStepCounter <= _ledStepCountMax)){
    	       _ledStepCounter++;
    	       _crgb->fadeLightBy(LED_FADE_FACTOR);
    	     }else{
    	        _crgb->maximizeBrightness(LED_MAX_BRIGHTNESS);
    	        _ledStepCounter = 0;
    	      }
			_lastLEDUpdate = millis();
		};

}

void Windmill::toggleOnOff(void){
		   if (_on_off_state == ON) {
			   turnOff();
		   }else{
			   turnOn();
		   };
}

void Windmill::turnOff(void){ //turns off the motor
   digitalWrite(_motorPin, LOW);
   *_crgb = CRGB::Black;
   _on_off_state = OFF;
   _statusHasChanged = true;

}

void Windmill::turnOn(void){//turns the motor on
   digitalWrite(_motorPin, HIGH);
   *_crgb = CRGB::Red;
   _on_off_state = ON;
   _statusHasChanged = true;
}


bool Windmill::isOff(void){
	return ((_on_off_state == OFF) ? true : false);
}

bool Windmill::isOn(void){
	return ((_on_off_state == ON) ? true : false);
}




bool Windmill::statusHasChanged(void){
    bool retStat = _statusHasChanged;
    _statusHasChanged = false;
    return retStat;
}

Windmill::~Windmill() {
	// TODO Auto-generated destructor stub
}

