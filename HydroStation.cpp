/*
 * HydroStation.cpp
 *
 *  Created on: 17 Feb 2020
 *      Author: maro
 */

#include "HydroStation.h"
#include "TypeDefs.h"

HydroStation::HydroStation(driverPin_t activationPin) {
	_activationPin = activationPin;
	_on_off_state = OFF;
	_statusHasChanged = true;

}

///////////public/////////////////
void HydroStation::setup(void){
   pinMode(_activationPin, OUTPUT);
   turnOn();
}

void HydroStation::turnOff(void){
   digitalWrite(_activationPin, LOW);
   _on_off_state = OFF;
   _statusHasChanged = true;
}

void HydroStation::turnOn(void){
   digitalWrite(_activationPin, HIGH);
   _on_off_state = ON;
   _statusHasChanged = true;
}

bool HydroStation::isOff(void){
	return ((_on_off_state == OFF) ? true : false);
}

bool HydroStation::isOn(void){
	return ((_on_off_state == ON) ? true : false);
}

bool HydroStation::statusHasChanged(void){
    bool retStat = _statusHasChanged;
    _statusHasChanged = false;
    return retStat;
}


/////////////private////////////////

HydroStation::~HydroStation() {
	// TODO Auto-generated destructor stub
}

