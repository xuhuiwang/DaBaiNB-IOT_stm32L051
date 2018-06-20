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
						 电池电量 < 7%   ： 蜂鸣器高频响2s后自动关机

//////////////////////////////////////////////////////
4、传感器：
	光敏：当g_lightValue > LIGHT_THRESHOLD 时（光线越暗g_lightValue数值越大），蜂鸣器1Hz的频率响一次
	温度：当温度值g_Sht20Temp > TEMP_THRESHOLD 摄氏度时（g_Sht20Temp的数值就是表示实际的温度），蜂鸣器1Hz的频率响一次
	湿度：当湿度值g_Sht20RH > RH_THRESHOLD 时（湿度越大g_Sht20RH的数值就越大），蜂鸣器1Hz的频率响一次

//////////////////////////////////////////////////////
5、按键和灯接口：
	key1(S1)是系统的开关机按键（开机后也可作为MCU按键接口使用）、key4(S4)按键是MCU的复位按键。
	key1(S1)、key2(S2)、key3(S3)都可以作为MCU的按键输入接口来使用
	当key2(S2)、key3(S3)不作为MCU的按键接口使用时，可当作普通的IO口来使用
	
	LED5作为电池电量和充电相关的指示灯，请勿用这个灯作为其他状态的指示
	LED1、LED2、LED3、LED4用户可以用来自定义状态指示灯。
	当LED1、LED2、LED3、LED4不作为MCU的LED灯接口来使用时，可以当作普通的IO口来使用

//////////////////////////////////////////////////////
6、BC95和BC28指令：
		由于BC95和BC28的AT指令完全相同，所以本工程文件的一些函数命名方式都是按照之前的BC95来命名的，
		对BC28模块完全适用，开发者不必担心这个问题

//////////////////////////////////////////////////////
7、TCP网络连接：
	本例程提供的是：使用AT指令，让BC28通过TCP协议连接大白的测试服务器，发送数据，
	服务器收到后返回发送出去的数据。
	网络传输数据操作步骤：
	1、BC28核心板插入NB物联网卡，核心板插入STM32板底座
	
	*****（注意：不要插反了否则会引起短路，烧坏线路板，请对照我们的图片接插）*****
	
	2、插入STM32板的MicroUSB线，电脑打开串口助手，选择这个USB的串口号和波特率（115200bps）,
	打开此串口，这个串口可以看到单片机向BC28核心板发送的数据和核心板返回的数据。
	3、系统开机后，短按一下key1(S1)按键，
		程序会自动初始化BC28模块，并且自动进行以下流程的操作：
		(1)、读取sim卡信息。  
				命令：AT+CIMI
		(2)、查看网络附着状态，如果网络还没连接，会每秒钟自动查看网络状态，连续查看十次。
				 如果网络连接成功LED2会点亮。
				命令：AT+CGATT?
		(3)、如果入网成功，下一步会自动查看信号强度。
				命令：AT+CSQ
		(4)、查看信号质量没问题后，开始创建TCP Socket。
				命令：AT+NSOCR=STREAM,6,56000,1
		(5)、连接大白自己搭建的TCP服务器。
				命令：AT+NSOCO=1,123.206.108.227,9099
		(6)、发送TCP数据。  			
				命令：AT+NSOSD=1,9,4461426169494F5400  ("DaBaiIOT"字符串的十六进制显示为：4461426169494F5400)
		(7)、读取发送的数据。
				命令：AT+NSORF=1,9
		(8)、关闭TCP连接。
			  命令：AT+NSOCL=1


8、基于coap协议的网络连接

我们的Coap示例连接的是华为IOT平台。

小贴士：华为的IOP平台连接之前，需要将移远模块屏蔽罩上印的IMEI号绑定到华为的IOT平台，平台才会接收模块发过来的数据。
				我们默认发货都是帮亲们绑定好的，并且测试没问题的。
				
				1、在DaBai_rtc.c文件中的HAL_RTC_AlarmAEventCallback函数里设置了每隔10分钟发送一次coap数据到IOT平台。
				数据格式为：
	数据	    m_batVol  m_temp    mRH       m_light   m_longitude   m_latitude   g_USB_insert  Reserve
	数据类型  Uint8_t   Int16_t   Int16_t   Uint16_t  Uint32_t      Uint32_t     Uint8_t       Uint8_t
	
	系统开机后，会自动建立coap的连接，然后每隔10分钟上传一次板子上的传感器数据。
	（注意：刚开机的时候信号不是很稳定，需要等待几十秒后才可能有信号，注册到网络。）
	

//////////////////////////////////////////////////////

* 版权归大白物联网所有。
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "DaBai_i2c.h"
#include "DaBai_usart.h"
#include "DaBai_ADC.h"
#include "DaBai_tim.h"
<<<<<<< HEAD:DaBai_APP/main.c
#include "DaBai_GPIO.h"
#include "DaBai_APP.h"


/** @addtogroup STM32L0xx_HAL_Examples
  * @{
  */

=======
#include "DaBai_APP.h"
#include "DaBai_GPIO.h"
#include "NB_Board_Cfg.h"
#include "timer_user_poll.h"
#include "DaBai_rtc.h"
>>>>>>> dev:DaBai_Src/main.c


#define TASKTIME_10MS  		10
#define TASKTIME_100MS  	100
#define TASKTIME_500MS  	500
#define TASKTIME_1000MS   1000
#define TASKTIME_1MIN     60000
#define TASKTIME_10MIN    600000

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);



uint8_t  readFlag = 0;
volatile uint32_t 	Delay1msCnt = 0;
volatile uint16_t g_TaskTime10ms = 0;
volatile uint16_t g_TaskTime100ms = 0;
volatile uint16_t g_TaskTime500ms = 0;
volatile uint16_t g_TaskTime1000ms = 0;
volatile uint32_t g_TaskTime1min  = 0;
volatile uint32_t g_TaskTime10min = 0;

char userPacket[30]= {0};

/* Buffer used for displaying Time */
uint8_t aShowTime[50] = {0};

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
<<<<<<< HEAD:DaBai_APP/main.c
	
float Sht20Temp = 0;
float Sht20RH = 0;
uint8_t uartData[5] = {1,2,3,4,5};
uint8_t ATCommand[] = "AT\r";
uint8_t  readFlag = 0;
uint32_t 	Delay1msCnt = 0;

int main(void)
{
	static uint32_t start_tick = 0;
	static uint32_t cur_tick = 0;
	
=======
int main(void)
{
	static  uint32_t start_tick = 0;
	static uint32_t cur_tick = 0;
 
>>>>>>> dev:DaBai_Src/main.c
  HAL_Init();
  SystemClock_Config();
	MX_GPIO_Init();
	MX_TIM_Init();
	MX_GPIO_Init();
	MX_I2C2_Init();
	MX_ADC_Init();
	MX_USART1_UART_Init();
	MX_LPUART1_UART_Init();
<<<<<<< HEAD:DaBai_APP/main.c
 
	printf("\r\nDaBai Init OK \r\n");
	
=======
	MX_RTC_Init();
	
	NBModule_open(&nb_config);
  //APP_STATE = NB_NONE;
	APP_STATE = NB_CoAP_SEVER;
>>>>>>> dev:DaBai_Src/main.c
	start_tick = HAL_GetTick();
	while((HAL_GetTick()- start_tick) <300)
	{
		//when power on beep Hint
	}
	  /* Configure RTC Alarm */
  RTC_AlarmConfig();
	
	LED1_OFF;
	LED2_OFF;
	LED3_OFF;
	LED4_OFF;
	CHG_LED5_OFF;
	printf("\r\nDaBai Init OK \r\n");
	
  /* Infinite loop */
  while (1)
<<<<<<< HEAD:DaBai_APP/main.c
  {
		KeyProcess();
		if(start_tick == 0)
			start_tick = HAL_GetTick();
		
		cur_tick = HAL_GetTick();
   
	  if((cur_tick - start_tick) > 500 )
=======
  {	
		HAL_UART_Poll();
		NBModule_Main(&nb_config);
    MX_TimerPoll();
		
		if(g_TaskTime10ms > TASKTIME_10MS)
>>>>>>> dev:DaBai_Src/main.c
		{
			g_TaskTime10ms = 0;
			DaBai_10msTask();
		}
		if(g_TaskTime100ms > TASKTIME_100MS)
		{
			g_TaskTime100ms = 0;
			DaBai_100msTask();
		}
		
<<<<<<< HEAD:DaBai_APP/main.c
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
		if(uwADCxConvertedValue > 600 || Sht20Temp > 30)
=======
		if(g_TaskTime500ms > TASKTIME_500MS)
>>>>>>> dev:DaBai_Src/main.c
		{
			g_TaskTime500ms = 0;
			g_BeepFreq = 0;
			DaBai_500msTask();
		}
		if(g_TaskTime1000ms > TASKTIME_1000MS)
		{
			g_TaskTime1000ms = 0;
			DaBai_1000msTask();
			 //RTC_TimeShow(aShowTime);
		}
		if(g_TaskTime1min > TASKTIME_1MIN)
		{
<<<<<<< HEAD:DaBai_APP/main.c
			HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_7);
			//HAL_UART_Log(uartData,5);
			//printf("\r\nDaBai Init OK \r\n");
			Sht20Temp = SHT20_Convert(SHT20_ReadTemp(),1);
			Sht20RH = SHT20_Convert(SHT20_ReadRH(),0);
			//HAL_LPUART1_Write(ATCommand,3);
=======
			g_TaskTime1min = 0;
			//DaBai_1MinTask();
		}
		
		if(g_TaskTime10min > TASKTIME_10MIN)
		{
			g_TaskTime10min = 0;
			//DaBai_10MinTask();
		}
		if(g_RTCAlarmFlag == 1)
		{
			g_RTCAlarmFlag = 0;
			APP_STATE = NB_CoAP_ST;
>>>>>>> dev:DaBai_Src/main.c
		}
		switch(APP_STATE)
    {
    case NB_NONE:
      {
        
      }
      break;
    case NB_INIT:
      {
        printf("\r\n<----BC28 Init---->\r\n");
        NBModule_Init(&nb_config);
        APP_STATE = NB_END;
      }
      break;
    case NB_SIGN:
      {
         printf("\r\n<----BC28 get signal---->\r\n");
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
        printf("\r\n<----Close tcp ---->\r\n");
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
			 case NB_CoAP_SEVER:
      {
        printf("\r\n<---- CoAP Server set ---->\r\n");
        NBModule_CoAPServer(&nb_config,1,NULL);
        APP_STATE = NB_END;
      }
      break;
    case NB_CoAP_ST:
      {
				uint8_t  m_batVol;
				int16_t m_temp,m_RH;
				uint16_t m_light;
				uint32_t m_longitude,m_latitude;
				
				m_batVol = g_BatVoltage;
				m_temp = (int16_t)g_Sht20Temp;
				m_RH   = (int16_t)g_Sht20RH;
				m_light = g_lightValue;
				m_longitude = (uint32_t)g_longitude*1000000;
				m_latitude  = (uint32_t)g_latitude*1000000; 
				
//				m_batVol = 1;
//				m_temp = -2;
//				m_RH   = 0x03;
//				m_light = 0x04;
//				m_longitude = 0x05;
//				m_latitude  = 0x06;
				
				userPacket[0] = m_batVol;
				Fill_int16_To_int8(m_temp,&userPacket[1],&userPacket[2]);
				Fill_int16_To_int8(m_RH,&userPacket[3],&userPacket[4]);
				Fill_u16_To_u8(m_light,&userPacket[5],&userPacket[6]);
				Fill_u32_To_u8(m_longitude,&userPacket[7],&userPacket[8],&userPacket[9],&userPacket[10]);
				Fill_u32_To_u8(m_latitude,&userPacket[11],&userPacket[12],&userPacket[13],&userPacket[14]);		
				userPacket[15] = 	g_USB_insert;
			
				bc95_coapSendMsg(&nb_config,sizeof(userPacket),userPacket);
        APP_STATE = NB_END;
      }
      break;
    case NB_CoAP_RE:
      {
        
      }
      break;
    default:
      {
        
      }
      break;
    }
		
		SetBeepFreq(g_BeepFreq);

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
<<<<<<< HEAD:DaBai_APP/main.c

	/**Configure LSE Drive Capability */
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);
	
	  /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE|RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL4;
=======
	
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE|RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;//open 8M晶振
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;//open 32.768KHz晶振 
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  //RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  //RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	//RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL4;
>>>>>>> dev:DaBai_Src/main.c
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }
<<<<<<< HEAD:DaBai_APP/main.c
	
//	    /**Initializes the CPU, AHB and APB busses clocks 
//    */
//  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_MSI;
//  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
//  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
//  RCC_OscInitStruct.MSICalibrationValue = 0;
//  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_5;
//  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
//  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
//  {
//    _Error_Handler(__FILE__, __LINE__);
//  }

//   /* Enable MSI Oscillator */
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
	
  /* Select PLLCLK as system clock source and configure the HCLK, PCLK1 and PCLK2 
=======
  
  /* Select HSE as system clock source and configure the HCLK, PCLK1 and PCLK2 
>>>>>>> dev:DaBai_Src/main.c
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
	
//  /* Enable Power Control clock */
//  __HAL_RCC_PWR_CLK_ENABLE();
//  
//  /* The voltage scaling allows optimizing the power consumption when the device is 
//     clocked below the maximum system frequency, to update the voltage scaling value 
//     regarding system frequency refer to product datasheet.  */
//  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);
//  
//  /* Disable Power Control clock */
//  __HAL_RCC_PWR_CLK_DISABLE();
	
	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1
                              |RCC_PERIPHCLK_LPUART1|RCC_PERIPHCLK_USART2|RCC_PERIPHCLK_RTC|RCC_PERIPHCLK_I2C2;//
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
<<<<<<< HEAD:DaBai_APP/main.c
  PeriphClkInit.Lpuart1ClockSelection = RCC_LPUART1CLKSOURCE_LSE;
=======
  PeriphClkInit.Lpuart1ClockSelection = RCC_LPUART1CLKSOURCE_PCLK1;
>>>>>>> dev:DaBai_Src/main.c
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    //_Error_Handler(__FILE__, __LINE__);
  }

<<<<<<< HEAD:DaBai_APP/main.c
  /**Configure the Systick interrupt time*/
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/2000);

  /**Configure the Systick */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
	
=======
  /**Configure the Systick interrupt time 1ms
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
>>>>>>> dev:DaBai_Src/main.c
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
//  while(1)
//  {
//  }
  /* USER CODE END Error_Handler_Debug */
}


<<<<<<< HEAD:DaBai_APP/main.c
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

/************************ (C) COPYRIGHT DaBai_IOT Club *****END OF FILE****/
=======
/************************ (C) COPYRIGHT DaBai_IOT *****END OF FILE****/
>>>>>>> dev:DaBai_Src/main.c
