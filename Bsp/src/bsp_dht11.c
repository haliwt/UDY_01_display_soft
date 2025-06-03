#include "bsp.h"


/****************************************************************
	*
	*Function Name :void disp_smg_blink_set_tempeature_value(void)
	*Function : set timer timing how many ?
	*Input Parameters :NO
	*Retrurn Parameter :NO
	*
*****************************************************************/
void disp_smg_blink_set_tempeature_value(void)
{
     static uint8_t counter_times;
	  //waiting for 4 s 
	  if(run_t.gTimer_key_temp_timing > 1 && run_t.set_temperature_special_flag ==1 && (gpro_t.set_timer_timing_doing_value==0 || gpro_t.set_timer_timing_doing_value==3)){
			
			
			run_t.set_temperature_special_flag ++;
			run_t.gTimer_set_temp_times =0; //couter time of smg blink timing 

	 }
	 //temperature of smg of LED blink .
	  if(run_t.set_temperature_special_flag ==2 && (gpro_t.set_timer_timing_doing_value==0 ||gpro_t.set_timer_timing_doing_value==3)){
	  	
	  	 
		  if(run_t.gTimer_set_temp_times  > 0  && run_t.set_temperature_special_flag !=0xff){ // 15ms * 4 =60ms
                 run_t.gTimer_set_temp_times=0;
                 counter_times++ ;  
// cancel display temperature SMG number led blink function.
//                 every_times ++;
//          if(every_times ==1){
//               
//		        TM1639_Write_2bit_SetUp_TempData(run_t.set_temperature_decade_value,run_t.set_temperature_unit_value,1);
//          }
//		  else{
//		  	   every_times=0;
			  //TM1639_Write_2bit_SetUp_TempData(run_t.set_temperature_decade_value,run_t.set_temperature_unit_value,0);
              TM1639_Display_Temperature(gpro_t.set_up_temperature_value);
		  }
          if(counter_times > 1){
			 
           		counter_times=0;
          
			 gpro_t.set_temp_value_success=1;
			 
	         run_t.set_temperature_special_flag =0xff;

			  run_t.gTimer_display_dht11 = 0;
		   
			  //TM1639_Write_2bit_SetUp_TempData(run_t.set_temperature_decade_value,run_t.set_temperature_unit_value,0);
		      
			  Display_DHT11_Value();
              gpro_t.g_manual_shutoff_dry_flag=0; //WT.EDIT 2025.05.28

		
			
			  
              
             }
		  
	     }


}

