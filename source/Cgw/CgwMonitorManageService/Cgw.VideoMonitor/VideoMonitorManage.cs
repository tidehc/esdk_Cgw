/*-----------------------------------------------------------------------
//文件名：TiandyVideoMonitor.cs
//版权：Copyright 2011-2012 Huawei Tech. Co. Ltd. All Rights Reserved. 
//作者：kf70422
//日期：2013-3-13
//描述：监控平台管理
//---------------------------------------------------------------------*/
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using CgwMonitorManage.SmcError;
using CgwMonitorManage.Common;
using System.IO;
using System.Xml;
using CgwMonitorManage.Tiandy;
using CgwMonitorManage.NLogEx;
using System.Threading;

namespace CgwMonitorManage.VideoMonitor
{
    public class VideoMonitorManage
    {
        /// <summary>
        /// 日志
        /// </summary>
        /// 
        private static NLog.Logger log = NLog.LogManager.GetCurrentClassLogger();

        /// <summary>
        /// 单例模式锁
        /// </summary>
        private static object instanceLocker = new object();

        /// <summary>
        /// 平台管理单例
        /// </summary>
        private static VideoMonitorManage platformManage = null;

        /// <summary>
        /// 码流回调函数，向上回调，平台管理接到码流后，解析，判断是哪个平台的回调，然后去掉平台标识，再执行回调转发码流
        /// </summary>
        private DataCallBack dataCallBack;

        /// <summary>
        /// 监控平台容器，
        /// key为ID，在配置文件里配置必须唯一，标识唯一的监控平台
        /// value为监控平台对应的实现类对象
        /// </summary>
        private Dictionary<string, IVideoMonitor> monitorsDictionary = new Dictionary<string, IVideoMonitor>();

        /// <summary>
        /// 监控平台容器操作锁
        /// </summary>
        private ReaderWriterLockSlim monitorsOperateLock = new ReaderWriterLockSlim();

        /// <summary>
        /// 单例模式，构造函数设置成private
        /// </summary>
        private VideoMonitorManage()
        {
        }

        /// <summary>
        /// 单例模式，获取监控平台管理实例
        /// </summary>
        /// <returns></returns>
        public static VideoMonitorManage Instance()
        {
            lock (instanceLocker)
            {
                if (null == platformManage)
                {
                    platformManage = new VideoMonitorManage();
                }
            }

            return platformManage;
        }

        /// <summary>
        /// 初始化监控平台
        /// </summary>
        /// <returns></returns>
        public SmcErr Load()
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Info("Enter: VideoMonitorManage.Load()");
            SmcErr err = new CgwError();
            XmlElement monitorsRootElement = ConfigSettings.MonitorSettings;
            if (monitorsRootElement == null)
            {
                err.SetErrorNo(CgwError.MONITOR_CONFIG_FILE_INVALID);
                logEx.Error("Monitor config file is invalid.Can not found monitorSettings node.");
                return err;
            }

            foreach (XmlNode monitorNode in monitorsRootElement)
            {
                XmlElement monitorElement = monitorNode as XmlElement;

                if (monitorElement == null)
                {
                    continue;
                }
                else if (CgwConst.MONITOR_TAG.Equals(monitorElement.Name))
                {
                    //判断监控平台是否启用
                    string isEnable = monitorElement.GetAttribute(CgwConst.ENABLE_TAG).Trim();
                    if (!CgwConst.TRUE_TAG.Equals(isEnable.ToLower()))
                    {
                        logEx.Warn("This monitor is disable.");
                        continue;
                    }

                    string monitorId;
                    string className;
                    err = GetMonitorConfig(monitorElement, out monitorId, out className);

                    if (!err.IsSuccess())
                    {
                        return err;
                    }

                    IVideoMonitor videoMonitor;
                    //加载监控平台
                    err = CreateMonitor(monitorId, className, out videoMonitor);
                    if (!err.IsSuccess())
                    {
                        logEx.Error("Create monitor failed.monitorId:{0}, className:{1}, Error code:{2}", monitorId, className, err.ErrNo);
                        return err;
                    }

                    err = videoMonitor.Load(monitorElement);
                    if (err.IsSuccess())
                    {
                        if (this.monitorsOperateLock.TryEnterWriteLock(CgwConst.ENTER_LOCK_WAIT_TIME))
                        {
                            try
                            {
                                monitorsDictionary.Add(monitorId, videoMonitor);
                            }
                            catch (Exception e)
                            {
                                logEx.Error("Load monitor failed.Execption message:{0}", e.Message);
                                return err;
                            }
                            finally
                            {
                                this.monitorsOperateLock.ExitWriteLock();
                            }
                        }
                    }
                    else
                    {
                        logEx.Error("Load monitor failed.MonitorId:{0},Error code:{1}", monitorId, err.ErrNo);
                        return err;
                    }
                }
            }

            logEx.Info("Load all monitors success.");
            return err;
        }

        /// <summary>
        /// 指定码流回调函数
        /// </summary>
        /// <param name="dataCallBack">回调函数</param>
        public void SetDataCallBackFunc(DataCallBack dataCallBack)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Trace("Enter: VideoMonitorManage.SetVideoDataCallBackFunc({0})", dataCallBack);
            //向上回调，平台管理接到码流后，对摄像头编号加上平台标识，再执行回调转发码流
            this.dataCallBack = dataCallBack;

            //向下注册回调，在该回调函数VideoDataCallBackFunc内要先对摄像头编号加上平台标识
            DataCallBack callBack = this.DataCallBackFunc;

            if (this.monitorsOperateLock.TryEnterReadLock(CgwConst.ENTER_LOCK_WAIT_TIME))
            {
                try
                {
                    foreach (KeyValuePair<string, IVideoMonitor> monitor in monitorsDictionary)
                    {
                        IVideoMonitor videoMonitor = monitor.Value;
                        videoMonitor.SetDataCallBackFunc(callBack, monitor.Key);
                    }
                }
                catch (Exception e)
                {
                    logEx.Error("Set VideoDataCallBackFunc failed.Execption message:{0}", e.Message);
                }
                finally
                {
                    this.monitorsOperateLock.ExitReadLock();
                }
            }
            logEx.Trace("Set VideoDataCallBackFunc success.");
        }

        /// <summary>
        /// 注销监控平台资源
        /// </summary>
        /// <returns></returns>
        public SmcErr Unload()
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Info("Enter: VideoMonitorManage.Unload().");
            SmcErr err = new CgwError();
            if (this.monitorsOperateLock.TryEnterWriteLock(CgwConst.ENTER_LOCK_WAIT_TIME))
            {
                try
                {
                    foreach (KeyValuePair<string, IVideoMonitor> monitor in monitorsDictionary)
                    {
                        IVideoMonitor videoMonitor = monitor.Value;
                        SmcErr result = videoMonitor.Unload();
                        if (!result.IsSuccess())
                        {
                            logEx.Error("Unload monitor failed.Monitor id:{0} ,Error code:{1}", monitor.Key, result.ErrNo);
                            //一个失败,记录失败，继续其他平台卸载
                            err.SetErrorNo(result.ErrNo);
                            continue;
                        }
                    }
                    monitorsDictionary.Clear();
                }
                catch (Exception e)
                {
                    logEx.Error("Unload monitor failed..Execption message:{0}", e.Message);
                }
                finally
                {
                    this.monitorsOperateLock.ExitWriteLock();
                }
            }

            return err;
        }

        /// <summary>
        /// 获取摄像头列表及分组信息
        /// </summary>
        /// <param name="fromMonitorSys">如果该值为true，则实时从监控平台获取,否则从融合网关缓存获取</param>
        /// <param name="cameraList">摄像头列表</param>
        /// <param name="groupList">组信息</param>
        /// <param name="nodeRelationList">分组关系</param>
        /// <returns></returns>
        public SmcErr GetAllCameras(PagesParam pageparam, out List<Camera> cameraList, out List<CameraGroup> groupList, out List<NodeRelation> nodeRelationList, out PageInfo pageinfo, PlatformLicenseInfo licenseInfo = null)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Trace("Enter: VideoMonitorManage.GetAllCameras().");
            SmcErr err = new CgwError();
            cameraList = new List<Camera>();
            groupList = new List<CameraGroup>();
            nodeRelationList = new List<NodeRelation>();
            pageinfo = new PageInfo();
            //copy 一份，防止长时间占用锁
            Dictionary<string, IVideoMonitor> monitorsDictionaryTemp = null;

            if (this.monitorsOperateLock.TryEnterReadLock(CgwConst.ENTER_LOCK_WAIT_TIME))
            {
                try
                {
                    monitorsDictionaryTemp = new Dictionary<string, IVideoMonitor>(monitorsDictionary);
                }
                catch (Exception e)
                {
                    logEx.Error("Get all cameras failed.Execption message:{0}", e.Message);
                }
                finally
                {
                    this.monitorsOperateLock.ExitReadLock();
                }
            }
            if (monitorsDictionaryTemp == null)
            {
                //记录日志，获取*监控平台的摄像头列表失败
                logEx.Error("Get all cameras from monitor failed.No any monitor.");
                err.SetErrorNo(CgwError.GET_ALL_CAMERAS_FAILED);
                return err;
            }
            //循环已加载的所有的监控平台，将所有摄像头和分组组成一个list返回
            foreach (KeyValuePair<string, IVideoMonitor> monitor in monitorsDictionaryTemp)
            {
                //if ((monitor.Key == "ivs" || monitor.Key == "tiandy" || monitor.Key == "T28181IVS") && licenseInfo.IsMonitorPlatform == false)
                //{
                //    logEx.Warn("GetAllCameras.smc license no right:{0}", monitor.Key);
                //    continue;
                //}
                //if (monitor.Key == "eLTE" && licenseInfo.IsEltePlatform == false)
                //{
                //    logEx.Warn("GetAllCameras.smc license no right:{0}", monitor.Key);
                //    continue;
                //}
                IVideoMonitor videoMonitor = monitor.Value;

                List<Camera> cameraListTemp;
                List<CameraGroup> groupListTemp;
                List<NodeRelation> nodeRelationListTemp;

                err = videoMonitor.GetAllCameras(out cameraListTemp, out groupListTemp, out nodeRelationListTemp);

                if (err.IsSuccess())
                {
                    //摄像头编号加上监控平台唯一标识
                    foreach (Camera camera in cameraListTemp)
                    {
                        camera.No = EncodeNo(camera.No, monitor.Key);
                        cameraList.Add(camera);
                    }

                    //组编号加上监控平台唯一标识
                    foreach (CameraGroup cameraGroup in groupListTemp)
                    {
                        cameraGroup.No = EncodeNo(cameraGroup.No, monitor.Key);
                        groupList.Add(cameraGroup);
                    }

                    //节点关系编号加上监控平台唯一标识
                    foreach (NodeRelation nodeRelation in nodeRelationListTemp)
                    {
                        nodeRelation.No = EncodeNo(nodeRelation.No, monitor.Key);
                        List<string> pathListTemp = null;
                        if (null != nodeRelation.Path)
                        {
                            pathListTemp = new List<string>();
                            foreach (string path in nodeRelation.Path)
                            {
                                pathListTemp.Add(EncodeNo(path, monitor.Key));
                            }
                        }
                        nodeRelation.Path = pathListTemp;

                        nodeRelationList.Add(nodeRelation);
                    }

                }
                else
                {
                    //记录日志，获取*监控平台的摄像头列表失败
                    logEx.Error("Get all cameras from monitor failed.Monitor id:{0} ,Error code:{1}", monitor.Key, err.ErrNo);
                    err.SetErrorNo(CgwError.GET_ALL_CAMERAS_FAILED);
                    continue;
                }
            }

            //2015/2/26 打印日志
            logEx.Info("all List Counts.");
            logEx.Info("cameraList:" + cameraList.Count);
            logEx.Info("groupList:" + groupList.Count);
            logEx.Info("nodeRelationList:" + nodeRelationList.Count);

            #region 判断传递的pageParam信息的合法性
            //计算cameraList、groupList、nodeRelationList中数据最多的列表长度
            int maxNum = (cameraList.Count > groupList.Count) ? cameraList.Count : groupList.Count;
            maxNum = maxNum > nodeRelationList.Count ? maxNum : nodeRelationList.Count;

            pageinfo.TotalRecords = maxNum;
            logEx.Info("GetAllCameras.pageparam.CurrentPage ={0},pageparam.NumberPerPage ={1}", pageparam.CurrentPage, pageparam.NumberPerPage);
            if (pageparam.CurrentPage <= 0 || pageparam.NumberPerPage <= 0) //如果当前页数或者每页条数小于0，则提示错误
            {
                logEx.Error("The CurrentPage or NumberPerPage can not be smaller than zero ,the CurrentPage is {0},the NumberPerPage is {1}", pageparam.CurrentPage, pageparam.NumberPerPage);
                err.SetErrorNo(CgwError.GET_ALL_CAMERAS_FAILED);
                cameraList = new List<Camera>();
                groupList = new List<CameraGroup>();
                nodeRelationList = new List<NodeRelation>();
                pageinfo = new PageInfo();
                return err;
            }
            else if (maxNum == 0)
            {
                pageinfo = new PageInfo();
                return err;
            }
            else if (maxNum % pageparam.NumberPerPage != 0)
            {
                if (pageparam.CurrentPage > (pageinfo.TotalRecords / pageparam.NumberPerPage + 1)) //如果当前页数大于记录总页数，则提示错误
                {
                    logEx.Error("The CurrentPage can not be more than The Total pageNum(odd),the CurrentPage is {0},the Total PageNum is {1}", pageparam.CurrentPage, pageinfo.TotalRecords / pageparam.NumberPerPage + 1);
                    err.SetErrorNo(CgwError.GET_ALL_CAMERAS_FAILED);
                    cameraList = new List<Camera>();
                    groupList = new List<CameraGroup>();
                    nodeRelationList = new List<NodeRelation>();
                    pageinfo = new PageInfo();
                    return err;
                }
            }
            else
            {
                if (pageparam.CurrentPage > (pageinfo.TotalRecords / pageparam.NumberPerPage)) //如果当前页数大于记录总页数，则提示错误
                {
                    logEx.Error("The CurrentPage can not be more than The Total pageNum(even),the CurrentPage is {0},the Total PageNum is {1}", pageparam.CurrentPage, pageinfo.TotalRecords / pageparam.NumberPerPage);
                    err.SetErrorNo(CgwError.GET_ALL_CAMERAS_FAILED);
                    cameraList = new List<Camera>();
                    groupList = new List<CameraGroup>();
                    nodeRelationList = new List<NodeRelation>();
                    pageinfo = new PageInfo();
                    return err;
                }
            }
            #endregion

            #region 设置返回项PageInfo的值
            pageinfo.TotalRecords = maxNum;
            pageinfo.CurrentPage = pageparam.CurrentPage;
            if (pageinfo.TotalRecords % pageparam.NumberPerPage != 0)
            {
                pageinfo.TotalPages = pageinfo.TotalRecords / pageparam.NumberPerPage + 1;
            }
            else
            {
                pageinfo.TotalPages = pageinfo.TotalRecords / pageparam.NumberPerPage;

            }
            #endregion

            #region 分割数据列表
            List<Camera> CameraListTemp = new List<Camera>(); ;
            List<CameraGroup> CameraGroupTemp = new List<CameraGroup>(); ;
            List<NodeRelation> CamerNodeRelationaListTemp = new List<NodeRelation>();

            int BeforeCurrentPageNum = (pageparam.CurrentPage - 1) * pageparam.NumberPerPage;   //vsd传递的当前页（PageParam.CurrentPage）之前的数据总数
            //cameraList ↓↓
            if (null != cameraList && cameraList.Count > 0)
            {
                for (int i = BeforeCurrentPageNum; i < BeforeCurrentPageNum + pageparam.NumberPerPage; i++)  //i从0开始
                {
                    if (i > cameraList.Count - 1)
                    {
                        break;
                    }
                    CameraListTemp.Add(cameraList[i]);
                }
                cameraList = CameraListTemp;
            }
            //groupList↓↓
            if (null != groupList && groupList.Count > 0)
            {
                for (int i = BeforeCurrentPageNum; i < BeforeCurrentPageNum + pageparam.NumberPerPage; i++)   //将请求页中包含的数据读取出来，并赋值给groupList
                {
                    if (i > groupList.Count - 1)
                    {
                        break;
                    }
                    CameraGroupTemp.Add(groupList[i]);
                }
                groupList = CameraGroupTemp;
            }
            //nodeRelationList↓↓
            if (null != nodeRelationList && nodeRelationList.Count > 0)
            {
                for (int i = BeforeCurrentPageNum; i < BeforeCurrentPageNum + pageparam.NumberPerPage; i++)   //将请求页中包含的数据读取出来，并赋值给nodeRelationList
                {
                    if (i > nodeRelationList.Count - 1)
                    {
                        break;
                    }
                    CamerNodeRelationaListTemp.Add(nodeRelationList[i]);
                }
                nodeRelationList = CamerNodeRelationaListTemp;
            }
            #endregion

            logEx.Info("Get all cameras success.");
            //特殊处理，监控平台统一返回成功，做轮询处理
            err.SetErrorNo(SmcErr.SMC_ERR_SUCCESS);
            return err;

        }

        /// <summary>
        /// 启动摄像头预览
        /// </summary>
        /// <param name="cameraNo">摄像头编号</param>
        /// <returns></returns>
        public SmcErr StartReceiveVideo(string cameraNo)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Trace("Enter: VideoMonitorManage.StartReceiveVideo({0}).", cameraNo);

            IVideoMonitor videoMonitor;
            string decodedCameraNo;

            SmcErr err = DecoderCameraNo(cameraNo, out decodedCameraNo, out videoMonitor);

            if (err.IsSuccess())
            {
                err = videoMonitor.StartReceiveVideo(decodedCameraNo);
                if (err.IsSuccess())
                {
                    logEx.Info("Start receive camera video data success.Camera no:{0}.", cameraNo);
                }
                else
                {
                    logEx.Error("Start receive camera video data failed.Camera no:{0},Error Code:{1} ", cameraNo, err.ErrNo);
                }

            }
            else
            {
                logEx.Error("Start receive camera video data failed. Camera no is not found.Camera no:{0}", cameraNo);
            }

            return err;

        }

        /// <summary>
        /// 停止预览
        /// </summary>
        /// <param name="cameraNo">摄像头编号</param>
        /// <returns>返回错误码</returns>
        public SmcErr StopReceiveVideo(string cameraNo)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Trace("Enter: VideoMonitorManage.StopReceiveVideo({0}).", cameraNo);
            IVideoMonitor videoMonitor;
            string decodedCameraNo;

            SmcErr err = DecoderCameraNo(cameraNo, out decodedCameraNo, out videoMonitor);

            if (err.IsSuccess())
            {
                err = videoMonitor.StopReceiveVideo(decodedCameraNo);
                if (err.IsSuccess())
                {
                    logEx.Info("Stop receive camera video data success.Camera no:{0}.", cameraNo);
                }
                else
                {
                    logEx.Error("Stop receive camera video data failed.Camera no:{0},Error Code:{1} ", cameraNo, err.ErrNo);
                }

            }
            else
            {
                logEx.Error("Stop receive camera video data failed.Camera no is not found.Camera no:{0}.", cameraNo);
            }
            return err;
        }

        /// <summary>
        /// PTZ(Pan/Tilt/Zoom)控制，该接口需要在预览模式下使用
        /// </summary>
        /// <param name="cameraNo">摄像头编号</param>
        /// <param name="ptzCommandType">命令类型</param>
        /// <param name="param">命令参数</param>
        /// <returns>返回错误码</returns>
        public SmcErr StartControlPtz(string cameraNo, PtzCommandType ptzCommandType, int param)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Trace("Enter: VideoMonitorManage.StartControlPtz({0},{1},{2}).", cameraNo, ptzCommandType, param);
            IVideoMonitor videoMonitor;
            string decodedCameraNo;

            SmcErr err = DecoderCameraNo(cameraNo, out decodedCameraNo, out videoMonitor);

            if (err.IsSuccess())
            {
                err = videoMonitor.StartControlPtz(decodedCameraNo, ptzCommandType, param);
                if (err.IsSuccess())
                {
                    logEx.Info("Start control ptz success.Camera no:{0},ptzCommandType:{1},param:{2}.", cameraNo, ptzCommandType, param);
                }
                else
                {
                    logEx.Error("Start control ptz failed.Camera no:{0},ptzCommandType:{1},param:{2},Error Code:{3} ", cameraNo, ptzCommandType, param, err.ErrNo);
                }

            }
            else
            {
                logEx.Error("Start control ptz failed. Camera no is not found.Camera no:{0}.", cameraNo);
            }

            return err;
        }

        /// <summary>
        /// 停止云台控制，摄像头控制
        /// </summary>
        /// <param name="cameraNo">摄像头编号</param>
        /// <param name="ptzCommandType">命令类型</param>
        /// <returns></returns>
        public SmcErr StopControlPtz(string cameraNo, PtzCommandType ptzCommandType)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Trace("Enter: VideoMonitorManage.StopControlPtz({0}).", cameraNo);
            IVideoMonitor videoMonitor;
            string decodedCameraNo;

            SmcErr err = DecoderCameraNo(cameraNo, out decodedCameraNo, out videoMonitor);

            if (err.IsSuccess())
            {
                err = videoMonitor.StopControlPtz(decodedCameraNo, ptzCommandType);
                if (err.IsSuccess())
                {
                    logEx.Info("Stop control ptz success.Camera no:{0},ptzCommandType:{1}.", cameraNo, ptzCommandType);
                }
                else
                {
                    logEx.Error("Stop control ptz failed.Camera no:{0},ptzCommandType:{1},Error Code:{1} ", cameraNo, ptzCommandType, err.ErrNo);
                }

            }
            else
            {
                logEx.Error("Stop control ptz failed. Camera no is not found.Camera no:{0}.", cameraNo);
            }
            return err;
        }

        /// <summary>
        /// 重发关键帧
        /// </summary>
        /// <param name="cameraNo">摄像头编号</param>
        /// <returns></returns>
        public SmcErr MakeIFrame(string cameraNo)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Trace("Enter: VideoMonitorManage.MakeIFrame({0}).", cameraNo);
            IVideoMonitor videoMonitor;
            string decodedCameraNo;

            SmcErr err = DecoderCameraNo(cameraNo, out decodedCameraNo, out videoMonitor);

            if (err.IsSuccess())
            {
                err = videoMonitor.MakeIFrame(decodedCameraNo);
                if (err.IsSuccess())
                {
                    logEx.Info("Make I Frame success.Camera no:{0}.", cameraNo);
                }
                else
                {
                    logEx.Error("Make I Frame failed.Camera no:{0},Error Code:{1}.", cameraNo, err.ErrNo);
                }

            }
            else
            {
                logEx.Error("Make I Frame failed. Camera no is not found.Camera no:{0}.", cameraNo);
            }

            return err;
        }

        /// <summary>
        /// 设置摄像头扬声器状态
        /// </summary>
        /// <param name="cameraNo">摄像头编号</param>
        /// <param name="isOn">扬声器是否开启</param>
        /// <returns></returns>
        public SmcErr SetSpeaker(string cameraNo, bool isOn)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Trace("Enter: VideoMonitorManage.SetSpeaker({0},{1}).", cameraNo, isOn);
            IVideoMonitor videoMonitor;
            string decodedCameraNo;

            SmcErr err = DecoderCameraNo(cameraNo, out decodedCameraNo, out videoMonitor);

            if (err.IsSuccess())
            {
                err = videoMonitor.SetSpeaker(decodedCameraNo, isOn);
                if (err.IsSuccess())
                {
                    logEx.Info("Set camera speaker status success.Camera no:{0}, isOn:{1}.", cameraNo, isOn);
                }
                else
                {
                    logEx.Error("Set camera speaker status failed.Camera no:{0}, isOn:{1}, Error Code:{1} ", cameraNo, isOn, err.ErrNo);
                }

            }
            else
            {
                logEx.Error("Set camera speaker status failed. Camera no is not found. Camera no:{0}, isOn:{1}.", cameraNo, isOn);
            }

            return err;
        }

        /// <summary>
        /// 设置摄像头麦克风状态
        /// </summary>
        /// <param name="cameraNo">摄像头编号</param>
        /// <param name="isOn">麦克风是否开启</param>
        /// <returns></returns>
        public SmcErr SetMic(string cameraNo, bool isOn)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Trace("Enter: VideoMonitorManage.SetMic({0},{1}).", cameraNo, isOn);
            IVideoMonitor videoMonitor;
            string decodedCameraNo;

            SmcErr err = DecoderCameraNo(cameraNo, out decodedCameraNo, out videoMonitor);

            if (err.IsSuccess())
            {
                err = videoMonitor.SetMic(decodedCameraNo, isOn);
                if (err.IsSuccess())
                {
                    logEx.Info("Set camera Mic status success.Camera no:{0}, isOn:{1}.", cameraNo, isOn);
                }
                else
                {
                    logEx.Error("Set camera Mic status failed. Camera no:{0}, isOn:{1}, Error Code:{2} ", cameraNo, isOn, err.ErrNo);
                }

            }
            else
            {
                logEx.Error("Set camera Mic status failed.Camera no is not found.Camera no:{0}, isOn:{1}.", cameraNo, isOn);
            }

            return err;
        }

        /// <summary>
        /// 刷新监控摄像头列表
        /// </summary>
        /// <returns></returns>
        public SmcErr RefreshMonitorCamera()
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Trace("Enter: VideoMonitorManage.RefreshMonitorCamera.");
            SmcErr err = new SmcErr();

            //copy 一份，防止长时间占用锁
            Dictionary<string, IVideoMonitor> monitorsDictionaryTemp = null;

            if (this.monitorsOperateLock.TryEnterReadLock(CgwConst.ENTER_LOCK_WAIT_TIME))
            {
                try
                {
                    monitorsDictionaryTemp = new Dictionary<string, IVideoMonitor>(monitorsDictionary);
                }
                catch (Exception e)
                {
                    logEx.Error("RefreshMonitorCamera failed.Execption message:{0}", e.Message);
                }
                finally
                {
                    this.monitorsOperateLock.ExitReadLock();
                }
            }
            if (monitorsDictionaryTemp == null)
            {
                //记录日志，获取*监控平台的摄像头列表失败
                logEx.Error("RefreshMonitorCamera from monitor failed.No any monitor.");
                err.SetErrorNo(CgwError.ERR_REFRESH_MONITOR_CAMERA_FAIL);
                return err;
            }

            //循环已加载的所有的监控平台，将所有摄像头和分组组成一个list返回
            foreach (KeyValuePair<string, IVideoMonitor> monitor in monitorsDictionaryTemp)
            {
                IVideoMonitor videoMonitor = monitor.Value;
                err = videoMonitor.RefreshMonitorCamera();

                if (err.IsSuccess())
                {
                    logEx.Info("RefreshMonitorCamera success.");
                }
                else
                {
                    logEx.Error("RefreshMonitorCamera failed.");
                    err.SetErrorNo(CgwError.ERR_REFRESH_MONITOR_CAMERA_FAIL);
                    break;
                }
            }
            return err;
        }

        /// <summary>
        /// 获取监控摄像头列表刷新状态，返回结果为0是表示刷新完毕，为1是刷新操作中。当查询刷新状态为0时，可调用获取监控摄像头列表接口，获取刷新后监控摄像头列表
        /// </summary>
        /// <returns></returns>
        public SmcErr GetRefreshStatus(out SmcErr refreshStatus)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Trace("Enter: VideoMonitorManage.GetRefreshStatus.");

            refreshStatus = new SmcErr();
            refreshStatus.ErrNo = CgwError.ERR_DEVICE_LIST_REFRESH_STATUS_END;
            SmcErr err = new SmcErr();

            //copy 一份，防止长时间占用锁
            Dictionary<string, IVideoMonitor> monitorsDictionaryTemp = null;
            if (this.monitorsOperateLock.TryEnterReadLock(CgwConst.ENTER_LOCK_WAIT_TIME))
            {
                try
                {
                    monitorsDictionaryTemp = new Dictionary<string, IVideoMonitor>(monitorsDictionary);
                }
                catch (Exception e)
                {
                    logEx.Error("GetRefreshStatus failed.Execption message:{0}", e.Message);
                }
                finally
                {
                    this.monitorsOperateLock.ExitReadLock();
                }
            }
            if (monitorsDictionaryTemp == null)
            {
                //记录日志，获取*监控平台的摄像头列表失败
                logEx.Error("GetRefreshStatus from monitor failed.No any monitor.");
                err.SetErrorNo(CgwError.ERR_DEVICE_LIST_REFRESH_STATUS_FAILED);
                return err;
            }
            //循环已加载的所有的监控平台，将所有摄像头和分组组成一个list返回

            SmcErr tempMonitor = new SmcErr();
            tempMonitor.ErrNo = CgwError.ERR_DEVICE_LIST_REFRESH_STATUS_END;
            SmcErr tempLTE = new SmcErr();
            tempLTE.ErrNo = CgwError.ERR_DEVICE_LIST_REFRESH_STATUS_END;

            foreach (KeyValuePair<string, IVideoMonitor> monitor in monitorsDictionaryTemp)
            {
                IVideoMonitor videoMonitor = monitor.Value;

                if (monitor.Key != "eLTE")
                {
                    if (tempMonitor.ErrNo == CgwError.ERR_DEVICE_LIST_REFRESH_STATUS_END)
                    {
                        err = videoMonitor.GetRefreshStatus(out tempMonitor);
                    }
                }
                else
                {
                    err = videoMonitor.GetRefreshStatus(out tempLTE);
                }

                if (!err.IsSuccess())
                {
                    logEx.Error("GetRefreshStatus failed.");
                    err.SetErrorNo(CgwError.ERR_DEVICE_LIST_REFRESH_STATUS_FAILED);
                    refreshStatus.ErrNo = CgwError.ERR_DEVICE_LIST_REFRESH_STATUS_FAILED;
                    return err;
                }
            }

            //监控和elte都刷新完毕
            if (tempMonitor.ErrNo == CgwError.ERR_DEVICE_LIST_REFRESH_STATUS_END && tempLTE.ErrNo == CgwError.ERR_DEVICE_LIST_REFRESH_STATUS_END)
            {
                refreshStatus.ErrNo = CgwError.ERR_DEVICE_LIST_REFRESH_STATUS_END;
            }
            //监控和elte都刷新执行中
            else if (tempMonitor.ErrNo == CgwError.ERR_DEVICE_LIST_REFRESH_STATUS_EXECUTING && tempLTE.ErrNo == CgwError.ERR_DEVICE_LIST_REFRESH_STATUS_EXECUTING)
            {
                refreshStatus.ErrNo = CgwError.ERR_DEVICE_LIST_REFRESH_STATUS_EXECUTING;
            }
            //监控和elte都刷新失败
            else if (tempMonitor.ErrNo == CgwError.ERR_DEVICE_LIST_REFRESH_STATUS_FAILED && tempLTE.ErrNo == CgwError.ERR_DEVICE_LIST_REFRESH_STATUS_FAILED)
            {
                refreshStatus.ErrNo = CgwError.ERR_DEVICE_LIST_REFRESH_STATUS_FAILED;
            }
            //监控成功、elte失败 
            else if (tempMonitor.ErrNo == CgwError.ERR_DEVICE_LIST_REFRESH_STATUS_END && tempLTE.ErrNo == CgwError.ERR_DEVICE_LIST_REFRESH_STATUS_FAILED)
            {
                refreshStatus.ErrNo = CgwError.ERR_DEVICE_LIST_REFRESH_STATUS_MONITOR_SUCCESS_ELTE_FAIL;
            }
            //监控成功、elte执行中  
            else if (tempMonitor.ErrNo == CgwError.ERR_DEVICE_LIST_REFRESH_STATUS_END && tempLTE.ErrNo == CgwError.ERR_DEVICE_LIST_REFRESH_STATUS_EXECUTING)
            {
                refreshStatus.ErrNo = CgwError.ERR_DEVICE_LIST_REFRESH_STATUS_MONITOR_SUCCESS_ELTE_EXECUTING;
            }
            //监控执行中、elte成功  
            else if (tempMonitor.ErrNo == CgwError.ERR_DEVICE_LIST_REFRESH_STATUS_EXECUTING && tempLTE.ErrNo == CgwError.ERR_DEVICE_LIST_REFRESH_STATUS_END)
            {
                refreshStatus.ErrNo = CgwError.ERR_DEVICE_LIST_REFRESH_STATUS_MONITOR_EXECUTING_ELTE_SUCCESS;
            }
            //监控执行中、elte失败   
            else if (tempMonitor.ErrNo == CgwError.ERR_DEVICE_LIST_REFRESH_STATUS_EXECUTING && tempLTE.ErrNo == CgwError.ERR_DEVICE_LIST_REFRESH_STATUS_FAILED)
            {
                refreshStatus.ErrNo = CgwError.ERR_DEVICE_LIST_REFRESH_STATUS_MONITOR_EXECUTING_ELTE_FAIL;
            }
            //监控失败、elte成功   
            else if (tempMonitor.ErrNo == CgwError.ERR_DEVICE_LIST_REFRESH_STATUS_FAILED && tempLTE.ErrNo == CgwError.ERR_DEVICE_LIST_REFRESH_STATUS_END)
            {
                refreshStatus.ErrNo = CgwError.ERR_DEVICE_LIST_REFRESH_STATUS_MONITOR_FAIL_ELTE_SUCCESS;
            }
            //监控失败、elte执行中    
            else if (tempMonitor.ErrNo == CgwError.ERR_DEVICE_LIST_REFRESH_STATUS_FAILED && tempLTE.ErrNo == CgwError.ERR_DEVICE_LIST_REFRESH_STATUS_EXECUTING)
            {
                refreshStatus.ErrNo = CgwError.ERR_DEVICE_LIST_REFRESH_STATUS_MONITOR_FAIL_ELTE_EXECUTING;
            }
            return err;
        }
        /// <summary>
        /// 获取监控平台配置
        /// </summary>
        /// <param name="monitorElement"></param>
        /// <returns></returns>
        private SmcErr GetMonitorConfig(XmlElement monitorElement, out string monitorId, out string className)
        {

            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Trace("Enter: VideoMonitorManage.GetMonitorConfig().");
            SmcErr err = new CgwError();

            monitorId = CommonFunction.GetSingleNodeValue(monitorElement, CgwConst.ID_TAG);

            className = CommonFunction.GetSingleNodeValue(monitorElement, CgwConst.CLASS_TAG);

            if (string.IsNullOrEmpty(monitorId))
            {
                //监控平台id不能为空
                err.SetErrorNo(CgwError.MONITOR_CONFIG_FILE_INVALID_ID_NULL);

                logEx.Error("Monitor id can not be null.");
                return err;
            }

            if (string.IsNullOrEmpty(className))
            {
                //监控平台 {0}的class不能为空
                err.SetErrorNo(CgwError.MONITOR_CONFIG_FILE_INVALID_CLASS_NULL);

                logEx.Error("The class of Monitor is null. Monitor id:{0}.", monitorId);
                return err;
            }

            return err;
        }

        /// <summary>
        /// 创建监控平台实例
        /// </summary>
        /// <param name="monitorElement"></param>
        /// <returns></returns>
        private SmcErr CreateMonitor(string monitorId, string className, out IVideoMonitor videoMonitor)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Trace("Enter: VideoMonitorManage.CreateMonitor().");
            SmcErr err = new CgwError();
            videoMonitor = null;
            if (this.monitorsOperateLock.TryEnterWriteLock(CgwConst.ENTER_LOCK_WAIT_TIME))
            {
                try
                {
                    if (monitorsDictionary.ContainsKey(monitorId))
                    {
                        //监控平台的id重复
                        err.SetErrorNo(CgwError.MONITOR_CONFIG_FILE_INVALID_ID_EXIST);
                        logEx.Error("Monitor id has been exist. Monitor id:{0}.", monitorId);
                        return err;
                    }
                }
                catch (Exception e)
                {
                    logEx.Error("CreateMonitor failed..Execption message:{0}", e.Message);
                }
                finally
                {
                    this.monitorsOperateLock.ExitWriteLock();
                }
            }

            Type monitorType = Type.GetType(className);
            if (monitorType == null)
            {
                //class不合法
                err.SetErrorNo(CgwError.MONITOR_CONFIG_FILE_INVALID_CLASS_INVALID);
                logEx.Error("The class of Monitor is invalid. Class:{0}, Monitor id:{1}.", className, monitorId);
                return err;
            }

            try
            {
                //create instance
                logEx.Info("VideoMonitorManage.className = {0}", className);

                if (className == "CgwMonitorManage.eLTE.eLTEVideoMonitor,CgwMonitorManage.eLTE")
                {
                    //eLTE dll放在eLTeSDK目录下，否则跟ivs DLL冲突
                    string strPath = System.AppDomain.CurrentDomain.BaseDirectory;
                    System.Reflection.Assembly assembly = System.Reflection.Assembly.LoadFrom(strPath + @"eLTeSDK\CgwMonitorManage.eLTE.dll");
                    Type[] types = assembly.GetTypes();

                    if (types.Length > 0)
                    {
                        monitorType = types.First((x) => x.FullName == "CgwMonitorManage.eLTE.eLTEVideoMonitor");
                    }
                }
                else if (className == "CgwMonitorManage.Ivs.IvsVideoMonitor,CgwMonitorManage.Ivs")
                {
                    string strPath = System.AppDomain.CurrentDomain.BaseDirectory;
                    System.Reflection.Assembly assembly = System.Reflection.Assembly.LoadFrom(strPath + @"IVSSDK\CgwMonitorManage.Ivs.dll");
                    Type[] types = assembly.GetTypes();
                    if (types.Length > 0)
                    {
                        monitorType = types.First((x) => x.FullName == "CgwMonitorManage.Ivs.IvsVideoMonitor");
                    }
                }
                logEx.Info("VideoMonitorManage.monitorType = {0}", monitorType);
                videoMonitor = Activator.CreateInstance(monitorType) as CgwMonitorManage.Common.IVideoMonitor;
            }
            catch (Exception e)
            {
                //无法加载指定的类
                err.SetErrorNo(CgwError.MONITOR_CONFIG_FILE_INVALID_CLASS_INVALID);
                logEx.Error("The class of Monitor is invalid. May be not as IVideoMonitor. Class:{0}, Monitor id:{1},Exception message:{2}.",
                             className, monitorId, e.Message);
                return err;
            }

            if (videoMonitor == null)
            {
                //无法加载指定的类
                err.SetErrorNo(CgwError.MONITOR_CONFIG_FILE_INVALID_CLASS_INVALID);
                logEx.Error("The class of Monitor is invalid. May be not as IVideoMonitor. Class:{0}, Monitor id:{1}.", className, monitorId);
                return err;
            }

            return err;
        }

        /// <summary>
        /// 解码摄像头编号
        /// </summary>
        /// <param name="encodedCameraNo">编码过的摄像头编号</param>
        /// <param name="cameraNo">解码后的摄像头编号</param>
        /// <param name="videoMonitor">监控平台实例</param>
        /// <returns></returns>
        private SmcErr DecoderCameraNo(string encodedCameraNo, out string cameraNo, out IVideoMonitor videoMonitor)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Trace("Enter: VideoMonitorManage.DecoderCameraNo({0}).", encodedCameraNo);
            SmcErr err = new CgwError();
            cameraNo = string.Empty;
            videoMonitor = null;

            //判断方式不合适
            if(string.IsNullOrEmpty(encodedCameraNo))
            //if (encodedCameraNo == null)
            {
                logEx.Error("Camera no is null.");
                err.SetErrorNo(CgwError.CAMERA_NOT_FOUND);
                return err;
            }

            if (!encodedCameraNo.Contains(CgwConst.MONITOR_SEPARATOR_TAG))
            {
                logEx.Error("Camera no is not found. Camera no:{0}.", encodedCameraNo);
                err.SetErrorNo(CgwError.CAMERA_NOT_FOUND);
                return err;
            }
            string[] monirotAndCamera = encodedCameraNo.Split(
                                                             new string[] { CgwConst.MONITOR_SEPARATOR_TAG },
                                                             StringSplitOptions.RemoveEmptyEntries);


            if (this.monitorsOperateLock.TryEnterReadLock(CgwConst.ENTER_LOCK_WAIT_TIME))
            {
                try
                {
                    //encodedCameraNo是用监控平台id+分割符MONITOR_SEPARATOR_TAG编码的，
                    //用分隔符MONITOR_SEPARATOR_TAG分割后，长度为2，否则为非法编号
                    if (monirotAndCamera.Length == CgwConst.ENCODE_CAMERANO_LENGHT)
                    {
                        if (monitorsDictionary.ContainsKey(monirotAndCamera[0]))
                        {
                            videoMonitor = monitorsDictionary[monirotAndCamera[0]];
                            cameraNo = monirotAndCamera[1];
                            return err;
                        }
                        else
                        {
                            logEx.Error("Monitor id is not found. Monitor id:{0}.", monirotAndCamera[0]);
                            err.SetErrorNo(CgwError.MONITOR_NOT_FOUND);
                            return err;
                        }
                    }
                    else
                    {
                        logEx.Error("Camera no is invalid. Camera no:{0}.", encodedCameraNo);
                        err.SetErrorNo(CgwError.CAMERA_NOT_FOUND);
                        return err;
                    }
                }
                catch (Exception e)
                {
                    err.SetErrorNo(CgwError.CAMERA_NOT_FOUND);
                    logEx.Error("Decoder camera no failed. Execption message:{0}", e.Message);
                }
                finally
                {
                    this.monitorsOperateLock.ExitReadLock();
                }
            }
            return err;
        }

        /// <summary>
        /// 对摄像头编号及组编号加上监控平台标记，防止不同监控平台间，编号重复
        /// </summary>
        /// <param name="No"></param>
        /// <param name="monitorId"></param>
        /// <returns></returns>
        private string EncodeNo(string No, string monitorId)
        {
            return monitorId + CgwConst.MONITOR_SEPARATOR_TAG + No;
        }

        //private FileStream fs = new FileStream(@"D:\IvsVideoStream2.264", FileMode.OpenOrCreate, FileAccess.Write, FileShare.None);
        //private int writeCount = 0;

        /// <summary>
        /// 向下注册的码流回调函数
        /// </summary>
        /// <param name="cameraNo">摄像头编号</param>
        /// <param name="mediaData">码流</param>
        /// <param name="monitorId">监控平台ID</param>
        private void DataCallBackFunc(string cameraNo, MediaData mediaData, string monitorId)
        {
            //摄像头编号要加上平台唯一标记
            cameraNo = EncodeNo(cameraNo, monitorId);

            dataCallBack(cameraNo, mediaData, monitorId);
        }

    }
}
