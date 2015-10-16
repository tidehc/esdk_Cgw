/*-----------------------------------------------------------------------
//文件名：IMonitorManageService.cs
//版权：Copyright 2011-2012 Huawei Tech. Co. Ltd. All Rights Reserved. 
//作者：w00206574
//日期：2014-6-12
//描述：用于定义对外开放wcf接口
//---------------------------------------------------------------------*/
using System.Runtime.Serialization;
using System.ServiceModel;
using System.Text;
using CgwMonitorManage.SmcError;
using CgwMonitorManage.Common;
using System.Collections.Generic;

namespace CgwMonitorManage.Service
{
    [ServiceContract(SessionMode = SessionMode.Required)]
    public interface IMonitorManageService
    {
        /// <summary>
        /// 连接监控平台管理服务
        /// </summary>
        /// <param name="password">加密后的密码字符串</param>
        /// <returns></returns>
        [OperationContract]
        SmcErr Connect(out string sessionID);

        /// <summary>
        /// 断开监控平台管理服务
        /// </summary>
        /// <returns></returns>
        [OperationContract]
        SmcErr Disconnect();

        /// <summary>
        /// 获取监控设备列表
        /// </summary>
        /// <param name="CameraInfoList">摄像头基本信息</param>
        /// <param name="CameraGroupList">组基本信息</param>
        /// <param name="nodeRelationList">所属分组关系信息</param>
        /// <returns></returns>
        [OperationContract]
        SmcErr GetAllCameras(PlatformType platformType, PagesParam pageParam, out List<Camera> cameraList, out List<CameraGroup> groupList, out List<NodeRelation> nodeRelationList, out PageInfo pagesInfo, PlatformLicenseInfo licenseInfo = null);

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
        /// 预览摄像头视频，使用命名管道方式接收数据
        /// </summary>
        /// <param name="cameraNo"></param>
        /// <param name="pipeName"></param>
        /// <returns></returns>
        [OperationContract]
        SmcErr StartReceiveVideoByPipe(string cameraNo, out string pipeName);

        /// <summary>
        /// 停止预览摄像头视频，并清理命名管道
        /// </summary>
        /// <param name="cameraNo"></param>
        /// <returns></returns>
        [OperationContract]
        SmcErr StopReceiveVideoByPipe(string cameraNo);

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
        SmcErr SetMic(string cameraNo, bool isOn);

        /// <summary>
        /// 设置扬声器
        /// </summary>
        /// <param name="cameraNo"></param>
        /// <param name="isOn"></param>
        /// <returns></returns>
        [OperationContract]
        SmcErr SetSpeaker(string cameraNo, bool isOn);

        /// <summary>
        /// 重发I帧（暂不支持）
        /// </summary>
        /// <param name="cameraNo">摄像头编号</param>
        /// <returns></returns>
        [OperationContract]
        SmcErr MakeIFrame(string cameraNo);

        /// <summary>
        /// 保持连接
        /// </summary>
        /// <returns></returns>
        [OperationContract]
        SmcErr KeepAliver(string sessionID);
    }
}
