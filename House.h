/*
 * House.h
 *
 *  Created on: 18 Feb 2020
 *      Author: maro
 */

#ifndef HOUSE_H_
#define HOUSE_H_
#include "TypeDefs.h"
#include <FastLED.h>
#include "LED.h"


class House {
public:
	House(CRGB* crgb, LED* led);
	House(CRGB* crgb);
	void setup(void);
	void loop(void);
	void lightOn(void);
	void lightOff(void);
	virtual ~House();

private:
	LED* _led;
	CRGB* _crgb;
};

#endif /* HOUSE_H_ */
