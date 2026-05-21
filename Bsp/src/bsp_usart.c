#include "bsp.h"

typedef struct Msg
{
	
	uint8_t  usData[12];
 
    uint8_t  rx_data_counter;
   
    uint8_t  bcc_check_code;
	uint8_t  data_lenght;
  
}MSG_T;

MSG_T   gl_tMsg; /* 定义丢�个结构体用于消息队列 */



uint8_t check_code;


/********************************************************************************
	**
	*Function Name:void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
	*Function :UART callback function  for UART interrupt for receive data
	*Input Ref: structure UART_HandleTypeDef pointer
	*Return Ref:NO
	*
*******************************************************************************/
void usart2_rx_data(uint8_t data)
{
   static uint8_t state=0;
      inputBuf[0] = data;
     switch(state){ 
		case 0:  //#0
			if(inputBuf[0] == 0x5A){  // 0x5A --main board singla
               gl_tMsg.rx_data_counter=0;
               gl_tMsg.usData[gl_tMsg.rx_data_counter] = inputBuf[0];
				state=1; //=1

             }
            else
                state=0;
		break;

       
		case 1: //#1

          
              /* 初始化结构体指针 */
               gl_tMsg.rx_data_counter++;
		     
	          gl_tMsg.usData[gl_tMsg.rx_data_counter] = inputBuf[0];

			  if(gl_tMsg.usData[gl_tMsg.rx_data_counter] ==0xFE  && gl_tMsg.rx_data_counter > 4){
                     
                    state = 2 ;
                          
              }

	    break;


		case 2:
              
			    gl_tMsg.rx_data_counter++;

                gl_tMsg.data_lenght = gl_tMsg.rx_data_counter;

				gl_tMsg.usData[gl_tMsg.rx_data_counter] = inputBuf[0];

				gl_tMsg.bcc_check_code=inputBuf[0];
				
                gl_tMsg.rx_data_counter =0;
				state = 0;

                semaphore_isr();

              
            break;


	   }

     
}


/**
* @brief  call back example,
* @notice 
* @param
*/
uint8_t check_code ;

void decoder_handler(void)
{
    
	
	check_code =  bcc_check(gl_tMsg.usData,gl_tMsg.data_lenght);

	if(check_code == gl_tMsg.bcc_check_code ){

	 receive_data_from_mainboard(gl_tMsg.usData);

	}


}


/**
* @brief  call back example,
* @notice //The second display board baud rate = 9600bps  usart1
* @param
*/
void usart2_dma_send(uint8_t *txbuf,uint16_t txlen)
{

    LL_DMA_DisableChannel(DMA1,LL_DMA_CHANNEL_1);

    LL_DMA_ConfigAddresses(DMA1,LL_DMA_CHANNEL_1,
                            (uint32_t)txbuf,
                            (uint32_t)&USART2->TDR,
                            LL_DMA_DIRECTION_MEMORY_TO_PERIPH);
    LL_DMA_SetDataLength(DMA1,LL_DMA_CHANNEL_1,txlen);

    LL_DMA_ClearFlag_TC1(DMA1);//TC1 -> clear transfer complete flag
    LL_DMA_EnableChannel(DMA1,LL_DMA_CHANNEL_1);

    LL_USART_EnableDMAReq_TX(USART2);

}



