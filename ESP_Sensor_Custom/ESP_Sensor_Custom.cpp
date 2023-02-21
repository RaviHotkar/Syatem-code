#include "SPIFFS.h"
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <Esp.h>

#include "EEPROM.h"
#include "DFRobot_SHT20.h"

#include "ESP_AWS_Custom.h"
#include "ESP_EEPROM_Custom.h"
#include "ESP_Global_Custom.h"
#include "ESP_Mega_Custom.h"
#include "ESP_BOT_Custom.h"
#include "ESP_Sensor_Custom.h"

DFRobot_SHT20 sht20;
//DFRobot_SHT20 Env_Sen;

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


void ESP_Sensor_Custom :: Sensor_Setup()                                                          // Assigned the Sensors setup functione
{
    sht20.initSHT20();                         // Init SHT20 Sensor
//	Env_Sen.initSHT20();                         // Init SHT20 Sensor
    delay(100);
    sht20.checkSHT20();                        // Check SHT20 Sensor
//	Env_Sen.checkSHT20();                        // Check SHT20 Sensor
}

void ESP_Sensor_Custom :: Sensor_Loop()
{
	if(1)//(System_On_Off_sequence == true)
      {
          
		  //Serial.println("In Sensor_loop......BEGIN..");
		  Environment_Sensor_Handling();
		  //Soil_Sensor_Handling();
		  //Light_Sensor_Handling();
		  
		  //Serial.println("In Sensor_loop.....END.");
      }
	
}

void ESP_Sensor_Custom :: Soil_Sensor_Handling()
{   


 
}

void ESP_Sensor_Custom :: Environment_Sensor_Handling()
{
					  
				  //Read Environment Temperature and Humidity.....
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
				    	Serial.println("Conditins met to START the exhaust due to Humidity HIGH..");
				      }
				    else if ((Env_Humidity < HUMIDITY_HIGHER_OFF)&&(exhast_status_humidity_high == true))
				      {  
				       exhast_status_humidity_high = false;
				       Serial.println("Conditins met to STOP the exhaust due to Humidity HIGH...");
				      } 


					else if (Env_Humidity < HUMIDITY_LOWER_ON)/*&&(exhast_status_humidity_high == false))*/
					  {
						exhast_status_humidity_low = true;
						Serial.println("Conditins met to START the exhaust due to Humidity LOW ...");
					  }
					else if ((Env_Humidity > HUMIDITY_LOWER_OFF)&&(exhast_status_humidity_low == true))
					  {  
					   //digitalWrite(RELAY1, LOW);  // Turn OFF exhaust
					   exhast_status_humidity_low = false;
					   Serial.println("Conditins met to STOP the exhaust due to Humidity LOW...");
					  } 

				//-------------------Humidity handnling------End------------
				
				
				//-------------------Temperature handnling----Begin---------
						if (Env_Temperature > TEMPERATURE_ON)
						 {
						//digitalWrite(RELAY1, HIGH);  // Turn ON exhaust
						exhast_status_temp = true;
							Serial.println("Conditins met to START the exhaust due to Temperature...");
						  }
						else if ((Env_Temperature < TEMPERATURE_OFF)&&(exhast_status_temp == true))
						{  
						   //digitalWrite(RELAY1, LOW);  // Turn OFF exhaust
						   exhast_status_temp = false;
						   Serial.println("Conditins met to STOP the exhaust due to Temperature...");
						} 

				//-------------------Temperature handnling--------End-----            
							
					Serial.print(" Temperature: ");
					Serial.print(Env_Temperature, 1);
					Serial.print("C");
					Serial.print("\t Humidity: ");
					Serial.print(Env_Humidity, 1);
					Serial.println("%");
					
				
                    Serial.println("       ");
				   // Serial3.clear();
					delay(300);
//					Serial3.print("*Sensor_Environment_Temperature: ");Serial3.print(Env_Temperature);Serial3.print("C ");Serial3.print("# ");
					delay(1000);
//					Serial3.print("*Sensor_Environment_Moisture: ");Serial3.print(Env_Humidity);Serial3.print("% ");Serial3.print("# ");
	
}

void ESP_Sensor_Custom :: Light_Sensor_Handling()
{
	
	
	
}
