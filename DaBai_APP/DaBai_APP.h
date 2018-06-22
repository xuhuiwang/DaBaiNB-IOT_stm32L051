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


#define KEY1		  0
#define KEY2		  1
#define KEY3			2

typedef enum {NO = 0, YES = !NO} Yes_No_Status;

typedef enum
{
  NB_NONE,
  NB_INIT,          // 初始化操作
  NB_MODULE,        // 获取NB模块厂商及固件，频段等信息
  NB_SIGN,
  NB_UDP_CR,        // 创建UDP
  NB_UDP_CL,        // 关闭UDP
  NB_UDP_ST,        // 利用已经创建的UDP发送数据
  NB_UDP_RE,        // UDP接收信息
  NB_TCP_CR,        // 创建TCP
  NB_TCP_CL,        // 关闭TCP
	NB_TCP_CNT,				// Connect TCP
  NB_TCP_ST,        // 利用已经创建的TCP发送数据
  NB_TCP_RE,        // TCP接收信息
  NB_CoAP_SEVER,    // CoAP远程地址设置与获取
  NB_CoAP_ST,       // 利用CoAP发送消息
  NB_CoAP_RE,       // CoAP返回信息
  NB_RESET,         // 复位NB
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

extern uint32_t m_fullBatTimeCnt;//记录充电时达到100%以上的时间
extern uint8_t g_BatVoltage;
extern uint16_t g_BeepFreq;

extern uint16_t g_lightValue;
extern float    g_Sht20Temp;
extern float    g_Sht20RH;
extern float g_longitude;//经度
extern float g_latitude;//纬度

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
