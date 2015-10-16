using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace CgwMonitorManage.eLTE
{
    /// <summary>
    /// eLTE登陆信息
    /// </summary>
    public class eLTELoginInfo
    {
        /// <summary>
        /// IP地址
        /// </summary>
        public string Ip { get; set; }

        /// <summary>
        /// 端口
        /// </summary>
        public int Port { get; set; }

        /// <summary>
        /// 用户
        /// </summary>
        public string User { get; set; }

        /// <summary>
        /// 密码
        /// </summary>
        public string Pwd { get; set; }

        /// <summary>
        /// PTT用户
        /// </summary>
        public bool PTTUSER { get; set; }

        /// <summary>
        /// 固定摄像头
        /// </summary>
        public bool FIXEDCAMERA { get; set; }

        /// <summary>
        /// 是否是第一次登录
        /// </summary>
        public bool NotFirstLogin { get; set; }

    }
}
