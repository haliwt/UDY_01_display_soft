#include "bsp.h"


#define KEY_POWER_SHORT  (1UL << 0)
#define KEY_POWER_LONG   (1UL << 1)


#define KEY_DRY_SHORT   (1UL << 2)

#define KEY_PLASMA_SHORT    (1UL<< 3)

#define KEY_MOUSE_SHORT      (1UL << 4)


#define KEY_UP_SHORT     (1UL << 5)
#define KEY_UP_LONG      (1UL<< 8 )

#define KEY_DOWN_SHORT   (1UL << 6)
#define KEY_DOWN_LONG    (1UL << 7)


#define DEBUG_ENABLE    0



#define STACK_SIZE_UI     896//896//1024 
#define STACK_SIZE_KEY    256//512
#define STACK_SIZE_DEC    512//512//
#define STACK_SIZE_EVENT  512//512//256

__attribute__((aligned(8))) static UCHAR stack_ui_pro[STACK_SIZE_UI];
__attribute__((aligned(8))) static UCHAR stack_key_pro[STACK_SIZE_KEY];
__attribute__((aligned(8))) static UCHAR stack_dec_pro[STACK_SIZE_DEC];
__attribute__((aligned(8))) static UCHAR stack_key_event[STACK_SIZE_EVENT];



static void vTaskUiPro(ULONG thread_input);
static void vTaskKeyPro(ULONG thread_input);
static void vTaskDecoderPro(ULONG thread_input);
static void vTaskKeyEvent(ULONG thread_input);


static TX_THREAD  thread_ui;
static TX_THREAD  thread_key;
static TX_THREAD  thread_decoder;
static TX_THREAD  thread_key_event;


TX_EVENT_FLAGS_GROUP key_event;

TX_SEMAPHORE      decoder_semaphore;

#if DEBUG_ENABLE

static void debug_stack_ui_check(void);

static void debug_stack_key_check(void);

static void debug_stack_decoder_check(void);

static void debug_stack_key_event_check(void);

ULONG unused_ui,unused_key,unused_decoder,unused_event ;
#endif 
//static uint16_t power_cnt = 0;

//uint8_t key_power_long_f;

/**
*@brief 
*@param
*@notice
*@retval
**/
void tx_application_define(VOID * first_unused_memory)
{



#if DEBUG_ENABLE
    /* 2. 只有当 stack_msg_pro 是全局定义的静态数组时，这样写才有效 */
    memset(stack_ui_pro, 0xEF, sizeof(stack_ui_pro));
    memset(stack_key_pro, 0xEF, sizeof(stack_key_pro));
	memset(stack_dec_pro, 0xEF, sizeof(stack_dec_pro));
	memset(stack_key_event, 0xEF, sizeof(stack_key_event));
#endif


	//tx_event_flags_create(&commEventFlags,"commEventFlags");
  tx_semaphore_create(&decoder_semaphore,"decoderSemaphore",0);
  tx_event_flags_create(&key_event, "key_event");

  tx_thread_create(&thread_decoder,
  					"DecoderPro",
  					vTaskDecoderPro,
  					0,
  					stack_dec_pro,
  					STACK_SIZE_DEC,
  					2,
  					2,
  					TX_NO_TIME_SLICE,
  					TX_AUTO_START);


   tx_thread_create(&thread_ui,
   					"MsgPro",
   					vTaskUiPro,
   					0,
   					stack_ui_pro,
   					STACK_SIZE_UI,
   					3,
   					3,
   					TX_NO_TIME_SLICE,
   					TX_AUTO_START);

	tx_thread_create(&thread_key,
					"KeyPro",
					vTaskKeyPro,
					0,
					stack_key_pro,
					STACK_SIZE_KEY,
					0,
					0,
					TX_NO_TIME_SLICE,
					TX_AUTO_START);
	
	 tx_thread_create(&thread_key_event, 		   /* ??????? */	  
					 "KeyEvent",				    /* ??? */
					  vTaskKeyEvent,				/* ???????? */
					  0,							/* ???????? */
					  stack_key_event,				/* ????? */
					  STACK_SIZE_EVENT,				/* ?????? */  
					  1,							/* ?????*/
					  1,							/* ?????? */
					  TX_NO_TIME_SLICE, 			/* ?????? */
					  TX_AUTO_START);				/* ??????? */
   
  

}

/**********************************************************************************************************
*   FunctionName: static void vTaskRunPro(void *pvParameters)
*	????: ????xTaskNotifyWait????vTaskTaskUserIF?�?�???????�?
*	�?    �?: pvParameters ?????????�?�???
*	�? �? �?: �?
*   �? �? �?: 1  
*   priority : 1  (?priority is 2,number is large priority is large???????uCOS??)
**********************************************************************************************************/
static void vTaskDecoderPro(ULONG thread_input)
{
    (void)thread_input;
	while(1)
    {
        if(tx_semaphore_get(&decoder_semaphore,TX_WAIT_FOREVER)==TX_SUCCESS){	   
				
            decoder_handler();
			#if DEBUG_ENABLE
              debug_stack_decoder_check();
            #endif 
				
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
	static uint8_t function_counter = 0; // Counter for time-sharing functions

	while(1)
    {
     
        if(run_t.gPower_On == power_on){
 
		  power_on_run_handler();

	    }
		else{
	        // In power off mode, always run power_off_run_handler
	        power_off_run_handler();
        }

		 LL_IWDG_ReloadCounter(IWDG);
		 #if DEBUG_ENABLE
              debug_stack_ui_check();
         #endif 
	     tx_thread_sleep(1);//20ms //vTaskDelay(10);
   } //wihile(1) ---end
}
/**********************************************************************************************************
*
*	Function Name: vTaskStart
*	Function: 
*	Input Ref: pvParameters ?????????�?�???
*	Return Ref:
*	priority: 3  (?�?�???????????uCOS??)
*
**********************************************************************************************************/



static void vTaskKeyPro(ULONG thread_input)
{
  (void)thread_input;  /* ?????????? */
    
    static uint8_t dry_cnt = 0;
	static uint16_t plasma_cnt = 0;
	static uint16_t mouse_cnt = 0;
    static uint16_t up_cnt = 0;
    static uint16_t down_cnt = 0;
    static uint16_t power_cnt = 0;
	// 新增：用于长按跑数的速度控制计数器
   static uint16_t up_repeat_cnt = 0;
   static uint16_t down_repeat_cnt = 0;

    const uint16_t LONG_PRESS_TIME = 40;   // 300 * 10ms = 3000ms
    const uint16_t LONG_PRESS_COUNTER = 20;
	const uint16_t REPEAT_INTERVAL = 1;       // 每 5 次循环跑数一次（约 30ms）
	
    while(1)
    {
      

	  
     if(DEC_KEY_VALUE()==KEY_DOWN && run_t.gPower_On == power_on){
           
           down_cnt++;
	        if (down_cnt >= LONG_PRESS_COUNTER)
            {
				 down_cnt = LONG_PRESS_COUNTER;

			     // 已经进入长按状态，启动连发跑数计数器
                 down_repeat_cnt++;
				if (down_repeat_cnt >REPEAT_INTERVAL)
                {
                    down_repeat_cnt=0;
				
					tx_event_flags_set(&key_event, KEY_DOWN_SHORT, TX_OR);
                }
            }
            
	  }
	  else if(down_cnt > 0){
	  	    if(down_cnt < LONG_PRESS_COUNTER)
			tx_event_flags_set(&key_event, KEY_DOWN_SHORT, TX_OR);
	      down_repeat_cnt =0;
		  down_cnt = 0;
      }
	  else if(ADD_KEY_VALUE()==KEY_DOWN && run_t.gPower_On == power_on){
          
           up_cnt++;
		   if(up_cnt >= LONG_PRESS_COUNTER)
           {
               up_cnt = LONG_PRESS_COUNTER +1;
		        // 跑数模式：每 REPEAT_INTERVAL 次触发一次
                up_repeat_cnt++;
                if (up_repeat_cnt >REPEAT_INTERVAL)
                {
                    up_repeat_cnt=0;
				
					tx_event_flags_set(&key_event, KEY_UP_SHORT, TX_OR);
                }
            }
	  
	  }
	  else if(up_cnt > 0){
	  	    if(up_cnt <LONG_PRESS_COUNTER)
            tx_event_flags_set(&key_event, KEY_UP_SHORT, TX_OR);
			
			up_repeat_cnt=0;
             up_cnt = 0;
	  }
	  else if(DRY_KEY_VALUE()==KEY_DOWN && run_t.gPower_On == power_on){

	       dry_cnt++ ;
		 
	  }
	  else  if(dry_cnt > 0){
                tx_event_flags_set(&key_event, KEY_DRY_SHORT, TX_OR); 

		 dry_cnt = 0;

	  }
	  else if(PLASMA_KEY_VALUE()==KEY_DOWN && run_t.gPower_On == power_on){
            plasma_cnt ++;    
            
	  }
	  else if(plasma_cnt > 0){
			 tx_event_flags_set(&key_event, KEY_PLASMA_SHORT, TX_OR); 
	  
			   plasma_cnt = 0;


	  }
	  else if(MOUSE_KEY_VALUE()==KEY_DOWN && run_t.gPower_On == power_on){
         
           mouse_cnt ++ ; 
	  }
	  else if(mouse_cnt > 0){
			tx_event_flags_set(&key_event, KEY_MOUSE_SHORT, TX_OR); 
		   
			mouse_cnt = 0;

	  }
	  else if(POWER_KEY_VALUE() == KEY_DOWN){
         
            power_cnt++;
            if(power_cnt == LONG_PRESS_TIME && run_t.gPower_On == power_on){
				
                tx_event_flags_set(&key_event, KEY_POWER_LONG, TX_OR);
             }

	  }
	  else if(power_cnt > 0){
			   if(power_cnt < LONG_PRESS_TIME){
                  tx_event_flags_set(&key_event, KEY_POWER_SHORT, TX_OR);
			   	}
				
			
            power_cnt = 0;
	  	}

	  

	  #if DEBUG_ENABLE
              debug_stack_key_check();
        #endif 

      tx_thread_sleep(6);//vTaskDelay(20);     
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

			 
			   power_key_long_handler();
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
         tx_thread_sleep(2);
		  #if DEBUG_ENABLE
              debug_stack_key_event_check();
          #endif 
		 
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
void power_on_off_tx_task(void)
{
   tx_event_flags_set(&key_event, KEY_POWER_SHORT, TX_OR);
}


#if DEBUG_ENABLE
static void debug_stack_ui_check(void)
{
    ULONG i;
   // ULONG unused = 0;
   ULONG temp_unused = 0; // ??????????

  
    // ??????????/?????????? 0xEF
    for (i = 0; i < STACK_SIZE_UI; i++)
    {
        if (stack_ui_pro[i] == 0xEF)
            temp_unused++;
        else
            break; 
    }
  
 
	
	unused_ui = temp_unused;  // ??????????????? Watch ????
    // ??? unused ??????�???�
    // ?? unused < 100 ????? G030 ?????
}

static void debug_stack_key_check(void)
{
    ULONG i;
   // ULONG unused = 0;
   ULONG temp_unused = 0; // ??????????


    // ??????????/?????????? 0xEF
    for (i = 0; i < STACK_SIZE_KEY; i++)
    {
        if (stack_key_pro[i] == 0xEF)
            temp_unused++;
        else
            break; 
    }
    unused_key = temp_unused;  // ??????????????? Watch ????
    // ??? unused ??????�???�
    // ?? unused < 100 ????? G030 ?????
}

static void debug_stack_decoder_check(void)
{
    ULONG i;
   // ULONG unused = 0;
   ULONG temp_unused = 0; // ??????????


    // ??????????/?????????? 0xEF
    for (i = 0; i < STACK_SIZE_DEC; i++)
    {
        if (stack_dec_pro[i] == 0xEF)
            temp_unused++;
        else
            break; 
    }
    unused_decoder = temp_unused;  // ??????????????? Watch ????
    // ??? unused ??????�???�
    // ?? unused < 100 ????? G030 ?????
}

static void debug_stack_key_event_check(void)
{
    ULONG i;
   // ULONG unused = 0;
   ULONG temp_unused = 0; // ??????????


    // ??????????/?????????? 0xEF
    for (i = 0; i < STACK_SIZE_EVENT; i++)
    {
        if (stack_key_event[i] == 0xEF)
            temp_unused++;
        else
            break; 
    }
    unused_event = temp_unused;  // ??????????????? Watch ????
    // ??? unused ??????�???�
    // ?? unused < 100 ????? G030 ?????
}


#endif 







