/*
 * GUI.h
 *
 *  Created on: 25 Feb 2020
 *      Author: maro
 */

#ifndef GUI_H_
#define GUI_H_
#include <stdio.h>
#include "TypeDefs.h"
#include "Solarfarm.h"
#include "Windmill.h"
#include "HydroStation.h"
#include "ChargingStation.h"

#define MAX_CMD_STR_BUFFER (25)
#define NUM_OF_CMD (14)
#define MODEM_CHECK_INTERVAL (100)

typedef struct tillCmd_t { //this is On Off time for the driving pin of the voltage meter
	//void (*call)(void);
    String cmdStr;
} tillCmd;

class GUI {
public:
	GUI(HardwareSerial* modem, Solarfarm* solarfarm, Windmill* windmill1, Windmill* windmill2,Windmill* windmill3, HydroStation* hydroStation, ChargingStation* chargingStation);
	virtual ~GUI();
	void setup(void);
	void loop(void);
	bool isAutoOn(void);
	bool statusHasChanged(void);
private:
	HardwareSerial* _modem;
	Solarfarm* _solarfarm;
	Windmill* _windmill1;
	Windmill* _windmill2;
	Windmill* _windmill3;
	HydroStation* _hydroStation;
	ChargingStation* _chargingStation;
	elapsedMillis_t _startMillis;
	interval_t _checkInterval;
	String _cmdString;
	//std::vector<tillCmd_t> _tillCmdVector; <-no std for vector inluded by default
	tillCmd_t* _tillCmdArray;
	on_off_state_t _auto;
	bool _statusHasChanged;
	void setAutomaticModusOn(void);
	void setAutomaticModusOff(void);
	void handleCmd(String cmdStr);
	bool cmdContains(String s, String search);

};

#endif /* GUI_H_ */
