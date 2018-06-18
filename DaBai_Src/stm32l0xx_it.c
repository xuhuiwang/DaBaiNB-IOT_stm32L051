/**
  ******************************************************************************
  * @file    stm32l0xx_it.c
  * @author  MCD Application Team
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and
  *          peripherals interrupt service routine.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/

#include "stm32l0xx_it.h"
#include "stm32l0xx_hal.h"
#include "DaBai_APP.h"

/* External variables --------------------------------------------------------*/
extern volatile uint16_t g_TaskTime10ms;
extern volatile uint16_t g_TaskTime100ms;
extern volatile uint16_t g_TaskTime500ms;
extern volatile uint16_t g_TaskTime1000ms;
extern volatile uint32_t g_TaskTime1min;
extern volatile uint32_t g_TaskTime10min;

extern DMA_HandleTypeDef hdma_lpuart_rx;
extern DMA_HandleTypeDef hdma_lpuart_tx;
extern UART_HandleTypeDef hlpuart1;
extern DMA_HandleTypeDef hdma_adc;
/******************************************************************************/
/*                 STM32L0xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32l0xx.s).                                               */
/******************************************************************************/


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M0+ Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief   This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
  HAL_IncTick();
	g_TaskTime10ms++;
	g_TaskTime100ms++;
	g_TaskTime500ms++;
	g_TaskTime1000ms++;
	g_TaskTime1min++;
	g_TaskTime10min++;
	if(g_USB_insert == YES)//USB插入
	{
		if(g_chargeing_flag == YES)//电池在充电
		{
			if(g_BatVoltage == 100)
			{
				m_fullBatTimeCnt ++;
			}
			else
			{
				m_fullBatTimeCnt = 0;
			}
		}
	}
	else
	{
		m_fullBatTimeCnt = 0;
	}
}

/**
* @brief This function handles DMA1 channel 1 interrupt.
*/
void DMA1_Channel1_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Channel1_IRQn 0 */

  /* USER CODE END DMA1_Channel1_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_adc);
  /* USER CODE BEGIN DMA1_Channel1_IRQn 1 */

  /* USER CODE END DMA1_Channel1_IRQn 1 */
}

void DMA1_Channel2_3_IRQHandler(void)
{
	HAL_DMA_IRQHandler(&hdma_lpuart_rx);
	HAL_DMA_IRQHandler(&hdma_lpuart_tx);
}

/**
* @brief This function handles LPUART1 global interrupt.
*/
void LPUART1_IRQHandler(void)
{
  /* USER CODE BEGIN LPUART1_IRQn 0 */

  /* USER CODE END LPUART1_IRQn 0 */
  HAL_UART_IRQHandler(&hlpuart1);
  /* USER CODE BEGIN LPUART1_IRQn 1 */

  /* USER CODE END LPUART1_IRQn 1 */
}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
