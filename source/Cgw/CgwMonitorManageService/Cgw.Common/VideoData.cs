/*-----------------------------------------------------------------------
//文件名：VideoData.cs
//版权：Copyright 2011-2012 Huawei Tech. Co. Ltd. All Rights Reserved. 
//作者：kf70422
//日期：2013-2-27
//描述：码流信息
//---------------------------------------------------------------------*/
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace CgwMonitorManage.Common
{
    /// <summary>
    /// 码流数据结构
    /// </summary>
    [Serializable]
    public class MediaData
    {
        /// <summary>
        /// 有效码流大小
        /// </summary>
        public uint Size { get; set; }

        /// <summary>
        /// 数据传输的类型
        /// </summary>
        public StreamType StreamType { get; set; }

        /// <summary>
        /// 码流类型
        /// </summary>
        public MediaDataType DataType { get; set; }

        /// <summary>
        /// 帧类型
        /// </summary>
        public FrameDataType FrameType { get; set; }

        /// <summary>
        /// 码流
        /// </summary>
        public byte[] Data { get; set; }
    }

    /// <summary>
    /// 码流类型
    /// </summary>
    public enum MediaDataType
    {
        RTP_DATA = 1,                       /* RTP封包后的码流 */
        FRAME_DATA = 2,                     /* 帧数据 */
    }

    /// <summary>
    /// 传输的数据类型
    /// </summary>
    public enum StreamType
    {
        UNDEFINED = 0,
        VIDEO_H264 = 1,                // 视频数据

        AUDIO_G711A = 10,              // G711A
        AUDIO_G711U = 11               //G711U
    }

    /// <summary>
    /// 帧数据类型
    /// </summary>
    public enum FrameDataType
    {
        H264_NALU_TYPE_UNDEFINED = 0,
        H264_NALU_TYPE_SLICE = 1,
        H264_NALU_TYPE_DPA = 2,
        H264_NALU_TYPE_DPB = 3,
        H264_NALU_TYPE_DPC = 4,
        H264_IDR_NALU_TYPE = 5,
        H264_SEI_NALU_TYPE = 6,
        H264_SPS_NALU_TYPE = 7,
        H264_PPS_NALU_TYPE = 8,
        H264_NALU_TYPE_AUD = 9,
        H264_NALU_TYPE_EOSEQ = 10,
        H264_NALU_TYPE_EOSTREAM = 11,
        H264_NALU_TYPE_FILL = 12,

        H264_NALU_TYPE_STAP_A = 24,
        H264_NALU_TYPE_STAP_B = 25,
        H264_NALU_TYPE_MTAP16 = 26,
        H264_NALU_TYPE_MTAP24 = 27,
        H264_NALU_TYPE_FU_A = 28,
        H264_NALU_TYPE_FU_B = 29,
        H264_NALU_TYPE_END = 30
    }

    public enum SotErrorType
    {
        InitStackErr,
        RtpSessionCreateErr
    }

    public delegate uint NLogCallback(NLogLevel level, string log, int len);
    public delegate uint IFrameCallback(string label);
    public delegate void SotErrorReport(string label, SotErrorType errType);

    public class SoftTerminalCallback
    {
        public NLogCallback NlogHandler;
        public IFrameCallback IFrameHandler;
        public SotErrorReport errReportHandler;
    }

    /// <summary>
    /// 软终端初始化参数
    /// </summary>
    public class InitSotParam
    {
        /// <summary>
        /// 融合网管ip
        /// </summary>
        public string cgwIp { get; set; }

        /// <summary>
        /// 音频协议
        /// </summary>
        public StreamType audioType { get; set; }

        /// <summary>
        /// rtp起始端口
        /// </summary>
        public int rtpStartPort { get; set; }

        /// <summary>
        /// rtp端口数量
        /// </summary>
        public int rtpPortCount { get; set; }

        /// <summary>
        /// 监控通道呼叫端口
        /// </summary>
        public int channelCallPort { get; set; }
    }
}
