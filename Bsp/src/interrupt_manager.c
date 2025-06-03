/*
 * interrupt_manager.c
 *
 *  Created on: 2025年3月4日
 *      Author: Administrator
 */
#include "bsp.h"

/********************************************************************************
	**
	*Function Name:void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
	*Function :UART callback function  for UART interrupt for receive data
	*Input Ref: structure UART_HandleTypeDef pointer
	*Return Ref:NO
	*
*******************************************************************************/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
   static  uint16_t tm0;
  

    if(htim->Instance==TIM17){ //timer number14 is 100ms.
       tm0++;
	   gpro_t.gTimer_4bitsmg_blink_times++;
	   if(tm0> 999){ //1s
	      tm0=0;
		  
		run_t.gTimer_error_digital++;
	

		run_t.gTimer_display_dht11++;

	
	
		run_t.gTimer_time_colon++ ;
		//--------------//

		run_t.gTimer_timer_seconds_counter ++;
		run_t.gTimer_timing_seconds_counter ++;


		run_t.gTimer_key_timing++;

		run_t.gTimer_key_temp_timing++ ;
	
		run_t.gTimer_set_temp_times++;

		//usart



		
	

		gpro_t.gTimer_again_send_power_on_off++;
	
	
		   
		  

		 }
	   

   }
}

