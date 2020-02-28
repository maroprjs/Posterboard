/*
 * LED.cpp
 *
 *  Created on: 18 Feb 2020
 *      Author: maro
 */

#include "LED.h"

LED::LED(driverPin_t driverPin) {
	_driverPin = driverPin;

}

void LED::setup(void){
	pinMode(_driverPin, OUTPUT);
}

void LED::loop(void){
	//TODO: Fill this if "LED animation" is needed ever
}

void LED::turnOn(void){
   analogWrite(_driverPin, MAX_BRIGHTNESS);
}

void LED::turnOff(void){
   digitalWrite(_driverPin, LOW);
}

LED::~LED() {
	// TODO Auto-generated destructor stub
}

