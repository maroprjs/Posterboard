/*
 * HydroStation.h
 *
 *  Created on: 17 Feb 2020
 *      Author: maro
 */

#ifndef HYDROSTATION_H_
#define HYDROSTATION_H_

#include "TypeDefs.h"

#define DFLT_ACTIVATION_PIN 30 //=#define CONTROLLINO_R8 30

class HydroStation {
public:
	HydroStation(driverPin_t activationPin = DFLT_ACTIVATION_PIN);
	virtual ~HydroStation();
	void setup();
	void turnOn(void);
	void turnOff(void);
	bool isOn(void);
	bool isOff(void);
	bool statusHasChanged(void);
private:
	driverPin_t _activationPin;
	on_off_state_t _on_off_state;
	bool _statusHasChanged;
};

#endif /* HYDROSTATION_H_ */
