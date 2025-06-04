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
void power_on_init(void)
{
    run_t.gPlasma=1;
	run_t.gDry =1;
    run_t.gMouse = 1;
		
    power_on_led();

	run_t.fan_warning=0;
	run_t.ptc_warning=0;
 

	gpro_t.set_timer_timing_doing_value = 0;
	gpro_t.g_manual_shutoff_dry_flag = 0; //allow open dry function .
    gpro_t.set_timer_timing_value_success=0;
	
	gpro_t.set_temp_value_success=0;
	gpro_t.temperature_init_value=0; //the first set up temperature value from 20:40
	
    run_t.gTimer_time_colon =0;
	//timer time
	run_t.gTimer_timer_seconds_counter=0;
	run_t.timer_dispTime_hours=0;
	run_t.timer_dispTime_minutes=0;
	
	//works time
    run_t.gTimer_timing_seconds_counter =0;
	run_t.works_dispTime_hours =0;
	run_t.works_dispTime_minutes =0;

	run_t.gTimer_display_dht11 = 20; //at once display temperature and humidity value.
    run_t.power_off_flag =0;
		
    TM1639_Display_4Bit_Time(run_t.works_dispTime_hours,run_t.works_dispTime_minutes);
     
    
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
          
	  power_on_init();
			
	   TM1639_Display_ON_OFF(1);
		run_t.power_on_step= 1;


            
	  break;

      case 1:


		   if(run_t.ptc_warning ==0 && run_t.fan_warning ==0){

		   if(gpro_t.set_timer_timing_doing_value==0 || gpro_t.set_timer_timing_doing_value==3){ //WT.EDIT 2025.05.07
		    { //read main board ptc_warning of ref.

				 Display_SmgTiming_Value();

				}
			 }
		   	}
			else{

			 Warning_Error_Numbers_Fun();

			 }
                        
         

            step_state=1;
       break;




	}
}

/******************************************************************************
	*
	*Function Name:void detected_ptc_or_fan_warning_fun(void)
	*Funcion: display ptc warning and fan warning
	*Input Ref: NO
	*Return Ref:NO
	*
******************************************************************************/
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


