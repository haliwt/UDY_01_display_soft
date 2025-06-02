/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32g0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
//TM1639 GPIO
#define TM1639_STB_Pin 				GPIO_PIN_7
#define TM1639_STB_GPIO_Port 			GPIOB

#define TM1639_DIO_Pin 					GPIO_PIN_9
#define TM1639_DIO_GPIO_Port 				GPIOB

#define TM1639_SCLK_Pin 					GPIO_PIN_15
#define TM1639_SCLK_GPIO_Port 			GPIOC

//LED GPIO
#define LED_POWER_Pin 					GPIO_PIN_3
#define LED_POWER_GPIO_Port 			GPIOB

#define LED_DRY_Pin 						GPIO_PIN_1
#define LED_DRY_GPIO_Port 				GPIOA

#define LED_MOUSE_Pin 					GPIO_PIN_11
#define LED_MOUSE_GPIO_Port 			GPIOA

#define LED_PLASMA_Pin 				GPIO_PIN_12
#define LED_PLASMA_GPIO_Port 			GPIOA

#define LED_CTL_Pin GPIO_PIN_0
#define LED_CTL_GPIO_Port GPIOA

//KEY GPIO

#define KEY_ADD_Pin GPIO_PIN_4
#define KEY_ADD_GPIO_Port GPIOA
#define KEY_ADD_EXTI_IRQn EXTI4_15_IRQn

#define KEY_DEC_Pin GPIO_PIN_5
#define KEY_DEC_GPIO_Port GPIOA
#define KEY_DEC_EXTI_IRQn EXTI4_15_IRQn

#define KEY_POWER_Pin GPIO_PIN_6
#define KEY_POWER_GPIO_Port GPIOA
#define KEY_POWER_EXTI_IRQn EXTI4_15_IRQn

#define KEY_DRY_Pin GPIO_PIN_7
#define KEY_DRY_GPIO_Port GPIOA
#define KEY_DRY_EXTI_IRQn EXTI4_15_IRQn

#define KEY_PLASMA_Pin GPIO_PIN_0
#define KEY_PLASMA_GPIO_Port GPIOB
#define KEY_PLASMA_EXTI_IRQn EXTI0_1_IRQn

#define KEY_MOUSE_Pin GPIO_PIN_13
#define KEY_MOUSE_GPIO_Port GPIOA



/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
