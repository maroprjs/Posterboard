/*
 * Solarfarm.cpp
 *
 *  Created on: 18 Feb 2020
 *      Author: maro
 */

#include "Solarfarm.h"
#include "Debug.h"

Solarfarm::Solarfarm(Solarmodul* solarmodul1, Solarmodul* solarmodul2, Solarmodul* solarmodul3, Solarmodul* solarmodul4) {
	_solarArray[0] = solarmodul1;
	_solarArray[1] = solarmodul2;
	_solarArray[2] = solarmodul3;
	_solarArray[3] = solarmodul4;
    _auto = ON;
    _lightOn = false;
	_statusHasChanged = true;

}

Solarfarm::Solarfarm(Solarmodul* solarmodul1) {
	_solarArray[0] = solarmodul1;
	_solarArray[1] = NULL;
	_solarArray[2] = NULL;
	_solarArray[3] = NULL;
    _auto = ON;
    _lightOn = false;
	_statusHasChanged = true;

}

void Solarfarm::setup(void){
	PRINTLN("Setting up solar array....takes some time...");
	for (int i=0; i < NUM_OF_SOLAR_MODULES; i++){
		_solarArray[i]->setup();
	}
	PRINTLN("Solarfarm Setup Complete! ");

}

void Solarfarm::loop(void){
	if (_auto == ON){
	   for (int i=0; i < NUM_OF_SOLAR_MODULES; i++){
		  _solarArray[i]->loop();
	   };
	};
}

void Solarfarm::setAutomaticModusOn(void){
	_statusHasChanged = true;
	_auto = ON;
}

void Solarfarm::setAutomaticModusOff(void){
	_statusHasChanged = true;
	_auto = OFF;
}


//returns 0 if no light recognized and numeration of active solar module (normally only one should be active)
int Solarfarm::isLightOn(void){
	int activeSolarmodul = 0;
	int count = 0;
	if (_auto == ON){
	   for (int i=0; i < NUM_OF_SOLAR_MODULES; i++){
		   if (_solarArray[i]->isOn()){
			   activeSolarmodul = i + 1;
			   count++;
		   };
	   };
	   if (count >= 1) _lightOn = true;
	   if (count == 0) _lightOn = false;
	   if (count > 1){ //there must be a problem with light conditions if more then one gets active
		   //better to calibrate:
		   DPRINTLN("Several active solars recognized...NO calibration");
		   /*TODO: check if it makes sense to e.g. activate this if all solars recognize light:(cound = 4 (NUMBER_OF_SOLAR_PANELS))
		   for (int i=0; i < NUM_OF_SOLAR_MODULES; i++){
			   _solarArray[i]->stop();
			   _solarArray[i]->calibrate();
			   _solarArray[i]->start();
		   };*/
		//   activeSolarmodul = 0;

	   };
	}else{
		activeSolarmodul = (int)_lightOn;//TODO: make this clean
	}
	return activeSolarmodul;
}

void Solarfarm::turnOn(void){
	if (_auto == ON){
	   for (int i=0; i < NUM_OF_SOLAR_MODULES; i++){
         _solarArray[i]->start();
	   };
	}else{
		_lightOn = true;
	}
	_statusHasChanged = true;
}

void Solarfarm::turnOff(void){
	if (_auto == ON){
	   for (int i=0; i < NUM_OF_SOLAR_MODULES; i++){
          _solarArray[i]->stop();
	   };
	}else{
		_lightOn = false;
	}
	_statusHasChanged = true;
}



bool Solarfarm::statusHasChanged(void){
    bool retStat = _statusHasChanged;
	for (int i=0; i < NUM_OF_SOLAR_MODULES; i++){
         retStat = (retStat || _solarArray[i]->statusHasChanged());
	};
    _statusHasChanged = false;
    return retStat;
}


Solarfarm::~Solarfarm() {
	// TODO Auto-generated destructor stub
}

