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
	//_solarCheckStopped = true;
	//_manualOnToggle = 0;
	//_manualOffToggle = 0;
	_statusHasChanged = true;

}

Solarfarm::Solarfarm(Solarmodul* solarmodul1) {
	_solarArray[0] = solarmodul1;
	_solarArray[1] = NULL;
	_solarArray[2] = NULL;
	_solarArray[3] = NULL;
	//_solarCheckStopped = true;
	//_manualOnToggle = 0;
	//_manualOffToggle = 0;
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
	//if (_solarCheckStopped == false){
	for (int i=0; i < NUM_OF_SOLAR_MODULES; i++){
		_solarArray[i]->loop();
	};
	//};
}

//returns 0 if no light recognized and numeration of active solar module (normally only one should be active)
int Solarfarm::isLightOn(void){
	int activeSolarmodul = 0;
	int count = 0;
	//if (_solarCheckStopped == false){
	   for (int i=0; i < NUM_OF_SOLAR_MODULES; i++){
		   if (_solarArray[i]->isOn()){
			   activeSolarmodul = i + 1;
			   count++;
		   };
	   };
	   if (count > 1){ //there must be a problem with light conditions if more then one gets active
		   //better to calibrate:
		   DPRINTLN("Several active solars recognized...NO calibration");
		   /*TODO: check if it makes sense to e.g. activate this if all solars recognize light:(cound = 4 (NUMBER_OF_SOLAR_PANELS))
		   for (int i=0; i < NUM_OF_SOLAR_MODULES; i++){
			   _solarArray[i]->stop();
			   _solarArray[i]->calibrate();
			   _solarArray[i]->start();
		   };*/
		   activeSolarmodul = 0;

	   }
	//}
	return activeSolarmodul;
}

void Solarfarm::turnOn(void){
	//_solarCheckStopped = true;
	for (int i=0; i < NUM_OF_SOLAR_MODULES; i++){
         _solarArray[i]->start();
	};
	_statusHasChanged = true;
}

void Solarfarm::turnOff(void){
	//_solarCheckStopped = false;
	for (int i=0; i < NUM_OF_SOLAR_MODULES; i++){
         _solarArray[i]->stop();
	};
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

