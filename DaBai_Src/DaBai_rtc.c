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

extern volatile uint32_t g_TaskTime1min;

//注意说明：只支持单独设置小时、分钟、秒钟的步长，不支持时分秒三者搭配设置。否则会引起闹钟间隔误差
#define  HOUR_STEP  			0
#define  MINUTE_STEP			10
#define	 SECOND_STEP			0


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
/******************************************************************************
// fn : SetRTC_AlarmWakeUpTime
//
// brief : 设置闹钟唤醒时间间隔
//
// param : hrtc -> RTCType
//         HourStep -> 间隔小时步长
//         MinuteStep  -> 间隔分钟步长
// 				 SecondStep ->  间隔秒钟步长

//注意说明：只支持单独设置小时、分钟、秒钟的步长，不支持时分秒三者搭配设置。否则会引起闹钟间隔误差
//例如：
				可以设置：HourStep = 1，MinuteStep = 0，SecondStep = 0，或者：HourStep = 0，MinuteStep = 10，SecondStep = 0
//		不可设置为：HourStep = 1，MinuteStep = 30，SecondStep = 0

// return : NONE
********************************************************************/
void SetRTC_AlarmWakeUpTime(RTC_HandleTypeDef *hrtc,uint8_t HourStep,uint8_t MinuteStep,uint8_t SecondStep)
{
	uint8_t m_ShowTime[50] = {0};
	RTC_DateTypeDef sdatestructureget;
  RTC_TimeTypeDef stimestructureget;
	RTC_AlarmTypeDef salarmstructure;
	
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

	salarmstructure.AlarmTime.Hours = stimestructureget.Hours + HourStep;
	if(salarmstructure.AlarmTime.Hours >= 24)
	{
		salarmstructure.AlarmTime.Hours = salarmstructure.AlarmTime.Hours - 24;	
	}

	salarmstructure.AlarmTime.Minutes = stimestructureget.Minutes + MinuteStep;
	if(salarmstructure.AlarmTime.Minutes >= 60)//大于60分
	{
		salarmstructure.AlarmTime.Minutes = salarmstructure.AlarmTime.Minutes - 60;
		salarmstructure.AlarmTime.Hours = stimestructureget.Hours + 1;
		if(salarmstructure.AlarmTime.Hours >= 24)
		{
			salarmstructure.AlarmTime.Hours = 0;			
		}
	}
	
	salarmstructure.AlarmTime.Seconds = stimestructureget.Seconds + SecondStep;
	if(salarmstructure.AlarmTime.Seconds >= 60)
	{
	  salarmstructure.AlarmTime.Seconds = salarmstructure.AlarmTime.Seconds - 60;
		salarmstructure.AlarmTime.Minutes = stimestructureget.Minutes + 1;
		if(salarmstructure.AlarmTime.Minutes >= 60)
		{
			salarmstructure.AlarmTime.Minutes = 0;
			salarmstructure.AlarmTime.Hours = stimestructureget.Hours + 1;
			if(salarmstructure.AlarmTime.Hours >= 24)
			{
				salarmstructure.AlarmTime.Hours = 0;
				
			}
		}
	}
	
  salarmstructure.AlarmTime.SubSeconds = 0x56;
  
	sprintf((char*)m_ShowTime,"year=20%02d,Month=%02d,Data=%02d,%02d:%02d:%02d",
					sdatestructureget.Year,sdatestructureget.Month,sdatestructureget.Date,
					salarmstructure.AlarmTime.Hours, salarmstructure.AlarmTime.Minutes, salarmstructure.AlarmTime.Seconds);

	printf("\r\nRTC Next AlarmA time = %s\r\n",m_ShowTime);
	
	
  if(HAL_RTC_SetAlarm_IT(hrtc,&salarmstructure,RTC_FORMAT_BIN) != HAL_OK)
  {
		_Error_Handler(__FILE__, __LINE__);  
	}
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
  stimestructure.Minutes = 0x59;
  stimestructure.Seconds = 0x00;
  stimestructure.TimeFormat = RTC_HOURFORMAT12_PM;
  stimestructure.DayLightSaving = RTC_DAYLIGHTSAVING_NONE ;
  stimestructure.StoreOperation = RTC_STOREOPERATION_RESET;
  
  if(HAL_RTC_SetTime(&hrtc,&stimestructure,RTC_FORMAT_BCD) != HAL_OK)
  {
		_Error_Handler(__FILE__, __LINE__);
  }  

  SetRTC_AlarmWakeUpTime(&hrtc,HOUR_STEP,MINUTE_STEP,SECOND_STEP);
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
	RTC_DateTypeDef sdatestructureget;
  RTC_TimeTypeDef stimestructureget;
	RTC_AlarmTypeDef salarmstructure;
	
		/**sysWakeUpConfig**/
	sysWakeUpConfig();
	SetRTC_AlarmWakeUpTime(hrtc,HOUR_STEP,MINUTE_STEP,SECOND_STEP);

	//HAL_GPIO_TogglePin(GPIOB,LED3_PIN);
	g_RTCAlarmFlag = 1;
	g_TaskTime1min = 0;
}



/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
