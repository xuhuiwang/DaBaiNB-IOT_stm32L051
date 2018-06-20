//******************************************************************************
//                                https://dabairobot.taobao.com
//
//                 Copyright (c) 2017-2018, DaBai_IOT CLUB
//                                All rights reserved.
//
//  FileName : NB_Board_Cfg.c
//  Date     : 2018-06-04 
//  Version  : V1.0
// 历史记录  : 1.第一次创建
//
// 说明：
// NB_IOT与平台相关的配置变量定义
//******************************************************************************

#include "NB_Board_Cfg.h"
#include "NB_Board.h"
#include "NB_BC95_28.h"
#include "DaBai_usart.h"
#include "timer_user_poll.h"

extern int NB_MsgreportCb(msg_types_t,int ,char*);

com_fxnTable  com_fxn = 
{
  .openFxn = HAL_UARTDMA_Init,
  .sendFxn = HAL_UART_Write,
  .closeFxn = HAL_UART_Close
};

time_fxnTable time_fxn = 
{
  .initTimerFxn = MX_TIM_Set,
  .startTimerFxn = MX_TIM_Start,
  .stopTimerFxn = MX_TIM_Stop
};

hw_object_t  HWAtrrs_object = 
{
  .baudrate = 9600,
  .uart_fxnTable = &com_fxn,
  .timer_fxnTable = &time_fxn
};

NB_Config  nb_config = 
{
  .fxnTablePtr = NULL,
  .object = (void*)&HWAtrrs_object,
  .AppReceCB = NB_MsgreportCb,
  .log = NULL
};
