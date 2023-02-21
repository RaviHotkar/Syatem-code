#include "SPIFFS.h"
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <Esp.h>

#include "EEPROM.h"

#include "ESP_AWS_Custom.h"
#include "ESP_EEPROM_Custom.h"
#include "ESP_Global_Custom.h"
#include "ESP_Mega_Custom.h"
#include "ESP_BOT_Custom.h"
#include "ESP_Sensor_Custom.h"

extern time_t          currentEpoc; 
extern ESP_AWS_Custom  AWS_handle;

void ESP_EEPROM_Custom :: EEPROM_write()
	{
        int temp_hour, temp_minute;
        if(1)//(WiFi.status() == WL_CONNECTED)                                                     // Wifi Status is connected to wifi
			{
//    				currentEpoc   = timeClient.getcurrentEpoc() ;  
				currentEpoc   = AWS_handle.timeClient_AWS();
				EEPROM.get(HOUR_ADDRESS, temp_hour);
				EEPROM.get(MINUTE_ADDRESS, temp_minute);

				if(((temp_hour==Hour)&&(temp_minute!=Minute)) || (temp_hour!=Hour))                //Save to EEPROM when either minute is changed or Hour.
					{
						EEPROM.put(CURRENTEPOC_ADDRESS, currentEpoc);                              // int - so 4 bytes (address is '0')
						EEPROM.put(MINUTE_ADDRESS, Minute);                                        // int - so 4 bytes (address is '4')
						EEPROM.put(HOUR_ADDRESS, Hour);                                            // int - so 4 bytes (address is '8')
						boolean write_status = EEPROM.commit();
						Serial.println((write_status) ? "Commit OK" : "Commit failed");
					}
			}
	}

void ESP_EEPROM_Custom :: Eeprom_setup()
	{
		EEPROM.begin(BUFFER_SIZE);                                                                 // The begin() call is required to initialise the EEPROM library
		EEPROM.get(CURRENTEPOC_ADDRESS, currentEpoc);                                              // int - so 4 bytes (next address is '4')
		EEPROM.get(EEPROM_STATUS_ADDRESS,  EEPROM_status);
        
		if(EEPROM_status == 0x01)
			{    
		
			    EEPROM.get(DAY_IN_WEEK_ADDRESS, Day_in_week);
			    EEPROM.get(PRE_DAY_IN_WEEK_ADDRESS, Pre_Day_in_week);
			    
			    char token5[30];
			    char token6[30];
			    
			    short int temp_ssid_len;
			    EEPROM.get(TEMP_SSID_LEN_ADDRESS, temp_ssid_len);
			    temp_ssid_len = (temp_ssid_len-1) & 0xFF;
			    Serial.print("temp_ssid_len 7 :  ");Serial.println(temp_ssid_len);
			    for (int index = BEGENING_SSID_ADDRESS,i=0; i < temp_ssid_len; index++,i++)
					{
					    EEPROM.get(index, token5[i]);                                               // Update all the data read in the buffer to EEPROM buffer.
					} 
			   
			    short int temp_pwd_len; 
			    EEPROM.get(TEMP_PWD_LEN_ADDRESS, temp_pwd_len); 
			    temp_pwd_len = (temp_pwd_len-1) & 0xFF;
			    Serial.print("temp_pwd_len 8 :  ");Serial.println(temp_pwd_len);       
			    for (int index=BEGENING_PWD_ADDRESS,j=0; j<temp_pwd_len; index++,j++)
					{
					    EEPROM.get(index, token6[j]);                                               // Update all the data read in the buffer to EEPROM buffer.
					} 
			    for (int i=0; i<30; i++)
			        {
				        ssid[i] ='\0';
			        }
			    for (int i=0;i<temp_ssid_len;i++)
				    {
				        ssid[i]= token5[i];
				    }
			    for (int i=0; i<30; i++)
			        {
				        password[i] ='\0';
			        }
			    for (int i=0;i<temp_pwd_len;i++)
				    {
				        password[i] = token6[i];
				    }
			    Serial.print("ssid 9:  ");Serial.println(ssid);
			    Serial.print("password 10:  ");Serial.println(password);
			    ssid_str = String(ssid); 
			    password_str = String(password); 
			    Serial.print("ssid_str 11:  ");Serial.println(ssid_str);
			    Serial.print("password_str 12:  ");Serial.println(password_str);
			    Serial.print("ssid_str_length 13:  ");Serial.println(ssid_str.length());
			    Serial.print("password_str_length 14:  ");Serial.println(password_str.length());
			    
			    EEPROM.get(ON_OFF_SYSTEM_SEQ_ADDRESS, On_Off_System_sequence);
			    
			    
			    for (int Pump_temp =0; Pump_temp < NUMBER_OF_PUMPS;Pump_temp++)
				    {
				        unsigned int Address_temp = BASE_ADDRESS_PUMP + OFFSET*Pump_temp;
			            EEPROM.get(Address_temp, Hour1[Pump_temp]);                          // int - so 4 bytes (address is '8')
				        Address_temp++;
			            EEPROM.get(Address_temp, Min1[Pump_temp]);
				        Address_temp++;
			            EEPROM.get(Address_temp, Hour2[Pump_temp]);                          // int - so 4 bytes (address is '8')
				        Address_temp++;
			            EEPROM.get(Address_temp, Min2[Pump_temp]);
				        Address_temp++;
			            EEPROM.get(Address_temp, Hour3[Pump_temp]);                          // int - so 4 bytes (address is '8')
				        Address_temp++;
			            EEPROM.get(Address_temp, Min3[Pump_temp]); 
			            Address_temp++;
				        EEPROM.get(Address_temp, Hour4[Pump_temp]);                          // int - so 4 bytes (address is '8')
			            Address_temp++;
				        EEPROM.get(Address_temp, Min4[Pump_temp]);
			            Address_temp++;
				        EEPROM.get(Address_temp, Day0[Pump_temp]);
			            Address_temp++;
				        EEPROM.get(Address_temp, Day1[Pump_temp]);
			            Address_temp++;
				        EEPROM.get(Address_temp, Day2[Pump_temp]);
			            Address_temp++;
				        EEPROM.get(Address_temp, Day3[Pump_temp]);
			            Address_temp++;
				        EEPROM.get(Address_temp, Day4[Pump_temp]);
			            Address_temp++;
				        EEPROM.get(Address_temp, Day5[Pump_temp]);
			            Address_temp++;
				        EEPROM.get(Address_temp, Day6[Pump_temp]);
                        Address_temp++;
				        EEPROM.get(Address_temp, Week_Freq[Pump_temp]);
				        Address_temp++;
				        EEPROM.get(Address_temp, On_Off_sequence[Pump_temp]);	
				        Address_temp++;
				        EEPROM.get(Address_temp, Week_Freq_Dyanamic_Count[Pump_temp]);	
				    }
			   
			   
			}
		else
			{
				EEPROM_status = 0x01;
				EEPROM.put(EEPROM_STATUS_ADDRESS,  EEPROM_status);
				
				EEPROM.put(DAY_IN_WEEK_ADDRESS, Day_in_week);
				EEPROM.put(PRE_DAY_IN_WEEK_ADDRESS, Pre_Day_in_week);

				short int temp_ssid_len = ssid_str.length();
				EEPROM.put(TEMP_SSID_LEN_ADDRESS,((temp_ssid_len+1)&0xFF));
				
				for (int index = BEGENING_SSID_ADDRESS,i=0; i < temp_ssid_len; index++,i++)
					{
					    EEPROM.put(index, ssid_str.charAt(i));                                      // Update all the data read in the buffer to EEPROM buffer.
					}
         
				EEPROM.put(BEGENING_SSID_ADDRESS + temp_ssid_len, '\0');
				Serial.print("temp_ssid_len 15:  ");Serial.println(temp_ssid_len); 
				
				short int temp_password_len = password_str.length();
				EEPROM.put(TEMP_PWD_LEN_ADDRESS, ((temp_password_len+1)&0xFF));    
				
				for (int index = BEGENING_PWD_ADDRESS,j=0; j<temp_password_len; index++,j++)
					{
					    EEPROM.put(index, password_str.charAt(j));                                  // Update all the data read in the buffer to EEPROM buffer.
					}
				EEPROM.put(BEGENING_PWD_ADDRESS + temp_password_len, '\0'); 
				Serial.print("temp_password_len 16:  ");Serial.println(temp_password_len); 
				
				Serial.print("ssid_write 17 :  ");Serial.println(ssid);
				Serial.print("password_write 18:  ");Serial.println(password);
			   
				Serial.print("ssid_str_write 19:  ");Serial.println(ssid_str);
				Serial.print("password_str_write 20:  ");Serial.println(password_str); 
				Serial.print("ssid_str_length 21:  ");Serial.println(ssid_str.length());
				Serial.print("password_str_length 22:  ");Serial.println(password_str.length()); 

				EEPROM.put(ON_OFF_SYSTEM_SEQ_ADDRESS, On_Off_System_sequence);
				
				
				//--------- All pumps data-----
				for (int Pump_temp =0; Pump_temp < NUMBER_OF_PUMPS;Pump_temp++)
				    {
				        unsigned int Address_temp = BASE_ADDRESS_PUMP + OFFSET*Pump_temp;
			            EEPROM.put(Address_temp, Hour1[Pump_temp]);                          // int - so 4 bytes (address is '8')
				        Address_temp++;
			            EEPROM.put(Address_temp, Min1[Pump_temp]);
				        Address_temp++;
			            EEPROM.put(Address_temp, Hour2[Pump_temp]);                          // int - so 4 bytes (address is '8')
				        Address_temp++;
			            EEPROM.put(Address_temp, Min2[Pump_temp]);
				        Address_temp++;
			            EEPROM.put(Address_temp, Hour3[Pump_temp]);                          // int - so 4 bytes (address is '8')
				        Address_temp++;
			            EEPROM.put(Address_temp, Min3[Pump_temp]); 
			            Address_temp++;
				        EEPROM.put(Address_temp, Hour4[Pump_temp]);                          // int - so 4 bytes (address is '8')
			            Address_temp++;
				        EEPROM.put(Address_temp, Min4[Pump_temp]);
			            Address_temp++;
				        EEPROM.put(Address_temp, Day0[Pump_temp]);
			            Address_temp++;
				        EEPROM.put(Address_temp, Day1[Pump_temp]);
			            Address_temp++;
				        EEPROM.put(Address_temp, Day2[Pump_temp]);
			            Address_temp++;
				        EEPROM.put(Address_temp, Day3[Pump_temp]);
			            Address_temp++;
				        EEPROM.put(Address_temp, Day4[Pump_temp]);
			            Address_temp++;
				        EEPROM.put(Address_temp, Day5[Pump_temp]);
			            Address_temp++;
				        EEPROM.put(Address_temp, Day6[Pump_temp]);
                        Address_temp++;
				        EEPROM.put(Address_temp, Week_Freq[Pump_temp]);
				        Address_temp++;
				        EEPROM.put(Address_temp, On_Off_sequence[Pump_temp]);	
				        Address_temp++;
				        EEPROM.put(Address_temp, Week_Freq_Dyanamic_Count[Pump_temp]);	
				    }
						
				boolean write_status = EEPROM.commit();
			}
	}

