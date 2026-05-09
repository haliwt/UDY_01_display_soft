#include "bsp.h"

#define TEMPERATURE_HIGH_THRESHOLD  39  // 高温阈值
#define TEMPERATURE_LOW_THRESHOLD   38  // 低温阈值
#define TEMPERATURE_DIFF_THRESHOLD  1   // 

#define CHECK_TIME_THRESHOLD_4S      3   //温度差阈值

//static void tempeature_compareison_value_handler(void);
//static void handleSetTemperatureControl(void) ;
//static void handleDefaultTemperatureControl(void);

uint8_t current_temperature;
/*********************************************************************************************
	*
	*Function Name :void disp_smg_blink_set_tempeature_value(void)
	*Function : set timer timing how many ?
	*Input Parameters :NO
	*Retrurn Parameter :NO
	*
**********************************************************************************************/
void disp_smg_blink_set_tempeature_value(void)
{

    #if 0


//   static uint8_t counter_times;
	  //waiting for 4 s 
	 if(run_t.gTimer_set_up_temperature_value > 2 && key_t.key_set_temperature_flag==1 && (gpro_t.set_timer_timing_doing_value==0 || gpro_t.set_timer_timing_doing_value==3)){
			
		 run_t.gTimer_display_dht11 = 0;
		 key_t.key_set_temperature_flag =2;

			 
          //temperature of smg of LED blink .
		   TM1639_Display_Temperature(gpro_t.set_up_temperature_value);
		 
          
			gpro_t.set_temp_value_success=1;
			
		   
	
              
             gpro_t.gTimer_set_temp_counter=6;
		     handleSetTemperatureControl();
			
          	  // Display_DHT11_Value();
             run_t.gTimer_display_dht11 = 4;
             run_t.gTimer_set_up_temperature_value =0;
		  
	     }
	     else{

			 if(key_t.key_set_temperature_flag ==2  && gpro_t.set_temp_value_success==1){

			         handleSetTemperatureControl();


		     }
			 else if(gpro_t.set_temp_value_success==0 && key_t.key_set_temperature_flag !=1 ){

			        handleDefaultTemperatureControl();


			 }
		 }
     #endif 
}

#if 0
void tempeature_compareison_value_handler(void)
{
	
       if(gpro_t.set_temp_value_success==1){
		
		       handleSetTemperatureControl();

			
        }
		else if(gpro_t.set_temp_value_success==0){ //don't set temperature value 
				handleDefaultTemperatureControl();
		 
		}
 }
#endif 
/******************************************************************************
	*
	*Function Name:static void handleTemperatureControl(void) 
	*Funcion: // 处理温度控制逻辑
	*Input Ref: NO
	*Return Ref:NO
	*
******************************************************************************/
#if 0
static void handleSetTemperatureControl(void) 
{
	
	//static uint8_t first_high_flag;
   
    if( gpro_t.gTimer_set_temp_counter >= CHECK_TIME_THRESHOLD_4S) { // 4秒
          gpro_t.gTimer_set_temp_counter =0;
         current_temperature = run_t.ntc_tem[0];//readTemperature();

        if (gpro_t.set_up_temperature_value <= current_temperature ){
            //run_t.gDry = 0;
		   
		    //LED_DRY_OFF();
	
		    //notice main board dry state 
			SendData_Set_Command(0x22, 0x00); //buzzer don't sound .
			tx_thread_sleep(10);
		    
		
			
        }
        else if ((gpro_t.set_up_temperature_value) > current_temperature ) {

		        
          
					//run_t.gDry = 1;
					//LED_DRY_ON();

					SendData_Set_Command(0x22, 0x01); //buzzer don't sound .干燥功能
				    tx_thread_sleep(10);
	              
            	
			
		   }
			
        }
	
}
#endif 
/******************************************************************************
	*
	*Function Name:static void handleDefaultTemperatureControl(void)
	*Funcion: 处理默认温度控制逻辑
	*Input Ref: NO
	*Return Ref:NO
	*
******************************************************************************/
#if 0
static void handleDefaultTemperatureControl(void) 
{
    

	static uint8_t default_first_close_dry;
  
    if (gpro_t.gTimer_set_temp_counter > 3) { // 3秒
         gpro_t.gTimer_set_temp_counter = 0;
        current_temperature = run_t.ntc_tem[0];

        if(current_temperature > 39) {
			default_first_close_dry=1;
			run_t.gDry = 0;
            
            LED_DRY_OFF();
			
		   
		     SendData_Set_Command(0x22, 0x0);//sendDisplayCommand(0x02,0x0); // send data to the second displayboard .关闭干燥功能
			 tx_thread_sleep(10);
			
			
        } 
		else{

		     if(default_first_close_dry==0 && current_temperature <=39){

	

					//run_t.gDry= 1;

			        //LED_DRY_ON();
				
			  	    SendData_Set_Command(0x22, 0x01); //sendDisplayCommand(0x02,0x01); // 打开干燥功能
					tx_thread_sleep(10);
				
				

			  }
              else if (current_temperature < 38 && default_first_close_dry==1) {
	        

			

					 //run_t.gDry= 1;

				    // LED_DRY_ON();
					
				      SendData_Set_Command(0x22, 0x01);//sendDisplayCommand(0x02,0x01); // 第二个显示板，打开干燥功能
				     tx_thread_sleep(10);
				 }
				 
	            

	    }
   }
}

#endif 
