/**
  ******************************************************************************
  * File Name          : DABAI_APP.c
  * Description        : This file provides code for the configuration
  *                      of the DABAI_APP instances.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "DaBai_APP.h"
#include "sht20_cfg.h"
#include "NB_Board.h"
#include "main.h"
#include "DaBai_i2c.h"
#include "DaBai_usart.h"
#include "DaBai_ADC.h"
#include "DaBai_tim.h"
#include "DaBai_APP.h"
#include "DaBai_GPIO.h"
#include "NB_Board_Cfg.h"
#include "timer_user_poll.h"
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

uint32_t m_fullBatTimeCnt = 0;//��¼���ʱ�ﵽ100%���ϵ�ʱ��

uint8_t  g_power_off_flag = 0;

uint8_t g_BatVoltage = 0;
uint16_t g_lightValue = 0;
float    g_Sht20Temp = 0;
float    g_Sht20RH = 0;
uint16_t g_BeepFreq = 0;
float    g_longitude = 0;//����
float    g_latitude = 0;//γ��


uint8_t g_BatVoltageLow = 0;//��������10%��־λ
uint8_t m_fullBatFlag = 0;
uint8_t m_fullBatHintCnt = 0;
uint32_t m_coapSendTimes = 0;
uint8_t g_enterStandbyFlag = 0;

volatile NB_STATE_e  APP_STATE= NB_NONE;


void sysStandbyModeConfig(void)
{
	SetBeepFreq(2500);
	HAL_Delay(100);
	SetBeepFreq(0);
	LED1_OFF;
	//LED2_OFF;
	LED3_OFF;
	LED4_OFF;
	CHG_LED5_OFF;
	POWER_ON;
	CHARGE_ON;
}

void sysWakeUpConfig(void)
{
	HAL_Init();
  SystemClock_Config();
	MX_TIM_Init();
	MX_GPIO_Init();
	MX_I2C2_Init();
	MX_ADC_Init();
	MX_USART1_UART_Init();
	MX_LPUART1_UART_Init();
	standbyInitConfig();

	SetBeepFreq(0);
	
	//NBModule_open(&nb_config);
}

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

brief : ��������ת����
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

brief : ������������
param : none

return : none

*************************************/

void KeyTask(void)
{
	uint8_t i = 0;
	m_key_State[KEY1] = 0,m_key_State[KEY2] = 0,m_key_State[KEY3] = 0;
	
	/*��ȡ����״̬*/
	if(HAL_GPIO_ReadPin(GPIOA, KEY1_PIN) == GPIO_PIN_SET)
		m_key[KEY1] = GPIO_PIN_RESET;
	else
		m_key[KEY1] = GPIO_PIN_SET;
	
	m_key[KEY2] = HAL_GPIO_ReadPin(GPIOB, KEY2_PIN);
	m_key[KEY3] = HAL_GPIO_ReadPin(GPIOB, KEY3_PIN);
		
	for(i = 0; i < 3; i++)//�ֱ��3��������״̬���д���
	{
		m_key_flag[i] <<= 1;
		if( m_key[i] == GPIO_PIN_RESET )//����������
		{
			m_key_flag[i] |= 0x01;
		}
		
		if( m_key_flag[i] == 0xff )//�����������������ȶ����£���ʱm_key_flag����0xff,ֱ�������ɿ�
		{
			m_key_counter[i]++;//��¼���������±�־����ֵԽ�󱻰���ʱ��Խ��
			if(m_key_counter[i] == 0) m_key_counter[i] = 1;//����ʱ����������ڱ�����
			switch(i)
			{
				case KEY1:
				{ 
					m_key_State[KEY1] = 1;
					if(m_key_counter[KEY1] > 100)//����key1����1s����
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
				}break;

				case KEY3:
				{
					m_key_State[KEY3] = 1;
				}break;

				default:
				{}break;
			}
		}
		else if(m_key_flag[i] == 0x00)//��������
		{	
			if( m_key_counter[i] >= 1 )//�൱�����ּ��
			{
				switch(i)
				{
					case KEY1:
					{ 
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
					}break;

					case KEY3:
					{
						printf("key3 pressed\r\n");
						//APP_STATE = NB_TCP_CR;//NB_TCP_ST;
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

brief : ��س������
1������ʱ��LED5����ָʾ�ƻ�1s��˸һ�Σ��������LED5����
param : none

return : none

*************************************/

void ChargeTask(void)
{
	uint8_t i;
	
	m_chargeState	= HAL_GPIO_ReadPin(GPIOA, CHG_DETECT_PIN);//USB�ӿڲ�����

	if( m_chargeState == GPIO_PIN_SET )////USB����
	{
		m_charge_counter++;
		if(m_charge_counter == 0) m_charge_counter = 1;//��ֹ���ʱ����������ڼӵ����ֵ������
		
		g_USB_insert = YES;//USB�ӿڲ���
  	g_chargeing_flag = YES;//�ڸ���س��
	}
	else
	{
		if( m_charge_counter >= 1 )//USB�γ����
		{ 
			m_charge_counter = 0;
			g_USB_insert = NO;
			g_chargeing_flag = NO;
			m_fullBatFlag = 0;
			m_fullBatHintCnt = 0;
		}
	}

	if(g_USB_insert == YES)//USB����
	{
		if(g_chargeing_flag == YES)//����ڳ��
		{		
			if(m_fullBatTimeCnt < 1200000)//20����
			{
				HAL_GPIO_TogglePin(GPIOB,CHG_LED5_PIN);//�ڳ��ʱ1s��˸һ��
			}	
			else//��ص����ﵽ100%���20���ӣ����һ�ᣬ�õ����ﵽ���
			{
				g_chargeing_flag = NO;
				m_fullBatFlag = 1;
			}

		}
		if(m_fullBatFlag == 1)//��س�����
		{
			CHG_LED5_ON;//�����磬һֱ��

			if(m_fullBatHintCnt <  10)//���������������5����ʾ
			{	
				m_fullBatHintCnt++;
				BeepToggle(BEEP_FREQ_1000Hz);
			}
		}
	}

}

/*************************************
fn : BatManageTask

brief : ��ع�������
1������ʱ��LED5����ָʾ�ƻ�1s��˸һ�Σ��������LED5����
2���ŵ��ʱ�� ��
		     ��ص��� >= 10% �� LED5Ϩ��
	 7% <= ��ص��� < 10%  �� LED5��5HzƵ����˸�����ҷ�����5HzƵ�ʱ���
		     ��ص��� < 7%   �� ��������1s���Զ��ػ�
param : none

return : none

*************************************/

void BatManageTask(void)
{
	uint16_t start_tick = 0;
	g_BatVoltageLow = 0;
	
	ChargeTask();
	
	g_BatVoltage = GetBatVoltage();
	
	if(g_USB_insert == NO && g_BatVoltage >= 10)//��ص�������10%
	{
		CHG_LED5_OFF;
	}	
	else if(g_USB_insert == NO && g_BatVoltage >= 7)//7%<=��ص���<10%
	{
		g_BatVoltageLow = 1;//�����ͱ�־λ
	}
	else if(g_USB_insert == NO && g_BatVoltage < 7)//��ص���<7%
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
	if(g_lightValue > LIGHT_THRESHOLD || g_Sht20Temp > TEMP_THRESHOLD || g_Sht20RH > RH_THRESHOLD)//��������ֵ�����趨����ֵ
	{
		//g_BeepFreq = BEEP_FREQ_1500Hz;
		//BeepToggle(BEEP_FREQ_2500Hz);		
	}
	g_Sht20Temp = SHT20_Convert(SHT20_ReadTemp(),1);
	g_Sht20RH   = SHT20_Convert(SHT20_ReadRH(),0);
}


/*************************************
fn : DaBai_10msTask

brief : 10ms����
param : none

return : none

*************************************/
void DaBai_10msTask(void)
{
	KeyTask();
}
/*************************************
fn : DaBai_100msTask

brief : 100ms����
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
	if(g_enterStandbyFlag == 1)
	{
		HAL_PWR_EnterSTANDBYMode();
	}
}

/*************************************
fn : DaBai_500msTask

brief : 500ms����
param : none

return : none

*************************************/

void DaBai_500msTask(void)
{
	HAL_GPIO_TogglePin(GPIOB,LED1_PIN);
	DaBaiSensorTask();
	BatManageTask();//��ع�����������رմ˺����������ز��׹����޷��鿴���״̬
}

/*************************************
fn : DaBai_1000msTask

brief : 1000ms����
param : none

return : none

*************************************/

void DaBai_1000msTask(void)
{

}

/*************************************
fn : DaBai_2000msTask

brief : 2000ms����
param : none

return : none

*************************************/

void DaBai_2000msTask(void)
{

}

/*************************************
fn : DaBai_1MinTask

brief : 60s����
param : none

return : none

*************************************/
void DaBai_1MinTask(void)
{
	APP_STATE = NB_CoAP_ST;
}

/*************************************
fn : DaBai_10MinTask

brief : 60s����
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
// brief : NBģ����Ϣ�ϱ��ص�
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
			APP_STATE = NB_CoAP_SEVER;
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
			if( msg[0] == 'S')
			{
				LED2_OFF;
				//HAL_GPIO_TogglePin(GPIOB,LED2_PIN);
			}
			m_coapSendTimes++;
      printf("\r\nCOAP_SENT = %s ,times = %d\r\n",msg,m_coapSendTimes);

#ifdef STANDBY_MODE
			sysStandbyModeConfig();
			g_enterStandbyFlag = 1;
			HAL_PWR_EnterSTANDBYMode();
#endif			
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
