/*
 * ChargingStation.h
 *
 *  Created on: 20 Feb 2020
 *      Author: maro
 */

#ifndef CHARGINGSTATION_H_
#define CHARGINGSTATION_H_

#include "TypeDefs.h"
#include <FastLED.h>

#define LED_UPDATE_INTERVAL (100)
//#define LED_FADING_TIME (1000)
#define LED_ANIMATION_BRIGHTNESS  (200)
#define LED_HUE_STEP  (5)
#define LED_HUE_STEP_COUNT_MAX  (18)
#define LED_HUE_STEP_COUNT_MIN  (1)
#define LED_SATURATION  (255) //for animation and idle
#define LED_IDLE_BRIGHTNESS  (50)
//#define LED_IDLE_HUE  (75) //like orange/yellow
#define LED_IDLE_HUE  (150) //likeblue

class ChargingStation {
public:
	ChargingStation(driverPin_t statusInPin, driverPin_t statusOutPin, CRGB* crgb);
	void setup(void);
	void loop(void);
	void setChargingOn(void);
	void setChargingOff(void);
	void ledChargingAnimation(void);
	bool isChargingOn(void);
	bool isChargingOff(void);
	bool statusHasChanged(void);
	virtual ~ChargingStation();
private:
	CRGB* _crgb;
	elapsedMillis_t _lastLEDUpdate;
	simpleCount_t _ledStepCounter;
	simpleCount_t _ledStepCountMin;
	on_off_state_t _charging;
	driverPin_t _statusInPin;
	driverPin_t _statusOutPin;
	bool _chargingManuallyOn;
	bool _statusHasChanged;
};

#endif /* CHARGINGSTATION_H_ */
