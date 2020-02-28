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
	_windCheckStopped = true;
	_ledStepCountMax = LED_FADING_TIME / LED_MAX_BRIGHTNESS;
	_ledStepCounter = 0;
	_lastLEDUpdate = 0;
	//_manualOnToggle = 0;
	//_manualOffToggle = 0;
	_statusHasChanged = true;


}


void Windmill::setup(void){
   pinMode(_motorPin, OUTPUT);
   turnOff();
   _elapsedTime = millis();
   startWindCheck();
}

void Windmill::loop(void){
	if ( ! _windCheckStopped){
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
	   //_manualOffToggle++;
	   //if (_manualOffToggle % 2 ){
		   //stopWindCheck();
		   if (_on_off_state == ON) {
			   turnOff();
		   }else{
			   turnOn();
		   };
	   //};
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

void Windmill::turnOnManually(void){//turns the motor on
   digitalWrite(_motorPin, HIGH);
   *_crgb = CRGB::Red;
   _on_off_state = ON;
   _elapsedTime = _elapsedTime + 20000;
   _statusHasChanged = true;
}


bool Windmill::isOff(void){
	return ((_on_off_state == OFF) ? true : false);
}

bool Windmill::isOn(void){
	return ((_on_off_state == ON) ? true : false);
}

//start-/stop- mechanism if windmill to be switched on via API ("manually").
void Windmill::startWindCheck(){ //starts to check wind measurement and turns motor on at threshold
	turnOff();
	_elapsedTime = millis();
	_windCheckStopped = false;

}

void Windmill::stopWindCheck(){//wind measurement stop
	_windCheckStopped = true;

}

bool Windmill::statusHasChanged(void){
    bool retStat = _statusHasChanged;
    _statusHasChanged = false;
    return retStat;
}

Windmill::~Windmill() {
	// TODO Auto-generated destructor stub
}

