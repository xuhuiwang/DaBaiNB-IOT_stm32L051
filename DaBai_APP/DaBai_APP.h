/**
  ******************************************************************************
  * File Name          : DABAI_APP.h
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DABAI_APP_H
#define __DABAI_APP_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l0xx_hal.h"
#include "sht20_cfg.h"

//#define  STANDBY_MODE

#define KEY1		  0
#define KEY2		  1
#define KEY3			2

typedef enum {NO = 0, YES = !NO} Yes_No_Status;

typedef enum
{
  NB_NONE,
  NB_INIT,          // ��ʼ������
  NB_MODULE,        // ��ȡNBģ�鳧�̼��̼���Ƶ�ε���Ϣ
  NB_SIGN,
  NB_UDP_CR,        // ����UDP
  NB_UDP_CL,        // �ر�UDP
  NB_UDP_ST,        // �����Ѿ�������UDP��������
  NB_UDP_RE,        // UDP������Ϣ
  NB_TCP_CR,        // ����TCP
  NB_TCP_CL,        // �ر�TCP
	NB_TCP_CNT,				// Connect TCP
  NB_TCP_ST,        // �����Ѿ�������TCP��������
  NB_TCP_RE,        // TCP������Ϣ
  NB_CoAP_SEVER,    // CoAPԶ�̵�ַ�������ȡ
  NB_CoAP_ST,       // ����CoAP������Ϣ
  NB_CoAP_RE,       // CoAP������Ϣ
  NB_RESET,         // ��λNB
  NB_END
}NB_STATE_e;

#define BEEP_FREQ_1000Hz  1000
#define BEEP_FREQ_1500Hz  1500
#define BEEP_FREQ_2000Hz  2000
#define BEEP_FREQ_2500Hz  2500

#define LIGHT_THRESHOLD   2000
#define TEMP_THRESHOLD    40
#define RH_THRESHOLD   		80

extern volatile NB_STATE_e  APP_STATE;

extern volatile Yes_No_Status g_USB_insert;
extern volatile Yes_No_Status g_chargeing_flag;

extern uint32_t m_fullBatTimeCnt;//��¼���ʱ�ﵽ100%���ϵ�ʱ��
extern uint8_t g_BatVoltage;
extern uint16_t g_BeepFreq;

extern uint16_t g_lightValue;
extern float    g_Sht20Temp;
extern float    g_Sht20RH;
extern float g_longitude;//����
extern float g_latitude;//γ��
extern uint8_t g_enterStandbyFlag;
void sysWakeUpConfig(void);
void Fill_u16_To_u8(uint16_t x, char* h, char* l);
void Fill_int16_To_int8(int16_t x, char* h, char* l);
void Fill_u32_To_u8(uint32_t x, char* hh, char* hl, char* lh, char* ll);

void DaBai_10msTask(void);
void DaBai_100msTask(void);
void DaBai_500msTask(void);
void DaBai_1000msTask(void);
void DaBai_1MinTask(void);
void DaBai_10MinTask(void);
#ifdef __cplusplus
}
#endif
#endif /*__ DABAI_APP_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
