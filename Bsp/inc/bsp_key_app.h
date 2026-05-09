/*
 * bsp_key_app.h
 *
 *  Created on: 2025年2月19日
 *      Author: Administrator
 */

#ifndef __BSP_KEY_APP_H_
#define __BSP_KEY_APP_H_
#include "main.h"


typedef enum{

   KEY_NULL,
   KEY_POWER_ID = 0x01,
   KEY_MODEL_ID = 0x02,
   KEY_DOWN_ID = 0x04,
   KEY_UP_ID = 0x08,

   KEY_DWON_RUN_ID= 0x14,

   KEY_LONG_POWER = 0x81,
   KEY_LONG_MODE = 0x82,

   

}KEYName_Typedef;





void process_keys(void) ;

void power_key_short_handler(void);


void power_key_long_handler(void);

void key_add_fun(void);

void key_dec_fun(void);


void dry_key_handler(void) ;

void mouse_key_handler(void);

void plasma_key_handler(void);



#endif /* BSP_INC_BSP_KEY_APP_H_ */

