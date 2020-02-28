/*
 * Multimeter.h
 *
 *  Created on: 22 Feb 2020
 *      Author: maro
 */

#ifndef MULTIMETER_H_
#define MULTIMETER_H_

#include "TypeDefs.h"
#define DEFAULT_SAMPLE_WINDOW 1000
#define CURR_RESOLUTION (0.00489)
#define VOLT_RESOLUTION (0.0263) //185mV/A  ACS712ELC-05B -> let's say 512 = 0; +-5A; 2,5V/5A=> ADC val = 38units/A

class Multimeter {
public:
	Multimeter(sensingPin_t currentPin,sensingPin_t voltPin, current_t* current,  voltage_t* volt);
	void setup(void);
	void loop(void);
	virtual ~Multimeter();
private:
	sensingPin_t _voltPin;
	sensingPin_t _currentPin;

	pinSample_t _voltADSample;
	pinSample_t _currentADSample;
	voltage_t* _voltPtr;
	current_t* _currentPtr;
	interval_t _sampleWindow;
	total_t _currSampleTotalSum;
	simpleCount_t _currSampleCount;
	pinSample_t _currSampleAverage;
	total_t _voltSampleTotalSum;
	simpleCount_t _voltSampleCount;
	pinSample_t _voltSampleAverage;
	elapsedMillis_t _startMillis;
};

#endif /* MULTIMETER_H_ */
