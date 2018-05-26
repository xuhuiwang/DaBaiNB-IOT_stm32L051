/**
  ******************************************************************************
  * File Name          : DABAI_APP.c
  * Description        : This file provides code for the configuration
  *                      of the DABAI_APP instances.
  ******************************************************************************
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
  */

/* Includes ------------------------------------------------------------------*/
#include "DaBai_APP.h"
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
	
	/*��ȡ����״̬*/
	key[KEY1] = HAL_GPIO_ReadPin(GPIOA, KEY1_Pin);
	key[KEY2] = HAL_GPIO_ReadPin(GPIOB, KEY2_Pin);
	key[KEY3] = HAL_GPIO_ReadPin(GPIOB, KEY3_Pin);
		
	for(i = 0; i < 3; i++)//�ֱ��3��������״̬���д���
	{
		key_flag[i] <<= 1;
		if( key[i] == GPIO_PIN_RESET )//����������
		{
			key_flag[i] |= 0x01;
		}
		
		if( key_flag[i] == 0xff )//�����������������ȶ����£���ʱkey_flag����0xff,ֱ�������ɿ�
		{
			key_counter[i]++;//��¼���������±�־����ֵԽ�󱻰���ʱ��Խ��
			if(key_counter[i] == 0) key_counter[i] = 1;//����ʱ����������ڱ�����
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
		else if(key_flag[i] == 0x00)//��������
		{	
			if( key_counter[i] >= 1 )//�൱�����ּ��
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
		//HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET);
	}
	else
	{
		//HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET);
	}

}




/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/