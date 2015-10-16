/*-----------------------------------------------------------------------
//文件名：RtpAdapter.cs
//版权：Copyright 2011-2012 Huawei Tech. Co. Ltd. All Rights Reserved. 
//作者：w00206574
//日期：2014-3-06
//描述：T28181调用rtp DLL解析rtp数据包
//---------------------------------------------------------------------*/
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace CgwMonitorManage.Common
{
    public class RtpAdapter
    {
        /// <summary>
        /// 日志
        /// </summary>
        private static NLog.Logger log = NLog.LogManager.GetCurrentClassLogger();

        private bool isInited = false;
        /// <summary>
        /// 初始化rtp模块
        /// </summary>
        public void ESDK_RTP_Init()
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Trace("Enter: RtpAdapter.ESDK_RTP_Init().");

            try
            {
                int iRet = 0;
                if (!isInited)
                {
                    iRet = SdkRtpInterface.ESDK_RTP_Init();
                    logEx.Trace("Enter: RtpAdapter.ESDK_RTP_Init(). This is first init!");
                }
                if (iRet != 0)
                {
                    logEx.Error("RtpAdapter.ESDK_RTP_Init ,error:{0}", iRet);
                    isInited = false;
                }
                else
                {
                    logEx.Trace("Enter: RtpAdapter.ESDK_RTP_Init().Success");
                    isInited = true;
                }
                logEx.Trace("Enter: RtpAdapter.ESDK_RTP_Init(). inited is {0}!", isInited);
            }
            catch (System.Exception ex)
            {
                logEx.Error("RtpAdapter.ESDK_RTP_Init ,Exception:{0}", ex.ToString());
                isInited = false;
            }
        }

        /// <summary>
        /// 卸载rtp模块
        /// </summary>
        public int ESDK_RTP_UnInit()
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Trace("Enter: RtpAdapter.ESDK_RTP_UnInit().");

            try
            {
                int iRet = SdkRtpInterface.ESDK_RTP_UnInit();

                if (iRet != 0)
                {
                    logEx.Error("RtpAdapter.ESDK_RTP_UnInit ,error:{0}", iRet);
                }
                return iRet;
            }
            catch (System.Exception ex)
            {
                logEx.Error("RtpAdapter.ESDK_RTP_UnInit ,Exception:{0}", ex.ToString());
            }
            return -1;
        }

        /// <summary>
        /// 打开rtp通道
        /// </summary>
        /// <param name="frameDataCallBack">回调函数,返回解析完成后的码流数据</param>
        /// <param name="uiChannel"></param>
        public void ESDK_RTP_OpenChannel(FrameDataCallBack frameDataCallBack, uint uiChannel)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Trace("Enter: RtpAdapter.ESDK_RTP_OpenChannel().");

            try
            {
                int iRet = SdkRtpInterface.ESDK_RTP_OpenChannel(frameDataCallBack, uiChannel);

                if (iRet != 0)
                {
                    logEx.Error("RtpAdapter.ESDK_RTP_OpenChannel ,error:{0}", iRet);
                }
            }
            catch (System.Exception ex)
            {
                logEx.Error("RtpAdapter.ESDK_RTP_OpenChannel ,Exception:{0}", ex.ToString());
            }
        }

        /// <summary>
        /// 解析rtp包
        /// </summary>
        /// <param name="pBuf"></param>
        /// <param name="uiBufSize"></param>
        /// <param name="uiChannel"></param>
        public void ESDK_RTP_ProcessPacket(IntPtr pBuf, uint uiBufSize, uint uiChannel)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);

            try
            {
                int iRet = SdkRtpInterface.ESDK_RTP_ProcessPacket(pBuf, uiBufSize, uiChannel);

                if (iRet != 0)
                {
                    logEx.Error("RtpAdapter.ESDK_RTP_ProcessPacket failed. uiBufSize ={0},uiChannel ={1} ", uiBufSize, uiChannel);
                }
            }
            catch (System.Exception ex)
            {
                logEx.Error("RtpAdapter.ESDK_RTP_ProcessPacket ,Exception:{0},uiBufSize ={1},uiChannel ={2} ", ex.ToString(), uiBufSize, uiChannel);
            }
        }

        /// <summary>
        /// 解析rtp包
        /// </summary>
        /// <param name="pBuf"></param>
        /// <param name="uiBufSize"></param>
        /// <param name="uiChannel"></param>
        public void ESDK_RTP_OpenESChannel(FrameDataCallBack frameDataCallBack, uint uiChannel)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);

            try
            {
                int iRet = SdkRtpInterface.ESDK_RTP_OpenESChannel(frameDataCallBack, uiChannel);

                if (iRet != 0)
                {
                    logEx.Error("RtpAdapter.ESDK_RTP_OpenESChannel failed.uiChannel ={0} ", uiChannel);
                }
            }
            catch (System.Exception ex)
            {
                logEx.Error("RtpAdapter.ESDK_RTP_OpenESChannel ,Exception:{0},uiChannel ={1} ", ex.ToString(), uiChannel);
            }
        }

        /// <summary>
        /// 设置音频数据
        /// </summary>
        /// <param name="bIsNeedAudioFrame"></param>
        /// <param name="uiChannel"></param>
        public void ESDK_RTP_SetIsNeedAudioFrame(bool bIsNeedAudioFrame, uint uiChannel)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Trace("Enter: RtpAdapter.ESDK_RTP_SetIsNeedAudioFrame().");

            try
            {
                int iRet = SdkRtpInterface.ESDK_RTP_SetIsNeedAudioFrame(bIsNeedAudioFrame, uiChannel);

                if (iRet != 0)
                {
                    logEx.Error("RtpAdapter.ESDK_RTP_SetIsNeedAudioFrame ,error:{0}", iRet);
                }
            }
            catch (System.Exception ex)
            {
                logEx.Error("RtpAdapter.ESDK_RTP_SetIsNeedAudioFrame ,Exception:{0}", ex.ToString());
            }
        }

        /// <summary>
        /// 关闭通道
        /// </summary>
        /// <param name="uiChannel"></param>
        public void ESDK_RTP_CloseChannel(uint uiChannel)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Trace("Enter: RtpAdapter.ESDK_RTP_CloseChannel().");

            try
            {
                int iRet = SdkRtpInterface.ESDK_RTP_CloseChannel(uiChannel);

                if (iRet != 0)
                {
                    logEx.Error("RtpAdapter.ESDK_RTP_CloseChannel ,error:{0}", iRet);
                }
            }
            catch (System.Exception ex)
            {
                logEx.Error("RtpAdapter.ESDK_RTP_CloseChannel ,Exception:{0}", ex.ToString());
            }
        }
    }
}
