/**
  ******************************************************************************
  * File Name          : DABAI_ADC.c
  * Description        : This file provides code for the configuration
  *                      of the DABAI_ADC instances.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "DaBai_ADC.h"



/** @addtogroup ADC_RegularConversion_Polling
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* ADC handler declaration */
ADC_HandleTypeDef    AdcHandle;
/* Variable used to get converted value */
__IO uint32_t uwADCxConvertedValue = 0;


/* ADC init function */
 /* ### - 1 - Initialize ADC peripheral #################################### */
  /*
   *  Instance                  = ADC1.
   *  OversamplingMode          = Disabled
   *  ClockPrescaler            = PCLK clock with no division.
   *  LowPowerAutoPowerOff      = Disabled (For this exemple continuous mode is enabled with software start)
   *  LowPowerFrequencyMode     = Enabled (To be enabled only if ADC clock is lower than 2.8MHz) 
   *  LowPowerAutoWait          = Disabled (New conversion starts only when the previous conversion is completed)       
   *  Resolution                = 12 bit (increased to 16 bit with oversampler)
   *  SamplingTime              = 7.5 cycles od ADC clock.
   *  ScanConvMode              = Forward
   *  DataAlign                 = Right
   *  ContinuousConvMode        = Enabled
   *  DiscontinuousConvMode     = Disabled
   *  ExternalTrigConvEdge      = None (Software start)
   *  EOCSelection              = End Of Conversion event
   *  DMAContinuousRequests     = DISABLE
   */
void MX_ADC_Init(void)
{

  ADC_ChannelConfTypeDef sConfig;
  
  AdcHandle.Instance = ADC1;

  AdcHandle.Init.OversamplingMode      = DISABLE;

  AdcHandle.Init.ClockPrescaler        = ADC_CLOCK_SYNC_PCLK_DIV1;
  AdcHandle.Init.LowPowerAutoPowerOff  = DISABLE;
  AdcHandle.Init.LowPowerFrequencyMode = ENABLE;
  AdcHandle.Init.LowPowerAutoWait      = DISABLE;
    
  AdcHandle.Init.Resolution            = ADC_RESOLUTION_12B;
  AdcHandle.Init.SamplingTime          = ADC_SAMPLETIME_7CYCLES_5;
  AdcHandle.Init.ScanConvMode          = ADC_SCAN_DIRECTION_FORWARD;
  AdcHandle.Init.DataAlign             = ADC_DATAALIGN_RIGHT;
  AdcHandle.Init.ContinuousConvMode    = ENABLE;
  AdcHandle.Init.DiscontinuousConvMode = DISABLE;
  AdcHandle.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_NONE;
  AdcHandle.Init.EOCSelection          = ADC_EOC_SINGLE_CONV;
  AdcHandle.Init.DMAContinuousRequests = DISABLE;

  /* Initialize ADC peripheral according to the passed parameters */
  if (HAL_ADC_Init(&AdcHandle) != HAL_OK)
  {
    //Error_Handler();
  }


  /* ### - 2 - Start calibration ############################################ */
  if (HAL_ADCEx_Calibration_Start(&AdcHandle, ADC_SINGLE_ENDED) != HAL_OK)
  {
    
  }

  /* ### - 3 - Channel configuration ######################################## */
	  /* Select Channel 8 to be converted */
//  sConfig.Channel = ADC_CHANNEL_8; 
//  sConfig.Rank = ADC_RANK_CHANNEL_NUMBER;	
//  if (HAL_ADC_ConfigChannel(&AdcHandle, &sConfig) != HAL_OK)
//  {
//    
//  }
  /* Select Channel 9 to be converted */
  sConfig.Channel = ADC_CHANNEL_9;    
  if (HAL_ADC_ConfigChannel(&AdcHandle, &sConfig) != HAL_OK)
  {
    
  }

 /*##- 4- Start the conversion process #######################################*/  
  if (HAL_ADC_Start(&AdcHandle) != HAL_OK)
  {
    /* Start Conversation Error */
    
  }

}

uint32_t getLightValue(void)
{
	HAL_ADC_PollForConversion(&AdcHandle, 10);

	/* Check if the continuous conversion of regular channel is finished */
	if ((HAL_ADC_GetState(&AdcHandle) & HAL_ADC_STATE_REG_EOC) == HAL_ADC_STATE_REG_EOC)
	{
		/*##-6- Get the converted value of regular channel  ########################*/
		uwADCxConvertedValue = HAL_ADC_GetValue(&AdcHandle);
	}
	return uwADCxConvertedValue;
}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
