#include "bsp.h"


void mouse_gpio_init(void)
{
   #if 0
   GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */

  __HAL_RCC_GPIOA_CLK_ENABLE();
  
  /*Configure GPIO pin : KEY_MOUSE_Pin */
  GPIO_InitStruct.Pin = KEY_MOUSE_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;//GPIO_NOPULL;
  HAL_GPIO_Init(KEY_MOUSE_GPIO_Port, &GPIO_InitStruct);
  
  #endif 

}



