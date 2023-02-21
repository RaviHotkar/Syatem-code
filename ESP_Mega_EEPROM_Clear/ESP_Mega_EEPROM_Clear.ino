#include <EEPROM.h>                    // For MEGA 4096 bytes & ESP 512 bytes

//#define MEGA_ENABLE 1                  // Comment this line while clearing the ESP32

#define EEPROM_SIZE_MEGA 4096
#define EEPROM_SIZE_ESP 512


void setup() 
{
  Serial.begin(9600);
  #ifdef MEGA_ENABLE

    for (int i = 0; i < EEPROM_SIZE_MEGA; i++) 
      {
        EEPROM.write(i,0);
      }
    Serial.print("Setup End ; Erase status for Mega... ");  
  
  #else
    
    int return_init  = EEPROM.begin(EEPROM_SIZE_ESP);
    Serial.print("Init End ; Erase status... ");Serial.println(return_init);
  
    for (int i = 0; i < EEPROM_SIZE_ESP; i++) 
      {
        EEPROM.write(i,0);
      }

    int return_commit = EEPROM.commit();                                    
    delay(500);
    Serial.print("Setup End ; Erase status... ");Serial.println(return_commit);
  #endif
}

void loop() 
{
  // put your main code here, to run repeatedly:
}
