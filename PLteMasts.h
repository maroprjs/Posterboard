/*
 * PLteMasts.h
 *
 *  Created on: 20 Feb 2020
 *      Author: maro
 */

#ifndef PLTEMASTS_H_
#define PLTEMASTS_H_

#include "TypeDefs.h"
#include <FastLED.h>

#define LED_UPDATE_INTERVAL (100)
#define LED_FADING_TIME (1000)
#define LED_MAX_BRIGHTNESS  (50)
#define LED_FADE_FACTOR  (100)

class PLteMasts {
public:
	PLteMasts(CRGB* crgb, CRGB baseColor = CRGB::Red);
	void setup(void);
	void loop(void);
	void beaconOn(void);
	void beaconOff(void);
	void changeColor(CRGB newColor);
	void ledBeaconAnimation(void);
	virtual ~PLteMasts();
private:
	CRGB* _crgb;
	CRGB _baseColor;
	elapsedMillis_t _lastLEDUpdate;
	simpleCount_t _ledStepCounter;
	simpleCount_t _ledStepCountMax;
};

#endif /* PLTEMASTS_H_ */
