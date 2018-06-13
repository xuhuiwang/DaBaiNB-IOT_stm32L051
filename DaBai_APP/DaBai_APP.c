/**
  ******************************************************************************
  * File Name          : DABAI_APP.c
  * Description        : This file provides code for the configuration
  *                      of the DABAI_APP instances.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "DaBai_APP.h"
#include "DaBai_GPIO.h"
#include "DaBai_tim.h"
#include "DaBai_ADC.h"
#include "sht20_cfg.h"
#include "NB_Board.h"

GPIO_PinState m_key[3] = {GPIO_PIN_RESET,GPIO_PIN_RESET,GPIO_PIN_RESET};
volatile uint8_t  m_key_flag[3]= {0};
volatile uint32_t m_key_counter[3] = {0};
volatile uint8_t  m_key_State[3] = {0};

GPIO_PinState m_chargeState = GPIO_PIN_RESET;
volatile uint8_t m_charge_flag = {0};
volatile uint8_t m_charge_counter = {0};
volatile Yes_No_Status g_USB_insert = NO;
volatile Yes_No_Status g_chargeing_flag = NO;

uint32_t m_fullBatTimeCnt = 0;//记录充电时达到100%以上的时间

uint8_t  g_power_off_flag = 0;
uint16_t g_lightValue = 0;
float    g_Sht20Temp = 0;
float    g_Sht20RH = 0;
uint16_t g_BeepFreq = 0;
uint8_t g_BatVoltage = 0;
uint8_t g_BatVoltageLow = 0;//电量低于10%标志位
uint8_t m_fullBatFlag = 0;
uint8_t m_fullBatHint = 0;

volatile NB_STATE_e  APP_STATE= NB_NONE;


/*************************************
fn : BeepToggle

brief : 蜂鸣器反转函数
param : none

return : none

*************************************/

void BeepToggle(void)
{
	static uint8_t m_lock = 0;
	
	if(m_lock == 0)
	{
		m_lock = 1;
		g_BeepFreq = 1000;//1KHz
	}
	else if(m_lock == 1)
	{
		m_lock = 0;
		g_BeepFreq = 0;
	}
}
/*************************************
fn : KeyTask

brief : 按键处理任务
param : none

return : none

*************************************/

void KeyTask(void)
{
	uint8_t i = 0;
	m_key_State[KEY1] = 0,m_key_State[KEY2] = 0,m_key_State[KEY3] = 0;
	
	/*读取按键状态*/
	if(HAL_GPIO_ReadPin(GPIOA, KEY1_PIN) == GPIO_PIN_SET)
		m_key[KEY1] = GPIO_PIN_RESET;
	else
		m_key[KEY1] = GPIO_PIN_SET;
	
	m_key[KEY2] = HAL_GPIO_ReadPin(GPIOB, KEY2_PIN);
	m_key[KEY3] = HAL_GPIO_ReadPin(GPIOB, KEY3_PIN);
		
	for(i = 0; i < 3; i++)//分别对3个按键的状态进行处理
	{
		m_key_flag[i] <<= 1;
		if( m_key[i] == GPIO_PIN_RESET )//按键被按下
		{
			m_key_flag[i] |= 0x01;
		}
		
		if( m_key_flag[i] == 0xff )//按键消斗，按键被稳定按下，这时m_key_flag等于0xff,直到按键松开
		{
			m_key_counter[i]++;//记录按键被按下标志，数值越大被按下时间越长
			if(m_key_counter[i] == 0) m_key_counter[i] = 1;//按的时间过长不至于被清零
			switch(i)
			{
				case KEY1:
				{ 
					m_key_State[KEY1] = 1;
					if(m_key_counter[KEY1] > 100)//长按key1按键1s以上
					{
						g_power_off_flag = 1;
						PowerOffGpioConfig();
						if(m_key_counter[KEY1] < 130)
							g_BeepFreq = 2000;
					}
				}break;
				case KEY2:
				{
					m_key_State[KEY2] = 1;
				}break;

				case KEY3:
				{
					m_key_State[KEY3] = 1;
				}break;

				default:
				{}break;
			}
		}
		else if(m_key_flag[i] == 0x00)//松手消斗
		{	
			if( m_key_counter[i] >= 1 )//相当于松手检测
			{
				switch(i)
				{
					case KEY1:
					{ 
						APP_STATE = NB_INIT;
						if(g_power_off_flag == 1)
							POWER_OFF;	
					}break;
					case KEY2:
					{					
						printf("key2 pressed\r\n");
						APP_STATE = NB_SIGN;
					}break;

					case KEY3:
					{
						printf("key3 pressed\r\n");
						APP_STATE = NB_TCP_CR;//NB_TCP_ST;
					}break;
					
					default:
					{}break;
				}	
				
				m_key_counter[i] = 0;
			}
		}
	}
	
	if(m_key_State[KEY2] == 1 || m_key_State[KEY3] == 1)
	{

	}
	else
	{

	}

}

/*************************************
fn : ChargeTask

brief : 电池充电任务
1、充电的时候LED5电量指示灯会1s闪烁一次，充满电后LED5常亮
param : none

return : none

*************************************/

void ChargeTask(void)
{
	uint8_t i;
	
	m_chargeState	= HAL_GPIO_ReadPin(GPIOA, CHG_DETECT_PIN);//USB接口插入检测

	if( m_chargeState == GPIO_PIN_SET )////USB插入
	{
		m_charge_counter++;
		if(m_charge_counter == 0) m_charge_counter = 1;//防止充电时间过长不至于加到最大值被清零
		
		g_USB_insert = YES;//USB接口插入
  	g_chargeing_flag = YES;//在给电池充电
	}
	else
	{
		if( m_charge_counter >= 1 )//USB拔出检测
		{ 
			m_charge_counter = 0;
			g_USB_insert = NO;
			g_chargeing_flag = NO;
			m_fullBatFlag = 0;
			m_fullBatHint = 0;
		}
	}

	if(g_USB_insert == YES)//USB插入
	{
		if(g_chargeing_flag == YES)//电池在充电
		{		
			if(m_fullBatTimeCnt < 5000)//15分钟
			{
				HAL_GPIO_TogglePin(GPIOB,CHG_LED5_PIN);//在充电时1s闪烁一次
			}	
			else//电池电量达到100%后的15分钟，多充一会，让电量达到最高
			{
				g_chargeing_flag = NO;
				m_fullBatFlag = 1;
			}

		}
		if(m_fullBatFlag == 1)//电池充满电
		{
			CHG_LED5_ON;//充满电，一直亮

			if(m_fullBatHint <  10)//充满电蜂鸣器鸣响5次提示
			{	
				m_fullBatHint++;
				BeepToggle();
			}
		}
	}

}

/*************************************
fn : BatManageTask

brief : 电池管理任务
1、充电的时候LED5电量指示灯会1s闪烁一次，充满电后LED5常亮
2、放电的时候 ：
		     电池电量 >= 10% ： LED5熄灭
	 7% <= 电池电量 < 10%  ： LED5以5Hz频率闪烁，并且蜂鸣器5Hz频率报警
		     电池电量 < 7%   ： 蜂鸣器响1s后自动关机
param : none

return : none

*************************************/

void BatManageTask(void)
{
	uint16_t start_tick = 0;
	g_BatVoltageLow = 0;
	
	ChargeTask();
	
	g_BatVoltage = GetBatVoltage();
	
	if(g_USB_insert == NO && g_BatVoltage >= 10)//电池电量大于10%
	{
		CHG_LED5_OFF;
	}	
	else if(g_USB_insert == NO && g_BatVoltage >= 7)//7%<=电池电量<10%
	{
		g_BatVoltageLow = 1;//电量低标志位
	}
	else if(g_USB_insert == NO && g_BatVoltage < 7)//电池电量<7%
	{
		g_power_off_flag = 1;
		start_tick = HAL_GetTick();
		while((HAL_GetTick()- start_tick) <1000)
		{
			g_BeepFreq = 2000;
			SetBeepFreq(g_BeepFreq);
		}
			POWER_OFF;
	}
}

void DaBaiSensorTask(void)
{
	
	g_lightValue = getLightValue();
	if(g_lightValue > 700 || g_Sht20Temp > 31 || g_Sht20RH > 70)
	{
		g_BeepFreq = 1500;	
	}
	g_Sht20Temp = SHT20_Convert(SHT20_ReadTemp(),1);
	g_Sht20RH   = SHT20_Convert(SHT20_ReadRH(),0);
}


/*************************************
fn : DaBai_10msTask

brief : 10ms任务
param : none

return : none

*************************************/
void DaBai_10msTask(void)
{
	KeyTask();
}
/*************************************
fn : DaBai_100msTask

brief : 100ms任务
param : none

return : none

*************************************/
void DaBai_100msTask(void)
{
	//HAL_GPIO_TogglePin(GPIOB,LED1_PIN);
	if(g_BatVoltageLow == 1)
	{
		BeepToggle();
		HAL_GPIO_TogglePin(GPIOB,CHG_LED5_PIN);
	}
}

/*************************************
fn : DaBai_500msTask

brief : 500ms任务
param : none

return : none

*************************************/

void DaBai_500msTask(void)
{
	HAL_GPIO_TogglePin(GPIOB,LED4_PIN);
	DaBaiSensorTask();
	BatManageTask();
}

/*************************************
fn : DaBai_1000msTask

brief : 1000ms任务
param : none

return : none

*************************************/

void DaBai_1000msTask(void)
{

}



//******************************************************************************
// fn : NB_MsgreportCb
//
// brief : NB模块消息上报回调
//
// param : 
//
// return : none
int  NB_MsgreportCb(msg_types_t types,int len,char* msg)
{
  switch(types)
  {
  case MSG_INIT:
    {
      printf("\r\nINIT=%s\r\n",msg);
      if(*msg == 'S')
      {
        LED1_ON;
        APP_STATE = NB_SIGN;
      }
    }
    break;
  case MSG_IMSI:
    {
      printf("\r\nIMSI=%s\r\n",msg);
    }
    break;
  case MSG_REG:
    {
			printf("\r\nNET=%s\r\n",(*msg) == 1 ?"ON":"0FF");
    }
    break;
  case MSG_SIGN:
    {
      printf("\r\n%sdbm\r\n",msg);
			APP_STATE = NB_TCP_CR;
    }
    break;
  case MSG_MODULE_INFO:
    {
      printf("\r\nMinfo=%s\r\n",msg);
    }
    break;
  case MSG_MID:
    {
      printf("\r\nMID=%s\r\n",msg);
    }
    break;
  case MSG_MMODEL:
    {
      printf("\r\nModel=%s\r\n",msg);
    }
    break;
  case MSG_MREV:
    {
      printf("\r\nREV=%s\r\n",msg);
    }
    break;
  case MSG_BAND:
    {
      printf("\r\nFreq=%s\r\n",msg);
    }
    break;
  case MSG_IMEI:
    {
      printf("\r\nIMEI=%s\r\n",msg);
    }
    break;
  case MSG_UDP_CREATE:
    {
      printf("\r\nUDP_CR=%s\r\n",msg);
    }
    break;
  case MSG_UDP_CLOSE:
    {
      printf("\r\nUDP_CL=%s\r\n",msg);
    }
    break;
  case MSG_UDP_SEND:
    {
      printf("\r\nUDP_SEND=%s\r\n",msg);
    }
    break;
  case MSG_UDP_RECE:
    {
      printf("\r\nUDP_RECE=%s\r\n",msg);
    }
    break;
	case MSG_TCP_CREATE:
    {
      printf("\r\nTCP_CR=%s\r\n",msg);
			APP_STATE = NB_TCP_CNT;
    }
    break;
  case MSG_TCP_CLOSE:
    {
      printf("\r\nTCP_CL=%s\r\n",msg);
    }
    break;
		
	case MSG_TCP_CONNECT:
    {
      printf("\r\nTCP_CONNECT=%s\r\n",msg);
			APP_STATE = NB_TCP_ST;
    }
    break;	
		
  case MSG_TCP_SEND:
    {
      printf("\r\nTCP_SEND=%s\r\n",msg);
    }
    break;
  case MSG_TCP_RECE:
    {
      printf("\r\nTCP_RECE=%s\r\n",msg);
			APP_STATE = NB_TCP_CL;
    }
    break;
  case MSG_COAP:
    {
      printf("\r\nCOAP=%s\r\n",msg);
    }
    break;
  case MSG_COAP_SEND:
    {
      printf("\r\nCOAP_SENT=%s\r\n",msg);
    }
    break;
    
  case MSG_COAP_RECE:
    {
      printf("\r\nCOAP_RECE=%s\r\n",msg);
    }
    break;
  default :
    {
      break;
    }
  }
  return 0;
}


/************************ (C) COPYRIGHT DaBai_IOT *****END OF FILE****/
