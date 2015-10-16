/*-----------------------------------------------------------------------
//文件名：Device.cs
//版权：Copyright 2011-2012 Huawei Tech. Co. Ltd. All Rights Reserved. 
//作者：kf70422
//日期：2013-3-18
//描述：流媒体服务器（天地伟业叫中间件服务器）信息，
//---------------------------------------------------------------------*/
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using CgwMonitorManage.Common;

namespace CgwMonitorManage.Tiandy
{
    /// <summary>
    /// 天地伟业摄像头信息
    /// </summary>
    internal class TiandyCamera : Camera
    {
        /// <summary>
        /// 主机ID，该摄像头所在的主机/编码器的IP
        /// </summary>
        public string HostNo { get; set; }

        /// <summary>
        /// 摄像头对应的通道
        /// </summary>
        public int Channel { get; set; }

        /// <summary>
        /// 构造函数
        /// </summary>
        public TiandyCamera(string no, string name)
            : base(no, name)
        {
            this.No = no;
            this.Name = name;
        }

    }

    /// <summary>
    /// 主机信息，通道必须登记在主机下面
    /// </summary>
    internal class Host
    {
        /// <summary>
        /// 主机Id
        /// </summary>
        public string No { get; set; }

        /// <summary>
        /// 主机登陆用户名
        /// </summary>
        public string User { get; set; }

        /// <summary>
        /// 主机登陆用户密码
        /// </summary>
        public string Pwd { get; set; }

        /// <summary>
        /// 流媒体服务器（设备接入服务器）Id
        /// </summary>
        public string MediaServerNo { get; set; }

        /// <summary>
        /// 构造函数
        /// </summary>
        public Host(string No, string User, string Pwd, string MediaServerNo)
        {
            this.No = No;
            this.MediaServerNo = MediaServerNo;
        }

    }

    /// <summary>
    /// 流媒体服务器（天地伟业叫中间件服务器）信息
    /// </summary>
    internal class MediaServer
    {
        /// <summary>
        /// 服务器Id
        /// </summary>
        public string No { get; set; }

        /// <summary>
        /// 服务器Ip
        /// </summary>
        public string Ip { get; set; }

        /// <summary>
        /// 服务器port
        /// </summary>
        public int Port { get; set; }

        /// <summary>
        /// 服务器登陆用户名
        /// </summary>
        public string User { get; set; }

        /// <summary>
        /// 服务器登陆用户密码
        /// </summary>
        public string Pwd { get; set; }


        /// <summary>
        /// 构造函数
        /// </summary>
        public MediaServer(string No, string Ip, int Port, string User, string Pwd)
        {
            this.No = No;
            this.Ip = Ip;
            this.Port = Port;
            this.User = User;
            this.Pwd = Pwd;
        }
    }
}
