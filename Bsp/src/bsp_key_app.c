/*
 * bsp_key_app.c
 *
 *  Created on: 2025 5  29日
 *      Author: Administrator
 */

#include "bsp.h"


uint8_t  set_temp_flag;
uint8_t power_on_key_counter;

static void adjust_timer_minutes(int8_t delta_min) ;
static void adjust_temperature_value(int8_t delta) ;




typedef struct {
    uint8_t *flag;
    uint8_t threshold;
    void (*onPress)(void);
} KeyHandler;

/****************************************************************
	*
	*Function Name :void handle_key(KeyHandler *handler) 
	*Function : set timer timing how many ?
	*Input Parameters : struct KeyHandler of reference
	*Retrurn Parameter :NO
	*
*****************************************************************/
void handle_key(KeyHandler *handler) 
{
    if (*(handler->flag) == 1) {
        *(handler->flag) += 1;

        if (handler->threshold > 0 && *(handler->flag) > handler->threshold) {
            *(handler->flag) = 80; // 特殊情况处理
        }

        if (handler->onPress) {
            handler->onPress();
        }
    }
}


/*********************************************************************************
 * 
 * Function Name:void mouse_on_off_handler(void)
 * // 设置温度并做边界检查
 * 
 **********************************************************************************/
static void adjust_temperature_value(int8_t delta) 
{
  
	 gpro_t.set_up_temperature_value += delta;
	 
	 if (run_t.set_temperature_value< 30) run_t.set_temperature_value= 30;
     if (run_t.set_temperature_value > 50) run_t.set_temperature_value= 50;
	  
     gpro_t.ptc_first_open_f = 0;
	 gpro_t.gTimer_set_timer_counter=0;
	 gpro_t.disp_set_temp_f =0;

    //TM1639_Display_4Bit_Temp(run_t.set_temperature_value);
    TM1639_Display_Temperature(run_t.set_temperature_value);

  
  
}

/*******************************************************
	*
	*Function Name: void bsp_plasma_handler(uint8_t data)
	*Function :
	*
	*
*******************************************************/
static void adjust_timer_minutes(int8_t delta_min) 
{
   
//	uint8_t copy_total_hour;
    run_t.timer_dispTime_hours += delta_min;

   // 限制时间范围在1~72小时
    if (run_t.timer_dispTime_hours > 24) {
        run_t.timer_dispTime_hours = 24;  // 超过72小时，循环回到1小时
    }
    else if (run_t.timer_dispTime_hours < 0) {
        run_t.timer_dispTime_hours = 0;  // 低于1小时，循环到72小时
    }

    gpro_t.gTimer_set_timer_counter=0;
	
	run_t.timer_dispTime_minutes = 0;
	
	
	TM1639_Display_4Bit_Time(run_t.timer_dispTime_hours,run_t.timer_dispTime_minutes);

}
/**********************************************************************************************************
    *
	*Function Name: void power_key_handler(void) 
	*Function	功能说明: 从按键FIFO缓冲区读取一个键值。
	*Inpur Ref:  无
	*Return Ref: 按
	*键代码
**********************************************************************************************************/
void power_key_short_handler(void)
{
	if(run_t.gPower_On == power_off){
		//run_t.gPower_On = power_on;
		SendData_PowerOnOff(1); // power on
		tx_thread_sleep(10);

	}
	else if(run_t.gPower_On == power_on){

		//run_t.gPower_On = power_off;
		SendData_PowerOnOff(0); // power off
		tx_thread_sleep(10);
	}
			 
		
  }

/**********************************************************************************************************
    *
	*Function Name: void power_key_long_handler(void)
	*Function:缓冲区读取一个键值。
	*Inpur Ref: NO 无
	*Return Ref: NO按
	*键代码
**********************************************************************************************************/
void power_key_long_handler(void)
{
	
	gpro_t.set_timer_timing_doing_value=1;

	// Reset timer for setting timeout
	gpro_t.gTimer_set_timer_counter = 0;

	gpro_t.key_add_dec_pressed_flag =0;

	SendData_Buzzer();
	tx_thread_sleep(10);
		
           

}
/**********************************************************************************************************
*	函 数 名: void plasma_key_handler(void) 
*	功能说明: 从按键FIFO缓冲区读取一个键值。
*	形    参:  无
*	返 回 值: 按键代码
**********************************************************************************************************/
void plasma_key_handler(void) 
{

        if(run_t.gPlasma == 1){
            run_t.gPlasma = 0;
            SendData_Set_Command(plasma_cmd, 0x00);
		    tx_thread_sleep(10);
            LED_PLASMA_OFF();
          
        } else {
            run_t.gPlasma = 1;
            SendData_Set_Command(plasma_cmd, 0x01);
			tx_thread_sleep(10);
            LED_PLASMA_ON();
            
        }
     

}
/****************************************************************
	*
	*Function Name :void mode_key_handler(void)
	*Function : set timer timing how many ?
	*Input Parameters :NO
	*Retrurn Parameter :NO
	*
*****************************************************************/
void dry_key_handler(void) 
{

        if(run_t.gDry == 0) {
            SendData_Set_Command(dry_cmd, 0x01);//sendCommandAndAck(dry_cmd, 0x01, check_ack_ptc_on);
			tx_thread_sleep(10);
            //run_t.gDry = 1;
			//LED_DRY_ON();
 
           
        } else {
            SendData_Set_Command(dry_cmd, 0x00);//sendCommandAndAck(dry_cmd, 0x00, check_ack_ptc_off);
			tx_thread_sleep(10);
            //run_t.gDry = 0;
			//LED_DRY_OFF();
  
           
        }

}
/****************************************************************
	*
	*Function Name :void mode_key_handler(void)
	*Function : set timer timing how many ?
	*Input Parameters :NO
	*Retrurn Parameter :NO
	*
*****************************************************************/
void mouse_key_handler(void) 
{


   
     if(run_t.gMouse == 0){
            // 开启 Mouse 功能
            
            run_t.gMouse = 1;
            LED_MOUSE_ON();
          SendData_Set_Command(mouse_cmd, 0x01);
            tx_thread_sleep(10);//对应的反馈类型
            

        }
		else{
            // 关闭 Mouse 功能
          
            run_t.gMouse = 0;
            LED_MOUSE_OFF();
         SendData_Set_Command(mouse_cmd, 0x00);
            tx_thread_sleep(10);//应的反馈类型
            
        }

  
 
 }
/****************************************************************
	*
	*Function Name :void mode_key_handler(void)
	*Function : set timer timing how many ?
	*Input Parameters :NO
	*Retrurn Parameter :NO
	*
*****************************************************************/
void key_add_fun(void)
{
   

     SendData_Buzzer();
	 tx_thread_sleep(10);

	 if(gpro_t.set_timer_timing_doing_value) {
	 	   gpro_t.key_add_dec_pressed_flag = 1;
		 // In timer setting mode, adjust timer hours
		#if 0
		 run_t.timer_dispTime_hours++;
		 if(run_t.timer_dispTime_hours > 24) {
			 run_t.timer_dispTime_hours = 24; // Maximum 24 hours
		 }
		 // Display the new timer value
		 TM1639_Display_4Bit_Time(run_t.timer_dispTime_hours, run_t.timer_dispTime_minutes);
		 // Reset the timeout counter
		 gpro_t.gTimer_set_timer_counter = 0;
		 #else
		 adjust_timer_minutes(1) ;

		 #endif 
	 }
	 else{
	 	  
	 	 if(gpro_t.set_timer_timing_doing_value==1) return ;
		 #if 0
		 // In temperature setting mode, adjust temperature
		 run_t.set_temperature_value++;
		 if(run_t.set_temperature_value > 50) {
			 run_t.set_temperature_value = 50; // Loop back to maxnimum (50°C)
		 }
		 TM1639_Display_4Bit_Temp(run_t.set_temperature_value);
		 #else 

          adjust_temperature_value(1) ;
		 #endif 
		 
	 }
           
    
}

/****************************************************************
	*
	*Function Name :void key_dec_fun(void)
	*Function : set temperature value is range is 30 ~ 50 degree. 
	*Input Parameters :NO
	*Retrurn Parameter :NO
	*
*****************************************************************/
void key_dec_fun(void)
{
  
	
	SendData_Buzzer();
	tx_thread_sleep(10);

	if(gpro_t.set_timer_timing_doing_value) {

	    gpro_t.key_add_dec_pressed_flag = 1;

	    #if 0
		// In timer setting mode, adjust timer hours
		run_t.timer_dispTime_hours--;
		if(run_t.timer_dispTime_hours < 0) {
			run_t.timer_dispTime_hours = 0; // Minimum 0 hours
		 }
		// Display the new timer value
		TM1639_Display_4Bit_Time(run_t.timer_dispTime_hours, run_t.timer_dispTime_minutes);
		// Reset the timeout counter
		gpro_t.gTimer_set_timer_counter = 0;
		#else
		adjust_timer_minutes(-1) ;

		#endif 
	}
	else{

	    if(gpro_t.set_timer_timing_doing_value==1) return ;
		
		#if 0
		// In temperature setting mode, adjust temperature
		run_t.set_temperature_value--;
		if(run_t.set_temperature_value < 30) {
			run_t.set_temperature_value = 30; // Loop back to  (30°C)
		 }
		TM1639_Display_4Bit_Temp(run_t.set_temperature_value);
		#else
		adjust_temperature_value(-1) ;

		#endif 
	}

}



#if 0
void process_keys(void) 
{
	//power_key_handler();


  //定义所有按键处理器
    KeyHandler handlers[] = {
        //{ &key_t.key_power_flag, 0, power_key_handler },
        //{ &key_t.key_mode_flag, 0, mode_key_handler },
        { &key_t.key_dec_flag, 0, key_dec_fun },
        { &key_t.key_add_flag, 0, key_add_fun },
        { &key_t.key_plasma_flag, 0, plasma_key_handler },
        { &key_t.key_dry_flag, 0, dry_key_handler },
       // { &key_t.key_mouse_flag, 0, mouse_key_handler },
    };

    // 循环处理每个按键
    for (int i = 0; i < sizeof(handlers)/sizeof(handlers[0]); i++) {
        handle_key(&handlers[i]);
    }
}
 
void process_keys(void) 
{
    static uint8_t mouse_power_on;
	if(key_t.key_power_flag == 1 && POWER_KEY_VALUE()== KEY_UP){
		key_t.key_power_flag++;
	   power_key_short_handler();

    }
	else if(key_t.key_dec_flag == 1 && DEC_KEY_VALUE()==KEY_UP){
		key_t.key_dec_flag++;
		key_dec_fun();
	}
	else if(key_t.key_add_flag == 1 && ADD_KEY_VALUE()==KEY_UP){
		key_t.key_add_flag ++;
		key_add_fun();
	}
	else if(key_t.key_dry_flag == 1 && DRY_KEY_VALUE()==KEY_UP){
		key_t.key_dry_flag ++;
		dry_key_handler() ;
	}
	else if(key_t.key_plasma_flag == 1 && PLASMA_KEY_VALUE()==KEY_UP){
		key_t.key_plasma_flag++;
		plasma_key_handler() ;
	}
	else if(key_t.key_mouse_flag == 1 &&  MOUSE_KEY_VALUE()==KEY_UP){
		key_t.key_mouse_flag ++;
		if(mouse_power_on==0){

		    mouse_power_on++;
		    run_t.gMouse = 1;
		   LED_MOUSE_ON();

		}
		else{
		 mouse_key_handler() ;
		}
	}

}



#endif 



