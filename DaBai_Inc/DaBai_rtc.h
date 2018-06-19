/**
  ******************************************************************************
  * File Name          : DABAI_RCT.h
  * Description        : This file provides code for the configuration
  *                      of the DABAI_TIM instances.
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DABAI_RTC_H
#define __DABAI_RTC_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l0xx_hal.h"

extern volatile uint8_t g_RTCAlarmFlag;

#define RTC_ASYNCH_PREDIV  0x7F
#define RTC_SYNCH_PREDIV   0x00FF

void MX_RTC_Init(void);
void RTC_AlarmConfig(void);
void RTC_TimeShow(uint8_t* showtime);

#ifdef __cplusplus
}
#endif
#endif /*__ DABAI_RTC_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT DaBai_IOT *****END OF FILE****/
