//******************************************************************************
//                                www.ghostyu.com
//
//                 Copyright (c) 2017-2018, WUXI Ghostyu Co.,Ltd.
//                                All rights reserved.
//
//  FileName : NB_BC95.c
//  Date     : 2017-12-04 16:11
//  Version  : V0001
// 历史记录  : 1.第一次创建
//
// 说明：
// BC95系列NB-IOT模块的AT指令产生及对指令响应进行解析
//******************************************************************************
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "NB_BC95_28.h"
#include "NB_Board.h"

// BC95-B5 AT指令语法
//--------------------|------------------------|-------------------------------------
//  Test Command        AT+<cmd>=?               Check possible sub-parameter values
//  Read Command        AT+<cmd>?                Check current sub-parameter values
//  Set Command         AT+<cmd>=p1[,p2[,p3]]]   Set command
//  Execution Command   AT+<cmd>                 Execution command
//--------------------|------------------------|-------------------------------------
  
// AT 指令响应
// Description:
// When the AT Command processor has finished processing a line,it will output either "OK" or "ERROR"
// indicating that it is ready to accept a new command.Solicited informational responses are sent before the
// final "OK" or "ERROR".Unsolicited information response will never occur between a solicited
// informational response and the final "OK" or "ERROR"

// Responses will be of the format:
// <CR><LF>+CMD1:<parameters><CR><LF>
// <CR><LF>OK<CR><LF>
// Or
// <CR><LF><parameters><CR><LF>
// <CR><LF>OK<CR><LF>
//  
  
//BC95_B5支持的AT指令说明
/* AT Command Description Implementation Status */
  
/* 3GPP Commands (27.007)*/
//------------|-----------------------------------------------------| 
// AT+CGMI    | Request Manufacturer Identification B350 or later   
//------------|-----------------------------------------------------|
// AT+CGMM    | Request Manufacturer Model B350 or later            
//------------|-----------------------------------------------------|
// AT+CGMR    | Request Manufacturer Revision B350 or later         
//------------|-----------------------------------------------------|
// AT+CGSN    | Request Product Serial Number B350 or later         
//------------|-----------------------------------------------------|
// AT+CEREG   | EPS Network Registration Status B350 or later       
//------------|-----------------------------------------------------|
// AT+CSCON   | Signalling Connection Status B350 or later
//------------|-----------------------------------------------------|
// AT+CLAC    | List Available Commands B350 or later
//------------|-----------------------------------------------------|
// AT+CSQ     | Get Signal Strength Indicator B350 or later
//------------|-----------------------------------------------------|
// AT+CGPADDR | Show PDP Addresses B350 or later
//------------|-----------------------------------------------------|
// AT+COPS    | PLMN Selection B350 or later
//------------|-----------------------------------------------------|
// AT+CGATT   | PS Attach or Detach B350 or later
//------------|-----------------------------------------------------|
// AT+CGACT   | Activate or Deactivate PDP Context B657SP1 or later
//------------|-----------------------------------------------------|
// AT+CIMI    | Request International Mobile Subscriber Identity B350 or later
//------------|-----------------------------------------------------|
// AT+CGDCONT | Define a PDP Context B350 or later
//------------|-----------------------------------------------------|
// AT+CFUN    | Set Phone Functionality B350 or later
//------------|-----------------------------------------------------|
// AT+CMEE    | Report Mobile Termination Error B600 or later
//------------|-----------------------------------------------------|
// AT+CCLK    | Return Current Date & Time B656 or later
//------------|-----------------------------------------------------|
// AT+CPSMS   | Power Saving Mode Setting B657SP1 or later
//------------|-----------------------------------------------------|
// AT+CEDRXS  | eDRX Setting B657SP1 or later
//------------|-----------------------------------------------------|
// AT+CEER    | Extended Error Report B657SP1 or later
//------------|-----------------------------------------------------|
// AT+CEDRXRDP| eDRX Read Dynamic Parameters B657SP1 or later
//------------|-----------------------------------------------------|
// AT+CTZR    | Time Zone Reporting B657SP1 or later
//------------|-----------------------------------------------------|

/*   ETSI Commands* (127.005)  <Under development> */
//------------|-----------------------------------------------------|
// AT+CSMS    | Select Message Service B657SP1 or later
//------------|-----------------------------------------------------|
// AT+CNMA    | New Message Acknowledgement to ME/TA B657SP1 or later
//------------|-----------------------------------------------------|
// AT+CSCA    | Service Centre Address B657SP1 or later
//------------|-----------------------------------------------------|
// AT+CMGS    | Send SMS Message B657SP1 or later
//------------|-----------------------------------------------------|
// AT+CMGC    | Send SMS Command B657SP1 or later
//------------|-----------------------------------------------------|
// AT+CSODCP  | Sending of Originating Data via the Control Plane B657SP1 or later
//------------|-----------------------------------------------------|
// AT+CRTDCP  | Reporting of Terminating Data via the Control Plane
//------------|-----------------------------------------------------|

/*   General Commands  */
//--------------|-----------------------------------------------------|
// AT+NMGS      | Send a Message B350 or later
//--------------|-----------------------------------------------------|
// AT+NMGR      | Get a Message B350 or later
//--------------|-----------------------------------------------------|
// AT+NNMI      | New Message Indications B350 or later
//--------------|-----------------------------------------------------|
// AT+NSMI      | Sent message Indications B350 or later
//--------------|-----------------------------------------------------|
// AT+NQMGR     | Query Messages Received B350 or later
//--------------|-----------------------------------------------------|
// AT+NQMGS     | Query Messages Sent B350 or later
//--------------|-----------------------------------------------------|
// AT+NMSTATUS  | Message Registration Status B657SP1 or later
//--------------|-----------------------------------------------------|
// AT+NRB       | Reboot B350 or later
//--------------|-----------------------------------------------------|
// AT+NCDP      | Configure and Query CDP Server Settings B350 or later
//--------------|-----------------------------------------------------|
// AT+NUESTATS  | Query UE Statistics B350 or later
//--------------|-----------------------------------------------------|
// AT+NEARFCN   | Specify Search Frequencies B350 or later
//--------------|-----------------------------------------------------|
// AT+NSOCR     | Create a Socket B350 or later
//--------------|-----------------------------------------------------|
// AT+NSOST     | SendTo Command (UDP Only) B350 or later
//--------------|-----------------------------------------------------|
// AT+NSOSTF    | SendTo Command with Flags (UDP Only) B656 or later
//--------------|-----------------------------------------------------|
// AT+NSORF     | Receive Command (UDP only) B350 or later
//--------------|-----------------------------------------------------|
// AT+NSOCL     | Close a Socket B350 or later
//--------------|-----------------------------------------------------|
// +NSONMI      | Socket Message Arrived Indicator (Response Only) B350 or later
//--------------|-----------------------------------------------------|
// AT+NPING     | Test IP Network Connectivity to a Remote Host B350 or later
//--------------|-----------------------------------------------------|
// AT+NBAND     | Set Supported Bands B600 or later
//--------------|-----------------------------------------------------|
// AT+NLOGLEVEL | Set Debug Logging Level B600 or later
//--------------|-----------------------------------------------------|
// AT+NCONFIG   | Configure UE Behaviour B650 or later
//--------------|-----------------------------------------------------|
// AT+NATSPEED  | Configure UART Port Baud Rate B656 or later
//--------------|-----------------------------------------------------|
// AT+NCCID     | Card Identification B657SP1 or later
//--------------|-----------------------------------------------------|
// AT+NFWUPD    | Firmware Update via UART B657SP1 or later
//--------------|-----------------------------------------------------|
// AT+NRDCTRL   | Control Radio Configurations B657SP1 or later
//--------------|-----------------------------------------------------|
// AT+NCHIPINFO | Read System Information B657SP1 or later
//--------------|-----------------------------------------------------|
  
/*  Temporary Commands     */
//--------------|-----------------------------------------------------|
// AT+NTSETID   | Set ID B350 or later
//--------------|-----------------------------------------------------|

/*
 * 常量定义区域
 */
const char* AT_SYNC      = "AT";
const char* AT_CGMI      = "AT+CGMI";
const char* AT_CGMM      = "AT+CGMM";
const char* AT_CGMR      = "AT+CGMR";
const char* AT_CGSN      = "AT+CGSN";
const char* AT_CEREG     = "AT+CEREG";
const char* AT_CSCON     = "AT+CSCON";
const char* AT_CLAC      = "AT+CLAC";
const char* AT_CSQ       = "AT+CSQ";
const char* AT_CGPADDR   = "AT+CGPADDR";
const char* AT_COPS      = "AT+COPS";
const char* AT_CGATT     = "AT+CGATT";
const char* AT_CGACT     = "AT+CGACT";
const char* AT_CIMI      = "AT+CIMI";
const char* AT_CGDCONT   = "AT+CGDCONT";
const char* AT_CFUN      = "AT+CFUN";
const char* AT_CMEE      = "AT+CMEE";
const char* AT_CCLK      = "AT+CCLK";
const char* AT_CPSMS     = "AT+CPSMS";
const char* AT_CEDRXS    = "AT+CEDRXS";
const char* AT_CEER      = "AT+CEER";
const char* AT_CEDRXRDP  = "AT+CEDRXRDP";
const char* AT_CTZR      = "AT+CTZR";

/* ETSI Commands* (127.005)  <Under development> */
/*
const char* AT_CSMS     = "AT+CSMS";
const char* AT_CNMA     = "AT+CNMA";
const char* AT_CSCA     = "AT+CSCA";
const char* AT_CMGS     = "AT+CMGS";
const char* AT_CMGC     = "AT+CMGC";
const char* AT_CSODCP   = "AT+CSODCP";
const char* AT_CRTDCP   = "AT+CRTDCP";
*/
const char* AT_NMGS     = "AT+NMGS";
const char* AT_NMGR     = "AT+NMGR";
const char* AT_NNMI     = "AT+NNMI";
const char* AT_NSMI     = "AT+NSMI";
const char* AT_NQMGR    = "AT+NQMGR";
const char* AT_NQMGS    = "AT+NQMGS";
const char* AT_NMSTATUS = "AT+NMSTATUS";
const char* AT_NRB      = "AT+NRB";
const char* AT_NCDP     = "AT+NCDP";

const char* AT_NUESTATS = "AT+NUESTATS";

const char* AT_NEARFCN  = "AT+NEARFCN";

const char* AT_NSOCR    = "AT+NSOCR";//creat a socket
const char* AT_NSOST    = "AT+NSOST";//sendTo Command(UDP only)
const char* AT_NSOSTF   = "AT+NSOSTF";
const char* AT_NSORF    = "AT+NSORF";//receive command

const char* AT_NSOCO		= "AT+NSOCO"; //connect command(TCP only)
const char* AT_NSOSD		= "AT+NSOSD";//send command(TCP only)

const char* AT_NSOCL    = "AT+NSOCL";//Close a socket

// const char*= "+NSONMI";      
const char*  AT_NPING        = "AT+NPING";
const char*  AT_NBAND        = "AT+NBAND";
const char*  AT_NLOGLEVEL    = "AT+NLOGLEVEL";
const char*  AT_NCONFIG      = "AT+NCONFIG";
const char*  AT_NATSPEED     = "AT+NATSPEED";
const char*  AT_NCCID        = "AT+NCCID";
const char*  AT_NFWUPD       = "AT+NFWUPD";
const char*  AT_NRDCTRL      = "AT+NRDCTRL";
const char*  AT_NCHIPINFO    = "AT+NCHIPINFO";
const char*  AT_NTSETID      = "AT+NTSETID";

//==============================================================================
//

#define CMD_TRY_TIMES           10
#define CMD_READ_ARGUMENT       "?"
#define CMD_TEST_ARGUMENT       "=?"

#define CMD_OK_RES              "OK"

#define REMOTE_SERVER_IP        "123.206.108.227"
#define REMOTE_SERVER_PORT      "9099"


#define REMOTE_COAP_INFO        "115.29.240.46,5683"

#define LOCAL_UDP_SET           "DGRAM,17,10000,1"

#define LOCAL_TCP_SET           "STREAM,6,56000,1"

#define BAND_850MHZ_ID           5
#define BAND_850MHZ_STR          "850"

#define BAND_900MHZ_ID           8
#define BAND_900MHZ_STR          "900"

#define BAND_800MHZ_ID           20
#define BAND_800MHZ_STR          "800"

#define BAND_700MHZ_ID           28
#define BAND_700MHZ_STR          "700"

//******************************************************************************
// 定义NB_FxnTable 变量将BC95操作函数结合在一起
//
const NB_FxnTable BC95_FxnTable = {
  .openFxn               = bc95_open,
  .nbModuleInit          = bc95_init,
  .getModuleInfo         = bc95_moduleInfo,
  .getModuleRegisterInfo = bc95_register,
  .getUSIMinfo           = bc95_getIMSI,
  .getSign               = bc95_getSignal,
  .createUdp             = bc95_createUDP,
  .closeUdp              = bc95_closeUDP,
  .sendUdp               = bc95_sendUDP,
	.createTcp             = bc95_createTCP,
  .closeTcp              = bc95_closeTCP,
	.connectTcp            = bc95_connectTCP,
  .sendTcp               = bc95_sendTCP,
  .coapServer            = bc95_coapServer,
  .coapSentIndication    = bc95_coapSentIndication,
  .coapSetReceMode       = bc95_coapReceIndication,
  .coapSentMsg           = bc95_coapSendMsg,
  .mainThread 					 = bc95_main
};

//******************************************************************************
// 用法说明：
// 此常量用于定义存放发送指令缓存区大小。考虑目前大多发送指令不会超过128个字节，
// 除udp与coap发送消息，与接收消息。如果用户需要发送长度数据指令，可以改大此数值
//

//定义全局缓存变量
#define NB_UART_RECE_BUF_MAX_LEN    512
#define NB_UART_SEND_BUF_MAX_LEN    512

//接收缓存空间
struct ReceBuf
{
  char    Buf[NB_UART_RECE_BUF_MAX_LEN];
  uint16_t  len;                                    //有效数据长度
}gNBReceBuf;

//发送缓存空间,
struct SendBuf
{
  char   Buf[NB_UART_SEND_BUF_MAX_LEN];
  uint16_t len;
}gNBSendBuf;

typedef struct
{
  uint8_t nb_socket_id;     //指示相应的socket id
  uint16_t nb_data_len;     //提示数据长度
}udp_rece_t;
//******************************************************************************
// Name : nb95_status_t
//
// brief : 定义存储BC95的状态信息
typedef struct
{
  uint8_t      nb95_connection_status;
  uint8_t      nb95_register_status;
  uint8_t      nb95_IMSI[16];
  uint8_t      nb95_IMEI[16];
  
  char         nb95_udp_id[2];  //存放已经创建的UDP SOCKET ID
  udp_rece_t   nb95_udp_len;    //指示接收到的UDP数据长度
	char         nb95_tcp_id[2];  //存放已经创建的TCP SOCKET ID
  udp_rece_t   nb95_tcp_len;    //指示接收到的TCP数据长度

}nb95_status_t;

//==============================================================================
static nb95_status_t g_bc95_status;

uint16_t      g_event_regTable = 0;//static 

#define    NB_SP_RECE_EVENT       0x0001           //收到串口数据
#define    NB_TIMEOUT_EVENT       0x0002           //超时事件
#define    NB_UDPRECE_EVENT       0x0004           //UDP接收事件
#define    NB_TCPRECE_EVENT       0x0020           //TCP接收事件
#define    NB_COAP_RE_EVENT       0X0008           //COAP接收事件
#define    NB_REG_STA_EVENT       0x0010           //NB IOT网络附着事件

//==============================================================================
//用于保存当前正在执行的AT指令
static cmd_info_t g_at_cmd;
//==============================================================================
//NBiot 操作状态
  module_state        g_nb_state = {PROCESS_NONE,0}; //static

//==============================================================================
//NB模块操作子状态定义
//state = PROCESS_INIT
typedef enum
{
  SUB_NONE,
  SUB_SYNC,
	#ifdef DABI_UDP
  SUB_CMEE,
  SUB_CFUN,
	SUB_CGSN,
	
	SUB_CSCON,
	SUB_NSMI,
  SUB_NNMI,
	#endif
	SUB_CEREG,
  SUB_CIMI, 

  SUB_CGATT,
  SUB_CGATT_QUERY,

  SUB_CGMI,
  SUB_CGMM,
  SUB_CGMR,
  SUB_NBAND,
  SUB_UDP_CR,
  SUB_UDP_CL,
  SUB_UDP_ST,
  SUB_UDP_RE,
	SUB_TCP_CR,
  SUB_TCP_CL,
	SUB_TCP_CNT,
  SUB_TCP_ST,
  SUB_TCP_RE,
	
  SUB_END
}sub_id_t;

//******************************************************************************
// bc95 初始化流程 
//
const uint8_t nb95_init_process[] = {SUB_NONE,SUB_SYNC,
                                     SUB_CIMI,
                                     SUB_CGATT,SUB_CGATT_QUERY,SUB_END};

//******************************************************************************
// bc95 获取信息流程
//
const uint8_t nb95_info_process[] = {SUB_NONE, SUB_CGMI,SUB_CGMM,SUB_CGMR,
                                     SUB_NBAND,SUB_END};

//******************************************************************************
// bc95 UDP创建流程 
//
const uint8_t nb95_udpcr_process[] = {SUB_NONE,SUB_UDP_CR,SUB_UDP_CL,SUB_UDP_CR,
                                      SUB_END};

// bc95 UDP关闭流程
const uint8_t nb95_udpcl_process[] = {SUB_NONE,SUB_UDP_CL,SUB_END};

// bc95 UDP发送流程
const uint8_t nb95_udpst_process[] = {SUB_NONE,SUB_UDP_ST,SUB_END};

//bc95 UDP
//const uint8_t nb95_udpre_process[] = {SUB_NONE,UB_UDP_RE,SUB_END};

//******************************************************************************
// bc95 TCP创建流程 
//
const uint8_t nb95_tcpcr_process[] = {SUB_NONE,SUB_TCP_CR,SUB_TCP_CNT,
                                      SUB_TCP_ST,SUB_END};

// bc95 TCP关闭流程
const uint8_t nb95_tcpcl_process[] = {SUB_NONE,SUB_TCP_CL,SUB_END};

// bc95 TCP发送流程
const uint8_t nb95_tcpst_process[] = {SUB_NONE,SUB_TCP_ST,SUB_END};

//==============================================================================
// 函数
//==============================================================================

//声明区域
//******************************************************************************
// fn : nbsend_msg_app
//
// brief : 通过回调函数，向应用层发送指令参数
//
// param : 
//
// return : none
static void nbsend_msg_app(NB_Handle handle, char**buf,Bool isOk);

//******************************************************************************
// fn : NB_Strtoul
//
// brief : 将字符型数字转成整型
//
// param : pStr -> 要转换内容地址
//         base -> 转换内容以多少进制进行转换（2-36）
//
// return : 整形数据
static uint32_t NB_Strtoul(const char* pStr,int base);
//******************************************************************************
// fn : NB_HexStrToNUM
//
// brief : 将十六进制字符串转成数字
//
// param : len -> 字符长度
//         str -> 十六进制字符串内容
//
// return : 返回转换后的长度
static uint16_t NB_HexStrToNum(char* str);
//******************************************************************************
// fn : cmd_generate
//
// brief : 通过cmd_info_t类型变量，生成AT指令
//
// param : cmdHandle->AT指令结构指针
//
// return : 生成字符串指令长度
static int cmd_generate(CmdHandle cmdHandle);
//******************************************************************************
// fn : bc95_receUDP
//
// brief : 接收UDP数据
//
// param : handle -> NB 结构信息指针
//
//
// return : FAIL -> 没有创建UDP,SUCCESS->指令执行成功
int bc95_receUDP(NB_Handle handle);


static void nbset_event(int event_id)
{
  g_event_regTable |= event_id;
}
//******************************************************************************
// fn : nbsend_msg_app
//
// brief : 通过回调函数，向应用层发送指令参数
//
// param : 
//
// return : none
static void nbstop_timer(HWAttrs_Handle hw_handle)
{
  if(g_at_cmd.max_timeout)
  {
    hw_handle->timer_fxnTable->stopTimerFxn();
  }
}
//******************************************************************************
// fn : NB_SendCmd
//
// brief : 通过注册的串口函数向外发送at指令
//
// param : hw_handle ->  硬件操作函数指针
//         cmdHandle -> 将要发送指令信息
//
// return : none
static void NB_SendCmd(HWAttrs_Handle hw_handle,CmdHandle cmdHandle)
{
  int strLen = 0;
  if(hw_handle == NULL || cmdHandle == NULL)
  {
    return;
  }
  strLen = cmd_generate(cmdHandle);
  
  hw_handle->uart_fxnTable->sendFxn((uint8_t*)gNBSendBuf.Buf,strLen);
  
  //开启定时器
  if(cmdHandle->max_timeout)
  {
    hw_handle->timer_fxnTable->startTimerFxn(g_at_cmd.max_timeout);
  }
}
//******************************************************************************
// fn : reset_rece_buf
//
// brief : 复位接收缓存
//
// param : none
//
// return : none
static void reset_rece_buf(void)
{
  memset(&gNBReceBuf,0,sizeof(struct ReceBuf));
}
//******************************************************************************
// fn : cmd_param_init
//
// brief : 对cmd_info_t类型变理进行初始化
//
// param : cmdHandle->AT指令结构指针
//
// return : none
static void cmd_param_init(CmdHandle cmdHandle,const char* AT,char* argument,cmd_property_t property)
{
  if(cmdHandle == null)
  {
    return;
  }
  cmdHandle->cmd_try = CMD_TRY_TIMES;
  cmdHandle->property = property;
  cmdHandle->cmd_action = ACTION_OK_NEXT_ERROR_TRY;
  cmdHandle->pArgument = argument;
  cmdHandle->pExpectRes = null;
  cmdHandle->haveTried = 0;
  cmdHandle->max_timeout = 2000;
  cmdHandle->pCMD = AT;
  cmdHandle->lmt_period = FALSE;
}
//******************************************************************************
// fn : cmd_next
//
// brief : 产生下一条AT指令
//
// param : none
//
// return : FALSE -> 所有要执行的指令都执行完成，TRUE->OK
// ACTION:
// 会用到全局变量nb_handle,nb_state
static Bool cmd_next()
{
  
  if(g_nb_state.state == PROCESS_INIT)
  {
    g_nb_state.sub_state++;
    if(nb95_init_process[g_nb_state.sub_state]  == SUB_END)
    {
      return FALSE;
    }
    
    switch(nb95_init_process[g_nb_state.sub_state])
    {
			
		#ifdef DABI_UDP	
    case SUB_CMEE:
      {
        cmd_param_init(&g_at_cmd,AT_CMEE,"1",CMD_SET);
      }
      break;
    case SUB_CFUN:
      {
        cmd_param_init(&g_at_cmd,AT_CFUN,"1",CMD_SET);
        g_at_cmd.max_timeout = 10000;          //10S 
      }
      break;
		case SUB_CGSN:
      {
        cmd_param_init(&g_at_cmd,AT_CGSN,"1",CMD_SET);
      }
      break;

    case SUB_CSCON:
      {
        cmd_param_init(&g_at_cmd,AT_CSCON,"1",CMD_SET);
      }
      break;	
		case SUB_NSMI:
      {
        //设置coap 消息发送完通知
        cmd_param_init(&g_at_cmd,AT_NSMI,"1",CMD_SET);
      }
      break;
    case SUB_NNMI:
      {
        //设置coap接收消息，给予指示
        cmd_param_init(&g_at_cmd,AT_NNMI,"2",CMD_SET);
      }
      break;
		
		#endif	
		case SUB_CEREG:
		{
			cmd_param_init(&g_at_cmd,AT_CEREG,"1",CMD_SET);
		}
		break;
		
    case SUB_CIMI:
      {
        cmd_param_init(&g_at_cmd,AT_CIMI,NULL,CMD_EXCUTE);
      }
      break;

    case SUB_CGATT:
      {
        cmd_param_init(&g_at_cmd,AT_CGATT,"1",CMD_SET);
        g_at_cmd.max_timeout = 3000;  //相邻指令之间限制3s
        g_at_cmd.lmt_period = TRUE;
      }
      break;
    case SUB_CGATT_QUERY:
      {
        cmd_param_init(&g_at_cmd,AT_CGATT,NULL,CMD_READ);
        g_at_cmd.max_timeout = 3000;  //相邻指令之间限制3s
        g_at_cmd.lmt_period = TRUE;
        g_at_cmd.pExpectRes = "CGATT:1";     //设置期望回复消息，如果指令执行完成
                                             //没有与期望的消息匹配，则认为出错
                                             //并进行出错尝试
      }
      break;

    }
    
  }
  else if(g_nb_state.state == PROCESS_MODULE_INFO)
  {
    g_nb_state.sub_state++;
    if(nb95_info_process[g_nb_state.sub_state] == SUB_END)
    {
      return FALSE;
    }
    switch(nb95_info_process[g_nb_state.sub_state])
    {
    case SUB_CGMM:
      {
        cmd_param_init(&g_at_cmd,AT_CGMM,null,CMD_EXCUTE);
      }
      break;
    case SUB_CGMR:
      {
        cmd_param_init(&g_at_cmd,AT_CGMR,null,CMD_EXCUTE);
      }
      break;
    case SUB_NBAND:
      {
        cmd_param_init(&g_at_cmd,AT_NBAND,null,CMD_READ);
      }
      break;
    }
  }
  else if (g_nb_state.state == PROCESS_SIGN)
  {
    g_nb_state.sub_state++;
    return FALSE;
  }
  else if(g_nb_state.state == PROCESS_UDP_CR)
  {
    g_nb_state.sub_state++;
    if(nb95_udpcr_process[g_nb_state.sub_state]  == SUB_END)
    {
      return FALSE;
    }
    switch(nb95_udpcr_process[g_nb_state.sub_state])
    {
    case SUB_UDP_CL:
      {
        cmd_param_init(&g_at_cmd,AT_NSOCL,"0",CMD_SET);
      }
      break;
    case SUB_UDP_CR:
      {
        cmd_param_init(&g_at_cmd,AT_NSOCR,LOCAL_UDP_SET,CMD_SET);
      }
      break;
    }
  }
  else if(g_nb_state.state == PROCESS_UDP_CL)
  {
    g_nb_state.sub_state++;
//    if(nb95_udpcl_process[g_nb_state.sub_state]  == SUB_END)
//    {
//      return FALSE;
//    }
    return FALSE;
  }
  else if(g_nb_state.state == PROCESS_UDP_ST)
  {
    g_nb_state.sub_state++;
    return FALSE;
  }
  else if(g_nb_state.state == PROCESS_UDP_RE)
  {
    g_nb_state.sub_state++;
    return FALSE;
  }
  else if(g_nb_state.state == PROCESS_COAP)
  {
    g_nb_state.sub_state++;
    return FALSE;
  }
  else if(g_nb_state.state == PROCESS_COAP_ST)
  {
    g_nb_state.sub_state++;
    return FALSE;
  }
  else if(g_nb_state.state == PROCESS_COAP_RE)
  {
    g_nb_state.sub_state++;
    return FALSE;
  }
  
  return TRUE;
}

//******************************************************************************
// fn : cmd_generate
//
// brief : 通过cmd_info_t类型变量，生成AT指令
//
// param : cmdHandle->AT指令结构指针
//
// return : 生成字符串指令长度
static int cmd_generate(CmdHandle cmdHandle)
{
  int cmdLen = 0;
  if(cmdHandle == null)
  {
    return cmdLen;
  }
  
  memset(gNBSendBuf.Buf,0,NB_UART_SEND_BUF_MAX_LEN);
  gNBSendBuf.len = 0;
  
  if(cmdHandle->property == CMD_TEST)
  {
    cmdLen = snprintf(gNBSendBuf.Buf,NB_UART_SEND_BUF_MAX_LEN,
                      "%s=?\r\n",
                      cmdHandle->pCMD);
  }
  else if(cmdHandle->property == CMD_READ)
  {
    cmdLen = snprintf(gNBSendBuf.Buf,NB_UART_SEND_BUF_MAX_LEN,
                  "%s?\r\n",
                  cmdHandle->pCMD);
  }
  else if(cmdHandle->property == CMD_EXCUTE)
  {
    cmdLen = snprintf(gNBSendBuf.Buf,NB_UART_SEND_BUF_MAX_LEN,
                  "%s\r\n",
                  cmdHandle->pCMD);    
  }
  
  else if(cmdHandle->property == CMD_SET)
  {
    cmdLen = snprintf(gNBSendBuf.Buf,NB_UART_SEND_BUF_MAX_LEN,
                  "%s=%s\r\n",
                  cmdHandle->pCMD,cmdHandle->pArgument);    
  }
  gNBSendBuf.len = cmdLen;
  return cmdLen;
}
//******************************************************************************
// fn : cmd_isPass
//
// brief : 判断bc95执行的AT指令是否执行成功
//
// param : none
//
// return : none
static int8_t cmd_isPass(char* buf)
{
  int8 result = -1;
  if(g_at_cmd.pExpectRes == null)
  {
    if (strstr(buf,"OK"))
    {
      result = TRUE; ;
    }
    else if (strstr(buf,"ERROR"))
    {
      //+CMEE ERROR=[][]
      result = FALSE;
    }
  }
  else
  {
    if(strstr(buf,"OK"))
    {
      if(strstr(buf,g_at_cmd.pExpectRes))
      {
        result = TRUE;
      }
      else
      {
        result = FALSE;
      }
        
    }
    else if(strstr(buf,"ERROR"))
    {
      //+CMEE ERROR
      result = FALSE;
    }
  }

  return result;
}
//******************************************************************************
// fn : addr_adjust
//
// brief : 根据异步消息，调整指令响应数据
//
// param : 
//
// return : TRUE ->表示要执行指令响应，FALSE ->不执行 
uint8_t addr_adjust(char* buf,char* pStart,uint16_t* plen)
{
  uint8_t isAsync = FALSE;
  char* pEnd = NULL;
  uint8_t msg_len = 0;
  if((pStart - buf )>= 2)
  {
    pStart -= 2;
  }
  if((pEnd = strstr(pStart,"\r\n")) != NULL)
  {
    if(pEnd == pStart)
    {
      pEnd = strstr(pStart + 2,"\r\n");
    }
    if(!pEnd)
    {
      *plen = (uint8_t)(pStart - buf);
      
       return !!(*plen);
    }
    pEnd += 2;
    msg_len = (uint8_t)(pEnd - pStart);

    if(*plen >= msg_len)
    {
      *plen -= msg_len;
      if(*plen == 0)
      {
        isAsync = TRUE;
      }
      else
      {
        if(pStart == buf)
        {
          buf += msg_len;
        }
      }
    }
  }
  return isAsync;
}
//******************************************************************************
// fn : bc95_AsyncNotification
//
// brief : 处理BC95异步返回的通知
//
// param : buf-> 消息内容指针
//         len-> 消息长度
//
// return : >0 是异步通知；=0 不是异步通知
uint8_t bc95_AsyncNotification(char* buf, uint16_t* len)
{
  uint8_t isAsync = FALSE;
  char* position_addr_start = NULL;

  if((position_addr_start = strstr(buf,"+CEREG:")) != NULL)
  {
    char* pColon = strchr(position_addr_start,',');
    if(pColon)
    {
      //g_bc95_status.nb95_connection_status = NB_Strtoul(pColon,10);
      pColon++;
      g_bc95_status.nb95_register_status = (*pColon - 0x30);
    }
    isAsync = addr_adjust(buf,position_addr_start,len);
    
    nbset_event(NB_REG_STA_EVENT); 
    //isAsync =TRUE;
  }
  else if((position_addr_start = strstr(buf,"+CSCON")) != NULL)
  {
    char* pColon = strchr(position_addr_start,':');
    if(pColon)
    {
      pColon++;
      //g_bc95_status.nb95_register_status = NB_Strtoul(pColon,10);
      g_bc95_status.nb95_connection_status = (*pColon - 0x30);
    }
    isAsync = addr_adjust(buf,position_addr_start,len);
    //isAsync =TRUE;
  }
	
#if 0
  else if((position_addr_start = strstr(buf,"+NSONMI")) != NULL)
  {
    //收到服务器端发来的UDP数据
    char* pColon = strchr(position_addr_start,':');
    if(pColon)
    {
      pColon++;
      g_bc95_status.nb95_udp_len.nb_socket_id = NB_Strtoul(pColon,10);
    }
    char* pComma = strchr(pColon,',');
    if(pComma)
    {
      pComma++;
      g_bc95_status.nb95_udp_len.nb_data_len = NB_Strtoul(pComma,10);
    }
    
    isAsync = addr_adjust(buf,position_addr_start,len);
    //isAsync =TRUE;
    nbset_event(NB_UDPRECE_EVENT);  
  }
#endif
	else if((position_addr_start = strstr(buf,"+NSONMI")) != NULL)
  {
    //收到服务器端发来的TCP数据
    char* pColon = strchr(position_addr_start,':');
    if(pColon)
    {
      pColon++;
      g_bc95_status.nb95_tcp_len.nb_socket_id = NB_Strtoul(pColon,10);
    }
    char* pComma = strchr(pColon,',');
    if(pComma)
    {
      pComma++;
      g_bc95_status.nb95_tcp_len.nb_data_len = NB_Strtoul(pComma,10);
    }
    
    isAsync = addr_adjust(buf,position_addr_start,len);
    //isAsync =TRUE;
    nbset_event(NB_TCPRECE_EVENT);  
  }
  else if((position_addr_start = strstr(buf,"+NNMI")) != NULL)
  {
    isAsync = addr_adjust(buf,position_addr_start,len);
    //isAsync =TRUE;
    nbset_event(NB_COAP_RE_EVENT);  
  }
  
  return isAsync;
}
//******************************************************************************
// fn : bc95_receCb
//
// brief : bc95 处理串口返回数据
//
// param : buf -> 接收的数据缓存地址
//         len -> 数据长度
//
// return : none
static void bc95_receCb(char* buf, uint16 len)
{
  if(len == 0)
  {
    return;
  }
  //printf("<1len=%d>",len);
  if(!bc95_AsyncNotification(buf,&len))
  {
    //printf("<2len=%d>",len);
    if(len == 0)
    {
      return;
    }
    if((gNBReceBuf.len + len) < NB_UART_RECE_BUF_MAX_LEN)
    {   
      memcpy(gNBReceBuf.Buf + gNBReceBuf.len ,buf,len);
      gNBReceBuf.len += len;
    }
    nbset_event(NB_SP_RECE_EVENT);
  }

}

//******************************************************************************
// fn : bc95_timeout_cb
//
// brief : bc95 处理串口返回数据
//
// param : none
//
// return : none
static void bc95_timeoutCb(void)
{
  nbset_event(NB_TIMEOUT_EVENT);
}
//******************************************************************************
// fn : state_reset
//
// brief : 复位NBiot进程状态
//
// param : none
//
// return : none
static void state_reset()
{
  g_nb_state.state = PROCESS_NONE;
  g_nb_state.sub_state = 0;
}
//******************************************************************************
// fn : bc95_open
//
// brief : 打开bc95使用的uart串口硬件
//
// param : handle ->nb对象指针
//
// return : none

int bc95_open(NB_Handle handle)
{
  HWAttrs_Handle  hw_handle = (HWAttrs_Handle)handle->object;
  
  g_event_regTable = 0;
  state_reset();
  reset_rece_buf();
  
  //注册串口接收回调函数，
  hw_handle->uart_fxnTable->openFxn(bc95_receCb, hw_handle->baudrate);
  
  //注册定时器超时回调函数
  hw_handle->timer_fxnTable->initTimerFxn(bc95_timeoutCb);
  
  return SUCCESS;
}

//******************************************************************************
// fn : bc95_init
//
// brief : 初始化BC95模块，并进行网络初始化
//
// param : handle ->nb对象指针
//
// return : FALSE -> 操作失败，TRUE -> 操作成功
int bc95_init(NB_Handle handle)
{
  
  HWAttrs_Handle hw_handle = (HWAttrs_Handle)handle->object;
  
  if(g_nb_state.state != PROCESS_NONE)
  {
    return FAIL;
  }
  
  cmd_param_init(&g_at_cmd,AT_SYNC,null,CMD_EXCUTE);
  
  //更改NBiot操作进程，进入Init状态
  g_nb_state.state = PROCESS_INIT;
  g_nb_state.sub_state = 1;
  
  //发送ASYNC 指令
  g_at_cmd.max_timeout = 2000;
  
  NB_SendCmd(hw_handle,&g_at_cmd);
  
  return SUCCESS;
}

//******************************************************************************
// fn : bc95_moduleInfo
//
// brief : 获取NB模块的信息。比如：Manufacturer ID, Device Module,
//         Firmware Version,Freqenucy band
//
// param : handle ->nb对象指针
//
// return : none
int bc95_moduleInfo(NB_Handle handle)
{
  HWAttrs_Handle hw_handle = (HWAttrs_Handle)handle->object;
  
  if(g_nb_state.state != PROCESS_NONE)
  {
    return FAIL;
  }
  
  cmd_param_init(&g_at_cmd,AT_CGMI,null,CMD_EXCUTE);
  
  //更改NBiot操作进程，进入INFO状态
  g_nb_state.state = PROCESS_MODULE_INFO;
  g_nb_state.sub_state = 1;
 
  
  //发送ASYNC 指令
  g_at_cmd.max_timeout = 1000;
  
  NB_SendCmd(hw_handle,&g_at_cmd);
  
  return SUCCESS;
}
//******************************************************************************
// fn : bc95_register
//
// brief : 查询NB模块是否注册入网
//
// param : handle ->nb对象指针
//
// return : none
int bc95_register(NB_Handle handle)
{
  return g_bc95_status.nb95_register_status;
}
//******************************************************************************
// fn : bc95_getIMSI
//
// brief : 读取模块IMSI号
//
// param : handle ->nb对象指针
//
// return : none
const char* bc95_getIMSI(NB_Handle handle)
{
  return (const char*)g_bc95_status.nb95_IMSI;
}

//******************************************************************************
// fn : bc95_getSignal
//
// brief : 读取当前信号强度
//
// param : handle ->nb对象指针
//
// return : none
int bc95_getSignal(NB_Handle handle)
{
  HWAttrs_Handle hw_handle = (HWAttrs_Handle)handle->object;
  
  if(g_nb_state.state != PROCESS_NONE)
  {
    return FAIL;
  }
  
  cmd_param_init(&g_at_cmd,AT_CSQ,null,CMD_EXCUTE);
  
  g_at_cmd.cmd_try = 1;
  
  //更改NBiot操作进程，进入Init状态
  g_nb_state.state = PROCESS_SIGN;
  g_nb_state.sub_state = 1;
  
  NB_SendCmd(hw_handle,&g_at_cmd);
  
  return SUCCESS;
}

//******************************************************************************
// fn : bc95_createUDP
//
// brief : 创建UDP
//
// param : handle ->nb对象指针
//
// return : none
int bc95_createUDP(NB_Handle handle)
{
  HWAttrs_Handle hw_handle = (HWAttrs_Handle)handle->object;
  
  if(g_nb_state.state != PROCESS_NONE)
  {
    return FAIL;
  }
  
  cmd_param_init(&g_at_cmd,AT_NSOCR,LOCAL_UDP_SET,CMD_SET);
  
  g_at_cmd.cmd_action = ACTION_OK_EXIT_ERROR_NEXT;
  g_at_cmd.cmd_try = 1;
  //更改NBiot操作进程，进入Init状态
  g_nb_state.state = PROCESS_UDP_CR;
  
  //UDP 的执行流程是通过SUB_UDP_PROCESS数组指定AT指令流程
  g_nb_state.sub_state = 1;
  
  NB_SendCmd(hw_handle,&g_at_cmd);
  
  return SUCCESS;
}
//******************************************************************************
// fn : bc95_closeUDP
//
// brief : 关闭当前创建UDP
//
// param : handle ->nb对象指针
//
// return : none
int bc95_closeUDP(NB_Handle handle)
{
  HWAttrs_Handle hw_handle = (HWAttrs_Handle)handle->object;
  
  if(g_nb_state.state != PROCESS_NONE)
  {
    return FAIL;
  }
  
  if(g_bc95_status.nb95_udp_id[0] < '0' || g_bc95_status.nb95_udp_id[0] > '6' )
  {
    return FAIL;
  }
    
  cmd_param_init(&g_at_cmd,AT_NSOCL,g_bc95_status.nb95_udp_id,CMD_SET);
  
  //更改NBiot操作进程，进入Close UDP状态
  g_nb_state.state = PROCESS_UDP_CL;
  
  //UDP 的执行流程是通过SUB_UDP_PROCESS数组指定AT指令流程
  g_nb_state.sub_state = 1;
  
  NB_SendCmd(hw_handle,&g_at_cmd);
  return SUCCESS;
}

//******************************************************************************
// fn : bc95_sendUDP
//
// brief : 发送UDP数据
//
// param : handle -> NB 结构信息指针
//         len -> 信息长度
//         msg -> 信息
//
// return : FAIL -> 没有创建UDP,SUCCESS->指令执行成功
int bc95_sendUDP(NB_Handle handle,int len,char* msg)
{
  HWAttrs_Handle hw_handle = (HWAttrs_Handle)handle->object;
  
  if(g_nb_state.state != PROCESS_NONE)
  {
    return FAIL;
  }
  
  //判断SOCKET ID 是否正确
  if(g_bc95_status.nb95_udp_id[0] < '0' || g_bc95_status.nb95_udp_id[0] > '6' )
  {
    return FAIL;
  }
  
  uint16_t max_len = (NB_UART_SEND_BUF_MAX_LEN - 40) >> 1;
  uint16_t str_len = 0;
  
  char  buf[NB_UART_SEND_BUF_MAX_LEN - 40];
  memset(buf,0,NB_UART_SEND_BUF_MAX_LEN - 40);
  
  if(len > max_len)
  {
    str_len  = max_len;
  }
  else
  {
    str_len = len;
  }
  
  uint16_t msg_len = snprintf(buf,NB_UART_SEND_BUF_MAX_LEN-40,"%s,%s,%s,%d,",
                                          g_bc95_status.nb95_udp_id,
                                          REMOTE_SERVER_IP,
                                          REMOTE_SERVER_PORT,
                                          str_len);
  
  for(uint16_t i = 0 ; i < str_len ; i++)
  {
    sprintf(&buf[msg_len + (i << 1)],"%02X",(uint8)msg[i]);
  }
  
  cmd_param_init(&g_at_cmd,AT_NSOST,buf,CMD_SET);
  g_at_cmd.max_timeout = 2000;
  
  //更改NBiot操作进程，进入Close UDP状态
  g_nb_state.state = PROCESS_UDP_ST;
  g_nb_state.sub_state = 1;
  
  NB_SendCmd(hw_handle,&g_at_cmd);
  
  return SUCCESS;
}
//******************************************************************************
// fn : bc95_receUDP
//
// brief : 接收UDP数据
//
// param : handle -> NB 结构信息指针
//
//
// return : FAIL -> 没有创建UDP,SUCCESS->指令执行成功
int bc95_receUDP(NB_Handle handle)
{
  HWAttrs_Handle hw_handle = (HWAttrs_Handle)handle->object;
  
  if(g_nb_state.state != PROCESS_NONE)
  {
    return FAIL;
  }
  uint16_t max_len = (NB_UART_SEND_BUF_MAX_LEN - 40) >> 1;
  uint16_t read_len = max_len;
  
  
  if(g_bc95_status.nb95_udp_len.nb_data_len < max_len)
  {
    read_len = g_bc95_status.nb95_udp_len.nb_data_len;
  }
  
  char buf[10] ;
  
  max_len = snprintf(buf,10,"%d,%d",g_bc95_status.nb95_udp_len.nb_socket_id,
                         read_len);
  buf[max_len] = 0;
  
  cmd_param_init(&g_at_cmd,AT_NSORF,buf,CMD_SET);
  
  //更改NBiot操作进程，进入 UDP READ状态
  g_nb_state.state = PROCESS_UDP_RE;
  g_nb_state.sub_state = 1;
  
  NB_SendCmd(hw_handle,&g_at_cmd);
  return SUCCESS;
}

//******************************************************************************
// fn : bc95_createTCP
//
// brief : 创建TCP
//
// param : handle ->nb对象指针
//
// return : none
int bc95_createTCP(NB_Handle handle)
{
  HWAttrs_Handle hw_handle = (HWAttrs_Handle)handle->object;
  
  if(g_nb_state.state != PROCESS_NONE)
  {
    return FAIL;
  }
  
  cmd_param_init(&g_at_cmd,AT_NSOCR,LOCAL_TCP_SET,CMD_SET);
  
  g_at_cmd.cmd_action = ACTION_OK_EXIT_ERROR_NEXT;
  g_at_cmd.cmd_try = 1;
  //更改NBiot操作进程，进入Init状态
  g_nb_state.state = PROCESS_TCP_CR;
  
  //TCP 的执行流程是通过SUB_TCP_PROCESS数组指定AT指令流程
  g_nb_state.sub_state = 1;
  
  NB_SendCmd(hw_handle,&g_at_cmd);
  
  return SUCCESS;
}
//******************************************************************************
// fn : bc95_closeTCP
//
// brief : 关闭当前创建TCP
//
// param : handle ->nb对象指针
//
// return : none
int bc95_closeTCP(NB_Handle handle)
{
  HWAttrs_Handle hw_handle = (HWAttrs_Handle)handle->object;
  
  if(g_nb_state.state != PROCESS_NONE)
  {
    return FAIL;
  }
  
  if(g_bc95_status.nb95_tcp_id[0] < '0' || g_bc95_status.nb95_tcp_id[0] > '6' )
  {
    return FAIL;
  }
    
  cmd_param_init(&g_at_cmd,AT_NSOCL,g_bc95_status.nb95_tcp_id,CMD_SET);
  
  //更改NBiot操作进程，进入Close UDP状态
  g_nb_state.state = PROCESS_TCP_CL;
  
  //TCP 的执行流程是通过SUB_TCP_PROCESS数组指定AT指令流程
  g_nb_state.sub_state = 1;
  
  NB_SendCmd(hw_handle,&g_at_cmd);
  return SUCCESS;
}




//******************************************************************************
// fn : bc95_connectTCP
//
// brief : 连接TCP
//
// param : handle -> NB 结构信息指针
//
// return : FAIL -> 没有创建TCP,SUCCESS->指令执行成功
int bc95_connectTCP(NB_Handle handle)
{
  HWAttrs_Handle hw_handle = (HWAttrs_Handle)handle->object;
  
  if(g_nb_state.state != PROCESS_NONE)
  {
    return FAIL;
  }
 
  char  buf[NB_UART_SEND_BUF_MAX_LEN - 40];
  memset(buf,0,NB_UART_SEND_BUF_MAX_LEN - 40);
  
  uint16_t msg_len = snprintf(buf,NB_UART_SEND_BUF_MAX_LEN-40,"%s,%s,%s",
                                          g_bc95_status.nb95_tcp_id,
                                          REMOTE_SERVER_IP,
                                          REMOTE_SERVER_PORT);
  
  cmd_param_init(&g_at_cmd,AT_NSOCO,buf,CMD_SET);
  g_at_cmd.max_timeout = 2000;
  
  //更改NBiot操作进程，进入Connect TCP状态
  g_nb_state.state = PROCESS_TCP_CNT;
  g_nb_state.sub_state = 1;
  
  NB_SendCmd(hw_handle,&g_at_cmd);
  
  return SUCCESS;
}



//******************************************************************************
// fn : bc95_sendTCP
//
// brief : 发送TCP数据
//
// param : handle -> NB 结构信息指针
//         len -> 信息长度
//         msg -> 信息
//
// return : FAIL -> 没有创建TCP,SUCCESS->指令执行成功
int bc95_sendTCP(NB_Handle handle,int len,char* msg)
{
  HWAttrs_Handle hw_handle = (HWAttrs_Handle)handle->object;
  
  if(g_nb_state.state != PROCESS_NONE)
  {
    return FAIL;
  }
 
  uint16_t max_len = (NB_UART_SEND_BUF_MAX_LEN - 40) >> 1;
  uint16_t str_len = 0;
  
  char  buf[NB_UART_SEND_BUF_MAX_LEN - 40];
  memset(buf,0,NB_UART_SEND_BUF_MAX_LEN - 40);
  
  if(len > max_len)
  {
    str_len  = max_len;
  }
  else
  {
    str_len = len;
  }
	
	char dataLen[10] = {0};
  sprintf(dataLen,"%d",len);
	
  uint16_t msg_len = snprintf(buf,NB_UART_SEND_BUF_MAX_LEN-40,"%s,%s,",
                                          "1",dataLen);
  
  for(uint16_t i = 0 ; i < str_len ; i++)
  {
    sprintf(&buf[msg_len + (i << 1)],"%02X",(uint8)msg[i]);
  }
  
  cmd_param_init(&g_at_cmd,AT_NSOSD,buf,CMD_SET);
  g_at_cmd.max_timeout = 2000;
  
  //更改NBiot操作进程，进入Close TCP状态
  g_nb_state.state = PROCESS_TCP_ST;
  g_nb_state.sub_state = 1;
  
  NB_SendCmd(hw_handle,&g_at_cmd);
  
  return SUCCESS;
}
//******************************************************************************
// fn : bc95_receTCP
//
// brief : 接收TCP数据
//
// param : handle -> NB 结构信息指针
//
//
// return : FAIL -> 没有创建TCP,SUCCESS->指令执行成功
int bc95_receTCP(NB_Handle handle)
{
  HWAttrs_Handle hw_handle = (HWAttrs_Handle)handle->object;
  
  if(g_nb_state.state != PROCESS_NONE)
  {
    return FAIL;
  }
  uint16_t max_len = (NB_UART_SEND_BUF_MAX_LEN - 40) >> 1;
  uint16_t read_len = max_len;
  
  
  if(g_bc95_status.nb95_tcp_len.nb_data_len < max_len)
  {
    read_len = g_bc95_status.nb95_tcp_len.nb_data_len;
  }
  
  char buf[10] ;
  
  max_len = snprintf(buf,10,"%d,%d",g_bc95_status.nb95_tcp_len.nb_socket_id,
                         read_len);
  buf[max_len] = 0;
  
  cmd_param_init(&g_at_cmd,AT_NSORF,buf,CMD_SET);
  
  //更改NBiot操作进程，进入 TCP READ状态
  g_nb_state.state = PROCESS_TCP_RE;
  g_nb_state.sub_state = 1;
  
  NB_SendCmd(hw_handle,&g_at_cmd);
  return SUCCESS;
}

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
extern int bc95_coapServer(NB_Handle handle,Bool isSet,char* coap)
{
  HWAttrs_Handle hw_handle = (HWAttrs_Handle)handle->object;
  char* buf = null;
  if(g_nb_state.state != PROCESS_NONE)
  {
    return FAIL;
  }
  
  if(isSet)
  {
    if(coap == NULL)
    {
      buf = REMOTE_COAP_INFO;
    }
    else
    {
      buf = coap;
    }
    cmd_param_init(&g_at_cmd,AT_NCDP,buf,CMD_SET);
  }
  else
  {
    cmd_param_init(&g_at_cmd,AT_NCDP,"?",CMD_READ);
  }
  //更改NBiot操作进程，进入 UDP READ状态
  g_nb_state.state = PROCESS_COAP;
  g_nb_state.sub_state = 1;
  
  NB_SendCmd(hw_handle,&g_at_cmd);
  return SUCCESS;
}

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
extern int bc95_coapSentIndication(NB_Handle handle,int code)
{
  return SUCCESS;
}

//******************************************************************************
// fn : bc95_coapReceIndication
//
// brief : 设置CoAP接收信息提示方式
//
// param : handle -> NB 结构信息指针
//         code   -> 0  code -> 0表示缓存，1 表示直接接收。（目前仅支持两个模式）
//
// return : none
int bc95_coapReceIndication(NB_Handle handle,int code)
{
  return SUCCESS;
}

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
int bc95_coapSendMsg(NB_Handle handle,int len,char*msg)
{
  HWAttrs_Handle hw_handle = (HWAttrs_Handle)handle->object;
  
  if(g_nb_state.state != PROCESS_NONE)
  {
    return FAIL;
  }
  
  //判断SOCKET ID 是否正确
  
  uint16_t max_len = (NB_UART_SEND_BUF_MAX_LEN - 40) >> 1;
  uint16_t str_len = 0;
  
  char  buf[NB_UART_SEND_BUF_MAX_LEN - 40];
  memset(buf,0,NB_UART_SEND_BUF_MAX_LEN - 40);
  
  if(len > max_len)
  {
    str_len  = max_len;
  }
  else
  {
    str_len = len;
  }
  
  uint16_t msg_len = snprintf(buf,NB_UART_SEND_BUF_MAX_LEN - 40,"%d,",str_len);
  
  for(uint16_t i = 0 ; i < str_len ; i++)
  {
    sprintf(&buf[msg_len + (i << 1)],"%02X",(uint8)msg[i]);
  }
  
  cmd_param_init(&g_at_cmd,AT_NMGS,buf,CMD_SET);
  g_at_cmd.max_timeout = 2000;
  
  //更改NBiot操作进程，进入Close UDP状态
  g_nb_state.state = PROCESS_COAP_ST;
  g_nb_state.sub_state = 1;
  
  NB_SendCmd(hw_handle,&g_at_cmd);
  return SUCCESS;
}

//******************************************************************************
// fn : bc95_coapReadMsg
//
// brief : 读取coap接收到的消息
//
// param : handle -> NB 结构信息指针
//
// return : none
extern int bc95_coapReadMsg(NB_Handle handle)
{
  HWAttrs_Handle hw_handle = (HWAttrs_Handle)handle->object;
  
  if(g_nb_state.state != PROCESS_NONE)
  {
    return FAIL;
  }

  cmd_param_init(&g_at_cmd,AT_NMGR,NULL,CMD_EXCUTE);
  
  //更改NBiot操作进程，进入 UDP READ状态
  g_nb_state.state = PROCESS_COAP_RE;
  g_nb_state.sub_state = 1;
  
  NB_SendCmd(hw_handle,&g_at_cmd);
  
  return SUCCESS;
}
//******************************************************************************
// fn : bc95_error_handle
//
// brief : bc95 指令返回出错处理
//
// param : none
//
// return : FALSE -> 操作失败，TRUE -> 操作成功
Bool bc95_result_handle(NB_Handle handle ,Bool isOk)
{
  Bool isNext = FALSE;
  HWAttrs_Handle hw_handle = (HWAttrs_Handle)handle->object;
  
  if(isOk)
  {
     if(g_at_cmd.cmd_action == ACTION_OK_NEXT_ERROR_TRY)
     {
        isNext = TRUE;
     }
     else
     {
        //复位状态标志
        state_reset();
     }
  }
  else
  {
    if(g_at_cmd.cmd_action == ACTION_OK_NEXT_ERROR_TRY)
    {
      g_at_cmd.haveTried++;
      if(g_at_cmd.haveTried < g_at_cmd.cmd_try)
      {
        hw_handle->uart_fxnTable->sendFxn((uint8_t*)gNBSendBuf.Buf,gNBSendBuf.len);
        if(g_at_cmd.max_timeout)
        {
          hw_handle->timer_fxnTable->startTimerFxn(g_at_cmd.max_timeout);
        }
      }
      else
      {
        //通知上层应用，此动作执行失败
        nbsend_msg_app(handle,NULL,FALSE);
        //复位状态标志
        state_reset();
      }
    }
    else
    {
      //ACTION_OK_EXIT_ERROR_NEXT
      isNext = TRUE;
    }
  }
  return isNext;
}
//******************************************************************************
// fn : bc95_main
//
// brief : 处理BC95所有事务
//
// param : none
//
// return : FALSE -> 操作失败，TRUE -> 操作成功
int bc95_main(NB_Handle handle)
{
  HWAttrs_Handle hw_handle = (HWAttrs_Handle)handle->object;
  Bool isNext = FALSE;
  
  if(g_event_regTable & NB_SP_RECE_EVENT)
  {
    char* tmp_buf = gNBReceBuf.Buf;
    char* param[15];
    uint8_t index = 0;
    int8_t isPass;
    
    isPass = cmd_isPass(gNBReceBuf.Buf);
    
    if((isPass >= FALSE) && (isPass <= TRUE))
    {
      //指令执行完成，停止定时器
      if(g_at_cmd.lmt_period == FALSE || isPass == TRUE)
      {
         nbstop_timer(hw_handle);
      }
      
      //提取AT指令返回的参数，此时gNBReceBuf已经遭到破坏
      while(( param[index] = strtok(tmp_buf,"\r\n")) != null)
      {
        index++;
        tmp_buf = null;
        if(index >= 15)
        {
          break;
        }
      }
      
      if(index == 0)
      {
        //未收到正确的数据帧
        return FALSE;
      }
      
    }

    if(isPass == TRUE)
    {
      //ok
      nbsend_msg_app(handle,param,TRUE);
      isNext = bc95_result_handle(handle,TRUE);
      reset_rece_buf();
    }
    else if(isPass == FALSE)
    {
      //error
      if(g_at_cmd.lmt_period == FALSE)
      {
        isNext = bc95_result_handle(handle,FALSE);
      }
      reset_rece_buf();
    }
    else
    {
      //此指令还未执行完
      //do nothing
    }
    
    g_event_regTable ^= NB_SP_RECE_EVENT;
  }
  
  if(g_event_regTable & NB_TIMEOUT_EVENT)
  {
    reset_rece_buf();
    isNext = bc95_result_handle(handle,FALSE);
    
    g_event_regTable ^= NB_TIMEOUT_EVENT;
  }
  
  //发送UDP接收指令，将接收数据
  if(g_event_regTable & NB_UDPRECE_EVENT)
  {
    if(bc95_receUDP(handle) == SUCCESS)
    {
      g_event_regTable ^= NB_UDPRECE_EVENT;
    }
  }
	 //发送TCP接收指令，将接收数据
  if(g_event_regTable & NB_TCPRECE_EVENT)
  {
    if(bc95_receTCP(handle) == SUCCESS)
    {
      g_event_regTable ^= NB_TCPRECE_EVENT;
    }
  }
  //发送COAP接收指令，将接收数据
  if(g_event_regTable & NB_COAP_RE_EVENT)
  {
    if(bc95_coapReadMsg(handle) == SUCCESS)
    {
      g_event_regTable ^= NB_COAP_RE_EVENT;
    }
  }
  
  if(g_event_regTable & NB_REG_STA_EVENT)
  {
    //向app层回调网络注册成功
    handle->AppReceCB((msg_types_t)PROCESS_NET_REG,1,(char*)&g_bc95_status.nb95_register_status);
    g_event_regTable ^= NB_REG_STA_EVENT;
  }
  
  if(isNext)
  {
    if(cmd_next())
    {
      NB_SendCmd(hw_handle,&g_at_cmd);
    }
    else
    {
      nbsend_msg_app(handle,NULL,TRUE);
      
      //复位状态标志
      state_reset();
    }
  }
  return TRUE;
}

//******************************************************************************
// fn : nbsend_msg_app
//
// brief : 通过回调函数，向应用层发送指令参数
//
// param : 
//
// return : none
void nbsend_msg_app(NB_Handle handle, char**buf,Bool isOk)
{
  if(handle == NULL)
  {
    return;
  }
  
  //出错，则上报此流程执行失败
  if(isOk == FALSE)
  {
    handle->AppReceCB((msg_types_t)g_nb_state.state,1,"F");
    return;
  }
  
  if(g_nb_state.state == PROCESS_INIT)
  {
    switch(nb95_init_process[g_nb_state.sub_state])
    {
    case SUB_SYNC:
      break;
		#ifdef DABI_UDP
    case SUB_CMEE:
      break;
    case SUB_CFUN:
      break;
		case SUB_CGSN:
		{
			char* pColon = strchr(buf[0],':');
			if(pColon)
			{
				pColon++;
				memcpy(g_bc95_status.nb95_IMEI,pColon,15);
				g_bc95_status.nb95_IMEI[15] = 0;
				handle->AppReceCB((msg_types_t)TYPES_CGSN,15,(char*)g_bc95_status.nb95_IMEI);
			}
		}
		break;
		case SUB_CEREG:
			break;
		case SUB_CSCON:
			break;
		#endif
    case SUB_CIMI:
      {
        memcpy(g_bc95_status.nb95_IMSI,buf[0],15);
        g_bc95_status.nb95_IMSI[15] = 0;
        handle->AppReceCB((msg_types_t)TYPES_CIMI,strlen(buf[0]),buf[0]);
      }
      break;

    case SUB_CGATT:
      break;
    case SUB_CGATT_QUERY:
      break;
    case SUB_END:
      handle->AppReceCB((msg_types_t)PROCESS_INIT,1,"S");
      break;
    }
  }
  else if(g_nb_state.state == PROCESS_MODULE_INFO)
  {
    switch(nb95_info_process[g_nb_state.sub_state])
    {
    case SUB_CGMI:
      {
        handle->AppReceCB((msg_types_t)TYPES_CGMI,strlen(buf[0]),buf[0]);
      }
      break;
    case SUB_CGMM:
      {
        handle->AppReceCB((msg_types_t)TYPES_CGMM,strlen(buf[0]),buf[0]);
      }
      break;
    case SUB_CGMR:
      {
        char* pComma = strchr(buf[0],',');
        if(pComma)
        {
          pComma++;
          handle->AppReceCB((msg_types_t)TYPES_CGMR,strlen(pComma),pComma);
        }
      }
      break;
    case SUB_NBAND:
      {
        char* pColon = strchr(buf[0],':');
        char* pFreq = null;
        if(pColon)
        {
          pColon++;
          uint8_t hz_id = NB_Strtoul(pColon,10);
          if(hz_id == BAND_850MHZ_ID)
          {
            //850MHZ
            pFreq = BAND_850MHZ_STR;
          }
          else if(hz_id == BAND_900MHZ_ID)
          {
            //900MHZ
            pFreq = BAND_900MHZ_STR;
          }
          else if(hz_id == BAND_800MHZ_ID)
          {
            //800MHZ 
            pFreq = BAND_800MHZ_STR;
          }
          else 
          {
            //700MHZ
            pFreq = BAND_700MHZ_STR;
          }
          handle->AppReceCB((msg_types_t)TYPES_NBAND,strlen(pFreq),pFreq);
        }
      }
      break;

    case SUB_END:
      {
        handle->AppReceCB((msg_types_t)PROCESS_MODULE_INFO,1,"S");
      }
      break;
    }
  }
  else if(g_nb_state.state == PROCESS_SIGN)
  {
    if(g_nb_state.sub_state == 1)
    {
      char* pColon = strchr(buf[0],':');
      pColon++;
      uint8_t lqi = NB_Strtoul(pColon,10);
      int8_t rssi = -113 + (lqi << 1);
      uint8_t len = snprintf(buf[0],10,"%d",rssi);
      *(buf[0]+len) = 0;
      handle->AppReceCB((msg_types_t)PROCESS_SIGN,len,buf[0]);
    }

  }
  else if(g_nb_state.state == PROCESS_UDP_CR)
  {
    switch(nb95_udpcr_process[g_nb_state.sub_state])
    {
    case SUB_UDP_CR:
      {
        memcpy(g_bc95_status.nb95_udp_id,buf[0],2);
        
        if(g_nb_state.sub_state == 1)
        {
          handle->AppReceCB((msg_types_t)PROCESS_UDP_CR,1,"S");
        }
      }
      break;
    case SUB_UDP_CL:
      {
        
      }
      break;
    case SUB_END:
      {
        handle->AppReceCB((msg_types_t)PROCESS_UDP_CR,1,"S");
      }
      break;
    }
  }
  else if(g_nb_state.state == PROCESS_UDP_CL)
  {
    switch(nb95_udpcl_process[g_nb_state.sub_state])
    {
    case SUB_UDP_CL:
      {
        g_bc95_status.nb95_udp_id[0] = 0;
        g_bc95_status.nb95_udp_id[1] = 0;
      }
      break;
    case SUB_END:
      {
        handle->AppReceCB((msg_types_t)PROCESS_UDP_CL,1,"S");
      }
      break;
    } 

  }
  else if(g_nb_state.state == PROCESS_UDP_ST)
  {
    switch(nb95_udpst_process[g_nb_state.sub_state])
    {
    case SUB_UDP_ST:
      {
        handle->AppReceCB((msg_types_t)PROCESS_UDP_ST,1,"S");
      }
      break;
    }
  }
  else if(g_nb_state.state == PROCESS_UDP_RE)
  {
    if(g_nb_state.sub_state == 1)
    {
      //
      char* param[6];
      uint16_t index = 0;
      char* tmp_buf = buf[0];
      while(( param[index] = strtok(tmp_buf,",")) != null)
      {
        index++;
        tmp_buf = null;
        if(index >= 6)
        {
          break;
        }
      }
      if(index != 6)
      {
        handle->AppReceCB((msg_types_t)PROCESS_UDP_RE,1,"F");
        return;
      }
      
      tmp_buf = param[4];
      index =  NB_HexStrToNum(tmp_buf);
      
      handle->AppReceCB((msg_types_t)PROCESS_UDP_RE,index,tmp_buf);
    }
    
  }
	else if(g_nb_state.state == PROCESS_TCP_CR)
  {
    switch(nb95_tcpcr_process[g_nb_state.sub_state])
    {
    case SUB_TCP_CR:
      {
        memcpy(g_bc95_status.nb95_tcp_id,buf[0],2);
        
        if(g_nb_state.sub_state == 1)
        {
          handle->AppReceCB((msg_types_t)PROCESS_TCP_CR,1,"S");
        }
      }
      break;
		 case SUB_TCP_CNT:
      {
        memcpy(g_bc95_status.nb95_tcp_id,buf[0],2);
        
        if(g_nb_state.sub_state == 1)
        {
          handle->AppReceCB((msg_types_t)PROCESS_TCP_CNT,1,"S");
        }
      }
      break;	
    case SUB_TCP_CL:
      {
        
      }
      break;
    case SUB_END:
      {
        handle->AppReceCB((msg_types_t)PROCESS_TCP_CR,1,"S");
      }
      break;
    }
  }
  else if(g_nb_state.state == PROCESS_TCP_CL)
  {
    switch(nb95_tcpcl_process[g_nb_state.sub_state])
    {
    case SUB_TCP_CL:
      {
        g_bc95_status.nb95_tcp_id[0] = 0;
        g_bc95_status.nb95_tcp_id[1] = 0;
      }
      break;
    case SUB_END:
      {
        handle->AppReceCB((msg_types_t)PROCESS_TCP_CL,1,"S");
      }
      break;
    } 

  }
	else if(g_nb_state.state == PROCESS_TCP_CNT)
  {
    switch(nb95_tcpst_process[g_nb_state.sub_state])
    {
    case SUB_TCP_CNT:
      {
        handle->AppReceCB((msg_types_t)PROCESS_TCP_CL,1,"S");
      }
      break;
    }
  }
  else if(g_nb_state.state == PROCESS_TCP_ST)
  {
    switch(nb95_tcpst_process[g_nb_state.sub_state])
    {
    case SUB_TCP_ST:
      {
        handle->AppReceCB((msg_types_t)PROCESS_TCP_CL,1,"S");
      }
      break;
    }
  }
  else if(g_nb_state.state == PROCESS_TCP_RE)
  {
    if(g_nb_state.sub_state == 1)
    {
      //
      char* param[6];
      uint16_t index = 0;
      char* tmp_buf = buf[0];
      while(( param[index] = strtok(tmp_buf,",")) != null)
      {
        index++;
        tmp_buf = null;
        if(index >= 6)
        {
          break;
        }
      }
      if(index != 6)
      {
        handle->AppReceCB((msg_types_t)PROCESS_TCP_RE,1,"F");
        return;
      }
      
      tmp_buf = param[4];
      index =  NB_HexStrToNum(tmp_buf);
      
      handle->AppReceCB((msg_types_t)PROCESS_TCP_RE,index,tmp_buf);
    }
    
  }
  else if(g_nb_state.state == PROCESS_COAP)
  {
    if(g_nb_state.sub_state == 1)
    {
      char* tmp_buf = NULL;
      if(strstr(buf[0],"OK"))
      {
        tmp_buf = "S";
      }
      else
      {
        tmp_buf = strchr(buf[1],':');
        if(tmp_buf)
        {
          tmp_buf++;
        }
      }
      
      handle->AppReceCB((msg_types_t)PROCESS_COAP,strlen(tmp_buf),tmp_buf);
    }
  }
  else if(g_nb_state.state == PROCESS_COAP_ST)
  {
    if(g_nb_state.sub_state == 1)
    {
      handle->AppReceCB((msg_types_t)PROCESS_COAP_ST,1,"S");
    }
  }
  else if(g_nb_state.state == PROCESS_COAP_RE)
  {
    if(g_nb_state.sub_state == 1)
    {
      uint16_t index = 0;
      char* tmp_buf = NULL;
     
      tmp_buf = strchr(buf[0],',');
      if(tmp_buf)
      {
        tmp_buf++;
        index =  NB_HexStrToNum(tmp_buf);
        handle->AppReceCB((msg_types_t)PROCESS_COAP_RE,index,tmp_buf); 
      }
   
    }
  }
    
}
//******************************************************************************
// fn : NB_Strtoul
//
// brief : 将字符型数字转成整型
//
// param : pStr -> 要转换内容地址
//         base -> 转换内容以多少进制进行转换（2-36）
//
// return : 整形数据
uint32_t NB_Strtoul(const char* pStr,int base)
{
  return strtoul(pStr,0,base);
}

//******************************************************************************
// fn : NB_HexStrToNUM
//
// brief : 将十六进制字符串转成数字
//
// param : 
//         str -> 十六进制字符串内容
//
// return : 地址
uint16_t NB_HexStrToNum(char* str)
{
  uint16_t i = 0;
  uint8_t tmp = 0;
  uint8_t tmp1 = 0;
  
  uint16_t len = strlen(str);
  if(len == 0)
  {
    return 0;
  }
 
  for( i = 0 ; i < len ; i++)
  {
    if(str[i] >= '0' && str[i] <= '9')
    {
      tmp1 = str[i] - '0';
    }
    else if(str[i] >= 'A' && str[i] <= 'F')
    {
     tmp1 = str[i] - 'A' + 10; 
    }
    else if(str[i] >= 'a' && str[i] <= 'f')
    {
      tmp1 = str[i] - 'a' + 10;
    }
    else
    {
      tmp1 = 0;
    }
    
    if((i%2) == 0)
    {
      tmp = tmp1; 
    }
    else
    {
      tmp <<= 4;
      tmp += tmp1;
      str[i>>1] = tmp;
    }
    
  }
  
  str[i >> 1] = 0;
  
  return (i >> 1);
}