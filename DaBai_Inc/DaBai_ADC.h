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


#define  ADC_DATA_BUFFER_SIZE  20

/*
电池电压为4.2V时 ADC采样值为 3799
电池电压为3.3V时 ADC采样值为 2988
当电压低于3.3V时，ADC的值始终为2988左右
*/
#define FULL_VOLTAGE  		4.2
#define EMPTY_VOLTAGE		3.0
#define FULL_4V2_ADC_VALUE  3720 //实测3720为4.1V，防止有的电池电压充不高，APP显示电量一直充不满
 //减去30是为了让 (采集到的电池电压 - FULL_3V3_ADC_VALUE) >0
 //当电压低于3.3V时，ADC的值始终为2988左右
#define FULL_3V3_ADC_VALUE  (2988-30)

extern void _Error_Handler(char *, int);
extern ADC_HandleTypeDef    AdcHandle;

void MX_ADC_Init(void);
uint16_t getLightValue(void);
uint8_t GetBatVoltage(void);
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
