/*
 * Debug.h
 *
 *  Created on: 12.02.2020
 *      Author: maro
 */

#ifndef DEBUG_H_
#define DEBUG_H_


//#define TEST_WITH_MEGA

#define DEBUG
#ifdef DEBUG
	#define PRINT_INIT(x)	Serial.begin(x)
	#define DPRINT(x)  	Serial.print(x)
	#define DPRINTLN(x)  Serial.println(x)
	#define ZVERSION "debug xxx " + String(__DATE__)

#else
	#define PRINT_INIT(...)
	#define DPRINT(...)
	#define DPRINTLN(...)
	#define ZVERSION "v1.1.0" + String(__DATE__)

#endif

	#define PRINT(x)  	Serial.print(x)
	#define PRINTLN(x)  Serial.println(x)
	#define ZUP_TIME " " + String(millis()/1000) + " [s]"

#endif /* DEBUG_H_ */
