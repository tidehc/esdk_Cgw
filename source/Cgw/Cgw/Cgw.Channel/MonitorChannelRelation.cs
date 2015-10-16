/*-----------------------------------------------------------------------
//文件名：MonitorChannelRelation.cs
//版权：Copyright 2011-2012 Huawei Tech. Co. Ltd. All Rights Reserved. 
//作者：fangmingliang f00204327
//日期：2013-3-12
//描述：监控通道关系
//---------------------------------------------------------------------*/
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Cgw.Channel
{
    /// <summary>
    /// 通道关系表项
    /// </summary>
    public class MonitorChannelRelation
    {
        /// <summary>
        /// 监控通道号码
        /// </summary>
        public string Label { get; set; }

        /// <summary>
        /// 会议No
        /// </summary>
        public string ConfAccessCode { get; set; }

        /// <summary>
        /// 摄像头编号
        /// </summary>
        public string CameraNo { get; set; }

        /// <summary>
        /// 是否有效
        /// </summary>
        public bool IsValid { get; set; }

        /// <summary>
        /// 构造函数
        /// </summary>
        /// <param name="channelNo">通道号码</param>
        public MonitorChannelRelation(string channelLabel)
        {
            this.Label = channelLabel;
            this.ConfAccessCode = string.Empty;
            this.CameraNo = string.Empty;
            this.IsValid = true;
        }
    }
}
