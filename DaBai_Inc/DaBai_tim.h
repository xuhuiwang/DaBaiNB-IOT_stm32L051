/**
  ******************************************************************************
  * File Name          : DABAI_TIM.h
  * Description        : This file provides code for the configuration
  *                      of the DABAI_TIM instances.
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DABAI_TIM_H
#define __DABAI_TIM_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l0xx_hal.h"


/* USER CODE BEGIN Includes */

/* USER CODE END Includes */


/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

	/* Timer handler declaration */
extern TIM_HandleTypeDef    TimHandle;
void MX_TIM_Init(void);



#ifdef __cplusplus
}
#endif
#endif /*__ DABAI_TIM_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT DaBai_IOT *****END OF FILE****/
