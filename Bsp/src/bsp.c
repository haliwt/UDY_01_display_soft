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

    run_t.gRunCommand_label =0xff;
    run_t.gPower_On = power_off;


}




