/*
 * Substation.cpp
 *
 *  Created on: 20 Feb 2020
 *      Author: maro
 */

#include "Substation.h"

Substation::Substation(CRGB* crgb) {
	_crgb = crgb;
	_statusHasChanged = true;
	_isActive = false;
}

void Substation::setup(void){
	if (_crgb != NULL) *_crgb = CRGB::Black;
	_statusHasChanged = true;

}

void Substation::loop(void){
	//not in use
}

void Substation::activate(void){
	*_crgb = CRGB::Blue;
	_isActive = true;
	//_crgb->fadeLightBy(xxx);<- if needed to reduce brightness
	_statusHasChanged = true;

}

void Substation::deactivate(void){
	*_crgb = CRGB::Black;
	_isActive = false;

}

bool Substation::isActive(){
	bool retVal = false;
	if (_isActive == true) retVal = true;
	return retVal;
}

bool Substation::statusHasChanged(void){
    bool retStat = _statusHasChanged;
    _statusHasChanged = false;
    return retStat;
}

Substation::~Substation() {
	// TODO Auto-generated destructor stub
}

