/*
 * ChargingStation.cpp
 *
 *  Created on: 20 Feb 2020
 *      Author: maro
 */

#include "ChargingStation.h"

ChargingStation::ChargingStation(driverPin_t statusInPin, driverPin_t statusOutPin, CRGB* crgb) {
	_crgb = crgb;
	_ledStepCountMin = LED_HUE_STEP_COUNT_MIN;
	_ledStepCounter = LED_HUE_STEP_COUNT_MAX;
	_lastLEDUpdate = 0;
	_charging = OFF;
	_statusInPin = statusInPin;
	_statusOutPin = statusOutPin;
	_chargingManuallyOn = false;//auto == on
	_statusHasChanged = true;

}

void ChargingStation::setup(void){
	pinMode(_statusInPin, INPUT); //pull down automatically
	pinMode(_statusOutPin, OUTPUT);
	digitalWrite(_statusOutPin, LOW); //normal operation
	_lastLEDUpdate = millis();
}


void ChargingStation::loop(void){

	   ledChargingAnimation();
	   if (!_chargingManuallyOn){ //normal operation if Manually Off
	      if (digitalRead(_statusInPin) == HIGH){
	    	if (_charging == OFF)  _statusHasChanged = true;
		   _charging = ON;
	      }else{
	    	  if (_charging == ON)  _statusHasChanged = true;
		     _charging = OFF;
	      }
	   };
}


void ChargingStation::setAutomaticModusOn(void){
	this->setChargingOff();
	_chargingManuallyOn = false;
}

void ChargingStation::setAutomaticModusOff(void){
	_chargingManuallyOn = true;
	_statusHasChanged = true;
}



//normal operation...if a_chargingManuallyOn car is in charing it will be animated
void ChargingStation::setChargingOff(void){
	digitalWrite(_statusOutPin, LOW);
	_charging = OFF; //animation
	_statusHasChanged = true;
}

// charging requested from GUI even if there is no car
void ChargingStation::setChargingOn(void){
	digitalWrite(_statusOutPin, HIGH);
	_charging = ON;//animation
	_statusHasChanged = true;

}

bool ChargingStation::isChargingOn(void){
	return ((_charging == ON) ? true : false);
}

bool ChargingStation::isChargingOff(void){
	return ((_charging == OFF) ? true : false);
}

void ChargingStation::ledChargingAnimation(void){

	if (millis() >= (_lastLEDUpdate + LED_UPDATE_INTERVAL)){

		if (_charging == ON){
			*_crgb = CHSV(_ledStepCounter*LED_HUE_STEP,LED_SATURATION,LED_ANIMATION_BRIGHTNESS);
			_ledStepCounter--;
    	    if ((_ledStepCounter <= _ledStepCountMin)){
    	    	_ledStepCounter = LED_HUE_STEP_COUNT_MAX;
    	     };
		}else{
			//currently no anuimation during idle (just orange light)
			*_crgb = CHSV(LED_IDLE_HUE,LED_SATURATION,LED_IDLE_BRIGHTNESS-_ledStepCounter * 2);
			_ledStepCounter--;
    	    if ((_ledStepCounter <= _ledStepCountMin)){
    	    	_ledStepCounter = LED_HUE_STEP_COUNT_MAX;//never mind the HUE
    	     };
		}
		_lastLEDUpdate = millis();
	};

}

bool ChargingStation::statusHasChanged(void){
    bool retStat = _statusHasChanged;
    _statusHasChanged = false;
    return retStat;
}

ChargingStation::~ChargingStation() {
	// TODO Auto-generated destructor stub
}

