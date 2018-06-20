//******************************************************************************
//                                https://dabairobot.taobao.com
//
//                 Copyright (c) 2017-2018, DaBai_IOT CLUB
//                                All rights reserved.
//
//  FileName : _dma_cfg.h
//  Date     : 2018-06-04
//  Version  : V0001
// 历史记录  : 第一次创建
//******************************************************************************
#ifndef _DMA_CFG_H
#define _DMA_CFG_H

#ifdef __cplusplus
extern "C"
{
#endif
  
#include "stm32l0xx_hal.h"
  
//******************************************************************************
// 为节省内存使用，这里使用了小内存。用户可以根据自己的需求进行更改
//
#define   RECE_BUF_MAX_LEN       256
#define   SENT_BUF_MAX_LEN       128
  
/* UART Events */
#define HAL_UART_RX_FULL         0x01
#define HAL_UART_RX_TIMEOUT      0x02

//******************************************************************************
// Name : dmaReceCfg
//
// brief : 定义串口以DMA形式接收时，配置结构。
typedef struct dmaReceCfg
{
  uint16_t  buf[RECE_BUF_MAX_LEN];// 缓存区
  uint16_t  rxHead;               // 有效数据开始头
  uint16_t  rxTail;               // 最后一次数据未尾
  uint8_t   rxTick;               // 帧超时时间，单位是ms
  uint32_t  rxShdw;               // 超时记数开始数值
  
  uint8_t   txBuf[2][SENT_BUF_MAX_LEN];
  uint8_t   txIdx[2];
  uint8_t   txSel;
  
  uint8_t   txTick;
  volatile uint32_t txShdw;
  volatile uint8_t  txShdwValid; // TX shadow value is valid
  uint8_t   txDMAPending;     // UART TX DMA is pending
  
}dmaCfg_t;


//******************************************************************************
//函数声明区域
//
typedef void (*sendData_cb)(uint8_t* buf,uint16_t len); 

//******************************************************************************
// fn : UartDma_Init
//
// brief : 初始化dmaReceCfg的结构
//
// param : none
//
// return : the address of rx Buffer
extern uint8_t* UartDma_Init(sendData_cb,USART_TypeDef*);

//******************************************************************************
// fn :     UartDma_Read
//
// brief :  从接收缓存里读取指定长度串口数据，并释放占用的空间
//
// param :  buf  - 有效长度的目标缓存
//          len  - 要读取的长度
//
// return : length of buffer that was read
//******************************************************************************
extern uint16_t UartDma_Read(uint8_t *buf, uint16_t len);

//******************************************************************************
// fn :     UartDma_Write
//
// brief :  将要发送的数据，拷贝到发送缓存中
//
// param :  buf  - 有效长度的目标缓存
//          len  - 要读取的长度
//
// return : length of buffer that was write
//******************************************************************************
extern uint16_t UartDma_Write(uint8_t *buf, uint16_t len);
//**************************************************************************************************
// fn  :  UartDma_Avail
//
// brief :  计算缓存区中数据个数 - the number of bytes in the buffer.
//
// param :  none
//
// return : the number of bytes
//**************************************************************************************************/
extern uint16_t UartDma_Avail(void);

//******************************************************************************
// fn : UartDma_Poll
//
// brief : 轮询接收缓存数据长度
//
// param : none
//
// return : none
//****************************************************************************/
extern uint8_t UartDma_Poll(void);
#ifdef __cplusplus
}
#endif

#endif   //_DMA_RECE_CFG_H
