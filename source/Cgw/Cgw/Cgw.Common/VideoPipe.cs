/*-----------------------------------------------------------------------
//文件名：VideoPipe.cs
//版权：Copyright 2011-2012 Huawei Tech. Co. Ltd. All Rights Reserved. 
//作者：w00206574
//日期：2014-6-12
//描述：实况管道类
//---------------------------------------------------------------------*/
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO.Pipes;
using System.Threading;
using System.IO;
using System.Runtime.Serialization.Formatters.Binary;
using System.Runtime.Serialization;
using System.Runtime.InteropServices;
using System.Timers;

namespace Cgw.Common
{
    /// <summary>
    /// 实况管道类
    /// </summary>
    public class VideoPipe
    {
        /// <summary>
        /// 日志
        /// </summary>
        private static NLog.Logger log = NLog.LogManager.GetCurrentClassLogger();

        /// <summary>
        /// 摄像机编码
        /// </summary>
        string cameraNo;
        public string CameraNo
        {
            get { return cameraNo; }
            set { cameraNo = value; }
        }

        /// <summary>
        /// 管道名称
        /// </summary>
        string pipeName;
        public string PipeName
        {
            get { return pipeName; }
            set { pipeName = value; }
        }

        //命名管道
        NamedPipeClientStream pipedStream;
        public System.IO.Pipes.NamedPipeClientStream PipedStream
        {
            get { return pipedStream; }
            set { pipedStream = value; }
        }

        /// <summary>
        /// 停止线程
        /// </summary>
        bool shouldStop = false;


        /// <summary>
        /// 管道已经连接
        /// </summary>
        public bool HaveConnect
        {
            get { return pipedStream.CanRead; }
        }

        /// <summary>
        /// 管道持续无数据的上限
        /// </summary>
        private int iNullCount = ConfigSettings.CgwChanelNoData;

        /// <summary>
        /// 管道无数据数量初始化
        /// </summary>
        private int iCount = 0;

        //MediaData mediaData = new MediaData();

        private UInt32 mediaDataSize = 0;

        /// <summary>
        /// 定时器器，用于监控管道有无数据
        /// </summary>
        private System.Timers.Timer namedPipeHasVideoDataTimer = new System.Timers.Timer();

        /// <summary>
        /// 构造函数，实例化命名管道
        /// </summary>
        public VideoPipe(string pipeName)
        {
            mediaDataSize = 0;
            this.pipeName = pipeName;
            this.pipedStream = new NamedPipeClientStream(".", pipeName, PipeDirection.InOut, PipeOptions.Asynchronous);

            //初始化定时器
            this.namedPipeHasVideoDataTimer.AutoReset = true;
            this.namedPipeHasVideoDataTimer.Elapsed += new ElapsedEventHandler(namePipeDataCheck_Elapsed);
            //管道监控时间间隔，1秒
            this.namedPipeHasVideoDataTimer.Interval = CgwConst.Thread_Sleep_Time;
            this.namedPipeHasVideoDataTimer.Start();

        }

        //private FileStream fs = new FileStream(@"D:\cgwVideoAfter3.264", FileMode.OpenOrCreate, FileAccess.Write, FileShare.None);
        //private int wCount = 0;

        void namePipeDataCheck_Elapsed(object sender, ElapsedEventArgs e)
        {
            //namedPipeHasVideoDataTimer.Enabled = false;
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            try
            {
                //uint compareSize = mediaData.Size;
                uint compareSize = mediaDataSize;
                Thread.Sleep(1500);
                //uint size = mediaData.Size;

                uint size = mediaDataSize;

                //if (compareSize != size && mediaData.Size != 0)
                if(compareSize!=size && size!=0)
                {
                    iCount = 0;
                }
                else
                {
                    iCount++;
                }
                if (iCount == iNullCount)
                {
                    logEx.Debug("VideoPipe.Timer Data end,iCount={0},cameraNo={1}.", iCount,cameraNo);
                    namedPipeHasVideoDataTimer.Enabled = false;
                    logEx.Info("STOP namedPipeHasVideoDataTimer.");
                    VideoPipeManage.Instance().DeleteVideoPipe(cameraNo);
                    Stop();
                    VideoPipeManage.Instance().DeleteNoDataVideoPipe(cameraNo);
                }
                //else
                //{
                //    namedPipeHasVideoDataTimer.Enabled = true;
                //}
            }
            catch (System.Exception ex)
            {
                logEx.Error("VideoPipe.Timer.delete(cameraNo{0}) catch() failed ,ex={1}", cameraNo, ex.ToString());
            }
        }


        /// <summary>
        /// 运行管道
        /// </summary>
        public void Run()
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);

            logEx.Trace("VideoPipe.Connect.pipeName = {0} ", pipeName);
            pipedStream.Connect(CgwConst.ENTER_PIPE_LOCK_WAIT_TIME);
        }

        /// <summary>
        /// 开始读取 码流数据
        /// </summary>
        /// <param name="str"></param>
        public void ReadData(Common.DataCallBack dataCallBack)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);

            Thread th = new Thread(new ThreadStart(() =>
            {
                while (!shouldStop)
                {
                    try
                    {
                        if (pipedStream != null && pipedStream.IsConnected)
                        {
                            byte[] byteArray = new byte[4]; //unit字节长度
                            pipedStream.ReadMode = PipeTransmissionMode.Byte;

                            MediaData mediaData = new MediaData();

                            pipedStream.Read(byteArray, 0, 4);
                            mediaData.Size = (uint)BitConverter.ToInt32(byteArray, 0);
                            if (mediaData.Size == 0)
                            {
                                continue;
                            }
                            pipedStream.Read(byteArray, 0, 4);
                            mediaData.StreamType = (StreamType)BitConverter.ToInt32(byteArray, 0);
                            pipedStream.Read(byteArray, 0, 4);
                            mediaData.DataType = (MediaDataType)BitConverter.ToInt32(byteArray, 0);
                            pipedStream.Read(byteArray, 0, 4);
                            mediaData.FrameType = (FrameDataType)BitConverter.ToInt32(byteArray, 0);

                            byte[] dataByteArray = new byte[mediaData.Size];
                            //读取管道中的流
                            pipedStream.Read(dataByteArray, 0, (int)mediaData.Size);
                            mediaData.Data = dataByteArray;

                            mediaDataSize = mediaData.Size;

                            //if (wCount < 1200)
                            //{
                            //    fs.Write(mediaData.Data, 0, (int)mediaData.Size);
                            //    wCount++;
                            //}

                            //回调函数
                            dataCallBack(cameraNo, mediaData, cameraNo.Substring(0, cameraNo.IndexOf("#")));
                        }
                    }
                    catch (Exception ex)
                    {
                        logEx.Error("VideoPipe.ReadData is error,ex={0}", ex.ToString());
                        if (pipedStream != null && !pipedStream.IsConnected)
                        {
                            VideoPipeManage.Instance().DeleteVideoPipe(cameraNo);
                            Stop();
                        }
                    }
                }
            }));
            th.IsBackground = true;
            th.Start();
        }

        ///// <summary>
        ///// 回调函数
        ///// </summary>
        ///// <param name="result"></param>
        //void CallbackFun(IAsyncResult result)
        //{
        //    NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
        //    try
        //    {
        //        pipedStream.EndRead(result);
        //    }
        //    catch (System.Exception ex)
        //    {
        //        logEx.Error(ex.ToString());
        //    }
        //}

        /// <summary>
        /// 停止管道
        /// </summary>
        public void Stop()
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            shouldStop = true;
            if (pipedStream == null)
            {
                return;
            }
            ShutdownPipedStream();

            //2015/3/3 销毁定时器，防止内存泄露
            if (namedPipeHasVideoDataTimer != null)
            {
                namedPipeHasVideoDataTimer.Close();
                namedPipeHasVideoDataTimer.Dispose();
                logEx.Debug("Stop namedPipeHasVideoDataTimer Success.");
            }
        }

        /// <summary>
        /// 关闭管道、释放资源
        /// </summary>
        void ShutdownPipedStream()
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            try
            {
                pipedStream.Close();
                pipedStream.Dispose();
                pipedStream = null;
            }
            catch (System.Exception ex)
            {
                logEx.Error(ex.ToString());
            }
        }
    }
}
