#include <Arduino.h>
#include <EEPROM.h>
#include "StopWatch.h"

#include <Wire.h>
#include "DFRobot_SHT20.h"

#include "Mega_Eeprom_Custom.h"
#include "Mega_ESP_Custom.h"
#include "Mega_Global_Custom.h"
#include "Mega_Pump_Custom.h"
#include "Mega_Sensor_Custom.h"

DFRobot_SHT20 sht20;

float Env_Humidity; 
float Env_Temperature; 


#define COUNT  10

#define HUMIDITY_HIGHER_OFF  78
#define HUMIDITY_HIGHER_ON   80

#define HUMIDITY_LOWER_OFF   52
#define HUMIDITY_LOWER_ON    50

#define TEMPERATURE_OFF      28
#define TEMPERATURE_ON       30

#define TEMPERATURE_OFFSET   2.5
#define HUMIDITY_OFFSET      10


void Mega_Sensor_Custom :: Sensor_Setup()                                                          // Assigned the Sensors setup functione
{
    sht20.initSHT20();                         // Init SHT20 Sensor
    delay(100);
    sht20.checkSHT20();                        // Check SHT20 Sensor
	Sensor_read_timer.start();
}
	
	

void Mega_Sensor_Custom :: Sensor_Loop()
{
	
	Sensor_read_timer_handle();                                                               // Pump activation flag is clearing after PUMP_ACT_TIME
	
	if(1)//(System_On_Off_sequence == true)
      {
          
		  if (Sensor_read_flag_set == true)
		  {
			   Environment_Sensor_Handling();
			   Sensor_read_flag_set = false; 
		  }
		  
		  //Soil_Sensor_Handling();
		  //Light_Sensor_Handling();

      }
	
}

void Mega_Sensor_Custom :: Soil_Sensor_Handling()
{   


 
}
void Mega_Sensor_Custom :: Environment_Sensor_Handling()
{

				  for (char loop = 0;loop<COUNT;loop++)
					{
						 Env_Humidity+= sht20.readHumidity();         // Read Humidity
						 Env_Temperature+= sht20.readTemperature();      // Read Temperature
						 delay(30);
						
					}
					Env_Humidity     = Env_Humidity / COUNT;
					Env_Temperature  = Env_Temperature / COUNT;
					
					Env_Temperature -= TEMPERATURE_OFFSET;
					Env_Humidity    -= HUMIDITY_OFFSET;
					 
				//-------------------Humidity handnling------Begin-------
				    if (Env_Humidity > HUMIDITY_HIGHER_ON)/*&&(exhast_status_humidity_high == false))*/
				      {
				    	exhast_status_humidity_high = true;
//				    	Serial.println("Conditins met to START the exhaust due to Humidity HIGH..");
				      }
				    else if ((Env_Humidity < HUMIDITY_HIGHER_OFF)&&(exhast_status_humidity_high == true))
				      {  
				       exhast_status_humidity_high = false;
//				       Serial.println("Conditins met to STOP the exhaust due to Humidity HIGH...");
				      } 


					else if (Env_Humidity < HUMIDITY_LOWER_ON)/*&&(exhast_status_humidity_high == false))*/
					  {
						exhast_status_humidity_low = true;
//						Serial.println("Conditins met to START the exhaust due to Humidity LOW ...");
					  }
					else if ((Env_Humidity > HUMIDITY_LOWER_OFF)&&(exhast_status_humidity_low == true))
					  {  
					   exhast_status_humidity_low = false;
//					   Serial.println("Conditins met to STOP the exhaust due to Humidity LOW...");
					  } 
				//-------------------Humidity handnling------End------------
							
				//-------------------Temperature handnling----Begin---------
						if (Env_Temperature > TEMPERATURE_ON)
						 {
						exhast_status_temp = true;
//							Serial.println("Conditins met to START the exhaust due to Temperature...");
						  }
						else if ((Env_Temperature < TEMPERATURE_OFF)&&(exhast_status_temp == true))
						{  
						   exhast_status_temp = false;
//						   Serial.println("Conditins met to STOP the exhaust due to Temperature...");
						} 
				//-------------------Temperature handnling--------End-----            
					Serial3.print("*Sensor_Env_Temp: ");Serial3.print(Env_Temperature);Serial3.print("C ");Serial3.println("# ");
					delay(1000);
					Serial3.print("*Sensor_Env_Moist: ");Serial3.print(Env_Humidity);Serial3.print("% ");Serial3.println("# ");
	
}

void Mega_Sensor_Custom :: Light_Sensor_Handling()
{
	
	
	
}

void Mega_Sensor_Custom :: Sensor_read_timer_handle()                                                               // Pump activation flag is clearing after PUMP_ACT_TIME
	{
        int flag_time_temp=int(Sensor_read_timer.value()/CONV_MS_SEC);
        if(flag_time_temp >= SENSOR_ACT_TIME) 
    			{       
    				Sensor_read_timer.stop();
    				Sensor_read_timer.reset();
					Sensor_read_flag_set = true;                                                            // Pump activation flag is clearing after PUMP_ACT_TIME
					Sensor_read_timer.start();
    			}

	}

