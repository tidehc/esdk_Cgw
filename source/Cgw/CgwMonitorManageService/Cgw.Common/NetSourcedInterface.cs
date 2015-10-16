/*-----------------------------------------------------------------------
//文件名：NetSourcedInterface.cs
//版权：Copyright 2011-2012 Huawei Tech. Co. Ltd. All Rights Reserved. 
//作者：w00206574
//日期：2014-3-06
//描述：用于适配底层NetSourced.DLL提供的接口，用户从某IP、Port获取实况RTP数据包
//---------------------------------------------------------------------*/
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;

namespace CgwMonitorManage.Common
{
    /// <summary>
    /// T28181实况回调，获取得到实况的RTP包
    /// </summary>
    /// <param name="pEventBuf">RTP字节数组包</param>
    /// <param name="uiSize">RTP包的大小</param>
    /// <param name="pUser">用户数据</param>
    public delegate void NET_DATA_CALLBACK(IntPtr pEventBuf, UInt32 uiSize, IntPtr pUser);

    public enum IVS_NETSOURCE_RESULT
    {
        SUCCESS = 0,	//成功
        FAILURE = 1,	//失败
    }
    /// <summary>
    /// 异常回调函数
    /// </summary>
    /// <param name="ulChannel">通道号</param>
    /// <param name="iMsgType">异常消息类型</param>
    /// <param name="pParam">对应异常的信息，可扩展为结构体指针</param>
    /// <param name="pUser">用户数据</param>
    public delegate void NET_EXCEPTION_CALLBACK(UInt32  ulChannel, UInt32  iMsgType,IntPtr  pParam, IntPtr  pUser); 

    public class NetSourcedInterface
    {
        /*****************************************************************************
         函数名称：IVS_NETSOURCE_Init
         功能描述：初始化;
         输入参数：const char* pLogPath = NULL
         输出参数：NA
         返 回 值：成功:IVS_SUCCEED;
                   失败:IVS_FAIL,PLAYER_RET_WSA_START_ERROR;
        *****************************************************************************/
        [DllImport(@".\SipStack\IVS_NetSource.dll", CallingConvention = CallingConvention.StdCall)]
        public extern static IVS_NETSOURCE_RESULT IVS_NETSOURCE_Init(string pLogPath);

        /*****************************************************************************
         函数名称：IVS_NETSOURCE_UnInit
         功能描述：清空收流库;
         输入参数：NA
         输出参数：NA
         返 回 值：成功:IVS_SUCCEED 失败:IVS_FAIL;
        *****************************************************************************/
        [DllImport(@".\SipStack\IVS_NetSource.dll", CallingConvention = CallingConvention.StdCall)]
        //public extern static IVS_NETSOURCE_RESULT IVS_NETSOURCE_UnInit();
        public extern static void IVS_NETSOURCE_UnInit();

        /*****************************************************************************
         函数名称：IVS_NETSOURCE_GetChannel
         功能描述：获取可用通道;
         输入参数：NA
         输出参数：pChannel 
         返 回 值：成功:IVS_SUCCEED 失败:IVS_FAIL;
        *****************************************************************************/
        [DllImport(@".\SipStack\IVS_NetSource.dll", CallingConvention = CallingConvention.StdCall)]
        public extern static IVS_NETSOURCE_RESULT IVS_NETSOURCE_GetChannel(ref UInt32 pChannel);

        /*****************************************************************************
         函数名称：IVS_NETSOURCE_FreeChannel
         功能描述：释放指定通道;
         输入参数：ulChannel 
         输出参数：NA
         返 回 值：成功:IVS_SUCCEED 失败:IVS_FAIL;
        *****************************************************************************/
        [DllImport(@".\SipStack\IVS_NetSource.dll", CallingConvention = CallingConvention.StdCall)]
        public extern static IVS_NETSOURCE_RESULT IVS_NETSOURCE_FreeChannel(UInt32 pChannel);

        /*****************************************************************************
         函数名称：IVS_NETSOURCE_SetProtocolType
         功能描述：设置媒体流传输协议类型;
         输入参数：ulChannel        通道号
                   uiProtocolType 协议类型;
         输出参数：NA
         返 回 值：成功:IVS_SUCCEED 失败:IVS_FAIL;
        *****************************************************************************/
        [DllImport(@".\SipStack\IVS_NetSource.dll", CallingConvention = CallingConvention.StdCall)]
        public extern static IVS_NETSOURCE_RESULT IVS_NETSOURCE_SetProtocolType(UInt32 ulChannel, uint uiProtocolType);

        /*****************************************************************************
         函数名称：IVS_NETSOURCE_SetLocalRecvStreamIP
         功能描述：设置本地接收码流的Ip地址;
         输入参数：ulChannel  通道号;   
                   pLocalRecvStreamIp  接收码流的ip;
         输出参数：NA
         返 回 值：成功:IVS_SUCCEED 失败:IVS_FAIL;
        *****************************************************************************/
        [DllImport(@".\SipStack\IVS_NetSource.dll", CallingConvention = CallingConvention.StdCall)]
         public extern static IVS_NETSOURCE_RESULT IVS_NETSOURCE_SetLocalRecvStreamIP(UInt32 ulChannel, string pLocalRecvStreamIp);

        /*****************************************************************************
         函数名称：IVS_NETSOURCE_GetLocalRecvStreamAddr
         功能描述：获取本地接收码流的地址IP和端口号;
         输入参数：ulChannel 通道号;
         输出参数：pLocalRecvStreamVideoRtpPort 接收视频码流的RTP端口号;
                   pLocalRecvStreamAudioRtpPort 接收音频码流的RTP端口号;
         返 回 值：成功:IVS_SUCCEED 失败:IVS_FAIL;
        *****************************************************************************/
        [DllImport(@".\SipStack\IVS_NetSource.dll", CallingConvention = CallingConvention.StdCall)]
         public extern static IVS_NETSOURCE_RESULT IVS_NETSOURCE_GetLocalRecvStreamPort(UInt32 ulChannel, ref uint pLocalRecvStreamVideoRtpPort, ref uint pLocalRecvStreamAudioRtpPort);

        /*****************************************************************************
         函数名称：IVS_NETSOURCE_OpenNetStream
         功能描述：打开网络流;
         输入参数：ulChannel                         通道号;
		           pRemoteSendStreamIp               源端发送码流IP;
		           uiRemoteSendStreamVideoPort       源端发送视频码流RTP端口号;
                   uiRemoteSendStreamAudioPort       源端发送音频码流RTP端口号;
         输出参数：NA
         返 回 值：成功:IVS_SUCCEED 失败:IVS_FAIL;
         *****************************************************************************/
        [DllImport(@".\SipStack\IVS_NetSource.dll", CallingConvention = CallingConvention.StdCall)]
         public extern static IVS_NETSOURCE_RESULT IVS_NETSOURCE_OpenNetStream(UInt32 ulChannel, string pRemoteSendStreamIp, uint uiRemoteSendStreamVideoPort, uint uiRemoteSendStreamAudioPort);

        /***************************************************************************
         函 数 名 : IVS_NETSOURCE_SetDataCallBack
         功能描述 : 设置媒体流回调函数;
         输入参数 : ulChannel          通道号;
                    cbDataCallBack    媒体流回调函数;
                    pUser             用户参数，回调函数中返回;
         输出参数 : NA
         返 回 值 : NA
        ***************************************************************************/
        [DllImport(@".\SipStack\IVS_NetSource.dll", CallingConvention = CallingConvention.StdCall)]
        public extern static void IVS_NETSOURCE_SetDataCallBack(UInt32 ulChannel, NET_DATA_CALLBACK cbDataCallBack,IntPtr pUser);

        /*****************************************************************************
         函数名称：IVS_NETSOURCE_StartRecvStream
         功能描述：启动收流;
         输入参数：ulChannel             通道号;
         输出参数：NA
         返 回 值：成功:IVS_SUCCEED 失败:IVS_FAIL;
        *****************************************************************************/
        [DllImport(@".\SipStack\IVS_NetSource.dll", CallingConvention = CallingConvention.StdCall)]
        public extern static IVS_NETSOURCE_RESULT IVS_NETSOURCE_StartRecvStream(UInt32 ulChannel);

        /*****************************************************************************
         函数名称：IVS_NETSOURCE_CloseNetStream
         功能描述：关闭网络流;
         输入参数：ulChannel             通道号;
         输出参数：NA
         返 回 值：成功:IVS_SUCCEED 失败:IVS_FAIL;
        *****************************************************************************/
        [DllImport(@".\SipStack\IVS_NetSource.dll", CallingConvention = CallingConvention.StdCall)]
         public extern static IVS_NETSOURCE_RESULT IVS_NETSOURCE_CloseNetStream(UInt32 ulChannel);

         /*****************************************************************************
         函数名称：IVS_NETSOURCE_CloseNetStream
         功能描述：关闭网络流;
         输入参数：ulChannel             通道号;
         输出参数：NA
         返 回 值：成功:IVS_SUCCEED 失败:IVS_FAIL;
        *****************************************************************************/
        [DllImport(@".\SipStack\IVS_NetSource.dll", CallingConvention = CallingConvention.StdCall)]
         public extern static IVS_NETSOURCE_RESULT IVS_NETSOURCE_SetExceptionCallBack(NET_EXCEPTION_CALLBACK cbExceptionCallBack, IntPtr pUser);


        /********************************
         函数名称：IVS_NETSOURCE_SetLocalRecvStreamAddr
         功能描述：设置本地接收码流的地址IP和端口号;
         输入参数：ulChannel 通道号;
		           pLocalRecvStreamVideoRtpPort 接收视频码流的RTP端口号;
                   pLocalRecvStreamAudioRtpPort 接收音频码流的RTP端口号;
         输出参数：NULL
         返 回 值：成功:IVS_SUCCEED 失败:IVS_FAIL;
        *****************************************************************************/
        [DllImport(@".\SipStack\IVS_NetSource.dll", CallingConvention = CallingConvention.StdCall)]
        public extern static IVS_NETSOURCE_RESULT IVS_NETSOURCE_SetLocalRecvStreamPort(UInt32 ulChannel,
                                                                           uint pLocalRecvStreamVideoRtpPort,
                                                                           uint pLocalRecvStreamAudioRtpPort);
    }
}
