/*
 * Solarmodul.h
 *
 *  Created on: 18 Feb 2020
 *      Author: maro
 */

#ifndef SOLARMODUL_H_
#define SOLARMODUL_H_

#include "TypeDefs.h"
#include "AvgStd.h"

#define INITIAL_CALIBRATION_TIME (10000) //DEFAULT_SAMPLE_INTERVAL * NUM_OF_STD_SAMPLES * 10 (10 more values for calibration)
#define DEFAULT_SAMPLE_INTERVAL (100) //[ms]
#define NUM_OF_STD_SAMPLES (10)
#define CALIBRATION_UPDATE_INTERVAL (60000)
#define SIGNAL_MIN (0)
#define SIGNAL_MAX (1024)
#define SOLAR_POWER_AVTIVE_TIME (30000) //how long solar panel will report light, even it was there only shortl

class Solarmodul {
public:
	Solarmodul(sensingPin_t sensingPin, pinSample_t* currentSample, interval_t interval = DEFAULT_SAMPLE_INTERVAL, interval_t powerActiveTime = SOLAR_POWER_AVTIVE_TIME);
	void setup();
	void loop();
	void calibrate(interval_t = INITIAL_CALIBRATION_TIME);
	void start();
	void stop();
	bool isOn(void);
	bool isOff(void);
	bool statusHasChanged(void);
	virtual ~Solarmodul();
private:
	interval_t _sampleInterval;
	interval_t _powerActiveTime;
	interval_t _calibrationValueUpdateInterval;
	sensingPin_t _sensingPin;
	pinSample_t* _currentSample;
	//pinSample_t _lastSample;
	pinSample_t _deviationMax;
	pinSample_t _sampleDeviation;
	AvgStd* _avgStd;
	on_off_state_t _emitSolarPower;//light emission on or off
	elapsedMillis_t _startMillis; //used for sample interval
	elapsedMillis_t _startMillisPowerActive; //when light is recognized
	elapsedMillis_t _lastCalibrationUpdated;
	bool _stopped;
	bool _statusHasChanged;


	void updateCalibrationValues(void);
};

#endif /* SOLARMODUL_H_ */
