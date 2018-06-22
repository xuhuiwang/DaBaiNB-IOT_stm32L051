/**
  ******************************************************************************
  * File Name          : DaBai_rtc.c
  * Description        : This file provides code for the configuration
  *                      of the DaBai_rtc instances.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "DaBai_rtc.h"
#include "main.h"
#include "DaBai_GPIO.h"
#include "DaBai_usart.h"
#include "DaBai_APP.h"

#define ALARM_10MIN  1
//#define ALARM_20S  1
volatile uint8_t g_RTCAlarmFlag = 0;

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

RTC_HandleTypeDef hrtc;


/* RTC init function */
void MX_RTC_Init(void)
{

    /**Initialize RTC Only 
    */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = RTC_ASYNCH_PREDIV;
  hrtc.Init.SynchPrediv = RTC_SYNCH_PREDIV;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutRemap = RTC_OUTPUT_REMAP_NONE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }
  /* Disable all used wakeup sources*/
  HAL_RTCEx_DeactivateWakeUpTimer(&hrtc);
}


/**
  * @brief  Configure the current time and date.
  * @param  None
  * @retval None
  */
void RTC_AlarmConfig(void)
{
  RTC_DateTypeDef  sdatestructure;
  RTC_TimeTypeDef  stimestructure;
  RTC_AlarmTypeDef salarmstructure;
 
  /*##-1- Configure the Date #################################################*/
  /* Set Date: MONDAY JUNE 18th 2018 */
  sdatestructure.Year = 0x18;
  sdatestructure.Month = RTC_MONTH_JUNE;
  sdatestructure.Date = 0x18;
  sdatestructure.WeekDay = RTC_WEEKDAY_MONDAY;
  
  if(HAL_RTC_SetDate(&hrtc,&sdatestructure,RTC_FORMAT_BCD) != HAL_OK)
  {
		_Error_Handler(__FILE__, __LINE__);
  } 
  
  /*##-2- Configure the Time #################################################*/
  /* Set Time: 18:30:00 */
  stimestructure.Hours = 0x23;
  stimestructure.Minutes = 0x30;
  stimestructure.Seconds = 0x00;
  stimestructure.TimeFormat = RTC_HOURFORMAT12_PM;
  stimestructure.DayLightSaving = RTC_DAYLIGHTSAVING_NONE ;
  stimestructure.StoreOperation = RTC_STOREOPERATION_RESET;
  
  if(HAL_RTC_SetTime(&hrtc,&stimestructure,RTC_FORMAT_BCD) != HAL_OK)
  {
		_Error_Handler(__FILE__, __LINE__);
  }  

  /*##-3- Configure the RTC Alarm peripheral #################################*/
  /* Set Alarm to 02:20:30 
     RTC Alarm Generation: Alarm on Hours, Minutes and Seconds */
  salarmstructure.Alarm = RTC_ALARM_A;
  salarmstructure.AlarmDateWeekDay = RTC_WEEKDAY_MONDAY;
  salarmstructure.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_WEEKDAY;
  salarmstructure.AlarmMask = RTC_ALARMMASK_DATEWEEKDAY;
  salarmstructure.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_NONE;
  salarmstructure.AlarmTime.TimeFormat = RTC_HOURFORMAT12_AM;
  salarmstructure.AlarmTime.Hours = 0x23;
  salarmstructure.AlarmTime.Minutes = 0x40;
  salarmstructure.AlarmTime.Seconds = 0x00;
  salarmstructure.AlarmTime.SubSeconds = 0x56;
  
  if(HAL_RTC_SetAlarm_IT(&hrtc,&salarmstructure,RTC_FORMAT_BCD) != HAL_OK)
  {
		_Error_Handler(__FILE__, __LINE__);  
	}
}

/**
  * @brief  Display the current time.
  * @param  showtime : pointer to buffer
  * @retval None
  */
void RTC_TimeShow(uint8_t* showtime)
{
  RTC_DateTypeDef sdatestructureget;
  RTC_TimeTypeDef stimestructureget;
  
  /* Get the RTC current Time */
  HAL_RTC_GetTime(&hrtc, &stimestructureget, RTC_FORMAT_BIN);
  /* Get the RTC current Date */
  HAL_RTC_GetDate(&hrtc, &sdatestructureget, RTC_FORMAT_BIN);
  /* Display time Format : hh:mm:ss */
  sprintf((char*)showtime,"year=20%02d,Month=%02d,Data=%02d,%02d:%02d:%02d",
					sdatestructureget.Year,sdatestructureget.Month,sdatestructureget.Date,
					stimestructureget.Hours, stimestructureget.Minutes, stimestructureget.Seconds);
	printf("\r\nshowtime = %s\r\n",showtime);
} 

/**
  * @brief  Alarm callback
  * @param  hrtc : RTC handle
  * @retval None
  */
void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc)
{
	uint8_t m_ShowTime[50] = {0};
	RTC_DateTypeDef sdatestructureget;
  RTC_TimeTypeDef stimestructureget;
	RTC_AlarmTypeDef salarmstructure;
	
		/**sysWakeUpConfig**/
	sysWakeUpConfig();
	
	  /* Get the RTC current Time */
  HAL_RTC_GetTime(hrtc, &stimestructureget, RTC_FORMAT_BIN);
  /* Get the RTC current Date */
  HAL_RTC_GetDate(hrtc, &sdatestructureget, RTC_FORMAT_BIN);
	sprintf((char*)m_ShowTime,"year=20%02d,Month=%02d,Data=%02d,%02d:%02d:%02d",
					sdatestructureget.Year,sdatestructureget.Month,sdatestructureget.Date,
					stimestructureget.Hours, stimestructureget.Minutes, stimestructureget.Seconds);

	printf("\r\nRTC AlarmA time = %s\r\n",m_ShowTime);
	
  /*RTC Alarm Generation: Alarm on Hours, Minutes and Seconds */
  salarmstructure.Alarm = RTC_ALARM_A;
  salarmstructure.AlarmDateWeekDay = RTC_WEEKDAY_TUESDAY;
  salarmstructure.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_WEEKDAY;
  salarmstructure.AlarmMask = RTC_ALARMMASK_DATEWEEKDAY;
  salarmstructure.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_NONE;
  salarmstructure.AlarmTime.TimeFormat = RTC_HOURFORMAT12_AM;
  salarmstructure.AlarmTime.Hours = stimestructureget.Hours;
  salarmstructure.AlarmTime.Minutes = stimestructureget.Minutes;
	
	#ifdef ALARM_20S
//20S发送一次数据	
	salarmstructure.AlarmTime.Seconds = stimestructureget.Seconds + 20;
	if(salarmstructure.AlarmTime.Seconds >= 60)
	{
	  salarmstructure.AlarmTime.Seconds = 0;
		salarmstructure.AlarmTime.Minutes = stimestructureget.Minutes + 1;
		if(salarmstructure.AlarmTime.Minutes >= 60)
		{
			salarmstructure.AlarmTime.Minutes = 0;
			salarmstructure.AlarmTime.Hours = stimestructureget.Hours + 1;
			if((salarmstructure.AlarmTime.Hours = stimestructureget.Hours + 1) >= 24)
			{
				salarmstructure.AlarmTime.Hours = 0;
				
			}
		}
	}
#endif

#ifdef ALARM_10MIN

////10分钟发送一次数据
	salarmstructure.AlarmTime.Minutes = stimestructureget.Minutes + 10;
	if(salarmstructure.AlarmTime.Minutes >= 60)
	{
		salarmstructure.AlarmTime.Minutes = 0;
		salarmstructure.AlarmTime.Hours = stimestructureget.Hours + 1;
		if((salarmstructure.AlarmTime.Hours = stimestructureget.Hours + 1) >= 24)
		{
			salarmstructure.AlarmTime.Hours = 0;
			
		}
	}
#endif	

  salarmstructure.AlarmTime.SubSeconds = 0x56;
  
  if(HAL_RTC_SetAlarm_IT(hrtc,&salarmstructure,RTC_FORMAT_BIN) != HAL_OK)
  {
		_Error_Handler(__FILE__, __LINE__);  
	}
	//HAL_GPIO_TogglePin(GPIOB,LED3_PIN);
	g_RTCAlarmFlag = 1;
}


/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
