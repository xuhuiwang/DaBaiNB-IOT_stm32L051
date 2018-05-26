/**
  ******************************************************************************
  * File Name          : DABAI_USART.h
  * Description        : This file provides code for the configuration
  *                      of the DABAI_USART instances.
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
#ifndef __DABAI_USART_H
#define __DABAI_USART_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l0xx_hal.h"


/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;

/* USER CODE BEGIN Private defines */

//���崮���û��ص�����
typedef void (*UART_UserCb)(uint8_t *pBuf,uint16_t size);
/* USER CODE END Private defines */

extern void _Error_Handler(char *, int);

void MX_USART1_UART_Init(void);
void MX_USART2_UART_Init(void);
void MX_LPUART1_UART_Init(void);
/* USER CODE BEGIN Prototypes */
//***********************************************
// fn : HAL_UART_Log
//
// brief : �ô��ڷ���ָ�������
//
// param : pData -> ��������ָ��
//         size  -> ���ݳ���
//
// return : none
void HAL_UART_Log(uint8_t *pData,uint16_t size);
//***************************************************************
// fn : HAL_UART_RegisterCb
//
// brief : ���ô��ڻص�����ָ��
//
// param : cb -> �ص�����ָ��
//
// return : none
void HAL_UART_RegisterCb(UART_UserCb cb);

//***************************************************************
// fn : HAL_UART_Write
//
// brief : �ô��ڷ���ָ�������
//
// param : pData -> ��������ָ��
//         size  -> ���ݳ���
//
// return : none
void HAL_UART2_Write(uint8_t *pData,uint16_t size);


//***************************************************************
// fn : HAL_LPUART1_Write
//
// brief : �ô��ڷ���ָ�������
//
// param : pData -> ��������ָ��
//         size  -> ���ݳ���
//
// return : none
void HAL_LPUART1_Write(uint8_t *pData,uint16_t size);

//***************************************************************
// fn : HAL_UART_Read
//
// brief : �ô��ڷ���ָ�������
//
// param : pData -> ��������ָ��
//         size  -> ���ݳ���
//
// return : �Ѿ����յ������ݳ���
uint16_t HAL_UART_Read(uint8_t *pData,uint16_t size);

//***********************************************
// fn : HAL_UART_Poll
//
// brief : ��ѯ���ڽ������ݣ�ֱ��֡����
//
// param : none
//
// return : !=0 this frame is end,others 
uint8_t HAL_UART_Poll(void);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__DABAI_USART_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/