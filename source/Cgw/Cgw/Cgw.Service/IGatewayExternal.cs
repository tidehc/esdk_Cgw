
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Serialization;
using System.ServiceModel;
using System.Text;
using Cgw.SmcError;
using Cgw.Common;

namespace Cgw.External
{
    /// <summary>
    /// 融合网关对外WCF接口
    /// </summary>
    [ServiceContract(SessionMode = SessionMode.Required)]
    public interface IGatewayExternal
    {


        /// <summary>
        /// 连接融合网关
        /// </summary>
        /// <param name="password">加密后的密码字符串</param>
        /// <returns></returns>
        [OperationContract]
        SmcErr Connect(string password);

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
        SmcErr AssignMonitorChannel(List<ChannelInfo> ChannelInfoList);

        /// <summary>
        /// 同步监控通道
        /// </summary>
        /// <param name="ChannelInfoList"></param>
        /// <returns></returns>
        [OperationContract]
        SmcErr SyncMonitorChannel(List<ChannelInfo> ChannelInfoList);

        /// <summary>
        /// 获取监控设备列表
        /// </summary>
        /// <param name="CameraInfoList">摄像头基本信息</param>
        /// <param name="CameraGroupList">组基本信息</param>
        /// <param name="nodeRelationList">所属分组关系信息</param>
        /// <returns></returns>
        [OperationContract]
        SmcErr QueryMonitorCamera(out List<Camera> CameraInfoList, out List<CameraGroup> CameraGroupList, out List<NodeRelation> nodeRelationList);

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
        SmcErr StopPTZ(string Camerano);
    }

    public class RegisterServicesClient : System.ServiceModel.ClientBase<IGatewayExternal>
    {
        public RegisterServicesClient()
        {
        }

        public RegisterServicesClient(string endpointConfigurationName) :
            base(endpointConfigurationName)
        {
        }

        public RegisterServicesClient(string endpointConfigurationName, string remoteAddress) :
            base(endpointConfigurationName, remoteAddress)
        {
        }

        public RegisterServicesClient(string endpointConfigurationName, System.ServiceModel.EndpointAddress remoteAddress) :
            base(endpointConfigurationName, remoteAddress)
        {
        }

        public RegisterServicesClient(System.ServiceModel.Channels.Binding binding, System.ServiceModel.EndpointAddress remoteAddress) :
            base(binding, remoteAddress)
        {
        }

        public IGatewayExternal services
        {
            get
            {
                return base.Channel;
            }
        }
    }




}
