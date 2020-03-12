/*
 * Solarfarm.h
 *
 *  Created on: 18 Feb 2020
 *      Author: maro
 */

#ifndef SOLARFARM_H_
#define SOLARFARM_H_

#include "Solarmodul.h"

#define NUM_OF_SOLAR_MODULES (4)

class Solarfarm {
public:
	Solarfarm(Solarmodul* solarmodul1, Solarmodul* solarmodul2, Solarmodul* solarmodul3, Solarmodul* solarmodul4);
	Solarfarm(Solarmodul* solarmodul1);//for testing
	void setup();
	void loop();
	int isLightOn(void);
	void turnOn(void);
	void turnOff(void);
	bool statusHasChanged(void);
	void setAutomaticModusOn(void);
	void setAutomaticModusOff(void);
	virtual ~Solarfarm();
private:
	Solarmodul* _solarArray[NUM_OF_SOLAR_MODULES];
	on_off_state_t _auto;
	bool _lightOn;
	bool _statusHasChanged;


};

#endif /* SOLARFARM_H_ */
