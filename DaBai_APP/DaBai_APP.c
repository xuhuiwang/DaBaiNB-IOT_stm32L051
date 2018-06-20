/**
  ******************************************************************************
  * File Name          : DABAI_APP.c
  * Description        : This file provides code for the configuration
  *                      of the DABAI_APP instances.
  ******************************************************************************
<<<<<<< HEAD
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2018 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
=======
>>>>>>> dev
  */

/* Includes ------------------------------------------------------------------*/
#include "DaBai_APP.h"
<<<<<<< HEAD
#include "main.h"
#include "DaBai_GPIO.h"

GPIO_PinState key[3] = {GPIO_PIN_SET,GPIO_PIN_SET,GPIO_PIN_SET};
uint8_t  key_flag[3]= {0};
uint16_t key_counter[3] = {0};
uint8_t  key_State[3] = {0};
void KeyProcess(void)
{
	uint8_t i = 0;
	key_State[KEY1] = 0,key_State[KEY2] = 0,key_State[KEY3] = 0;
	
	/*读取按键状态*/
	key[KEY1] = HAL_GPIO_ReadPin(GPIOA, KEY1_Pin);
	key[KEY2] = HAL_GPIO_ReadPin(GPIOB, KEY2_Pin);
	key[KEY3] = HAL_GPIO_ReadPin(GPIOB, KEY3_Pin);
		
	for(i = 0; i < 3; i++)//分别对3个按键的状态进行处理
	{
		key_flag[i] <<= 1;
		if( key[i] == GPIO_PIN_RESET )//按键被按下
		{
			key_flag[i] |= 0x01;
		}
		
		if( key_flag[i] == 0xff )//按键消斗，按键被稳定按下，这时key_flag等于0xff,直到按键松开
		{
			key_counter[i]++;//记录按键被按下标志，数值越大被按下时间越长
			if(key_counter[i] == 0) key_counter[i] = 1;//按的时间过长不至于被清零
=======
#include "DaBai_GPIO.h"
#include "DaBai_tim.h"
#include "DaBai_ADC.h"
#include "sht20_cfg.h"
#include "NB_Board.h"
#include "DaBai_rtc.h"


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

uint8_t g_BatVoltage = 0;
uint16_t g_lightValue = 0;
float    g_Sht20Temp = 0;
float    g_Sht20RH = 0;
uint16_t g_BeepFreq = 0;
float    g_longitude = 0;//经度
float    g_latitude = 0;//纬度


uint8_t g_BatVoltageLow = 0;//电量低于10%标志位
uint8_t m_fullBatFlag = 0;
uint8_t m_fullBatHintCnt = 0;
uint32_t m_coapSendTimes = 0;

volatile NB_STATE_e  APP_STATE= NB_NONE;


void Fill_u16_To_u8(uint16_t x, char* h, char* l)
{
	*h = (char)(x >> 8);
	*l = (char)(x >> 0);
}

void Fill_int16_To_int8(int16_t x, char* h, char* l)
{
	*h = (char)(x >> 8);
	*l = (char)(x >> 0);
}

void Fill_u32_To_u8(uint32_t x, char* hh, char* hl, char* lh, char* ll)
{
	*hh = (char)(x >> 24);
	*hl = (char)(x >> 16);
	*lh = (char)(x >> 8);
	*ll = (char)(x >> 0);
}

/*************************************
fn : BeepToggle

brief : 蜂鸣器反转函数
param : none

return : none

*************************************/

void BeepToggle(uint16_t beepFrequency)
{
	static uint8_t m_lock = 0;
	
	if(m_lock == 0)
	{
		m_lock = 1;
		g_BeepFreq = beepFrequency;//1KHz
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
>>>>>>> dev
			switch(i)
			{
				case KEY1:
				{ 
<<<<<<< HEAD
					key_State[KEY1] = 1;
				}break;
				case KEY2:
				{
					key_State[KEY2] = 1;
=======
					m_key_State[KEY1] = 1;
					if(m_key_counter[KEY1] > 100)//长按key1按键1s以上
					{
						g_power_off_flag = 1;
						PowerOffGpioConfig();
						if(m_key_counter[KEY1] < 130)
							g_BeepFreq = BEEP_FREQ_2000Hz;
					}
				}break;
				case KEY2:
				{
					m_key_State[KEY2] = 1;
>>>>>>> dev
				}break;

				case KEY3:
				{
<<<<<<< HEAD
					key_State[KEY3] = 1;
=======
					m_key_State[KEY3] = 1;
>>>>>>> dev
				}break;

				default:
				{}break;
			}
		}
<<<<<<< HEAD
		else if(key_flag[i] == 0x00)//松手消斗
		{	
			if( key_counter[i] >= 1 )//相当于松手检测
=======
		else if(m_key_flag[i] == 0x00)//松手消斗
		{	
			if( m_key_counter[i] >= 1 )//相当于松手检测
>>>>>>> dev
			{
				switch(i)
				{
					case KEY1:
					{ 
<<<<<<< HEAD

					}break;
					case KEY2:
					{

=======
						printf("key1 pressed\r\n");
						APP_STATE = NB_INIT;
						if(g_power_off_flag == 1)
							POWER_OFF;	
					}break;
					case KEY2:
					{					
						printf("key2 pressed\r\n");
						APP_STATE = NB_CoAP_SEVER;
						//APP_STATE = NB_SIGN;
>>>>>>> dev
					}break;

					case KEY3:
					{
<<<<<<< HEAD

=======
						printf("key3 pressed\r\n");
						//APP_STATE = NB_TCP_CR;//NB_TCP_ST;
>>>>>>> dev
					}break;
					
					default:
					{}break;
				}	
				
<<<<<<< HEAD
				key_counter[i] = 0;
=======
				m_key_counter[i] = 0;
>>>>>>> dev
			}
		}
	}
	
<<<<<<< HEAD
	if(key_State[KEY2] == 1 || key_State[KEY3] == 1)
	{
		//HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET);
	}
	else
	{
		//HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET);
=======
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
			m_fullBatHintCnt = 0;
		}
	}

	if(g_USB_insert == YES)//USB插入
	{
		if(g_chargeing_flag == YES)//电池在充电
		{		
			if(m_fullBatTimeCnt < 1200000)//20分钟
			{
				HAL_GPIO_TogglePin(GPIOB,CHG_LED5_PIN);//在充电时1s闪烁一次
			}	
			else//电池电量达到100%后的20分钟，多充一会，让电量达到最高
			{
				g_chargeing_flag = NO;
				m_fullBatFlag = 1;
			}

		}
		if(m_fullBatFlag == 1)//电池充满电
		{
			CHG_LED5_ON;//充满电，一直亮

			if(m_fullBatHintCnt <  10)//充满电蜂鸣器鸣响5次提示
			{	
				m_fullBatHintCnt++;
				BeepToggle(BEEP_FREQ_1000Hz);
			}
		}
>>>>>>> dev
	}

}

<<<<<<< HEAD



/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
=======
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
		while((HAL_GetTick()- start_tick) <2000)
		{
			g_BeepFreq = BEEP_FREQ_2000Hz;
			SetBeepFreq(g_BeepFreq);
		}
			POWER_OFF;
	}
}

void DaBaiSensorTask(void)
{
	
	g_lightValue = getLightValue();
	if(g_lightValue > LIGHT_THRESHOLD || g_Sht20Temp > TEMP_THRESHOLD || g_Sht20RH > RH_THRESHOLD)//传感器数值超出设定的阈值
	{
		//g_BeepFreq = BEEP_FREQ_1500Hz;
		//BeepToggle(BEEP_FREQ_2500Hz);		
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
		BeepToggle(BEEP_FREQ_1000Hz);
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
	HAL_GPIO_TogglePin(GPIOB,LED1_PIN);
	DaBaiSensorTask();
	BatManageTask();//电池管理任务，切勿关闭此函数，否则电池不易管理，无法查看电池状态
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

/*************************************
fn : DaBai_1MinTask

brief : 60s任务
param : none

return : none

*************************************/
void DaBai_1MinTask(void)
{
	APP_STATE = NB_CoAP_ST;
}

/*************************************
fn : DaBai_10MinTask

brief : 60s任务
param : none

return : none

*************************************/
void DaBai_10MinTask(void)
{
	APP_STATE = NB_CoAP_ST;
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
      printf("\r\nINIT = %s\r\n",msg);
      if(*msg == 'S')
      {
        LED2_ON;
        APP_STATE = NB_SIGN;
      }
    }
    break;
  case MSG_IMSI:
    {
      printf("\r\nIMSI = %s\r\n",msg);
    }
    break;
  case MSG_REG:
    {
			printf("\r\nNET = %s\r\n",(*msg) == 1 ?"ON":"0FF");
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
      printf("\r\nMinfo = %s\r\n",msg);
    }
    break;
  case MSG_MID:
    {
      printf("\r\nMID = %s\r\n",msg);
    }
    break;
  case MSG_MMODEL:
    {
      printf("\r\nModel = %s\r\n",msg);
    }
    break;
  case MSG_MREV:
    {
      printf("\r\nREV = %s\r\n",msg);
    }
    break;
  case MSG_BAND:
    {
      printf("\r\nFreq = %s\r\n",msg);
    }
    break;
  case MSG_IMEI:
    {
      printf("\r\nIMEI = %s\r\n",msg);
    }
    break;
  case MSG_UDP_CREATE:
    {
      printf("\r\nUDP_CR = %s\r\n",msg);
    }
    break;
  case MSG_UDP_CLOSE:
    {
      printf("\r\nUDP_CL = %s\r\n",msg);
    }
    break;
  case MSG_UDP_SEND:
    {
      printf("\r\nUDP_SEND = %s\r\n",msg);
    }
    break;
  case MSG_UDP_RECE:
    {
      printf("\r\nUDP_RECE = %s\r\n",msg);
    }
    break;
	case MSG_TCP_CREATE:
    {
      printf("\r\nTCP_CR = %s\r\n",msg);
			APP_STATE = NB_TCP_CNT;
    }
    break;
  case MSG_TCP_CLOSE:
    {
      printf("\r\nTCP_CL = %s\r\n",msg);
    }
    break;
		
	case MSG_TCP_CONNECT:
    {
      printf("\r\nTCP_CONNECT = %s\r\n",msg);
			APP_STATE = NB_TCP_ST;
    }
    break;	
		
  case MSG_TCP_SEND:
    {
      printf("\r\nTCP_SEND = %s\r\n",msg);
    }
    break;
  case MSG_TCP_RECE:
    {
      printf("\r\nTCP_RECE = %s\r\n",msg);
			APP_STATE = NB_TCP_CL;
    }
    break;
  case MSG_COAP:
    {
      printf("\r\nCOAP = %s\r\n",msg);
			APP_STATE = NB_CoAP_ST;
    }
    break;
  case MSG_COAP_SEND:
    {
			m_coapSendTimes++;
      printf("\r\nCOAP_SENT = %s ,times = %d\r\n",msg,m_coapSendTimes);
    }
    break;
    
  case MSG_COAP_RECE:
    {
      printf("\r\nCOAP_RECE = %s\r\n",msg);
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
>>>>>>> dev
