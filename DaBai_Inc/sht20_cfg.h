//******************************************************************************
//                                www.ghostyu.com
//
//                 Copyright (c) 2017-2018, WUXI Ghostyu Co.,Ltd.
//                                All rights reserved.
//
//  FileName : sht20_cfg.h
//  Date     : 2017-12-23 13:48
//  Version  : V0001
// 历史记录  : 1.第一次创建
//
// 说明：
// sht20 iic 方式通信驱动实现
//******************************************************************************
#ifndef __SHT20_CFG_H
#define __SHT20_CFG_H

#ifdef __cplusplus
extern "C"
{
#endif
#include "DaBai_i2c.h"
  
//******************************************************************************
// fn : SHT20_ReadTemp
//
// brief : read tempture from sht20 with iic
//
// param : none
//
// return : none
extern uint16_t SHT20_ReadTemp(void);

//******************************************************************************
// fn : SHT20_ReadRH
//
// brief : read RH from sht20 with iic
//
// param : none
//
// return : none
extern uint16_t SHT20_ReadRH(void);

//******************************************************************************
// fn : SHT20_SoftReset
//
// brief : softreset sht20
//
// param : none
//
// return : none
extern void SHT20_SoftReset(void);

//******************************************************************************
// fn : SHT20_Convert
//
// brief : 将原始的AD值转成相应的温度值与湿度值
//
// param : value-> ad值
//         isTemp-> >0 温度值，=0 湿度值
//
// return : 直观的温度值
extern float SHT20_Convert(uint16_t value,uint8_t isTemp);
  

#ifdef __cplusplus
}
#endif
#endif   //__SHT20_CFG_H
