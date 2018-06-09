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

uint16_t   gADCxConvertedData[ADC_DATA_BUFFER_SIZE];

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
  //AdcHandle.Init.DiscontinuousConvMode = DISABLE;
  AdcHandle.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_NONE;
	//AdcHandle.Init.ExternalTrigConv      = ADC_SOFTWARE_START;//add 20180609
  AdcHandle.Init.EOCSelection          = ADC_EOC_SINGLE_CONV;
	//AdcHandle.Init.Overrun 							 = ADC_OVR_DATA_PRESERVED;//add 20180609
  AdcHandle.Init.DMAContinuousRequests = ENABLE;

  /* Initialize ADC peripheral according to the passed parameters */
  if (HAL_ADC_Init(&AdcHandle) != HAL_OK)
  {
    //Error_Handler();
  }


  /* ### - 2 - Start calibration ############################################ */
  if (HAL_ADCEx_Calibration_Start(&AdcHandle, ADC_SINGLE_ENDED) != HAL_OK)
  {
    
  }

  /* ### - Channel configuration ######################################## */
	  /* Select Channel 8 to be converted */
  sConfig.Channel = ADC_CHANNEL_8; 
  sConfig.Rank = ADC_RANK_CHANNEL_NUMBER;	
  if (HAL_ADC_ConfigChannel(&AdcHandle, &sConfig) != HAL_OK)
  {
    
  }
  /* Select Channel 9 to be converted */
  sConfig.Channel = ADC_CHANNEL_9;    
  if (HAL_ADC_ConfigChannel(&AdcHandle, &sConfig) != HAL_OK)
  {
    
  }

// /*##- 4- Start the conversion process #######################################*/  
//  if (HAL_ADC_Start(&AdcHandle) != HAL_OK)
//  {
//    /* Start Conversation Error */
//    
//  }
	
	  /* Start conversion in DMA mode ################################# */
	HAL_ADC_Start_DMA(&AdcHandle,(uint32_t *)gADCxConvertedData,ADC_DATA_BUFFER_SIZE);
}

uint16_t getLightValue(void)
{
	uint8_t i = 0;
	uint32_t sum = 0;
	uint16_t averValue = 0;
	for(i = 1; i < ADC_DATA_BUFFER_SIZE; i+=(ADC_DATA_BUFFER_SIZE/10))
	{
		sum += gADCxConvertedData[i]; 
	}
	averValue = (uint16_t)((sum / 8));
	return averValue;
}
//电池电量低于10%需要充电
uint8_t GetBatVoltage(void)
{
	uint8_t i = 0;
	uint32_t sum = 0;
	uint8_t preVBat = 0;//显示为电池电量的百分比
	
	for(i = 0; i < ADC_DATA_BUFFER_SIZE; i+=(ADC_DATA_BUFFER_SIZE/10))
	{
		sum += gADCxConvertedData[i]; 
	}
	preVBat = (((sum / 10) - FULL_3V3_ADC_VALUE)*100) / (FULL_4V2_ADC_VALUE - FULL_3V3_ADC_VALUE) ;

	if(preVBat > 100)
		preVBat =100;
	
	return preVBat; 
}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
