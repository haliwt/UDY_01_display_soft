#include "bsp.h"

RUN_T run_t;



uint8_t power_on_off_flag;

static void Power_Off(void);
static void power_on_init(void);


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
static void power_on_init(void)
{
    run_t.gPlasma=1;
	run_t.gDry =1;
    run_t.gMouse = 1;
	
	LED_TIME_Init();	
    power_on_led();
	
    gpro_t.key_add_dec_pressed_flag=0;
   
	run_t.gTimer_disp_ntc =7;
    run_t.gTimer_time_colon =0;
	//timer time
   
	run_t.timer_dispTime_hours=8;
	run_t.timer_dispTime_minutes=0;
	run_t.gTimer_timer_seconds_counter=0;
	 run_t.disp_health_minutes=0;
	
	// Temperature setting initialization
	run_t.set_temperature_value = 50; // Default temperature 50°C (maximum)
	run_t.temp_setting_mode = 0; // 0: normal mode, 1: temperature setting mode
	run_t.ptc_open_f = 0; // PTC heating off by default
	
	//works time

    run_t.power_off_flag =0;
		
    TM1639_Display_4Bit_Time(run_t.timer_dispTime_hours,run_t.timer_dispTime_minutes);
     
    
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
void disp_set_timer_fun(void)
{

	// Check for timer setting timeout (3 seconds)
	if(gpro_t.set_timer_timing_doing_value == 1 && (gpro_t.gTimer_set_temp_counter >= 3)){

		// Switch to temperature setting mode
		gpro_t.set_timer_timing_doing_value = 0;
		run_t.temp_setting_mode = 1;
		gpro_t.gTimer_set_temp_counter = 0;
		// Display the current temperature setting
		TM1639_Display_4Bit_Temp(run_t.set_temperature_value);
		SendData_Buzzer();

	}
	else if(gpro_t.key_add_dec_pressed_flag !=1 && gpro_t.set_timer_timing_doing_value != 1){ //WT.EDIT 2025.05.07
	{ //read main board ptc_warning of ref.

		Display_SmgTiming_Value();

	}
}

void power_on_init_reference(void)
{
    if(run_t.power_on_step==0){
	  power_on_init();
			
	   TM1639_Display_ON_OFF(1);
	   
	  
	   run_t.power_on_step= 1;
     }


}
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
*Function : Compare environmental temperature with set temperature
*           and control LED_DRY accordingly
*
*******************************************************/
void immediately_compare_temp_value(void)
{
    // If set temperature is >= 50°C, must turn off heating immediately
    if(run_t.ptc_open_f == 1){
        run_t.ptc_open_f = 0;
        if(run_t.ntc_tem[0] < run_t.set_temperature_value) {
            // Turn on heating
            LED_DRY_ON();
            // Send command to mainboard to turn on heating
            SendData_Set_Command(0x02, 0x01);
            tx_thread_sleep(10);
          
        }
		else{
             // Turn on heating
            LED_DRY_OFF();
            // Send command to mainboard to turn on heating
            SendData_Set_Command(0x02, 0x0);
            tx_thread_sleep(10);
        }


	}
}

/****************************************************************
	*
	*Function Name :void compare_temp_value(void) 
	*Function : 
	*Input Parameters :NO
	*Retrurn Parameter :NO
	*
*****************************************************************/
void compare_temp_value(void)  
{
    if(run_t.ntc_tem[0] >= 50) {
        
            // Turn off heating
            LED_DRY_OFF();
			run_t.ptc_open_times_counter = 1;
            // Send command to mainboard to turn off heating
            SendData_Set_Command(0x02, 0x00);
            tx_thread_sleep(10);
            // Update flag
          
        
        return;
    }
    
    // If ptc_open_f is 0 (first comparison or currently off), compare immediately
    if(run_t.ptc_open_times_counter == 0) {
        if(run_t.ntc_tem[0] < run_t.set_temperature_value) {
            // Turn on heating
            LED_DRY_ON();
			run_t.ptc_open_times_counter = 1;
            // Send command to mainboard to turn on heating
            SendData_Set_Command(0x02, 0x01);
            tx_thread_sleep(10);
            // Update flag
           
        }
		else{
           // Turn on heating
            LED_DRY_OFF();
            // Send command to mainboard to turn on heating
            SendData_Set_Command(0x02, 0);
            tx_thread_sleep(10);
            // Update flag


		}
        // If temperature is already >= set temperature, keep off
    } 
	else {
        // If ptc_open_f is 1 (currently on), use hysteresis of -2°C
        if(run_t.ntc_tem[0] >= run_t.set_temperature_value) {
            // Turn off heating when temperature reaches set value
            LED_DRY_OFF();
            // Send command to mainboard to turn off heating
            SendData_Set_Command(0x02, 0x00);
            tx_thread_sleep(10);
            // Update flag
            run_t.ptc_open_f = 0;
        } 
		else if(run_t.ntc_tem[0] < (run_t.set_temperature_value - 2)) {
            // Turn on heating when temperature is 2°C below set value
            LED_DRY_ON();
            // Send command to mainboard to turn on heating
            SendData_Set_Command(0x02, 0x01);
            tx_thread_sleep(10);
            // Update flag
            
        }
        // If temperature is between (set-2) and set, maintain current state
    }
}


void power_on_run_handler(void)
{
        power_on_init_reference();
        // Time-sharing execution: each function runs every 20ms
        switch(function_counter) {
            case 0:
                disp_set_timer_fun();
                break;
            case 1:
                set_timer_fun_led_blink();
                break;
            case 2:
                Display_TimeColon_Blink_Fun();
                break;
            case 3:
                disp_ntc_temperature_value();
                break;
            case 4:
                // Compare temperature and control LED_DRY
                compare_temp_value();
                break;
            default:
                break;
        }
        
        // Increment counter and wrap around after all functions
        function_counter++;
        if(function_counter >= 5) {
            function_counter = 0;
        }

}
