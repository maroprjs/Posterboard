/*
 * ControlCenter.cpp
 *
 *  Created on: 21 Feb 2020
 *      Author: maro
 *
 *      from:
 *      https://github.com/mathertel/RotaryEncoder/blob/master/examples/LimitedRotator/LimitedRotator.ino
 *
 *
 */

#include "ControlCenter.h"
#include "Debug.h"

ControlCenter::ControlCenter(RotaryEncoder* encoder, driverPin_t pushKeyPin,driverPin_t voltMeterPin ,int* encPosition, Substation* ss1, Substation* ss2, Substation* ss3 ) {
	_encoder = encoder;
	_ss1 = ss1;
	_ss2 = ss2;
	_ss3 = ss3;
	_pushKeyPin = pushKeyPin;
	_voltMeterPin = voltMeterPin;
	_lastEncPos = -1;
	_newEncPos = encPosition;
	*_newEncPos = -1;
	_lastPinUpdate = 0;
	_onOffTime = 0;
	_onOffTimeSet = NULL;

}



void ControlCenter::setup(void){

	_encoder->setPosition(0 / ROTARYSTEPS); // start with the value of 0.
	pinMode(_voltMeterPin, OUTPUT);
	digitalWrite(_voltMeterPin, LOW);
	_onOffTimeSet = new onOffTimeSet_t[ROTARYMAX];

	_onOffTimeSet[0].on = 0; _onOffTimeSet[0].off = 0;
	_onOffTimeSet[1].on = 45; _onOffTimeSet[1].off = 1;
	_onOffTimeSet[2].on = 40; _onOffTimeSet[2].off = 1;
	_onOffTimeSet[3].on = 35; _onOffTimeSet[3].off = 1;
	_onOffTimeSet[4].on = 30; _onOffTimeSet[4].off = 1;
	_onOffTimeSet[5].on = 30; _onOffTimeSet[5].off = 3;
	_onOffTimeSet[6].on = 30; _onOffTimeSet[6].off = 5;
	_onOffTimeSet[7].on = 28; _onOffTimeSet[7].off = 7;
	_onOffTimeSet[8].on = 26; _onOffTimeSet[8].off = 8;
	_onOffTimeSet[9].on = 25; _onOffTimeSet[9].off = 10;
	_onOffTimeSet[10].on = 23; _onOffTimeSet[10].off = 12;
	_onOffTimeSet[11].on = 22; _onOffTimeSet[11].off = 14;
	_onOffTimeSet[12].on = 21; _onOffTimeSet[12].off = 15;
	_onOffTimeSet[13].on = 19; _onOffTimeSet[13].off = 16;
	_onOffTimeSet[14].on = 18; _onOffTimeSet[14].off = 18;
	_onOffTimeSet[15].on = 16; _onOffTimeSet[15].off = 19;
	_onOffTimeSet[16].on = 15; _onOffTimeSet[16].off = 21;
	_onOffTimeSet[17].on = 14; _onOffTimeSet[17].off = 23;
	_onOffTimeSet[18].on = 12; _onOffTimeSet[18].off = 25;
	_onOffTimeSet[19].on = 11; _onOffTimeSet[19].off = 24;
	_onOffTimeSet[20].on = 0; _onOffTimeSet[20].off = 0;

	_lastPinUpdate = millis();

}

void ControlCenter::loop(void){
	  _encoder->tick();

	  // get the current physical position and calc the logical position
	  *_newEncPos = _encoder->getPosition() * ROTARYSTEPS;

	  if (*_newEncPos < ROTARYMIN) {
	    _encoder->setPosition(ROTARYMIN / ROTARYSTEPS);
	    *_newEncPos = ROTARYMIN;

	  } else if (*_newEncPos > ROTARYMAX) {
	    _encoder->setPosition(ROTARYMAX / ROTARYSTEPS);
	    *_newEncPos = ROTARYMAX;
	  } // if


	  if (_lastEncPos != *_newEncPos) {
	    //DPRINTLN(*_newEncPos);
	    _lastEncPos = *_newEncPos;

	    if (_lastEncPos >= THR_SUBSTATION3){
	    	_ss1->activate();
	    	_ss2->activate();
	    	_ss3->activate();
	    }
	    if ((_lastEncPos >= THR_SUBSTATION2) && (_lastEncPos < THR_SUBSTATION3)){
	    	_ss1->activate();
	    	_ss2->activate();
	    	_ss3->deactivate();
	    }
	    if ((_lastEncPos >= THR_SUBSTATION1) && (_lastEncPos < THR_SUBSTATION2)){
	    	_ss1->activate();
	    	_ss2->deactivate();
	    	_ss3->deactivate();
	    }
	    if (_lastEncPos < THR_SUBSTATION1){
	    	_ss1->deactivate();
	    	_ss2->deactivate();
	    	_ss3->deactivate();
	    };
      };
	  animateVoltage();

}

//so that GUI can set position as well
void ControlCenter::setNewEncPos(int newEncPos){
	*_newEncPos = newEncPos;
}

//try without interrupt
//this is On Off time for the driving pin of the voltage meter
int ControlCenter::getOffTime(void){
	return _onOffTimeSet[_lastEncPos].off;
}

int ControlCenter::getOnTime(void){
	return _onOffTimeSet[_lastEncPos].on;
}

void ControlCenter::animateVoltage(void){

	if ((_lastEncPos > ROTARYMIN) && (_lastEncPos < ROTARYMAX)){
		int onoff = (digitalRead(_voltMeterPin) == HIGH) ? getOnTime() : getOffTime();
		if (millis() >= (_lastPinUpdate + _onOffTime)){
			_onOffTime = onoff;
			digitalWrite(_voltMeterPin, !digitalRead(_voltMeterPin));
			_lastPinUpdate = millis();
		};
	};

	if (_lastEncPos == ROTARYMIN) digitalWrite(_voltMeterPin, LOW);

	if (_lastEncPos == ROTARYMAX) digitalWrite(_voltMeterPin, HIGH);


};


ControlCenter::~ControlCenter() {
	// TODO Auto-generated destructor stub
}

