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
    if(breathing_time >= 50) //20ms * 50 // 呼吸周期1秒
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


























