//******************************************************************************
//                                https://dabairobot.taobao.com
//
//                 Copyright (c) 2017-2018, DaBai_IOT CLUB
//                                All rights reserved.
//
//  FileName : timer_user_poll.c
//  Date     : 2018-06-04
//  Version  : V1.0
// 历史记录  : 1.第一次创建
//
// 说明：
// BC95\28系列NB-IOT模块的AT指令产生及对指令响应进行解析
//******************************************************************************
#include "timer_user_poll.h"
#include "nb_bc95_28.h"

bc95_timeout_cb  nb_timeoutCb;

uint32_t  gTickStart = 0;
uint32_t  gTickDelta = 0;
uint8_t   gTimeOpenFlag = 0;

void MX_TIM_Set(bc95_timeout_cb cb)
{
  nb_timeoutCb = cb;
}

void MX_TIM_Start(uint32_t ms)
{
  gTickDelta = ms;
  gTickStart = HAL_GetTick();
  gTimeOpenFlag = 1;
}

void MX_TIM_Stop(void)
{
  gTimeOpenFlag = 0;
}


void MX_TimerPoll(void)
{
  uint32_t tick;
  if(gTimeOpenFlag)
  {
    tick = HAL_GetTick();
    uint32_t de = tick >= gTickStart ? tick - gTickStart : tick + UINT32_MAX - gTickStart;
    if(gTickDelta > de)
    {
      gTickDelta -= de;
      gTickStart = tick;
    }
    else
    {
      gTickDelta = 0;
      gTimeOpenFlag = 0;
      if(nb_timeoutCb)
      {
        nb_timeoutCb();
      }
    }
    
  }
}
