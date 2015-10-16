/*-----------------------------------------------------------------------
//文件名：SdkRtpInterface.cs
//版权：Copyright 2011-2012 Huawei Tech. Co. Ltd. All Rights Reserved. 
//作者：w00206574
//日期：2014-3-06
//描述：用于rtp包数据转码
//---------------------------------------------------------------------*/
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;

namespace CgwMonitorManage.Common
{
    //回调的帧数据类型
    [StructLayout(LayoutKind.Sequential)]
    public struct ST_FRAME_DATA
    {
        public int iStreamType;		//流类型
        public int iFrameDataType;	//帧类型
    };

    /// <summary>
    /// rtp包转码完成后返回帧数据的回调函数
    /// </summary>
    /// <param name="pBuf">帧数据字节数组</param>
    /// <param name="uiBufSize">数组大小</param>
    /// <param name="pFrameData">帧数据类型</param>
    /// <param name="uiChannel">通道</param>
    public delegate void FrameDataCallBack(IntPtr pBuf, uint uiBufSize,ref ST_FRAME_DATA pFrameData, uint uiChannel);

    public class SdkRtpInterface
    {
        [DllImport(@".\SipStack\rtpconvertsdk.dll", CallingConvention = CallingConvention.StdCall)]
        public extern static int ESDK_RTP_Init();

        [DllImport(@".\SipStack\rtpconvertsdk.dll", CallingConvention = CallingConvention.StdCall)]
        public extern static int ESDK_RTP_OpenChannel(FrameDataCallBack frameDataCallBack, uint uiChannel);

        [DllImport(@".\SipStack\rtpconvertsdk.dll", CallingConvention = CallingConvention.StdCall)]
        public extern static int ESDK_RTP_ProcessPacket(IntPtr pBuf, uint uiBufSize, uint uiChannel);

        [DllImport(@".\SipStack\rtpconvertsdk.dll", CallingConvention = CallingConvention.StdCall)]
        public extern static int ESDK_RTP_OpenESChannel(FrameDataCallBack frameDataCallBack, uint uiChannel);

        [DllImport(@".\SipStack\rtpconvertsdk.dll", CallingConvention = CallingConvention.StdCall)]
        public extern static int ESDK_RTP_SetIsNeedAudioFrame(bool bIsNeedAudioFrame, uint uiChannel);

        [DllImport(@".\SipStack\rtpconvertsdk.dll", CallingConvention = CallingConvention.StdCall)]
        public extern static int ESDK_RTP_CloseChannel(uint uiChannel);

        [DllImport(@".\SipStack\rtpconvertsdk.dll", CallingConvention = CallingConvention.StdCall)]
        public extern static int ESDK_RTP_UnInit();
    }
}
