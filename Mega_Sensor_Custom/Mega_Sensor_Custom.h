#ifndef Mega_Sensor_Custom_h
#define Mega_Sensor_Custom_h

class Mega_Sensor_Custom
{
	public 	  :			
				void Sensor_Setup();
				void Sensor_Loop();
				void Soil_Sensor_Handling();
				void Environment_Sensor_Handling();
				void Light_Sensor_Handling();
				void Sensor_read_timer_handle() ;
				
	private   :			
				
	protected : 
};

#endif