/**
  ******************************************************************************
  * File Name          : DaBai_tim.c
  * Description        : This file provides code for the configuration
  *                      of the DaBai_tim instances.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "DaBai_tim.h"



/* Private typedef -----------------------------------------------------------*/
#define  PERIOD_VALUE       (uint32_t)(1000 - 1)  /* Period Value  */
#define  PULSE2_VALUE       (uint32_t)(PERIOD_VALUE/2) /* Capture Compare 2 Value  */


/* USER CODE BEGIN 0 */

/* USER CODE END 0 */


	/* Timer handler declaration */
	TIM_HandleTypeDef    TimHandle;
/* Counter Prescaler value */
uint32_t uhPrescalerValue = 0;

/* TIM init function */
void MX_TIM_Init(void)
{


	/* Timer Output Compare Configuration Structure declaration */
	TIM_OC_InitTypeDef sConfig;
  /* Initialize TIMx peripheral as follows:
       + Prescaler = (SystemCoreClock / 2000000) - 1
       + Period = (700 - 1)
       + ClockDivision = 0
       + Counter direction = Up
  */
	  /* Compute the prescaler value to have TIM2 counter clock equal to 2000000 Hz */
  uhPrescalerValue = (uint32_t)(SystemCoreClock / 2000000) - 1;
	
  TimHandle.Instance = TIM2;

  TimHandle.Init.Prescaler         = uhPrescalerValue;
  TimHandle.Init.Period            = PERIOD_VALUE;
  TimHandle.Init.ClockDivision     = 0;
  TimHandle.Init.CounterMode       = TIM_COUNTERMODE_UP;
  if (HAL_TIM_PWM_Init(&TimHandle) != HAL_OK)
  {
    /* Initialization Error */
    //_Error_Handler(__FILE__, __LINE__);
  }

  /*##-2- Configure the PWM channels #########################################*/
  /* Common configuration for all channels */
  sConfig.OCMode       = TIM_OCMODE_PWM1;
  sConfig.OCPolarity   = TIM_OCPOLARITY_HIGH;
  sConfig.OCFastMode   = TIM_OCFAST_DISABLE;
	
	  /* Set the pulse value for channel 2 */
  sConfig.Pulse = PULSE2_VALUE;
  if (HAL_TIM_PWM_ConfigChannel(&TimHandle, &sConfig, TIM_CHANNEL_2) != HAL_OK)
  {
    /* Configuration Error */
     //_Error_Handler(__FILE__, __LINE__);
  }
	
	  /* Start channel 2 */
  if (HAL_TIM_PWM_Start(&TimHandle, TIM_CHANNEL_2) != HAL_OK)
  {
    /* PWM Generation Error */
    //_Error_Handler(__FILE__, __LINE__);
  }
}

void SetBeepFreq(uint16_t freq_param)//freq < BEEP_MAX_FREQ Hz
{
	int32_t period = 0;

	freq_param = (freq_param < BEEP_MAX_FREQ ? freq_param : BEEP_MAX_FREQ);
	
	if(freq_param >= 400)
	{ 
		//TIM2->CR1 |= TIM_CR1_CEN; /* Enable the TIM Counter */
		period = 1000000 / freq_param;//1000000为TIM3的预分频 
		
	}
	else
	{
		TIM2->ARR = 1000;
		TIM2->CCR2 = 0; //输出低电平，蜂鸣器不通电
		//TIM2->CR1 &= (uint16_t)(~((uint16_t)TIM_CR1_CEN));//TIM1 Disable 
	}
	if(TIM2->CNT >= TIM2->ARR)
	{
		TIM2->CNT = 0;
	}
	TIM2->ARR = period;
	TIM2->CCR2 = period>>1; 
}



/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
