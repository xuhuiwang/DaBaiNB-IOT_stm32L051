/**
  ******************************************************************************
  * File Name          : DABAI_GPIO.h
  * Description        : This file provides code for the configuration
  *                      of the DABAI_GPIO instances.
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2018 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DABAI_GPIO_H
#define __DABAI_GPIO_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l0xx_hal.h"



#define KEY1_Pin  GPIO_PIN_0

#define KEY2_Pin  GPIO_PIN_12
#define KEY3_Pin  GPIO_PIN_15

#define LED1_PIN			GPIO_PIN_3
#define LED2_PIN			GPIO_PIN_4
#define LED3_PIN			GPIO_PIN_5
#define LED4_PIN			GPIO_PIN_6
#define LED5_PIN			GPIO_PIN_7

#define SYS_PW_CTRL_PIN  GPIO_PIN_15

#define POWER_ON	  HAL_GPIO_WritePin(GPIOA, SYS_PW_CTRL_PIN, GPIO_PIN_SET)
#define POWER_OFF   HAL_GPIO_WritePin(GPIOA, SYS_PW_CTRL_PIN, GPIO_PIN_RESET)
#define BEPP_ON			HAL_TIM_PWM_Start(&TimHandle, TIM_CHANNEL_2)
#define BEPP_OFF		HAL_TIM_PWM_Stop(&TimHandle, TIM_CHANNEL_2)

#define LED1_ON			HAL_GPIO_WritePin(GPIOB,LED1_PIN, GPIO_PIN_RESET)
#define LED1_OFF		HAL_GPIO_WritePin(GPIOB,LED1_PIN, GPIO_PIN_SET)
#define LED2_ON			HAL_GPIO_WritePin(GPIOB,LED2_PIN, GPIO_PIN_RESET)
#define LED2_OFF		HAL_GPIO_WritePin(GPIOB,LED2_PIN, GPIO_PIN_SET)
#define LED3_ON			HAL_GPIO_WritePin(GPIOB,LED3_PIN, GPIO_PIN_RESET)
#define LED3_OFF		HAL_GPIO_WritePin(GPIOB,LED3_PIN, GPIO_PIN_SET)
#define LED4_ON			HAL_GPIO_WritePin(GPIOB,LED4_PIN, GPIO_PIN_RESET)
#define LED4_OFF		HAL_GPIO_WritePin(GPIOB,LED4_PIN, GPIO_PIN_SET)
#define LED5_ON			HAL_GPIO_WritePin(GPIOB,LED5_PIN, GPIO_PIN_RESET)
#define LED5_OFF		HAL_GPIO_WritePin(GPIOB,LED5_PIN, GPIO_PIN_SET)

extern void _Error_Handler(char *, int);

void MX_GPIO_Init(void);
void PowerOffGpioConfig(void);
#ifdef __cplusplus
}
#endif
#endif /*__ DABAI_GPIO_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
