/*
 * bsp_led.c
 *
 *  Created on: 2025年3月4日
 *      Author: Administrator
 */
#include "bsp.h"

/*
	@
	@ led time init handler
	@


*/
void LED_TIME_Init(void)
{
  
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	
	  /* GPIO Ports Clock Enable */
	
	  __HAL_RCC_GPIOA_CLK_ENABLE();
	
	  /*Configure GPIO pin Output Level */
	  HAL_GPIO_WritePin(GPIOA, LED_TIME_Pin, GPIO_PIN_RESET);
	
	
	
	  /*Configure GPIO pin : LED_CTL_Pin */
		GPIO_InitStruct.Pin = LED_TIME_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Pull = GPIO_PULLDOWN;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
	    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);


}


/**************************************************************************
 * power_on_led(void)
 * 功能:开启所有LED
 * 参数:无
 * 返回值:无
 ************************************************************************/
void power_on_led(void)
{
    LED_POWER_ON();
    LED_TIME_ON();
    LED_DRY_ON();
    LED_PLASMA_ON();
  
    LED_MOUSE_ON();
   
	
}



/**************************************************************************
 * power_off_led(void)
 * 功能:关闭所有LED
 * 参数:无
 * 返回值:无
 ************************************************************************/
void power_off_led(void)
{
    LED_POWER_OFF();
    LED_TIME_OFF();
    LED_DRY_OFF();
    LED_PLASMA_OFF();
    
    LED_MOUSE_OFF();
	TM1639_Display_ON_OFF(0);
	
}
/************************************************************************
 * LED_Power_Breathing(void)
 * 功能:LED呼吸灯函数
 * 参数:无
 * 返回值:无
 ************************************************************************/
void LED_Power_Breathing(void)
{
    static uint8_t breathing_time = 0;
    static uint8_t led_state = 0;
    
    breathing_time++;
    if(breathing_time >= 100)  // 呼吸周期1秒
    {
        breathing_time = 0;
        led_state = !led_state;
        
        if(led_state)
        {
            LED_POWER_ON();
        }
        else
        {
            LED_POWER_OFF();
        }
    }
}
/************************************************************************
 * Function Name: LED_Power_Breathing(void)
 * 功能:
 * 参数:无
 * 返回值:无
 ************************************************************************/
void Led_Panel_OnOff(void)
{



}


























