/*
 * Solarmodul.cpp
 *
 *  Created on: 18 Feb 2020
 *      Author: maro
 */

#include "Solarmodul.h"
#include "Debug.h"

Solarmodul::Solarmodul(sensingPin_t sensingPin, pinSample_t* currentSample, pinSample_t sampleOffset, interval_t interval, interval_t powerActiveTime) {
	_sampleInterval = interval;
	_powerActiveTime = powerActiveTime;
	_sensingPin = sensingPin;
	_currentSample = currentSample; //pointer to global measurement "register"
	_lastSample = 0;
	_samplePeakToPeak = 0;
	_sampleAverage = 0;
	_sampleVariance = 0;
	_sampleTotalSum = 0;
	_sampleCount = 0;
	_sampleMax = SIGNAL_MIN;
	_sampleMin = SIGNAL_MAX;
	_sampleNewMax = SIGNAL_MIN;
	_sampleNewMin = SIGNAL_MAX;
	_startMillis = 0;
	_stopped = true;
	_emitSolarPower = OFF;
	_startMillisPowerActive = 0;
	_calibrationValueUpdateInterval = CALIBRATION_UPDATE_INTERVAL;
	_lastCalibrationUpdated = 0;
	_sampleOffset = sampleOffset;
	_statusHasChanged = true;
}

void Solarmodul::setup(void){
	//pinMode(_sensingPin, INPUT_ANALOG);
	calibrate(INITIAL_CALIBRATION_TIME);
	start(); // Start of sample window <-make sure this is called as one of the last function in setup

}

//collect some sensor values during setup, in order to adapt to ambient light conditions
void Solarmodul::calibrate(interval_t calibrationTime){
	elapsedMillis_t startMillis = millis();
	_sampleCount = 0;
	_sampleTotalSum = 0;
	_sampleAverage = 0;
	_sampleVariance = 0;
	_samplePeakToPeak = 0;
	while (millis() < startMillis + calibrationTime) {
	   *_currentSample = analogRead(_sensingPin);
	   _sampleTotalSum += *_currentSample;
	   _sampleCount++;

	   // record the maximum sensor value
	   if (*_currentSample > _sampleMax) {
	      _sampleMax = *_currentSample;
	   };
	    // record the minimum sensor value
	    if (*_currentSample < _sampleMin) {
	      _sampleMin = *_currentSample;
	    }
	    delay(100);
	};
	_sampleAverage = _sampleTotalSum / _sampleCount;
	_sampleVariance = abs(_sampleMax - _sampleAverage);
	_samplePeakToPeak = _sampleMax - _sampleMin; // this should be he highest gradient to be expected
	_lastSample = *_currentSample;
    PRINT("SolarMod Av: ");PRINT(_sampleAverage);PRINT(" Var: ");PRINT(_sampleVariance);PRINT(" PeakPeak: ");PRINTLN(_samplePeakToPeak);
}

//start-/stop- mechanism if heavy delay in main loop requires it.
void Solarmodul::start(){
	_sampleMax = SIGNAL_MIN;
	_sampleMin = SIGNAL_MAX;
	_sampleNewMax = SIGNAL_MIN;
	_sampleNewMin = SIGNAL_MAX;
	//_sampleVariance = 0;
	//_sampleAverage = 0;
	_sampleTotalSum = 0;
	_sampleCount = 0;
	_startMillis = 0;
	//TODO: better to take last or average or none?:
	//*_currentSample = *_currentSample;
	//*_currentSample = _sampleAverage;
	//_lastSample = _lastSample;
	//_lastSample = _sampleAverage;
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
	      if(millis() - _startMillis >= _sampleInterval)
	      {
	    	  _lastSample = *_currentSample;
	         *_currentSample = analogRead(_sensingPin);
             //Serial.println(*_currentSample);

	         //if (*_currentSample - _lastSample > _sampleAverage + _sampleVariance/2){//_samplePeakToPeak){ //TODO: or should I just simply define an offset?
		     if (*_currentSample - _lastSample > _samplePeakToPeak + _sampleOffset){//_sampleVariance){ //TODO: or should I just simply define an offset?
                 PRINT("Solar Emition Recognized! peakPeak+Offset: ");PRINT(_samplePeakToPeak + _sampleOffset);PRINT(" gradient: ");PRINTLN(*_currentSample - _lastSample);
	        	 //emit solar power for some time
	        	 _emitSolarPower = ON;//true;
	        	 _statusHasChanged = true;
	        	 _startMillisPowerActive = millis();
	         }
	         else{
		         updateCalibrationValues();//TODO: check if enough to be updated here
		         _startMillis= millis();  // Start the new interval
	         };
	      };
	   };
}


//light conditions might change over time....contineously ongoing calibration should avoid unmanaged solar activation
void Solarmodul::updateCalibrationValues(void){

	if (millis() - _lastCalibrationUpdated <= _calibrationValueUpdateInterval) {
        _sampleTotalSum += *_currentSample;
        _sampleCount += 1;
	   // record the maximum sensor value
	   if (*_currentSample > _sampleNewMax) {
	      _sampleNewMax = *_currentSample;
	   };
	   // record the minimum sensor value
	   if (*_currentSample < _sampleNewMin) {
	      _sampleNewMin = *_currentSample;
       }
    }else{
       _sampleAverage = _sampleTotalSum / _sampleCount; //new average
       _sampleVariance = abs(_sampleNewMax - _sampleAverage); //new variance
       _samplePeakToPeak = _sampleNewMax - _sampleNewMin; // this should be he highest gradient to be expected
   	   _sampleMax = _sampleNewMax;
   	   _sampleMin = _sampleNewMin;
   	   _sampleNewMin = SIGNAL_MAX;
   	   _sampleNewMax = SIGNAL_MIN;
   	   _sampleTotalSum = 0;
   	   _sampleCount = 0;
       PRINT("New SolarMod Av: ");PRINT(_sampleAverage);PRINT(" Var: ");PRINT(_sampleVariance);PRINT(" PeakPeak: ");PRINTLN(_samplePeakToPeak);
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

void Solarmodul::setNewSampleOffset(pinSample_t offset){
	_sampleOffset = offset;
}
void Solarmodul::resetSampleOffset(){
	_sampleOffset = SAMPLE_OFFSET;
}

bool Solarmodul::statusHasChanged(void){
    bool retStat = _statusHasChanged;
    _statusHasChanged = false;
    return retStat;
}

Solarmodul::~Solarmodul() {
	// TODO Auto-generated destructor stub
}

