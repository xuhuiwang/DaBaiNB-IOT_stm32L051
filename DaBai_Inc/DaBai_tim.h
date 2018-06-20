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

<<<<<<< HEAD
=======
#define BEEP_MAX_FREQ			2730
>>>>>>> dev

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */


/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

	/* Timer handler declaration */
extern TIM_HandleTypeDef    TimHandle;

extern void _Error_Handler(char *, int);

void MX_TIM_Init(void);
void SetBeepFreq(uint16_t freq_param);


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
