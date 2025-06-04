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
    
   TM1639_Display_Temperature(run_t.gReal_humtemp[1]);//TM1639_Write_2bit_TempData(temp1,temp2);
    
	TM1639_Display_Humidity(run_t.gReal_humtemp[0]);//TM1639_Write_2bit_HumData(hum1,hum2);
	


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
	*Function Name: static void disp_dht11_value(void)
	*Function : display pannel display conetent
	*Input Ref:NO
	*Return Ref:NO
	*
*******************************************************/
void disp_dht11_value(void)
{

  if(run_t.gTimer_display_dht11 > 5 && key_t.key_set_temperature_flag !=1){
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
void Warning_Error_Numbers_Fun(void)
{

    static uint8_t alternate_flag,times;

	  if(run_t.gTimer_error_digital > 2){//10ms * 51= 510
	       run_t.gTimer_error_digital=0;

      if(run_t.ptc_warning ==1 && run_t.fan_warning ==1)alternate_flag =1;
       else alternate_flag =0;


		gpro_t.g_time_disp_colon_flag = gpro_t.g_time_disp_colon_flag ^ 0x01;
 
		 

		    times = times ^ 0x01;
		
            if(alternate_flag ==0){
				
			   	
                 if(run_t.ptc_warning ==1){
                 
					Display_Error_Digital(0x01);
			     }
				 else if(run_t.fan_warning ==1){

					  
                    Display_Error_Digital(0x02);
                     

			      }

				 
			  }
			  else if(alternate_flag ==1){

			      
                  if(times ==1){

				   if(run_t.ptc_warning ==1){ // && run_t.fan_warning ==1){
                       
                    
                        Display_Error_Digital(0x01);

				   	}
				    

                   }
				   else{

				         if(run_t.fan_warning ==1){
										 
						   Display_Error_Digital(0x02);
						}



				   }
				


			   }
			   

		   }
		   

                
 	
		   
}







