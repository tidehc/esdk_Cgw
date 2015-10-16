/*-----------------------------------------------------------------------
//文件名：IVideoMonitor.cs
//版权：Copyright 2011-2012 Huawei Tech. Co. Ltd. All Rights Reserved. 
//作者：kf70422
//日期：2013-2-27
//描述：监控平台统一接口
//---------------------------------------------------------------------*/
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;
using CgwMonitorManage.SmcError;

namespace CgwMonitorManage.Common
{
    public interface IVideoMonitor
    {
        /// <summary>
        /// 加载监控平台
        /// </summary>
        /// <param name="monitorConfigNode">监控平台配置节点</param>
        /// <returns></returns>
        SmcErr Load(XmlElement monitorConfigNode);

        /// <summary>
        /// 卸载监控平台
        /// </summary>
        /// <returns></returns>
        SmcErr Unload();

        /// <summary>
        /// 注册码流回调函数
        /// </summary>
        /// <param name="dataCallBack">回调函数</param>
        /// <param name="sender">调用者</param>
        /// <returns></returns>
        void SetDataCallBackFunc(DataCallBack dataCallBack, string sender);

        /// <summary>
        /// 获取摄像头列表及分组信息
        /// </summary>
        /// <param name="fromMonitorSys">如果该值为true，则实时从监控平台获取,否则从融合网关缓存获取</param>
        /// <param name="cameraList">摄像头列表</param>
        /// <param name="groupList">组信息</param>
        /// <param name="nodeRelationList">分组关系</param>
        /// <returns></returns>
        SmcErr GetAllCameras(out List<Camera> cameraList, out List<CameraGroup> groupList, out List<NodeRelation> nodeRelationList);

        /// <summary>
        /// 预览摄像头
        /// </summary>
        /// <param name="cameraNo">摄像头编号</param>
        /// <returns></returns>
        SmcErr StartReceiveVideo(string cameraNo);

        /// <summary>
        ///停止预览摄像头
        /// </summary>
        /// <param name="cameraNo">摄像头编号</param>
        /// <returns></returns>
        SmcErr StopReceiveVideo(string cameraNo);

        /// <summary>
        /// 开始云台控制，摄像头控制
        /// </summary>
        /// <param name="cameraNo">摄像头编号</param>
        /// <param name="ptzCommandType">命令类型</param>
        /// <param name="param">命令参数，速度或倍数</param>
        /// <returns></returns>
        SmcErr StartControlPtz(string cameraNo, PtzCommandType ptzCommandType, int param);

        /// <summary>
        /// 停止云台控制，摄像头控制
        /// </summary>
        /// <param name="cameraNo">摄像头编号</param>
        /// <param name="ptzCommandType">命令类型</param>
        /// <returns></returns>
        SmcErr StopControlPtz(string cameraNo, PtzCommandType ptzCommandType);

        /// <summary>
        /// 重发I帧
        /// </summary>
        /// <param name="cameraNo">摄像头编号</param>
        /// <returns></returns>
        SmcErr MakeIFrame(string cameraNo);

        /// <summary>
        /// 设置扬声器状态
        /// </summary>
        /// <param name="cameraNo">摄像头编号</param>
        /// <param name="isOn">扬声器是否开启</param>
        /// <returns></returns>
        SmcErr SetSpeaker(string cameraNo, bool isOn);

        /// <summary>
        /// 设置麦克风状态
        /// </summary>
        /// <param name="cameraNo">摄像头编号</param>
        /// <param name="isOn">麦克风是否开启</param>
        /// <returns></returns>
        SmcErr SetMic(string cameraNo, bool isOn);

        /// <summary>
        /// 刷新监控摄像头列表
        /// </summary>
        SmcErr RefreshMonitorCamera();

        /// <summary>
        /// 获取监控摄像头列表刷新状态，返回结果为0是表示刷新完毕，为1是刷新操作中。当查询刷新状态为0时，可调用获取监控摄像头列表接口，获取刷新后监控摄像头列表
        /// </summary>
        /// <param name="refreshStatus">刷新状态</param>
        /// <returns></returns>
        SmcErr GetRefreshStatus(out SmcErr refreshStatus);
    }

    /// <summary>
    /// 码流回调委托
    /// </summary>
    /// <param name="cameraNo">摄像头编号</param>
    /// <param name="mediaData">码流</param>
    /// <param name="sender">调用者,区别哪个平台的回调</param>
    public delegate void DataCallBack(string cameraNo, MediaData mediaData, string sender);
}
