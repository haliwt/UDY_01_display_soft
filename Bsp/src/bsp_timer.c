#include "bsp.h"



/***********************************************************************************
	 *
	 * Function Name:void set_timer_fun_led_blink(void)
	 * Function:
	 * Input Ref:NO
	 * Return Ref:NO
	 *
************************************************************************************/
#if 0
void set_timer_fun_led_blink(void)
{
   static uint8_t time_smg_blink;

   if(gpro_t.key_add_dec_pressed_flag ==1 && gpro_t.gTimer_set_timer_counter  > 2){
  

        TM1639_donotDisplay_4Bit_Time();
        tx_thread_sleep(30);//300ms
        TM1639_Display_4Bit_Time(run_t.timer_dispTime_hours,run_t.timer_dispTime_minutes);
	    tx_thread_sleep(30);
	    TM1639_donotDisplay_4Bit_Time();
	    tx_thread_sleep(30);
	    TM1639_Display_4Bit_Time(run_t.timer_dispTime_hours,run_t.timer_dispTime_minutes);

	 
	    gpro_t.key_add_dec_pressed_flag=0;
      
      	run_t.timer_dispTime_minutes=0;
	    run_t.gTimer_timer_seconds_counter =0;

      

   	}

 }
#endif 
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

        run_t.gPower_On = power_off;
		SendData_PowerOnOff(0);//power off
		//tx_thread_sheep(10);

		}
		Display_Timing(run_t.timer_dispTime_hours,run_t.timer_dispTime_minutes,0);
	}

	
}

