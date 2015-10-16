/*-----------------------------------------------------------------------
//文件名：TiandyVideoMonitor.cs
//版权：Copyright 2011-2012 Huawei Tech. Co. Ltd. All Rights Reserved. 
//作者：w00206574
//日期：2014-3-13
//描述：Sip 接口
//---------------------------------------------------------------------*/
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;

namespace CgwMonitorManage.T28181
{
    //函数返回结果
    public enum EM_SIP_RESULT
    {
        RET_SUCCESS = 0,	//成功
        RET_FAILURE = 1,	//失败
        RET_NOT_INIT = 2,	//未初始化
        RET_HAS_INIT = 3,	//已经初始化
        RET_NOT_REGISTER = 4,	//未注册
        RET_INVALID_PARAM = 5	//参数错误
    };

    //收到的消息类型
    public enum EM_SIP_MSG_TYPE
    {
        SIP_REQUEST_MSG = 0,	//收到的请求消息
        SIP_RESPONSE_MSG = 1	//收到的响应消息
    };

    //请求消息的类型
    public enum EM_SIP_REQUEST_EVENT
    {
        SIP_REQ_REGISTER = 0,	//注册请求事件
        SIP_REQ_INVITE = 1,	//邀请请求事件
        SIP_REQ_MESSAGE = 2	//消息请求事件
    };

    //响应消息的类型
    public enum EM_SIP_RESPONSE_EVENT
    {
        SIP_RSP_SUCCESSS = 200,	//响应成功事件
        SIP_RSP_NOT_REG = 403		//注册失败响应事件
    };

    /// <summary>
    /// 消息通知回调函数结果
    /// </summary>
    /// <param name="msgType"></param>
    /// <param name="eventType"></param>
    /// <param name="pPara"></param>
    /// <returns></returns>
    public delegate EM_SIP_RESULT SIP_CallBack(int msgType, int eventType, StringBuilder pPara);

    public class SipSDKInterface
    {

        /**
	     *初始化
	     * 
	     *该函数用于初始化SIP协议栈
	     *	 
	     *@param[in]	pLocalSipAccount	本端sip鉴权用户名	 
	     *@param[in]	pLocalSipPasswd		本端sip鉴权密码
	     *@param[in]	pLocalID		本地ID	
	     *@param[in]	pLocalIP		本地IP
	     *@param[in]	iLocalPort		本地端口
	     *@param[in]	pServerSipAccount	对端sip鉴权用户名	 
	     *@param[in]	pServerSipPasswd	对端sip鉴权密码
	     *@param[in]	pServerID		对方ID
	     *@param[in]	pServerIP		对方IP
	     *@param[in]	iServerPort		对方端口
	     *@param[in]	pCallBackFunc	回调函数
	     *@return		0	成功
	     *@return		非0	失败（参考错误返回码）
	     *@attention	最先开始调用的函数,pSipAccount和pSipPasswd可以为空字符串，其他不允许为空
	     *@par			无
	    **/
        [DllImport(@".\SipStack\sipsdk.dll", CallingConvention = CallingConvention.StdCall)]
        public extern static EM_SIP_RESULT SIP_SDK_Init(string pLocalSipAccount, string pLocalSipPasswd, string pLocalID, string pLocalIP, int iLocalPort, string pServerSipAccount, string pServerSipPasswd, string pServerID, string pServerIP, int iServerPort, SIP_CallBack pCallBackFunc);

        /**
         *去初始化
         * 
         *该函数用于去初始化SIP协议栈
         *
         *@return		0	成功
         *@return		非0	失败（参考错误返回码）
         *@attention	
         *@par			无
        **/
        [DllImport(@".\SipStack\sipsdk.dll", CallingConvention = CallingConvention.StdCall)]
        public extern static EM_SIP_RESULT SIP_SDK_UnInit();

        /**
         *SIP注册
         * 
         *该函数用于发送SIP Register消息
         *
         *@return		0	成功
         *@return		非0	失败（参考错误返回码）
         *@attention	
         *@par			无
        **/
        [DllImport(@".\SipStack\sipsdk.dll", CallingConvention = CallingConvention.StdCall)]
        public extern static EM_SIP_RESULT SIP_SDK_REGISTER();

        /**
         *SIP注注销
         * 
         *该函数用于发送SIP Register消息，有效期为0
         *
         *@return		0	成功
         *@return		非0	失败（参考错误返回码）
         *@attention	
         *@par			无
        **/
        [DllImport(@".\SipStack\sipsdk.dll", CallingConvention = CallingConvention.StdCall)]
        public extern static EM_SIP_RESULT SIP_SDK_UNREGISTER();

        /**
         *SIP Invite
         * 
         *该函数用于发送SIP Invite消息
         *
         *@param[in]	pAccount		对方账号
         *@param[in]	pSdpBody		SDP描述字符串
         *@return		0	成功
         *@return		非0	失败（参考错误返回码）
         *@attention	
         *@par			无
        **/
        [DllImport(@".\SipStack\sipsdk.dll", CallingConvention = CallingConvention.StdCall)]
        public extern static EM_SIP_RESULT SIP_SDK_INVITE(string pAccount, string pSdpBody, ref int iResponseID, StringBuilder strRemoteBody);

        /**
         *SIP Message
         * 
         *@param[in]	pAccount		对方账号
         *@param[in]	pBody			发送的xml字段
         *该函数用于发送SIP Message消息
         *
         *@return		0	成功
         *@return		非0	失败（参考错误返回码）
         *@attention	
         *@par			无
        **/
        [DllImport(@".\SipStack\sipsdk.dll", CallingConvention = CallingConvention.StdCall)]
        public extern static EM_SIP_RESULT SIP_SDK_MESSAGE(string pDeviceID, string pBody);

        /**
         *SIP ACK
         * 
         *该函数用于发送SIP ACK消息
         *
         *@return		0	成功
         *@return		非0	失败（参考错误返回码）
         *@attention	
         *@par			无
        **/
        [DllImport(@".\SipStack\sipsdk.dll", CallingConvention = CallingConvention.StdCall)]
        public extern static EM_SIP_RESULT SIP_SDK_ACK();

        /**
         *SIP BYE
         * 
         *该函数用于发送SIP BYE消息
         *
         *@return		0	成功
         *@return		非0	失败（参考错误返回码）
         *@attention	
         *@par			无
        **/
        [DllImport(@".\SipStack\sipsdk.dll", CallingConvention = CallingConvention.StdCall)]
        public extern static EM_SIP_RESULT SIP_SDK_BYE(int responseID);

        /**
         *订阅
         * 
         *该函数用于发送subscribe消息
         *
         *@param[in]	pBody			body体
         *@return		0	成功
         *@return		非0	失败（参考错误返回码）
         *@attention	最先开始调用的函数
         *@par			无
        **/
        [DllImport(@".\SipStack\sipsdk.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        public extern static EM_SIP_RESULT SIP_SDK_Subscribe(string pBody);

        /**
        *该函数用于发送SIP ACK消息
        *
        *@param[in]	iResponseID		Invite成功后的响应ID
        *@return		0	成功
        *@return		非0	失败（参考错误返回码）
        *@attention	调用者需要传入SIP_SDK_INVITE返回的值
        *@par			无
       **/
        [DllImport(@".\SipStack\sipsdk.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        public extern static EM_SIP_RESULT SIP_SDK_ACK(int iResponseID);
    }
}
