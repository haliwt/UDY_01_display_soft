/*
 * bsp_led.h
 *
 *  Created on: 2025年3月4日
 *      Author: Administrator
 */

#ifndef __BSP_LED_H_
#define __BSP_LED_H_
#include "main.h"




#define LED_POWER_ON()       do{LED_POWER_GPIO_Port->BSRR |=LED_POWER_Pin;}while(0)//HAL_GPIO_WritePin(LED_POWER_GPIO_Port, LED_POWER_Pin, GPIO_PIN_RESET)
#define LED_POWER_OFF()          do{LED_POWER_GPIO_Port->BSRR |=(uint32_t)LED_POWER_Pin<<16;}while(0)//sys_write_gpio_pin_value(LED_POWER_GPIO_Port,LED_POWER_Pin,GPIO_PIN_SET)//HAL_GPIO_WritePin(LED_POWER_GPIO_Port, LED_POWER_Pin, GPIO_PIN_SET)

#define LED_TIME_ON()      do{LED_CTL_GPIO_Port->BSRR |=LED_CTL_Pin;}while(0)    
#define LED_TIME_OFF()      do{LED_CTL_GPIO_Port->BSRR |=(uint32_t)LED_CTL_Pin<<16;}while(0)//        sys_write_gpio_pin_value(LED_TEMP_SYMBOL_GPIO_Port, LED_TEMP_SYMBOL_Pin, GPIO_PIN_RESET)
 

#define LED_DRY_ON()       do{LED_DRY_GPIO_Port->BSRR |=(uint32_t)LED_DRY_Pin<<16;}while(0)
#define LED_DRY_OFF()       do{LED_DRY_GPIO_Port->BSRR |=LED_DRY_Pin;}while(0)//sys_write_gpio_pin_value(LED_HUM_SYMBOL_GPIO_Port, LED_HUM_SYMBOL_Pin, GPIO_PIN_SET)

#define LED_PLASMA_ON()          do{LED_PLASMA_GPIO_Port->BSRR |=(uint32_t)LED_PLASMA_Pin<<16;}while(0)
#define LED_PLASMA_OFF()          do{LED_PLASMA_GPIO_Port->BSRR |=LED_PLASMA_Pin;}while(0)  

#define LED_MOUSE_ON()          do{LED_MOUSE_GPIO_Port->BSRR |=(uint32_t)LED_MOUSE_Pin<<16;}while(0)
#define LED_MOUSE_OFF()           do{LED_MOUSE_GPIO_Port->BSRR |=LED_MOUSE_Pin;}while(0)  

void power_on_led(void);
void power_off_led(void);
void LED_Power_Breathing(void);   // LED呼吸灯函数

void Led_Panel_OnOff(void);


#endif /* BSP_INC_BSP_LED_H_ */


