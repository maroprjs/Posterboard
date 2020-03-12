/*
 * Windmill.h
 *
 *  Created on: 17 Feb 2020
 *      Author: maro
 */

#ifndef WINDMILL_H_
#define WINDMILL_H_

#include "TypeDefs.h"
#include <FastLED.h>

#define DEFAULT_MEASUREMENT_INTERVAL 500
#define WIND_THRESHOLD_ON   (2.0)
#define WIND_THRESHOLD_OFF   (0)

#define LED_UPDATE_INTERVAL (100)
#define LED_FADING_TIME (1000)
#define LED_MAX_BRIGHTNESS  (50)
#define LED_FADE_FACTOR  (100)

class Windmill {
public:
	Windmill(driverPin_t motorPin, CRGB* crgb, windspeed_t* wind);
	void setup(void);
	void loop(void);
	void turnOn(void);
	void turnOff(void);
	void toggleOnOff(void);
	bool isOn(void);
	bool isOff(void);
	void ledAnimation(void);
	bool statusHasChanged(void);
	void setAutomaticModusOn(void);
	void setAutomaticModusOff(void);
	virtual ~Windmill();
private:
	driverPin_t _motorPin;
	 CRGB* _crgb;
	 windspeed_t* _wind;
	 on_off_state_t _on_off_state;
	 interval_t _measurementInterval;
	 elapsedMillis_t _elapsedTime;
	 elapsedMillis_t _lastLEDUpdate;
	 simpleCount_t _ledStepCounter;
	 simpleCount_t _ledStepCountMax;
	 on_off_state_t _auto;
	 bool _statusHasChanged;
};

#endif /* WINDMILL_H_ */
