/**
 *  @file		Posterboard.ino
 *
 *  @author		maro
 *  @date		mm/dd/yyyy
 *  @version	x.y
 *
 *  @brief 		Main
 *
 *  @section DESCRIPTION
 *
 *  Instantiation and control of objects placed onto poster and table of Wireless Ulitility ENT.5 Demo MWC'20
 *
 *  used libraries and references:
 *  	-Modbus.h (turned into C++ Class) from "https://github.com/CONTROLLINO-PLC/CONTROLLINO_Library"
 *  	-"https://github.com/FastLED/FastLED" to drive LED stripe
 *  	-https://github.com/mathertel/RotaryEncoder
 *
 */



/////////// INcludes /////////////////
//TODO: check if Controllino header makes sense (currently I don't see an asset):
//#include <Controllino.h>  /* Usage of CONTROLLINO library allows you to use CONTROLLINO_xx aliases in your sketch. */
#include "Debug.h"
#include "TypeDefs.h"
#include <stdio.h>

#include <FastLED.h>
#include "LED.h"
#include "Multimeter.h"
#include "HydroStation.h"
#include "Solarmodul.h"
#include "Solarfarm.h"
#include "Windmill.h"
#include "House.h"
#include "PLteMasts.h"
#include "Substation.h"
#include "ChargingStation.h"

#include "Modbus.h"
#include "HumiditySensor.h"
#include "WindSensor.h"

#include <RotaryEncoder.h>
#include "ControlCenter.h"

#include "GUI.h"

/////////defines //////////////////////
#define CONTROLLINO_R0 22 //relay #0 for windmill 1 motor //= mega pin 22
#define CONTROLLINO_R1 23 //relay #1 for windmill 2 motor //= mega pin 23
#define CONTROLLINO_R2 24 //relay #2 for windmill 2 motor //= mega pin 24
#define CONTROLLINO_R8 30 //relay #8 for hydro station //= mega pin 30

#define CONTROLLINO_D6 8 //digital 6 rotary encoder s1 //= mega pin 8
#define CONTROLLINO_D7 9 //digital 7 rotary encoder s2 //= mega pin 9
#define CONTROLLINO_D8 10 //digital 8 rotary encoder key//= mega pin 10
#define CONTROLLINO_D9 11 //digital 9 driver pin for volt meter panel//= mega pin 11

#define CONTROLLINO_D10 12 //digittal 10 used for LED stripe driver //= mega pin 12
#define CONTROLLINO_PIN_HEADER_DIGITAL_OUT_12 42 //digital 12 used for house 1 LED //= mega pin 42
#define CONTROLLINO_PIN_HEADER_DIGITAL_OUT_13 43 //digital 13 used for house 2 LED //= mega pin 43
#define CONTROLLINO_PIN_HEADER_DIGITAL_OUT_14 44 //digital 14 used for charging station INPUT //= mega pin 44
#define CONTROLLINO_PIN_HEADER_DIGITAL_OUT_15 45 //digital 15 used to charging station OUTPUT //= mega pin 45

#define CONTROLLINO_A6 60//analog #6 for current sensor  //= mega PIN_A6
#define CONTROLLINO_A7 61//analog #7 for voltage solar sensor  //= mega PIN_A7
#define CONTROLLINO_PIN_HEADER_ANALOG_ADC_IN_08 62 //analog #8 for house 1 solar sensor  //= mega PIN_A8
#define CONTROLLINO_PIN_HEADER_ANALOG_ADC_IN_09 63 //analog #9 for house 2 solar sensor //= mega PIN_A9
#define CONTROLLINO_PIN_HEADER_ANALOG_ADC_IN_12 66 //analog #12 for windmill 1 solar sensor //= mega PIN_A12
#define CONTROLLINO_PIN_HEADER_ANALOG_ADC_IN_13 67 //analog #13 for windmill 2 solar sensor //= mega PIN_A13


#define LED_STRIPE_PIN CONTROLLINO_D10
#define NUM_OF_LEDS 28 //on stripe

#define MODBUS_SPEED (9600)

#define PUBLISH_INTERVAL   (1500)

////////// Object Instantiation //////////////////
/**************************
 * LED stripe
 *
 * # (note the count from 1)
 * 1 house 1
 * 2 antenne 1 (= PLTE Mast)
 * 4 windmill 1
 * 6 windmill 2
 * 8 antenne 2 (= PLTE Mast)
 * 10 house 2
 * 12 solarpark (= house class)
 * 13 house 3
 * 15 house 4
 * 16 chargin station
 * 19 solar toproof (= PLTE Mast)
 * 23 substation 1
 * 25 substation 2
 * 28 substation 3
 *
 */
CRGB leds[NUM_OF_LEDS];

/*************************
 * SAR Modem
 */
char measData[512] ;
char stateData[512] ;
HardwareSerial* sarModem;
//placeholder

/****************
 * Multimeter
 */
current_t current = 0;
char str_current[8];
voltage_t volt = 0;
char str_volt[8];
Multimeter multimeter(CONTROLLINO_A6, CONTROLLINO_A7, &current, &volt);

/****************
 * Hydro Station
 */
HydroStation hydroStation(CONTROLLINO_R8);

/*******************
 * Solarfarm
 *
 * TODO: change this, Solar Modules to be instantiated by Solarfarm object
 * -the SolarModule setup() functions are called inside solarfarm.
 * -Salarfarm has been introduced, in order to have more control on posterboard level,
 *  instead of instantiating solar modules with houses and windmills to where they
 *  actually belong to.
 */
bool solarOn = 0;
pinSample_t solar1ADSample = 0;;
pinSample_t solar2ADSample = 0;;
pinSample_t solar3ADSample = 0;;
pinSample_t solar4ADSample = 0;;
int offset1 = -2;//the windmill solars need a little more light
int offset2 = 0;
Solarmodul solarmodul1(CONTROLLINO_PIN_HEADER_ANALOG_ADC_IN_12, &solar1ADSample, offset1); //from Windmill1
Solarmodul solarmodul2(CONTROLLINO_PIN_HEADER_ANALOG_ADC_IN_13, &solar2ADSample, offset1); //from Windmill2
Solarmodul solarmodul3(CONTROLLINO_PIN_HEADER_ANALOG_ADC_IN_08, &solar3ADSample, offset2); //from House1
Solarmodul solarmodul4(CONTROLLINO_PIN_HEADER_ANALOG_ADC_IN_09, &solar4ADSample, offset2); //from House2
Solarfarm solarfarm(&solarmodul1, &solarmodul2, &solarmodul3, &solarmodul4);
//Solarfarm solarfarm(&solarmodul1); //for testing

/******************************************
 * Modbus for Temperature-/Humidity- and Wind- Sensor
 *
 *TODO: don't forget to switch back DE and nRE ports in Modbus.h (flag set in Debug.h)
 */
Modbus modbus(0,3,0);//master adrress, serial id, tx pin(not used, this parameter is overwritten in library)


/******************************************
 * RS485 Temperature- and Humidity- Sensor
 */
temperature_t temp = 0;
char str_temp[6];
humidity_t humi = 0;
char str_humi[6];
HumiditySensor humiditySensor(&modbus, &temp, &humi);

/*******************
 * RS485 Wind Sensor
 */
windspeed_t wind = 0;
char str_wind[6];
WindSensor windSensor(&modbus, &wind);

/*******************
 * Wind Generators
 */
bool windmill1On = false;
bool windmill2On = false;
Windmill windmill1(CONTROLLINO_R0, &leds[5], &wind);
Windmill windmill2(CONTROLLINO_R1, &leds[3], &wind);
Windmill windmill3(CONTROLLINO_R2, &leds[3], &wind); //this is spare for e.g. lego

/*******************
 * Houses
 */
LED led1(CONTROLLINO_PIN_HEADER_DIGITAL_OUT_12);//the houses have a normal led as well
House house1(&leds[0], &led1); //look leds[x] mapping above
LED led2(CONTROLLINO_PIN_HEADER_DIGITAL_OUT_13);//the houses have a normal led as well
House house2(&leds[9], &led2); //look leds[x] mapping above
House house3(&leds[12]); //look leds[x] mapping above
House house4(&leds[14]); //look leds[x] mapping above
/*******************
 * solar park
 *
 * NOTE the difference to "solarfarm" (the solarpark is just a drawn object with a LED)
 *
 */
House solarpark(&leds[11]); //look leds[x] mapping above

/*******************
 * pLTE Masts
 */
PLteMasts mast1(&leds[1]); //look leds[x] mapping above
PLteMasts mast2(&leds[7]); //look leds[x] mapping above
PLteMasts mast3(&leds[18]); //look leds[x] mapping above

/************************
 * Sub Stations
 *
 */
Substation substation1(&leds[22]); //look leds[x] mapping above
Substation substation2(&leds[24]); //look leds[x] mapping above
Substation substation3(&leds[27]); //look leds[x] mapping above

/*
 * Control Station (rotary encoder)
 *
 *  encoder setup done in control center
 */
int encPosition = 0;;
RotaryEncoder encoder(CONTROLLINO_D6 /*s1*/, CONTROLLINO_D7 /*s2*/);
ControlCenter controlCenter(&encoder, CONTROLLINO_D8 /*push key*/,CONTROLLINO_D9 /*voltmeter pin*/, &encPosition, &substation1,&substation2,&substation3);

/************************
 * Charging Station
 *
 * first pin as input pin, second as output pin!!
 */
ChargingStation chargingStation(CONTROLLINO_PIN_HEADER_DIGITAL_OUT_14, CONTROLLINO_PIN_HEADER_DIGITAL_OUT_15, &leds[15]);

/************************
 * GUI
 *
 * publishin and presentation related functions
 */
GUI gui(&Serial2, &solarfarm, &windmill1, &windmill2, &windmill3, &hydroStation, &chargingStation);

//////// Global primitive variables /////////////////
interval_t publish_interval = PUBLISH_INTERVAL;
elapsedMillis_t currentMillis = 0;
elapsedMillis_t previousMillis = 0;



void setup()
{
	PRINT_INIT( 9600 );
	sarModem = &Serial2;
	sarModem->begin(9600);
	FastLED.addLeds<WS2811, LED_STRIPE_PIN, RGB>(leds, NUM_OF_LEDS);
	hydroStation.setup();
	solarfarm.setup();
	modbus.begin(MODBUS_SPEED);
	humiditySensor.setup();
	windSensor.setup();
	windmill1.setup();
	windmill2.setup();
	windmill3.setup();
	house1.setup();
	house2.setup();
	house3.setup();
	house4.setup();
	solarpark.setup();
	mast1.setup();
	mast2.setup();
	mast3.setup();
    substation1.setup();
    substation2.setup();
    substation3.setup();
    controlCenter.setup();
    chargingStation.setup();
    multimeter.setup();
    gui.setup();




}


void loop()
{

	//////////////////// Let it roll  ////////////////////////////////
	solarfarm.loop();
	humiditySensor.loop();
	windSensor.loop();
	windmill1.loop();
	windmill2.loop();
	windmill3.loop();
	house1.loop();
	house2.loop();
	house3.loop();
	house4.loop();
	solarpark.loop();
	mast1.loop();
	mast2.loop();
	mast3.loop();
    substation1.loop();
    substation2.loop();
    substation3.loop();
	controlCenter.loop();
	chargingStation.loop();
	multimeter.loop();
	gui.loop();
	FastLED.show();
	currentMillis = millis();
	if (currentMillis - previousMillis >= publish_interval) {

		/////////////////////   Measurement Reports /////////////////////////
       /****
        *   %.2f not working here due performance...
        *  3 is mininum width, 1 is precision; float value is copied onto str_temp
        */
       dtostrf(temp, 3, 1, str_temp);
       dtostrf(humi, 3, 1, str_humi);
       dtostrf(wind, 3, 1, str_wind);
       //dtostrf(current, 4, 3, str_current);
       //dtostrf(volt, 4, 3, str_volt);
	   sprintf(measData,"{\"topic\":\"meas\",\"temp\":%s,\"humi\":%s,\"wind\":%s,\"volt\":%d,\"curr\":%d,\"enc_pos\":%d,\"sol1\":%d,\"sol2\":%d,\"sol3\":%d,\"sol4\":%d}", str_temp, str_humi, str_wind, volt, current, encPosition, solar1ADSample,solar2ADSample,solar3ADSample,solar4ADSample );
	   //sprintf(measData,"{\"topic\":\"meas\",\"temp\":%.2f,\"humi\":%.2f,\"wind\":%.2f,\"volt\":%.2f,\"curr\":%.2f,\"enc_pos\":%d,\"sol1\":%d,\"sol2\":%d,\"sol3\":%d,\"sol4\":%d}", temp, humi, wind, volt, current, encPosition, solar1ADSample,solar2ADSample,solar3ADSample,solar4ADSample );
	   PRINTLN(measData);
	   sarModem->write(measData);
	   memset(measData, 0, 512);

	   //////////////////// Status Check  ////////////////////////////////
	  //TODO: check status change in objects and publish via sarModem as well
	   //check status windill 1 & 2

	   solarOn = solarfarm.isLightOn(); //status house 1-4 and solarpark, by checking associated Solarfarm as a whole
	   if (solarOn > 0) { //can also check return value which solarpanel is active
		   house1.lightOn();house2.lightOn();house3.lightOn();house4.lightOn();solarpark.lightOn();
	   }else{
		   house1.lightOff();house2.lightOff();house3.lightOff();house4.lightOff();solarpark.lightOff();
	   };
	   windmill1On = windmill1.isOn();
	   windmill2On = windmill1.isOn();
	   if ((solarOn > 0) && ((windmill1On == true) || (windmill2On == true))){
		   if (hydroStation.isOn()) hydroStation.turnOff();
	   }else{
		   if (hydroStation.isOff()) hydroStation.turnOn();
	   }
	   //////////////////// Status  Report ////////////////////////////////
	   bool newObjStatus = false;;
	   //windmills
	   bool wm1 = windmill1.statusHasChanged();
	   bool wm2 = windmill2.statusHasChanged();
	   bool wm3 = windmill3.statusHasChanged();
	   //solarfarm
	   bool solar = solarfarm.statusHasChanged();
	   //status charging station
	   bool charge = chargingStation.statusHasChanged();
	   //status substation 1-3
	   bool sub1 = substation1.statusHasChanged();
	   bool sub2 = substation2.statusHasChanged();
	   bool sub3 = substation3.statusHasChanged();
	   //hydro station
	   bool hydro = hydroStation.statusHasChanged();
	   //DPRINT(wm1);DPRINT(wm2);DPRINT(wm3);DPRINT(solar);DPRINT(charge);DPRINT(sub1);DPRINT(sub2);DPRINTLN(sub3);
	   newObjStatus = (wm1 || wm2 || wm3 || solar || charge || sub1 || sub2 || sub3 || hydro); //DPRINTLN(newObjStatus);
       if (newObjStatus == true){
    	   wm1 = windmill1.isOn();wm2 = windmill2.isOn();wm3 = windmill3.isOn();
    	   solar = solarfarm.isLightOn();
    	   charge = chargingStation.isChargingOn();
    	   sub1 = substation1.isActive();sub2 = substation2.isActive();sub3 = substation3.isActive();
    	   hydro = hydroStation.isOn();
    	   sprintf(stateData,"{\"topic\":\"status\",\"wm1\":%d,\"wm2\":%d,\"wm3\":%d,\"solar\":%d,\"charge\":%d,\"sub1\":%d,\"sub2\":%d,\"sub3\":%d,\"hydro\":%d}", wm1, wm2, wm3, solar, charge,sub1, sub2,sub3,hydro );
    	   PRINTLN(stateData);
    	   sarModem->write(stateData);
    	   newObjStatus = false;
       }
	   previousMillis = millis();
    }

}
