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

#define MAX_LENGTH_OF_STRING  150             //Once the string content reaches this length then clear it..
#define STRING_LENGTH_OVERFLOW_VALUE 60000   // This is needed to avoid the pattern searching function to detect failure after reaching 65535.

extern ESP_AWS_Custom  AWS_handle;

void ESP_Mega_Custom :: test_function()                                                            // Afer PUMP turn on for timer will run upto one minute , will flag clear after assigned time 
	{ 
		if(test_time == false)
			{
				test_time = true;
				Serial.println("{\n\"message\": \n}"); 
				test_timer.reset();
				test_timer.start();
			}
	}

void ESP_Mega_Custom :: read_time_activate_pump()                                                  // Read Time, check if it matches time slots then give message to mega to activate pumps
    {
	    if(On_Off_System_sequence == true)
            {
		        for (unsigned char Pump_Temp = 0; Pump_Temp<NUMBER_OF_PUMPS;Pump_Temp++)
			    {
			    	if(On_Off_sequence[Pump_Temp] == true)
			    	{
			    		char temp_day_match = false;
			    						
			    		if ((Day_in_week == Day0[Pump_Temp])||(Day_in_week == Day1[Pump_Temp])||(Day_in_week == Day2[Pump_Temp])||(Day_in_week == Day3[Pump_Temp])||(Day_in_week == Day4[Pump_Temp])||(Day_in_week == Day5[Pump_Temp])||(Day_in_week == Day6[Pump_Temp]))
			    			{ 
			    			   
			    			    // temp_day_match = true;
	//		    			    Serial.print("Day match has been detected for Pump = ");Serial.println(Pump_Temp+1);
			    			   
			    			    if (Week_Freq[Pump_Temp]!=1)
			    			        {
			    			            if (Day_in_week!=Pre_Day_in_week)
			    			        		{
			    			        			// Week_Freq1_Dyanamic_Count++;
			    			        			Week_Freq_Dyanamic_Count[Pump_Temp]+=1;
			    			        			unsigned char Week_Freq_Dyanamic_Count_Address = BASE_ADDRESS_PUMP + OFFSET*Pump_Temp+DYNAMIC_COUNT_OFFSET;
			    			        			//  EEPROM.put(WEEK_FREQ_COUNT1_ADDRESS, Week_Freq1_Dyanamic_Count);											
			    			        			EEPROM.put(Week_Freq_Dyanamic_Count_Address, Week_Freq_Dyanamic_Count[Pump_Temp]);
			    			        			boolean write_status = EEPROM.commit();
			    			        	  
			    			        			if(Week_Freq_Dyanamic_Count[Pump_Temp] == Week_Freq[Pump_Temp]) 
			    			        			    {
			    			        				    Week_Freq_Dyanamic_Count[Pump_Temp] = 0;
			    			        				    temp_day_match = true;
		//	    			        				    Serial.print("WEEK match has been detected for Pump  :     "); Serial.print(Pump_Temp+1);Serial.print("   Week =  ");Serial.println(Week_Freq[Pump_Temp]);
			    			        				  
			    			        			    }
			    			        		}
			    			        }
			    			    else 
			    			        {
			    					    temp_day_match = true;
			    			        }
			    			}
			    	
			    		if((((Hour == Hour1[Pump_Temp])&&(Minute == Min1[Pump_Temp])) || ((Hour == Hour2[Pump_Temp])&&(Minute == Min2[Pump_Temp])) || ((Hour == Hour3[Pump_Temp])&&(Minute == Min3[Pump_Temp])) || ((Hour == Hour4[Pump_Temp])&&(Minute == Min4[Pump_Temp])))&&(time_slot_match_pump_act == false)&&(temp_day_match==true)) 
			    			{

								Serial.print("{\n\"message\": ");Serial.print(Pump_Temp*2+1);Serial.println(" \n}");
			    				time_slot_match_pump_act = true;  
			    				flag_timer.reset();
			    				flag_timer.start();                                                        // Start timer for 70 sec(>one minute), Not to activate multiple time
			    				
			    //				Serial.print("Time match has been detected for PUMP   ");Serial.println(Pump_Temp+1);
        #ifdef BOT_ENABLE			    				
			    				
			    				if(Pump_Temp == BOT_MOVEMENT_SCHEDULE) //Is it the first Time slot ; then move the BOT..
			    				{
			    					msgReceived_BOT_Move = true;
			    					Serial.println("******~~~~~~~~~~~~BOT time match detected....");
			    				}
			    				
			    				if(Pump_Temp == BOT_SPRAY_CHARGE_SCHEDULE)
			    				{
			    					msgReceived_BOT_Spray_charge = true;
			    		            msgReceived_BOT_Spray_charge2 = false;
			    					Serial.println("******~~~~~~~~~~~~Time to Charge the SPRAY....");
			    				}
		#endif
		
			    			}
		#ifdef BOT_ENABLE
			    			
			    		if((temp_day_match == true)&&(Pump_Temp == BOT_SPRAY_SCHEDULE))
			    		{
			    			
			    			msgReceived_BOT_Spray = true;
			    			Serial.println("******~~~~~~~~~~~~Day of spraying....");
			    		}	
		#endif
			    	}
			    }
		        Pre_Day_in_week = Day_in_week;
            }
    }

void ESP_Mega_Custom :: read_mega_status_upadate_server()
	{
		unsigned int Mega_Msg_Begin = 0 , Mega_Msg_End = 0;
		unsigned int Received_String_Length = 0;
		while(Serial.available())                                                                  // Is data available from mega
			{
				char Rdata;
				Rdata=Serial.read();
				Serial.print(Rdata);
				received_mega_data = received_mega_data + Rdata;                                   // Read data and stored in buffer
      		}
		//if(received_mega_data.indexOf("#")>= 0)                                            // IS the Received data ending with #
		Mega_Msg_Begin = received_mega_data.indexOf("*");
		Mega_Msg_End = received_mega_data.indexOf("#");
		
		    if((Mega_Msg_Begin>=0)&&(Mega_Msg_End>0)&&(Mega_Msg_End>Mega_Msg_Begin)&&(Mega_Msg_End<STRING_LENGTH_OVERFLOW_VALUE)&&(Mega_Msg_Begin<STRING_LENGTH_OVERFLOW_VALUE))   // IS the Received data beginning with "{" ending with "}"
		    { 
				msgReceived_mega = true;                                                   // Set up Flag to indicate complete reception of data 
				Serial.print("MSG received from mega..........1 == ");Serial.println(received_mega_data);
			}
			

		if (msgReceived_mega == true)
			{
				msgReceived_mega = false;
				int len = received_mega_data.length();                                             // Check the length of the received data 
				char msg[len+1];
				received_mega_data.toCharArray(msg, len+1);                                        // Store the received data in temp string
				char send_data[BUFFER_SIZE];

				for (int i=0;i<BUFFER_SIZE;i++)
					send_data[i]=0;//'\0';
				send_data[0] = '{'; send_data[1] = '"'; send_data[2] = 't'; send_data[3] = 'i'; send_data[4] = 'm'; 
				send_data[5] = 'e'; send_data[6] = '"'; send_data[7] = ':'; send_data[8] = '"';
            
				for(int i=9,j=0; j<10; i++,j++)
					send_data[i] = Date[j];
				send_data[19] = ' ';
				
				for(int i=20,j=0; j<8; i++,j++)
					send_data[i] = Time[j];
            
				send_data[28] = '"'; send_data[29] = ','; send_data[30] = '\n'; send_data[31] = '"'; 
				send_data[32] = 'p'; send_data[33] = 'u'; send_data[34] = 'm'; send_data[35] = 'p';
				send_data[36] = '"'; send_data[37] = ':'; send_data[38] = '"' ;    
				
				for(int i=39,j=0; j<(len+1); i++,j++)
					send_data[i] = msg[j];
				send_data[39+len] = '"'; send_data[40+len] = '}'; 
            
				if(received_mega_data.indexOf("*Pump")>= 0)                   					   // IS the Received data starting with *Pump"
                  AWS_handle.loop_AWS_publish(OUTTOPIC, send_data); 
				
				if(received_mega_data.indexOf("*Duration")>= 0)                                    // IS the Received data starting with *Duration"
				  AWS_handle.loop_AWS_publish(SETDONE_TOPIC, send_data); 

//				if(received_mega_data.indexOf("*Sensor_On_Off")>= 0)                               // IS the Received data starting with *Sensor_On_Off"
//				  AWS_handle.loop_AWS_publish(SETDONE_TOPIC, send_data); 
				  
				if(received_mega_data.indexOf("*Sensor")>= 0)                               // IS the Received data starting with *Sensor_On_Off"
				  AWS_handle.loop_AWS_publish(SENSOR_RESP_TOPIC, send_data); 

				if(received_mega_data.indexOf("*System_On_Off")>= 0)                               // IS the Received data starting with *System_On_Off"
				  AWS_handle.loop_AWS_publish(SETDONE_TOPIC, send_data); 
				  
				if(received_mega_data.indexOf("*Enquiry")>= 0)                                     // IS the Received data starting with *Enquiry"
				  AWS_handle.loop_AWS_publish(ENQ_RESP_TOPIC, send_data);  
								
				received_mega_data = "";                                                           // Clearing the data buffer
			}
		Received_String_Length = received_mega_data.length();	
		if (Received_String_Length > MAX_LENGTH_OF_STRING)                              // is the string length more than 100? that means it is about to overflow and it cannot take new messages...
		   received_mega_data = "";  	
	}

void ESP_Mega_Custom :: Flag_clear()                                                               // Pump activation flag is clearing after PUMP_ACT_TIME
	{
        int flag_time_temp=int(flag_timer.value()/CONV_MS_SEC);
        if((flag_time_temp >= PUMP_ACT_TIME) && (time_slot_match_pump_act == true))
    			{       
    				time_slot_match_pump_act = false; 
    				flag_timer.stop();
    				flag_timer.reset();
    			}
        flag_time_temp=int(test_timer.value()/CONV_MS_SEC);
        if((flag_time_temp >= CLEAR_MESSAGE) && (test_time == true))
    			{       
    				test_time = false; 
    				test_timer.stop();
    				test_timer.reset();
    			}
	}
