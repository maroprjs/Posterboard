/*
 * GUI.cpp
 *
 *  Created on: 25 Feb 2020
 *      Author: maro
 */

#include "GUI.h"
#include "Debug.h"

GUI::GUI(HardwareSerial* modem, Solarfarm* solarfarm, Windmill* windmill1, Windmill* windmill2,Windmill* windmill3, HydroStation* hydroStation, ChargingStation* chargingStation) {
	_modem = modem;
	_solarfarm = solarfarm;
	_windmill1 = windmill1;
	_windmill2 = windmill2;
	_windmill3 = windmill3;
	_hydroStation = hydroStation;
	_chargingStation = chargingStation;
	_cmdString = "";
	_tillCmdArray = NULL;
	_startMillis = 0;
	_checkInterval = MODEM_CHECK_INTERVAL;
	_auto = ON;
	_statusHasChanged = true;
}

void GUI::setup(void){
	_cmdString.reserve(MAX_CMD_STR_BUFFER);
	_tillCmdArray = new tillCmd_t[NUM_OF_CMD];
	_tillCmdArray[0].cmdStr = "{\"windmill1\":1}\n"; //_tillCmdArray[0].call = windmill1On;
	_tillCmdArray[1].cmdStr = "{\"windmill1\":0}\n"; //_tillCmdArray[1].call = windmill1Off;
	_tillCmdArray[2].cmdStr = "{\"windmill2\":1}\n"; //_tillCmdArray[2].call = windmill2On;
	_tillCmdArray[3].cmdStr = "{\"windmill2\":0}\n"; //_tillCmdArray[3].call = windmill2Off;
	_tillCmdArray[4].cmdStr = "{\"windmill3\":1}\n"; //_tillCmdArray[2].call = windmill2On;
	_tillCmdArray[5].cmdStr = "{\"windmill3\":0}\n";
	_tillCmdArray[6].cmdStr = "{\"solar\":1}\n"; //_tillCmdArray[4].call = solarfarmOn;
	_tillCmdArray[7].cmdStr = "{\"solar\":0}\n"; //_tillCmdArray[5].call = solarfarmOff;
	_tillCmdArray[8].cmdStr = "{\"charge\":1}\n"; //_tillCmdArray[6].call = chargingStationOn;
	_tillCmdArray[9].cmdStr = "{\"charge\":0}\n"; //_tillCmdArray[7].call = chargingStationOff;
	_tillCmdArray[10].cmdStr = "{\"hydro\":1}\n"; //_tillCmdArray[8].call = hydroOn;
	_tillCmdArray[11].cmdStr = "{\"hydro\":0}\n"; //_tillCmdArray[9].call = hydroOff;
	_tillCmdArray[12].cmdStr = "{\"auto\":1}\n"; //
	_tillCmdArray[13].cmdStr = "{\"auto\":0}\n"; //
	_startMillis= millis();
}


void GUI::loop(void){

	//bool cmdComplete = false;
    if(millis() - _startMillis >= _checkInterval)
    {


	   while (_modem->available()){
	      char inChar = (char)_modem->read();
		  _cmdString += inChar;
          if (inChar == '\n') {
             DPRINT("cmd received: ");DPRINTLN(_cmdString);
             handleCmd(_cmdString);
             _cmdString = "";
             //cmdComplete = true;
          };
	  };
	  _startMillis= millis();
    };
}

bool GUI::isAutoOn(){
	return ((_auto == ON) ? true : false);
}

void GUI::handleCmd(String cmdStr){
	int i = 0;
	while (i <= NUM_OF_CMD){
		if (cmdStr == _tillCmdArray[i].cmdStr) break;
		i++;
	};
	DPRINT(_tillCmdArray[i].cmdStr);
	switch( i ) {
	   case 0:
		   _windmill1->turnOn();
		   break;
	   case 1:
		   _windmill1->turnOff();
		   break;
	   case 2:
		   _windmill2->turnOn();
		   break;
	   case 3:
		   _windmill2->turnOff();
		   break;
	   case 4:
		   _windmill3->turnOn();
		   break;
	   case 5:
		   _windmill3->turnOff();
		   break;
	   case 6:
		   _solarfarm->turnOn();
		   break;
	   case 7:
		   _solarfarm->turnOff();
		   break;
	   case 8:
		   _chargingStation->setChargingOn();
		   break;
	   case 9:
		   _chargingStation->setChargingOff();
		   break;
	   case 10:
		   _hydroStation->turnOn();
		   break;
	   case 11:
		   _hydroStation->turnOff();
		   break;
	   case 12:
		   this->setAutomaticModusOn();
		   break;
	   case 13:
		   this->setAutomaticModusOff();
		   break;
	   case 14:
		   PRINTLN("Command received but not recognized!!");
	};
}



void GUI::setAutomaticModusOn(void){
	_auto = ON;
	_windmill1->setAutomaticModusOn();
	_windmill2->setAutomaticModusOn();
	_windmill3->setAutomaticModusOn();
	_solarfarm->setAutomaticModusOn();
	_chargingStation->setAutomaticModusOn();
	//_hydroStation <- not needed here, controlled by posterboard.ino
	_statusHasChanged = true;
}

void GUI::setAutomaticModusOff(void){
	_auto = OFF;
	_windmill1->setAutomaticModusOff();
	_windmill2->setAutomaticModusOff();
	_windmill3->setAutomaticModusOff();
	_solarfarm->setAutomaticModusOff();
	_chargingStation->setAutomaticModusOff();
	//_hydroStation <- not needed here, controlled by posterboard.ino
	_statusHasChanged = true;
}

bool GUI::statusHasChanged(void){
    bool retStat = _statusHasChanged;
    _statusHasChanged = false;
    return retStat;
}


bool GUI::cmdContains(String s, String search) {
	bool retVal = false;
	if (search.length() > s.length()) return retVal;
	if (s.substring(0, search.length()) == search) retVal = true;

    return retVal;
}


GUI::~GUI() {
	// TODO Auto-generated destructor stub
}

