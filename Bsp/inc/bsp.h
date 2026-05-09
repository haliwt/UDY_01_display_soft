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
#include <string.h>






#include "bsp_key.h"
#include "bsp_key_app.h"
#include "bsp_timer.h"
#include "bsp_delay.h"
#include "bsp_mouse.h"

#include "bsp_tm1639.h"
#include "bsp_smg.h"
#include "bsp_power.h"
#include "bsp_led.h"
#include "bsp_smg_app.h"
#include "bsp_dht11.h"

#include "bsp_message.h"
#include "bsp_cmd_link.h"



#include "interrupt_manager.h"

#include "bsp_threadx.h"






//mainboard


#include "tim.h"
#include "usart.h"
#include "gpio.h"

//freeRtos
#include "app_threadx.h"




#define  USE_Threadx      1
  
#define  TEST_UNIT        0


#if USE_ThreadX == 1

#include "tx_api.h"

static UINT _old_posture;

#define DISABLE_INT()   _old_posture = tx_interrupt_control(TX_INT_DISABLE)
#define ENABLE_INT()    tx_interrupt_control(_old_posture)

#else

#define ENABLE_INT()    __set_PRIMASK(0)
#define DISABLE_INT()   __set_PRIMASK(1)

#endif


/* Ä¬ÈÏÊÇ¹Ø±Õ×´Ì¬ */
#define  Enable_EventRecorder  0

#if Enable_EventRecorder == 1
	#include "EventRecorder.h"
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

   uint8_t set_up_temperature_value;
   uint8_t  temperature_init_value;
  
  
   uint8_t answer_signal_flag;


  


  

   uint8_t DMA_txComplete;
  
   uint8_t g_time_disp_colon_flag;
   
   uint8_t key_add_dec_pressed_flag;
   
   uint8_t gTimer_disp_timer_counter ;
   uint8_t gTimer_set_temp_counter;

   uint16_t gTimer_4bitsmg_blink_times;
  
   
}pro_run_t;

extern pro_run_t  gpro_t;


void bsp_init(void);
















#endif /* BSP_INC_BSP_H_ */


