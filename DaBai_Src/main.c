/**

  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "DaBai_i2c.h"
#include "sht20_cfg.h"
#include "DaBai_usart.h"
#include "DaBai_ADC.h"
#include "DaBai_tim.h"
#include "DaBai_APP.h"
#include "DaBai_GPIO.h"


/* Variable used to get converted value */
__IO uint32_t uwADCxConvertedValue = 0;

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

static GPIO_InitTypeDef  GPIO_InitStruct;
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
	
float Sht20Temp = 0;
float Sht20RH = 0;
uint8_t uartData[5] = {1,2,3,4,5};
uint8_t ATCommand[] = "AT\r";
uint8_t  readFlag = 0;
uint32_t 	Delay1msCnt = 0;


int main(void)
{
	static  uint32_t start_tick = 0;
	static uint32_t cur_tick = 0;
 
  HAL_Init();
	/* Configure the System clock to have a frequency of 2 MHz (Up to 32MHZ possible) */
  SystemClock_Config();
	MX_GPIO_Init();
	MX_TIM_Init();
	MX_I2C2_Init();
	MX_ADC_Init();
	MX_USART1_UART_Init();
	MX_LPUART1_UART_Init();
	
	start_tick = HAL_GetTick();
	while((HAL_GetTick()- start_tick) <300)
	{
		// power on beep remind
	}
	printf("\r\nDaBai Init OK \r\n");
	
  /* Infinite loop */
  while (1)
  {
		KeyProcess();
		if(start_tick == 0)
			start_tick = HAL_GetTick();
		
		cur_tick = HAL_GetTick();
   
	  if((cur_tick - start_tick) > 100 )
		{
			start_tick = 0;
			cur_tick = 0;
			readFlag = 1;
		}
		else
		{
			readFlag = 0;
		}

    if (HAL_ADC_PollForConversion(&AdcHandle, 10) != HAL_OK)
    {
      /* End Of Conversion flag not set on time */
      
    }

    /* Check if the continuous conversion of regular channel is finished */
    if ((HAL_ADC_GetState(&AdcHandle) & HAL_ADC_STATE_REG_EOC) == HAL_ADC_STATE_REG_EOC)
    {
        /*##-6- Get the converted value of regular channel  ########################*/
        uwADCxConvertedValue = HAL_ADC_GetValue(&AdcHandle);
    }
		if(uwADCxConvertedValue > 500 || Sht20Temp > 29)
		{
			//HAL_TIM_PWM_Start(&TimHandle, TIM_CHANNEL_2);
			//HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7, GPIO_PIN_RESET);
		}
		else{
			HAL_TIM_PWM_Stop(&TimHandle, TIM_CHANNEL_2);
			//HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7, GPIO_PIN_SET);
		}
		if(readFlag)
		{
			//HAL_UART_Log(uartData,5);
			//printf("\r\nDaBai Init OK \r\n");
			//Sht20Temp = SHT20_Convert(SHT20_ReadTemp(),1);
			//Sht20RH = SHT20_Convert(SHT20_ReadRH(),0);
			HAL_GPIO_TogglePin(GPIOB,LED5);

			//HAL_LPUART1_Write(ATCommand,3);
		}
  }
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = MSI
  *            SYSCLK(Hz)                     = 2000000
  *            HCLK(Hz)                       = 2000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 1
  *            APB2 Prescaler                 = 1
  *            Flash Latency(WS)              = 0
  *            Main regulator output voltage  = Scale3 mode
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit;
//  /* Enable MSI Oscillator */
//  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
//  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
//  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_5;
//  RCC_OscInitStruct.MSICalibrationValue=0x00;
//  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
//  if (HAL_RCC_OscConfig(&RCC_OscInitStruct)!= HAL_OK)
//  {
//    /* Initialization Error */
//    while(1); 
//  }
	
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE|RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;//open 8M¾§Õñ
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;//open 32.768KHz¾§Õñ 
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }
  
  /* Select MSI as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSE;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;  
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0)!= HAL_OK)
  {
    /* Initialization Error */
    while(1); 
  }
  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();
  
  /* The voltage scaling allows optimizing the power consumption when the device is 
     clocked below the maximum system frequency, to update the voltage scaling value 
     regarding system frequency refer to product datasheet.  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);
  
  /* Disable Power Control clock */
  __HAL_RCC_PWR_CLK_DISABLE();
	
	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1|RCC_PERIPHCLK_USART2
                              |RCC_PERIPHCLK_LPUART1|RCC_PERIPHCLK_RTC|RCC_PERIPHCLK_I2C2;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
  PeriphClkInit.Lpuart1ClockSelection = RCC_LPUART1CLKSOURCE_PCLK1;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  /**Configure the Systick interrupt time 1ms
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}


/**
  * @brief  This function is executed in case of error occurrence.
  * @param  file: The file name as string.
  * @param  line: The line in file as a number.
  * @retval None
  */
void _Error_Handler(char *file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}


#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}

#endif

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
