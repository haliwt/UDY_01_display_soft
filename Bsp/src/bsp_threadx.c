#include "bsp.h"


#define KEY_POWER_SHORT  (1UL << 0)
#define KEY_POWER_LONG   (1UL << 1)


#define KEY_DRY_SHORT   (1UL << 2)

#define KEY_PLASMA_SHORT    (1UL<< 3)

#define KEY_MOUSE_SHORT      (1UL << 4)


#define KEY_UP_SHORT     (1UL << 5)

#define KEY_DOWN_SHORT   (1UL << 6)




#define STACK_SIZE_UI     896//1024 
#define STACK_SIZE_KEY    256//512
#define STACK_SIZE_DEC    512//512//
#define STACK_SIZE_EVENT  512//512//256





static TX_THREAD  thread_ui;
static TX_THREAD  thread_key;
static TX_THREAD  thread_decoder;
static TX_THREAD  thread_key_event;


static UCHAR stack_ui_pro[STACK_SIZE_UI];
static UCHAR stack_key_pro[STACK_SIZE_KEY];
static UCHAR stack_dec_pro[STACK_SIZE_DEC];
static UCHAR stack_key_event[STACK_SIZE_EVENT];





static void vTaskUiPro(ULONG thread_input);
static void vTaskKeyPro(ULONG thread_input);
static void vTaskDecoderPro(ULONG thread_input);
static void vTaskKeyEvent(ULONG thread_input);


TX_EVENT_FLAGS_GROUP key_event;

TX_SEMAPHORE      decoder_semaphore;

#if DEBUG_ENABLE

static void debug_stack_ui_check(void);

static void debug_stack_key_check(void);

static void debug_stack_decoder_check(void);

static void debug_stack_key_event_check(void);

ULONG unused_ui,unused_key,unused_decoder,unused_event ;
#endif 


/**
*@brief 
*@param
*@notice
*@retval
**/
void tx_application_define(VOID * first_unused_memory)
{



#if DEBUG_ENABLE
    /* 2. åªæœ‰å½“ stack_msg_pro æ˜¯å…¨å±€å®šä¹‰çš„é™æ€æ•°ç»„æ—¶ï¼Œè¿™æ ·å†™æ‰æœ‰æ•ˆ */
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
					1,
					1,
					TX_NO_TIME_SLICE,
					TX_AUTO_START);
	
	 tx_thread_create(&thread_key_event, 		   /* ??????? */	  
					 "KeyEvent",				    /* ??? */
					  vTaskKeyEvent,				/* ???????? */
					  0,							/* ???????? */
					  stack_key_event,				/* ????? */
					  STACK_SIZE_EVENT,				/* ?????? */  
					  2,							/* ?????*/
					  2,							/* ?????? */
					  TX_NO_TIME_SLICE, 			/* ?????? */
					  TX_AUTO_START);				/* ??????? */
   
  

}

/**********************************************************************************************************
*   FunctionName: static void vTaskRunPro(void *pvParameters)
*	????: ????xTaskNotifyWait????vTaskTaskUserIF??????????
*	?    ?: pvParameters ?????????????
*	? ? ?: ?
*   ? ? ?: 1  
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
	     tx_thread_sleep(2);//20ms //vTaskDelay(10);
   } //wihile(1) ---end
}
/**********************************************************************************************************
*
*	Function Name: vTaskStart
*	Function: 
*	Input Ref: pvParameters ?????????????
*	Return Ref:
*	priority: 3  (?????????????uCOS??)
*
**********************************************************************************************************/
static void vTaskKeyPro(ULONG thread_input)
{
  (void)thread_input;  /* ?????????? */
    
    static uint16_t dry_cnt = 0;
	static uint16_t plasma_cnt = 0;
	static uint16_t mouse_cnt = 0;
    static uint16_t up_cnt = 0;
    static uint16_t down_cnt = 0;
    static uint16_t power_cnt = 0;

    const uint16_t LONG_PRESS_TIME = 130;   // 300 * 10ms = 3000ms
	
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

	  #if DEBUG_ENABLE
              debug_stack_key_check();
        #endif 

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

			// Handle long power key press (enter timer setting mode)
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

		  #if DEBUG_ENABLE
              debug_stack_key_event_check();
          #endif 
		 
		 tx_thread_sleep(1);
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
    // ??? unused ?????????
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
    // ??? unused ?????????
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
    // ??? unused ?????????
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
    // ??? unused ?????????
    // ?? unused < 100 ????? G030 ?????
}


#endif 







