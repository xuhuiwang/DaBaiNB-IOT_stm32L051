/**
  ******************************************************************************
  * File Name          : DABAI_ADC.h
  * Description        : This file provides code for the configuration
  *                      of the DABAI_ADC instances.
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DABAI_ADC_H
#define __DABAI_ADC_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l0xx_hal.h"


extern void _Error_Handler(char *, int);
extern ADC_HandleTypeDef    AdcHandle;

void MX_ADC_Init(void);
uint32_t getLightValue(void);
#ifdef __cplusplus
}
#endif
#endif /*__ DABAI_ADC_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
