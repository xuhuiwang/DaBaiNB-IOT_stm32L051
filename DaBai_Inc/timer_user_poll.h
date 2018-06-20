//******************************************************************************
//                                https://dabairobot.taobao.com
//
//                 Copyright (c) 2017-2018, DaBai_IOT CLUB
//                                All rights reserved.
//
//  FileName : timer_user_poll.h
//  Date     : 2018-06-04
//  Version  : V0001
// 历史记录  : 第一次创建
//******************************************************************************

#ifndef _TIMER_USER_POLL_H
#define _TIMER_USER_POLL_H

#ifdef __cplusplus
extern "C"
{
#endif
#include "stm32l0xx_hal.h"
#include "nb_bc95_28.h"
  
void MX_TIM_Set(bc95_timeout_cb cb);

void MX_TIM_Start(uint32_t ms);

void MX_TIM_Stop(void);

void MX_TimerPoll(void);

#ifdef __cplusplus
}
#endif
#endif   //_TIMER_USER_POLL_H
