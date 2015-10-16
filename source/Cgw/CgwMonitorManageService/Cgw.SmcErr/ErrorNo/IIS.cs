//-----------------------------------------------------------------------
//文件名：ErrorNo/IIS.cs
//版权：Copyright 2010-2012 Huawei Tech. Co. Ltd. All Rights Reserved. 
//描述：IIS服务的错误码
//-----------------------------------------------------------------------
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace CgwMonitorManage.SmcError
{
    /// <summary>
    /// SmcErr类：设备服务错误码，错误码从0x50900000开始
    /// </summary>
    public partial class SmcErr
    {
        #region 服务相关错误码(从0x50900000开始)
        #endregion

        #region 会话管理错误码(从0x50901000开始)
        /// <summary>
        /// 会话管理错误码0x50901000
        /// </summary>
        public const uint ERR_IIS_SESSION = 0x50901000;

        /// <summary>
        /// 用户注销
        /// </summary>
        public const uint ERR_IIS_SESSION_LOGOUT = ERR_IIS_SESSION + 0x01;

        /// <summary>
        /// 退出原因：服务异常
        /// </summary>
        public const uint ERR_IIS_SESSION_LOGOUT_RESELT_SERVICEEXCEPTION = ERR_IIS_SESSION + 0x02;

        /// <summary>
        /// 退出原因：管理员重置密码
        /// </summary>
        public const uint ERR_IIS_SESSION_LOGOUT_RESELT_RESETPASSWORD = ERR_IIS_SESSION + 0x03;

        /// <summary>
        /// 退出原因：用户被删除
        /// </summary>
        public const uint ERR_IIS_SESSION_LOGOUT_RESELT_DELETED = ERR_IIS_SESSION + 0x04;

        /// <summary>
        /// 退出原因：登录超时
        /// </summary>
        public const uint ERR_IIS_SESSION_LOGOUT_RESELT_LOGINTIMEOUT = ERR_IIS_SESSION + 0x05;

        /// <summary>
        /// 退出原因：其他用户登录
        /// </summary>
        public const uint ERR_IIS_SESSION_LOGOUT_RESELT_LOGINBYOTHER = ERR_IIS_SESSION + 0x06;

        /// <summary>
        /// 退出原因：权限变化
        /// </summary>
        public const uint ERR_IIS_SESSION_LOGOUT_RESELT_PERMISSIONCHANGED = ERR_IIS_SESSION + 0x07;

        /// <summary>
        /// 退出原因：手动注销
        /// </summary>
        public const uint ERR_IIS_SESSION_LOGOUT_RESELT_BYSEFT = ERR_IIS_SESSION + 0x08;

        /// <summary>
        /// 用户登录成功
        /// </summary>
        public const uint ERR_IIS_SESSION_LOGIN_SUCCESS = ERR_IIS_SESSION + 0x09;

        /// <summary>
        /// 用户登录失败
        /// </summary>
        public const uint ERR_IIS_SESSION_LOGIN_FAILED = ERR_IIS_SESSION + 0x0A;
        #endregion

        #region 设备管理错误码(从0x50902000开始)
        /// <summary>
        /// 设备管理错误码0x50902000
        /// </summary>
        public const uint ERR_IIS_DEVICE = 0x50902000;
        #endregion

        #region 设备配置管理错误码(从0x50903000开始)
        /// <summary>
        /// 设备配置管理错误码0x50903000
        /// </summary>
        public const uint ERR_IIS_DEVICE_CONFIGTEMPLATE = 0x50903000;
        #endregion

        #region 软件管理错误码(从0x50904000开始)
        /// <summary>
        /// 软件管理错误码0x50904000
        /// </summary>
        public const uint ERR_IIS_DEVICE_SOFTWARE = 0x50904000;
        #endregion

        #region 设备升级管理错误码(从0x50905000开始)
        /// <summary>
        /// 设备升级管理错误码0x50905000
        /// </summary>
        public const uint ERR_IIS_DEVICE_UPGRADE = 0x50905000;
        #endregion

        #region 告警管理错误码(从0x50906000开始)
        /// <summary>
        /// 告警管理错误码0x50906000
        /// </summary>
        public const uint ERR_IIS_ALARM = 0x50906000;
        #endregion

        #region 日志错误码(从0x50907000开始)
        /// <summary>
        /// 日志错误码0x50907000
        /// </summary>
        public const uint ERR_IIS_LOG = 0x50907000;

        /// <summary>
        /// 转换日志文档失败
        /// </summary>
        public const uint ERR_IIS_SVCFILE = ERR_IIS_LOG + 0x01;

        /// <summary>
        /// 下载日志文档失败
        /// </summary>
        public const uint ERR_IIS_DOWNFILE = ERR_IIS_LOG + 0x02;

        /// <summary>
        /// 导出日志没有数据
        /// </summary>
        public const uint ERR_IIS_EXPORT_NOTDATA = ERR_IIS_LOG + 0x03;

        /// <summary>
        /// 导出系统日志成功。
        /// </summary>
        public const uint ERR_SERVICES_LOADSYSLOG_SUCCESS = ERR_IIS_LOG + 0x04;

        /// <summary>
        /// 导出系统日志失败。
        /// </summary>
        public const uint ERR_SERVICES_LOADSYSLOG_FAILED = ERR_IIS_LOG + 0x05;

        /// <summary>
        /// 导出设备"{devicename}{0}"的操作日志成功。
        /// </summary>
        public const uint ERR_DEVICE_EXPORT_EX_LOG_SUCC = ERR_IIS_LOG + 0x06;

        /// <summary>
        /// 导出设备"{devicename}{0}"的操作日志失败，原因：{err}{0}
        /// </summary>
        public const uint ERR_DEVICE_EXPORT_EX_LOG_FAIL = ERR_IIS_LOG + 0x07;

        /// <summary>
        /// 导出设备"{devicename}{0}"的设备日志。
        /// </summary>
        public const uint ERR_DEVICE_EXPORT_FILE = ERR_IIS_LOG + 0x08;
        #endregion

        #region 系统配置管理
        /// <summary>
        /// 系统配置管理错误码0x50908000
        /// </summary>
        public const uint ERR_IIS_CONFIG= 0x50908000;

        /// <summary>
        /// 修改会议配置成功
        /// </summary>
        public const uint ERR_IIS_CONFIG_SAVE_CONF_SUCCESS = ERR_IIS_CONFIG + 0x01;

        /// <summary>
        ///修改会议配置失败
        /// </summary>
        public const uint ERR_IIS_CONFIG_SAVE_CONF_FAIL = ERR_IIS_CONFIG + 0x02;

        /// <summary>
        /// 设置速率范围成功
        /// </summary>
        public const uint ERR_IIS_CONFIG_SAVE_CONFRATEAREA_SUCCESS = ERR_IIS_CONFIG + 0x03;

        /// <summary>
        /// 设置速率范围失败
        /// </summary>
        public const uint ERR_IIS_CONFIG_SAVE_CONFRATEAREA_FAIL = ERR_IIS_CONFIG + 0x04;

        /// <summary>
        /// 设置子会议区间成功
        /// </summary>
        public const uint ERR_IIS_CONFIG_SAVE_SUBCONFAREA_SUCCESS = ERR_IIS_CONFIG + 0x05;

        /// <summary>
        /// 设置子会议区间失败
        /// </summary>
        public const uint ERR_IIS_CONFIG_SAVE_SUBCONFAREA_FAIL = ERR_IIS_CONFIG + 0x06;

        /// <summary>
        /// 修改NlogV2配置成功
        /// </summary>
        public const uint ERR_IIS_CONFIG_SAVE_NLOGV2_SUCCESS = ERR_IIS_CONFIG + 0x07;

        /// <summary>
        /// 修改NlogV2配置失败
        /// </summary>
        public const uint ERR_IIS_CONFIG_SAVE_NLOGV2_FAIL = ERR_IIS_CONFIG + 0x08;

        /// <summary>
        /// 修改设备配置成功
        /// </summary>
        public const uint ERR_IIS_CONFIG_SAVE_DEVICE_SUCCESS = ERR_IIS_CONFIG + 0x09;

        /// <summary>
        /// 修改设备配置失败
        /// </summary>
        public const uint ERR_IIS_CONFIG_SAVE_DEVICE_FAIL = ERR_IIS_CONFIG + 0x0A;

        /// <summary>
        /// 修改Email配置成功
        /// </summary>
        public const uint ERR_IIS_CONFIG_SAVE_EMAIL_SUCCESS = ERR_IIS_CONFIG + 0x0B;

        /// <summary>
        /// 修改Email配置失败
        /// </summary>
        public const uint ERR_IIS_CONFIG_SAVE_EMAIL_FAIL = ERR_IIS_CONFIG + 0x0C;

        /// <summary>
        /// 修改网络服务配置成功
        /// </summary>
        public const uint ERR_IIS_CONFIG_SAVE_NET_SUCCESS = ERR_IIS_CONFIG + 0x0D;

        /// <summary>
        /// 修改网络服务配置失败
        /// </summary>
        public const uint ERR_IIS_CONFIG_SAVE_NET_FAIL = ERR_IIS_CONFIG + 0x0E;

        /// <summary>
        /// 修改报表配置成功
        /// </summary>
        public const uint ERR_IIS_CONFIG_SAVE_REPORT_SUCCESS = ERR_IIS_CONFIG + 0x0F;

        /// <summary>
        /// 修改报表失败
        /// </summary>
        public const uint ERR_IIS_CONFIG_SAVE_REPORT_FAIL = ERR_IIS_CONFIG + 0x10;

        /// <summary>
        /// 添加计费码"{billingcode}{0}"成功
        /// </summary>
        public const uint ERR_IIS_CONFIG_ADD_BILLCODE_SUCCESS = ERR_IIS_CONFIG + 0x11;

        /// <summary>
        /// 添加计费码"{billingcode}{0}"失败。原因：{err}{0}
        /// </summary>
        public const uint ERR_IIS_CONFIG_ADD_BILLCODE_FAIL = ERR_IIS_CONFIG + 0x12;

        /// <summary>
        /// 修改计费码"{billingcode}{0}"成功
        /// </summary>
        public const uint ERR_IIS_CONFIG_EDIT_BILLCODE_SUCCESS = ERR_IIS_CONFIG + 0x13;

        /// <summary>
        /// 修改计费码"{billingcode}{0}"失败。原因：{err}{0}
        /// </summary>
        public const uint ERR_IIS_CONFIG_EDIT_BILLCODE_FAIL = ERR_IIS_CONFIG + 0x14;

        /// <summary>
        /// 删除计费码"{billingcode}{0}"成功
        /// </summary>
        public const uint ERR_IIS_CONFIG_DEL_BILLCODE_SUCCESS = ERR_IIS_CONFIG + 0x15;

        /// <summary>
        /// 删除计费码"{billingcode}{0}"失败。原因：{err}{0}
        /// </summary>
        public const uint ERR_IIS_CONFIG_DEL_BILLCODE_FAIL = ERR_IIS_CONFIG + 0x16;

        /// <summary>
        /// 添加服务区“{domain}{0}”成功
        /// </summary>
        public const uint ERR_IIS_CONFIG_ADD_DOMAIN_SUCCESS = ERR_IIS_CONFIG + 0x17;

        /// <summary>
        /// 添加服务区“{domain}{0}”失败。原因：{err}{0}
        /// </summary>
        public const uint ERR_IIS_CONFIG_ADD_DOMAIN_FAIL = ERR_IIS_CONFIG + 0x18;

        /// <summary>
        /// 修改服务区“{domain}{0}”成功
        /// </summary>
        public const uint ERR_IIS_CONFIG_EDIT_DOMAIN_SUCCESS = ERR_IIS_CONFIG + 0x19;

        /// <summary>
        /// 修改服务区“{domain}{0}”失败。原因：{err}{0}
        /// </summary>
        public const uint ERR_IIS_CONFIG_EDIT_DOMAIN_FAIL = ERR_IIS_CONFIG + 0x1A;

        /// <summary>
        /// 删除服务区“{domain}{0}”成功
        /// </summary>
        public const uint ERR_IIS_CONFIG_DEL_DOMAIN_SUCCESS = ERR_IIS_CONFIG + 0x1B;

        /// <summary>
        /// 删除服务区“{domain}{0}”失败。原因：{err}{0}
        /// </summary>
        public const uint ERR_IIS_CONFIG_DEL_DOMAIN_FAIL = ERR_IIS_CONFIG + 0x1C;

        /// <summary>
        /// 添加服务区字冠“{domaincode}{0}”成功
        /// </summary>
        public const uint ERR_IIS_CONFIG_ADD_DOMAIN_CODE_SUCCESS = ERR_IIS_CONFIG + 0x1D;

        /// <summary>
        /// 添加服务区字冠“{domaincode}{0}”失败。原因：{err}{0}
        /// </summary>
        public const uint ERR_IIS_CONFIG_ADD_DOMAIN_CODE_FAIL = ERR_IIS_CONFIG + 0x1E;

        /// <summary>
        /// 修改服务区字冠“{domaincode}{0}”成功
        /// </summary>
        public const uint ERR_IIS_CONFIG_EDIT_DOMAIN_CODE_SUCCESS = ERR_IIS_CONFIG + 0x1F;

        /// <summary>
        /// 修改服务区字冠“{domaincode}{0}”失败。原因：{err}{0}
        /// </summary>
        public const uint ERR_IIS_CONFIG_EDIT_DOMAIN_CODE_FAIL = ERR_IIS_CONFIG + 0x20;

        /// <summary>
        /// 删除服务区字冠“{domaincode}{0}”成功
        /// </summary>
        public const uint ERR_IIS_CONFIG_DEL_DOMAIN_CODE_SUCCESS = ERR_IIS_CONFIG + 0x21;

        /// <summary>
        /// 删除服务区字冠“{domaincode}{0}”失败。原因：{err}{0}
        /// </summary>
        public const uint ERR_IIS_CONFIG_DEL_DOMAIN_CODE_FAIL = ERR_IIS_CONFIG + 0x22;

        #endregion
    }
}
