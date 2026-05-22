#include "bsp.h"







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

   static uint8_t temp_disp =0 ;
   
   if(gpro_t.power_on_f ==1 && run_t.ntc_tem[0] >0){
         gpro_t.power_on_f ++;
	     temp_disp  = run_t.ntc_tem[0];
   }

	if(temp_disp < run_t.ntc_tem[0]){

        temp_disp ++ ;
	}
	else if(temp_disp > run_t.ntc_tem[0]){

          temp_disp -- ;
   }

   TM1639_Display_Temperature(temp_disp);//TM1639_Write_2bit_TempData(temp1,temp2);
    
}  

void disp_health_index(void)
{

  TM1639_Display_Health(run_t.disp_health_minutes);

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

  if(gpro_t.disp_set_temp_f ==1 &&  gpro_t.gTimer_set_timer_counter <4){

       TM1639_Display_Temperature(run_t.set_temperature_value);
	   disp_health_index();

  }
  else if(gpro_t.disp_set_temp_f ==1 &&  gpro_t.gTimer_set_timer_counter > 3){
       gpro_t.disp_set_temp_f ++;
       Display_DHT11_Value();
  }
  else if(run_t.gTimer_disp_ntc > 2 && gpro_t.disp_set_temp_f !=1){
	    run_t.gTimer_disp_ntc=0;
		
		Display_DHT11_Value();
       
  }

  if(gpro_t.gTimer_disp_health_counter > 15){
	   gpro_t.gTimer_disp_health_counter=0;

       disp_health_index();

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
/*************************************************************************
*
*Function Name:void SmgBlink_Colon_Function(uint8_t twobit,uint8_t threebit,uint8_t sel)
*Function :display smg of colon ":" blink 
*Input Ref:
*
*
*************************************************************************/









