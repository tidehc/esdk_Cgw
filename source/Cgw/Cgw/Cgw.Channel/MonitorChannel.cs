/*-----------------------------------------------------------------------
//文件名：MonitorChannel.cs
//版权：Copyright 2011-2012 Huawei Tech. Co. Ltd. All Rights Reserved. 
//作者：fangmingliang f00204327
//日期：2013-3-12
//描述：监控通道,负责码流的发送
//---------------------------------------------------------------------*/
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using Cgw.SmcError;
using System.Timers;
using Cgw.Common;

namespace Cgw.Channel
{
    /// <summary>
    /// 监控通道
    /// </summary>
    public class MonitorChannel
    {
        /// <summary>
        /// 监控通道号码 code@ip
        /// </summary>
        public string Label { get; private set; }

        /// <summary>
        /// 监控通道code 
        /// 相比较Label去掉了@ip段
        /// </summary>
        private string code = null;

        /// <summary>
        /// 工作线程
        /// </summary>
        private Thread workThread;

        /// <summary>
        /// rtp队列
        /// </summary>
        private Queue<MediaData> mediaDataQueue;

        /// <summary>
        /// rtp队列读写锁
        /// </summary>
        private ReaderWriterLockSlim meidaDataQueueLocker;

        /// <summary>
        /// 等待事件， rtpQueue队列中插入数据时Set
        /// </summary>
        private AutoResetEvent waitEvent;

        /// <summary>
        /// 软终端接口适配
        /// </summary>
        private SoftTerminalAdapter softTerminalAdapter = null;

        /// <summary>
        /// 日志
        /// </summary>
        private NLog.Logger log = NLog.LogManager.GetCurrentClassLogger();

        /// <summary>
        ///  跟发送码流结果比较，不同则打印日志
        /// </summary>
        private bool? printFlag = null;

        /// <summary>
        /// 通道是否有效，创建rtp会话失败会导致通道失效，失效就不会发送码流
        /// </summary>
        private bool isValid = true;

        /// <summary>
        /// 失效打印日志标志
        /// </summary>
        private bool flag = false;

        /// <summary>
        /// 构造函数
        /// </summary>
        /// <param name="no"></param>
        public MonitorChannel(string label, SoftTerminalAdapter adapter)
        {
            this.Label = label;
            this.softTerminalAdapter = adapter;
            this.code = CommonFunction.ConvertChannelLabel(label);
            this.mediaDataQueue = new Queue<MediaData>();
            this.waitEvent = new AutoResetEvent(false);
            this.meidaDataQueueLocker = new ReaderWriterLockSlim();


            this.workThread = new Thread(this.WorkThreadRun);
            this.workThread.Start();
        }

        /// <summary>
        /// 发送码流添加进队列
        /// </summary>
        /// <param name="trp"></param>
        public void AddMediaData(MediaData data)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);

            // 通道失效，不发送码流
            if (isValid == false)
            {
                if (flag == false)
                {
                    logEx.Error("AddMediaData label={0} is invalid.");
                    flag = true;
                }
                
                return;
            }

            try
            {
                bool successed = this.meidaDataQueueLocker.TryEnterWriteLock(CgwConst.ENTER_LOCK_WAIT_TIME);
                // 申请互斥
                if (successed)
                {
                    try
                    {
                        this.mediaDataQueue.Enqueue(data);
                    }
                    finally
                    {
                        // 释放互斥量
                        this.meidaDataQueueLocker.ExitWriteLock();
                    }
                }
                else
                {
                    // 日志
                    logEx.Error("AddRtp: Enert Write Lock Failed.WaitingReadCount:{0};WaitingWriteCount:{1}.", this.meidaDataQueueLocker.WaitingReadCount, this.meidaDataQueueLocker.WaitingWriteCount);
                }
            }
            catch (Exception ex)
            {
                // 日志
                logEx.Error(ex, "AddRtp: Enert Write Lock Exception.");
            }
         
            this.waitEvent.Set();
        }

        /// <summary>
        /// 停止线程
        /// </summary>
        public void StopThread()
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);

            try
            {
                this.workThread.Abort();
            }
            catch (System.Exception ex)
            {
                logEx.Error(ex, "StopThread exceprion.");
            }  
        }

        /// <summary>
        /// 通道失效
        /// </summary>
        public void Disable()
        {
            this.isValid = false;
        }

        /// <summary>
        /// 获取队列中的码流
        /// </summary>
        /// <returns></returns>
        private List<MediaData> GetMediaData()
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            List<MediaData> rtpDataList = new List<MediaData>();

            try
            {
                bool successed = this.meidaDataQueueLocker.TryEnterWriteLock(CgwConst.ENTER_LOCK_WAIT_TIME);
                // 申请互斥
                if (successed)
                {
                    try
                    {
                        while (mediaDataQueue.Count > 0)
                        {
                            rtpDataList.Add(mediaDataQueue.Dequeue());
                        }
                    }
                    finally
                    {
                        // 释放互斥量
                        this.meidaDataQueueLocker.ExitWriteLock();
                    }
                }
                else
                {
                    // 日志
                    logEx.Error("GetRtpData: Enert Write Lock Failed.WaitingReadCount:{0};WaitingWriteCount:{1}.", this.meidaDataQueueLocker.WaitingReadCount, this.meidaDataQueueLocker.WaitingWriteCount);
                }
            }
            catch (Exception ex)
            {
                // 日志
                logEx.Error(ex, "GetRtpData: Enert Write Lock Exception.");
            }

            return rtpDataList;
        }

        /// <summary>
        /// 工作线程函数
        /// </summary>
        private void WorkThreadRun()
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);

            while (true)
            {
                try
                {
                    this.waitEvent.WaitOne();

                    List<MediaData> rtpList = this.GetMediaData();
                    if (null == rtpList)
                    {
                        continue;
                    }

                    foreach (MediaData item in rtpList)
                    {
                        bool isSuccess = this.softTerminalAdapter.SendMediaData(code, item);
                        if (isSuccess != printFlag)
                        {
                            printFlag = isSuccess;
                            logEx.Trace("WorkThreadRun send media stream label={0} code={1} data={2} result={3}.", Label, code, item, isSuccess);
                        }
                    }
                }
                catch (Exception ex)
                {
                    logEx.Error(ex, "WorkThreadRun Exception.");
                }
            }
        }
    }
}
