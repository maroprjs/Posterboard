/*
 * PLteMasts.cpp
 *
 *  Created on: 20 Feb 2020
 *      Author: maro
 */

#include "PLteMasts.h"

PLteMasts::PLteMasts(CRGB* crgb, CRGB baseColor) {
	_crgb = crgb;
	_baseColor = baseColor;
	_ledStepCountMax = LED_FADING_TIME / LED_MAX_BRIGHTNESS;
	_ledStepCounter = 0;
	_lastLEDUpdate = 0;

}

void PLteMasts::setup(void){
	_lastLEDUpdate = millis();
	beaconOn();
}


void PLteMasts::loop(void){
	ledBeaconAnimation();
}

void PLteMasts::beaconOn(void){
	*_crgb = _baseColor;

}

void PLteMasts::beaconOff(void){
	*_crgb = CRGB::Black;
}


void PLteMasts::changeColor(CRGB newColor){
	_baseColor = newColor;
	*_crgb = _baseColor;
}

void PLteMasts::ledBeaconAnimation(void){

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

PLteMasts::~PLteMasts() {
	// TODO Auto-generated destructor stub
}

