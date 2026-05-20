#ifndef __BSP_POWER_H
#define __BSP_POWER_H
#include "main.h"


typedef enum TIMING_T{

   timing_donot,
   timing_success ,
   timing_power_off,
   timing_null
}timing_t;


typedef enum _Signal{
  
	PANEL_DATA=0x01,
    ORDER_DATA,
    ANSWER_DATA,
   
    POWER_NULL

}signal_t;



typedef struct __RUN{

  uint8_t gPower_On;




  volatile int8_t timer_dispTime_hours;
  volatile int8_t timer_dispTime_minutes;
   

   uint8_t power_on_step;
   uint8_t power_off_flag;
  
   uint8_t disp_health_value;
   
  uint8_t  gPlasma;

   uint8_t  gDry;
   uint8_t  gMouse;
  


   uint8_t  ptc_warning;
   uint8_t  fan_warning;

   //key ref
   uint8_t recoder_start_conuter_flag;

   uint8_t ntc_tem[1];
   int8_t temporary_timer_dispTime_hours;
   int8_t temporary_timer_dispTime_minutes;

   /***/
   //timer timing ref

   uint8_t gTimer_timer_seconds_counter;

   


	uint8_t gTimer_disp_ntc ;

    uint16_t disp_health_minutes;

    uint8_t gTimer_time_colon;

    // Temperature setting variables
    uint8_t set_temperature_value;
   
    
    // PTC heating control flag
    uint8_t ptc_open_f;


 }RUN_T;

extern RUN_T run_t;


void SplitDispose_Key(uint8_t value);
void Decode_Handler(uint8_t data);

void Power_Off_Fun(void);


void power_on_init_reference(void);


void power_on_handler(void);


void disp_set_timer_fun(void);


void power_off_run_handler(void);

void compare_temp_value(void);
void immediately_compare_temp_value(void);

void power_on_run_handler(void);


#endif 




