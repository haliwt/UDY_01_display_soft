#include "bsp.h"





//static void TimeColon_Smg_Blink_Fun(void);


/**********************************************************************
*
*Functin Name: void Display_DHT11_Value(void)
*Function : Timer of key be pressed handle
*Input Ref:  key of value
*Return Ref: NO
*
**********************************************************************/
void Display_DHT11_Value(void)
{
    
   TM1639_Display_Temperature(run_t.ntc_tem[0]);//TM1639_Write_2bit_TempData(temp1,temp2);
    
	TM1639_Display_Health(run_t.timer_dispTime_minutes,run_t.timer_dispTime_hours);//TM1639_Write_2bit_HumData(hum1,hum2);
	


}  


/**********************************************************************
*
*Functin Name: void Display_Timing(uint8_t hours,uint8_t minutes)
*Function : Timer of key be pressed handle
*Input Ref:  key of value
*Return Ref: NO
*
**********************************************************************/
void Display_Timing(uint8_t hours,uint8_t minutes,uint8_t disp)
{ 
  
	TM1639_Display_4Bit_Time(hours,minutes) ; //timer is default 12 hours "12:00"


}
/*******************************************************
	*
	*Function Name: static void disp_ntc_temperature_value(void)
	*Function : display pannel display conetent
	*Input Ref:NO
	*Return Ref:NO
	*
*******************************************************/
void disp_ntc_temperature_value(void)
{

  if(run_t.gTimer_display_dht11 > 4){
	    run_t.gTimer_display_dht11=0;
       	Display_DHT11_Value();
       
     
	}

}

/********************************************************************************
*
*Functin Name: void Display_Error_Digital(uint8_t errnumbers,uint8_t sel)
*Function : Timer of key be pressed handle
*Input Ref:  error digital 
*Return Ref: NO
*
********************************************************************************/
void Display_Error_Digital(uint8_t sel)
{ 
 
	//TM1639_Display_4Bit_Time(m,run_t.hours_two_unit_bit,run_t.minutes_one_decade_bit,sel) ; //timer is default 12 hours "12:00"
    TM1639_Display_4Bit_Error(sel);

}

/********************************************************************************
*
*Functin Name: static void TimeColon_Smg_Blink_Fun(void)
*Function : Timer of key be pressed handle
*Input Ref:  NO
*Return Ref: NO
*
********************************************************************************/
//static void TimeColon_Smg_Blink_Fun(void)
//{
//	//if(run_t.gTimer_colon < 1){ //2
//	static uint8_t i;

//     i++ ;
//     if(i==1)
//		  SmgBlink_Colon_Function(run_t.hours_two_unit_bit ,run_t.minutes_one_decade_bit,0);
//     else{
//          i=0;
//	
//		   SmgBlink_Colon_Function(run_t.hours_two_unit_bit ,run_t.minutes_one_decade_bit,1);
//        }

//	
//}

/********************************************************************************
	*
	*Functin Name: void Display_TimeColon_Blink_Fun(void)
	*Function : 
	*Input Ref:  NO
	*Return Ref: NO
	*
********************************************************************************/








