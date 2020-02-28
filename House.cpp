/*
 * House.cpp
 *
 *  Created on: 18 Feb 2020
 *      Author: maro
 */

#include "House.h"

House::House(CRGB* crgb, LED* led) {
	_crgb = crgb;
	_led = led;

}

House::House(CRGB* crgb) {
	_crgb = crgb;
	_led = NULL; //not used
}

void House::setup(void){
	if (_crgb != NULL) *_crgb = CRGB::Black;
	if (_led != NULL) _led->setup();
}

void House::loop(void){
	//not in use
}

void House::lightOn(void){
	*_crgb = CRGB::Green;
	//_crgb->fadeLightBy(xxx);<- if needed to reduce brightness
	_led->turnOn();
}

void House::lightOff(void){
	*_crgb = CRGB::Black;
	_led->turnOff();
}

House::~House() {
	// TODO Auto-generated destructor stub
}

