#ifndef __BSP_SMG_APP_H
#define __BSP_SMG_APP_H
#include "main.h"


void Display_DHT11_Value(void);

//void DisplaySMG_LED(void);




void Display_Timing(uint8_t hours,uint8_t minutes,uint8_t disp);


void Display_Error_Digital(uint8_t sel);

void disp_ntc_temperature_value(void);


void Display_TimeColon_Blink_Fun(void);







#endif 

