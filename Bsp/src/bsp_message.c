/*
 * bsp_message.c
 *
 *  Created on: 2025年2月19日
 *      Author: Administrator
 */
#include "bsp.h"

static void copy_cmd_data_from_mainboard(uint8_t *pdata);




/******************************************************************************
*
*Function Name:void receive_data_from_mainboard(uint8_t *pdata,uint8_t len)
*Funcion: handle of tall process
*Input Ref:
*Return Ref:
*
******************************************************************************/
void receive_data_from_mainboard(uint8_t *pdata)
{
    
   //static uint8_t power_on_counter;
    switch(pdata[2]){

     case 0:


     break;

     case  power_cmd:
          

            if(pdata[3]== 0x01){
			run_t.gPower_On = power_on;
            run_t.power_on_step =0;
     
            power_on_handler();
            if(gpro_t.dma_tx_done ==1)SendData_Set_Command(0x11,0x01); //0x11 :send to main has the second display board exit.
			//tx_thread_sheep(10);
           }
           else{ //power off

            run_t.gPower_On = power_off;
            run_t.power_on_step =0;
            if(gpro_t.dma_tx_done ==1)SendData_Set_Command(0x11,0x01);
			//tx_thread_sheep(10);
           
           }
          

     break;

	 case 0x21: //smart phone power on or off that App timer .
       

		   if(pdata[3]==0x01){
             run_t.gPower_On = power_on;
			 power_on_handler();
		   	}
		    else{

				run_t.gPower_On = power_off;
                run_t.power_on_step =0;

			}
           
             
         
       
     break; 

	 case 0x20: //手机定时开机，发送的数据，3个。

	     if(pdata[3]==0x0F){ //power on by smart phone APP

		   if(pdata[4]==0x03){

                run_t.gDry =pdata[5];
				if(run_t.gDry == 0){
                  
                  LED_DRY_OFF();
				}
				else{
                   LED_DRY_ON();
				}

				run_t.gPlasma=pdata[6];
				if(run_t.gPlasma ==1){
                  LED_PLASMA_ON();
				}
				else{
				   LED_PLASMA_OFF();

				}
		       
                run_t.gMouse =pdata[7];
				if(run_t.gMouse==1){
					LED_MOUSE_ON();
				}
				 else{
                   LED_MOUSE_OFF();

				}



		   	}

	     }
	 


	 break;

	 case 0x23: //smart phone app timer opower of of dry 
	   if(pdata[3] == 0x00){
	 
		   if(pdata[4]== 0x01){

		      run_t.gDry=1;
			  LED_DRY_ON();

		   	}
		    else{

	            
	            run_t.gDry =0;
			    LED_DRY_OFF();   

			}


		 }


	 break;

     case dry_cmd: //PTC打开关闭指令
       
     

	   if(pdata[3]== 0x01 && run_t.gPower_On == power_on){

            run_t.gDry =1 ;//&& run_t.gPlasma ==1  && run_t.gUltransonic==1
            LED_DRY_ON();
        }
        else if(pdata[3] == 0x0){

            run_t.gDry =0;
		    LED_DRY_OFF();
          

        }
    	
     break;

     case plasma_cmd: //PLASMA 打开关闭指令

		
		
			if(pdata[3]== 0x01){
	          
	        run_t.gPlasma =1;


	        }
	        else if(pdata[3] == 0x0){
	          
	         run_t.gPlasma =0;

	        }
		

     break;


      case mouse_cmd: //ultrasonic  打开关闭指令

        
	     if(pdata[3]== 0x01){
	           
	           run_t.gMouse = 1;

	        }
	        else { //close
	         run_t.gMouse = 0;
	        }

		


     break;


	case temp_warning: //temperature of high warning.

			
			if(pdata[3]== 0x01){
                
	            run_t.ptc_warning = 1;
		
	            run_t.gDry =0;
			    LED_DRY_OFF();
	           

	        }
	        else if(pdata[4] == 0x0){ //close

	           run_t.ptc_warning = 0;


	        }

	    

      break;

      case fan_warning: //fan of default of warning.

       

            if(pdata[3]==1){
            run_t.fan_warning = 1;
        
           run_t.gDry =0;
		   LED_DRY_OFF();
          
           }
           else if(pdata[3] == 0x0){ //close

           run_t.fan_warning = 0;


          }


      break;

	 case 0x1A: //read real temperature value
	
			if(pdata[4] == 0x01){ //数据,one 

	 			run_t.ntc_tem[0] = pdata[5]; //temperature value
	 			//TM1639_Display_Temperature(run_t.ntc_tem[0]);
			}
			else if(pdata[4] == 0x02){


			}
	 

	 break;

	 case copy_cmd: // copy send cmd acknowlege
          copy_cmd_data_from_mainboard(pdata);

     break;

     }

 }


static void copy_cmd_data_from_mainboard(uint8_t *pdata )
{
    
    switch(pdata[3]){

    case CMD_POWER : //power_on 
    

	 if(pdata[4]==0x01){
	 	//run_t.gPower_On = power_on;
        //power_on_handler();
         gpro_t.rx_data_power_on_f =1;

     }
     else if(pdata[4]==0){ //power offf

        //run_t.gPower_On = power_off;
        //run_t.power_off_flag=0;
       
        gpro_t.rx_data_power_on_f = 2;
     }
    
    break;

    case ack_ptc:
    
    if(pdata[4]==1){ //if pdata[4] == 0x0F ,传输的是数据。

   
		 run_t.gDry =1 ;//&& run_t.gPlasma ==1  && run_t.gUltransonic==1
        
		 LED_DRY_ON();
    }
    else{
     
	
      run_t.gDry =0;
	  LED_DRY_OFF();

    }

    

    break;

	case 0x22:
		
		    if(pdata[4]==1){

		       
				 run_t.gDry =1 ;//&& run_t.gPlasma ==1  && run_t.gUltransonic==1
		        
				 LED_DRY_ON();
		    }
		    else{

			
		      run_t.gDry =0;
			  LED_DRY_OFF();

		    }
        
   break;

    case ack_plasma:
	

	    if(pdata[4]==1){

	       
	    }
	    else{
	    

	    }
	
    break;

    case ack_with_buzzer:
        if(pdata[5] == 1){  //buzzer answer command

           

        }
        else{
         


        }
     break;
    }


}


/******************************************************************************
*
*Function Name:uint8_t bcc_check(const unsigned char *data, int len)
*Funcion: BCC check code
*Input Ref: data:input data, len: data of length
*Return Ref:
*
******************************************************************************/
uint8_t bcc_check(const unsigned char *data, int len)
{
    unsigned char bcc = 0;
    for (int i = 0; i < len; i++) {
        bcc ^= data[i];
    }
    return bcc;
}




