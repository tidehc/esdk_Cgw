using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Collections;
using NLog;
using Cgw.SmcError;

namespace Cgw.NLogEx
{
   
    /// <summary>
    /// 日志错误记录
    /// </summary>
    public class LoggerEx
    {
        #region 私有成员
        /// <summary>
        /// 日志对象
        /// </summary>
        private NLog.Logger Log;

        /// <summary>
        /// 接口字典
        /// </summary>
        private IDictionary<string, object> Properties;
        #endregion

        #region 日志基础属性
        /////// <summary>
        /////// 操作信息，包含用户ID，和本次请求的GUID。
        /////// </summary>
        ////public OperatorInfo OpInfo
        ////{
        ////    set
        ////    {
        ////        this.Properties["userid"] = value.userId;
        ////        if (value.requestSerial != null)
        ////        {
        ////            this.Properties["requestid"] = value.requestSerial;
        ////        }
        ////    }
        ////}

        ///// <summary>
        ///// MCU ID。
        ///// </summary>
        //public uint McuId
        //{
        //    set
        //    {
        //        this.Properties["mcuid"] = value;
        //    }
        //}

        ///// <summary>
        ///// 录播服务器 ID。
        ///// </summary>
        //public uint NrsId
        //{
        //    set
        //    {
        //        this.Properties["nrsid"] = value;
        //    }
        //}

        ///// <summary>
        ///// 会议ID。各种会议ID，包含预定义会议，活动会议等。
        ///// </summary>
        //public uint ConfId
        //{
        //    set
        //    {
        //        this.Properties["confid"] = value;
        //    }
        //}

        ///// <summary>
        ///// 子会议ID
        ///// </summary>
        //public uint SubConfId
        //{
        //    set
        //    {
        //        this.Properties["SubConfId"] = value;
        //    }
        //}

        ///// <summary>
        ///// 会议的GUID。主要提供活动会议使用。
        ///// </summary>
        //public Guid ConfGuid
        //{
        //    set
        //    {
        //        this.Properties["confguid"] = value;
        //    }
        //}

        ///// <summary>
        ///// 会场ID。各种会场ID，包含预定义会场，活动会场ID。
        ///// </summary>
        //public uint SiteId
        //{
        //    set
        //    {
        //        this.Properties["siteid"] = value;
        //    }
        //}

        ///// <summary>
        ///// 会场的GUID。主要提供活动会场使用。
        ///// </summary>
        //public Guid SiteGuid
        //{
        //    set
        //    {
        //        this.Properties["siteguid"] = value;
        //    }
        //}

        ///// <summary>
        ///// GK ID。
        ///// </summary>
        //public uint GkId
        //{
        //    set
        //    {
        //        this.Properties["gkid"] = value;
        //    }
        //}

        ///// <summary>
        ///// 节点前台ID。
        ///// </summary>
        //public uint EndpointFrontId
        //{
        //    set
        //    {
        //        this.Properties["endpointFrontId"] = value;
        //    }
        //}

        ///// <summary>
        ///// 节点后台ID。
        ///// </summary>
        //public uint EndpointBackId
        //{
        //    set
        //    {
        //        this.Properties["endpointBackId"] = value;
        //    }
        //}

        ///// <summary>
        ///// 邻居GK ID。
        ///// </summary>
        //public uint GkRouteId
        //{
        //    set
        //    {
        //        this.Properties["gkRouteId"] = value;
        //    }
        //}

        ///// <summary>
        ///// 私网区域ID。
        ///// </summary>
        //public uint PrivatZoneId
        //{
        //    set
        //    {
        //        this.Properties["privateZoneId"] = value;
        //    }
        //}

        ///// <summary>
        ///// 私网区域规则ID。
        ///// </summary>
        //public uint ZoneRuleId
        //{
        //    set
        //    {
        //        this.Properties["zoneRuleId"] = value;
        //    }
        //}

        ///// <summary>
        ///// 网络地准备ID。
        ///// </summary>
        //public uint NetworkAddrId
        //{
        //    set
        //    {
        //        this.Properties["networkAddrId"] = value;
        //    }
        //}

        ///// <summary>
        ///// 组织ID。
        ///// </summary>
        //public string OrgId
        //{
        //    set
        //    {
        //        this.Properties["orgId"] = value;
        //    }
        //}

        ///// <summary>
        ///// 计费码ID。
        ///// </summary>
        //public uint BillCodeId
        //{
        //    set
        //    {
        //        this.Properties["billCodeId"] = value;
        //    }
        //}

        ///// <summary>
        ///// 服务区ID。
        ///// </summary>
        //public uint DomainId
        //{
        //    set
        //    {
        //        this.Properties["domainId"] = value;
        //    }
        //}

        ///// <summary>
        ///// 服务区字冠ID。
        ///// </summary>
        //public uint DomainCodeId
        //{
        //    set
        //    {
        //        this.Properties["domainCodeId"] = value;
        //    }
        //}

        ///// <summary>
        ///// 用户类型ID。
        ///// </summary>
        //public uint PermissionGroupId
        //{
        //    set
        //    {
        //        this.Properties["permissionGroupId"] = value;
        //    }
        //}

        /// <summary>
        /// 摄像机号码
        /// </summary>
        public string CameraNo
        {
            set
            {
                this.Properties["cameraNo"] = value;
            }
        }

        /// <summary>
        /// 会场label
        /// </summary>
        public string SiteLabel
        {
            set
            {
                this.Properties["siteLabel"] = value;
            }
        }

        /// <summary>
        /// 设备GuiD
        /// </summary>
        public int DeviceGuid
        {
            set
            {
                this.Properties["deviceguid"] = value;
            }
        }

        /// <summary>
        /// 日志错误码
        /// </summary>
        public uint ErrNo
        {
            set
            {
                this.Properties["errno"] = value;
            }
        }

        /// <summary>
        /// 日志记录模块
        /// </summary>
        public string Model
        {
            set
            {
                this.Properties["model"] = value;
            }
        }

        /// <summary>
        /// 日志描述的参数
        /// </summary>
        public string Param
        {
            set
            {
                this.Properties["param"] = value;
            }
        }
        #endregion

        #region 构造函数
        /// <summary>
        /// LoggerEx扩展类的构造函数。
        /// </summary>
        /// <param name="logger">传入NLog的logger对象</param>
        public LoggerEx(NLog.Logger logger)
        {
            this.Log = logger;
            this.Properties = new Dictionary<string, object>();
        }

        ///// <summary>
        ///// LoggerEx扩展类的构造函数。
        ///// </summary>
        ///// <param name="logger">传入NLog的logger对象</param>
        //public LoggerEx(NLog.Logger logger,string logType,NlogIdType nlogIdType)
        //{
        //    this.Log = logger;
        //    this.Properties = new Dictionary<string, object>();
        //    if(NlogIdType.CameraNo==nlogIdType)
        //    {
        //    this.CameraNo=logType;
        //    }
        //    if (NlogIdType.SiteLabel == nlogIdType)
        //    {
        //        this.SiteLabel = logType;
        //    }
        //}

        ///// <summary>
        ///// LoggerEx扩展类的构造函数。
        ///// </summary>
        ///// <param name="logger">传入NLog的logger对象</param>
        ///// <param name="opUser">操作用户</param>
        //public LoggerEx(NLog.Logger logger, OperatorInfo opUser)
        //{
        //    this.Log = logger;
        //    this.Properties = new Dictionary<string, object>();
        //    if (opUser != null)
        //    {
        //        this.OpInfo = opUser;
        //    }
        //}

        ///// <summary>
        ///// LoggerEx扩展类的构造函数。GuidOperateId
        ///// </summary>
        ///// <param name="logger">传入NLog的logger对象</param>
        ///// <param name="opUser">操作用户</param>
        ///// <param name="orgId">组织ID</param>
        //public LoggerEx(NLog.Logger logger, OperatorInfo opUser, string orgId)
        //{
        //    this.Log = logger;
        //    this.Properties = new Dictionary<string, object>();
        //    if (opUser != null)
        //    {
        //        this.OpInfo = opUser;
        //    }

        //    this.OrgId = orgId;
        //}

        ///// <summary>
        ///// LoggerEx扩展类的构造函数,R1
        ///// </summary>
        ///// <param name="logger"></param>
        ///// <param name="opUser"></param>
        ///// <param name="siteGuid"></param>
        //public LoggerEx(NLog.Logger logger, OperatorInfo opUser, Guid siteGuid)
        //{
        //    this.Log = logger;
        //    this.Properties = new Dictionary<string, object>();
        //    if (opUser != null)
        //    {
        //        this.OpInfo = opUser;
        //    }

        //    this.DeviceGuid = siteGuid;
        //}

        ///// <summary>
        ///// LoggerEx扩展类的构造函数,R2
        ///// </summary>
        ///// <param name="logger">logger</param>
        ///// <param name="deviceGuid">设备guid</param>
        ///// <param name="opUser">操作者</param>
        //public LoggerEx(NLog.Logger logger, Guid deviceGuid, OperatorInfo opUser)
        //{
        //    this.Log = logger;
        //    this.Properties = new Dictionary<string, object>();
        //    if (opUser != null)
        //    {
        //        this.OpInfo = opUser;
        //    }

        //    this.DeviceGuid = deviceGuid;
        //}

        ///// <summary>
        ///// LoggerEx扩展类的构造函数
        ///// </summary>
        ///// <param name="logger"></param>
        ///// <param name="opUser"></param>
        ///// <param name="guid"></param>
        //public LoggerEx(NLog.Logger logger, OperatorInfo opUser, GuidOperateId guid)
        //{
        //    this.Log = logger;
        //    this.Properties = new Dictionary<string, object>();
        //    if (opUser != null)
        //    {
        //        this.OpInfo = opUser;
        //    }

        //    if (null != guid)
        //    {
        //        this.DeviceGuid = guid.guid;
        //    }
        //}

        ///// <summary>
        ///// LoggerEx扩展类的构造函数。
        ///// </summary>
        ///// <param name="logger">传入NLog的logger对象</param>
        ///// <param name="opUser">操作用户</param>
        ///// <param name="id">操作对象的ID</param>
        //public LoggerEx(NLog.Logger logger, OperatorInfo opUser, uint id, NlogIdType idType)
        //{
        //    this.Log = logger;
        //    this.Properties = new Dictionary<string, object>();
        //    if (opUser != null)
        //    {
        //        this.OpInfo = opUser;
        //    }

        //    if (NlogIdType.ConfId == idType)
        //    {
        //        this.ConfId = id;
        //    }
        //    else if (NlogIdType.BillCodeId == idType)
        //    {
        //        this.BillCodeId = id;
        //    }
        //    else if (NlogIdType.DomainCodeId == idType)
        //    {
        //        this.DomainCodeId = id;
        //    }
        //    else if (NlogIdType.DomainId == idType)
        //    {
        //        this.DomainId = id;
        //    }
        //    else if (NlogIdType.EndpointBackId == idType)
        //    {
        //        this.EndpointBackId = id;
        //    }
        //    else if (NlogIdType.EndpointFrontId == idType)
        //    {
        //        this.EndpointFrontId = id;
        //    }
        //    else if (NlogIdType.GkId == idType)
        //    {
        //        this.GkId = id;
        //    }
        //    else if (NlogIdType.GkRouteId == idType)
        //    {
        //        this.GkRouteId = id;
        //    }
        //    else if (NlogIdType.McuId == idType)
        //    {
        //        this.McuId = id;
        //    }
        //    else if (NlogIdType.NrsId == idType)
        //    {
        //        this.NrsId = id;
        //    }
        //    else if (NlogIdType.NetworkAddrId == idType)
        //    {
        //        this.NetworkAddrId = id;
        //    }
        //    else if (NlogIdType.PermissionGroupId == idType)
        //    {
        //        this.PermissionGroupId = id;
        //    }
        //    else if (NlogIdType.PrivatZoneId == idType)
        //    {
        //        this.PrivatZoneId = id;
        //    }
        //    else if (NlogIdType.SiteId == idType)
        //    {
        //        this.SiteId = id;
        //    }
        //    else if (NlogIdType.SubConfId == idType)
        //    {
        //        this.SubConfId = id;
        //    }
        //    else if (NlogIdType.ZoneRuleId == idType)
        //    {
        //        this.ZoneRuleId = id;
        //    }
        //}

        ///// <summary>
        ///// LoggerEx扩展类的构造函数。
        ///// </summary>
        ///// <param name="logger">传入NLog的logger对象</param>
        ///// <param name="opUser">操作用户</param>
        ///// <param name="confId">会议Id</param>
        ///// <param name="siteId">会场Id</param>
        //public LoggerEx(NLog.Logger logger, OperatorInfo opUser, uint confId, uint siteId)
        //{
        //    this.Log = logger;
        //    this.Properties = new Dictionary<string, object>();
        //    if (opUser != null)
        //    {
        //        this.OpInfo = opUser;
        //    }

        //    this.ConfId = confId;
        //    this.SiteId = siteId;
        //}

        ///// <summary>
        ///// LoggerEx扩展类的构造函数。
        ///// </summary>
        ///// <param name="logger">传入NLog的logger对象</param>
        ///// <param name="opUser">操作用户</param>
        ///// <param name="confId">会议Id</param>
        //public LoggerEx(NLog.Logger logger, OperatorInfo opUser, uint confId)
        //{
        //    this.Log = logger;
        //    this.Properties = new Dictionary<string, object>();
        //    if (opUser != null)
        //    {
        //        this.OpInfo = opUser;
        //    }

        //    this.ConfId = confId;
        //}

        ///// <summary>
        ///// LoggerEx扩展类的构造函数。
        ///// </summary>
        ///// <param name="logger">传入NLog的logger对象</param>
        ///// <param name="opUser">操作用户</param>
        ///// <param name="gkId">GKId</param>
        ///// <param name="oneId">第一个ID，一般指父ID</param>
        ///// <param name="secondId">第二个ID，一般指子ID</param>
        ///// <param name="oneIdType"></param>
        ///// <param name="secondIdType"></param>
        //public LoggerEx(NLog.Logger logger, OperatorInfo opUser, uint gkId, uint oneId, uint secondId, NlogIdType oneIdType, NlogIdType secondIdType)
        //{
        //    this.Log = logger;
        //    this.Properties = new Dictionary<string, object>();
        //    if (opUser != null)
        //    {
        //        this.OpInfo = opUser;
        //    }

        //    this.GkId = gkId;
        //    if ((NlogIdType.EndpointBackId == oneIdType) && (NlogIdType.EndpointFrontId == secondIdType))
        //    {
        //        this.EndpointFrontId = oneId;
        //        this.EndpointBackId = secondId;
        //    }
        //    else if ((NlogIdType.PrivatZoneId == oneIdType) && (NlogIdType.ZoneRuleId == secondIdType))
        //    {
        //        this.PrivatZoneId = oneId;
        //        this.ZoneRuleId = secondId;
        //    }
        //}

        ///// <summary>
        ///// LoggerEx扩展类的构造函数。
        ///// </summary>
        ///// <param name="logger">传入NLog的logger对象</param>
        ///// <param name="opUser">操作用户</param>
        ///// <param name="gkId">GKId</param>
        ///// <param name="id">GK下的操作对象ID</param>
        //public LoggerEx(NLog.Logger logger, OperatorInfo opUser, uint gkId, uint id, NlogIdType idType)
        //{
        //    this.Log = logger;
        //    this.Properties = new Dictionary<string, object>();
        //    if (opUser != null)
        //    {
        //        this.OpInfo = opUser;
        //    }

        //    this.GkId = gkId;
        //    if (NlogIdType.PrivatZoneId == idType)
        //    {
        //        this.PrivatZoneId = id;
        //    }
        //    else if (NlogIdType.NetworkAddrId == idType)
        //    {
        //        this.NetworkAddrId = id;
        //    }
        //}
        #endregion

        #region 日志
        /// <summary>
        /// 清除当前绑定的扩展字段信息
        /// </summary>
        /// <remarks>
        /// 建议如果需要删除部分字段，建议使用该函数后再进行新的赋值。
        /// 如果只是对已有扩展字段修改，则直接对该字段重新赋值即可。
        /// </remarks>
        public void ClearProperties()
        {
            if (this.Properties.Count != 0)
            {
                this.Properties.Clear();
            }
        }

        /// <summary>
        /// Trace类日志输出。
        /// </summary>
        /// <param name="message">message</param>
        /// <param name="args">args</param>
        public void Trace(string message, params object[] args)
        {
            if (this.Log.IsTraceEnabled)
            {
                this.WriteToLog(LogLevel.Trace, null, message, args);
            }
        }

        /// <summary>
        /// 带异常信息的Trace类日志输出。
        /// </summary>
        /// <param name="err">错误码,即信息</param>
        /// <param name="e">e</param>
        /// <param name="message">message</param>
        /// <param name="args">args</param>
        public void Trace(SmcErr err, string message, params object[] args)
        {
            if (this.Log.IsTraceEnabled)
            {
                this.WriteToLog(LogLevel.Trace, err, null, message, args);
            }
        }

        /// <summary>
        /// 带异常信息的Trace类日志输出。
        /// </summary>
        /// <param name="e">e</param>
        /// <param name="message">message</param>
        /// <param name="args">args</param>
        public void Trace(Exception e, string message, params object[] args)
        {
            if (this.Log.IsTraceEnabled)
            {
                this.WriteToLog(LogLevel.Trace, e, message, args);
            }
        }

        /// <summary>
        /// 带异常信息的Trace类日志输出。
        /// </summary>
        /// <param name="err">错误码,即信息</param>
        /// <param name="e">e</param>
        /// <param name="message">message</param>
        /// <param name="args">args</param>
        public void Trace(SmcErr err, Exception e, string message, params object[] args)
        {
            if (this.Log.IsTraceEnabled)
            {
                this.WriteToLog(LogLevel.Trace, err, e, message, args);
            }
        }

        /// <summary>
        /// Debug类日志输出。
        /// </summary>
        /// <param name="message">message</param>
        /// <param name="args">args</param>
        public void Debug(string message, params object[] args)
        {
            if (this.Log.IsDebugEnabled)
            {
                this.WriteToLog(LogLevel.Debug, null, message, args);
            }
        }

        /// <summary>
        /// Debug类日志输出。
        /// </summary>
        /// <param name="err">错误码,即信息</param>
        /// <param name="message">message</param>
        /// <param name="args">args</param>
        public void Debug(SmcErr err, string message, params object[] args)
        {
            if (this.Log.IsDebugEnabled)
            {
                this.WriteToLog(LogLevel.Debug, err, null, message, args);
            }
        }

        /// <summary>
        /// 带异常信息的Debug日志输出。
        /// </summary>
        /// <param name="e">e</param>
        /// <param name="message">message</param>
        /// <param name="args">args</param>
        public void Debug(Exception e, string message, params object[] args)
        {
            if (this.Log.IsDebugEnabled)
            {
                this.WriteToLog(LogLevel.Debug, e, message, args);
            }
        }

        /// <summary>
        /// 带异常信息的Debug日志输出。
        /// </summary>
        /// <param name="err">错误码,即信息</param>
        /// <param name="e">e</param>
        /// <param name="message">message</param>
        /// <param name="args">args</param>
        public void Debug(SmcErr err, Exception e, string message, params object[] args)
        {
            if (this.Log.IsDebugEnabled)
            {
                this.WriteToLog(LogLevel.Debug, err, e, message, args);
            }
        }

        /// <summary>
        /// Info类日志输出。
        /// </summary>
        /// <param name="message">message</param>
        /// <param name="args">args</param>
        public void Info(string message, params object[] args)
        {
            if (this.Log.IsInfoEnabled)
            {
                this.WriteToLog(LogLevel.Info, null, message, args);
            }
        }

        /// <summary>
        /// Info类日志输出。
        /// </summary>
        /// <param name="err">错误码,即信息</param>
        /// <param name="message">message</param>
        /// <param name="args">args</param>
        public void Info(SmcErr err)
        {
            if (this.Log.IsInfoEnabled)
            {
                this.WriteToLog(LogLevel.Info, err, null, String.Empty, null);
            }
        }

        /// <summary>
        /// Info类日志输出。
        /// </summary>
        /// <param name="err">错误码</param>
        /// <param name="args">参数</param>
        public void Info(SmcErr err, params object[] args)
        {
            if (this.Log.IsInfoEnabled)
            {
                this.WriteToLog(LogLevel.Info, err, null, String.Empty, args);
            }
        }

        /// <summary>
        /// Info类日志输出。
        /// </summary>
        /// <param name="err">错误码,即信息</param>
        /// <param name="message">message</param>
        /// <param name="args">args</param>
        public void Info(SmcErr err, string message, params object[] args)
        {
            if (this.Log.IsInfoEnabled)
            {
                this.WriteToLog(LogLevel.Info, err, null, message, args);
            }
        }

        /// <summary>
        /// 带异常信息的Info类日志输出。
        /// </summary>
        /// <param name="e">e</param>
        /// <param name="message">message</param>
        /// <param name="args">args</param>
        public void Info(Exception e, string message, params object[] args)
        {
            if (this.Log.IsInfoEnabled)
            {
                this.WriteToLog(LogLevel.Info, e, message, args);
            }
        }

        /// <summary>
        /// 带异常信息的Info类日志输出。
        /// </summary>
        /// <param name="err">错误码,即信息</param>
        /// <param name="e">e</param>
        /// <param name="message">message</param>
        /// <param name="args">args</param>
        public void Info(SmcErr err, Exception e, string message, params object[] args)
        {
            if (this.Log.IsInfoEnabled)
            {
                this.WriteToLog(LogLevel.Info, err, e, message, args);
            }
        }

        /// <summary>
        /// Warn类日志输出。
        /// </summary>
        /// <param name="message">message</param>
        /// <param name="args">args</param>
        public void Warn(string message, params object[] args)
        {
            if (this.Log.IsWarnEnabled)
            {
                this.WriteToLog(LogLevel.Warn, null, message, args);
            }
        }

        /// <summary>
        /// Warn类日志输出。
        /// </summary>
        /// <param name="err">错误码</param>
        /// <param name="args">参数</param>
        public void Warn(SmcErr err, params object[] args)
        {
            if (this.Log.IsWarnEnabled)
            {
                this.WriteToLog(LogLevel.Warn, err, null, String.Empty, args);
            }
        }

        /// <summary>
        /// Warn类日志输出。
        /// </summary>
        /// <param name="err">错误码,即信息</param>
        /// <param name="message">message</param>
        /// <param name="args">args</param>
        public void Warn(SmcErr err, string message, params object[] args)
        {
            if (this.Log.IsWarnEnabled)
            {
                this.WriteToLog(LogLevel.Warn, err, null, message, args);
            }
        }

        /// <summary>
        /// 带异常信息的Warn类日志输出。
        /// </summary>
        /// <param name="e">e</param>
        /// <param name="message">message</param>
        /// <param name="args">args</param>
        public void Warn(Exception e, string message, params object[] args)
        {
            if (this.Log.IsWarnEnabled)
            {
                this.WriteToLog(LogLevel.Warn, e, message, args);
            }
        }

        /// <summary>
        /// 带异常信息的Warn类日志输出。
        /// </summary>
        /// <param name="err">错误码,即信息</param>
        /// <param name="e">e</param>
        /// <param name="message">message</param>
        /// <param name="args">args</param>
        public void Warn(SmcErr err, Exception e, string message, params object[] args)
        {
            if (this.Log.IsWarnEnabled)
            {
                this.WriteToLog(LogLevel.Warn, err, e, message, args);
            }
        }

        /// <summary>
        /// Error类日志输出。
        /// </summary>
        /// <param name="message">message</param>
        /// <param name="args">args</param>
        public void Error(string message, params object[] args)
        {
            if (this.Log.IsErrorEnabled)
            {
                this.WriteToLog(LogLevel.Error, null, message, args);
            }
        }

        /// <summary>
        /// Error类日志输出。
        /// </summary>
        /// <param name="err">错误码,即信息</param>
        /// <param name="message">message</param>
        /// <param name="args">args</param>
        public void Error(SmcErr err)
        {
            if (this.Log.IsErrorEnabled)
            {
                this.WriteToLog(LogLevel.Error, err, null, String.Empty, null);
            }
        }

        /// <summary>
        /// Error类日志输出。
        /// </summary>
        /// <param name="err">错误码,即信息</param>
        /// <param name="args">args</param>
        public void Error(SmcErr err, params object[] args)
        {
            if (this.Log.IsErrorEnabled)
            {
                this.WriteToLog(LogLevel.Error, err, null, String.Empty, args);
            }
        }

        /// <summary>
        /// Error类日志输出。
        /// </summary>
        /// <param name="err">错误码,即信息</param>
        /// <param name="message">message</param>
        /// <param name="args">args</param>
        public void Error(SmcErr err, string message, params object[] args)
        {
            if (this.Log.IsErrorEnabled)
            {
                this.WriteToLog(LogLevel.Error, err, null, message, args);
            }
        }

        /// <summary>
        /// 带异常信息的Error类日志输出。
        /// </summary>
        /// <param name="e">e</param>
        /// <param name="message">message</param>
        /// <param name="args">args</param>
        public void Error(Exception e, string message, params object[] args)
        {
            if (this.Log.IsErrorEnabled)
            {
                this.WriteToLog(LogLevel.Error, e, message, args);
            }
        }

        /// <summary>
        /// 带异常信息的Error类日志输出。
        /// </summary>
        /// <param name="err">错误码,即信息</param>
        /// <param name="e">e</param>
        /// <param name="message">message</param>
        /// <param name="args">args</param>
        public void Error(SmcErr err, Exception e, string message, params object[] args)
        {
            if (this.Log.IsErrorEnabled)
            {
                this.WriteToLog(LogLevel.Error, err, e, message, args);
            }
        }

        /// <summary>
        /// Fatal类（致命）日志输出。
        /// </summary>
        /// <param name="message">message</param>
        /// <param name="args">args</param>
        public void Fatal(string message, params object[] args)
        {
            if (this.Log.IsFatalEnabled)
            {
                this.WriteToLog(LogLevel.Fatal, null, message, args);
            }
        }

        /// <summary>
        /// Fatal类（致命）日志输出。
        /// </summary>
        /// <param name="err">错误码,即信息</param>
        /// <param name="args">args</param>
        public void Fatal(SmcErr err, params object[] args)
        {
            if (this.Log.IsFatalEnabled)
            {
                this.WriteToLog(LogLevel.Fatal, err, null, String.Empty, args);
            }
        }

        /// <summary>
        /// Fatal类（致命）日志输出。
        /// </summary>
        /// <param name="err">错误码,即信息</param>
        /// <param name="message">message</param>
        /// <param name="args">args</param>
        public void Fatal(SmcErr err, string message, params object[] args)
        {
            if (this.Log.IsFatalEnabled)
            {
                this.WriteToLog(LogLevel.Fatal, err, null, message, args);
            }
        }

        /// <summary>
        /// 带异常信息的Fatal类日志输出。
        /// </summary>
        /// <param name="e">e</param>
        /// <param name="message">message</param>
        /// <param name="args">args</param>
        public void Fatal(Exception e, string message, params object[] args)
        {
            if (this.Log.IsFatalEnabled)
            {
                this.WriteToLog(LogLevel.Fatal, e, message, args);
            }
        }

        /// <summary>
        /// 带异常信息的Fatal类日志输出。
        /// </summary>
        /// <param name="err">错误码,即信息</param>
        /// <param name="e">e</param>
        /// <param name="message">message</param>
        /// <param name="args">args</param>
        public void Fatal(SmcErr err, Exception e, string message, params object[] args)
        {
            if (this.Log.IsFatalEnabled)
            {
                this.WriteToLog(LogLevel.Fatal, err, e, message, args);
            }
        }

        /// <summary>
        /// 写入日志
        /// </summary>
        /// <param name="level">日志等级</param>
        /// <param name="e">异常</param>
        /// <param name="message">信息</param>
        /// <param name="args">args</param>
        private void WriteToLog(LogLevel level, Exception e, string message, params object[] args)
        {
            //暂时先这样处理，保证log2console能够看到异常信息
            StringBuilder log = new StringBuilder();
            if (null != e)
            {
                log.AppendFormat("{0},{1}", e.Message, e.StackTrace);
                log.AppendLine();
                if (null != e.InnerException)
                {
                    log.AppendFormat("InnerException {0},{1}", e.InnerException.Message, e.InnerException.StackTrace);
                    log.AppendLine();
                }
            }

            if (null != args && args.Length == 0)
            {
                log.Append(message);
            }
            else
            {
                log.AppendFormat(message, args);
            }

            LogEventInfo logEvent = new LogEventInfo(level, this.Log.Name, null, log.ToString(), null, null);
            foreach (KeyValuePair<string, object> de in this.Properties)
            {
                if (de.Key != null && de.Value != null)
                {
                    logEvent.Properties[de.Key] = de.Value;
                }
            }

            //清除ErrNo信息，屏蔽trace或老日志显示在系统操作日志中
            logEvent.Properties.Remove("errno");
            /*以下屏蔽语句直接决定R1方式的日志是否可用*/
            this.Log.Log(logEvent);
        }

        /// <summary>
        /// 写入日志
        /// </summary>
        /// <param name="level">日志等级</param>
        /// <param name="err">err</param>
        private void WriteToLog(LogLevel level, SmcErr err, Exception e, string message, params object[] args)
        {
            //暂时先这样处理，保证log2console能够看到异常信息
            StringBuilder log = new StringBuilder();
            if (null != e)
            {
                log.AppendFormat("{0},{1}", e.Message, e.StackTrace);
                log.AppendLine();
                if (null != e.InnerException)
                {
                    log.AppendFormat("InnerException {0},{1}", e.InnerException.Message, e.InnerException.StackTrace);
                    log.AppendLine();
                }
            }

            if (null == args || (null != args && args.Length == 0))
            {
                log.Append(message);
            }
            else
            {
                log.AppendFormat(message, args);
            }

            LogEventInfo logEvent = new LogEventInfo(level, this.Log.Name, null, log.ToString(), null, null);
            if (null != err)
            {
                Param = err.GetXmlParamter();
                Model = err.GetModelName();
                ErrNo = err.ErrNo;
            }

            foreach (KeyValuePair<string, object> de in this.Properties)
            {
                logEvent.Properties[de.Key] = de.Value;
            }

            this.Log.Log(logEvent);
            //ClearProperties();
        }

        /// <summary>
        /// 记录操作日志
        /// </summary>
        /// <param name="operateName">操作名称</param>
        /// <param name="err">错误码</param>
        public void RecordOperateLog(string operateName, SmcErr err)
        {
            if (err.IsSuccess())
            {
                this.Info("{0} Succeed", operateName);
            }
            else
            {
                this.Error("{0} Failed,ErrNo:{1}", operateName, err.ErrNo);
            }
        }
        #endregion
    }
}