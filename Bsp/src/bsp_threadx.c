#include "bsp.h"


#define KEY_POWER_SHORT  (1UL << 0)
#define KEY_POWER_LONG   (1UL << 1)


#define KEY_DRY_SHORT   (1UL << 2)

#define KEY_PLASMA_SHORT    (1UL<< 3)

#define KEY_MOUSE_SHORT      (1UL << 4)


#define KEY_UP_SHORT     (1UL << 5)

#define KEY_DOWN_SHORT   (1UL << 6)




#define STACK_SIZE_UI     1024 
#define STACK_SIZE_KEY    256//512
#define STACK_SIZE_DEC    512//
#define STACK_SIZE_EVENT  256




static TX_THREAD  thread_ui;
static TX_THREAD  thread_key;
static TX_THREAD  thread_decoder;
static TX_THREAD  thread_key_event;


static UCHAR stack_ui_pro[STACK_SIZE_UI];
static UCHAR stack_key_pro[STACK_SIZE_KEY];
static UCHAR stack_decoder_pro[STACK_SIZE_DEC];
static UCHAR stack_key_event[STACK_SIZE_EVENT];





static void vTaskUiPro(ULONG thread_input);
static void vTaskKeyPro(ULONG thread_input);
static void vTaskDecoderPro(ULONG thread_input);
static void vTaskKeyEvent(ULONG thread_input);


TX_EVENT_FLAGS_GROUP key_event;

TX_SEMAPHORE      decoder_semaphore;



/**
*@brief 
*@param
*@notice
*@retval
**/
void tx_application_define(VOID * first_unused_memory)
{
	//tx_event_flags_create(&commEventFlags,"commEventFlags");
  tx_semaphore_create(&decoder_semaphore,"decoderSemaphore",0);
  tx_event_flags_create(&key_event, "key_event");

  tx_thread_create(&thread_decoder,
  					"DecoderPro",
  					vTaskDecoderPro,
  					0,
  					stack_decoder_pro,
  					STACK_SIZE_DEC,
  					0,
  					0,
  					TX_NO_TIME_SLICE,
  					TX_AUTO_START);


   tx_thread_create(&thread_ui,
   					"MsgPro",
   					vTaskUiPro,
   					0,
   					stack_ui_pro,
   					STACK_SIZE_UI,
   					2,
   					2,
   					TX_NO_TIME_SLICE,
   					TX_AUTO_START);

	tx_thread_create(&thread_key,
					"KeyPro",
					vTaskKeyPro,
					0,
					stack_key_pro,
					STACK_SIZE_KEY,
					1,
					1,
					TX_NO_TIME_SLICE,
					TX_AUTO_START);
	
	 tx_thread_create(&thread_key_event, 		   /* 任务控制块地址 */	  
					 "KeyEvent",				    /* 任务名 */
					  vTaskKeyEvent,				/* 启动任务函数地址 */
					  0,							/* 传递给任务的参数 */
					  stack_key_event,				/* 堆栈基地址 */
					  STACK_SIZE_EVENT,				/* 堆栈空间大小 */  
					  2,							/* 任务优先级*/
					  2,							/* 任务抢占阀值 */
					  TX_NO_TIME_SLICE, 			/* 不开启时间片 */
					  TX_AUTO_START);				/* 创建后立即启动 */
   
  

}






/**********************************************************************************************************
*   FunctionName: static void vTaskRunPro(void *pvParameters)
*	功能说明: 使用函数xTaskNotifyWait接收任务vTaskTaskUserIF发�?�的事件标志位设�?
*	�?    �?: pvParameters 是在创建该任务时传�?�的形参
*	�? �? �?: �?
*   �? �? �?: 1  

*   priority : 1  (数priority is 2,number is large priority is large值越低，这个跟uCOS相反)
**********************************************************************************************************/
static void vTaskDecoderPro(ULONG thread_input)
{
    (void)thread_input;
	

    while(1)
    {


		if(tx_semaphore_get(&decoder_semaphore,TX_WAIT_FOREVER)==TX_SUCCESS){	   
				
            decoder_handler();
				
        }
 }
}
/**
*@brief 
*@param
*@notice
*@retval
**/
static void vTaskUiPro(ULONG thread_input)
{
	(void)thread_input;

	while(1)
    {
     
    //process_keys() ;
	
	if(run_t.gPower_On == power_on){


	   power_on_run_handler();
     
       set_timer_fun_led_blink();
  
       Display_TimeColon_Blink_Fun();

	   disp_ntc_temperature_value();

      }
	  else{

	    power_off_run_handler();

	  }


	  
       tx_thread_sleep(1);//10ms //vTaskDelay(10);
     

       } //wihile(1) ---end
}
/**********************************************************************************************************
*
*	Function Name: vTaskStart
*	Function: 
*	Input Ref: pvParameters 是在创建该任务时传�?�的形参
*	Return Ref:
*	priority: 3  (数�?�越小优先级越低，这个跟uCOS相反)
*
**********************************************************************************************************/
static void vTaskKeyPro(ULONG thread_input)
{
  (void)thread_input;  /* 消除未使用的参数警告 */
    
    static uint16_t dry_cnt = 0;
	static uint16_t plasma_cnt = 0;
	static uint16_t mouse_cnt = 0;
    static uint16_t up_cnt = 0;
    static uint16_t down_cnt = 0;
    static uint16_t power_cnt = 0;

    const uint16_t LONG_PRESS_TIME = 90;   // 300 * 10ms = 3000ms
	
    while(1)
    {
      if(POWER_KEY_VALUE() == KEY_DOWN){
         
            power_cnt++;
            if(power_cnt == LONG_PRESS_TIME && run_t.gPower_On == power_on){
                tx_event_flags_set(&key_event, KEY_POWER_LONG, TX_OR);
             }

	  }
	  else{
          if(power_cnt > 1 && power_cnt < LONG_PRESS_TIME)
                tx_event_flags_set(&key_event, KEY_POWER_SHORT, TX_OR);

            power_cnt = 0;

	  }
	  
	  if(DEC_KEY_VALUE()==KEY_DOWN && run_t.gPower_On == power_on){
           
           down_cnt++;
	  }
	  else{
	       if(down_cnt > 1 && down_cnt < LONG_PRESS_TIME)
					 tx_event_flags_set(&key_event, KEY_DOWN_SHORT, TX_OR);
	  
				 down_cnt = 0;


	  }

	  if(ADD_KEY_VALUE()==KEY_DOWN && run_t.gPower_On == power_on){
          
           up_cnt++;
          
	  }
	  else{
          if(up_cnt > 1 && up_cnt < LONG_PRESS_TIME)
                tx_event_flags_set(&key_event, KEY_UP_SHORT, TX_OR);

            up_cnt = 0;

	  }

	  if(DRY_KEY_VALUE()==KEY_DOWN && run_t.gPower_On == power_on){

	       dry_cnt++ ;
            
	  }
	  else{
        if(dry_cnt > 1 && up_cnt < LONG_PRESS_TIME)
                tx_event_flags_set(&key_event, KEY_DRY_SHORT, TX_OR); 

		 dry_cnt = 0;

	  }

	  if(PLASMA_KEY_VALUE()==KEY_DOWN && run_t.gPower_On == power_on){
            plasma_cnt ++;    
            
	  }
	  else{
	          if(plasma_cnt > 1 && up_cnt < LONG_PRESS_TIME)
					  tx_event_flags_set(&key_event, KEY_PLASMA_SHORT, TX_OR); 
	  
			   plasma_cnt = 0;


	  }

	  if(MOUSE_KEY_VALUE()==KEY_DOWN && run_t.gPower_On == power_on){
         
           mouse_cnt ++ ; 
	  }
	  else{
	      if(mouse_cnt > 1 && up_cnt < LONG_PRESS_TIME)
						   tx_event_flags_set(&key_event, KEY_MOUSE_SHORT, TX_OR); 
		   
					mouse_cnt = 0;

	  }

      tx_thread_sleep(2);//vTaskDelay(20);     
     }
}

 /**
 *@brief  threadx for task handler key event
 *@param
 *@notice
 *@retval
 *
 **/
 static void vTaskKeyEvent(ULONG thread_input)
 {
   (void)thread_input;
   ULONG flags;
   UINT status;
   while(1){
 
 
	  status = tx_event_flags_get(&key_event,
							0xFFFFFFFF,
							TX_OR_CLEAR,
							&flags,
							TX_WAIT_FOREVER);//TX_NO_WAIT);//TX_WAIT_FOREVER);//
							
	  if(status == TX_SUCCESS){
 
		 if(flags & KEY_POWER_SHORT){
 
			power_key_short_handler();
 
		 }
		 else if(flags & KEY_POWER_LONG){
 
			SendData_Set_Command(0x05,0x01); // link wifi of command .
			tx_thread_sleep(10);
 
		 }
		 else if(flags & KEY_UP_SHORT){
			   key_add_fun();
				
		 }	
		 else if(flags & KEY_DOWN_SHORT){
		  
               key_dec_fun();
		 }
		 else if(flags & KEY_DRY_SHORT){
            dry_key_handler() ;

		 }
		 else if(flags & KEY_PLASMA_SHORT){
		     plasma_key_handler() ;


		 }
		 else if(flags & KEY_MOUSE_SHORT){
       
		      mouse_key_handler() ;
	     }
		
	  }
 
		
	}
 }


/*************************************************************************
*
*	Funtion Name:
*	Function: 
*	Input Ref: 
*	Return Ref:
*
**************************************************************************/
void semaphore_isr(void)
{
   //tx_event_flags_set(&commEventFlags,(1<<9),TX_OR);
     tx_semaphore_put(&decoder_semaphore);
}


/**
*@brief 
*@param
*@notice
*@retval
*
**/



#if DEBUG_ENABLE
static void debug_stack_ui_check(void)
{
    ULONG i;
   // ULONG unused = 0;
   ULONG temp_unused = 0; // 使用局部变量进行统计

  
    // 从数组起始位置（栈底/低地址）开始数连续的 0xEF
    for (i = 0; i < STACK_SIZE_UI; i++)
    {
        if (stack_ui_pro[i] == 0xEF)
            temp_unused++;
        else
            break; 
    }
  
 
	
	unused = temp_unused;  // 统计完后再赋值给全局变量，方便 Watch 窗口查看
    // 剩下的 unused 就是你安全的“护城河”
    // 如果 unused < 100 字节，你的 G030 就危险了！
}

static void debug_stack_key_check(void)
{
    ULONG i;
   // ULONG unused = 0;
   ULONG temp_unused = 0; // 使用局部变量进行统计


    // 从数组起始位置（栈底/低地址）开始数连续的 0xEF
    for (i = 0; i < STACK_SIZE_KEY; i++)
    {
        if (stack_start_pro[i] == 0xEF)
            temp_unused++;
        else
            break; 
    }
    unused_key = temp_unused;  // 统计完后再赋值给全局变量，方便 Watch 窗口查看
    // 剩下的 unused 就是你安全的“护城河”
    // 如果 unused < 100 字节，你的 G030 就危险了！
}

static void debug_stack_decoder_check(void)
{
    ULONG i;
   // ULONG unused = 0;
   ULONG temp_unused = 0; // 使用局部变量进行统计


    // 从数组起始位置（栈底/低地址）开始数连续的 0xEF
    for (i = 0; i < STACK_SIZE_DECODER; i++)
    {
        if (stack_decoder_pro[i] == 0xEF)
            temp_unused++;
        else
            break; 
    }
    unused_decoder = temp_unused;  // 统计完后再赋值给全局变量，方便 Watch 窗口查看
    // 剩下的 unused 就是你安全的“护城河”
    // 如果 unused < 100 字节，你的 G030 就危险了！
}

static void debug_stack_key_event_check(void)
{
    ULONG i;
   // ULONG unused = 0;
   ULONG temp_unused = 0; // 使用局部变量进行统计


    // 从数组起始位置（栈底/低地址）开始数连续的 0xEF
    for (i = 0; i < STACK_SIZE_EVENT; i++)
    {
        if (stack_key_event[i] == 0xEF)
            temp_unused++;
        else
            break; 
    }
    unused_event = temp_unused;  // 统计完后再赋值给全局变量，方便 Watch 窗口查看
    // 剩下的 unused 就是你安全的“护城河”
    // 如果 unused < 100 字节，你的 G030 就危险了！
}


#endif 







