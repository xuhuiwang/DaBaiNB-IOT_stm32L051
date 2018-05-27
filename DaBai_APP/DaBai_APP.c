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
			switch(i)
			{
				case KEY1:
				{ 
					key_State[KEY1] = 1;
				}break;
				case KEY2:
				{
					key_State[KEY2] = 1;
				}break;

				case KEY3:
				{
					key_State[KEY3] = 1;
				}break;

				default:
				{}break;
			}
		}
		else if(key_flag[i] == 0x00)//松手消斗
		{	
			if( key_counter[i] >= 1 )//相当于松手检测
			{
				switch(i)
				{
					case KEY1:
					{ 

					}break;
					case KEY2:
					{

					}break;

					case KEY3:
					{

					}break;
					
					default:
					{}break;
				}	
				
				key_counter[i] = 0;
			}
		}
	}
	
	if(key_State[KEY2] == 1 || key_State[KEY3] == 1)
	{
		//HAL_TIM_PWM_Start(&TimHandle, TIM_CHANNEL_2);
		//HAL_GPIO_WritePin(GPIOB, LED5, GPIO_PIN_RESET);
	}
	else
	{
		//HAL_TIM_PWM_Stop(&TimHandle, TIM_CHANNEL_2);
		//HAL_GPIO_WritePin(GPIOB, LED5, GPIO_PIN_SET);
	}

}




/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
