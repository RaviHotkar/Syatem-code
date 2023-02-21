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

#define CODE_DOWNLOAD_LINK   "https://raw.githubusercontent.com/RaviHotkar/ESP8266OTA/master/ESP8266OTA.bin"

extern ESP_EEPROM_Custom  eeprom_handle;
extern hw_timer_t *timer;

char* host = "djxmmx.net";                                                                         // Dummy website to check internet connection
uint16_t port = 17;

tm*      timeinfo;                                                                                 // localtime returns a pointer to a tm structstatic int Second;
time_t   Epoch,currentEpoc; 

static IPAddress ip;                                                                               // To check the ip address of wifi

WiFiUDP ntpUDP;                                                                                    // Wifi class defination
NTPClient timeClient(ntpUDP, NTP_SERVER, GMT_OFFSET_SEC);                                          // Function defination to get the time for NTP server
//NTPClient time_client;

const int   mqtt_port = 8883;                                                                      // MQTT port number

void callback(char* topic, byte* payload, unsigned int length)                                     // This fuction is called when aws incoming MSG from AWS
{
		char Rdata;
		for (int i = 0; i < length; i++) 
			{
				//Serial.println("IN CALL back very eginning..::: Received data = ");
				Serial.print((char)payload[i]);                                                    // Print payload content
				received_data = received_data + ((char)payload[i]);                                // collect incoming data into buffer
				timerWrite(timer, 0); 
			}
		timerWrite(timer, 0); 
		if (received_data.indexOf("PUMP") >= 0)
			 msgReceived_PUMP_generic = true;  

		if (received_data.indexOf("ssid_pwd") >= 0)                                                // If "ssid_pwd" is found, then ssid and password is change 
		    {
			    msgReceived_ssid_pwd = true;                                                       // Set msgReceived_ssid_pwd flag to true                           
		    }
		if (received_data.indexOf("enquiry") >= 0)                                                 // If "enquiry" is found, then whole system information is get 
		    {
				msgReceived_enquiry = true;                                                        // Set msgReceived_enquiry flag to true                           
		    }
		if (received_data.indexOf("On_Off_System") >= 0)                                           // If "On_Off_System" is found, then whole system is "ON" or "OFF"
		    {
				msgReceived_On_Off_System = true;                                                  // Set msgReceived_On_Off_System flag to true                           
		    }
		if (received_data.indexOf("sensor") >= 0)                                           // If "On_Off_System" is found, then whole system is "ON" or "OFF"
		    {
				msgReceived_sensor = true;                                                  // Set msgReceived_On_Off_System flag to true                           
		    }
/*		if (received_data.indexOf("OTA_ESP") >= 0)                                                 // If "OTA_ESP" is found, then whole program can be installed via OTA
		    {
				msgReceived_OTA_ESP = true;                                                        // Set msgReceived_OTA_ESP flag to true                           
		    }
*/
}

WiFiClientSecure espClient;                                                                        // Wifi client class defination
PubSubClient client(AWS_endpoint, mqtt_port, callback, espClient);                                 // set  MQTT port number to 8883 as per standard

void ESP_AWS_Custom :: setup_wifi()                                                                // To assign the WiFi network
{
		delay(10);
		//espClient.setBufferSizes(512, 512);                                                      // We start by connecting to a WiFi network
		Serial.print("Connecting to ");     Serial.println(ssid);                                  // Printing ssid and connecting

/* Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default,
     would try to act as both a client and an access-point and could cause
     network-issues with your other WiFi-devices on your WiFi-network. */
		WiFi.mode(WIFI_STA);                                                     
		WiFi.begin(ssid, password);                                                                // wifi ssid and password are assigned
		for(int i=0; i<NUMBER_RETRIES; i++)                                                        // For loop for NUMBER_RETRIES
			{
				delay(500);                                                                        // Delay 500 ms
				timerWrite(timer, 0);                                                              //reset timer (feed watchdog)
				Serial.print("*"); 
				if(WiFi.status() == WL_CONNECTED)                                                  // If wifi connected then break
				break;
			}
    
		if(WiFi.status() == WL_CONNECTED)                                                          // If wifi connected then
			{
				WiFiClient client1; 
				Serial.println("WiFi connected");                                                  // Print WiFi connected
				Serial.println("IP address: ");                                                    // Print IP address
				Serial.println(WiFi.localIP()); 
                timerWrite(timer, 0);                                              //reset timer (feed watchdog); 				
				if (true)//(client1.connect(host, port)== true) 
/* 18-12-2022: Checking internet connectivitty is removed, as the original check was for ESP8266; not so important as new stragey is to move to BLE solution instead of internet for ESP32.*/
					{
						timerWrite(timer, 0);                                              //reset timer (feed watchdog); 
						timeClient.begin();                                                   
						for(int i=0; i<NUMBER_RETRIES; i++)                                        // For loop for NUMBER_RETRIES
							{
								timeClient.forceUpdate();                                          // Update the time into timeclient
							    timerWrite(timer, 0);                                              //reset timer (feed watchdog); 
								if(timeClient.update() == true)                                    // Breack the for loop when time is update successfully
								break;
							}
//						espClient.setX509Time(timeClient.getEpochTime());                          // Set the time
                        timerWrite(timer, 0);                                              //reset timer (feed watchdog); 
					}
				timerWrite(timer, 0);                                              //reset timer (feed watchdog); 
			}
		//Serial.println("setup_wifi_end..................");                                        // Print setup_wifi_end
}

void ESP_AWS_Custom :: getNTP()                                                                    // To get the data from NTP server
{
		timeClient.begin();
		timeClient.update();
}

void ESP_AWS_Custom :: getEpoch()                                                                  // Read the time
{ 
		WiFiClient client1;
		if((WiFi.status() == WL_CONNECTED) /*&& (client1.connect(host, port)== true)*/)
/* 18-12-2022: Checking internet connectivitty is removed, as the original check was for ESP8266; not so important as new stragey is to move to BLE solution instead of internet for ESP32.*/
			Epoch   = timeClient.getEpochTime();
		else 
			Epoch   = timeClient.getEpochTime(currentEpoc);
}


void ESP_AWS_Custom :: getDay_Of_Week()                                                            // Read the time
{ 
		WiFiClient client1;
		if((WiFi.status() == WL_CONNECTED)/* && (client1.connect(host, port)== true)*/)
/* 18-12-2022: Checking internet connectivitty is removed, as the original check was for ESP8266; not so important as new stragey is to move to BLE solution instead of internet for ESP32.*/
			Day_in_week = timeClient.getDay(); 
		else 
			Day_in_week   =   (((timeClient.getEpochTime(currentEpoc)  / 86400L) + 4 ) % 7);             //calculate time with stored Epoch.
		
}

void ESP_AWS_Custom :: getTimeData()                                                               // Read the time in required format
{
		timeinfo  = localtime(&Epoch);                                                             // cf: https://www.cplusplus.com/reference/ctime/localtime/
		Second    = (unsigned char)timeinfo->tm_sec;
		Minute    = (unsigned char)timeinfo->tm_min;
		Hour      = (unsigned char)timeinfo->tm_hour;
		Day       = (unsigned char)timeinfo->tm_mday;
		Month     = (unsigned char)timeinfo->tm_mon + 1;
		Year      = timeinfo->tm_year + 1900;                                                      // returns years since 1900
		strftime (Time,10, "%T", timeinfo);
		strftime (Date,12, "%d/%m/%Y", timeinfo);
}

void ESP_AWS_Custom :: reconnect()                                                                 // Loop until we're reconnected
{
		WiFiClient client1;
		if (true)//(client1.connect(host, port)== true)                                                    // reconnect only when internet connection is available
/* 18-12-2022: Checking internet connectivitty is removed, as the original check was for ESP8266; not so important as new stragey is to move to BLE solution instead of internet for ESP32.*/
			{
				for(int i=0; i<NUMBER_RETRIES_AWS; i++)                                            // For loop for NUMBER_RETRIES_AWS
					{
					    timerWrite(timer, 0);                                                      // reset timer (feed watchdog);                                                      
						if(client.connected()==true)                                               // if connected successfully to AWS then break the loop
						break;
						Serial.println("Attempting MQTT connection...");
						eeprom_handle.EEPROM_write();                                              // Storing present time in EEPROM
            
						if (client.connect("ESPthing"))                                            // Attempt to connect
							{
								Serial.println("connected");
								client.subscribe(INTOPIC);                                         // ... and resubscribe
								client.subscribe(SETTOPIC);
								client.subscribe(ON_OFF_TOPIC);
								client.subscribe(DURATION_TOPIC);
								client.subscribe(SSID_PWD_TOPIC);
                                client.subscribe(ENQUIRY_TOPIC);
								client.subscribe(SENSOR_TOPIC);
//								client.subscribe(OTA_TOPIC);
							} 
						else 
							{
								Serial.print("failed, rc=");                                       // if not connectted mqtt then print the failed rc and print reconnect after 1 sec
						//		Serial.print(client.state());
          
								char buf[256];
//								espClient.getLastSSLError(buf,256);
							    timerWrite(timer, 0);                                              // reset timer (feed watchdog); 
								Serial.print("WiFiClientSecure SSL error: ");                      // if not connectted mqtt then print the WiFiClientSecure SSL error
						//		Serial.println(buf);
							}
					}
			}
		else
			Serial.println("connection failed");                                                   // if not connected print the connection failed
}

void ESP_AWS_Custom :: reconnect_WiFi_AWS()                                                        // Reconnect the WiFi again and again when was disconnected
{
	//	Serial.println("Inside - reconnect_WiFi_AWS : before Wifi Status....");
		wifi_current = WiFi.status();
		WiFiClient client1;
	//	Serial.println("Inside - reconnect_WiFi_AWS : checking internet......");
		internet_conn_current = true; //client1.connect(host, port); 
/* 18-12-2022: Checking internet connectivitty is removed, as the original check was for ESP8266; not so important as new stragey is to move to BLE solution instead of internet for ESP32.*/
	//	Serial.println("Inside - reconnect_WiFi_AWS : Before all checks....");
		if(((WiFi.status()== WL_CONNECTED) && (wifi_previous != WL_CONNECTED)) || ((internet_conn_current == true) && (internet_conn_previous == false)))
		{
			setup_wifi();                                                                          // Wifi function added
			getNTP();                                                                              // NTP server function added
			certificates();
	//		Serial.println("Inside - reconnect_WiFi_AWS : Inside IF after setup WIFI , NTP and certificates....");
		}
		if (!client.connected())                                                                   // If AWS is not connected
		{
		    timerWrite(timer, 0);                                                                  //reset timer (feed watchdog);                                                             
			if ((WiFi.status()!= WL_CONNECTED))
			    setup_wifi(); 
			reconnect();                                                                           // recoonect the aws
		    timerWrite(timer, 0);                                                                  //reset timer (feed watchdog);
    //        Serial.println("Inside - reconnect_WiFi_AWS : After Reconnect is done......");			
			
		}
		wifi_previous = wifi_current;
		internet_conn_previous = internet_conn_current;
}

void ESP_AWS_Custom :: read_data_AWS()                                                             // Read AWS related functions
{
        if (msgReceived_PUMP_generic == true)
        {			
			msgReceived_PUMP_generic = false;
			for (int temp_pumps = 0; temp_pumps < NUMBER_OF_PUMPS;temp_pumps++ )
			{
				
				if (received_data.indexOf("\"PUMP"+(String)(temp_pumps+1)+ "\"") >= 0)                                                   // If "PUMP1" is found, then pump1 is "ON"
				{
					msgReceived_Pump[temp_pumps] = true;                                                          // Set msgReceived_Pump1 flag to true
	//				Serial.print("read_data_AWS --> PUMP :  ");Serial.println(temp_pumps+1);
				}
				
				read_data_AWS_Pump(temp_pumps);	
				timerWrite(timer, 0); 
			}
		
		}
		read_data_AWS_ssid_pwd();                                                                  // Function which changes ssid and password manually
		read_data_AWS_On_Off_System();                                                             // Function which Turn 'ON' and Turn 'OFF' the whole system manually
		read_data_AWS_enquiry();                                                                   // Function which get All Information about system functionalitycs
//		read_data_AWS_OTA_ESP();
}

void ESP_AWS_Custom :: read_data_AWS_Pump(unsigned char Pump_temp)                                                       // Read Function to set Topic of Pump1 slots (hours & minites) at single time
{
	unsigned char temp_hour1,temp_min1,temp_hour2,temp_min2,temp_hour3,temp_min3,temp_hour4,temp_min4;
	unsigned char temp_day0_pump , temp_day1_pump, temp_day2_pump, temp_day3_pump, temp_day4_pump,temp_day5_pump,temp_day6_pump;
    unsigned char temp_Week_Freq_pump,temp_on_off_sequence;
	if (msgReceived_Pump[Pump_temp] == true)
        {                                                                 						   // input format {"msgReceived_Pump1": 1}
            msgReceived_Pump[Pump_temp] = false;
		    
            int aws_index = received_data.indexOf("\"PUMP"+String(Pump_temp+1)+"\":");          						   // Store index of "PUMP1" in aws_index
            String tmp_str = received_data.substring(aws_index);                                     // Substring of received_data starting from the index of "PUMP1" to end of the string
 		    char msg[tmp_str.length()+1];                                                            // Declare an array of characters of length equal to length of the tmp_str+1
            received_data = "";
            tmp_str.toCharArray(msg, tmp_str.length()+1);                                            // Copy the character's of string tmp_str to the supplied buffer "msg" of length is equal to (length of tmp_str+1)
            
		    String temp_pump = (String)"\"PUMP"+ (String)(Pump_temp+1)+(String)"\":" ;
 		    
		    char temp_pump_process[temp_pump.length()+1];
		    temp_pump.toCharArray(temp_pump_process, temp_pump.length()+1);
		    
		    if ( strstr(msg,temp_pump_process) != NULL )                                                  // If the first occurrence of "PUMP1" in the string "msg" is not NULL then
                {
                    char *token,*token1;                                                              // Declare a character pointer variable token
                    
				    String temp_input;
				    
   				    token   = strtok(msg, " ");                                                      // Get the content of string seperated by space 
                    token1  = strtok(NULL, " ");                                                     // Get the next remaining string into token 
                    
				    temp_input = token1;
				    temp_hour1 = (unsigned char)temp_input.toInt();
				    
				    token1  = strtok(NULL, " ");
				    temp_input = token1; 
                    temp_min1 = (char)temp_input.toInt();
				    
				    token1  = strtok(NULL, " ");
				    temp_input = token1; 
                    temp_hour2 = (unsigned char)temp_input.toInt();
				    
				    token1  = strtok(NULL, " ");
				    temp_input = token1; 
                    temp_min2 = (unsigned char)temp_input.toInt();
				    
				    token1  = strtok(NULL, " ");
				    temp_input = token1;
				    temp_hour3 = (unsigned char)temp_input.toInt();
				    
				    token1  = strtok(NULL, " ");
				    temp_input = token1; 
                    temp_min3 = (unsigned char)temp_input.toInt();
				    
				    token1  = strtok(NULL, " ");
				    temp_input = token1; 
                    temp_hour4 = (unsigned char)temp_input.toInt();
				    
				    token1  = strtok(NULL, " ");
				    temp_input = token1; 
                    temp_min4 = (unsigned char)temp_input.toInt();
				    
				    token1  = strtok(NULL, " ");
				    temp_input = token1; 
                    temp_day0_pump = (unsigned char)temp_input.toInt();
				    
				    token1  = strtok(NULL, " ");
				    temp_input = token1; 
                    temp_day1_pump = (unsigned char)temp_input.toInt();
				    
				    token1  = strtok(NULL, " ");
				    temp_input = token1; 
                    temp_day2_pump = (unsigned char)temp_input.toInt();
				    
				    token1  = strtok(NULL, " ");
				    temp_input = token1; 
                    temp_day3_pump = (unsigned char)temp_input.toInt();
				    
				    token1  = strtok(NULL, " ");
				    temp_input = token1; 
                    temp_day4_pump = (unsigned char)temp_input.toInt();
				    
				    token1  = strtok(NULL, " ");
				    temp_input = token1; 
                    temp_day5_pump = (unsigned char)temp_input.toInt();
				    
				    token1  = strtok(NULL, " ");
				    temp_input = token1; 
                    temp_day6_pump = (unsigned char)temp_input.toInt();
				    
				    token1  = strtok(NULL, " ");
				    temp_input = token1; 
                    temp_Week_Freq_pump = (unsigned char)temp_input.toInt();
				    
				    token1  = strtok(NULL, " ");
				    temp_input = token1; 
                    temp_on_off_sequence = (unsigned char)temp_input.toInt();
				    
//				    Serial.print("Pump_temp index  = ");Serial.println(Pump_temp);
//				    Serial.println("Data received....");
				    
				    Hour1[Pump_temp] = temp_hour1;
				    Min1[Pump_temp]  = temp_min1;
				    
				    Hour2[Pump_temp] = temp_hour2;
				    Min2[Pump_temp]  = temp_min2;
				    
				    Hour3[Pump_temp] = temp_hour3;
				    Min3[Pump_temp]  = temp_min3;
				    
				    Hour4[Pump_temp] = temp_hour4;
				    Min4[Pump_temp]  = temp_min4;
				    
				    Day0[Pump_temp] = temp_day0_pump;	  
				    Day1[Pump_temp] = temp_day1_pump;
				    Day2[Pump_temp] = temp_day2_pump;
				    Day3[Pump_temp] = temp_day3_pump;
				    Day4[Pump_temp] = temp_day4_pump;
				    Day5[Pump_temp] = temp_day5_pump;
				    Day6[Pump_temp] = temp_day6_pump;
				    
                    Week_Freq[Pump_temp]  = temp_Week_Freq_pump;
                    On_Off_sequence[Pump_temp] = temp_on_off_sequence;
				     
     		        timerWrite(timer, 0);                                                          // reset timer (feed watchdog);   
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
					
					boolean write_status = EEPROM.commit();
					Serial.println((write_status) ? "Commit OK" : "Commit failed");
				    
				    timerWrite(timer, 0);                                                                  //reset timer (feed watchdog);  
				    String temp_pump_local = (String)"PUMP"+String(Pump_temp+1)+(String)"_TIME_SET_DONE"; 
		            char temp_array_output[temp_pump_local.length()+1];
				    temp_pump_local.toCharArray(temp_array_output, temp_pump_local.length()+1);
				    
				    char *temp_pump_process;
		            temp_pump_process = temp_array_output;
                    
				    timerWrite(timer, 0); 
//				    Serial.print("In read_data_AWS_Pump --> PUMP_Number:   ");Serial.println(Pump_temp+1);
                    client.publish(SETDONE_TOPIC, temp_pump_process);
                }       
        }
}  


void ESP_AWS_Custom :: read_data_AWS_ssid_pwd()                                                    // Read Function to change ssid & password of WiFi network
{
    if (msgReceived_ssid_pwd == true)
        {                                                                                            // input format {"msgReceived_ssid_pwd": 1}
            msgReceived_ssid_pwd = false;
            int aws_index = received_data.indexOf("\"ssid_pwd\":");                                  // Store index of "ssid_pwd" in aws_index
		    
            String tmp_str = received_data.substring(aws_index);                                     // Substring of received_data starting from the index of "ssid_pwd" to end of the string
            char msg[tmp_str.length()+1];                                                            // Declare an array of characters of length equal to length of the tmp_str+1
            received_data = "";
            tmp_str.toCharArray(msg, tmp_str.length()+1);                                            // Copy the character's of string tmp_str to the supplied buffer "msg" of length is equal to (length of tmp_str+1)
		    
            unsigned char temp_cmd = 0; 
            if ( strstr(msg,"\"ssid_pwd\":") != NULL )                                               // If the first occurrence of "ssid_pwd" in the string "msg" is not NULL then
                {
                    char *token1,*token2;
                    char token3[30],token4[30];                                                      // Declare a character pointer variable token
                    token1 = strtok(msg, " ");                                                       // Get the content of string seperated by space 
                    String token5 = strtok(NULL, " ");                                               // Get the next remaining string into token 
                    String token6 = strtok(NULL, " ");                                               // Get the next remaining string into token 
				    String token7 = strtok(NULL, " ");
                    token2 = strtok(NULL, " ");                                                      // Get the next remaining string into token 
				    int temp_save_ssid_pwd = token7.toInt();
				    
                    token5.toCharArray(token3, token5.length()+1);  
                    token6.toCharArray(token4, token6.length()+1);
                    token3[token5.length()+1]  = '\0';
                    token4[token6.length()+1]  = '\0';
				    
                    short unsigned int temp_ssid_len = token5.length();
                    short unsigned int temp_password_len = token6.length();
                    Serial.print("token5_length...1........ :  ");Serial.println(token5.length());
                    Serial.print("token6_length...2........ :  ");Serial.println(token6.length());
                    
                    for(int i=0; i<30; i++)
                        {
                            ssid[i] ='\0';
                        }
                    for(int i=0;i<temp_ssid_len;i++)
                        {
                            ssid[i]= token3[i];
                        }
				    for(int i=0; i<30; i++)
                        {
                            password[i] ='\0';
                        } 
                    for(int i=0;i<temp_password_len;i++)
                        {
                            password[i] = token4[i];
                        }
                    
                    ssid_str = String(ssid); 
                    password_str = String(password);
                    
                    Serial.print("ssid will be changed 3:  ");Serial.println(ssid);
                    Serial.print("password will be changed 4:  ");Serial.println(password);
                    setup_wifi();
                    
				    if(temp_save_ssid_pwd == 0x01)
				    {
					    temp_ssid_len = ssid_str.length();
					    EEPROM.put(TEMP_SSID_LEN_ADDRESS,((temp_ssid_len+1)&0xFF));
					    Serial.print("temp_ssid_len 5:  ");Serial.println(temp_ssid_len);
					    
					    for (int index = BEGENING_SSID_ADDRESS,i=0; i < temp_ssid_len; index++,i++)
						    {
							    EEPROM.put(index, ssid_str.charAt(i));                                   // Update all the data read in the buffer to EEPROM buffer.
						    } 
					    EEPROM.put(BEGENING_SSID_ADDRESS+temp_ssid_len, '\0');
					    
					    temp_password_len = password_str.length();
					    EEPROM.put(TEMP_PWD_LEN_ADDRESS, ((temp_password_len+1)&0xFF));
					    Serial.print("temp_password_len 6:  ");Serial.println(temp_password_len);    
					    
					    for (int index = BEGENING_PWD_ADDRESS,j=0; j < temp_password_len; index++,j++)
						    {
							    EEPROM.put(index, password_str.charAt(j));                               // Update all the data read in the buffer to EEPROM buffer.
						    }
					  EEPROM.put(BEGENING_PWD_ADDRESS+temp_password_len, '\0'); 
                                                                           
					  boolean write_status = EEPROM.commit();
					  client.publish(SETDONE_TOPIC, "ssid_pwd_Done saved to EEPROM");
				    }
				  
				    else
					    client.publish(SETDONE_TOPIC, "ssid_pwd_Done not_saved to EEPROM");
                }
        }
}

void ESP_AWS_Custom :: read_data_AWS_On_Off_System()                                               // Read Function to change System "ON & OFF" status 
{
    if (msgReceived_On_Off_System == true)
        {                                                                                          // input format {"msgReceived_On_Off_System": 1}
            msgReceived_On_Off_System = false;
            int aws_index = received_data.indexOf("\"On_Off_System\":");                           // Store index of "On_Off_System" in aws_index
        
            String tmp_str = received_data.substring(aws_index);                                   // Substring of received_data starting from the index of "On_Off_System" to end of the string
            char msg[tmp_str.length()+1];                                                          // Declare an array of characters of length equal to length of the tmp_str+1
            received_data = "";
            tmp_str.toCharArray(msg, tmp_str.length()+1);                                          // Copy the character's of string tmp_str to the supplied buffer "msg" of length is equal to (length of tmp_str+1)
  
            unsigned int temp_On_Off_System_status = 0;  
            if ( strstr(msg,"\"On_Off_System\":") != NULL )                                        // If the first occurrence of "On_Off_System" in the string "msg" is not NULL then
                {
                    char *token,*token1;                                                           // Declare a character pointer variable token
                    token = strtok(msg, " ");                                                      // Get the content of string seperated by space 
                    token1 = strtok(NULL, " ");                                                    // Get the next remaining string into token 
  
                    String temp_input = token1; 
                    temp_On_Off_System_status = temp_input.toInt();
                    
                    char *temp_output;  
                    On_Off_System_sequence = temp_On_Off_System_status;
                    EEPROM.put(ON_OFF_SYSTEM_SEQ_ADDRESS, On_Off_System_sequence);
                    boolean write_status = EEPROM.commit();
                    if(On_Off_System_sequence == true)
                        temp_output = "System_ESP_seq_change_ON";
                    else
                        temp_output = "System_ESP_seq_change_OFF";
                    client.publish(SETDONE_TOPIC, temp_output);
                }
        }
}

void ESP_AWS_Custom :: read_data_AWS_enquiry()                                                     // Information of the whole system
{  
    if (msgReceived_enquiry == true)                                                             // input format {"msgReceived_enquiry": 1}
        {                                                                                      
            String pump_info[NUMBER_OF_PUMPS];
			msgReceived_enquiry = false;
            String Space_str = "  ";
			received_data = "";               //Clear the message recevied data string.
		
			String enquiry = "---------------ENQUIRY--------BEGIN---------------\r\n";
			enquiry+= "SW_VER:" + Space_str + SW_VER + Space_str + CUSTOMER_NUMBER + Space_str + ssid_str + Space_str + password_str + Space_str + " SYSTEM:  " + On_Off_System_sequence + "\r\n";
    	
			timerWrite(timer, 0);  
			for (int Pump_temp=0;Pump_temp<NUMBER_OF_PUMPS;Pump_temp++)
			{
			      pump_info[Pump_temp] = (String)"PUMP" + String(Pump_temp+1) + ":" + Space_str;
				  
				  pump_info[Pump_temp] += String(Hour1[Pump_temp]) + Space_str + String(Min1[Pump_temp])+ Space_str + String(Hour2[Pump_temp])+ Space_str +String( Min2[Pump_temp])+ Space_str;
				  pump_info[Pump_temp] += String(Hour3[Pump_temp]) + Space_str + String(Min3[Pump_temp])+ Space_str + String(Hour4[Pump_temp])+ Space_str +String( Min4[Pump_temp])+ Space_str;
				  pump_info[Pump_temp] += String(Day0[Pump_temp]) + Space_str + String(Day1[Pump_temp])+ Space_str + String(Day2[Pump_temp])+ Space_str +String( Day3[Pump_temp])+ Space_str;
				  pump_info[Pump_temp] += String(Day4[Pump_temp]) + Space_str + String(Day5[Pump_temp])+ Space_str + String(Day6[Pump_temp])+ Space_str +String( Week_Freq[Pump_temp])+ Space_str;
				  pump_info[Pump_temp] += String(On_Off_sequence[Pump_temp]) + "\r\n";
				  timerWrite(timer, 0);  
				  enquiry+=pump_info[Pump_temp];
			}
			
			enquiry += "---------------ENQUIRY--------END---------------\r\n";
			
			char msg1[enquiry.length()+1];
			enquiry.toCharArray(msg1, enquiry.length()+1);
			client.publish(ENQ_RESP_TOPIC, msg1); 
		}
}
 /* 
void ESP_AWS_Custom :: read_data_AWS_OTA_ESP()
{
    if (msgReceived_OTA_ESP == true)                                                               // input format {"msgReceived_OTA_ESP": 1}
      {                                                                                            
		  msgReceived_OTA_ESP = false;
          int aws_index = received_data.indexOf("\"OTA_ESP\":");                                   // Store index of "OTA_ESP" in aws_index
      
          String tmp_str = received_data.substring(aws_index);                                     // Substring of received_data starting from the index of "OTA_ESP" to end of the string
          char msg[tmp_str.length()+1];                                                            // Declare an array of characters of length equal to length of the tmp_str+1
          received_data = "";
          tmp_str.toCharArray(msg, tmp_str.length()+1);                                            // Copy the character's of string tmp_str to the supplied buffer "msg" of length is equal to (length of tmp_str+1)

          unsigned char temp_cmd = 0; 
          if ( strstr(msg,"\"OTA_ESP\":") != NULL )                                                // If the first occurrence of "OTA_ESP" in the string "msg" is not NULL then
              {
                  char *token1,*token2;
                  char token3[30],token4[30];                                                      // Declare a character pointer variable token
                  token1 = strtok(msg, " ");                                                       // Get the content of string seperated by space 
                  String token5 = strtok(NULL, " ");                                               // Get the next remaining string into token 
				  String token6 = strtok(NULL, "#");
				  Serial.print("OTA Link : ");Serial.println(token5);
				  Serial.print("Finger print: ");Serial.println(token6);
	
	              String url = CODE_DOWNLOAD_LINK;
                  for (uint8_t t = 4; t > 0; t--) 
				  {
					Serial.printf("[SETUP] WAIT %d...\n", t);
					Serial.flush();
					delay(1000);
				  }
			      if (1)
				  {
					ESPhttpUpdate.rebootOnUpdate(0x01);
					Serial.println("Before starting to download from the link.....");
					t_httpUpdate_return ret = ESPhttpUpdate.update(url,"","70 94 DE DD E6 C4 69 48 3A 92 70 A1 48 56 78 2D 18 64 E0 B7");
					Serial.println("After OTA completered#############***********...");
				  
				  
					switch (ret) 
						{
							case HTTP_UPDATE_FAILED:
							Serial.printf("HTTP_UPDATE_FAILD Error (%d): %s\n", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
							break;

							case HTTP_UPDATE_NO_UPDATES:
							Serial.println("HTTP_UPDATE_NO_UPDATES");
							break;

							case HTTP_UPDATE_OK:
							Serial.println("HTTP_UPDATE_OK");
							break;
					   }
				  }
              }
      }
}
*/ 
void ESP_AWS_Custom :: loop_AWS_NTP()
{
	timeClient.update();                                                                           // should be called from time to time to refresh NTP when expired, does nothing most of the time.
	getEpoch();                                                                                    // writes the Epoch (Numbers of seconds till 1.1.1970...
	getTimeData();                                                                                 // breaks down the Epoch into discrete values.

    //Day_in_week = timeClient.getDay(); 
	getDay_Of_Week();
	//Serial.print("Day of the week = "); Serial.println(Day_in_week);
}

void ESP_AWS_Custom :: loop_AWS_pub()
{
	timerWrite(timer, 0); 
	client.loop(); 
	timerWrite(timer, 0); 
}

void ESP_AWS_Custom :: loop_AWS_publish(const char* topic, const char* payload)
{
	timerWrite(timer, 0); 
	client.publish(topic, payload); 
	timerWrite(timer, 0); 
}

time_t ESP_AWS_Custom :: timeClient_AWS()
{
	time_t currentEpoc_temp   = timeClient.getcurrentEpoc() ;   
	return currentEpoc_temp;
}


void ESP_AWS_Custom :: certificates()                                                              // Read Function for AWS certificates
{
  //=============================================================
  if (!SPIFFS.begin(true)) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  //=======================================
  //Root CA File Reading.
  File file2 = SPIFFS.open("/AmazonRootCA1.pem", "r");
  if (!file2) {
//    Serial.println("Failed to open file for reading");
    return;
  }
//  Serial.println("Root CA File Content:");
  while (file2.available()) {
    Read_rootca = file2.readString();
//    Serial.println(Read_rootca);
  }
  //=============================================
  // Cert file reading
  File file4 = SPIFFS.open("/certificate.pem.crt", "r");
  if (!file4) {
//    Serial.println("Failed to open file for reading");
    return;
  }
//  Serial.println("Cert File Content:");
  while (file4.available()) {
    Read_cert = file4.readString();
//    Serial.println(Read_cert);
  }
  //=================================================
  //Privatekey file reading
  File file6 = SPIFFS.open("/private.pem.key", "r");
  if (!file6) {
//    Serial.println("Failed to open file for reading");
    return;
  }
//  Serial.println("privateKey File Content:");
  while (file6.available()) {
    Read_privatekey = file6.readString();
//    Serial.println(Read_privatekey);
  }
  //=====================================================

  char* pRead_rootca;
  pRead_rootca = (char *)malloc(sizeof(char) * (Read_rootca.length() + 1));
  strcpy(pRead_rootca, Read_rootca.c_str());

  char* pRead_cert;
  pRead_cert = (char *)malloc(sizeof(char) * (Read_cert.length() + 1));
  strcpy(pRead_cert, Read_cert.c_str());

  char* pRead_privatekey;
  pRead_privatekey = (char *)malloc(sizeof(char) * (Read_privatekey.length() + 1));
  strcpy(pRead_privatekey, Read_privatekey.c_str());
/*
  Serial.println("================================================================================================");
  Serial.println("Certificates that passing to espClient Method");
  Serial.println();
  Serial.println("Root CA:");
  Serial.write(pRead_rootca);
  Serial.println("================================================================================================");
  Serial.println();
  Serial.println("Cert:");
  Serial.write(pRead_cert);
  Serial.println("================================================================================================");
  Serial.println();
  Serial.println("privateKey:");
  Serial.write(pRead_privatekey);
  Serial.println("================================================================================================");
*/
  espClient.setCACert(pRead_rootca);
  espClient.setCertificate(pRead_cert);
  espClient.setPrivateKey(pRead_privatekey);

//  client.setServer(AWS_endpoint, mqtt_port);
//  client.setCallback(callback);

  //====================================================================================================================
  WiFi.macAddress(mac);
  snprintf(mac_Id, sizeof(mac_Id), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  Serial.print(mac_Id);
  //=====================================================================================================================
  delay(2000);
  
}
