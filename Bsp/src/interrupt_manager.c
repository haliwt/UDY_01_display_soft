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
void tim17_invoke_callback(void)//tim17_isr_callback_handler();
{
   static  uint16_t tm0;
  

   
       tm0++;
	   gpro_t.gTimer_4bitsmg_blink_times++;
	   if(tm0> 999){ //1s
	      tm0=0;
		  
		
         run_t.gTimer_disp_ntc ++;
         run_t.gTimer_time_colon++ ;
		//--------------//

		run_t.gTimer_timer_seconds_counter ++;
	
     
		
		  

		//usart
    
        gpro_t.gTimer_disp_timer_counter ++;
		gpro_t.gTimer_set_temp_counter++;
	
	   }
	   

 }

