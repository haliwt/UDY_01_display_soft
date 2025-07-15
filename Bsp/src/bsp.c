#include "bsp.h"



pro_run_t  gpro_t;

typedef enum {
    TEMP_SETTING_MODE = 0,
    TIMER_SETTING_MODE = 1
} SettingMode;




uint8_t hours_one,hours_two,minutes_one,minutes_two;

uint8_t  step_state;
uint8_t  first_set_temperature_value;



void bsp_init(void)
{

#if 0  
	/* 初始化EventRecorder并开启 */
	EventRecorderInitialize(EventRecordAll, 1U);
	EventRecorderStart();
#endif
    run_t.power_on_step =0;
    run_t.gPower_On = power_off;


}

#if 0

/**
  * ????????: ?????c????printf??DEBUG_USARTx
  * ???????: ??
  * ?? ?? ?: ??
  * ?    ??????
  */
int fputc(int ch, FILE *f)
{
  HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, 0xffff);
  return ch;
}

/**
  * ????????: ?????c????getchar,scanf??DEBUG_USARTx
  * ???????: ??
  * ?? ?? ?: ??
  * ?    ??????
  */
int fgetc(FILE * f)
{
  uint8_t ch = 0;
  HAL_UART_Receive(&huart2,&ch, 1, 0xffff);
  return ch;
}

#endif 

