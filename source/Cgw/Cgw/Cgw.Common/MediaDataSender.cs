/*-----------------------------------------------------------------------
//文件名：MediaDataSender.cs
//版权：Copyright 2011-2012 Huawei Tech. Co. Ltd. All Rights Reserved. 
//作者：kf70422
//日期：2013-5-21
//描述：码流发送
// 1、对于IVS监控平台，SPS PPS IDR分开回调，会导致软终端在打RTP包的时候，时间戳不一致，导致HME无法解码，因此需要进行拼帧处理
//---------------------------------------------------------------------*/
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
//using System.IO;

namespace Cgw.Common
{
    /// <summary>
    /// 码流回调委托
    /// </summary>
    /// <param name="cameraNo">摄像头编号</param>
    /// <param name="mediaData">码流</param>
    /// <param name="sender">调用者,区别哪个平台的回调</param>
    public delegate void DataCallBack(string cameraNo, MediaData mediaData, string sender);


    public delegate void DataExceptionCallBack(string cameraNo);

    public class MediaDataSender
    {
        /// <summary>
        /// 码流回调函数
        /// </summary>
        public DataCallBack DataCallBack { get; set; }

        /// <summary>
        /// 摄像头编号
        /// </summary>
        public string CameraNo { get; set; }

        /// <summary>
        /// 最后一次缓存的数据的帧类型
        /// </summary>
        public FrameDataType LastCacheFrameType { get; set; }

        //数据缓存，完成帧数据拼接后一起发送，完整的I帧数据为SPS+PPS+IDR
        public byte[] CacheData { get; set; }

        public MediaDataSender(string cameraNo, DataCallBack dataCallBack)
        {
            this.CameraNo = cameraNo;
            this.DataCallBack = dataCallBack;
        }

        //private FileStream fs = new FileStream(@"D:\IvsVideoStreambefore.264", FileMode.OpenOrCreate, FileAccess.Write, FileShare.None);
        //private int writeCount = 0;

        /// <summary>
        /// 发送数据
        /// </summary>
        /// <param name="videoData"></param>
        /// <param name="sender"></param>
        public void SendData(MediaData mediaData, string sender)
        {
            if (mediaData.StreamType == StreamType.VIDEO_H264)
            {
                //if (writeCount < 1000)
                //{
                //    fs.Write(mediaData.Data, 0, (int)mediaData.Size);
                //    writeCount++;
                //}

                SendVideoData(mediaData, sender);
            }
            if (mediaData.StreamType == StreamType.AUDIO_G711A || mediaData.StreamType == StreamType.AUDIO_G711U)
            {
                SendAudioData(mediaData, sender);
            }
        }

        /// <summary>
        /// 发送视频码流,
        /// 对于H264的帧数据，如果监控平台回调的码流，SPS PPS IDR是分开发送的,则需要进行拼帧处理，将SPS+PPS+IDR拼接在一起发送
        /// </summary>
        /// <param name="mediaData"></param>
        /// <param name="sender"></param>
        public void SendVideoData(MediaData mediaData, string sender)
        {
            //对于H264的帧数据，如果监控平台回调的码流，SPS PPS IDR是分开发送的，则需要进行拼帧处理，将SPS+PPS+IDR拼接在一起发送
            if (mediaData.DataType == MediaDataType.FRAME_DATA)
            {
                switch (mediaData.FrameType)
                {
                    case FrameDataType.H264_SEI_NALU_TYPE:
                        //SEI的包，直接丢弃
                        return;

                    case FrameDataType.H264_SPS_NALU_TYPE:
                        //SPS的类型，先清空缓存
                        //this.CacheData = new byte[mediaData.Size];
                        //将SPS的数据缓存
                        this.CacheData = mediaData.Data;
                        this.LastCacheFrameType = mediaData.FrameType;

                        //直接return，暂不发送
                        return;

                    case FrameDataType.H264_PPS_NALU_TYPE:
                        //如果缓存最后一帧数据不为SPS或者PPS,说明数据乱序，舍弃。
                        if ((this.LastCacheFrameType != FrameDataType.H264_SPS_NALU_TYPE) && (this.LastCacheFrameType != FrameDataType.H264_PPS_NALU_TYPE))
                        {
                            return;
                        }

                        //将PPS的数据缓存，暂不发送
                        byte[] byteTemp = new byte[this.CacheData.Length + mediaData.Data.Length];
                        this.CacheData.CopyTo(byteTemp, 0);
                        mediaData.Data.CopyTo(byteTemp, this.CacheData.Length);
                        this.CacheData = byteTemp;

                        this.LastCacheFrameType = mediaData.FrameType;

                        //直接return，暂不发送
                        return;

                    case FrameDataType.H264_IDR_NALU_TYPE:
                        //如果缓存是上最后一帧数据不为PPS,说明数据乱序，舍弃
                        if (this.LastCacheFrameType != FrameDataType.H264_PPS_NALU_TYPE)
                        {
                            return;
                        }

                        byteTemp = new byte[this.CacheData.Length + mediaData.Data.Length];
                        this.CacheData.CopyTo(byteTemp, 0);
                        mediaData.Data.CopyTo(byteTemp, this.CacheData.Length);
                        mediaData.Data = byteTemp;
                        mediaData.Size = (uint)mediaData.Data.Length;
                        //break跳出 switch，进行发送
                        break;

                    default:
                        //其他类型的数据，直接发送
                        break;
                }
            }

            this.DataCallBack(this.CameraNo, mediaData, sender);
        }

        /// <summary>
        /// 发送音频码流
        /// </summary>
        /// <param name="videoData"></param>
        /// <param name="sender"></param>
        public void SendAudioData(MediaData videoData, string sender)
        {
            this.DataCallBack(this.CameraNo, videoData, sender);
        }

    }
}
