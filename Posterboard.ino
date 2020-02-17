//#include <Controllino.h>  /* Usage of CONTROLLINO library allows you to use CONTROLLINO_xx aliases in your sketch. */
//The setup function is called once at startup of the sketch
#include "Modbus.h"

#define MasterModbusAdd  0
#define SlaveModbusAdd  2
#define RS485Serial     3
Modbus modbus(MasterModbusAdd, RS485Serial,0);
uint16_t response[16];
modbus_t request;
//uint8_t myState; // machine state
uint8_t currentQuery; // pointer to message query
unsigned long u32wait;
uint8_t u8state;

float temperature;
float humidity;
void setup()
{
	  Serial.begin( 9600 ); // baud-rate at 19200
	  modbus.begin(9600);
	  modbus.setTimeOut( 2000 ); // if there is no answer in 2000 ms, roll over
	  u32wait = millis() + 5000;
	  u8state = 0;
}

// The loop function is called in an endless loop
void loop()
{
	  switch( u8state ) {
	  case 0:
	    if (millis() > u32wait) u8state++; // wait state
	    break;
	  case 1:
	    request.u8id = 2; // slave address
	    request.u8fct = 4; // function code (this one is registers read)
	    request.u16RegAdd = 1; // start address in slave
	    request.u16CoilsNo = 2; // number of elements (coils or registers) to read
	    request.au16reg = response; // pointer to a memory array in the Arduino

	    modbus.query( request ); // send query (only once)
	    u8state++;
	    break;
	  case 2:
		 int bufferSize = modbus.poll();
		 if (modbus.getState() == COM_IDLE) {
            if((bufferSize > 0) && (bufferSize < 16)){
     	      for (int i = 0; i < 2; i++){

     	      Serial.println(response[i], DEC);
     	      };
            }else{
            	Serial.print("Check Erorr from buffersize: ");
            	Serial.println(bufferSize);


            };
		 };
		 u8state = 0;
		 u32wait = millis() + 1000;
         break;

	  }
}
