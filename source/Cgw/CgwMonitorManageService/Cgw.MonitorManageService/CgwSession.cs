/*-----------------------------------------------------------------------
//文件名：CgwSession.cs
//版权：Copyright 2011-2012 Huawei Tech. Co. Ltd. All Rights Reserved. 
//作者：w00206574
//日期：2014-6-12
//描述：session类
//---------------------------------------------------------------------*/
using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;

namespace CgwMonitorManage.Service
{
    public class CgwSession
    {
        /// <summary>
        /// 最后访问时间
        /// 用来验证用户访问超时。
        /// </summary>
        public DateTime LastVisitedTime { get; set; }

        /// <summary>
        /// 当前Session的唯一标识
        /// </summary>
        public Guid SessionGuid;

        public string SessionId;

        /// <summary>
        /// 构造函数
        /// </summary>
        /// <param name="sessionGuid">唯一标识</param>
        public CgwSession(Guid sessionGuid, string sessionId)
        {
            SessionGuid = sessionGuid;
            SessionId = sessionId;
            LastVisitedTime = DateTime.Now;
            SessionManage.Instance().AddSession(sessionGuid, this);
        }
    }
}