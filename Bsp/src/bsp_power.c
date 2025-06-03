#include "bsp.h"

RUN_T run_t;



uint8_t power_on_off_flag;

void Power_Off(void);


/**********************************************************************
*
*Functin Name: void Display_DHT11_Value(void)
*Function : Timer of key be pressed handle
*Input Ref:  key of value
*Return Ref: NO
*
**********************************************************************/
void Power_Off(void)
{
    
    power_off_led();//SMG_POWER_OFF()	;
	run_t.gPlasma=0;
	run_t.gDry=0;
	run_t.gMouse =0;
	
			
}


/**********************************************************************
*
*Functin Name: void Receive_ManiBoard_Cmd(uint8_t cmd)
*Function :  wifi recieve data
*Input Ref:  receive wifi send order
*Return Ref: NO
*
**********************************************************************/
void Power_On_Fun(void)
{
    run_t.gPlasma=1;
	run_t.gDry =1;
    run_t.gMouse = 1;
		
    power_on_led();

	run_t.fan_warning=0;
	run_t.ptc_warning=0;
 
	
    run_t.gTimer_timer_seconds_counter=0;
    gpro_t.set_timer_timing_value_success =0 ;
    gpro_t.set_temp_value_success = 0;
    run_t.timer_dispTime_hours =0;
	run_t.timer_dispTime_minutes=0;
		
     //TM1639_Write_4Bit_Time(hour_decade,run_t.hours_two_unit_bit,run_t.minutes_one_decade_bit,minutes_two,0);
	 TM1639_Display_4Bit_Time(run_t.works_dispTime_hours,run_t.works_dispTime_minutes);
     //Display_DHT11_Value(); //WT.EIDT 2025.05.10
    
}


/************************************************************************
	*
	*Function Name: static void Power_Off_Fun(void)
	*
	*
	*
	*
************************************************************************/
void Power_Off_Fun(void)
{
	
 
		run_t.gPlasma=0;
		run_t.gDry =0;
		run_t.gMouse = 0;
		
} 

void power_on_handler(void)
{

	run_t.gTimer_set_temp_times=0; //conflict with send temperatur value

	run_t.gPower_On = power_on;
	run_t.power_on_step =0;
	run_t.power_off_flag = 0;
	
}

void power_off_handler(void)
{

    run_t.gPower_On = power_off;
	run_t.power_on_step =0;

 }

/******************************************************************************
	*
	*Function Name:void RunPocess_Command_Handler(void)
	*Funcion: display pannel run of process 
	*Input Ref: NO
	*Return Ref:NO
	*
******************************************************************************/
void power_on_run_handler(void)
{

    static uint8_t  step_state;
   switch(run_t.power_on_step){

      case 0:
          
	  
           run_t.gTimer_time_colon =0;
	       run_t.set_temperature_decade_value=40;
           
			Power_On_Fun();
			run_t.gTimer_display_dht11 = 20; //at once display temperature and humidity value.
			gpro_t.set_timer_timing_doing_value = 0;
            gpro_t.g_manual_shutoff_dry_flag = 0; //allow open dry function .
      

			gpro_t.set_timer_timing_value_success=0;
			run_t.timer_dispTime_hours=0;
		    run_t.timer_dispTime_minutes=0;

		
			 run_t.works_dispTime_hours=0;
			  run_t.works_dispTime_minutes=0;
			  run_t.gTimer_timing_seconds_counter =0;

			
			gpro_t.set_temp_value_success=0;
			run_t.power_off_flag =0;
			TM1639_Display_ON_OFF(1);
			run_t.power_on_step= 1;


            
	  break;

      case 1:

          if(gpro_t.set_timer_timing_doing_value == 1 && run_t.ptc_warning ==0 && run_t.fan_warning ==0){

                   Set_TimerTiming_Number_Value();
                   
           }
           else if((gpro_t.set_timer_timing_doing_value == 0 ||gpro_t.set_timer_timing_doing_value == 3 )&&  run_t.set_temperature_special_flag   >0 &&  run_t.set_temperature_special_flag != 0xff ){

                   disp_smg_blink_set_tempeature_value();
	              
						
           }
           else{

              switch(step_state){

					case 0:
						Led_Panel_OnOff();
					    step_state=1;
					break;
                    
                    case 1: //display 1:   timing times  2: timer times.

				       if(gpro_t.set_timer_timing_doing_value==0 || gpro_t.set_timer_timing_doing_value==3 \
					   	   &&  run_t.ptc_warning ==0 && run_t.fan_warning ==0 ){ //WT.EDIT 2025.05.07
                        if(run_t.ptc_warning ==0 && run_t.fan_warning ==0){ //read main board ptc_warning of ref.
                            
							   Display_SmgTiming_Value();

                            

                         }
                        else{

                            Warning_Error_Numbers_Fun();

                        }
                        
                        }

                     step_state=1;
                    break;

              }
             	}
            
      break;

	}
}


void detected_ptc_or_fan_warning_fun(void)
{

    Warning_Error_Numbers_Fun();

}

/******************************************************************************
	*
	*Function Name:void mode_key_long_fun(void)
	*Funcion: exit this mode set fun ,
	*Input Ref: NO
	*Return Ref:NO
	*
******************************************************************************/


/******************************************************************************
	*
	*Function Name:void RunPocess_Command_Handler(void)
	*Funcion: display pannel run of process 
	*Input Ref: NO
	*Return Ref:NO
	*
******************************************************************************/
void power_off_run_handler(void)
{
   
    switch(run_t.power_off_flag){
     case 0://2
	  
		 run_t.ptc_warning =0;
		 run_t.fan_warning =0;
		 run_t.power_on_step=0;
          power_off_led();

		   gpro_t.set_timer_timing_value_success=0;
			run_t.timer_dispTime_hours=0;
		    run_t.timer_dispTime_minutes=0;
			
         run_t.power_off_flag = 1;
       break;

       case 1://4


           
		  
           LED_Power_Breathing();
		 
		 break;
       }

}





/*******************************************************
*
*Function Name: void compare_temp_value()
*Function :
*
*
*******************************************************/


