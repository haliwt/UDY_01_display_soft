#include "bsp.h"


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
  static uint8_t default_numbers =0xff;
   if(gpro_t.set_timer_timing_doing_value==1){
   //set timer timing value 
    if(run_t.gTimer_key_timing > 3){
		run_t.gTimer_key_timing =0;		
		gpro_t.set_timer_timing_doing_value ++ ;
	
	 }

    }

    if(gpro_t.set_timer_timing_doing_value==2){
    	gpro_t.set_timer_timing_doing_value++;
		if(gpro_t.set_timer_timing_value_success  == TIMER_SUCCESS && gpro_t.key_add_dec_pressed_flag ==0){// ADD and DEC key don't be pressed
             //run_t.hours_two_decade_bit = run_t.timer_dispTime_hours/10,
        	 //run_t.hours_two_unit_bit  = run_t.timer_dispTime_hours %10;
        	 //run_t.minutes_one_decade_bit = run_t.timer_dispTime_minutes /10;
        	 //run_t.minutes_one_unit_bit = run_t.timer_dispTime_minutes %10;
        	 Display_Timing(run_t.timer_dispTime_hours,run_t.timer_dispTime_minutes,0);

		}
		else if(run_t.temporary_timer_dispTime_hours >0 && gpro_t.key_add_dec_pressed_flag ==1){// ADD and DEC key be pressed
			gpro_t.set_timer_timing_value_success  = TIMER_SUCCESS;
			

			run_t.timer_dispTime_hours = run_t.temporary_timer_dispTime_hours ;
	        run_t.timer_dispTime_minutes = 0;
			run_t.gTimer_timer_seconds_counter = 0;

			Display_Timing(run_t.timer_dispTime_hours,run_t.timer_dispTime_minutes,0);


		}
		else{

			gpro_t.set_timer_timing_value_success  = 0;



		}


    }
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
 
   if(gpro_t.set_timer_timing_doing_value==1){

     if(gpro_t.key_add_dec_pressed_flag ==1 && gpro_t.gTimer_4bitsmg_blink_times  > 300){//if has a key be pressed "+" key or "-" key

    	gpro_t.gTimer_4bitsmg_blink_times =0;
        time_smg_blink = time_smg_blink ^ 0x01;
	    if(time_smg_blink == 1){
       // TM1639_Write_4Bit_Time_sync_close(run_t.hours_two_decade_bit,run_t.hours_two_unit_bit, run_t.minutes_one_decade_bit,run_t.minutes_one_unit_bit,time_smg_blink) ;
	      TM1639_Display_4Bit_Time(run_t.timer_dispTime_hours,run_t.timer_dispTime_minutes);

	   }
	   else{
	   	TM1639_donotDisplay_4Bit_Time();
	   }
   	
     }
     else if(gpro_t.key_add_dec_pressed_flag ==0 && gpro_t.gTimer_4bitsmg_blink_times  > 300){// don't key be pressd,only display timer timing value 
       gpro_t.gTimer_4bitsmg_blink_times =0;

       time_smg_blink = time_smg_blink ^ 0x01;

	    if(time_smg_blink == 1){
       // TM1639_Write_4Bit_Time_sync_close(run_t.hours_two_decade_bit,run_t.hours_two_unit_bit, run_t.minutes_one_decade_bit,run_t.minutes_one_unit_bit,time_smg_blink) ;
	      TM1639_Display_4Bit_Time(run_t.timer_dispTime_hours,run_t.timer_dispTime_minutes);

	   }
	   else{
	   	TM1639_donotDisplay_4Bit_Time();
	   }

       if(gpro_t.set_timer_timing_value_success==0){

      	run_t.timer_dispTime_hours=0;
      	run_t.timer_dispTime_minutes=0;

      	

      	}
         else{
        	// run_t.hours_two_decade_bit = run_t.timer_dispTime_hours/10,
        	/// run_t.hours_two_unit_bit  = run_t.timer_dispTime_hours %10;
        	// run_t.minutes_one_decade_bit = run_t.timer_dispTime_minutes /10;
        	// run_t.minutes_one_unit_bit = run_t.timer_dispTime_minutes %10;
        	 //TM1639_Write_4Bit_Time_sync_close(run_t.hours_two_decade_bit,run_t.hours_two_unit_bit, run_t.minutes_one_decade_bit,run_t.minutes_one_unit_bit,time_smg_blink) ;
		     TM1639_Display_4Bit_Time(run_t.timer_dispTime_hours,run_t.timer_dispTime_minutes);

         }
     
       
    }

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
	 switch(gpro_t.set_timer_timing_value_success){

	   case TIMER_SUCCESS:
           if(run_t.gTimer_timer_seconds_counter > 59){
			    run_t.gTimer_timer_seconds_counter =0;
			
				run_t.timer_dispTime_minutes -- ;
			
			    if(run_t.timer_dispTime_minutes <  0 ){
					 
				   run_t.timer_dispTime_hours -- ;
				   run_t.timer_dispTime_minutes =59;
				
				  // uint8_t dataToSend[3] = {run_t.timer_dispTime_hours,run_t.timer_dispTime_minutes, run_t.gTimer_timer_seconds_counter}; // 要发送的 3 个数据
           		  // SendData_ToMainboard_Data(0x5C, dataToSend, 3); // cmd=0x1A, 数据长度=3
                  //  osDelay(5);
		         }

				
				
				 if(run_t.timer_dispTime_hours < 0 ){
				 
					run_t.gTimer_timer_seconds_counter = 57 ;
					run_t.timer_dispTime_hours=0;
					run_t.timer_dispTime_minutes=0;
		             
			        gpro_t.send_ack_cmd = check_ack_power_off;//ack_power_off;
					gpro_t.gTimer_again_send_power_on_off =0;
					SendData_PowerOnOff(0);//power off
					
			      }
		}

        Display_Timing(run_t.timer_dispTime_hours,run_t.timer_dispTime_minutes,0);
       // WorksTime_DonotDisplay_Fun();
        
	    break;

		case TIMER_NORMAL_TIMING: //NO_AI_MODE by timer timing  auto be changed AI_MODE
			
    
          if(run_t.gTimer_timing_seconds_counter > 59){
    		   run_t.gTimer_timing_seconds_counter=0;
    		 
    		   run_t.works_dispTime_minutes++; //1 minute 
    		
    		 
    		   if(run_t.works_dispTime_minutes> 59){ //1 hour
    		   run_t.works_dispTime_minutes=0;
    		   run_t.works_dispTime_hours++;
    		   if(run_t.works_dispTime_hours > 99){ //works time timing is 100 hours.
    		        run_t.works_dispTime_hours =0;
    		   }
    	      }
           }
  
            Display_Timing(run_t.works_dispTime_hours,run_t.works_dispTime_minutes,0);
           // Display_Works_Time_Fun();
           // Timer_Timing_Donot_Display();
		break;

	   	}


}

