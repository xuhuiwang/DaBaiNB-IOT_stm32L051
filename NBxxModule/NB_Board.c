//******************************************************************************
//                                www.ghostyu.com
//
//                 Copyright (c) 2017-2018, WUXI Ghostyu Co.,Ltd.
//                                All rights reserved.
//
//  FileName : NB_Board.c
//  Date     : 2017-12-11 11:18
//  Version  : V0001
// 历史记录  : 第一次创建
//******************************************************************************
#include "NB_Board.h"


//==============================================================================
extern const NB_FxnTable BC95_FxnTable;

//函数定义
//******************************************************************************
// fn : NBModule_open
//
// brief : 初始化与之相关的硬件
//
// param : none
//
// return : none
extern Bool NBModule_open(NB_Handle handle)
{
  if(handle == null)
  {
    return FALSE;
  }
  
  if(handle->fxnTablePtr->openFxn == null)
  {
    return FALSE;
  }
  handle->fxnTablePtr = (void*)&BC95_FxnTable;
  //打相应的硬件
  handle->fxnTablePtr->openFxn(handle);
  
  return TRUE;
}
//******************************************************************************
// fn : NBModule_Init
//
// brief : 初始化NB模块网络及获取一些信息
//
// param : none
//
// return : none
extern Bool NBModule_Init(NB_Handle handle)
{
  if(handle == null)
  {
    return FALSE;
  }
  if(handle->fxnTablePtr->nbModuleInit == null)
  {
    return FALSE;
  }
  
  //调用
  handle->fxnTablePtr->nbModuleInit(handle);
  return TRUE;
}
//******************************************************************************
// fn : NB_ModuleInfo
//
// brief : 获取NB模块的信息。比如：Manufacturer ID, Device Module,
//         Firmware Version,Freqenucy band
//
// param : none
//
// return : none
uint8 NBModule_Info(NB_Handle handle)
{
  if(handle == null)
  {
    return FALSE;
  }
  if(handle->fxnTablePtr->getModuleInfo == null)
  {
    return FALSE;
  }
  
  handle->fxnTablePtr->getModuleInfo(handle);
  
  return TRUE;
}
//******************************************************************************
// fn : NBModule_isRegister
//
// brief : 查询NB模块是否注册入网
//
// param : none
//
// return : none
uint8 NBModule_isRegister(NB_Handle handle)
{
  if(handle == null)
  {
    return FALSE;
  }
  if(handle->fxnTablePtr->getModuleRegisterInfo == null)
  {
    return FALSE;
  }
  
  return handle->fxnTablePtr->getModuleRegisterInfo(handle);

}
//******************************************************************************
// fn : NBModule_IMSI
//
// brief : 读取模块IMSI号
//
// param : none
//
// return : none
const char* NBModule_IMSI(NB_Handle handle)
{
  if(handle == null)
  {
    return FALSE;
  }
  if(handle->fxnTablePtr->getUSIMinfo == null)
  {
    return FALSE;
  }
  return handle->fxnTablePtr->getUSIMinfo(handle);
}

//******************************************************************************
// fn : NBModule_Sign
//
// brief : 获取当前信号值
//
// param : none
//
// return : TRUE-> 指令已执行，FALSE->执行过程中发生错误
// 注意：
// 具本的信号值，会通过消息异步到回调函数中
extern uint8 NBModule_Sign(NB_Handle handle)
{
  if(handle == null)
  {
    return FALSE;
  }
  if(handle->fxnTablePtr->getSign == null)
  {
    return FALSE;
  }
  return handle->fxnTablePtr->getSign(handle);
}
//******************************************************************************
// fn : NBModule_CreateUDP
//
// brief : 创建UDP
//
// param : none
//
// return : none
uint8 NBModule_CreateUDP(NB_Handle handle)
{
  if(handle == null)
  {
    return FALSE;
  }
  if(handle->fxnTablePtr->createUdp == null)
  {
    return FALSE;
  }
  
  return handle->fxnTablePtr->createUdp(handle);
}
//******************************************************************************
// fn : NBModule_CloseUDP
//
// brief : 关闭指定的udp
//
// param : handle -> NB 结构信息指针
//         socketId -> 关闭的socket id号
//
// return : none
uint8 NBModule_CloseUDP(NB_Handle handle)
{
  if(handle == null)
  {
    return FALSE;
  }
  if(handle->fxnTablePtr->closeUdp == null)
  {
    return FALSE;
  }
  return handle->fxnTablePtr->closeUdp(handle);
}
//******************************************************************************
// fn : NBModule_SendData
//
// brief : 向udp发送数据
//
// param : handle -> NB 结构信息指针
//         len -> 信息长度
//         msg -> 信息
//
// return : none
uint8 NBModule_SendData(NB_Handle handle,int len,char* msg)
{
  if(handle == null)
  {
    return FALSE;
  }
  if(handle->fxnTablePtr->sendUdp == null)
  {
    return FALSE;
  }
  return handle->fxnTablePtr->sendUdp(handle,len,msg);
}

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
uint8 NBModule_CoAPServer(NB_Handle handle,Bool isSet,char*coap)
{
  if(handle == null)
  {
    return FALSE;
  }
  if(handle->fxnTablePtr->coapServer == null)
  {
    return FALSE;
  }
  return handle->fxnTablePtr->coapServer(handle,isSet,coap);
}

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
uint8 NBModule_CoAPSentIndication(NB_Handle handle,int code)
{
  if(handle == null)
  {
    return FALSE;
  }
  if(handle->fxnTablePtr->coapSentIndication == null)
  {
    return FALSE;
  }
  return handle->fxnTablePtr->coapSentIndication(handle,code);
}

//******************************************************************************
// fn : NBModule_CoAPReceIndication
//
// brief : 设置CoAP接收信息提示方式
//
// param : handle -> NB 结构信息指针
//         code   -> 0  code -> 0表示缓存，1 表示直接接收。（目前仅支持两个模式）
//
// return : none
uint8 NBModule_CoAPReceIndication(NB_Handle handle,int code)
{
  if(handle == null)
  {
    return FALSE;
  }
  if(handle->fxnTablePtr->coapSetReceMode == null)
  {
    return FALSE;
  }
  return handle->fxnTablePtr->coapSetReceMode(handle,code);
}

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
uint8 NBModule_CoAPSendMsg(NB_Handle handle,int len,char*msg)
{
  if(handle == null)
  {
    return FALSE;
  }
  if(handle->fxnTablePtr->coapSentMsg == null)
  {
    return FALSE;
  }
  return handle->fxnTablePtr->coapSentMsg(handle,len,msg);
}
//******************************************************************************
// fn : NBModule_Main
//
// brief : 循环中连续调用
//
// param : handle -> NB 结构信息指针
//
// return : none
extern int NBModule_Main(NB_Handle handle)
{
  if(handle == null)
  {
    return FALSE;
  }
  if(handle->fxnTablePtr->mainThread == null)
  {
    return FALSE;
  }
  
  //调用
  return handle->fxnTablePtr->mainThread(handle);

}
