/*-----------------------------------------------------------------------
//文件名：Camera.cs
//版权：Copyright 2011-2012 Huawei Tech. Co. Ltd. All Rights Reserved. 
//作者：kf70422
//日期：2013-2-27
//描述：摄像头信息
//---------------------------------------------------------------------*/
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;
using System.ServiceModel;

namespace Cgw.Common
{
    /// <summary>
    /// 摄像头信息
    /// </summary>
    [DataContract]
    public class Camera
    {
        /// <summary>
        /// 摄像头编号
        /// </summary>
        [DataMember]
        public string No { get; set; }

        /// <summary>
        /// 摄像头名称
        /// </summary>
        [DataMember]
        public string Name { get; set; }

        /// <summary>
        /// 摄像头状态 在线/不在线
        /// </summary>
        [DataMember]
        public CameraStatus Status { get; set; }

        /// <summary>
        /// 是否闭音
        /// </summary>
        [DataMember]
        public bool IsMute { get; set; }

        /// <summary>
        /// 是否静音
        /// </summary>
        [DataMember]
        public bool IsQuiet { get; set; }

        /// <summary>
        /// 父节点ID
        /// </summary>
        [DataMember]
        public string ParentID { get; set; }

        /// <summary>
        /// 设备类型 00:共享主设备类型 01:共享摄像机类型
        /// </summary>
        [DataMember]
        public string DeviceType { get; set; }

        /// <summary>
        /// 构造函数
        /// </summary>
        public Camera(string no, string name,CameraStatus state = CameraStatus.Online)
        {
            this.No = no;
            this.Name = name;
            this.Status = state;
            this.IsMute = true;
            this.IsQuiet = true;
        }
    }


    /// <summary>
    /// 摄像头状态
    /// </summary>
    [DataContract]
    public enum CameraStatus
    {
        /// <summary>
        /// 不在线
        /// </summary>
        [EnumMember]
        Offline = 0,

        /// <summary>
        /// 在线
        /// </summary>
        [EnumMember]
        Online = 1
    }
}
