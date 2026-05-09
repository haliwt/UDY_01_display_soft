#include "bsp.h"


void set_up_timer_timing_handler(void)
{
  
	//Set_TimerTiming_Number_Value();
	                   
}

/****************************************************************
	*
	*Function Name :void Set_Timing_Temperature_Number_Value(void)
	*Function : set timer timing how many ?
	*Input Parameters :NO
	*Retrurn Parameter :NO
	*
*****************************************************************/
void Set_TimerTiming_Number_Value(void)
{
  #if 0
  if(gpro_t.key_add_dec_pressed_flag == 1 && run_t.gTimer_set_timer_timing_value >2){
    	gpro_t.key_add_dec_pressed_flag = ++;
		
       
         //Display_Timing(run_t.timer_dispTime_hours,run_t.timer_dispTime_minutes,0);
	    // tx_thread_sleep(300);
		 TM1639_donotDisplay_4Bit_Time();
		 tx_thread_sleep(300);
		 Display_Timing(run_t.timer_dispTime_hours,run_t.timer_dispTime_minutes,0);

		
	}
  #endif 	
}

/***********************************************************************************
	 *
	 * Function Name:void set_timer_fun_led_blink(void)
	 * Function:
	 * Input Ref:NO
	 * Return Ref:NO
	 *
************************************************************************************/
void set_timer_fun_led_blink(void)
{
   static uint8_t time_smg_blink;

   if(gpro_t.key_add_dec_pressed_flag ==1 && gpro_t.gTimer_set_temp_counter  > 2){
  // if(gpro_t.key_add_dec_pressed_flag ==1 && gpro_t.gTimer_4bitsmg_blink_times  > 300){//if has a key be pressed "+" key or "-" key

     

    	TM1639_donotDisplay_4Bit_Time();
        tx_thread_sleep(300);
        TM1639_Display_4Bit_Time(run_t.timer_dispTime_hours,run_t.timer_dispTime_minutes);
	    tx_thread_sleep(300);
	    TM1639_donotDisplay_4Bit_Time();
	    tx_thread_sleep(300);
	    TM1639_Display_4Bit_Time(run_t.timer_dispTime_hours,run_t.timer_dispTime_minutes);

	 
	    gpro_t.key_add_dec_pressed_flag=0;

      	run_t.timer_dispTime_minutes=0;
	    run_t.gTimer_timer_seconds_counter =0;

      

   	}

 }
 /***********************************************************************************
	 *
	 * Function Name:void Display_SmgTiming_Value(void)
	 * Function:
	 * Input Ref:NO
	 * Return Ref:NO
	 *
************************************************************************************/
void Display_SmgTiming_Value(void)
{

	if(run_t.gTimer_timer_seconds_counter > 59){
		run_t.gTimer_timer_seconds_counter =0;

		run_t.timer_dispTime_minutes -- ;
	    run_t.disp_health_minutes++;

		if(run_t.timer_dispTime_minutes <  0 ){

		run_t.timer_dispTime_hours -- ;
		run_t.timer_dispTime_minutes =59;

		}



		if(run_t.timer_dispTime_hours < 0 ){

		run_t.gTimer_timer_seconds_counter = 57 ;
		run_t.timer_dispTime_hours=0;
		run_t.timer_dispTime_minutes=0;


		SendData_PowerOnOff(0);//power off
		tx_thread_sleep(10);

		}
		Display_Timing(run_t.timer_dispTime_hours,run_t.timer_dispTime_minutes,0);
	}

	//if(gpro_t.gTimer_disp_timer_counter > 1){
	//gpro_t.gTimer_disp_timer_counter=0;
	//Display_Timing(run_t.timer_dispTime_hours,run_t.timer_dispTime_minutes,0);

	//}


}

