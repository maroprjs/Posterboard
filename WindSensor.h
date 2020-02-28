/*
 * WindSensor.h
 *
 *  Created on: 21 Feb 2020
 *      Author: maro
 */

#ifndef WINDSENSOR_H_
#define WINDSENSOR_H_
#include "TypeDefs.h"
#include "Modbus.h"

#define DEFAULT_MEASUREMENT_INTERVAL 500

   #define  WIND_SENSOR_ADDRESS 1 // slave address
   #define  WIND_FUNCTION_CODE 3 // function code
   #define  WIND_START_ADDRESS 22 // 0x16 start address
   #define  WIND_NUM_OF_REQUESTED_REGISTERS 1 //number of elements (coils or registers) to read -wind speed

class WindSensor {
public:
	WindSensor(Modbus* modbus, windspeed_t* windspeed, interval_t interval = DEFAULT_MEASUREMENT_INTERVAL);
	void setup(void);
	void loop(void);
	virtual ~WindSensor();
private:
	windspeed_t* _windspeed;
	Modbus* _modbus;
	uint8_t _u8state;
	modbus_t _request;
	//uint16_t _response[WIND_NUM_OF_REQUESTED_REGISTERS]; //query response on modbus
	uint16_t* _response;//there was probably a stack versus heap aka freestore
	elapsedMillis_t _elapsedTime;
	interval_t _measurementInterval;
	windspeed_t _windspeed10;
};

#endif /* WINDSENSOR_H_ */
