/**
  ******************************************************************************
  * File Name          : DABAI_USART.c
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

/* Includes ------------------------------------------------------------------*/
#include "DaBai_usart.h"
#include "NB_BC95_28.h"


/* USER CODE BEGIN 0 */
#define UART_BUF_LEN     128
static uint8_t gBuf[UART_BUF_LEN];
volatile uint8_t  msgLenIndex = 0;

static UART_UserCb  pHalUartCb = NULL;
/* USER CODE END 0 */

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;

UART_HandleTypeDef hlpuart1;


/* USART1 init function */

void MX_USART1_UART_Init(void)
{

  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
   // _Error_Handler(__FILE__, __LINE__);
  }

}
/* USART2 init function */

void MX_USART2_UART_Init(void)
{

  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    //_Error_Handler(__FILE__, __LINE__);
  }

}

/* LPUART1 init function */

void MX_LPUART1_UART_Init(void)
{

  hlpuart1.Instance = LPUART1;
  hlpuart1.Init.BaudRate = 9600;
  hlpuart1.Init.WordLength = UART_WORDLENGTH_8B;
  hlpuart1.Init.StopBits = UART_STOPBITS_1;
  hlpuart1.Init.Parity = UART_PARITY_NONE;
  hlpuart1.Init.Mode = UART_MODE_TX_RX;
  hlpuart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  hlpuart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  hlpuart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&hlpuart1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}


/* USER CODE BEGIN 1 */
//***************************************************************
// fn : HAL_UART_RegisterCb
//
// brief : 设置串口回调函数指针
//
// param : cb -> 回调函数指针
//
// return : none
void HAL_UART_RegisterCb(UART_UserCb cb)
{
  pHalUartCb = cb;
}
//***********************************************
// fn : HAL_UART_Log
//
// brief : 用串口发送指令长度数据
//
// param : pData -> 发送数据指针
//         size  -> 数据长度
//
// return : none
void HAL_UART_Log(uint8_t *pData,uint16_t size)
{
  if(size == 0)
  {
    return;
  }
  if(pData == 0)
  {
    return;
  }
  HAL_UART_Transmit(&huart1,pData,size,HAL_MAX_DELAY);
}
//***********************************************
// fn : HAL_UART_Write
//
// brief : 用串口发送指令长度数据
//
// param : pData -> 发送数据指针
//         size  -> 数据长度
//
// return : none
void HAL_UART2_Write(uint8_t *pData,uint16_t size)
{
  if(size == 0)
  {
    return;
  }
  if(pData == 0)
  {
    return;
  }
  HAL_UART_Transmit(&huart2,pData,size,HAL_MAX_DELAY);
}

//***********************************************
// fn : HAL_UART_Write
//
// brief : 用串口发送指令长度数据
//
// param : pData -> 发送数据指针
//         size  -> 数据长度
//
// return : none
void HAL_LPUART1_Write(uint8_t *pData,uint16_t size)
{
  if(size == 0)
  {
    return;
  }
  if(pData == 0)
  {
    return;
  }
  HAL_UART_Transmit(&hlpuart1,pData,size,HAL_MAX_DELAY);
}


//***********************************************
// fn : HAL_UART_Read
//
// brief : 用串口发送指令长度数据
//
// param : pData -> 发送数据指针
//         size  -> 数据长度
//
// return : 已经接收到的数据长度
uint16_t HAL_UART_Read(uint8_t *pData,uint16_t size)
{
  uint16_t len = 0;
  
  HAL_UART_Receive(&huart2,pData,size,1);
  
  len = size - huart1.RxXferCount;
  return len;
}
//***********************************************
// fn : HAL_UART_Poll
//
// brief : 轮询串口接收数据，直到帧结束
//
// param : none
//
// return : none
uint8_t HAL_UART_Poll(void)
{
  uint8_t isEnd = 0;
  if(HAL_UART_Receive(&huart2, gBuf+msgLenIndex,1,2))
  {
    if(msgLenIndex)
    {
      HAL_UART2_Write(gBuf,msgLenIndex);
      
      //执行回调
      if(pHalUartCb)
      {
        gBuf[msgLenIndex] = 0;
        pHalUartCb(gBuf,msgLenIndex);
      }
      msgLenIndex = 0;
      isEnd = !0;
    }

  }
  else
  {
    msgLenIndex++;
    if(msgLenIndex >= (UART_BUF_LEN - 1) )
    {
      HAL_UART2_Write(gBuf,msgLenIndex);
      //执行回调
      if(pHalUartCb)
      {
        gBuf[msgLenIndex] = 0;
        pHalUartCb(gBuf,msgLenIndex);
      }
      msgLenIndex = 0;
      isEnd = !0;
    }
  }
  return isEnd;
}

#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the EVAL_COM1 and Loop until the end of transmission */
  HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xFFFF);
 
  return ch;
}


/* USER CODE BEGIN 1 */
void HAL_UARTDMA_Init(bc95_receive_cb cb,uint32_t baud)
{
  baud = baud;
  //nb_receCb = cb;
  //HAL_UART_Receive_DMA(&hlpuart1,UartDma_Init(uart_dma_send,LPUART1),RECE_BUF_MAX_LEN);
}


/* USER CODE END 1 */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
