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
using System.Timers;

namespace CgwMonitorManage.Common
{
    /// <summary>
    /// 实况管道类
    /// </summary>
    [Serializable]
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
        NamedPipeServerStream pipedStream;
        public System.IO.Pipes.NamedPipeServerStream PipedStream
        {
            get { return pipedStream; }
            set { pipedStream = value; }
        }

        /// <summary>
        /// 管道已经连接
        /// </summary>
        bool haveConnect = false;
        public bool HaveConnect
        {
            get { return haveConnect; }
            set { haveConnect = value; }
        }

        /// <summary>
        /// 构造函数，实例化命名管道
        /// </summary>
        public VideoPipe(string pipeName)
        {
            this.pipeName = pipeName;
            this.pipedStream = new NamedPipeServerStream(pipeName, PipeDirection.InOut, 1, PipeTransmissionMode.Byte, PipeOptions.Asynchronous);
        }

        /// <summary>
        /// 运行管道
        /// </summary>
        public void Run()
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            Thread th = new Thread(new ThreadStart(() =>
            {
                try
                {
                    pipedStream.WaitForConnection();
                    haveConnect = true;
                }
                catch
                {
                    logEx.Error("Run failed");
                }

            }));
            th.Start();
        }

        /// <summary>
        /// 开始发送码流数据
        /// </summary>
        /// <param name="str"></param>
        public void SenderData(MediaData mediaData)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            try
            {
                if (pipedStream == null)
                {
                    return;
                }
                //同步写数据
                byte[] byteArray1 = BitConverter.GetBytes(mediaData.Size);
                byte[] byteArray2 = BitConverter.GetBytes((Int32)mediaData.StreamType);
                byte[] byteArray3 = BitConverter.GetBytes((Int32)mediaData.DataType);
                byte[] byteArray4 = BitConverter.GetBytes((Int32)mediaData.FrameType);
                byte[] byteArray5 = mediaData.Data;
                //将数据合并起来
                byte[] byteArray = new byte[byteArray1.Length + byteArray2.Length + byteArray3.Length + byteArray4.Length + byteArray5.Length];

                byteArray1.CopyTo(byteArray, 0);
                byteArray2.CopyTo(byteArray, byteArray1.Length);
                byteArray3.CopyTo(byteArray, byteArray1.Length + byteArray2.Length);
                byteArray4.CopyTo(byteArray, byteArray1.Length + byteArray2.Length + byteArray3.Length);
                byteArray5.CopyTo(byteArray, byteArray1.Length + byteArray2.Length + byteArray3.Length + byteArray4.Length);
                //写入管道
                //pipedStream.BeginWrite(byteArray, 0, byteArray.Length, new AsyncCallback(CallbackFun), null);
                pipedStream.Write(byteArray, 0, byteArray.Length);
            }
            catch (Exception ex)
            {
                logEx.Error(ex.ToString());
                if (ex is System.IO.IOException)
                {
                    this.Stop();
                }
            }
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
        //        pipedStream.EndWrite(result);
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
            try
            {
                if (pipedStream == null)
                {
                    return;
                }
                ShutdownPipedStream();
            }
            catch (Exception ex)
            {
                logEx.Error(ex.ToString());
            }
        }

        /// <summary>
        /// 关闭管道、释放资源
        /// </summary>
        void ShutdownPipedStream()
        {
            pipedStream.Close();
            pipedStream.Dispose();
            pipedStream = null;
        }
    }
}
