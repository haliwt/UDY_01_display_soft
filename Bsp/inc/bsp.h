/*
 * bsp.h
 *
 *  Created on: 2025年3月4日
 *      Author: Administrator
 */

#ifndef __BSP_H_
#define __BSP_H_
#include <stdio.h>
#include <stdlib.h>


#include "main.h"
#include "stdio.h"
#include "string.h"




#include "bsp_freertos.h"

#include "bsp_key.h"
#include "bsp_key_app.h"
#include "bsp_timer.h"
#include "bsp_delay.h"

#include "bsp_tm1639.h"
#include "bsp_smg.h"
#include "bsp_power.h"
#include "bsp_led.h"
#include "bsp_smg_app.h"
#include "bsp_dht11.h"

#include "bsp_message.h"
#include "bsp_cmd_link.h"



#include "interrupt_manager.h"







//mainboard


#include "tim.h"
#include "usart.h"
#include "gpio.h"

//freeRtos
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"


#define  USE_FreeRTOS      1
  
#define  TEST_UNIT        0


#if USE_FreeRTOS == 1
	//#include "FreeRTOS.h"
	///#include "task.h"
	#define DISABLE_INT()    taskENTER_CRITICAL()
	#define ENABLE_INT()     taskEXIT_CRITICAL()
#else
	/* ����ȫ���жϵĺ� */
	#define ENABLE_INT()	__set_PRIMASK(0)	/* ʹ��ȫ���ж� */
	#define DISABLE_INT()	__set_PRIMASK(1)	/* ��ֹȫ���ж� */
#endif


typedef enum{

  TIMER_NORMAL_TIMING,
  TIMER_SUCCESS

}TIMER_STATE;


typedef enum power_onoff_state_t{

    power_off,
    power_on


}power_onoff_stae;


typedef enum{

  no_ai_mode,
  ai_mode

}ai_mode_typedef;

typedef struct _pro_t{

   
   uint8_t set_timer_timing_value_success;
   uint8_t set_timer_timing_doing_value;
   uint8_t set_temp_value_success;
   uint8_t set_up_temperature_value;
   //uint8_t mode_Key_long_counter;
   //uint8_t  mode_key_shot_flag ;
  
   uint8_t answer_signal_flag;


   uint8_t send_ack_cmd;

   uint8_t g_manual_shutoff_dry_flag;
  

   uint8_t DMA_txComplete;
  
   uint8_t g_time_disp_colon_flag;
   
   uint8_t key_add_dec_pressed_flag;
   
   uint8_t gTimer_again_send_power_on_off;
   uint8_t gTimer_set_temp_counter;
   uint16_t gTimer_4bitsmg_blink_times;
  
   
}pro_run_t;

extern pro_run_t  gpro_t;


void bsp_init(void);
















#endif /* BSP_INC_BSP_H_ */


