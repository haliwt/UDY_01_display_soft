/*
 * bsp_key_app.c
 *
 *  Created on: 2025 5  29日
 *      Author: Administrator
 */

#include "bsp.h"


KEY_T_TYPEDEF key_t;





uint8_t  set_temp_flag;
uint8_t power_on_key_counter;


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
 * Function Name:void ai_on_off_handler(void)
 * 
 * 
 **********************************************************************************/
//void SetDataTemperatureValue(void)
//{
//    if(set_temp_flag ==1){
//	 set_temp_flag++;
//
//     //SendData_Tx_Data(0x11,gpro_t.set_up_temperature_value);
//     SendData_ToMainboard_Data(0x2A,&gpro_t.set_up_temperature_value,0x01);
//     osDelay(5);
//	}  
//
//
//}

/*********************************************************************************
 * 
 * Function Name:void mouse_on_off_handler(void)
 * // 设置温度并做边界检查
 * 
 **********************************************************************************/
void set_temperature_value(int8_t delta) 
{
    uint8_t new_temp;
	

	if(gpro_t.temperature_init_value == 0 && gpro_t.set_temp_value_success==0){
        gpro_t.temperature_init_value++;
        gpro_t.set_up_temperature_value = (delta > 0) ? 20 : 40;
	    new_temp = gpro_t.set_up_temperature_value;
    }
	else{

	   	new_temp = gpro_t.set_up_temperature_value + delta;
	    if (new_temp < 20) new_temp = 20;
        if (new_temp > 40) new_temp = 40;
   }

	gpro_t.set_up_temperature_value = new_temp;

    //run_t.set_temperature_decade_value = new_temp / 10;
    //run_t.set_temperature_unit_value   = new_temp % 10;

    key_t.key_set_temperature_flag  = 1;
    run_t.gTimer_set_up_temperature_value = 0;
    gpro_t.g_manual_shutoff_dry_flag   = 0;
 

    //SendData_ToMainboard_Data(0x2A,&new_temp,0x01);
   // osDelay(5);

    TM1639_Display_Temperature(gpro_t.set_up_temperature_value);//TM1639_Write_2bit_SetUp_TempData(run_t.set_temperature_decade_value, run_t.set_temperature_unit_value, 0);
	
}

/*******************************************************
	*
	*Function Name: void bsp_plasma_handler(uint8_t data)
	*Function :
	*
	*
*******************************************************/
void adjust_timer_minutes(int8_t delta_min) 
{
    int8_t total_hour = run_t.temporary_timer_dispTime_hours ;
//	uint8_t copy_total_hour;
    total_hour += delta_min;

   if(total_hour > 24){
         total_hour =24;
   	}
	else if (total_hour < 0) {
        total_hour = 0 ;  // 循环处理负值
    }

    //total_hour %= 24 ;  //证在一天范围内

    run_t.temporary_timer_dispTime_hours   = total_hour;
    run_t.temporary_timer_dispTime_minutes = 0;


    run_t.timer_dispTime_hours=  run_t.temporary_timer_dispTime_hours;
	run_t.timer_dispTime_minutes = run_t.temporary_timer_dispTime_minutes;
	
    run_t.gTimer_set_timer_timing_value=0;
	//copy_total_hour=(uint8_t)total_hour;
	//SendData_ToMainboard_Data(0x4C,&copy_total_hour,0x01);
	//osDelay(5);
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
	if(key_t.key_power_flag == 1){
		
       if(POWER_KEY_VALUE() ==KEY_UP && key_t.key_long_power_flag != KEY_LONG_POWER){
			
			power_on_key_counter=0;
		    key_t.key_long_power_flag=0;//WT.EDIT 2025.06.04
			if(run_t.gPower_On == power_off){
				//run_t.gPower_On = power_on;
				 SendData_PowerOnOff(1); // power on
                 osDelay(10);

			}
			else{

				//run_t.gPower_On = power_off;
				SendData_PowerOnOff(0); // power off
                osDelay(10);
			}
			 key_t.key_power_flag++;
		}
       else if(POWER_KEY_VALUE() ==KEY_UP && key_t.key_long_power_flag == KEY_LONG_POWER ){
		   key_t.key_power_flag++;
		   key_t.key_long_power_flag=0;//WT.EDIT 2025.06.04
           power_on_key_counter=0;
    

       }
	  
	}
}
void power_key_long_handler(void)
{
	power_on_key_counter++;
	if(POWER_KEY_VALUE() ==KEY_DOWN && run_t.gPower_On == power_on && (power_on_key_counter  >= 60 && power_on_key_counter < 200)){
		
			power_on_key_counter=202;
			key_t.key_long_power_flag =  KEY_LONG_POWER; //timer is OK.
			gpro_t.set_timer_timing_doing_value=1;
			
			run_t.gTimer_set_timer_timing_value=0;
			gpro_t.key_add_dec_pressed_flag =0;
			
			SendData_Buzzer();
			osDelay(5);
			key_t.key_power_flag = 1;
	}
	else{
	  // handle_key_power_long_pressed();
		key_t.key_power_flag = 1;

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
		    osDelay(5);
            LED_PLASMA_OFF();
            gpro_t.send_ack_cmd = check_ack_plasma_off;
        } else {
            run_t.gPlasma = 1;
            SendData_Set_Command(plasma_cmd, 0x01);
			osDelay(5);
            LED_PLASMA_ON();
            gpro_t.send_ack_cmd = check_ack_plasma_on;
        }
        gpro_t.gTimer_again_send_power_on_off = 0;

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
			osDelay(5);
            //run_t.gDry = 1;
			//LED_DRY_ON();
            gpro_t.g_manual_shutoff_dry_flag = 0;
           
        } else {
            SendData_Set_Command(dry_cmd, 0x00);//sendCommandAndAck(dry_cmd, 0x00, check_ack_ptc_off);
			osDelay(5);
            //run_t.gDry = 0;
			//LED_DRY_OFF();
            gpro_t.g_manual_shutoff_dry_flag = 1; // 手动关闭后不再自动开启
           
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

	 static uint8_t k1;

	 if(run_t.gPower_On == power_on){
	 if(MOUSE_KEY_VALUE()==KEY_DOWN){
          if(k1< 200){
               k1++;

		 }

     }
	 
	 if(k1==200){
	  
        if(run_t.gMouse == 0){
            // 开启 Mouse 功能
            SendData_Set_Command(mouse_cmd, 0x01);
            osDelay(5);
            run_t.gMouse = 1;
            LED_MOUSE_ON();
            gpro_t.send_ack_cmd = check_ack_mouse_on;  // 假设有对应的反馈类型
            gpro_t.gTimer_again_send_power_on_off = 0;

        }
		else{
            // 关闭 Mouse 功能
            SendData_Set_Command(mouse_cmd, 0x00);
            osDelay(5);
            run_t.gMouse = 0;
            LED_MOUSE_OFF();
            gpro_t.send_ack_cmd = check_ack_mouse_off;  // 假设有对应的反馈类型
            gpro_t.gTimer_again_send_power_on_off = 0;
        }
	   k1=0;
       return ;
     }
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
    if(run_t.ptc_warning != 0) return;

    run_t.gTimer_time_colon = 0;

    switch(gpro_t.set_timer_timing_doing_value)
    {

	    case 3:
		case 0:  // 设置温度增加
            SendData_Buzzer();
			osDelay(5);
            set_temperature_value(+1);
            break;

        case 1:  // 设置定时增加（每次加60分钟）
            SendData_Buzzer();
			osDelay(5);
            run_t.gTimer_set_timer_timing_value = 0;
            gpro_t.key_add_dec_pressed_flag = 1;
            adjust_timer_minutes(1);  // 固定每次加60分钟
            break;
    }
}


/****************************************************************
	*
	*Function Name :void key_dec_fun(void)
	*Function : 
	*Input Parameters :NO
	*Retrurn Parameter :NO
	*
*****************************************************************/
void key_dec_fun(void)
{
    if(run_t.ptc_warning != 0) return;

    switch(gpro_t.set_timer_timing_doing_value)
    {

        case 3:
		case 0:  // 设置温度减少
             SendData_Buzzer();
		     osDelay(5);
            set_temperature_value(-1);
            break;

        case 1:  // 设置定时减少（每次减60分钟）
            SendData_Buzzer();
			osDelay(5);
            run_t.gTimer_set_timer_timing_value = 0;
            gpro_t.key_add_dec_pressed_flag = 1;
            adjust_timer_minutes(-1);  // 固定每次减60分钟
        break;
    }
}


/****************************************************************
	*
	*Function Name :void wifi_mode_key_handler(void)
	*Function : 
	*Input Parameters :NO
	*Retrurn Parameter :NO
	*
*****************************************************************/
 


/*
*********************************************************************************************************
*	函 数 名: void process_keys(void) 
*	功能说明:
*	形    参：无
*	返 回 值: 按键代码
*********************************************************************************************************
*/

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
#else 
void process_keys(void) 
{
    if(key_t.key_power_flag == 1){
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
//	else if(key_t.key_mouse_flag == 1 &&  MOUSE_KEY_VALUE()==KEY_UP){
//		key_t.key_mouse_flag ++;
//		mouse_key_handler() ;
//	}







}







#endif 



