/*-----------------------------------------------------------------------
//文件名：CgwMonitorManageAdapter.cs
//版权：Copyright 2011-2012 Huawei Tech. Co. Ltd. All Rights Reserved. 
//作者：w00206574
//日期：2014-6-12
//描述：用于对外开放监控通道管理WCF服务接口
//---------------------------------------------------------------------*/

using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Serialization;
using System.ServiceModel;
using System.Text;
using Cgw.SmcError;
using Cgw.Common;

namespace Cgw.Service
{
    /// <summary>
    /// 融合网关对外WCF接口
    /// </summary>
    [ServiceContract(SessionMode = SessionMode.Required)]
    public interface ICgwService
    {


        /// <summary>
        /// 连接融合网关
        /// </summary>
        /// <param name="password">加密后的密码字符串</param>
        /// <returns></returns>
        [OperationContract]
        SmcErr Connect(string password);

        /// <summary>
        /// 断开融合网关
        /// </summary>
        /// <returns></returns>
        [OperationContract]
        SmcErr Disconnect();


        /// <summary>
        /// 保持连接
        /// </summary>
        /// <returns></returns>
        [OperationContract]
        SmcErr KeepAliver();

        /// <summary>
        /// 分配监控通道
        /// </summary>
        /// <param name="ChannelInfoList"></param>
        /// <returns></returns>
        [OperationContract]
        SmcErr UpdateMonitorChannel(List<ChannelInfo> channelInfoList);

        /// <summary>
        /// 同步监控通道
        /// </summary>
        /// <param name="ChannelInfoList"></param>
        /// <returns></returns>
        [OperationContract]
        SmcErr SyncMonitorChannel(ref List<ChannelInfo> channelInfoList);

        /// <summary>
        /// 获取监控设备列表
        /// </summary>
        /// <param name="CameraInfoList">摄像头基本信息</param>
        /// <param name="CameraGroupList">组基本信息</param>
        /// <param name="nodeRelationList">所属分组关系信息</param>
        /// <returns></returns>
        [OperationContract]
        SmcErr QueryMonitorCamera(PlatformType platformType, PageParam pageParam, out List<Camera> cameraInfoList, out List<CameraGroup> cameraGroupList, out List<NodeRelation> nodeRelationList, out PagesInfo pagesInfo);

        /// <summary>
        /// 刷新监控摄像头列表
        /// </summary>
        /// <returns></returns>
        [OperationContract]
        SmcErr RefreshMonitorCamera();

        /// <summary>
        /// 获取监控摄像头列表刷新状态，返回结果为0是表示刷新完毕，为1是刷新操作中。当查询刷新状态为0时，可调用获取监控摄像头列表接口，获取刷新后监控摄像头列表
        /// </summary>
        /// <param name="refreshStatus">返回值，刷新状态</param>
        /// <returns></returns>
        [OperationContract]
        SmcErr GetRefreshStatus(out SmcErr refreshStatus);

        /// <summary>
        /// 设置监控通道视频源
        /// </summary>
        /// <param name="channelLabel">通道label</param>
        /// <param name="cameraNo">摄像头NO</param>
        /// <returns></returns>
        [OperationContract]
        SmcErr SwitchChannelVideoSource(string channelLabel, string cameraNo);

        /// <summary>
        /// 开始摄像头PTZ操作
        /// </summary>
        /// <param name="cameraNo">摄像头NO</param>
        /// <param name="ptzCommand">操作类型</param>
        /// <param name="param">命令参数</param>
        /// <returns></returns>
        [OperationContract]
        SmcErr StartControlPtz(string cameraNo, PtzCommandType ptzCommand, int param);

        /// <summary>
        /// 关闭PTZ操作
        /// </summary>
        /// <param name="Camerano">摄像头</param>
        /// <returns></returns>
        [OperationContract]
        SmcErr StopControlPtz(string cameraNo, PtzCommandType ptzCommandType);


        /// <summary>
        /// 设置麦克风
        /// </summary>
        /// <param name="cameraNo"></param>
        /// <param name="isOn"></param>
        /// <returns></returns>
        [OperationContract]
        SmcErr SetMic(string channelLabel, string cameraNo, bool isOn);

        /// <summary>
        /// 设置扬声器
        /// </summary>
        /// <param name="cameraNo"></param>
        /// <param name="isOn"></param>
        /// <returns></returns>
        [OperationContract]
        SmcErr SetSpeaker(string channelLabel, string cameraNo, bool isOn);

        /// <summary>
        /// 获取软终端被叫sip端口号
        /// </summary>
        /// <param name="sipPort"></param>
        /// <returns></returns>
        [OperationContract]
        SmcErr GetSipPort(out uint sipPort);

        /// <summary>
        /// 导入网关license文件
        /// </summary>
        /// <param name="license"></param>
        /// <returns></returns>
        [OperationContract]
        SmcErr ImportCGWLicense(string license);
    }
}
