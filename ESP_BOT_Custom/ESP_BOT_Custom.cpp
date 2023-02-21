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

////////////////Bluetooth Functions Start/////////////////
 
#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;
char Battery_Level_Debounce_low =0 , Battery_Level_Debounce_high = 0;

extern hw_timer_t *timer;

void ESP_BOT_Custom :: Bot_steps_setup() 
	{
		// put your setup code here, to run once:
		////////////////Bluetooth Functions Start/////////////////
			SerialBT.begin(BLE_NAME); //Bluetooth device name, can give anything
			Serial.println("The device started, now you can pair it with bluetooth!");
			delay(500);
		////////////////Bluetooth Functions Stop/////////////////

//		Serial.println("BOT Set up Start... Before PWM init.....");  
  
		ledcSetup(ledChannel_R1, freq, resolution);
		ledcSetup(ledChannel_L1, freq, resolution);
		ledcSetup(ledChannel_R2, freq, resolution);
		ledcSetup(ledChannel_L2, freq, resolution);
		
//		Serial.println("BOT Set up Start... After PWM init.....");
		
		// attach the channel to the GPIO to be controlled
		ledcAttachPin(RPWM_1, ledChannel_R1);
		ledcAttachPin(LPWM_1, ledChannel_L1);
		ledcAttachPin(RPWM_2, ledChannel_R2);
		ledcAttachPin(LPWM_2, ledChannel_L2);
		
		//Initialize the PWM driver-------------BEGIN
		ledcWrite(ledChannel_R1, 0);
		ledcWrite(ledChannel_L1, 0);
		ledcWrite(ledChannel_R2, 0);
		ledcWrite(ledChannel_L2, 0);
		//--------------------------------------END
		
//		Serial.println("BOT Set up Start... After PWM channel init.....");
    
    /////////////ENABLE PIN CODE STARTS ////////////	
		pinMode(R_EN1, OUTPUT);
		pinMode(L_EN1, OUTPUT);
		pinMode(R_EN2, OUTPUT);
        pinMode(L_EN2, OUTPUT);

        digitalWrite(R_EN1, 0);
		digitalWrite(L_EN1, 0);
		digitalWrite(R_EN2, 0);
        digitalWrite(L_EN2, 0);
	/////////////ENABLE PIN ENDS ////////////			
		
    /////////////RELAY PIN CODE STARTS ////////////	
	    pinMode(Relay_Spray_ON, OUTPUT);
		pinMode(Relay_Spray_Charge, OUTPUT);
		pinMode(Relay_Mobile_Charge, OUTPUT);  
		
		digitalWrite(Relay_Spray_ON, HIGH);
		digitalWrite(Relay_Spray_Charge, HIGH);
		digitalWrite(Relay_Mobile_Charge, HIGH);
	/////////////RELAY PIN CODE ENDS ////////////		
		delay(1000);
	
	/////////////SENSOR PIN CODE STARTS ////////////	
		pinMode(trigPin_Front, OUTPUT);
		pinMode(echoPin_Front, INPUT);
		
		pinMode(trigPin_Back, OUTPUT);
		pinMode(echoPin_Back, INPUT);
    /////////////SENSOR PIN CODE ENDS ////////////
		Serial.println("BOT Set up END.....");

	}
	
void ESP_BOT_Custom :: Bot_steps_loop() 
	{
	/////////////ENABLE PIN CODE STARTS ////////////	
        digitalWrite(R_EN1, 1);
		digitalWrite(L_EN1, 1);
		digitalWrite(R_EN2, 1);
        digitalWrite(L_EN2, 1);
		
	/////////////ENABLE PIN ENDS ////////////			
        timerWrite(timer, 0);                                                 // reset timer (feed watchdog)  
		Serial.println("Bot_loop.........START.........");
		
		if (msgReceived_BOT_Spray_charge == true) 
			{
				Serial.println("BOT_Spray_charge.....Main loop.............");
				BOT_Spray_charge();
				Flag_clear_BOT();
			}
		
		BOT_Mobile_Charge();
		
		
		#ifndef BOT_TESTING_ENABLE
		     Read_Time_From_Mobiel();
		#endif
		
		
		if (msgReceived_BOT_Move == true)
			{
				Serial.println("BOT_Move..................");
				msgReceived_BOT_Move = false;
                BOT_FORWARD_LOOP();
                BOT_BACKWARD_LOOP();
			}
                       
			if ((msgReceived_BOT_Spray == true) && (Hour>=15))// && (Minute>=15)))
     		        msgReceived_BOT_Spray = false;
/*
			SerialBT.end(); //Bluetooth device name, can give anything
			Serial.println("The device DISCONNECTED WITH  bluetooth!");
			delay(2000);
			WiFi.begin(ssid, password); */                                                               // wifi ssid and password are assigned
		Serial.println("Bot_loop.........END.........");
	}

void ESP_BOT_Custom ::BOT_FORWARD_LOOP()
	{
	    int count = 0;               /***********************Forward loop starts****************************/
		while (count < LOOP_COUNT) // change this count in every loop, use this if you have specific checkpoints (increment when cpoint detects)
			{
                Read_All_Distance_Sensors();                                              

                distance_chk_F = Forward_Distance_Check();
                if(distance_chk_F==true)
                    break;
                
                Slow_Start_Forward();
                Slow_Stop_Forward();   // Stop in every loop...
                count ++;
                
                Serial.print("Loop count Forward  = ");Serial.println(count);
			}
	}
void ESP_BOT_Custom ::BOT_BACKWARD_LOOP()
	{
	    int count = 0;                    /***********************Backward loop starts****************************/
        while (count < LOOP_COUNT) // change this count in every loop, use this if you have specific checkpoints (increment when cpoint detects)
			{
                Read_All_Distance_Sensors();

                distance_chk_B= Backward_Distance_Check();
                if(distance_chk_B==true)
                    break;
                
                Slow_Start_Backward();
                Slow_Stop_Backward();
                count ++;
                               
                Serial.print("Loop count Backward  = ");Serial.println(count);
			}
	}

void ESP_BOT_Custom ::BOT_Mobile_Charge()
	{
	    timerWrite(timer, 0);                                                 // reset timer (feed watchdog)  
	    SerialBT.print("Battery Status");
        //Serial.println("Battery Status");
	    delay(1000);
        if (SerialBT.available() != 0) 
	    {
			timerWrite(timer, 0);                                                 // reset timer (feed watchdog)  
			String Battery_level = SerialBT.readString();
			Serial.print("Battrery level = ");Serial.println(Battery_level);
			
			int Battery_charge_percent = Battery_level.toInt();
			
			if(Battery_charge_percent<=BOT_MOBILE_CHARGE_MIN_PERCENTAGE)
				{ 
					Battery_Level_Debounce_low++;
					if(Battery_Level_Debounce_low >=5)
						{
							Battery_Level_Debounce_low = 0;
							digitalWrite(Relay_Mobile_Charge, LOW);
							Serial.println("Charge mobile....");
						}
					
				}
			else if(Battery_charge_percent>=BOT_MOBILE_CHARGE_MAX_PERCENTAGE)
				{ 
					Battery_Level_Debounce_high++;
					if(Battery_Level_Debounce_high >=5)
						{
							Battery_Level_Debounce_high = 0;
							digitalWrite(Relay_Mobile_Charge, HIGH);
							Serial.println(" Stop Charging mobile....");
						}
					
				}
		}
	}

void ESP_BOT_Custom :: Read_All_Distance_Sensors()
    {       
	//-----Sensor Check only--------------------
        delta_Front = measure_distance_Front();
        delta_Back = measure_distance_Back();
    
        Serial.print("delta_Front  = ");Serial.println(delta_Front);
        Serial.print("delta_Back   = ");Serial.println(delta_Back); 
        delay(2000);
    }

char ESP_BOT_Custom :: Forward_Distance_Check()
	{
        delta_Front = measure_distance_Front();
        if( delta_Front < FRONT_LIMIT )
			{
				stopp();
				// delay(1000);
				//break;
				return true; 
			}
	}

char ESP_BOT_Custom :: Backward_Distance_Check()
	{
        delta_Back = measure_distance_Back();
        if( delta_Back < BACK_LIMIT )
			{
				stopp();
				// delay(1000);
				//break;
				return true; 
			}
	}

long ESP_BOT_Custom :: microsecondsToCentimeters(long microseconds)
	{
		return ((microseconds*0.0343)/2);
	}

void ESP_BOT_Custom :: Slow_Start_Forward()
	{
        for(int dutyCycle = MIN_PWM; dutyCycle <= MAX_PWM; )
			{   
                // changing the LED brightness with PWM
                ledcWrite(ledChannel_R1, dutyCycle);
                ledcWrite(ledChannel_L1, 0);
                ledcWrite(ledChannel_R2, dutyCycle);
                ledcWrite(ledChannel_L2, 0);
                dutyCycle+=STEP_SIZE;
                delay(100);
                
				Serial.print("Start up phase...Forward         Duty = ");   Serial.println(dutyCycle);
                
				distance_chk_F = Forward_Distance_Check();
                if(distance_chk_F==true)
                     break;
            }
        Serial.println("Slow start forward...");
	}

void ESP_BOT_Custom :: Slow_Start_Backward()
	{
        for(int dutyCycle = MIN_PWM; dutyCycle <= MAX_PWM; )
			{   
                // changing the LED brightness with PWM
                ledcWrite(ledChannel_R1, 0);
                ledcWrite(ledChannel_L1, dutyCycle);
                ledcWrite(ledChannel_R2, 0);
                ledcWrite(ledChannel_L2, dutyCycle);
                delay(100);
                dutyCycle+=STEP_SIZE;
                
				Serial.print("Start up phase...Backward         Duty = ");   Serial.println(dutyCycle);
                
				distance_chk_B= Backward_Distance_Check();
                if(distance_chk_B==true)
                       break;
            }
		Serial.println("Slow start backward...");
	}

void ESP_BOT_Custom :: Slow_Stop_Forward()
	{
        for(int dutyCycle = MAX_PWM; dutyCycle >= MIN_PWM; )
			{   
                // changing the LED brightness with PWM
                ledcWrite(ledChannel_R1, dutyCycle);
                ledcWrite(ledChannel_L1, 0);
                ledcWrite(ledChannel_R2, dutyCycle);
                ledcWrite(ledChannel_L2, 0);
                delay(100);
                dutyCycle-=STEP_SIZE;
                
				Serial.print("Slow stop Forward         Duty = ");   Serial.println(dutyCycle);

                distance_chk_F = Forward_Distance_Check();
                if(distance_chk_F==true)
                     break;
            }
        stopp();
        Serial.println("Slow STOP forward...");
	}

void ESP_BOT_Custom :: Slow_Stop_Backward()
	{
        for(int dutyCycle = MAX_PWM; dutyCycle >= MIN_PWM; )
			{    
                // changing the LED brightness with PWM
                ledcWrite(ledChannel_R1, 0);
                ledcWrite(ledChannel_L1, dutyCycle);
                ledcWrite(ledChannel_R2, 0);
                ledcWrite(ledChannel_L2, dutyCycle);
                dutyCycle-=STEP_SIZE;
                delay(100);
                
				Serial.print("Slow stop Backward         Duty = ");   Serial.println(dutyCycle);
              
                distance_chk_B= Backward_Distance_Check();
                if(distance_chk_B==true)
                     break;
            }
        stopp();
        Serial.println("Slow STOP backward...");
	}

void ESP_BOT_Custom :: stopp()
	{
		ledcWrite(ledChannel_R1, 0);
		ledcWrite(ledChannel_L1, 0);
		ledcWrite(ledChannel_R2, 0);
		ledcWrite(ledChannel_L2, 0);
		
		delay(100);
		Serial.println("In Stop");
		
		////////////////Bluetooth Functions Start/////////////////
			Serial.println("Taking picture...");
			SerialBT.print("Camera On");
			Serial.println("Camera On");
			delay(1000);
			String Response_To_Image = SerialBT.readString();
			Serial.print("Image condition is   = ");Serial.println(Response_To_Image);
		////////////////Bluetooth Functions Stop/////////////////
		BOT_Spray();
	}

void ESP_BOT_Custom :: BOT_Spray()
	{ 
		if ((msgReceived_BOT_Spray == true) && (Hour>=15))//&&(Minute>=15)))
		{
//			msgReceived_BOT_Spray = false;
			Serial.println("Spraying...");
			digitalWrite(Relay_Spray_ON, LOW);
			delay(2000);
			digitalWrite(Relay_Spray_ON, HIGH);
			delay(500);
		}
	}

void ESP_BOT_Custom :: BOT_Spray_charge() 
	{
		if (msgReceived_BOT_Spray_charge2 == false) 
		{
			msgReceived_BOT_Spray_charge2 = true; 
			Serial.println("Spray charging..Timer started only once.");
			digitalWrite(Relay_Spray_Charge, LOW);
			BOT_Spray_Charge_Timer.start();
			delay(500);
        }
	}
	
void ESP_BOT_Custom :: Flag_clear_BOT()                                                               // Pump activation flag is clearing after PUMP_ACT_TIME
	{
        if (msgReceived_BOT_Spray_charge2 == true) 
		{
			int flag_time_temp=int(BOT_Spray_Charge_Timer.value()/CONV_MS_SEC);
			if(flag_time_temp >= BOT_SPRAY_CHARGE_TIME)//&& (time_slot_match_pump_act == true))
    		{       
				BOT_Spray_Charge_Timer.stop();
    			BOT_Spray_Charge_Timer.reset();
				digitalWrite(Relay_Spray_Charge, HIGH);
				msgReceived_BOT_Spray_charge = false;
				msgReceived_BOT_Spray_charge2 = false;
    		}
		}
	}
long ESP_BOT_Custom :: measure_distance_Front() 
	{
		long duration, inches, cm;
		
		digitalWrite(trigPin_Front, LOW);
		delayMicroseconds(2);
		digitalWrite(trigPin_Front, HIGH);
		delayMicroseconds(10);
		digitalWrite(trigPin_Front, LOW);
		delayMicroseconds(2);
		
		duration = pulseIn(echoPin_Front, HIGH);
		
		cm = microsecondsToCentimeters(duration);
		//  cm = duration/ 29 / 2;
		return cm;
	}

long ESP_BOT_Custom :: measure_distance_Back()
	{
		long duration, inches, cm;
		
		digitalWrite(trigPin_Back, LOW);
		delayMicroseconds(2);
		digitalWrite(trigPin_Back, HIGH);
		delayMicroseconds(10);
		digitalWrite(trigPin_Back, LOW);
		delayMicroseconds(2);
		
		duration = pulseIn(echoPin_Back, HIGH);
		
		cm = microsecondsToCentimeters(duration);
		//  cm = duration/ 29 / 2;
		return cm;
	}


void ESP_BOT_Custom :: Read_Time_From_Mobiel()
{
	SerialBT.print("TIME");
	//Serial.println("Reading time from Mobile");
	delay(1000);
	if (SerialBT.available() != 0) 
	    {
			timerWrite(timer, 0);                                                 // reset timer (feed watchdog)  
			String Date_time = SerialBT.readString();
		//	Serial.print("Present Date time is  = ");Serial.println(Date_time);
			
			
			char *token,*token1;                                                              // Declare a character pointer variable token
             
            unsigned char temp_min, temp_hour, temp_date;			 
			
			char msg[Date_time.length()+1];                                                            // Declare an array of characters of length equal to length of the tmp_str+1

            Date_time.toCharArray(msg, Date_time.length()+1);
			
			String temp_input;
			
   			token   = strtok(msg, " : ");                                                      // Get the content of string seperated by space 
            token1  = strtok(NULL, ":");                                                     // Get the next remaining string into token 
            
			temp_input = token1;
			temp_date = (unsigned char)temp_input.toInt();
			
			token1  = strtok(NULL, ":");
			temp_input = token1; 
            temp_hour = (char)temp_input.toInt();
			
			token1  = strtok(NULL, ":");
			temp_input = token1; 
            temp_min = (unsigned char)temp_input.toInt();
			
	/*		Serial.print("Day_in_week = "); Serial.println(temp_date);
			Serial.print("Hour is = "); Serial.println(temp_hour);
			Serial.print("Min is = "); Serial.println(temp_min);
	*/		
			Hour = temp_hour;
			Minute = temp_min;
			Day_in_week = temp_date;
		}
		
		
}