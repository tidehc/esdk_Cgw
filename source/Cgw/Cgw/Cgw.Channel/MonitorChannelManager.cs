/*-----------------------------------------------------------------------
//文件名：MonitorChannelManager.cs
//版权：Copyright 2011-2012 Huawei Tech. Co. Ltd. All Rights Reserved. 
//作者：fangmingliang f00204327
//日期：2013-3-12
//描述：监控通道管理
//      负责码流分发，以及调用软终端接口
//---------------------------------------------------------------------*/
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Cgw.SmcError;
using System.Threading;
using Cgw.Common;

namespace Cgw.Channel
{
    /// <summary>
    /// 通道管理
    /// </summary>
    public class MonitorChannelManager
    {
        /// <summary>
        /// 通道字典 key 通道号码，value 监控通道
        /// </summary>
        private Dictionary<string, MonitorChannel> monitorChannelDic;

        /// <summary>
        /// 通道字典读写锁
        /// </summary>
        private ReaderWriterLockSlim channelDicLocker;

        /// <summary>
        /// 软终端接口适配
        /// </summary>
        private SoftTerminalAdapter softTerminalAdapter = null;

        /// <summary>
        /// I帧重传回调
        /// </summary>
        private IFrameCallback makeIFrameCallback = null;

        /// <summary>
        /// sot错误回调
        /// </summary>
        private SotErrorReport errReportCallback = null;

        /// <summary>
        /// 日志
        /// </summary>
        private NLog.Logger log = NLog.LogManager.GetCurrentClassLogger();

        /// <summary>
        /// 构造函数
        /// </summary>
        public MonitorChannelManager(IFrameCallback iFrameHandler, SotErrorReport errReportHandler)
        {
            this.monitorChannelDic = new Dictionary<string, MonitorChannel>();
            this.channelDicLocker = new ReaderWriterLockSlim();
            this.softTerminalAdapter = new SoftTerminalAdapter();
            this.makeIFrameCallback = iFrameHandler;
            this.errReportCallback = errReportHandler;
        }

        #region 通道字典操作
        /// <summary>
        /// 添加通道
        /// </summary>
        /// <param name="channelNo">通道号码</param>
        public void AddChannel(string channelLabel)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);

            try
            {
                bool successed = this.channelDicLocker.TryEnterWriteLock(CgwConst.ENTER_LOCK_WAIT_TIME);
                // 申请互斥
                if (successed)
                {
                    try
                    {
                        MonitorChannel channel = new MonitorChannel(channelLabel, this.softTerminalAdapter);
                        if (!this.monitorChannelDic.ContainsKey(channelLabel))
                        {
                            this.monitorChannelDic.Add(channelLabel, channel);
                        }

                        logEx.Trace("Add Channel channelNo:{0}", channelLabel);
                    }
                    finally
                    {
                        // 释放互斥量
                        this.channelDicLocker.ExitWriteLock();
                    }
                }
                else
                {
                    // 日志
                    logEx.Error("AddChannel: Enert Write Lock Failed.WaitingReadCount:{0};WaitingWriteCount:{1}.", this.channelDicLocker.WaitingReadCount, this.channelDicLocker.WaitingWriteCount);
                }
            }
            catch (Exception ex)
            {
                // 日志
                logEx.Error(ex, "AddChannel: Enert Write Lock Exception.");
            }
        }

        /// <summary>
        /// 移除通道
        /// </summary>
        /// <param name="channelNo">通道号码</param>
        public void RemoveChannel(string channelLabel)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);

            try
            {
                bool successed = this.channelDicLocker.TryEnterWriteLock(CgwConst.ENTER_LOCK_WAIT_TIME);
                // 申请互斥
                if (successed)
                {
                    try
                    {
                        if (this.monitorChannelDic.ContainsKey(channelLabel))
                        {
                            this.monitorChannelDic[channelLabel].StopThread();
                            this.monitorChannelDic.Remove(channelLabel);
                        }

                        logEx.Trace("Remove Channel channelLabel:{0}", channelLabel);
                    }
                    finally
                    {
                        // 释放互斥量
                        this.channelDicLocker.ExitWriteLock();
                    }
                }
                else
                {
                    // 日志
                    logEx.Error("RemoveChannel: Enert Write Lock Failed.WaitingReadCount:{0};WaitingWriteCount:{1}.", this.channelDicLocker.WaitingReadCount, this.channelDicLocker.WaitingWriteCount);
                }
            }
            catch (Exception ex)
            {
                // 日志
                logEx.Error(ex, "RemoveChannel: Enert Write Lock Exception.");
            }
        }
        #endregion

        #region 软终端接口调用
        /// <summary>
        /// 软终端初始化
        /// </summary>
        /// <param name="addr"></param>
        /// <returns></returns>
        public SmcErr Init(InitSotParam param)
        {
            SmcErr err = new CgwError();
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);

            try
            {
                SoftTerminalCallback handler = new SoftTerminalCallback();
                handler.NlogHandler = new NLogCallback(this.WriteLog);
                handler.IFrameHandler = this.makeIFrameCallback;
                handler.errReportHandler = this.errReportCallback;
                bool isSuccess = this.softTerminalAdapter.RegistSotCallback(handler);
                if (!isSuccess)
                {
                    logEx.Trace("Init Regist callback failed.");
                    err.SetErrorNo(CgwError.ERR_CGW_CHANNEL_SOT_REGIST_CALLBACK_FAIL);
                    return err;
                }

                err = this.softTerminalAdapter.Init(param);
                if (!err.IsSuccess())
                {
                    logEx.Trace("Init failed.");
                    return err;
                }
            }
            catch (System.Exception ex)
            {
                logEx.Error(ex, "Init Exception.");
            }
            

            logEx.Trace("Init successfully.");   
            return err;
        }

        /// <summary>
        /// 添加通道到软终端
        /// </summary>
        /// <param name="channelNo">通道号码</param>
        /// <param name="confNo">会议号</param>
        public bool AddSotChannel(string channelLabel, string confAccessCode)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            bool isSuccess = false;
            logEx.Trace("AddSotChannel channelLabel:{0} confAccessCode:{1}.", channelLabel, confAccessCode);
            string channelCode = CommonFunction.ConvertChannelLabel(channelLabel);


            try
            {
                isSuccess = this.softTerminalAdapter.AddChannel(channelCode, confAccessCode);
                if (isSuccess)
                {
                    logEx.Trace("AddSotChannel channelCode:{0} confAccessCode:{1} successfully.", channelCode, confAccessCode);
                }
                else
                {
                    logEx.Trace("AddSotChannel channelCode:{0} confAccessCode{1} failed.", channelCode, confAccessCode);
                }
            }
            catch (System.Exception ex)
            {
                logEx.Error(ex, "AddSotChannel Exception."); 
            }
            
            return isSuccess;
        }

        /// <summary>
        /// 软终端删除通道
        /// </summary>
        /// <param name="channelNo">通道号码</param>
        /// <returns>删除结果</returns>
        public bool RemoveSotChannel(string channelLabel)
        {
            bool isSuccess = false;
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Trace("RemoveSotChannel channelLabel:{0}.", channelLabel);
            string channelCode = CommonFunction.ConvertChannelLabel(channelLabel);

            try
            {
                isSuccess = this.softTerminalAdapter.RemoveChannel(channelCode);
                if (isSuccess)
                {
                    logEx.Trace("RemoveSotChannel channelCode:{0} successfully.", channelCode);
                }
                else
                {
                    logEx.Trace("RemoveSotChannel channelCode:{0} failed.", channelCode);
                }
            }
            catch (System.Exception ex)
            {
                logEx.Error(ex, "RemoveSotChannel Exception."); 
            }
            

            return isSuccess;
        }

        /// <summary>
        /// 分发码流
        /// </summary>
        /// <param name="channelNo">通道号码</param>
        /// <param name="data">码流</param>
        public void SendMediaData(string channelLabel, MediaData data)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);

            try
            {
                bool successed = this.channelDicLocker.TryEnterReadLock(CgwConst.ENTER_LOCK_WAIT_TIME);
                // 申请互斥
                if (successed)
                {
                    try
                    {
                        if (this.monitorChannelDic.ContainsKey(channelLabel))
                        {
                            this.monitorChannelDic[channelLabel].AddMediaData(data);
                        }
                    }
                    finally
                    {
                        // 释放互斥量
                        this.channelDicLocker.ExitReadLock();
                    }
                }
                else
                {
                    // 日志
                    logEx.Error("SendRtpData: Enert Write Lock Failed.WaitingReadCount:{0};WaitingWriteCount:{1}.", this.channelDicLocker.WaitingReadCount, this.channelDicLocker.WaitingWriteCount);
                }
            }
            catch (Exception ex)
            {
                // 日志
                logEx.Error(ex, "SendRtpData: Enert Write Lock Exception.");
            }
        }

        /// <summary>
        /// 失效通道
        /// </summary>
        /// <param name="channelLabel"></param>
        public void DisableChannel(string channelLabel)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);

            try
            {
                bool successed = this.channelDicLocker.TryEnterReadLock(CgwConst.ENTER_LOCK_WAIT_TIME);
                // 申请互斥
                if (successed)
                {
                    try
                    {
                        if (this.monitorChannelDic.ContainsKey(channelLabel))
                        {
                            this.monitorChannelDic[channelLabel].Disable();
                        }
                        else
                        {
                            logEx.Warn("DisableChannel can't find channel label={0}.", channelLabel);
                        }
                    }
                    finally
                    {
                        // 释放互斥量
                        this.channelDicLocker.ExitReadLock();
                    }
                }
                else
                {
                    // 日志
                    logEx.Error("DisableChannel: Enert Write Lock Failed.WaitingReadCount:{0};WaitingWriteCount:{1}.", this.channelDicLocker.WaitingReadCount, this.channelDicLocker.WaitingWriteCount);
                }
            }
            catch (Exception ex)
            {
                // 日志
                logEx.Error(ex, "DisableChannel: Enert Write Lock Exception.");
            }
        }

        /// <summary>
        /// 写日志回调函数
        /// </summary>
        /// <param name="level"></param>
        /// <param name="log"></param>
        /// <param name="len"></param>
        /// <returns></returns>
        private uint WriteLog(NLogLevel level, string log, int len)
        {
            NLog.Logger sotLog = NLog.LogManager.GetLogger("SoftTerminal");
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(sotLog);

            try
            {
                switch (level)
                {
                    case NLogLevel.Error:
                        logEx.Error(log);
                        break;
                    case NLogLevel.Warn:
                        logEx.Warn(log);
                        break;
                    case NLogLevel.Info:
                        logEx.Info(log);
                        break;
                    case NLogLevel.Trace:
                        logEx.Trace(log);
                        break;
                    default:
                        break;
                }
            }
            catch (System.Exception ex)
            {
                logEx.Error(ex, "WriteLog Exception level:{0}, log:{1}.", level, log);
            }
            
            return 1;
        }
        #endregion
    }
}
