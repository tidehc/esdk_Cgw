/*-----------------------------------------------------------------------
//文件名：MonitorChannelRelationManager.cs
//版权：Copyright 2011-2012 Huawei Tech. Co. Ltd. All Rights Reserved. 
//作者：fangmingliang f00204327
//日期：2013-3-12
//描述：监控通道关系管理
//      维护通道关系列表，提供同步通道，分配通道，切换通道关联等对外接口
//---------------------------------------------------------------------*/
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Cgw.SmcError;
using System.Threading;
using Cgw.Common;
using Cgw.NLogEx;
using Cgw.MonitorManageAdapter;
namespace Cgw.Channel
{
    /// <summary>
    /// 通道关系管理
    /// </summary>
    public class MonitorChannelRelationManager
    {

        /// <summary>
        /// 通道关系字典  key：通道标识 value：通道关系
        /// </summary>
        private Dictionary<string, MonitorChannelRelation> monitorChannelRelationDic;

        /// <summary>
        /// 通道关系字典读写锁
        /// </summary>
        private ReaderWriterLockSlim monitorChannelRelationDicLocker;

        /// <summary>
        /// 通道管理
        /// </summary>
        private MonitorChannelManager monitorChannelManager;

        /// <summary>
        /// 摄像头视频关联通道字典  key：摄像头编号 value：需要接收该摄像头视频码流的通道标识列表
        /// </summary>
        private Dictionary<string, List<string>> cameraVideoRelationDic;

        /// <summary>
        /// 摄像头视频关联通道字典读写锁
        /// </summary>
        private ReaderWriterLockSlim cameraVideoRelationDicLocker;

        /// <summary>
        /// 摄像头音频关联通道字典 key：摄像头编号 value：需要接收该摄像头音频码流的通道标识列表
        /// </summary>
        private Dictionary<string, List<string>> cameraAudioRelationDic;

        /// <summary>
        /// 摄像头音频关联通道字典读写锁
        /// </summary>
        private ReaderWriterLockSlim cameraAudioRelationDicLocker;

        /// <summary>
        /// 2015/2/29 保存设备重起离线的摄像头，用作在播放时判断Start
        /// </summary>
        private HashSet<string> offLineCameras = new HashSet<string>();

        private List<string> startCameraLists = new List<string>();

        /// <summary>
        /// 日志
        /// </summary>
        private NLog.Logger log = NLog.LogManager.GetCurrentClassLogger();

        /// <summary>
        /// 构造函数
        /// </summary>
        public MonitorChannelRelationManager()
        {
            this.monitorChannelRelationDic = new Dictionary<string, MonitorChannelRelation>();
            this.monitorChannelManager = new MonitorChannelManager(this.MakeIFrame, this.ErrorReport);
            this.monitorChannelRelationDicLocker = new ReaderWriterLockSlim();
            this.cameraVideoRelationDic = new Dictionary<string, List<string>>();
            this.cameraVideoRelationDicLocker = new ReaderWriterLockSlim();
            this.cameraAudioRelationDic = new Dictionary<string, List<string>>();
            this.cameraAudioRelationDicLocker = new ReaderWriterLockSlim();
            CgwMonitorManageAdapter.Instance().SetDataCallBackFunc(this.SendMediaData);
            CgwMonitorManageAdapter.Instance().SetDataExceptionCallBackFunc(this.DataException);
        }

        private void DataException(string cameraNo)
        {
            SmcErr err = new CgwError();
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Trace("DataException start.");
            logEx.Trace("DataException cameraNo={0}.", cameraNo);
            try
            {
                if (this.cameraVideoRelationDic.ContainsKey(cameraNo))
                {
                    List<string> channelList = new List<string>(this.cameraVideoRelationDic[cameraNo]);

                    for (int i = 0; i < channelList.Count; i++)
                    {
                        logEx.Trace("DataException channelList={0}.", channelList[i]);
                        //this.monitorChannelRelationDic[channelList[i]].CameraNo = null;
                        offLineCameras.Add(cameraNo);
                        logEx.Trace("DataException offLineCameras count:{0}", offLineCameras.Count);
                        this.TryStopSendRtpData(cameraNo, channelList[i]);
                    }
                }
            }
            catch (Exception ex)
            {
                // 日志
                err.SetErrorNo(CgwError.ERR_CGW_CHANNEL_SERVICE_ADNORMAL);
                logEx.Error("DataException:{0}", ex);
            }
            logEx.Trace("DataException end.");
        }

        #region 对外接口
        /// <summary>
        /// 初始化
        /// </summary>
        /// <param name="addr">融合网关ip</param>
        /// <returns>结果</returns>
        public SmcErr Init(InitSotParam param)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Info("channelRelationManager Init start.");

            SmcErr err = this.monitorChannelManager.Init(param);
            if (!err.IsSuccess())
            {
                logEx.Error("channelRelationManager Init failed.");
            }
            else
            {
                logEx.Info("channelRelationManager Init successfully.");
            }

            return err;
        }

        /// <summary>
        /// 分配通道
        /// 调度会议时，输入smc分配的通道标识和会议号
        /// 结束会议时，需要再次输入分配的通道标识，会议号设置为空
        /// </summary>
        /// <param name="channelInfoList">通道信息列表</param>
        /// <returns>错误码</returns>
        public SmcErr AssignChannel(List<ChannelInfo> channelInfoList)
        {
            SmcErr err = new CgwError();
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Info("AssignChannel start.");

            if (null == channelInfoList || 0 == channelInfoList.Count)
            {
                // 错误码 日志
                logEx.Error("AssignChannel param error");
                err.SetErrorNo(CgwError.ERR_CGW_CHANNEL_INPUT_ERROR);
                return err;
            }

            try
            {
                bool successed = this.monitorChannelRelationDicLocker.TryEnterWriteLock(CgwConst.ENTER_LOCK_WAIT_TIME);
                // 申请互斥
                if (successed)
                {
                    try
                    {
                        foreach (ChannelInfo item in channelInfoList)
                        {
                            // 存在任一在通道字典中找不到的通道号码，返回错误
                            if (!this.monitorChannelRelationDic.ContainsKey(item.ChannelLabel))
                            {
                                // 错误码 日志
                                logEx.Error("AssignChannel can't find channelLabel:{0}", item.ChannelLabel);
                                err.SetErrorNo(CgwError.ERR_CGW_CHANNEL_NOT_FIND_CHANNELNO);
                                return err;
                            }

                            // 存在任一license无效的通道号码，返回错误
                            if (false == this.monitorChannelRelationDic[item.ChannelLabel].IsValid)
                            {
                                // 错误码 日志
                                logEx.Error("AssignChannel channelLabel:{0} is inValid.", item.ChannelLabel);
                                err.SetErrorNo(CgwError.ERR_CGW_CHANNEL_CHANNEL_INVALID);
                                return err;
                            }
                        }

                        foreach (ChannelInfo item in channelInfoList)
                        {
                            // 设置通道会议号
                            err = this.SyncChannelConfAccessCode(item.ChannelLabel, item.AccessCode, ChannelControlType.None);
                            if (!err.IsSuccess())
                            {
                                logEx.Error("AssignChannel syncChannelConfAccessCode ChannelLabel:{0}, ConfAccessCode:{1} failed.", item.ChannelLabel, item.AccessCode);
                                return err;
                            }
                        }

                        // 日志
                        string channellLog = "Log AssignChannel result:";
                        foreach (KeyValuePair<string, MonitorChannelRelation> pair in this.monitorChannelRelationDic)
                        {
                            string name = string.Format("  channelLabel:{0},confAccessCode:{1}, cameraNo:{2}, isValid:{3};", pair.Value.Label, pair.Value.ConfAccessCode, pair.Value.CameraNo, pair.Value.IsValid);
                            channellLog += name;
                        }
                        logEx.Trace(channellLog);
                    }
                    finally
                    {
                        // 释放互斥量
                        this.monitorChannelRelationDicLocker.ExitWriteLock();
                    }
                }
                else
                {
                    // 日志
                    logEx.Error("AssignChannel: Enert Write Lock Failed.WaitingReadCount:{0};WaitingWriteCount:{1}.", this.monitorChannelRelationDicLocker.WaitingReadCount, this.monitorChannelRelationDicLocker.WaitingWriteCount);
                    err.SetErrorNo(CgwError.ERR_CGW_CHANNEL_GET_LOCK_FAIL);
                }
            }
            catch (Exception ex)
            {
                // 日志
                logEx.Error(ex, "AssignChannel: Enert Write Lock Exception.");
                err.SetErrorNo(CgwError.ERR_CGW_CHANNEL_SERVICE_ADNORMAL);
            }

            // 成功日志
            logEx.Info("AssignChannel successfully");

            return err;
        }

        /// <summary>
        /// 切换通道关联
        /// </summary>
        /// <param name="channelLabel">通道号码</param>
        /// <param name="cameraNo">摄像头编号</param>
        /// <returns>错误码</returns>
        public SmcErr SwitchChannelRelation(string channelLabel, string cameraNo)
        {
            SmcErr err = new CgwError();
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Info("SwitchChannelRelation start.");

            try
            {
                bool successed = this.monitorChannelRelationDicLocker.TryEnterReadLock(CgwConst.ENTER_LOCK_WAIT_TIME);
                // 申请互斥
                if (successed)
                {
                    try
                    {
                        if (!this.monitorChannelRelationDic.ContainsKey(channelLabel))
                        {
                            // 错误码 日志
                            logEx.Error("SwitchChannelRelation can't find channelLabel:{0}", channelLabel);
                            err.SetErrorNo(CgwError.ERR_CGW_CHANNEL_NOT_FIND_CHANNELNO);
                            return err;
                        }

                        // 如果通道没有入会，不允许关联摄像头
                        if (string.IsNullOrEmpty(this.monitorChannelRelationDic[channelLabel].ConfAccessCode))
                        {
                            // 错误码 日志
                            logEx.Error("SwitchChannelRelation channelNo:{0} confAccessCode is null", channelLabel);
                            err.SetErrorNo(CgwError.ERR_CGW_CHANNEL_NOT_IN_CONF);
                            return err;
                        }

                        
                    }
                    finally
                    {
                        // 释放互斥量
                        this.monitorChannelRelationDicLocker.ExitReadLock();
                    }
                }
                else
                {
                    // 日志
                    logEx.Error("SwitchChannelRelation: Enter Write Lock Failed.WaitingReadCount:{0};WaitingWriteCount:{1}.", this.monitorChannelRelationDicLocker.WaitingReadCount, this.monitorChannelRelationDicLocker.WaitingWriteCount);
                    err.SetErrorNo(CgwError.ERR_CGW_CHANNEL_GET_LOCK_FAIL);
                }

                // 调用内部接口，切换通道视频
                err = this.SwitchChannelVideo(channelLabel, cameraNo);
                if (!err.IsSuccess())
                {
                    // 日志
                    this.PrintLog();
                    logEx.Error("SwitchChannelRelation SwitchChannelVideo channelNo:{0} cameraNo:{1} failed", channelLabel, cameraNo);
                    return err;
                }
            }
            catch (Exception ex)
            {
                // 日志
                logEx.Error(ex, "SwitchChannelRelation: Enter Write Lock Exception.");
                err.SetErrorNo(CgwError.ERR_CGW_CHANNEL_SERVICE_ADNORMAL);
            }

            // I帧重传
            this.MakeIFrame(channelLabel);

            this.PrintLog();

            // 成功日志
            logEx.Info("SwitchChannelRelation successfully,channelLabel:{0},cameraNo:{1}", channelLabel, cameraNo);

            return err;
        }

        /// <summary>
        ///  同步通道信息 
        ///  将输入的通道信息更新到本地，本地存在但输入中没有的通道需要删除，本地没有而输入中有的通道需要添加
        ///  本地存在并且输入中也有的通道，也得判断会议号是否变化，有变化还得更新码流，删除/添加软终端呼叫通道等
        /// </summary>
        /// <param name="channelDic">通道标识与会议号字典</param>
        /// <returns>错误码</returns>
        public SmcErr SyncChannelInfo(ref List<ChannelInfo> channelInfoList, int maxChannelNum)
        {
            SmcErr err = new CgwError();
            Dictionary<string, MonitorChannelRelation> temp = null;   // 本地通道号码列表跟输入通道号码列表比较，本地通道号码列表需要删除部分
            List<ChannelInfo> addChannelInfoList = new List<ChannelInfo>();  // 本地通道号码列表跟输入通道号码列表比较，本地通道号码列表需要另外添加部分
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Info("SyncChannelInfo start.");

            if (null == channelInfoList || 0 == channelInfoList.Count || maxChannelNum < 0)
            {
                // 错误码 日志
                logEx.Error("SyncChannelInfo param error");
                err.SetErrorNo(CgwError.ERR_CGW_CHANNEL_INPUT_ERROR);
                return err;
            }

            // 根据license中最大可用通道数量，修改输入通道列表中各通道是否有效
            this.ChangeLicense(ref channelInfoList, maxChannelNum);

            try
            {
                bool successed = this.monitorChannelRelationDicLocker.TryEnterWriteLock(CgwConst.ENTER_LOCK_WAIT_TIME);
                // 申请互斥
                if (successed)
                {
                    try
                    {
                        // 区分需要删除的通道和需要添加的通道
                        temp = new Dictionary<string, MonitorChannelRelation>(this.monitorChannelRelationDic);
                        foreach (ChannelInfo item in channelInfoList)
                        {
                            if (this.monitorChannelRelationDic.ContainsKey(item.ChannelLabel))
                            {
                                // 同步通道会议信息
                                err = this.SyncChannelConfAccessCode(item.ChannelLabel, item.AccessCode, ChannelControlType.None);
                                if (!err.IsSuccess())
                                {
                                    logEx.Trace("SyncChannelInfo modify channelLabel:{0} failed, errNo={1}", item.ChannelLabel, err.ErrNo);
                                    return err;
                                }

                                // 修改license是否有效
                                if (null != this.monitorChannelRelationDic[item.ChannelLabel])
                                {
                                    this.monitorChannelRelationDic[item.ChannelLabel].IsValid = item.IsValid;
                                }

                                temp.Remove(item.ChannelLabel);
                            }
                            else
                            {
                                addChannelInfoList.Add(item);
                            }
                        }

                        // 删除通道
                        foreach (KeyValuePair<string, MonitorChannelRelation> item in temp)
                        {
                            // 设置会议号为空，同时删除通道关系字典中通道
                            err = this.SyncChannelConfAccessCode(item.Key, string.Empty, ChannelControlType.Remove);
                            if (!err.IsSuccess())
                            {
                                logEx.Error("SyncChannelInfo del channelLabel:{0} failed, errNo={1}", item.Key, err.ErrNo);
                                return err;
                            }
                        }

                        // 添加通道
                        foreach (ChannelInfo item in addChannelInfoList)
                        {
                            if (this.monitorChannelRelationDic.ContainsKey(item.ChannelLabel))
                            {
                                continue;
                            }

                            // 添加通道，同时设置通道的会议号
                            err = this.SyncChannelConfAccessCode(item.ChannelLabel, item.AccessCode, ChannelControlType.Add);
                            if (!err.IsSuccess())
                            {
                                logEx.Error("SyncChannelInfo add channelLabel:{0} failed, errNo={1}", item.ChannelLabel, err.ErrNo);
                                return err;
                            }

                            // 修改license是否有效
                            if (null != this.monitorChannelRelationDic[item.ChannelLabel])
                            {
                                this.monitorChannelRelationDic[item.ChannelLabel].IsValid = item.IsValid;
                            }
                        }

                        // 日志
                        string channellLog = "Log SyncChannelInfo result:";
                        foreach (KeyValuePair<string, MonitorChannelRelation> pair in this.monitorChannelRelationDic)
                        {
                            string name = string.Format("  ChannelLabel:{0},confAccessCode:{1}, cameraNo:{2}, isValid:{3};", pair.Value.Label, pair.Value.ConfAccessCode, pair.Value.CameraNo, pair.Value.IsValid);
                            channellLog += name;
                        }
                        logEx.Trace(channellLog);

                    }
                    finally
                    {
                        // 释放互斥量
                        this.monitorChannelRelationDicLocker.ExitWriteLock();
                    }
                }
                else
                {
                    // 日志
                    logEx.Error("SyncChannelInfo: Enert Write Lock Failed.WaitingReadCount:{0};WaitingWriteCount:{1}.", this.monitorChannelRelationDicLocker.WaitingReadCount, this.monitorChannelRelationDicLocker.WaitingWriteCount);
                    err.SetErrorNo(CgwError.ERR_CGW_CHANNEL_GET_LOCK_FAIL);
                }
            }
            catch (Exception ex)
            {
                // 日志
                logEx.Error(ex, "SyncChannelInfo: Enert Write Lock Exception.");
                err.SetErrorNo(CgwError.ERR_CGW_CHANNEL_SERVICE_ADNORMAL);
            }

            // 成功日志
            logEx.Info("SyncChannelInfo successfully");

            return err;
        }

        /// <summary>
        /// 设置摄像头音频状态
        /// </summary>
        /// <param name="channelLabel"></param>
        /// <param name="cameraNo"></param>
        /// <param name="isOn"></param>
        /// <returns></returns>
        public SmcErr SetMic(string channelLabel, string cameraNo, bool isOn)
        {
            SmcErr err = new SmcErr();
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Info("SetMic start.");

            try
            {
                bool successed = this.monitorChannelRelationDicLocker.TryEnterWriteLock(CgwConst.ENTER_LOCK_WAIT_TIME);
                // 申请互斥
                if (successed)
                {
                    try
                    {
                        err = this.SetCameraAudio(channelLabel, cameraNo, isOn);
                        if (!err.IsSuccess())
                        {
                            logEx.Error("SetMic channelLabel={0}, cameraNo={1},isOn={2} failed.", channelLabel, cameraNo, isOn);
                            return err;
                        }
                    }
                    finally
                    {
                        // 释放互斥量
                        this.monitorChannelRelationDicLocker.ExitWriteLock();
                    }
                }
                else
                {
                    // 日志
                    logEx.Error("SetMic: Enert Write Lock Failed.WaitingReadCount:{0};WaitingWriteCount:{1}.", this.monitorChannelRelationDicLocker.WaitingReadCount, this.monitorChannelRelationDicLocker.WaitingWriteCount);
                    err.SetErrorNo(CgwError.ERR_CGW_CHANNEL_GET_LOCK_FAIL);
                }
            }
            catch (Exception ex)
            {
                // 日志
                logEx.Error(ex, "SetMic: Enert Write Lock Exception.");
                err.SetErrorNo(CgwError.ERR_CGW_CHANNEL_SERVICE_ADNORMAL);
            }

            logEx.Info("SetMic SetMic={0}, cameraNo={1},isOn={2} successfully.", channelLabel, cameraNo, isOn);
            return err;
        }

        #endregion

        #region 内部接口

        /// <summary>
        /// 发送rtp码流
        /// </summary>
        /// <param name="cameraNo">摄像头编号</param>
        /// <param name="data">码流</param>
        /// <param name="sender">监控系统</param>
        private void SendMediaData(string cameraNo, MediaData videoData, string sender)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);

            if (videoData.StreamType == StreamType.VIDEO_H264)
            {
                this.SendVideoData(cameraNo, videoData, sender);
            }
            else
            {
                this.SendAudioData(cameraNo, videoData, sender);
            }
        }

        /// <summary>
        /// 发送视频码流
        /// </summary>
        /// <param name="cameraNo"></param>
        /// <param name="videoData"></param>
        /// <param name="sender"></param>
        private void SendVideoData(string cameraNo, MediaData videoData, string sender)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);

            try
            {
                Dictionary<string, List<string>> temp = new Dictionary<string, List<string>>(this.cameraVideoRelationDic);
                if (null == temp)
                {
                    logEx.Trace("SendVideoData temp is null.");
                    return;
                }

                if (!temp.ContainsKey(cameraNo))
                {
                    logEx.Trace("SendVideoData can't find cameraNo={0}.", cameraNo);
                    return;
                }

                // 分发视频码流
                foreach (string item in temp[cameraNo])
                {
                    this.monitorChannelManager.SendMediaData(item, videoData);
                }
            }
            catch (Exception ex)
            {
                // 日志
                logEx.Error(ex, "SendVideoData: Enert Write Lock Exception.");
            }
        }

        /// <summary>
        /// 发送音频码流
        /// </summary>
        /// <param name="cameraNo"></param>
        /// <param name="videoData"></param>
        /// <param name="sender"></param>
        private void SendAudioData(string cameraNo, MediaData videoData, string sender)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);

            try
            {
                Dictionary<string, List<string>> temp = new Dictionary<string, List<string>>(this.cameraAudioRelationDic);
                if (null == temp)
                {
                    logEx.Trace("SendAudioData temp is null.");
                    return;
                }

                if (!temp.ContainsKey(cameraNo))
                {
                    logEx.Trace("SendAudioData can't find cameraNo={0}.", cameraNo);
                    return;
                }

                // 分发音频码流
                foreach (string item in temp[cameraNo])
                {
                    this.monitorChannelManager.SendMediaData(item, videoData);
                }
            }
            catch (Exception ex)
            {
                // 日志
                logEx.Error(ex, "SendAudioData: Enert Write Lock Exception.");
            }
        }

        /// <summary>
        /// I帧重传
        /// 参数label，由软终端回调的是code，通道管理调用的是channelLabel
        /// </summary>
        /// <param name="channelNo">通道标识或者code</param>
        /// <returns>错误码</returns>
        private uint MakeIFrame(string label)
        {
            SmcErr err = new CgwError();
            string channelLabel = string.Empty;
            string cameraNo = string.Empty;
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Trace("MakeIFrame start. label={0}", label);

            // 判断输入号码是否为空
            if (string.IsNullOrEmpty(label))
            {
                // 错误码 日志
                logEx.Trace("MakeIFrame param error");
                return 0;
            }

            try
            {
                bool successed = this.monitorChannelRelationDicLocker.TryEnterReadLock(CgwConst.ENTER_LOCK_WAIT_TIME);
                // 申请互斥
                if (successed)
                {
                    try
                    {
                        foreach (KeyValuePair<string, MonitorChannelRelation> pair in this.monitorChannelRelationDic)
                        {
                            if (pair.Key.IndexOf(label) == 0)
                            {
                                channelLabel = pair.Key;
                                break;
                            }
                        }

                        if (string.IsNullOrEmpty(channelLabel))
                        {
                            // 错误码 日志
                            logEx.Trace("MakeIFrame can't find label:{0}", label);
                            return 0;
                        }

                        cameraNo = this.monitorChannelRelationDic[channelLabel].CameraNo;
                    }
                    finally
                    {
                        // 释放互斥量
                        this.monitorChannelRelationDicLocker.ExitReadLock();
                    }
                }
                else
                {
                    // 日志
                    logEx.Error("MakeIFrame: Enert Read Lock Failed.WaitingReadCount:{0};WaitingWriteCount:{1}.", this.monitorChannelRelationDicLocker.WaitingReadCount, this.monitorChannelRelationDicLocker.WaitingWriteCount);
                    return 0;
                }
            }
            catch (Exception ex)
            {
                // 日志
                logEx.Error(ex, "MakeIFrame: Enter Read Lock Exception.");
                return 0;
            }

            // 调用平台管理接口
            err = CgwMonitorManageAdapter.Instance().MakeIFrame(cameraNo);
            if (!err.IsSuccess())
            {
                logEx.Trace("MakeIFrame cameraNo:{0} failed errNo={1}.", cameraNo, err.ErrNo);
            }

            // 成功日志
            logEx.Trace("MakeIFrame channelLabel={0} cameraNo={1} successfully", channelLabel, cameraNo);

            return 1;
        }

        /// <summary>
        /// 同步通道会议信息
        /// </summary>
        /// <param name="channelNo">通道号码</param>
        /// <param name="confNo">会议号</param>
        private SmcErr SyncChannelConfAccessCode(string channelLabel, string confAccessCode, ChannelControlType controlType)
        {
            SmcErr err = new SmcErr();
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            string oldConfAccessCode = string.Empty;
            logEx.Trace("SyncChannelConfAccessCode Start. input channelLabel:{0}, ConfAccessCode:{1}, ControlType:{2}.", channelLabel, confAccessCode, controlType);

            if ((this.monitorChannelRelationDic.ContainsKey(channelLabel) && controlType == ChannelControlType.Add)
                || (!this.monitorChannelRelationDic.ContainsKey(channelLabel) && controlType != ChannelControlType.Add))
            {
                err.SetErrorNo(CgwError.ERR_CGW_CHANNEL_INPUT_ERROR);
                logEx.Trace("SyncChannelConfAccessCode input error channelLabel:{0}, ConfAccessCode:{1}, ControlType:{2}.", channelLabel, confAccessCode, controlType);
                return err;
            }

            // 判断是否需要添加通道
            if (ChannelControlType.Add == controlType)
            {
                // 通道关系字典添加一个通道
                this.monitorChannelRelationDic.Add(channelLabel, new MonitorChannelRelation(channelLabel));

                // 通道字典添加一个新通道
                this.monitorChannelManager.AddChannel(channelLabel);

                // 判断需要切换的会议号是否为空
                if (!string.IsNullOrEmpty(confAccessCode))
                {
                    // 添加通道到软终端
                    this.monitorChannelManager.AddSotChannel(channelLabel, confAccessCode);
                    this.monitorChannelRelationDic[channelLabel].ConfAccessCode = confAccessCode;
                }

                logEx.Trace("SyncChannelConfAccessCode add channel:{0} successfully.", channelLabel);
            }
            else if (ChannelControlType.None == controlType)
            {
                oldConfAccessCode = this.monitorChannelRelationDic[channelLabel].ConfAccessCode;
                if (string.IsNullOrEmpty(oldConfAccessCode))    // 判断本地通道号码是否入会
                {
                    // 判断需要切换的会议号是否为空
                    if (!string.IsNullOrEmpty(confAccessCode))
                    {
                        // 添加通道到软终端
                        this.monitorChannelManager.AddSotChannel(channelLabel, confAccessCode);
                        this.monitorChannelRelationDic[channelLabel].ConfAccessCode = confAccessCode;
                    }
                }
                else
                {
                    // 判断需要切换的会议号是否为空
                    if (!string.IsNullOrEmpty(confAccessCode))
                    {
                        if (confAccessCode == oldConfAccessCode)
                        {
                            logEx.Trace("SyncChannelConfAccessCode [oldConfAccessCode:{0},ConfAccessNo:{1}] is same.", oldConfAccessCode, confAccessCode);
                            return err;
                        }

                        // 关闭视频码流
                        //err = this.SwitchChannelVideo(channelLabel, string.Empty);
                        err = this.StopCameraRtpAudio(channelLabel);
                        if (!err.IsSuccess())
                        {
                            logEx.Trace("SyncChannelConfAccessCode [oldConfAccessCode:{0},ConfAccessNo:{1}] stoprtp failed.", oldConfAccessCode, confAccessCode);
                            return err;
                        }

                        // 软终端删除该通道
                        this.monitorChannelManager.RemoveSotChannel(channelLabel);

                        // 软终端重新添加该通道
                        this.monitorChannelManager.AddSotChannel(channelLabel, confAccessCode);
                    }
                    else
                    {
                        // 关闭视频码流
                        //err = this.SwitchChannelVideo(channelLabel, string.Empty);
                        err = this.StopCameraRtpAudio(channelLabel);
                        if (!err.IsSuccess())
                        {
                            logEx.Trace("SyncChannelConfAccessCode [oldConfAccessCode:{0},ConfAccessNo:{1}] stoprtp failed.", oldConfAccessCode, confAccessCode);
                            return err;
                        }

                        // 软终端删除该通道
                        this.monitorChannelManager.RemoveSotChannel(channelLabel);
                    }

                    this.monitorChannelRelationDic[channelLabel].ConfAccessCode = confAccessCode;
                }
            }
            else
            {
                oldConfAccessCode = this.monitorChannelRelationDic[channelLabel].ConfAccessCode;
                if (!string.IsNullOrEmpty(oldConfAccessCode))
                {
                    // 关闭视频码流
                    //err = this.SwitchChannelVideo(channelLabel, string.Empty);
                    err = this.StopCameraRtpAudio(channelLabel);
                    if (!err.IsSuccess())
                    {
                        logEx.Trace("SyncChannelConfAccessCode [oldConfAccessCode:{0},ConfAccessNo:{1}] stoprtp failed.", oldConfAccessCode, confAccessCode);
                        return err;
                    }

                    // 软终端删除该通道
                    this.monitorChannelManager.RemoveSotChannel(channelLabel);
                    this.monitorChannelRelationDic[channelLabel].ConfAccessCode = string.Empty;
                }

                // 通道关系字典删除该通道
                if (this.monitorChannelRelationDic.ContainsKey(channelLabel))
                {
                    this.monitorChannelRelationDic.Remove(channelLabel);
                }

                // 通道字典删除该通道
                this.monitorChannelManager.RemoveChannel(channelLabel);
                logEx.Trace("SyncChannelConfAccessCode remove channel:{0} successfully.", channelLabel);
            }

            logEx.Trace("SyncChannelConfAccessCode [oldConfAccessCode:{0},ConfAccessNo:{1}] successfully.", oldConfAccessCode, confAccessCode);
            return err;
        }

        /// <summary>
        /// 设置音频开启/关闭
        /// </summary>
        /// <param name="channelLabel"></param>
        /// <param name="cameraNo"></param>
        /// <param name="isOn"></param>
        /// <returns></returns>
        private SmcErr SetCameraAudio(string channelLabel, string cameraNo, bool isOn)
        {
            SmcErr err = new CgwError();
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            //List<string> channelLabelList = null;

            logEx.Trace("SetCameraAudio,channelLabel:{0},cameraNo:{1},isOn:{2}",channelLabel,cameraNo,isOn);

            //注销代码，默认该摄像头的所有channelLabel的通道列表 by zWX231378 2015/7/13(对SetMic接口会有影响，因暂不支持音频，不修改)
            //// 获取跟channelLabel相同摄像头和会议号的通道列表
            //err = GetCameraRelaitonChannel(channelLabel, cameraNo, out channelLabelList);
            //if (!err.IsSuccess())
            //{
            //    logEx.Trace("SetCameraAudio GetCameraRelaitonChannel failed, errNo={0}.", err.ErrNo);
            //}

            if (true == isOn)
            {
                this.TryStartSendAudio(channelLabel, cameraNo);
            }
            else
            {
                //this.TryStopSendAudio(channelLabelList, cameraNo);
                this.TryStopSendAudio(channelLabel, cameraNo);
            }

            // 成功日志
            logEx.Trace("SetCameraAudio successfully,channelLabel:{0},cameraNo:{1}, isOn={2}.", channelLabel, cameraNo, isOn);

            return err;
        }

        /// <summary>
        /// 获取跟channelLabel相同摄像头和会议号的通道列表
        /// </summary>
        /// <param name="channelLabel"></param>
        /// <param name="cameraNo"></param>
        /// <param name="channelLabelList"></param>
        /// <returns></returns>
        private SmcErr GetCameraRelaitonChannel(string channelLabel, string cameraNo, out List<string> channelLabelList)
        {
            SmcErr err = new CgwError();
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            channelLabelList = new List<string>();
            string confAccessCode = string.Empty;

            // 查找对应的会议号
            if (this.monitorChannelRelationDic.ContainsKey(channelLabel))
            {
                if (this.monitorChannelRelationDic[channelLabel].CameraNo == cameraNo)
                {
                    confAccessCode = this.monitorChannelRelationDic[channelLabel].ConfAccessCode;
                }
                else
                {
                    err.SetErrorNo(CgwError.ERR_CGW_CHANNEL_NOT_FIND_CHANNELNO);
                    logEx.Trace("GetCameraRelaitonChannel can't find channelLabel:{0}, cameraNo={1}.", channelLabel, cameraNo);
                    return err;
                }
            }
            else
            {
                err.SetErrorNo(CgwError.ERR_CGW_CHANNEL_NOT_FIND_CHANNELNO);
                logEx.Trace("GetCameraRelaitonChannel can't find channelLabel:{0}, cameraNo={1}.", channelLabel, cameraNo);
                return err;
            }

            // 查找关联相同摄像头和相同会议号的通道列表
            foreach (KeyValuePair<string, MonitorChannelRelation> pair in this.monitorChannelRelationDic)
            {
                if (pair.Value.CameraNo == cameraNo && pair.Value.ConfAccessCode == confAccessCode)
                {
                    channelLabelList.Add(pair.Key);
                }
            }

            logEx.Trace("GetCameraRelaitonChannel get channellist channelLabel={0}, cameraNo={1}, count={2}", channelLabel, cameraNo, channelLabelList.Count);
            return err;
        }

        /// <summary>
        /// 软终端错误回调
        /// </summary>
        /// <param name="label"></param>
        /// <param name="errType"></param>
        private void ErrorReport(string label, SotErrorType errType)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Trace("ErrorReport label={0}, errType={1}.", label, errType);

            if (errType == SotErrorType.InitStackErr)
            {
                // to do
                logEx.Error("ErrorReport InitStackErr.");
            }
            else if (errType == SotErrorType.RtpSessionCreateErr)
            {
                this.monitorChannelManager.DisableChannel(label);
                logEx.Error("ErrorReport create rtp session error, label={0}.", label);
            }
            else
            {

            }
        }

        /// <summary>
        /// 重新开启实况 
        /// </summary>
        public void ReChannelVideo()
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Trace("Start ReChannelVideo");

            //copy 一份，防止长时间占用锁
            Dictionary<string, MonitorChannelRelation> monitorChannelRelationDicTemp = null;
            try
            {
                monitorChannelRelationDicTemp = new Dictionary<string, MonitorChannelRelation>(monitorChannelRelationDic);
            }
            catch (Exception e)
            {
                logEx.Error("ReChannelVideo.Execption message:{0}", e.Message);
            }

            if (monitorChannelRelationDicTemp == null)
            {
                //记录日志，获取*监控平台的摄像头列表失败
                logEx.Error("ReChannelVideo failed.No any monitorChannelRelationDic.");
                return;
            }

            this.cameraVideoRelationDic = new Dictionary<string, List<string>>();

            //循环通道关系字典
            foreach (KeyValuePair<string, MonitorChannelRelation> monitorChannelRelation in monitorChannelRelationDicTemp)
            {
                //if (monitorChannelRelation.Value.IsValid && monitorChannelRelation.Value.ConfAccessCode != "" && monitorChannelRelation.Value.CameraNo != "")
                if (monitorChannelRelation.Value.IsValid && !string.IsNullOrEmpty(monitorChannelRelation.Value.ConfAccessCode) && !string.IsNullOrEmpty(monitorChannelRelation.Value.CameraNo))
                {
                    string cameraCode = monitorChannelRelation.Value.CameraNo;
                    string channelLabel = monitorChannelRelation.Value.Label;

                    logEx.Trace("ReChannelVideo Before SendRtpData,cameraNo:{0},ChannelLabel:{1}",cameraCode,channelLabel);

                    //移除断线设备
                    offLineCameras.Remove(cameraCode);

                    // 启动新摄像头码流
                    SmcErr err = this.TryStartSendRtpData(cameraCode, channelLabel);
                    if (!err.IsSuccess())
                    {
                        // 原有摄像头编号要置为空，因为旧摄像头已经关闭，避免再次预览旧摄像头，以为还在开启
                        this.monitorChannelRelationDic[channelLabel].CameraNo = null;
                        logEx.Trace("ReChannelVideo cameraNo:{0},Channel:{1} StartRtp failed,errNo={2}.", cameraCode, channelLabel, err.ErrNo);
                        continue;
                    }

                    // 关闭音频码流
                    err = this.SetCameraAudio(channelLabel, cameraCode, false);
                    if (!err.IsSuccess())
                    {
                        logEx.Trace("ReChannelVideo channelLabel:{0},cameraNo:{1} StartAudio failed,errNo={2}.", channelLabel, cameraCode, err.ErrNo);
                        continue;
                    }
                }
            }
        }

        /// <summary>
        /// 清除管道数据
        /// </summary>
        public void StopChannelData()
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            SmcErr err = new CgwError();

            logEx.Trace("start StopChannelData");

            //copy 一份，防止长时间占用锁
            Dictionary<string, MonitorChannelRelation> monitorChannelRelationDicTemp = null;
            try
            {
                monitorChannelRelationDicTemp = new Dictionary<string, MonitorChannelRelation>(monitorChannelRelationDic);
            }
            catch (Exception e)
            {
                logEx.Error("StopChannelData.Execption message:{0}", e.Message);
            }

            if (monitorChannelRelationDicTemp == null)
            {
                //记录日志，获取*监控平台的摄像头列表失败
                logEx.Error("stopChannelVideo failed.No any monitorChannelRelationDic.");
            }
            //循环通道关系字典
            foreach (KeyValuePair<string, MonitorChannelRelation> monitorChannelRelation in monitorChannelRelationDicTemp)
            {
                ////2015/2/13 待测试，判断方式不正确
                if (monitorChannelRelation.Value.IsValid && !string.IsNullOrEmpty(monitorChannelRelation.Value.ConfAccessCode) && !string.IsNullOrEmpty(monitorChannelRelation.Value.CameraNo))
                //if (monitorChannelRelation.Value.IsValid && monitorChannelRelation.Value.ConfAccessCode != "" && monitorChannelRelation.Value.CameraNo != "")
                {
                    string cameraCode = monitorChannelRelation.Value.CameraNo;
                    string channelLabel = monitorChannelRelation.Value.Label;
                    // 停止码流
                    SmcErr errs = this.TryStopSendRtpData(cameraCode, channelLabel);
                    if (!errs.IsSuccess())
                    {
                        logEx.Trace("stopChannelVideo cameraNo:{0} stoprtp failed,errNo={1}.", cameraCode, err.ErrNo);
                    }
                    else
                    {
                        logEx.Trace("stopChannelVideo cameraNo:{0} stoprtp success ", cameraCode);
                    }

                    // 关闭音频码流
                    errs = this.SetCameraAudio(channelLabel, cameraCode, false);
                    if (!err.IsSuccess())
                    { 
                        logEx.Trace("stopChannelVideo channelLabel:{0},cameraNo:{1} StartAudio failed,errNo={2}.", channelLabel, cameraCode, err.ErrNo);
                    }
                    else
                    {
                        logEx.Trace("stopChannelVideo cameraNo:{0} stoprtp success ", cameraCode);
                    }
                }
            }
        }

        #region 摄像头码流操作
        /// <summary>
        /// 切换视频码流 内部接口
        /// </summary>
        /// <param name="channelNo">通道号码</param>
        /// <param name="cameraNo">摄像头编号</param>
        private SmcErr SwitchChannelVideo(string channelLabel, string cameraNo)
        {
            SmcErr err = new CgwError();
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Trace("SwitchChannelVideo Start. input localCameraNo:{0} cameraNo:{1}", channelLabel, cameraNo);

            string oldCameraNo = null;
            if (this.monitorChannelRelationDicLocker.TryEnterReadLock(CgwConst.ENTER_LOCK_WAIT_TIME))
            {
                try
                {
                    // 判断监控通道是否存在
                    if (!this.monitorChannelRelationDic.ContainsKey(channelLabel))
                    {
                        // 日志
                        err.SetErrorNo(CgwError.ERR_CGW_BUSINESS_CHANNELNO_NULL);
                        logEx.Trace("SwitchChannelVideo can't find channelLabel:{0}", channelLabel);
                        return err;
                    }
                    oldCameraNo = this.monitorChannelRelationDic[channelLabel].CameraNo;
                }
                finally
                {
                    this.monitorChannelRelationDicLocker.ExitReadLock();
                }
            }
            else
            {
                err.SetErrorNo(CgwError.ERR_CGW_CHANNEL_GET_LOCK_FAIL);
                logEx.Error("SwitchChannelVideo: Enter Read Lock Failed.WaitingReadCount:{0};WaitingWriteCount:{1}.", this.monitorChannelRelationDicLocker.WaitingReadCount, this.monitorChannelRelationDicLocker.WaitingWriteCount);
                return err;
            }

            logEx.Trace("SwitchChannelVideo oldCameraNo:{0}",oldCameraNo);

            //2015/2/9 重起的摄像头，重新开始播放视频
            if (offLineCameras.Contains(cameraNo))
            {
                logEx.Trace("SwitchChannelVideo offLineCameras cameraNo:{0}",cameraNo);
                offLineCameras.Remove(cameraNo);

                //停止摄像头码流
                if (!string.IsNullOrEmpty(oldCameraNo))
                {
                    // 停止码流
                    err = this.TryStopSendRtpData(oldCameraNo, channelLabel);
                    if (!err.IsSuccess())//停止失败
                    {
                        //this.monitorChannelRelationDic[channelLabel].CameraNo = null;
                        logEx.Trace("SwitchChannelVideo offLineCameras stop cameraNo:{0} failed,errNo={1}", oldCameraNo, err.ErrNo);
                        //return err;
                    }
                }

                // 启动摄像头码流
                err = this.TryStartSendRtpData(cameraNo, channelLabel);
                if (!err.IsSuccess())
                {
                    //this.monitorChannelRelationDic[channelLabel].CameraNo = null;
                    logEx.Trace("SwitchChannelVideo offLineCameras start (oldCameraNo:{0},cameraNo:{1}) startRtp failed,errNo={2}.", oldCameraNo, cameraNo, err.ErrNo);
                    //return err;
                }
                //this.monitorChannelRelationDic[channelLabel].CameraNo = cameraNo;            
            }
            else
            {
                if (string.IsNullOrEmpty(oldCameraNo))
                {
                    if (!string.IsNullOrEmpty(cameraNo))
                    {
                        // 启动摄像头码流
                        err = this.TryStartSendRtpData(cameraNo, channelLabel);
                        if (!err.IsSuccess())
                        {
                            logEx.Trace("SwitchChannelVideo (oldCameraNo:{0},cameraNo:{1}) startRtp failed,errNo={2}.", oldCameraNo, cameraNo, err.ErrNo);
                            return err;
                        }
                        //this.monitorChannelRelationDic[channelLabel].CameraNo = cameraNo;
                    }
                }
                else
                {
                    if (!string.IsNullOrEmpty(cameraNo))
                    {
                        if (cameraNo == oldCameraNo)
                        {
                            logEx.Trace("SwitchChannelVideo oldCameraNo equal CameraNo:{0}.", cameraNo);
                            return err;
                        }

                        // 停止码流
                        err = this.TryStopSendRtpData(oldCameraNo, channelLabel);
                        if (!err.IsSuccess())
                        {
                            logEx.Trace("SwitchChannelVideo (oldCameraNo:{0},cameraNo:{1}) stoprtp failed,errNo={2}.", oldCameraNo, cameraNo, err.ErrNo);
                            return err;
                        }

                        // 启动新摄像头码流
                        err = this.TryStartSendRtpData(cameraNo, channelLabel);
                        if (!err.IsSuccess())
                        {
                            // 原有摄像头编号要置为空，因为旧摄像头已经关闭，避免再次预览旧摄像头，以为还在开启
                            //this.monitorChannelRelationDic[channelLabel].CameraNo = null;
                            logEx.Trace("SwitchChannelVideo (oldCameraNo:{0},cameraNo:{1}) StartRtp failed,errNo={2}.", oldCameraNo, cameraNo, err.ErrNo);
                            //return err;
                        }
                        else
                        {
                            // 关闭音频码流
                            err = this.SetCameraAudio(channelLabel, oldCameraNo, false);
                            if (!err.IsSuccess())
                            {
                                logEx.Trace("SwitchChannelVideo (channelLabel:{0},oldCameraNo:{1},cameraNo:{2}) StartAudio failed,errNo={2}.", channelLabel, oldCameraNo, cameraNo, err.ErrNo);
                                return err;
                            }
                        }
                    }
                    else
                    {
                        // 停止码流
                        err = this.TryStopSendRtpData(oldCameraNo, channelLabel);
                        if (!err.IsSuccess())
                        {
                            logEx.Trace("SwitchChannelVideo (oldCameraNo:{0},cameraNo:{1}) StopRtp failed,errNo={2}.", oldCameraNo, cameraNo, err.ErrNo);
                            return err;
                        }

                        // 关闭音频码流
                        err = this.SetCameraAudio(channelLabel, oldCameraNo, false);
                        if (!err.IsSuccess())
                        {
                            logEx.Trace("SwitchChannelVideo (channelLabel:{0},oldCameraNo:{1},cameraNo:{2}) StartAudio failed,errNo={2}.", channelLabel, oldCameraNo, cameraNo, err.ErrNo);
                            return err;
                        }
                    }

                    //this.monitorChannelRelationDic[channelLabel].CameraNo = cameraNo;
                }
            }
            if (this.monitorChannelRelationDicLocker.TryEnterWriteLock(CgwConst.ENTER_LOCK_WAIT_TIME))
            {
                try
                {
                    if (err.IsSuccess())
                    {
                        this.monitorChannelRelationDic[channelLabel].CameraNo = cameraNo;
                    }
                    else
                    {
                        this.monitorChannelRelationDic[channelLabel].CameraNo = null;
                        return err;
                    }
                }
                finally
                {
                    this.monitorChannelRelationDicLocker.ExitWriteLock();
                }
            }
            else
            {
                err.SetErrorNo(CgwError.ERR_CGW_CHANNEL_GET_LOCK_FAIL);
                logEx.Error("SwitchChannelVideo: Enter Write Lock Failed.WaitingReadCount:{0};WaitingWriteCount:{1}.", this.monitorChannelRelationDicLocker.WaitingReadCount, this.monitorChannelRelationDicLocker.WaitingWriteCount);
                return err;
            }
            logEx.Trace("SwitchChannelVideo localCameraNo:{0} cameraNo:{1} successfully.", channelLabel, cameraNo);
            return err;
        }

        /// <summary>
        /// 停止摄像头的码流和音频流
        /// 同步通道会议信息调用，防止锁占用
        /// </summary>
        /// <param name="channelLabel"></param>
        /// <returns></returns>
        private SmcErr StopCameraRtpAudio(string channelLabel)
        {
            SmcErr err = new CgwError();
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            // 判断监控通道是否存在
            if (!this.monitorChannelRelationDic.ContainsKey(channelLabel))
            {
                // 日志
                err.SetErrorNo(CgwError.ERR_CGW_BUSINESS_CHANNELNO_NULL);
                logEx.Trace("StopCameraRtpAudio can't find channelLabel:{0}", channelLabel);
                return err;
            }
            string oldCameraNo = this.monitorChannelRelationDic[channelLabel].CameraNo;
            if (!string.IsNullOrEmpty(oldCameraNo))
            {
                // 停止码流
                err = this.TryStopSendRtpData(oldCameraNo, channelLabel);
                if (!err.IsSuccess())
                {
                    logEx.Trace("StopCameraRtpAudio (channelLabel:{0},oldCameraNo:{1}) StopRtp failed,errNo={2}.", channelLabel, oldCameraNo, err.ErrNo);
                    return err;
                }

                // 关闭音频码流
                err = this.SetCameraAudio(channelLabel, oldCameraNo, false);
                if (!err.IsSuccess())
                {
                    logEx.Trace("StopCameraRtpAudio (channelLabel:{0},oldCameraNo:{1}) StartAudio failed,errNo={2}.", channelLabel, oldCameraNo, err.ErrNo);
                    return err;
                }

                this.monitorChannelRelationDic[channelLabel].CameraNo = null;

            }
            return err;
        }

        /// <summary>
        /// 尝试启动摄像头码流
        /// 如果关联摄像头通道个数大于0时，不需要任何操作
        /// </summary>
        /// <param name="cameraNo">摄像头编号</param>
        /// <param name="cameraNo">关联的通道编号</param>
        /// <returns>结果</returns>
        private SmcErr TryStartSendRtpData(string cameraNo, string channelLabel)
        {
            SmcErr err = new CgwError();
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Trace("TryStartSendRtpData start.cameraNo:{0},channelLabel:{1}", cameraNo, channelLabel);

            try
            {
                //bool successed = this.cameraVideoRelationDicLocker.TryEnterWriteLock(CgwConst.ENTER_LOCK_WAIT_TIME);
                //// 申请互斥
                //if (successed)
                //{
                //    try
                //    {

                if (!this.cameraVideoRelationDic.ContainsKey(cameraNo))
                {
                    if (this.cameraVideoRelationDicLocker.TryEnterWriteLock(CgwConst.ENTER_LOCK_WAIT_TIME))
                    {
                        try
                        {
                            if (startCameraLists.Contains(cameraNo))
                            {
                                logEx.Trace("TryStartSendRtpData start.Contains CameraNo:{0}",cameraNo);
                                err.SetErrorNo(CgwError.START_RECEIVE_VIDEO_FAILED);
                                return err;
                            }
                            else
                            {
                                startCameraLists.Add(cameraNo);
                                logEx.Info("TryStartSendRtpData start.Add CameraNo:{0}",cameraNo);
                            }
                        }
                        finally
                        {
                            this.cameraVideoRelationDicLocker.ExitWriteLock();
                        }
                    }
                    string pipeName = "";
                    logEx.Trace("TryStartSendRtpData Begin StartReceiveVideo cameraNo={0}", cameraNo);
                    err = CgwMonitorManageAdapter.Instance().StartReceiveVideoByPipe(cameraNo, out pipeName);
                    //startCameraLists.Remove(cameraNo);
                    if (!err.IsSuccess())
                    {
                        logEx.Error("TryStartSendRtpData StartReceiveVideo cameraNo={0} failed.", cameraNo);
                        startCameraLists.Remove(cameraNo);
                        return err;
                    }
                    //this.cameraVideoRelationDic.Add(cameraNo, new List<string>());
                }

                if (this.cameraVideoRelationDicLocker.TryEnterWriteLock(CgwConst.ENTER_LOCK_WAIT_TIME))
                {
                    try
                    {
                        if (!this.cameraVideoRelationDic.ContainsKey(cameraNo))
                        {
                            this.cameraVideoRelationDic.Add(cameraNo, new List<string>());
                        }
                        if (startCameraLists.Contains(cameraNo))
                        {
                            startCameraLists.Remove(cameraNo);
                        }

                        // 判断通道是否已经存在
                        bool isExist = this.cameraVideoRelationDic[cameraNo].Exists(a => { return a == channelLabel; });
                        if (!isExist)
                        {
                            logEx.Trace("TryStartSendRtpData add CameraVideoRelationDic cameraNo={0},channelLabel={1}", cameraNo, channelLabel);
                            this.cameraVideoRelationDic[cameraNo].Add(channelLabel);
                        }
                    }
                    finally
                    {
                        this.cameraVideoRelationDicLocker.ExitWriteLock();
                    }
                }
                else
                {
                    err.SetErrorNo(CgwError.ERR_CGW_CHANNEL_GET_LOCK_FAIL);
                    logEx.Error("TryStartSendRtpData: Enert Write Lock Failed.WaitingReadCount:{0};WaitingWriteCount:{1}.", this.cameraVideoRelationDicLocker.WaitingReadCount, this.cameraVideoRelationDicLocker.WaitingWriteCount);
                }
                //}
                //finally
                //{
                //    // 释放互斥量
                //    this.cameraVideoRelationDicLocker.ExitWriteLock();
                //}
                //}
                //else
                //{
                //    // 日志
                //    err.SetErrorNo(CgwError.ERR_CGW_CHANNEL_GET_LOCK_FAIL);
                //    logEx.Error("TryStartSendRtpData: Enert Write Lock Failed.WaitingReadCount:{0};WaitingWriteCount:{1}.", this.cameraVideoRelationDicLocker.WaitingReadCount, this.cameraVideoRelationDicLocker.WaitingWriteCount);
                //}
            }
            catch (Exception ex)
            {
                // 日志
                err.SetErrorNo(CgwError.ERR_CGW_CHANNEL_SERVICE_ADNORMAL);
                logEx.Error(ex, "TryStartSendRtpData: Enert Write Lock Exception.");
            }

            logEx.Trace("TryStartSendRtpData cameraNo={0}, channelLabel={1} successfully.", cameraNo, channelLabel);
            return err;
        }

        /// <summary>
        /// 尝试停止摄像头码流发送
        /// 只有当关联摄像头通道为1时，才需要停止发送码流
        /// </summary>
        /// <param name="cameraNo">摄像头编号</param>
        /// <param name="channelLabel">原先关联的通道编号</param>
        /// <returns>结果</returns>
        private SmcErr TryStopSendRtpData(string cameraNo, string channelLabel)
        {
            SmcErr err = new CgwError();
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Trace("TryStopSendRtpData start.");

            try
            {
                bool successed = this.cameraVideoRelationDicLocker.TryEnterWriteLock(CgwConst.ENTER_LOCK_WAIT_TIME);
                // 申请互斥
                if (successed)
                {
                    try
                    {
                        if (!this.cameraVideoRelationDic.ContainsKey(cameraNo))
                        {
                            logEx.Trace("TryStopSendRtpData cameraNo={0}, channelLabel={1} can't find cameraNo.", cameraNo, channelLabel);
                            return err;
                        }
                        this.cameraVideoRelationDic[cameraNo].RemoveAll(a => { return a == channelLabel; });
                        // 判断通道是否已经存在
                        if (null == this.cameraVideoRelationDic[cameraNo] || 0 == this.cameraVideoRelationDic[cameraNo].Count)
                        {
                            logEx.Trace("TryStopSendRtpData StopReceiveVideoByPipe cameraNo={0},channelLabel={1}", cameraNo, channelLabel);
                            // 停止视频码流
                            CgwMonitorManageAdapter.Instance().StopReceiveVideoByPipe(cameraNo);

                            this.cameraVideoRelationDic.Remove(cameraNo);
                        }
                    }
                    finally
                    {
                        // 释放互斥量
                        this.cameraVideoRelationDicLocker.ExitWriteLock();
                    }
                }
                else
                {
                    // 日志
                    err.SetErrorNo(CgwError.ERR_CGW_CHANNEL_GET_LOCK_FAIL);
                    logEx.Error("TryStopSendRtpData: Enert Write Lock Failed.WaitingReadCount:{0};WaitingWriteCount:{1}.", this.cameraVideoRelationDicLocker.WaitingReadCount, this.cameraVideoRelationDicLocker.WaitingWriteCount);
                }
            }
            catch (Exception ex)
            {
                // 日志
                err.SetErrorNo(CgwError.ERR_CGW_CHANNEL_SERVICE_ADNORMAL);
                logEx.Error(ex, "TryStopSendRtpData: Enert Write Lock Exception.");
            }

            logEx.Trace("TryStopSendRtpData cameraNo={0}, channelLabel={1} successfully.", cameraNo, channelLabel);
            return err;
        }

        /// <summary>
        /// 根据传入license中最大通道数量，修改通道可用状态（默认通道都是可用）
        /// </summary>
        /// <param name="channelInfoList"></param>
        /// <param name="maxChannelNum"></param>
        private void ChangeLicense(ref List<ChannelInfo> channelInfoList, int maxChannelNum)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);

            List<ChannelInfo> inConfList = channelInfoList.FindAll(a => { return !string.IsNullOrEmpty(a.AccessCode); });
            List<ChannelInfo> outConfList = channelInfoList.FindAll(a => { return string.IsNullOrEmpty(a.AccessCode); });
            int inConfCount = inConfList.Count;
            int outConfCount = outConfList.Count;
            logEx.Trace("ChangeLicense licenseNum={0}, inConfCount={1}, outConfCount={2}", maxChannelNum, inConfCount, outConfCount);

            // 先将所有的isValid设置为true,后面再根据maxChannelNum修改
            foreach (ChannelInfo item in channelInfoList)
            {
                item.IsValid = true;
            }

            // 比较maxChannelNum跟入会会场数量
            // 优先将已入会的会场保留为true
            if (maxChannelNum <= inConfCount)
            {
                List<ChannelInfo> list = inConfList.GetRange(maxChannelNum, inConfCount - maxChannelNum);
                foreach (ChannelInfo item in list)
                {
                    item.IsValid = false;
                }

                foreach (ChannelInfo item in outConfList)
                {
                    item.IsValid = false;
                }
            }
            else if (maxChannelNum > inConfCount && maxChannelNum < inConfCount + outConfCount)
            {
                List<ChannelInfo> list = outConfList.GetRange(maxChannelNum - inConfCount, inConfCount + outConfCount - maxChannelNum);
                foreach (ChannelInfo item in list)
                {
                    item.IsValid = false;
                }
            }
            else
            {
                return;
            }
        }

        /// <summary>
        /// 开启音频码流
        /// </summary>
        /// <param name="channelLabel"></param>
        /// <param name="cameraNo"></param>
        /// <returns></returns>
        private SmcErr TryStartSendAudio(string channelLabel, string cameraNo)
        {
            SmcErr err = new CgwError();
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Trace("TryStartSendAudio start.");

            try
            {
                bool successed = this.cameraAudioRelationDicLocker.TryEnterWriteLock(CgwConst.ENTER_LOCK_WAIT_TIME);
                // 申请互斥
                if (successed)
                {
                    try
                    {
                        if (!this.cameraAudioRelationDic.ContainsKey(cameraNo))
                        {
                            err = CgwMonitorManageAdapter.Instance().SetMic(cameraNo, true);
                            if (!err.IsSuccess())
                            {
                                logEx.Error("TryStartSendAudio SetMic start cameraNo={0} failed.", cameraNo);
                                return err;
                            }

                            this.cameraAudioRelationDic.Add(cameraNo, new List<string>());
                        }

                        // 判断通道是否已经存在
                        bool isExist = this.cameraAudioRelationDic[cameraNo].Exists(a => { return a == channelLabel; });
                        if (!isExist)
                        {
                            this.cameraAudioRelationDic[cameraNo].Add(channelLabel);
                        }
                    }
                    finally
                    {
                        // 释放互斥量
                        this.cameraAudioRelationDicLocker.ExitWriteLock();
                    }
                }
                else
                {
                    // 日志
                    err.SetErrorNo(CgwError.ERR_CGW_CHANNEL_GET_LOCK_FAIL);
                    logEx.Error("TryStartSendAudio: Enert Write Lock Failed.WaitingReadCount:{0};WaitingWriteCount:{1}.", this.cameraAudioRelationDicLocker.WaitingReadCount, this.cameraAudioRelationDicLocker.WaitingWriteCount);
                }
            }
            catch (Exception ex)
            {
                // 日志
                err.SetErrorNo(CgwError.ERR_CGW_CHANNEL_SERVICE_ADNORMAL);
                logEx.Error(ex, "TryStartSendAudio: Enert Write Lock Exception.");
            }

            logEx.Trace("TryStartSendAudio cameraNo={0}, channelLabel={1} successfully.", cameraNo, channelLabel);
            return err;
        }

        /// <summary>
        /// 关闭音频码流
        /// </summary>
        /// <param name="channelLabelList"></param>
        /// <param name="cameraNo"></param>
        /// <returns></returns>
        private SmcErr TryStopSendAudio(string channelLabel, string cameraNo)
        {
            SmcErr err = new CgwError();
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Trace("TryStopSendAudio start.");

            try
            {
                bool successed = this.cameraAudioRelationDicLocker.TryEnterWriteLock(CgwConst.ENTER_LOCK_WAIT_TIME);
                // 申请互斥
                if (successed)
                {
                    try
                    {
                        if (!this.cameraAudioRelationDic.ContainsKey(cameraNo))
                        {
                            logEx.Trace("TryStopSendAudio cameraNo={0} can't find cameraNo.", cameraNo);
                            return err;
                        }

                        //foreach (string channelLabel in channelLabelList)
                        //{
                        //    this.cameraAudioRelationDic[cameraNo].RemoveAll(a => { return a == channelLabel; });
                        //}
                        this.cameraAudioRelationDic[cameraNo].RemoveAll(a => { return a == channelLabel; });

                        // 判断通道是否已经存在
                        if (null == this.cameraAudioRelationDic[cameraNo] || 0 == this.cameraAudioRelationDic[cameraNo].Count)
                        {
                            // 停止音频码流
                            CgwMonitorManageAdapter.Instance().SetMic(cameraNo, false);

                            this.cameraAudioRelationDic.Remove(cameraNo);
                        }
                    }
                    finally
                    {
                        // 释放互斥量
                        this.cameraAudioRelationDicLocker.ExitWriteLock();
                    }
                }
                else
                {
                    // 日志
                    err.SetErrorNo(CgwError.ERR_CGW_CHANNEL_GET_LOCK_FAIL);
                    logEx.Error("TryStopSendAudio: Enert Write Lock Failed.WaitingReadCount:{0};WaitingWriteCount:{1}.", this.cameraAudioRelationDicLocker.WaitingReadCount, this.cameraAudioRelationDicLocker.WaitingWriteCount);
                }
            }
            catch (Exception ex)
            {
                // 日志
                err.SetErrorNo(CgwError.ERR_CGW_CHANNEL_SERVICE_ADNORMAL);
                logEx.Error(ex, "TryStopSendAudio: Enert Write Lock Exception.");
            }

            logEx.Trace("TryStopSendAudio cameraNo={0} successfully.", cameraNo);
            return err;
        }

        /// <summary>
        /// 打印日志
        /// </summary>
        private void PrintLog()
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);

            // 摄像头视频关系日志
            string videoLog = "Log CameraVideoRelation: ";
            Dictionary<string, List<string>> videoDic = new Dictionary<string, List<string>>(this.cameraVideoRelationDic);
            if (null != videoDic)
            {
                foreach (KeyValuePair<string, List<string>> pair in videoDic)
                {
                    videoLog += pair.Key;

                    if (null == pair.Value)
                    {
                        continue;
                    }

                    foreach (string item in pair.Value)
                    {
                        videoLog += item;
                    }

                    videoLog += "  ";
                }
            }
            logEx.Info(videoLog);

            string audioLog = "Log CamreaAudioRelation: ";
            Dictionary<string, List<string>> audioDic = new Dictionary<string, List<string>>(this.cameraAudioRelationDic);
            if (null != audioDic)
            {
                foreach (KeyValuePair<string, List<string>> pair in audioDic)
                {
                    audioLog += pair.Key;

                    if (null == pair.Value)
                    {
                        continue;
                    }

                    foreach (string item in pair.Value)
                    {
                        audioLog += item;
                    }

                    audioLog += "  ";
                }
            }
            logEx.Info(audioLog);

        }

        #endregion

        #endregion

    }
}
