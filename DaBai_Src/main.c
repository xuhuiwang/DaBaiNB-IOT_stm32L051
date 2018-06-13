/**
******************************************************************************
* 文件名程: main.c 
* 作    者: DaBai_IOT Cifi
* 版    本: V1.0
* 编写日期: 2018-6-04
* 功    能: BC95、BC28 AT指令驱动程序
******************************************************************************
* 说明：
* 本例程配套大白stm32开发板DaBai_L051_V1.0使用。
* 
大白淘宝网址：https://dabairobot.taobao.com

下面是对本开发板基本操作的说明：

//////////////////////////////////////////////////////
1、供电说明：
在接入电池后，系统不会自动开机，需要短按key1(S1)按键才会开机。
在不接入电池时，可以直接用USB线供电，插入USB线后，系统会自动上电开机。

//////////////////////////////////////////////////////
2、开关机逻辑：
		开机：短按key1(S1)按键，系统会开机，蜂鸣器会响0.3s,5个LED灯全亮0.3s,随后系统运行指示灯LED1以1Hz的频率闪烁
		关机：长按key1(S1)按键1s以上，蜂鸣器会响1s左右，蜂鸣器鸣响结束后松开key1(S1)按键，系统会自动关机

//////////////////////////////////////////////////////
3、电池管理任务：
		充电：充电的时候电量指示灯LED5会1s闪烁一次，充满电后LED5常亮并且蜂鸣器鸣响5次提示
		放电：放电的时候 
						 电池电量 >= 10% ： LED5熄灭
			 7% <= 电池电量 < 10%  ： LED5以5Hz频率闪烁，并且蜂鸣器以5Hz频率报警
						 电池电量 < 7%   ： 蜂鸣器响1s后自动关机

//////////////////////////////////////////////////////
4、传感器：
	光敏：当g_lightValue > 700时（光线越暗g_lightValue数值越大），蜂鸣器响
	温度：当温度值g_Sht20Temp > 31摄氏度时（g_Sht20Temp的数值就是表示实际的温度），蜂鸣器响
	湿度：当湿度值g_Sht20RH > 70时（湿度越大g_Sht20RH的数值就越大），蜂鸣器响

//////////////////////////////////////////////////////
5、按键和灯接口：
	key4(S4)按键是MCU的复位按键。
	key1(S1)、key2(S2)、key3(S3)都可以作为MCU的按键输入接口来使用
	当key2(S2)、key3(S3)不作为MCU的按键接口使用时，可当作普通的IO口来使用
	
	LED5作为电池电量和充电相关的指示灯，请勿用这个灯作为其他状态的指示
	LED1、LED2、LED3、LED4用户可以用来自定义状态指示灯。
	当LED1、LED2、LED3、LED4不作为MCU的LED灯接口来使用时，可以当作普通的IO口来使用

//////////////////////////////////////////////////////
6、BC95和BC28指令：
		由于BC95和BC28的AT指令完全相同，所以本工程文件的一些函数命名方式都是按照之前的BC95来命名的，
		对BC28模块完全适用，开发者不必担心这个问题
		
7、网络连接：
	本例程提供的是：使用AT指令，让BC28通过TCP协议连接大白的测试服务器，发送数据，
	服务器收到后返回发送出去的数据。
	网络传输数据操作步骤：
	1、BC28核心板插入NB物联网卡，核心板插入STM32板底座。
	2、插入STM32板的MicroUSB线，电脑打开串口助手，选择这个USB的串口号和波特率（115200bps）,打开此串口。
	3、系统开机后，短按一下key1(S1)按键，
		程序会自动初始化BC28模块，并且进行以下流程的操作：
		(1)、
	后续还会推出基于coap协议的网络连接，用于连接IOT平台。
	


//////////////////////////////////////////////////////

* 版权归大白物联网所有。
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "DaBai_i2c.h"
#include "DaBai_usart.h"
#include "DaBai_ADC.h"
#include "DaBai_tim.h"
#include "DaBai_APP.h"
#include "DaBai_GPIO.h"
#include "NB_Board_Cfg.h"
#include "timer_user_poll.h"

#define TASKTIME_10MS  		10
#define TASKTIME_100MS  	100
#define TASKTIME_500MS  	500
#define TASKTIME_1000MS   1000

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

static GPIO_InitTypeDef  GPIO_InitStruct;
/* Private functions ---------------------------------------------------------*/


	

uint8_t uartData[5] = {1,2,3,4,5};
uint8_t  readFlag = 0;
uint32_t 	Delay1msCnt = 0;
uint16_t g_TaskTime10ms = 0;
uint16_t g_TaskTime100ms = 0;
uint16_t g_TaskTime500ms = 0;
uint16_t g_TaskTime1000ms = 0;


/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
	static  uint32_t start_tick = 0;
	static uint32_t cur_tick = 0;
 
  HAL_Init();
  SystemClock_Config();
	MX_TIM_Init();
	MX_GPIO_Init();
	MX_I2C2_Init();
	MX_ADC_Init();
	MX_USART1_UART_Init();
	MX_LPUART1_UART_Init();
	
	NBModule_open(&nb_config);
  APP_STATE = NB_NONE;
	
	start_tick = HAL_GetTick();
	while((HAL_GetTick()- start_tick) <300)
	{
		// power on beep remind
	}
	LED1_OFF;
	LED2_OFF;
	LED3_OFF;
	LED4_OFF;
	CHG_LED5_OFF;
	printf("\r\nDaBai Init OK \r\n");
	
  /* Infinite loop */
  while (1)
  {	
		HAL_UART_Poll();
		NBModule_Main(&nb_config);
    MX_TimerPoll();
		
		if(g_TaskTime10ms > TASKTIME_10MS)
		{
			g_TaskTime10ms = 0;
			DaBai_10msTask();
		}
		if(g_TaskTime100ms > TASKTIME_100MS)
		{
			g_TaskTime100ms = 0;
			DaBai_100msTask();
		}
		
		if(g_TaskTime500ms > TASKTIME_500MS)
		{
			g_TaskTime500ms = 0;
			g_BeepFreq = 0;
			DaBai_500msTask();
		}
		if(g_TaskTime1000ms > TASKTIME_1000MS)
		{
			g_TaskTime1000ms = 0;
			DaBai_1000msTask();
		}
		switch(APP_STATE)
    {
    case NB_NONE:
      {
        
      }
      break;
    case NB_INIT:
      {
        printf("\r\n<----BC95\28 Init---->\r\n");
        NBModule_Init(&nb_config);
        APP_STATE = NB_END;
      }
      break;
    case NB_SIGN:
      {
         printf("\r\n<----BC95\28 get signal---->\r\n");
         NBModule_Sign(&nb_config);
         APP_STATE = NB_END;
      }
      break;
    case NB_MODULE:
      {
        printf("\r\n<----Module info ---->\r\n");
        NBModule_Info(&nb_config);
        APP_STATE = NB_END;
      }
      break;
    case NB_TCP_CR:
      {
        printf("\r\n<----Create tcp ---->\r\n");
        NBModule_CreateTCP(&nb_config);
        APP_STATE = NB_END;
      }
      break;
    case NB_TCP_CL:
      {
        printf("\r\n<----Close udp ---->\r\n");
        NBModule_CloseTCP(&nb_config);
        APP_STATE = NB_END;
      }
      break;
    case NB_TCP_CNT:
      {
        printf("\r\n<---- Connect tcp ---->\r\n");
        NBModule_ConnectTcp(&nb_config);
        
        APP_STATE = NB_END;
      }
      break;
		case NB_TCP_ST:
      {
        printf("\r\n<---- Send tcp ---->\r\n");
        char* userPacket = "DaBaiIOT";
        NBModule_SendTcpData(&nb_config,sizeof("DaBaiIOT"),userPacket);
        
        APP_STATE = NB_END;
      }
      break;
    case NB_TCP_RE:
      {
        //do nothing
        APP_STATE = NB_END; 
      }
      break;
    default:
      {
        
      }
      break;
    }
		
		SetBeepFreq(g_BeepFreq);
		
	/**uart debug start**/	
	//HAL_UART_Log(uartData,5);	
	//HAL_UART_Transmit_DMA(&hlpuart1,uartData,5); 
	//HAL_LPUART1_Write(uartData,5);
	/**uart debug end**/
  }
	
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = HSE
  *            SYSCLK(Hz)                     = 8000000
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit;
	
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE|RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;//open 8M晶振
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;//open 32.768KHz晶振 
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }
  
  /* Select HSE as system clock source and configure the HCLK, PCLK1 and PCLK2 
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
	
	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1
                              |RCC_PERIPHCLK_LPUART1|RCC_PERIPHCLK_USART2|RCC_PERIPHCLK_RTC|RCC_PERIPHCLK_I2C2;//
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
  PeriphClkInit.Lpuart1ClockSelection = RCC_LPUART1CLKSOURCE_PCLK1;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
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


/************************ (C) COPYRIGHT DaBai_IOT *****END OF FILE****/
