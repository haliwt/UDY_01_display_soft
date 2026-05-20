#include "bsp.h"

#define TEMPERATURE_HIGH_THRESHOLD  39  // 高温阈值
#define TEMPERATURE_LOW_THRESHOLD   38  // 低温阈值
#define TEMPERATURE_DIFF_THRESHOLD  1   // 

#define CHECK_TIME_THRESHOLD_4S      3   //温度差阈值

//static void tempeature_compareison_value_handler(void);
//static void handleSetTemperatureControl(void) ;
//static void handleDefaultTemperatureControl(void);

uint8_t current_temperature;


