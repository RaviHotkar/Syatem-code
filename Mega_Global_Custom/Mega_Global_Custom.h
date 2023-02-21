#include "StopWatch.h"

#define SOFTWARE_VER         "Mega_00.01.00"                                   // Software version of Mega_2560

/*****Mega_Sensor_Custom Pin Numbers start*****/ 

/*****Mega_Sensor_Custom Pin Numbers End*****/

/*****Mega_Sensor_Custom Parameter start*****/ 
#define SENSOR_ACT_TIME  ONE_MIN_TIME 
#define CLEAR_MESSAGE   ONE_MIN_TIME                                    // For every one minute sensors(DHT and Soil Moisture) will read
/*****Mega_Sensor_Custom Parameter End*****/

/*****Mega_Pump_Valve_Custom Pin Numbers start*****/ 
#define RELAY1  22                                                               // Make Relay1 as output for pump
#define RELAY2  23 
#define RELAY3  24
#define RELAY4  25

#define RELAY5  26
#define RELAY6  27                                                              
#define RELAY7  28
#define RELAY8  29                                                             // Make Relay8 as output for pump

#define RELAY9  30                                                               // Make Relay1 as output for pump
#define RELAY10 31 
#define RELAY11 32
#define RELAY12 33

#define RELAY13 34
#define RELAY14 35                                                              
#define RELAY15 36
#define RELAY16 37 

#define RELAY17 38                                                               // Make Relay1 as output for pump
#define RELAY18 39 
#define RELAY19 40
#define RELAY20 41

#define LED1    42 
#define LED2    43
#define LED3    44
/*****Mega_Pump_Valve_Custom Pin Numbers End*****/

/*****Mega_Global_Custom Parameter start*****/ 
#define CONV_MS_SEC          1000                                              // conversion of mili_sec to sec
#define HALF_MIN_TIME        30                                                // 30   for half minute
#define ONE_MIN_TIME         60                                                // 60   for 1    minute
#define TWO_MIN_TIME         120                                               // 120  for 2    minute
#define THREE_MIN_TIME       180                                               // 180  for 3    minute
#define FOUR_MIN_TIME        240                                               // 240  for 4    minute
#define TEN_MIN_TIME         600                                               // 600  for 10   minute
#define FIFTEEN_MIN_TIME     900                                               // 900  for 15   minute
#define TWENTY_MIN_TIME      1200                                              // 1200 for 20   minute
#define TWENTY_FIVE_MIN_TIME 1500                                              // 1200 for 25   minute
#define THIRTY_MIN_TIME      1800                                              // 1800 for 30   minute
#define ONE_HOUR_TIME        3600                                              // 3600 FOR 60   minute
/*****Mega_Global_Custom Parameter End*****/

/*****Mega_Eeprom_Custom Address Assigning start*****/ 
#define EEPROM_STATUS_ADDRESS  0

#define PUMP1_DURATION_ADDRESS 1
#define PUMP2_DURATION_ADDRESS 5
#define PUMP3_DURATION_ADDRESS 9
#define PUMP4_DURATION_ADDRESS 13

#define PUMP5_DURATION_ADDRESS 17
#define PUMP6_DURATION_ADDRESS 21
#define PUMP7_DURATION_ADDRESS 25
#define PUMP8_DURATION_ADDRESS 29

#define SHT_ON_OFF_ADDRESS     34
#define MOIST_ON_OFF_ADDRESS   38

#define SYSTEM_ON_OFF_ADDRESS  42
//#define ACCOUNT_ON_OFF_ADDRESS 46

#define PUMP9_DURATION_ADDRESS  50
#define PUMP10_DURATION_ADDRESS 54
#define PUMP11_DURATION_ADDRESS 58
#define PUMP12_DURATION_ADDRESS 62

#define PUMP13_DURATION_ADDRESS 66
#define PUMP14_DURATION_ADDRESS 70
#define PUMP15_DURATION_ADDRESS 74
#define PUMP16_DURATION_ADDRESS 78

#define PUMP17_DURATION_ADDRESS 82
#define PUMP18_DURATION_ADDRESS 86
#define PUMP19_DURATION_ADDRESS 90
#define PUMP20_DURATION_ADDRESS 94

/*****Mega_Eeprom_Custom Address Assigning End*****/

/************************************************************Variables Start***********************************************************/
#ifndef Mega_Global_Custom_h

//				extern long lastMsg1;                                          // To check the time elapse in sensor
				
/*****Mega_ESP_Custom Variables Start*****/
                extern String SW_VER;
				extern String received_data;                                   // string to receive data from aws
				extern bool msgReceived;                                       // flag to indicate complete frame is received from esp8266
				
				extern String enquiry;
				extern String enquiry1;
				extern String enquiry2;
/*****Mega_ESP_Custom Variables End*****/

/*****Mega_Pump_Valve_Custom Variables Start*****/	
				extern StopWatch pump1_timer;                                  // StopWatch for pump activated timer completed 
				extern StopWatch pump2_timer;
				extern StopWatch pump3_timer;
				extern StopWatch pump4_timer;
				
				extern StopWatch pump5_timer;
				extern StopWatch pump6_timer;
				extern StopWatch pump7_timer;
				extern StopWatch pump8_timer;
				
				extern StopWatch pump9_timer;                                  // StopWatch for pump activated timer completed 
				extern StopWatch pump10_timer;
				extern StopWatch pump11_timer;
				extern StopWatch pump12_timer;
				
				extern StopWatch pump13_timer;
				extern StopWatch pump14_timer;
				extern StopWatch pump15_timer;
				extern StopWatch pump16_timer;
				
				extern StopWatch pump17_timer;
				extern StopWatch pump18_timer;
				extern StopWatch pump19_timer;
				extern StopWatch pump20_timer;
				
				extern bool pump1_flag;                                        // flag to indicate the pump gets off after assigned time completed
				extern bool pump2_flag;
				extern bool pump3_flag;
				extern bool pump4_flag;
				
				extern bool pump5_flag;
				extern bool pump6_flag;
				extern bool pump7_flag;
				extern bool pump8_flag;
				
				extern bool pump9_flag;                                        // flag to indicate the pump gets off after assigned time completed
				extern bool pump10_flag;
				extern bool pump11_flag;
				extern bool pump12_flag;
				
				extern bool pump13_flag;
				extern bool pump14_flag;
				extern bool pump15_flag;
				extern bool pump16_flag;
				
				extern bool pump17_flag;
				extern bool pump18_flag;
				extern bool pump19_flag;
				extern bool pump20_flag;
				
				extern bool Pump19_20_On;
				
				extern bool Sensor_read_flag_set;// = false;
/*****Mega_Pump_Valve_Custom Variables End*****/

/*****Mega_Sensor_Custom Variables Start*****/	
				extern StopWatch Sensor_read_timer;                                   // StopWatch for pump activated timer completed then clearing flag


				extern bool time_slot_sensor;
/*****Mega_Sensor_Custom Variables End*****/

/*****Mega_Eeprom_Custom, Mega_ESP_Custom and Mega_Pump_Valve_Custom Variables Start*****/	
				extern unsigned int PUMP1_TIME;                                // PUMP1 time assigned                   
				extern unsigned int PUMP2_TIME;                                      
				extern unsigned int PUMP3_TIME;                                      
				extern unsigned int PUMP4_TIME;
				
				extern unsigned int PUMP5_TIME;
				extern unsigned int PUMP6_TIME;
				extern unsigned int PUMP7_TIME;
				extern unsigned int PUMP8_TIME;
				
				extern unsigned int PUMP9_TIME;                                // PUMP1 time assigned                   
				extern unsigned int PUMP10_TIME;                                      
				extern unsigned int PUMP11_TIME;                                      
				extern unsigned int PUMP12_TIME;
				
				extern unsigned int PUMP13_TIME;
				extern unsigned int PUMP14_TIME;
				extern unsigned int PUMP15_TIME;
				extern unsigned int PUMP16_TIME;
				
				extern unsigned int PUMP17_TIME;
				extern unsigned int PUMP18_TIME;
				extern unsigned int PUMP19_TIME;
				extern unsigned int PUMP20_TIME;
/*****Mega_Eeprom_Custom, Mega_ESP_Custom and Mega_Pump_Valve_Custom Variables End*****/

/*****Mega_Eeprom_Custom and Mega_ESP_Custom Variables Start*****/
				extern unsigned char SHT_On_Off_sequence;                      // DHT_Sensor On Off Status
				extern unsigned char Moist_On_Off_sequence;                    // Soil_Moisture_Sensor On Off Status

				extern unsigned char System_On_Off_sequence;                   // System On Off Status
/*****Mega_Eeprom_Custom and Mega_ESP_Custom Variables End*****/

/*****Mega_ESP_Custom and Mega_Pump_Valve_Custom Variables Start*****/
				extern int input_command;                                      // pump activation command
				
				extern bool exhast_status_temp,exhast_status_humidity_high, exhast_status_humidity_low;
/*****Mega_ESP_Custom and Mega_Pump_Valve_Custom Variables End*****/

#else
//				long lastMsg1 = 0;                                             // To check the time elapse in sensor
                
/*****Mega_ESP_Custom Variables Start*****/
                String SW_VER = SOFTWARE_VER;
				String received_data;                                          // string to receive data from aws
				bool msgReceived;                                              // flag to indicate complete frame is received from esp8266
				
				String enquiry;
				String enquiry1;
				String enquiry2;
/*****Mega_ESP_Custom Variables End*****/

/*****Mega_Pump_Valve_Custom Variables Start*****/	
				StopWatch pump1_timer;                                         // StopWatch for pump activated timer completed 
				StopWatch pump2_timer;
				StopWatch pump3_timer;
				StopWatch pump4_timer;
				
				StopWatch pump5_timer;
				StopWatch pump6_timer;
				StopWatch pump7_timer;
				StopWatch pump8_timer;
				
				StopWatch pump9_timer;                                         // StopWatch for pump activated timer completed 
				StopWatch pump10_timer;
				StopWatch pump11_timer;
				StopWatch pump12_timer;
				
				StopWatch pump13_timer;
				StopWatch pump14_timer;
				StopWatch pump15_timer;
				StopWatch pump16_timer;
				
				StopWatch pump17_timer;
				StopWatch pump18_timer;
				StopWatch pump19_timer;
				StopWatch pump20_timer;
				
				bool pump1_flag  = false;                                               // flag to indicate the pump gets off after assigned time completed
				bool pump2_flag  = false;
				bool pump3_flag  = false;
				bool pump4_flag  = false;
				
				bool pump5_flag  = false;
				bool pump6_flag  = false;
				bool pump7_flag  = false;
				bool pump8_flag  = false;
				
				bool pump9_flag  = false;                                               // flag to indicate the pump gets off after assigned time completed
				bool pump10_flag = false;
				bool pump11_flag = false;
				bool pump12_flag = false;
				
				bool pump13_flag = false;
				bool pump14_flag = false;
				bool pump15_flag = false;
				bool pump16_flag = false;
				
				bool pump17_flag = false;
				bool pump18_flag = false;
				bool pump19_flag = false;
				bool pump20_flag = false;
				
				bool Pump19_20_On = false;
				
				bool Sensor_read_flag_set = false;
/*****Mega_Pump_Valve_Custom Variables End*****/

/*****Mega_Sensor_Custom Variables Start*****/	
				StopWatch Sensor_read_timer;                                          // StopWatch for pump activated timer completed then clearing flag
					
				bool time_slot_sensor = false; 
/*****Mega_Sensor_Custom Variables End*****/

/*****Mega_Eeprom_Custom, Mega_ESP_Custom and Mega_Pump_Valve_Custom Variables Start*****/	
				unsigned int PUMP1_TIME  = HALF_MIN_TIME;                        // PUMP1 time assigned                   
				unsigned int PUMP2_TIME  = HALF_MIN_TIME;                                       
				unsigned int PUMP3_TIME  = HALF_MIN_TIME;                                       
				unsigned int PUMP4_TIME  = HALF_MIN_TIME;
				
				unsigned int PUMP5_TIME  = ONE_MIN_TIME; 
				unsigned int PUMP6_TIME  = ONE_MIN_TIME; 
				unsigned int PUMP7_TIME  = ONE_MIN_TIME; 
				unsigned int PUMP8_TIME  = ONE_MIN_TIME;
				
				unsigned int PUMP9_TIME  = ONE_MIN_TIME;                        // PUMP1 time assigned                   
				unsigned int PUMP10_TIME = ONE_MIN_TIME;                                       
				unsigned int PUMP11_TIME = ONE_MIN_TIME;                                       
				unsigned int PUMP12_TIME = ONE_MIN_TIME;
				
				unsigned int PUMP13_TIME = ONE_MIN_TIME; 
				unsigned int PUMP14_TIME = ONE_MIN_TIME; 
				unsigned int PUMP15_TIME = ONE_MIN_TIME; 
				unsigned int PUMP16_TIME = ONE_MIN_TIME;
				
				unsigned int PUMP17_TIME = ONE_MIN_TIME; 
				unsigned int PUMP18_TIME = ONE_MIN_TIME; 
				unsigned int PUMP19_TIME = ONE_MIN_TIME; 
				unsigned int PUMP20_TIME = ONE_MIN_TIME;
/*****Mega_Eeprom_Custom, Mega_ESP_Custom and Mega_Pump_Valve_Custom Variables End*****/

/*****Mega_Eeprom_Custom and Mega_ESP_Custom Variables Start*****/
				unsigned char SHT_On_Off_sequence = true;                      // DHT_Sensor On Off Status
				unsigned char Moist_On_Off_sequence = true;                    // Soil_Moisture_Sensor On Off Status

				unsigned char System_On_Off_sequence = true;                   // System On Off Status   
/*****Mega_Eeprom_Custom and Mega_ESP_Custom Variables End*****/

/*****Mega_ESP_Custom and Mega_Pump_Valve_Custom Variables Start*****/
				int input_command;                                             // pump activation command
				
				bool exhast_status_temp,exhast_status_humidity_high, exhast_status_humidity_low;
/*****Mega_ESP_Custom and Mega_Pump_Valve_Custom Variables End*****/

#endif

/************************************************************Variables End***********************************************************/

class Mega_Global_Custom
{
	public 	  :
				
	private   :			

	protected : 
};