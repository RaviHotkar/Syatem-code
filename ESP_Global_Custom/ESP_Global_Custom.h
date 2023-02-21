#include "SPIFFS.h"
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <Esp.h>
#include "ESP_AWS_Custom.h"

#include "StopWatch.h"

//-----------------------------#DEFINES BEGIN-----------------------------------------------------------------

#define SOFTWARE_VER    "ESP_00.01.00"                                                        // Software version of ESP_8266

//#define BOT_ENABLE 1
//Comment the above line for System SW. Keep it uncommented for BOT SW.

//#define BOT_TESTING_ENABLE
//Uncomment the above for BOT testing code.

/************************Customer's specific changes begin******************************/

#define AWS_END_POINT   "a2x632alje8yap-ats.iot.ap-south-1.amazonaws.com"                     // AWS_END_POINT for the address of AWS_ACC

#define WIFI_SSID       "BOT_1"                                                               // ssid for the customer
#define WIFI_PWD        "11223344"                                                            // password for the ssid of the customer

#define BLE_NAME        "BOT_1"

#define CUSTOMER_NUMBER "JSW001"                                                               // Customer's unique number

#define INTOPIC         "inTopic11"                                                            // Manually PUMP are "ON" & "OFF"
#define OUTTOPIC        "outTopic11"                                                           // Output of Manually PUMP are "ON" & "OFF"

#define SETTOPIC        "setTopic11"                                                           // PUMP number according to time slot are change 
#define ON_OFF_TOPIC    "On_Off_Topic11"                                                       // PUMP SENSOR and SYSTEM are "ON" & "OFF"
#define DURATION_TOPIC  "Duration_Topic11"                                                     // Duration of PUMP are change
#define SSID_PWD_TOPIC  "ssid_pwd_Topic11"                                                     // ssid & pwd are change 
#define SETDONE_TOPIC   "setdoneTopic11"                                                       // Output of SETTOPIC,ON_OFF_TOPIC,DURATION_TOPIC,SSID_PWD_TOPIC and ERROR_TOPIC

#define ENQUIRY_TOPIC   "enquiry_Topic11"                                                      // Whole system information
#define ENQ_RESP_TOPIC  "enq_resp_Topic11"                                                     // Output of Whole system information

#define SENSOR_TOPIC    "sensor_Topic11"                                                      // Whole system information
#define SENSOR_RESP_TOPIC  "sensor_resp_Topic11"                                                     // Output of Whole system information

//#define OTA_TOPIC       "ota_Topic11"

/************************Customer's specific changes end******************************/

/*****ESP_AWS_Custom Parameter Start*****/ 
#define NUMBER_OF_PUMPS  20  //Define maximum pumps here...

//#define HOST_NAME "ESP-IoT"
#define TZ           5.5                                                                      // (utc+) TZ in hours
//#define DST_MN        60                                                                      // use 60mn for summer time in some countries
#define GMT_OFFSET_SEC 3600 * TZ                                                              // Do not change here...
//#define DAIYLIGHT_OFFSET_SEC 60 * DST_MN                                                      // Do not change here...
#define NTP_SERVER "de.pool.ntp.org"                                                          // NTP server website address

#define NUMBER_RETRIES            5                                                          // setup_wifi() 
#define NUMBER_RETRIES_AWS        2                                                           // reconnect() 

/*****ESP_AWS_Custom Parameter End*****/

#define CONV_MS_SEC               1000                                                        // conversion of mili_sec to sec

/*****ESP_Mega_Custom Parameter Start*****/
#ifdef BOT_TESTING_ENABLE

        #define PUMP_ACT_TIME             20
#else
        #define PUMP_ACT_TIME             62
#endif
#define CLEAR_MESSAGE             60
/*****ESP_Mega_Custom Parameter End*****/

/*****ESP_Mega_Custom & ESP_EEPROM_Custom Parameter Start*****/
#define BUFFER_SIZE               512
/*****ESP_Mega_Custom & ESP_EEPROM_Custom Parameter End*****/

/**********ESP32_Start***********/
//#define BUFFER_LEN 256
/**********ESP32_End***********/

/*****ESP_EEPROM_Custom Address Start*****/

		//-----Generic for calculation-----BEGIN
		#define BASE_ADDRESS_PUMP       100
		#define OFFSET                   20
		#define DYNAMIC_COUNT_OFFSET     17
		//-----Generic for calculation-----END

		#define CURRENTEPOC_ADDRESS                0
		#define HOUR_ADDRESS                       4
		#define MINUTE_ADDRESS                     5

		#define EEPROM_STATUS_ADDRESS              6

		#define TEMP_SSID_LEN_ADDRESS              7
		#define BEGENING_SSID_ADDRESS              8
		#define TEMP_PWD_LEN_ADDRESS              38
		#define BEGENING_PWD_ADDRESS              39
											      
		#define ON_OFF_SYSTEM_SEQ_ADDRESS         69

		#define DAY_IN_WEEK_ADDRESS               70
		#define PRE_DAY_IN_WEEK_ADDRESS           71
		
		//----- 100 to 500 address are reserved for all the 20 PUMP info...
		
/*****ESP_EEPROM_Custom Address End*****/

//----------------------------1.Generics------------Begin----------------------------
				/*****ESP_TIME Parameter start*****/ 
//				#define CONV_MS_SEC          1000                                              // conversion of mili_sec to sec
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
				/*****ESP_TIME Parameter End*****/

//------------------------BOT related BEGIN--------------------------------

				#define LOOP_COUNT 10//25
				
				#define SENSOR_DISTANCE_CHECK 10.0
				#define DELTA 0.0
				
				#define BACK_LIMIT 20  //30
				#define FRONT_LIMIT 20 //30 
				
				#define DUTY_CORRECTION 30
				
				#define STEP_SIZE 10
				
				#define SENSOR_DEBOUNCE 1
				
	#ifdef BOT_TESTING_ENABLE
				#define BOT_SPRAY_CHARGE_TIME 15
	#else
				#define BOT_SPRAY_CHARGE_TIME FIFTEEN_MIN_TIME
	#endif
				
				#define BOT_MOBILE_CHARGE_MIN_PERCENTAGE  40
				#define BOT_MOBILE_CHARGE_MAX_PERCENTAGE  95
				
				
				#define BOT_MOVEMENT_SCHEDULE            0
				#define BOT_SPRAY_SCHEDULE               1
				#define BOT_SPRAY_CHARGE_SCHEDULE        2
				
				#define MAX_PWM 140
				#define MIN_PWM 60
				
//------------------------BOT related BEGIN--------------------------------END-----------------------------------				

//-----------------------------#DEFINES ---------END-----------------------------------------------------------------

/****************************Variables Start***********************************/
#ifdef ESP_Global_Custom_h

/*****ESP_EEPROM_Custom Variables Start*****/
				unsigned char EEPROM_status;                                                  // EEPROM status 
/*****ESP_EEPROM_Custom Variables End*****/	

/*****ESP_Mega_Custom Variables Start*****/	 
				StopWatch flag_timer,test_timer,BOT_Spray_Charge_Timer;                                              // StopWatch timer for PUMP off and clear the message
				String received_mega_data;                                                    // string to receive data from mega
				bool msgReceived_mega = false;                                                // flag to indicate complete frame is received from mega
				bool time_slot_match_pump_act = false;                                        // flag to indicate complete time match for one perticular time slot
				bool test_time = false;
/*****ESP_Mega_Custom Variables End*****/

/*****ESP_AWS_Custom Variables Start*****/			
				String SW_VER = SOFTWARE_VER;
				String received_data;                                                         // string to receive data from aws

				bool internet_conn_current,internet_conn_previous;
				unsigned char wifi_current,wifi_previous;
//------------------------------------~~~~~~~~-----BOT BEGIN----~~~~~~******------//
				//----------------------------------------------------------------------------------
				
				//----------------------------1.Generics------------Begin----------------------------
				long delta_Front , delta_Back ;
				char distance_chk_F, distance_chk_B;
				
				bool exhast_status_temp,exhast_status_humidity_high, exhast_status_humidity_low;
				
				//---------------------------1.Generics------------End-------------------------------
				
				//------------------------2.PWM Begin------------------------------------------------				
				// -------Valid pins
				int RPWM_1 = 4;// Right Motor
				int LPWM_1 = 0;
				int RPWM_2 = 17; // Left Motor
				int LPWM_2 = 16;
				//-----------------
				
				// setting PWM properties
				
				int ledChannel_R1 = 0;
				int ledChannel_L1 = 2;
				int ledChannel_R2 = 4;
				int ledChannel_L2 = 6;
				int freq = 20000;
				int resolution = 8;
				
				//-------------------------2.PWM End-------------------------------------------------
				//------3. Ultra Sonic sensor pins BEGIN---------
				
				int trigPin_Front = 25;
				int echoPin_Front = 34;
				
				int trigPin_Back  = 26;
				int echoPin_Back  = 2;
				
				//---------3.Ultra Sonic sensor END----------------------------------
				
				//---------ENABLE PINS BEGIN------------
				int R_EN1 = 12;
				int L_EN1 = 13;
				int R_EN2 = 27;
				int L_EN2 = 14;
				//---------ENABLE PINS ENDS-------------
				
				//----------4. Relay-Begin---------------------------
				
				int Relay_Spray_ON      = 5;
				int Relay_Spray_Charge  = 18;
				int Relay_Mobile_Charge = 19;
				int Relay4              = 23;
				//----------4. END---------------------------------
				bool msgReceived_BOT_Move = false;
				bool msgReceived_BOT_Spray = false;
				bool msgReceived_BOT_Spray_charge = false;
				bool msgReceived_BOT_Spray_charge2 = true;
				
				//-------------------------------------------
//-----~~~~~~~~-----BOT END----~~~~~~******------//
//------------------------------------------- 
				
                unsigned char Hour1[NUMBER_OF_PUMPS], Min1[NUMBER_OF_PUMPS];
				unsigned char Hour2[NUMBER_OF_PUMPS], Min2[NUMBER_OF_PUMPS];
				unsigned char Hour3[NUMBER_OF_PUMPS], Min3[NUMBER_OF_PUMPS];
				unsigned char Hour4[NUMBER_OF_PUMPS], Min4[NUMBER_OF_PUMPS];	

				unsigned char Day0[NUMBER_OF_PUMPS];                                             // Pump20 weekly trigger
				unsigned char Day1[NUMBER_OF_PUMPS];
				unsigned char Day2[NUMBER_OF_PUMPS];
				unsigned char Day3[NUMBER_OF_PUMPS];
				unsigned char Day4[NUMBER_OF_PUMPS];
				unsigned char Day5[NUMBER_OF_PUMPS];
				unsigned char Day6[NUMBER_OF_PUMPS];
				
				unsigned char Week_Freq[NUMBER_OF_PUMPS], Week_Freq_Dyanamic_Count[NUMBER_OF_PUMPS] ;
				unsigned char On_Off_sequence[NUMBER_OF_PUMPS];	
				
				unsigned char Day_in_week = 0, Pre_Day_in_week = 0;

				unsigned char On_Off_System_sequence = true;

                bool msgReceived_Pump[NUMBER_OF_PUMPS];
				bool msgReceived_PUMP_generic = false;

				bool msgReceived_ssid_pwd = false;                                            // SSID and Password change about the unit Flag
				bool msgReceived_enquiry = false;                                             // Complete information about the whole system Flag
				bool msgReceived_On_Off_System = false;                                       // Complete System On Off Flag
				bool msgReceived_OTA_ESP = false;
				bool msgReceived_sensor = false;                                             // Complete information about the whole system Flag

				unsigned char  Second;
				unsigned char  Minute;
				unsigned char  Hour;
				unsigned char  Day;
				unsigned char  Month;
				int Year;
				char       Time[10];
				char       Date[12];
//				long lastMsg1 = 0;                                                            // To check the time elapse in sensor
				char ssid[30] = WIFI_SSID;                                                    // "Redmi note 7 pro";//"Ravi_Jio";     
				char password[30] = WIFI_PWD;                                                 // "30152676708";//"Bhimu#31"; 
				String ssid_str = WIFI_SSID; 
				String password_str = WIFI_PWD;          
				
				char* AWS_endpoint = AWS_END_POINT;                             // MQTT broker ip
						
				String Read_rootca;
				String Read_cert;
				String Read_privatekey;
				byte mac[6];
				char mac_Id[18];

/////////////////ESP32_End///////////////////
				
/*****ESP_Global_Custom Variables start*****/

/*****ESP_Global_Custom Variables End*****/

#else
    			
/*****ESP_EEPROM_Custom Variables Start*****/
				extern unsigned char EEPROM_status;                                           // EEPROM status 
/*****ESP_EEPROM_Custom Variables End*****/	

/*****ESP_Mega_Custom Variables Start*****/	 
				extern StopWatch flag_timer,test_timer,BOT_Spray_Charge_Timer;                                       // StopWatch timer for PUMP off and clear the message
				extern String received_mega_data;                                             // string to receive data from mega
				extern bool msgReceived_mega;                                                 // flag to indicate complete frame is received from mega
				extern bool time_slot_match_pump_act;                                         // flag to indicate complete time match for one perticular time slot
				extern bool test_time;
				
/*****ESP_Mega_Custom Variables End*****/

/*****ESP_AWS_Custom Variables Start*****/				
				extern String SW_VER;
				extern String received_data;                                                  // string to receive data from aws

				extern bool internet_conn_current,internet_conn_previous;
				extern unsigned char wifi_current,wifi_previous;

//------------------------------------~~~~~~~~-----BOT BEGIN----~~~~~~******------//
				//----------------------------------------------------------------------------------
				extern long delta_Front, delta_Back;
				extern char distance_chk_F, distance_chk_B;
				
				extern bool exhast_status_temp,exhast_status_humidity_high, exhast_status_humidity_low;
								
				//---------------------------1.Generics------------End-------------------------------
				
				//------------------------2.PWM Begin------------------------------------------------

				// -------Valid pins
				extern int RPWM_1; // Right Motor
				extern int LPWM_1;
				extern int RPWM_2; // Left Motor
				extern int LPWM_2;
				//-----------------
				extern bool msgReceived_BOT_Move;
				extern bool msgReceived_BOT_Spray;
				extern bool msgReceived_BOT_Spray_charge;
				extern bool msgReceived_BOT_Spray_charge2;// = false;
				// setting PWM properties
				
				extern int ledChannel_R1;
				extern int ledChannel_L1;
				extern int ledChannel_R2;
				extern int ledChannel_L2;
				extern int freq;
				extern int resolution;
				
				//-------------------------2.PWM End-------------------------------------------------
				
				//------3. Ultra Sonic sensor pins BEGIN---------

				extern int trigPin_Front;// = 27;
				extern int echoPin_Front;// = 34;
				
				extern int trigPin_Back;//  = 14;
				extern int echoPin_Back;//  = 35;

				//---------3.Ultra Sonic sensor END----------------------------------
				//---------ENABLE PINS BEGIN------------
				extern int R_EN1;
				extern int L_EN1;
				extern int R_EN2;
				extern int L_EN2;
				//---------ENABLE PINS ENDS-------------
				
				//----------4. Relay-Begin---------------------------
				
				extern int Relay_Spray_ON;
				extern int Relay_Spray_Charge;
				extern int Relay_Mobile_Charge;
				extern int Relay4;
				//----------4. END---------------------------------				
				
				//-------------------------------------------
//-----~~~~~~~~-----BOT END----~~~~~~******------//
//-------------------------------------------
				extern unsigned char On_Off_System_sequence;                                  // System On Off Status

				extern unsigned char Hour1[NUMBER_OF_PUMPS], Min1[NUMBER_OF_PUMPS];
				extern unsigned char Hour2[NUMBER_OF_PUMPS], Min2[NUMBER_OF_PUMPS];
				extern unsigned char Hour3[NUMBER_OF_PUMPS], Min3[NUMBER_OF_PUMPS];
				extern unsigned char Hour4[NUMBER_OF_PUMPS], Min4[NUMBER_OF_PUMPS];	

				extern unsigned char Day0[NUMBER_OF_PUMPS];                                             
				extern unsigned char Day1[NUMBER_OF_PUMPS];
				extern unsigned char Day2[NUMBER_OF_PUMPS];
				extern unsigned char Day3[NUMBER_OF_PUMPS];
				extern unsigned char Day4[NUMBER_OF_PUMPS];
				extern unsigned char Day5[NUMBER_OF_PUMPS];
				extern unsigned char Day6[NUMBER_OF_PUMPS];
				
				extern unsigned char Day_in_week, Pre_Day_in_week ;
				extern unsigned char Week_Freq[NUMBER_OF_PUMPS], Week_Freq_Dyanamic_Count[NUMBER_OF_PUMPS] ;
				extern unsigned char On_Off_sequence[NUMBER_OF_PUMPS];

                extern bool msgReceived_Pump[NUMBER_OF_PUMPS];
			
				extern bool msgReceived_PUMP_generic;
				extern bool msgReceived_ssid_pwd;                                             // SSID and Password change about the unit Flag
				extern bool msgReceived_enquiry;                                              // Complete information about the whole system Flag
				extern bool msgReceived_On_Off_System;                                        // Complete System On Off Flag
				extern bool msgReceived_OTA_ESP;
				extern bool msgReceived_sensor;                                              // Complete information about the whole system Flag

				extern unsigned char Second;
				extern unsigned char Minute;
				extern unsigned char Hour;
				extern unsigned char Day;
				extern unsigned char Month;
				extern int Year;
				extern char Time[10];
				extern char Date[12];
//				extern long lastMsg1;                                                         // To check the time elapse in sensor
				extern char ssid[30];                                                         // "Redmi note 7 pro";//"Ravi_Jio";     
				extern char password[30];                                                     // "30152676708";//"Bhimu#31"; 
				extern String ssid_str;
				extern String password_str;         
                extern char* AWS_endpoint;                                                   // MQTT broker ip	

/////////////////ESP32_start///////////////////
		
				extern String Read_rootca;
				extern String Read_cert;
				extern String Read_privatekey;
				extern byte mac[6];
				extern char mac_Id[18];

/////////////////ESP32_End///////////////////
	                
#endif

/****************************Variables End***********************************/

class ESP_Global_Custom
{
	public 	  :
				void Initialize_Global_Variables();
	private   :			

	protected : 
};

