#include "bsp.h"


/***********************************************************
*
*
*
*
*
***********************************************************/
#if 0
uint8_t KEY_Scan(void)
{
  uint8_t  reval = 0;
  key_t.read = _KEY_ALL_OFF; //0xFF 


    if(WIFI_KEY_VALUE() ==1 ) //WIFI_KEY_ID = 0x80
	{
		key_t.read &= ~0x80; // 0x1f & 0x7F =  0x7F
	}
    else if(MOUSE_KEY_VALUE()   ==1 ) //FAN_KEY_ID = 0x10
	{
		  key_t.read &= ~0x10; // 0xFf & 0xEF =  0xEF
	}
	else if(PLASMA_KEY_VALUE()   ==1 ) //PLASMA_KEY_ID = 0x20
	{
		  key_t.read &= ~0x20; // 0xFf & 0xDF =  0xDF
	}
	else if(DRY_KEY_VALUE()  ==1 ) //DRY_KEY_ID = 0x40
	{
		  key_t.read &= ~0x40; // 0xFf & 0xBF =  0xBF
	}

	
	
   
    switch(key_t.state )
	{
		case start:
		{
			if(key_t.read != _KEY_ALL_OFF)
			{
				key_t.buffer   = key_t.read; //??:key.buffer = 0xFE  POWER KEY 
				key_t.state    = first;
				key_t.on_time  = 0;
				key_t.off_time = 0;
            
                
			}
			break;
		}
		case first:
		{
			if(key_t.read == key_t.buffer) //  short  key be down ->continunce be pressed key
			{
				if(++key_t.on_time>35 )//25 //10000  0.5us
				{
					//run_t.power_times++;
                    key_t.value = key_t.buffer^_KEY_ALL_OFF; // key.value = 0xFE ^ 0xFF = 0x01
					key_t.on_time = 0;                      //key .value = 0xEF ^ 0XFF = 0X10
                    key_t.state   = second;
                   
                    
				}
			  
			}
			else
			{
				key_t.state   = start;
			}
			break;
		}
		case second:
		{
			if(key_t.read == key_t.buffer) //long key key if be pressed down 
			{
				if(++key_t.on_time>70000)// 80000 long key be down
				{
				    key_t.value = key_t.value|0x80; //key.value = 0x02 | 0x80  =0x82
                    key_t.on_time = 0;
					key_t.state   = finish;
	               
				}
					
			}
			else if(key_t.read == _KEY_ALL_OFF)  // loose hand 
			{
					if(++key_t.off_time> 0) //20//30 don't holding key dithering
					{
						key_t.value = key_t.buffer^_KEY_ALL_OFF; // key.value = 0x1E ^ 0x1f = 0x01
						
						key_t.state   = finish; // loose hand
					}
			}
		   
			break;
		}
		case finish:
		{
		
			reval = key_t.value; // is short time  TIMER_KEY = 0x01  2. long times TIMER_KEY = 0X81

			key_t.state   = end;
         
			break;
		}
		case end:
		{
			if(key_t.read == _KEY_ALL_OFF)
			{
				if(++key_t.off_time>0)//5//10//50 //100
				{
					key_t.state   = start;
                  
				}
			}
			break;
		}
		default:
		{
			key_t.state   = start;
         
			break;
		}
	}
	return  reval;


}

#endif 

/****************************************************************
	*
	*Function Name :void Set_Timing_Temperature_Number_Value(void)
	*Function : set timer timing how many ?
	*Input Parameters :NO
	*Retrurn Parameter :NO
	*
*****************************************************************/

/***********************************************************************************
	 *
	 * Function Name:void set_timer_fun_led_blink(void)
	 * Function:
	 * Input Ref:NO
	 * Return Ref:NO
	 *
************************************************************************************/





/****************************************************************
	*
	*Function Name :void mode_key_handler(void) 
	*Function : 
	*Input Parameters :NO
	*Retrurn Parameter :NO
	*
*****************************************************************/
#if 0

typedef struct {
    uint8_t *flag;          // 按键状态标志（也可以用作计时器）
    uint16_t press_time;    // 记录按键按下的时间（单位ms）
    uint16_t long_press_threshold; // 长按阈值（100ms）
    void (*onShortPress)(void);    // 短按回调
    void (*onLongPress)(void);     // 长按回调
} KeyHandler;

void handle_key(KeyHandler *handler) {
    if (*(handler->flag) == 1) { // 如果按键按下
        handler->press_time += 10; // 假设每10ms调用一次此函数（模拟时间累积）

        // 长按检测（≥100ms）
        if (handler->press_time >= handler->long_press_threshold) {
            if (handler->onLongPress) {
                handler->onLongPress(); // 触发长按回调
            }
        }
    } 
    else { // 按键释放
        // 短按检测（<100ms）
        if (handler->press_time > 0 && handler->press_time < handler->long_press_threshold) {
            if (handler->onShortPress) {
                handler->onShortPress(); // 触发短按回调
            }
        }
        handler->press_time = 0; // 重置计时
    }
}


void short_press_action() {
    printf("Short Press (<100ms)\n");
}

void long_press_action() {
    printf("Long Press (≥100ms)\n");
}

int main() {
    uint8_t key_flag = 0; // 0=释放, 1=按下
    KeyHandler key = {
        .flag = &key_flag,
        .press_time = 0,
        .long_press_threshold = 100, // 100ms长按阈值
        .onShortPress = short_press_action,
        .onLongPress = long_press_action
    };

    // 模拟按键按下（持续110ms）
    *(key.flag) = 1;
    for (int i = 0; i < 11; i++) { // 11次×10ms=110ms
        handle_key(&key);
    }

    // 模拟按键释放
    *(key.flag) = 0;
    handle_key(&key); // 会触发长按回调

    return 0;
}

#endif 

