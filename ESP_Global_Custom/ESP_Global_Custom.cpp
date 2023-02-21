#include "SPIFFS.h"
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <Esp.h>

#include "EEPROM.h"

#include "ESP_AWS_Custom.h"
#include "ESP_BOT_Custom.h"
#include "ESP_EEPROM_Custom.h"
#include "ESP_Global_Custom.h"
#include "ESP_Mega_Custom.h"
#include "ESP_Sensor_Custom.h"


void ESP_Global_Custom ::  Initialize_Global_Variables()
{
	
	#ifdef BOT_ENABLE
	
	    Serial.println("BOT variables are being initialized....");
	
	//-----PUMP1(Move the BOT) initialization----BEGIN----
	    Hour1[0]   = 7; 
	     Min1[0]   = 30;
	    Hour2[0]   = 7; 
	     Min2[0]   = 30;
	    Hour3[0]   = 16; 
	     Min3[0]   = 30;
        Hour4[0]   = 16; 
         Min4[0]   = 30;
        
        Day0[0] = 0;                                            
        Day1[0] = 1;
        Day2[0] = 2;
        Day3[0] = 3;
        Day4[0] = 4;
        Day5[0] = 5;
        Day6[0] = 6;
        
        Week_Freq[0]   = 1 ; 
        On_Off_sequence[0]  = 1 ;
    //-----PUMP1(Move the BOT) initialization----END----	
	
	//-----PUMP2(Realay 1 for Spray) initialization----BEGIN----
	    Hour1[1]   = 6; 
	     Min1[1]   = 0;
	    Hour2[1]   = 9; 
	     Min2[1]   = 0;
	    Hour3[1]   = 12;  
	     Min3[1]   = 0;
        Hour4[1]   = 15;  
         Min4[1]   = 0;		
        
        Day0[1] = 1;                                            
        Day1[1] = 1;
        Day2[1] = 3;
        Day3[1] = 3;
        Day4[1] = 3;
        Day5[1] = 5;
        Day6[1] = 5;
        
        Week_Freq[1]   = 1 ; 
        On_Off_sequence[1]  = 1 ;
    //-----PUMP2(Realay 1 for Spray) initialization----END----	
	
	//-----PUMP3(Realay 2 for Spray Charger) initialization----BEGIN----
        Hour1[2]   = 6; 
         Min1[2]   = 0;
        Hour2[2]   = 6; 
         Min2[2]   = 0;
        Hour3[2]   = 6; 
         Min3[2]   = 0;
        Hour4[2]   = 6; 
         Min4[2]   = 0;	
        
        Day0[2] = 2;                                            
        Day1[2] = 2;
        Day2[2] = 2;
        Day3[2] = 4;
        Day4[2] = 4;
        Day5[2] = 6;
        Day6[2] = 6;
        
        Week_Freq[2]   = 1 ; 
        On_Off_sequence[2]  = 1 ;
    //-----PUMP3(Realay 2 for Spray Charger) initialization----END----	
	
	#else 
	//-----PUMP11 initialization----BEGIN----
        Serial.println("System variables are being initialized....");
		
	    Hour1[10]   = 6; 
	     Min1[10]   = 0;
	    Hour2[10]   = 9; 
	     Min2[10]   = 0;
	    Hour3[10]   = 12; 
	     Min3[10]   = 0;
        Hour4[10]   = 15; 
         Min4[10]   = 0;	
        
        Day0[10] = 0;                                            
        Day1[10] = 1;
        Day2[10] = 2;
        Day3[10] = 3;
        Day4[10] = 4;
        Day5[10] = 5;
        Day6[10] = 6;
        
        Week_Freq[10]   = 1 ; 
        On_Off_sequence[10]  = 1 ;
    //-----PUMP11 initialization----END----	
	
	//-----PUMP16 initialization----BEGIN----
	    Hour1[15]   = 6; 
	     Min1[15]   = 0;
	    Hour2[15]   = 10; 
	     Min2[15]   = 0;
	    Hour3[15]   = 18; 
	     Min3[15]   = 0;
        Hour4[15]   = 22; 
         Min4[15]   = 0;	
        
        Day0[15] = 0;                                           
        Day1[15] = 1;
        Day2[15] = 2;
        Day3[15] = 3;
        Day4[15] = 4;
        Day5[15] = 5;
        Day6[15] = 6;
        
        Week_Freq[15]   = 1 ; 
        On_Off_sequence[15]  = 1 ;
    //-----PUMP16 initialization----END----	
	
	//-----PUMP17 initialization----BEGIN----
        Hour1[16]   = 6; 
         Min1[16]   = 3;
        Hour2[16]   = 10; 
         Min2[16]   = 3;
        Hour3[16]   = 18; 
         Min3[16]   = 3;
        Hour4[16]   = 22; 
         Min4[16]   = 3;	
        
        Day0[16] = 0;                                           
        Day1[16] = 1;
        Day2[16] = 2;
        Day3[16] = 3;
        Day4[16] = 4;
        Day5[16] = 5;
        Day6[16] = 6;
        
        Week_Freq[16]   = 1 ; 
        On_Off_sequence[16]  = 1 ;
    //-----PUMP17 initialization----END----			
	
	
	//-----PUMP18 initialization----BEGIN----
        Serial.println("System variables are being initialized....");
		
	    Hour1[17]   = 6; 
	     Min1[17]   = 6;
	    Hour2[17]   = 6; 
	     Min2[17]   = 6;
	    Hour3[17]   = 18; 
	     Min3[17]   = 6;
        Hour4[17]   = 18; 
         Min4[17]   = 6;	
        
        Day0[17] = 0;                                            
        Day1[17] = 1;
        Day2[17] = 2;
        Day3[17] = 3;
        Day4[17] = 4;
        Day5[17] = 5;
        Day6[17] = 6;
        
        Week_Freq[17]   = 1 ; 
        On_Off_sequence[17]  = 0 ;
    //-----PUMP18 initialization----END----	
	
	//-----PUMP19 initialization----BEGIN----
	    Hour1[18]   = 6; 
	     Min1[18]   = 9;
	    Hour2[18]   = 6; 
	     Min2[18]   = 9;
	    Hour3[18]   = 18; 
	     Min3[18]   = 9;
        Hour4[18]   = 18; 
         Min4[18]   = 9;	
        
        Day0[18] = 0;                                            
        Day1[18] = 1;
        Day2[18] = 2;
        Day3[18] = 3;
        Day4[18] = 4;
        Day5[18] = 5;
        Day6[18] = 6;
        
        Week_Freq[18]   = 1 ; 
        On_Off_sequence[18]  = 0 ;
    //-----PUMP19 initialization----END----	
	
	//-----PUMP20 initialization----BEGIN----
        Hour1[19]   = 6; 
         Min1[19]   = 12;
        Hour2[19]   = 9; 
         Min2[19]   = 12;
        Hour3[19]   = 18; 
         Min3[19]   = 12;
        Hour4[19]   = 18; 
         Min4[19]   = 12;	
        
        Day0[19] = 0;                                           
        Day1[19] = 1;
        Day2[19] = 2;
        Day3[19] = 3;
        Day4[19] = 4;
        Day5[19] = 5;
        Day6[19] = 6;
        
        Week_Freq[19]   = 1 ; 
        On_Off_sequence[19]  = 0 ;
    //-----PUMP20 initialization----END----			
	
	
	
	#endif
	
}