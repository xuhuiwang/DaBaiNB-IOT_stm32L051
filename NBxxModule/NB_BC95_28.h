//******************************************************************************
//                                https://dabairobot.taobao.com
//
//                 Copyright (c) 2017-2018, DaBai_IOT CLUB
//                                All rights reserved.
//
//  FileName : NB_BC95_28.h
//  Date     : 2018-06-04
//  Version  : V0001
// 历史记录  : 第一次创建
//******************************************************************************

#ifndef NB_BC95_28_H
#define NB_BC95_28_H

#ifdef __cplusplus
extern "C"
{
#endif
 
#include <stdint.h>
#include "NB_Board.h"
  
//******************************************************************************
//BC95系列模块，主要是基于 UART与用户进行交互。下面所有关于BC95\28的操作都是围绕
//UART。
//
  
  
/*
 * cmd 属性枚举
 */
typedef enum
{
	CMD_TEST,         //命令TEST操作
	CMD_READ,         //命令READ操作
	CMD_SET,          //命令SET操作
	CMD_EXCUTE        //命令EXCUTE操作
}cmd_property_t;

/*
 * AT指令动作行为枚举
 */
typedef enum
{
  ACTION_OK_EXIT_ERROR_NEXT,     //成功执行后退出，错误将继续执行下一条指令
  ACTION_OK_NEXT_ERROR_TRY       //成功执行后继续执行下一条指令，出错后进行尝试
                                 //。如果达到最大尝试次数后，仍没有成功，将退出
}cmd_action_t;

//AT指令结构类型
typedef struct
{
	const char*     pCMD;          //  AT指令
	cmd_property_t  property;      // 指令当前属性(TEST,READ,SET,EXCUTE)
	char*           pArgument;     // 指令参数
  char*           pExpectRes;    // 期望得到回复
	unsigned char   cmd_try;       // 出错尝试次数
  unsigned char   haveTried;     // 已经出次尝试的次数
  cmd_action_t    cmd_action;    // AT指令行为
  uint16_t        max_timeout;   // 最大超时时间
  uint8_t         lmt_period;    // 重复执行限定时间间隔
}cmd_info_t;

//声明cmd结构指针类型
typedef cmd_info_t* CmdHandle;

//******************************************************************************
// Name: process_e
//
// brief :表示当前要对bc95进行的操作
typedef enum
{
  PROCESS_NONE,     //表示bc95\28模块当前操作为空闲，可以进行下一行为操作
  PROCESS_INIT = MSG_INIT,
  PROCESS_MODULE_INFO = MSG_MODULE_INFO,
  PROCESS_SIGN = MSG_SIGN,
  PROCESS_NET_REG = MSG_REG,
  PROCESS_UDP_CR = MSG_UDP_CREATE,
  PROCESS_UDP_CL = MSG_UDP_CLOSE,
  PROCESS_UDP_ST = MSG_UDP_SEND,
  PROCESS_UDP_RE = MSG_UDP_RECE,
	
	PROCESS_TCP_CR = MSG_TCP_CREATE,
  PROCESS_TCP_CL = MSG_TCP_CLOSE,
	PROCESS_TCP_CNT = MSG_TCP_CONNECT,
  PROCESS_TCP_ST = MSG_TCP_SEND,
  PROCESS_TCP_RE = MSG_TCP_RECE,
	
  PROCESS_COAP  = MSG_COAP,
  PROCESS_COAP_ST = MSG_COAP_SEND,
  PROCESS_COAP_RE = MSG_COAP_RECE
}process_t;

typedef enum
{
  TYPES_CIMI = MSG_IMSI,
  TYPES_CGSN = MSG_IMEI,
  TYPES_CGMI = MSG_MID,
  TYPES_CGMM = MSG_MMODEL,
  TYPES_CGMR = MSG_MREV,
  TYPES_NBAND = MSG_BAND,
  TYPES_UDP_CR = MSG_UDP_CREATE,
  TYPES_UDP_CL = MSG_UDP_CLOSE,
  TYPES_UDP_SEND = MSG_UDP_SEND,
  TYPES_UDP_RECE = MSG_UDP_RECE,
	TYPES_TCP_CR = MSG_TCP_CREATE,
  TYPES_TCP_CL = MSG_TCP_CLOSE,
	TYPES_TCP_CNT = MSG_TCP_CONNECT,
  TYPES_TCP_SEND = MSG_TCP_SEND,
  TYPES_TCP_RECE = MSG_TCP_RECE
}report_msgType_e;

typedef struct
{
  process_t    state;
  int          sub_state;
}module_state;


//声明处理UART接收回调
typedef void (*bc95_receive_cb)(char*, uint16_t);
//==============================================================================

/*
 * 定义串口操作相关函数指针
 */
typedef void (*com_open)(bc95_receive_cb, uint32_t);
typedef void (*com_send)(uint8_t*,uint16_t);
typedef void (*com_close)(void);

typedef struct
{
  com_open  openFxn;
  com_send  sendFxn;
  com_close closeFxn;
}com_fxnTable;

typedef void (*bc95_timeout_cb)(void);

//==============================================================================
typedef void (*timer_init)(bc95_timeout_cb);
typedef void (*timer_start)(uint32_t);
typedef void (*timer_stop)(void);

typedef struct
{
  timer_init   initTimerFxn;
  timer_start  startTimerFxn;
  timer_stop   stopTimerFxn;
}time_fxnTable;


//bc95\28对象结构说明
typedef struct
{
  const uint32     baudrate;
  com_fxnTable     *uart_fxnTable;
  
  time_fxnTable    *timer_fxnTable;
}hw_object_t;

typedef hw_object_t* HWAttrs_Handle;

//==============================================================================

/*
 * 相关操作函数声明
 */
   
//******************************************************************************
// fn : bc95_open
//
// brief : 打开bc95使用的uart串口硬件
//
// param : handle ->nb对象指针
//
// return : none
extern int bc95_open(NB_Handle handle);

//******************************************************************************
// fn : bc95_init
//
// brief : 初始化BC95模块，并进行网络初始化
//
// param : handle ->nb对象指针
//
// return : none
extern int bc95_init(NB_Handle handle);

//******************************************************************************
// fn : bc95_moduleInfo
//
// brief : 获取NB模块的信息。比如：Manufacturer ID, Device Module,
//         Firmware Version,Freqenucy band
//
// param : handle ->nb对象指针
//
// return : none
extern int bc95_moduleInfo(NB_Handle handle);

//******************************************************************************
// fn : bc95_register
//
// brief : 查询NB模块是否注册入网
//
// param : handle ->nb对象指针
//
// return : none
extern int bc95_register(NB_Handle handle);

//******************************************************************************
// fn : bc95_getIMSI
//
// brief : 读取模块IMSI号
//
// param : handle ->nb对象指针
//
// return : none
extern const char* bc95_getIMSI(NB_Handle handle);

//******************************************************************************
// fn : bc95_getSignal
//
// brief : 读取当前信号强度
//
// param : handle ->nb对象指针
//
// return : none
extern int bc95_getSignal(NB_Handle handle);
//******************************************************************************
// fn : bc95_createUDP
//
// brief : 创建UDP
//
// param : handle ->nb对象指针
//
// return : none
extern int bc95_createUDP(NB_Handle handle);
//******************************************************************************
// fn : bc95_closeUDP
//
// brief : 关闭当前创建UDP
//
// param : handle ->nb对象指针
//
// return : none
extern int bc95_closeUDP(NB_Handle handle);

//******************************************************************************
// fn : bc95_sendUDP
//
// brief : 发送UDP数据
//
// param : handle -> NB 结构信息指针
//         len -> 信息长度
//         msg -> 信息
//
// return : none
extern int bc95_sendUDP(NB_Handle handle,int len,char* msg);
//******************************************************************************
// fn : bc95_coapServer
//
// brief : 查询与设置bc95COAP服务端信息
//
// param : handle -> NB 结构信息指针
//         isSet  -> true -> write,
//                   false-> read
//
// return : sucess or fail

//******************************************************************************
// fn : bc95_createTCP
//
// brief : 创建TCP
//
// param : handle ->nb对象指针
//
// return : none
extern int bc95_createTCP(NB_Handle handle);
//******************************************************************************
// fn : bc95_closeTCP
//
// brief : 关闭当前创建TCP
//
// param : handle ->nb对象指针
//
// return : none
extern int bc95_closeTCP(NB_Handle handle);

//******************************************************************************
// fn : bc95_connectTCP
//
// brief : 连接TCP
//
// param : handle -> NB 结构信息指针
//
// return : none
extern int bc95_connectTCP(NB_Handle handle);

//******************************************************************************
// fn : bc95_sendTCP
//
// brief : 发送TCP数据
//
// param : handle -> NB 结构信息指针
//         len -> 信息长度
//         msg -> 信息
//
// return : none
extern int bc95_sendTCP(NB_Handle handle,int len,char* msg);

//******************************************************************************
// fn : bc95_coapServer
//
// brief : 查询与设置bc95COAP服务端信息
//
// param : handle -> NB 结构信息指针
//         isSet  -> true -> write,
//                   false-> read
//
// return : sucess or fail


extern int bc95_coapServer(NB_Handle handle,Bool isSet,char* coap);

//******************************************************************************
// fn : bc95_coapSentIndication
//
// brief : 设置CoAP发送信息后结果指示方式
//
// param : handle -> NB 结构信息指针
//         code   -> 0  不作任何反应
//                   !0 响应信息
//
// return : none
extern int bc95_coapSentIndication(NB_Handle handle,int code);

//******************************************************************************
// fn : bc95_coapReceIndication
//
// brief : 设置CoAP接收信息提示方式
//
// param : handle -> NB 结构信息指针
//         code   -> 0  code -> 0表示缓存，1 表示直接接收。（目前仅支持两个模式）
//
// return : none
extern int bc95_coapReceIndication(NB_Handle handle,int code);

//******************************************************************************
// fn : bc95_coapSendMsg
//
// brief : 利用COAP发送信息到COAP服务器
//
// param : handle -> NB 结构信息指针
//         len    -> 信息长度
//         msg    -> 信息内容
//
// return : none
extern int bc95_coapSendMsg(NB_Handle handle,int len,char*msg);

//******************************************************************************
// fn : bc95_main
//
// brief : 处理BC95所有事务
//
// param : none
//
// return : FALSE -> 操作失败，TRUE -> 操作成功
extern int bc95_main(NB_Handle handle);
#ifdef __cplusplus
}
#endif
#endif   //NB_BC95_28_H
