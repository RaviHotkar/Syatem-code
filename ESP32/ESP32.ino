#include <Arduino.h>
#include "SPIFFS.h"
#include "esp_system.h"
#include "EEPROM.h"
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <NTPClient.h>

#define ESP_Global_Custom_h
#include <ESP_Global_Custom.h>
#undef ESP_Global_Custom_h

#include "ESP_AWS_Custom.h"
#include "ESP_EEPROM_Custom.h"
#include "ESP_Mega_Custom.h"
#include "ESP_BOT_Custom.h"
#include "ESP_Sensor_Custom.h"

ESP_AWS_Custom     AWS_handle;
ESP_EEPROM_Custom  eeprom_handle;
ESP_Global_Custom  ESP_global_handle;  
ESP_Mega_Custom    Mega_handle;
ESP_BOT_Custom     BOT_handle;
ESP_Sensor_Custom  Sensor_handle;

const int wdtTimeout = 8000;                                              // time in ms to trigger the watchdog
hw_timer_t *timer = NULL;

///////Software testing itself.......START//////////
#ifdef BOT_TESTING_ENABLE
    int temp_loop =0;
#endif
///////Software testing itself.......END//////////



void setup() 
	{
        timer = timerBegin(0, 80, true);                                      // timer 0, div 80
        timerAlarmWrite(timer, wdtTimeout * 1000, false);                     // set time in us
        timerAlarmEnable(timer);                                              // enable interrupt
    		Serial.begin(9600);                                                   // baudrate set
        ESP_global_handle.Initialize_Global_Variables();
    		eeprom_handle.Eeprom_setup();
       
        #ifndef BOT_ENABLE
          		 AWS_handle.setup_wifi();                                              // Wifi function added
          		 AWS_handle.getNTP();                                                  // NTP server function added
        		   AWS_handle.certificates();
               Serial.println("System SW ; BOT is disabled...");
        #endif
        
        timerWrite(timer, 0);                                                 // reset timer (feed watchdog)	
        
        #ifdef BOT_ENABLE
           BOT_handle.Bot_steps_setup();
        #endif
        
    		Serial.println("ESP_32_SETUP_END............................"); 
       /////////////////Testing start////////////////
       #ifdef BOT_TESTING_ENABLE
              Hour = 14;
              Minute = 0;
       #endif
        /////////////////Testing end////////////////
 }

void loop() 
	{
         // Serial.println("~~~~~~~~~~~~~ESP_32_LOOP_START............................~~~~~~~~~~~~");    
          timerWrite(timer, 0);                                    // reset timer (feed watchdog) 
          
          #ifndef BOT_ENABLE
                	AWS_handle.loop_AWS_NTP();
              		AWS_handle.reconnect_WiFi_AWS();                                      // reconnect_WiFi_AWS function added
                  timerWrite(timer, 0); 
                  timerWrite(timer, 0);  
                  AWS_handle.loop_AWS_pub();
                  AWS_handle.read_data_AWS();             
              		timerWrite(timer, 0);                                                 // reset timer (feed watchdog)
              		Mega_handle.read_mega_status_upadate_server();                        // read_mega_status_upadate_server function added
          #endif
    
      		Mega_handle.read_time_activate_pump();                                // read_time_activate_pump function added
      		Mega_handle.Flag_clear();                                             // Flag_clear function added

          #ifndef BOT_ENABLE
              Mega_handle.test_function(); 
          #endif
      	  timerWrite(timer, 0);                                                 // reset timer (feed watchdog)
      	  
      	  #ifdef BOT_ENABLE
              Serial.print("Time is "); Serial.print( Hour );  Serial.print(":"); Serial.println(Minute);
              Serial.print("Day_in_week = ");Serial.println(Day_in_week);
      	      BOT_handle.Bot_steps_loop(); 
          #endif
    
    //Sensor_handle.Sensor_Loop(); 
  //  timerWrite(timer, 0);                                                 // reset timer (feed watchdog)  

///////Software testing itself.......START//////////
#ifdef BOT_TESTING_ENABLE
    temp_loop++;
    Serial.print("temp_loop............................"); Serial.println(temp_loop); 
    if (temp_loop >= 5)
        {
          temp_loop = 0;
          Minute++;
          if(Minute>=6)
          {
            Minute=0;
            Hour++;
          }
          if(Hour>=18)
          {
            Hour = 14;
            Day_in_week++;
            
            if (Day_in_week == 7)
              Day_in_week = 0;
          }
        }
#endif
///////Software testing itself.......END//////////

//Serial.println("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ESP_32_LOOP_END..........................~~~~~~~~~~~~~~~~~~~~~~~~~..");  
       
	}
