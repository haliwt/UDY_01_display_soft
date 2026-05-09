#ifndef __BSP_USART_H
#define __BSP_USART_H
#include "main.h"




void usart2_rx_data(uint8_t data);

void decoder_handler(void);

void usart2_dma_send(uint8_t *txbuf,uint16_t txlen);


#endif 

