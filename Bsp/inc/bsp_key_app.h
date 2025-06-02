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



typedef struct{

 //SELECT KEY
  uint8_t key_power_flag;
  uint8_t key_long_power_flag;

  uint8_t key_dec_flag;
  uint8_t key_add_flag;
  

  //FUNCTION KEY
  uint8_t key_plasma_flag;
  uint8_t key_dry_flag;
  uint8_t key_mouse_flag;


}KEY_T_TYPEDEF;

extern KEY_T_TYPEDEF key_t;

void process_keys(void) ;

//void SetDataTemperatureValue(void);

//void mode_key_handler(void);

//void wifi_mode_key_handler(void);




#endif /* BSP_INC_BSP_KEY_APP_H_ */

