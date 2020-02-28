/*
 * WindSensor.cpp
 *
 *  Created on: 21 Feb 2020
 *      Author: maro
 */

#include "WindSensor.h"
//#include <cstdlib.h>

WindSensor::WindSensor(Modbus* modbus, windspeed_t* windspeed, interval_t interval) {
	_windspeed = windspeed;
	_modbus = modbus;
	_u8state = 0;
	_measurementInterval = interval;
	_elapsedTime = 0;
	_windspeed10 = 0;
	_response = new uint16_t[WIND_NUM_OF_REQUESTED_REGISTERS];

}

void WindSensor::setup(void){
	//_modbus->begin(MODBUS_SPEED);
	//_modbus->setTimeOut( 2000 ); // if there is no answer in 2000 ms, roll over <- default is 1000

	_elapsedTime = millis();
}

void WindSensor::loop(void) {
	//_elapsedTime = millis();
	//int bufferSize = 0;
   switch( _u8state ) {
	  case 0:
	    if (millis() >= (_elapsedTime + _measurementInterval)) _u8state++; // wait state
	    break;
	  case 1:
	    _request.u8id = WIND_SENSOR_ADDRESS; // slave address
	    _request.u8fct = WIND_FUNCTION_CODE; // function code (this one is registers read)
	    _request.u16RegAdd = WIND_START_ADDRESS; // start address in slave
	    _request.u16CoilsNo = WIND_NUM_OF_REQUESTED_REGISTERS; // number of elements (coils or registers) to read
	    _request.au16reg = _response; // pointer to a memory array in the Arduino
	    _modbus->query( _request );
	    _u8state++;
	    break;
	  case 2:
		 _modbus->poll();
		 if (_modbus->getState() == COM_IDLE) {
			  //DPRINTLN("IDLE");
   	          for (int i = 0; i < WIND_NUM_OF_REQUESTED_REGISTERS; i++){
   	              //DPRINT(_response[i]);
   	              _windspeed10 = _response[0];
  	              *_windspeed = _windspeed10 / 10;
  	              //DPRINTLN(*_windspeed);
   	           };
		 };
		 _u8state = 0;
		 _elapsedTime = millis() + (rand() % 50 + 50); //random betwen 50 and 100
         break;
   }
}

WindSensor::~WindSensor() {
	// TODO Auto-generated destructor stub
}

