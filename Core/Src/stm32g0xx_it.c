/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32g0xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32g0xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "bsp.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern TIM_HandleTypeDef htim16;

/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M0+ Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
   while (1)
  {
  }
  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/******************************************************************************/
/* STM32G0xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32g0xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles DMA1 channel 1 interrupt.
  */
void DMA1_Channel1_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Channel1_IRQn 0 */
  if(LL_DMA_IsActiveFlag_TC1(DMA1) != RESET){
      if(LL_DMA_IsActiveFlag_TC1(DMA1) != RESET) LL_DMA_ClearFlag_TC1(DMA1);

     
       //shut off DMA channel
       LL_DMA_DisableChannel(DMA1,LL_DMA_CHANNEL_1);
     #if 0
      //Waiting shift register send data over 
      while(!LL_USART_IsActiveFlag_TC(USART2));

	  //Clear USART2 TC FLAG
	  LL_USART_ClearFlag_TC(USART2);

	  //Shut down DMA requre 
	  LL_USART_DisableDMAReq_TX(USART2);
	  //send data complish callback ref 
	  gpro_t.dma_tx_done = 1;
	  #else 
	  // 2. 开启 USART 的 TC（传输完成）中断，让硬件去等，我们不在这里死等
        LL_USART_ClearFlag_TC(USART2); // 先清一下标志位防止误触发
        LL_USART_EnableIT_TC(USART2);  // 开启串口发送完成中断

	  #endif 
	  
  }
  /* USER CODE END DMA1_Channel1_IRQn 0 */
  /* USER CODE BEGIN DMA1_Channel1_IRQn 1 */
   if(LL_DMA_IsActiveFlag_TE1(DMA1) !=RESET) LL_DMA_ClearFlag_TE1(DMA1);

  /* USER CODE END DMA1_Channel1_IRQn 1 */
}

/**
  * @brief This function handles DMA1 channel 2 and channel 3 interrupts.
  */
void DMA1_Channel2_3_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Channel2_3_IRQn 0 */

  /* USER CODE END DMA1_Channel2_3_IRQn 0 */
  /* USER CODE BEGIN DMA1_Channel2_3_IRQn 1 */

  /* USER CODE END DMA1_Channel2_3_IRQn 1 */
}

/**
  * @brief This function handles TIM16 global interrupt.
  */
void TIM16_IRQHandler(void)
{
  /* USER CODE BEGIN TIM16_IRQn 0 */

  /* USER CODE END TIM16_IRQn 0 */
  HAL_TIM_IRQHandler(&htim16);
  /* USER CODE BEGIN TIM16_IRQn 1 */

  /* USER CODE END TIM16_IRQn 1 */
}

/**
  * @brief This function handles TIM17 global interrupt.
  */
void TIM17_IRQHandler(void)
{
  /* USER CODE BEGIN TIM17_IRQn 0 */
    if(LL_TIM_IsActiveFlag_UPDATE(TIM17)){
  	
      LL_TIM_ClearFlag_UPDATE(TIM17); // ✅ 清除更新中断标志
      tim17_invoke_callback();//tim17_isr_callback_handler();


  }
  /* USER CODE END TIM17_IRQn 0 */
  /* USER CODE BEGIN TIM17_IRQn 1 */

  /* USER CODE END TIM17_IRQn 1 */
}

/**
  * @brief This function handles USART2 global interrupt / USART2 wake-up interrupt through EXTI line 26.
  */
void USART2_IRQHandler(void)
{
  /* USER CODE BEGIN USART2_IRQn 0 */
  volatile uint8_t data;
  if(LL_USART_IsActiveFlag_RXNE(USART2)){

     data = LL_USART_ReceiveData8(USART2);
	  
	  usart2_rx_data(data);

  }

  // 2. 新增的发送完成逻辑：
  // 必须同时判断“开启了TC中断”并且“硬件TC标志位置1”，防止未发送时误触发
  if(LL_USART_IsEnabledIT_TC(USART2) && LL_USART_IsActiveFlag_TC(USART2))
  {
      // 清除串口 TC 标志位
      LL_USART_ClearFlag_TC(USART2);
      
      // 关闭串口 TC 中断（因为是單次发送，发完就关掉，下次发送再开启）
      LL_USART_DisableIT_TC(USART2);
      
      // 关闭串口的 DMA 发送请求
      LL_USART_DisableDMAReq_TX(USART2);

      // 正式宣告：数据已全部物理发送到芯片外部引脚！
      gpro_t.dma_tx_done = 1;
      
      // (可选) 如果有发送完成的回调函数，可以在这里调用
      // usart2_tx_complete_callback(); 
  }

  /* USER CODE END USART2_IRQn 0 */
  /* USER CODE BEGIN USART2_IRQn 1 */
  
   LL_USART_ClearFlag_ORE(USART2);
   LL_USART_ClearFlag_NE(USART2);
   LL_USART_ClearFlag_FE(USART2);
  /* USER CODE END USART2_IRQn 1 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
