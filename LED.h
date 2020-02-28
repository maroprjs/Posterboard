/*
 * LED.h
 *
 *  Created on: 18 Feb 2020
 *      Author: maro
 */

#ifndef LED_H_
#define LED_H_

#include "TypeDefs.h"

#define MAX_BRIGHTNESS (150)

class LED {
public:
	LED(driverPin_t driverPin);
	void setup();
	void loop();
	void turnOn(void);
    void turnOff(void);
	virtual ~LED();
private:
	driverPin_t _driverPin;
};

#endif /* LED_H_ */
