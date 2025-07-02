/*
 * bsp_smg.h
 *
 *  Created on: 2025年3月4日
 *      Author: Administrator
 */

#ifndef BSP_INC_BSP_SMG_H_
#define BSP_INC_BSP_SMG_H_
#include "main.h"

//函数声明
void TM1639_Display_4Bit_Time(uint8_t hours,uint8_t minitues);

void TM1639_donotDisplay_4Bit_Time(void);



void TM1639_Display_4Bit_Error(uint8_t sle);

void Display_TimeColon_Blink_Fun(void);



#endif /* BSP_INC_BSP_SMG_H_ */
