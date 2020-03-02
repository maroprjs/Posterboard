/*
 * Solarmodul.cpp
 *
 *  Created on: 18 Feb 2020
 *      Author: maro
 */

#include "Solarmodul.h"
#include "Debug.h"

Solarmodul::Solarmodul(sensingPin_t sensingPin, pinSample_t* currentSample, interval_t interval, interval_t powerActiveTime) {
	_sampleInterval = interval;
	_powerActiveTime = powerActiveTime;
	_sensingPin = sensingPin;
	_currentSample = currentSample; //pointer to global measurement "register"
	_sampleDeviation = 0;
	_deviationMax = SIGNAL_MIN;
	_startMillis = 0;
	_stopped = true;
	_emitSolarPower = OFF;
	_startMillisPowerActive = 0;
	_calibrationValueUpdateInterval = CALIBRATION_UPDATE_INTERVAL;
	_lastCalibrationUpdated = 0;
	_statusHasChanged = true;
	_avgStd = NULL;
}

void Solarmodul::setup(void){
	//pinMode(_sensingPin, INPUT_ANALOG);
	_avgStd = new AvgStd(NUM_OF_STD_SAMPLES);
	calibrate(INITIAL_CALIBRATION_TIME);
	start(); // Start of sample window <-make sure this is called as one of the last function in setup


}

//collect some sensor values during setup, in order to adapt to ambient light conditions
void Solarmodul::calibrate(interval_t calibrationTime){
	elapsedMillis_t startMillis = millis();
	_deviationMax = SIGNAL_MIN;
	_sampleDeviation = 0;

	while (millis() < startMillis + calibrationTime) {
	   *_currentSample = analogRead(_sensingPin);
	   _avgStd->addReading(*_currentSample);
	   if (_avgStd->getN() >= NUM_OF_STD_SAMPLES){
		   _sampleDeviation = _avgStd->getStd() ;
		    // record the maximum sensor value
		    if (_sampleDeviation > _deviationMax) {
		      _deviationMax = _sampleDeviation;
		    }
	   }
	   delay(DEFAULT_SAMPLE_INTERVAL);
	};
	_avgStd->setRejectionSigma(_deviationMax);
    PRINT("SolarMod Mean: ");PRINT(_avgStd->getMean());PRINT(" DevMax: ");PRINTLN(_deviationMax);
	_deviationMax = SIGNAL_MIN;
}

//start-/stop- mechanism if heavy delay in main loop requires it.
void Solarmodul::start(){
	_startMillis = 0;
	_startMillis= millis(); // Start of sample window <-make sure this is called as one of the last function in setup
	_lastCalibrationUpdated = millis();
	 _emitSolarPower = ON;//true;
	 _startMillisPowerActive = millis();
	_stopped = false;
	_statusHasChanged = true;

}

void Solarmodul::stop(){
	_stopped = true;
	_emitSolarPower = OFF;
	_statusHasChanged = true;

}

void Solarmodul::loop(void){
       if ((_emitSolarPower == ON /*true*/) && (millis() >= (_startMillisPowerActive + _powerActiveTime))){
    	   _emitSolarPower = OFF; //false;
    	   _statusHasChanged = true;
       }

	   if ((_stopped == false) && (_emitSolarPower == OFF)){

		  // collect data at given interval
	      if((millis() - _startMillis >= _sampleInterval) && (_avgStd->getN() >= NUM_OF_STD_SAMPLES))
	      {
	         *_currentSample = analogRead(_sensingPin);
		     if (_avgStd->checkAndAddReading(*_currentSample) == false){ //readning not added due bigger than deviation
                 PRINT("Solar Emition Recognized! currentSample: ");PRINT(*_currentSample);PRINT(" STD deviation: ");PRINTLN(_avgStd->getStd());
	        	 //emit solar power for some time
	        	 _emitSolarPower = ON;//true;
	        	 _statusHasChanged = true;
	        	 _startMillisPowerActive = millis();
	         }
	         else{
		         updateCalibrationValues();//TODO: check if enough to be updated here
		         _startMillis= millis();  // Start the new interval
	         };
		     *_currentSample = _avgStd->getMean();//just in case Till reads is, it shouldn't fluctuate too much
	      };
	   };
}


//light conditions might change over time....contineously ongoing calibration should avoid unmanaged solar activation
void Solarmodul::updateCalibrationValues(void){

	if (millis() - _lastCalibrationUpdated <= _calibrationValueUpdateInterval) {

		   _sampleDeviation = _avgStd->getStd() ;
		    if (_sampleDeviation > _deviationMax) {
		      _deviationMax = _sampleDeviation;
		    };

    }else{
    	_avgStd->reset();
   	   _avgStd->setRejectionSigma(_deviationMax);
       PRINT("SolarMod Mean: ");PRINT(_avgStd->getMean());PRINT(" DevMax: ");PRINTLN(_deviationMax);
   	   _deviationMax = SIGNAL_MIN;
   	   _lastCalibrationUpdated = millis();
    }
}


//API to check if light emission is recognized
bool Solarmodul::isOff(void){
	return ((_emitSolarPower == OFF) ? true : false);
}

bool Solarmodul::isOn(void){
	return ((_emitSolarPower == ON) ? true : false);
}


bool Solarmodul::statusHasChanged(void){
    bool retStat = _statusHasChanged;
    _statusHasChanged = false;
    return retStat;
}

Solarmodul::~Solarmodul() {
	// TODO Auto-generated destructor stub
}

