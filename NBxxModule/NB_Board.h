//******************************************************************************
//                                www.ghostyu.com
//
//                 Copyright (c) 2017-2018, WUXI Ghostyu Co.,Ltd.
//                                All rights reserved.
//
//  FileName : nb_Board.h
//  Date     : 2017-12-11 11:18
//  Version  : V0001
// 历史记录  : 第一次创建
//******************************************************************************
#ifndef NB_BOARD_H
#define NB_BOARD_H

#ifdef __cplusplus
extern "C"
{
#endif
  
#include "nb_types.h" 
//******************************************************************************
// Const define 
//******************************************************************************
  
//Error const number
 
#define SUCCESS     0

#define FAIL        1
  

//******************************************************************************
//Typedef 
//******************************************************************************

//定义接收消息类型
typedef enum
{
  MSG_NONE,
  //================
  MSG_INIT,
  MSG_IMSI,
  //================
  MSG_MODULE_INFO,
  MSG_MID,     //制造商ID
  MSG_MMODEL,  //厂商型号
  MSG_MREV,    //厂家版本号
  MSG_BAND,    //工作频段
  MSG_IMEI,    //移动设备身份码
  //===================================
  MSG_SIGN,    //信号强度
  MSG_REG,
  //===================================
  MSG_UDP_CREATE,
  MSG_UDP_CLOSE,
  MSG_UDP_SEND,
  MSG_UDP_RECE,
	
	MSG_TCP_CREATE,
  MSG_TCP_CLOSE,
	MSG_TCP_CONNECT,
  MSG_TCP_SEND,
  MSG_TCP_RECE,
	
  //
  MSG_COAP,
  MSG_COAP_SEND,
  MSG_COAP_RECE,
  
  MSG_END
}msg_types_t;

//指向指定的NBiot模块
typedef struct NB_Config *NB_Handle;


//对NB模块操作进行前期的初始化操作
typedef int (*NB_Open)(NB_Handle);

//对NB模块操作的占用的资源进行释放
typedef int (*NB_Close)(NB_Handle);

//自动发送相关初始化指令
typedef int (*NB_AutoInitModule)(NB_Handle);

//获取NB模块的信息。比如：Manufacturer ID, Device Module,Firmware Version,Freqenucy band
typedef int (*NB_ModuleInfo)(NB_Handle);

//获取NB模块的网络注册信息
typedef int (*NB_NetRegisterInfo)(NB_Handle);

//获取NB模块SIM 卡的信息
typedef const char* (*NB_USIMInfo)(NB_Handle);

//获取NB模块信号强度
typedef int (*NB_NetSign)(NB_Handle);

//定义NB模块通信上下文
typedef int (*NB_DefPDP)(NB_Handle);

//失能NB模块上下文信息
typedef int (*NB_DeactPDP)(NB_Handle);

//创建UDP
typedef int (*NB_CreateUdp)(NB_Handle);

//关闭UDP
typedef int (*NB_CloseUdp)(NB_Handle);

//发送UDP数据
typedef int (*NB_SendUdpData)(NB_Handle,int,char*);



//创建TCP
typedef int (*NB_CreateTcp)(NB_Handle);

//关闭TCP
typedef int (*NB_CloseTcp)(NB_Handle);

//连接TCP
typedef int (*NB_ConnectTcp)(NB_Handle);

//发送TCP数据
typedef int (*NB_SendTcpData)(NB_Handle,int,char*);


//CoAP 信息
typedef int (*NB_CoAPServer)(NB_Handle, Bool,char*);

//CoAP 发送提示方式设置
typedef int (*NB_CoAPSentIndication)(NB_Handle,int code);

//CoAP 接收数据方式设置
typedef int (*NB_CoAPReceMode)(NB_Handle,int);

//CoAP协议信息发送
typedef int (*NB_CoAPSentMsg)(NB_Handle,int,char*);

//NB Modle reset cmd
typedef int (*NB_Reset)(NB_Handle);

//NB ping function
typedef int (*NB_Ping)(NB_Handle);

//判断接收数据是不是Udp
//typedef int (*NB_isUdpReceData)(NB_Handle,int,char*);

//判断接收数据是否为CoAP数据
//typedef int (*NB_IsCoAPReceData)(NB_Handle,int,char*);


//接收UDP数据
//typedef int (*NB_ReceUdpData)(NB_Handle);

//CoAP协议信息接收
//typedef int (*NB_CoAPReceMsg)(NB_Handle);

typedef int (*NB_MainThread)(NB_Handle);
//******************************************************************************
// Name : NB_FxnTable
//
// brief : 定义NBIOT模块必须要实现的功能函数
//
typedef struct
{
  NB_Open                 openFxn;
  NB_AutoInitModule       nbModuleInit;
  NB_ModuleInfo           getModuleInfo;
  NB_NetRegisterInfo      getModuleRegisterInfo;
  NB_USIMInfo             getUSIMinfo;
  NB_NetSign              getSign;
//  NB_DefPDP               definePDP;
//  NB_DeactPDP             deactPDP;
  NB_CreateUdp            createUdp;
  NB_CloseUdp             closeUdp;
  NB_SendUdpData          sendUdp;
	
	NB_CreateTcp            createTcp;
  NB_CloseTcp             closeTcp;
	NB_ConnectTcp						connectTcp;
  NB_SendTcpData          sendTcp;
//  
  NB_CoAPServer           coapServer;
  NB_CoAPSentIndication   coapSentIndication;
  NB_CoAPReceMode         coapSetReceMode;
  NB_CoAPSentMsg          coapSentMsg;
//  
//  NB_Reset                nbReset;
//  NB_Ping                 nbPing;
//  
//  NB_Close                closeFxn;
  NB_MainThread           mainThread;
}NB_FxnTable;


typedef int (*NB_ReceCB)(msg_types_t,int ,char*);

typedef int (*NB_Log)(char*,int);
//******************************************************************************
// Name : NB_Config
//
// brief : NBIOT模块的功能结构数据
//
typedef struct NB_Config
{
  NB_FxnTable        *fxnTablePtr;
  void               *object;         //NB uart对像指针
  NB_ReceCB          AppReceCB;
  NB_Log             log;
}NB_Config;


//******************************************************************************
// function declare
//******************************************************************************

//******************************************************************************
// fn : NBModule_open
//
// brief : 初始化与之相关的硬件
//
// param : handle -> 用户定义的NB_Config对像指针
//
// return : none
extern uint8 NBModule_open(NB_Handle handle);

//******************************************************************************
// fn : NBModule_Init
//
// brief : 初始化NB模块网络及获取一些信息
//
// param : none
//
// return : none
extern uint8 NBModule_Init(NB_Handle handle);
//******************************************************************************
// fn : NB_ModuleInfo
//
// brief : 获取NB模块的信息。比如：Manufacturer ID, Device Module,
//         Firmware Version,Freqenucy band
//
// param : none
//
// return : none
extern uint8 NBModule_Info(NB_Handle handle);

//******************************************************************************
// fn : NBModule_isRegister
//
// brief : 查询NB模块是否注册入网
//
// param : none
//
// return : none
extern uint8 NBModule_isRegister(NB_Handle handle);
//******************************************************************************
// fn : NBModule_IMSI
//
// brief : 读取模块IMSI号
//
// param : none
//
// return : none
extern const char* NBModule_IMSI(NB_Handle handle);

//******************************************************************************
// fn : NBModule_Sign
//
// brief : 获取当前信号值
//
// param : none
//
// return : none
extern uint8 NBModule_Sign(NB_Handle handle);

//******************************************************************************
// fn : NBModule_ActPDP
//
// brief : 定义当前NB模块通信上下文
//
// param : none
//
// return : none
extern uint8 NBModule_ActPDP(NB_Handle handle);

//******************************************************************************
// fn : NBModule_DeactPDP
//
// brief : 失能模块上下文
//
// param : none
//
// return : none
extern uint8 NBModule_DeactPDP(NB_Handle handle);

//******************************************************************************
// fn : NBModule_CreateUDP
//
// brief : 创建UDP
//
// param : none
//
// return : none
extern uint8 NBModule_CreateUDP(NB_Handle handle);

//******************************************************************************
// fn : NBModule_CloseUDP
//
// brief : 关闭指定的udp
//
// param : handle -> NB 结构信息指针
//         socketId -> 关闭的socket id号
//
// return : none
extern uint8 NBModule_CloseUDP(NB_Handle handle);

//******************************************************************************
// fn : NBModule_SendUdpData
//
// brief : 向udp发送数据
//
// param : handle -> NB 结构信息指针
//         len -> 信息长度
//         msg -> 信息
//
// return : none
extern uint8 NBModule_SendUdpData(NB_Handle handle,int len,char* msg);


//******************************************************************************
// fn : NBModule_CreateTCP
//
// brief : 创建TCP
//
// param : none
//
// return : none
extern uint8 NBModule_CreateTCP(NB_Handle handle);

//******************************************************************************
// fn : NBModule_CloseTCP
//
// brief : 关闭指定的tcp
//
// param : handle -> NB 结构信息指针
//         socketId -> 关闭的socket id号
//
// return : none
extern uint8 NBModule_CloseTCP(NB_Handle handle);

//******************************************************************************
// fn : NBModule_ConnectTcp
//
// brief : 连接TCP
//
// param : handle -> NB 结构信息指针
// return : none
extern uint8 NBModule_ConnectTcp(NB_Handle handle);


//******************************************************************************
// fn : NBModule_SendUdpData
//
// brief : 向tcp发送数据
//
// param : handle -> NB 结构信息指针
//         len -> 信息长度
//         msg -> 信息
//
// return : none
extern uint8 NBModule_SendTcpData(NB_Handle handle,int len,char* msg);



//******************************************************************************
// fn : NBModule_CoAPServer
//
// brief : 设置与查询NB模块当前CoAP服务器信息
//
// param : handle -> NB 结构信息指针
//         isSet  -> true -> write,
//                   false-> read
//
// return : none
extern uint8 NBModule_CoAPServer(NB_Handle handle,Bool isSet,char*coap);

//******************************************************************************
// fn : NBModule_CoAPSentIndication
//
// brief : 设置CoAP发送信息后结果指示方式
//
// param : handle -> NB 结构信息指针
//         code   -> 0  不作任何反应
//                   !0 响应信息
//
// return : none
extern uint8 NBModule_CoAPSentIndication(NB_Handle handle,int code);

//******************************************************************************
// fn : NBModule_CoAPReceIndication
//
// brief : 设置CoAP接收信息提示方式
//
// param : handle -> NB 结构信息指针
//         code   -> 0  code -> 0表示缓存，1 表示直接接收。（目前仅支持两个模式）
//
// return : none
extern uint8 NBModule_CoAPReceIndication(NB_Handle handle,int code);

//******************************************************************************
// fn : NBModule_CoAPSendMsg
//
// brief : 利用COAP发送信息到COAP服务器
//
// param : handle -> NB 结构信息指针
//         len    -> 信息长度
//         msg    -> 信息内容
//
// return : none
extern uint8 NBModule_CoAPSendMsg(NB_Handle handle,int len,char*msg);
//******************************************************************************
// fn : NBModule_CoAPSendMsg
//
// brief : 利用COAP发送信息到COAP服务器
//
// param : handle -> NB 结构信息指针
//
// return : none
extern void NBModule_Reset(NB_Handle handle);
//******************************************************************************
// fn : NBModule_Ping
//
// brief : ping remote address
//
// param : handle -> NB 结构信息指针
//         ipAddr -> 远程ip地址.如118.29.240.46
//
// return : none
extern void NBModule_Ping(NB_Handle handle,char *ipAddr);

//******************************************************************************
// fn : NBModule_Main
//
// brief : 循环中连续调用
//
// param : handle -> NB 结构信息指针
//
// return : none
extern int NBModule_Main(NB_Handle handle);

#ifdef __cplusplus
}
#endif

#endif    //NB_BOARD_H
