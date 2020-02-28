/*
 * ControlCenter.h
 *
 *  Created on: 21 Feb 2020
 *      Author: maro
 */

#ifndef CONTROLCENTER_H_
#define CONTROLCENTER_H_
#include "TypeDefs.h"
#include <RotaryEncoder.h>
#include "Substation.h"

#define ROTARYSTEPS 2
#define ROTARYMIN 0
#define ROTARYMAX 20

#define THR_SUBSTATION1 2
#define THR_SUBSTATION2 11
#define THR_SUBSTATION3 17

typedef struct onOffTimeSet_t { //this is On Off time for the driving pin of the voltage meter
   uint8_t on;
   uint8_t off;
} onOffTimeSet;

class ControlCenter {
public:
	ControlCenter(RotaryEncoder* encoder, driverPin_t pushKeyPin, driverPin_t voltMeterPin, int* encPosition, Substation* ss1, Substation* ss2, Substation* ss3 );
	void setup(void);
	void loop(void);
	void setNewEncPos(int newEncPos);
	virtual ~ControlCenter();
private:
	RotaryEncoder* _encoder;
	Substation* _ss1;
	Substation* _ss2;
	Substation* _ss3;
	driverPin_t _pushKeyPin;
	driverPin_t _voltMeterPin;
	int _lastEncPos;
	int* _newEncPos;
	elapsedMillis_t _lastPinUpdate;
	elapsedMillis_t _onOffTime; //this is On Off time for the driving pin of the voltage meter
	onOffTimeSet_t* _onOffTimeSet;

	int getOffTime(void);
	int getOnTime(void);
	void animateVoltage(void);

};

#endif /* CONTROLCENTER_H_ */
