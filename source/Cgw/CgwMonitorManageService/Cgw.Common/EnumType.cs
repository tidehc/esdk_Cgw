/*-----------------------------------------------------------------------
//文件名：EnumType.cs
//版权：Copyright 2011-2012 Huawei Tech. Co. Ltd. All Rights Reserved. 
//作者：f00204327
//日期：2013-3-22
//描述：公共枚举类
//---------------------------------------------------------------------*/
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace CgwMonitorManage.Common
{
    /// <summary>
    /// 通道操作类型
    /// </summary>
    public enum ChannelControlType
    {
        /// <summary>
        /// 不做任何操作
        /// </summary>
        None,
        /// <summary>
        /// 添加通道
        /// </summary>
        Add,

        /// <summary>
        /// 删除通道
        /// </summary>
        Remove
    }

    /// <summary>
    /// 日志级别 
    /// </summary>
    public enum NLogLevel
    {
        /// <summary>
        /// 错误
        /// </summary>
        Error,

        /// <summary>
        /// 警告
        /// </summary>
        Warn,

        /// <summary>
        /// 信息
        /// </summary>
        Info,

        /// <summary>
        /// 调试
        /// </summary>
        Trace
    }

}
