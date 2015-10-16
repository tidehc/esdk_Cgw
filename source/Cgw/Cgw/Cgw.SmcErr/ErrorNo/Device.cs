//-----------------------------------------------------------------------
//文件名：ErrorNo/Device.cs
//版权：Copyright 2010-2012 Huawei Tech. Co. Ltd. All Rights Reserved. 
//描述：设备服务的错误码
//-----------------------------------------------------------------------
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Cgw.SmcError
{
    /// <summary>
    /// SmcErr类：设备服务错误码，错误码从0x50B00000开始
    /// </summary>
    public partial class SmcErr
    {
        #region 设备管理的错误码(从0x50B00000开始)
        #region 错误码
        /// <summary>
        /// 设备服务的错误码0x50B00000
        /// </summary>
        public const uint ERR_DEVICE_SERVICE = 0x50B00000;

        /// <summary>
        /// 是否恢复已删除设备
        /// </summary>
        public const uint ERR_DEVICE_CONFIRM_USE_OLD_DATA = ERR_DEVICE_SERVICE + 0x01;

        /// <summary>
        /// 操作超时，请关闭当前页面
        /// </summary>
        public const uint ERR_TEMP_DEVICE_NONEXISTENCE = ERR_DEVICE_SERVICE + 0x02;

        /// <summary>
        /// 设备不存在或已被删除
        /// </summary>
        public const uint ERR_DEVICE_NOT_EXIT = ERR_DEVICE_SERVICE + 0x03;

        /// <summary>
        /// 设备已经存在。
        /// </summary>
        public const uint ERR_DEVICE_EXIT = ERR_DEVICE_SERVICE + 0x04;

        /// <summary>
        /// 设备通信连接异常
        /// </summary>
        public const uint ERR_DEVICE_COM_CONNECTION = ERR_DEVICE_SERVICE + 0x05;

        /// <summary>
        /// SMC不支持与此设备通信
        /// </summary>
        public const uint ERR_DEVICE_UNKOWN_DEVICECOM = ERR_DEVICE_SERVICE + 0x06;

        /// <summary>
        /// 设备类型与指定类型不匹配
        /// </summary>
        public const uint ERR_DEVICE_CONVERTMANAGER_CLASS = ERR_DEVICE_SERVICE + 0x07;

        /// <summary>
        /// IP不能为空
        /// </summary>
        public const uint ERR_DEVICE_CONVERTMANAGER_IP_INVALID = ERR_DEVICE_SERVICE + 0x08;

        /// <summary>
        /// 消息发送超时
        /// </summary>
        public const uint ERR_DEVICE_REQUEST_TIMEOUT = ERR_DEVICE_SERVICE + 0x09;

        /// <summary>
        /// 修改失败,数据类型转换失败
        /// </summary>
        public const uint ERR_DEVICE_FIELD_VALUE_CONVERT_ERROR = ERR_DEVICE_SERVICE + 0x0A;

        /// <summary>
        /// 正在搜索设备...........
        /// </summary>
        public const uint ERR_DEVICE_SEARCH_SCHEDULE_STEP1 = ERR_DEVICE_SERVICE + 0x0B;

        /// <summary>
        /// 正在连接设备..........
        /// </summary>
        public const uint ERR_DEVICE_SEARCH_SCHEDULE_STEP2 = ERR_DEVICE_SERVICE + 0x0C;

        /// <summary>
        /// 正在获取配置信息.......
        /// </summary>
        public const uint ERR_DEVICE_SEARCH_SCHEDULE_STEP3 = ERR_DEVICE_SERVICE + 0x0D;

        /// <summary>
        /// 添加的设备超过License限制
        /// </summary>
        public const uint ERR_DEVICE_OVER_LICENSE = ERR_DEVICE_SERVICE + 0x0E;

        /// <summary>
        /// SMC系统内存不足，请联系系统管理员
        /// </summary>
        public const uint DEVICE_FAILED_ALLOCATE_MEMEORY = ERR_DEVICE_SERVICE + 0x0F;

        /// <summary>
        /// 设备型号不一致
        /// </summary>
        public const uint ERR_DEVICE_MODEL_CHANGED = ERR_DEVICE_SERVICE + 0x10;

        /// <summary>
        /// 该标识与设备实际标识{urisite}{0}不一致，建议修改标识保持一致
        /// </summary>
        public const uint ERR_DEVICE_URI_ISNOTMATCH = ERR_DEVICE_SERVICE + 0x11;

        /// <summary>
        /// H323ID与设备实际的H323ID不一致，请修改H323ID
        /// </summary>
        public const uint ERR_DEVICE_H323ID_ISNOTMATCH = ERR_DEVICE_SERVICE + 0x12;

        /// <summary>
        /// 非MCU设备，不支持查询日志文件列表
        /// </summary>
        public const uint ERR_DEVICE_NOT_SUPPORT_QUERY_FILELOG = ERR_DEVICE_SERVICE + 0x13;

        /// <summary>
        /// 不正确的日志路径
        /// </summary>
        public const uint ERR_DEVICE_WRONG_FILELOG_URL = ERR_DEVICE_SERVICE + 0x14;

        /// <summary>
        /// 搜索地址不正确，请输入IPv4、IPv6或DNS地址
        /// </summary>
        public const uint ERR_DEVICE_SEARCH_ADDRESS_INVALID = ERR_DEVICE_SERVICE + 0x15;

        /// <summary>
        /// 域名搜索失败
        /// </summary>
        public const uint ERR_DEVICE_DOMAIN_SEARCH_ERROR = ERR_DEVICE_SERVICE + 0x16;

        /// <summary>
        /// 文件{filename}{0}不存在
        /// </summary>
        public const uint ERR_LOAD_DEVICE_OID_FILE_FAILED = ERR_DEVICE_SERVICE + 0x17;

        /// <summary>
        /// 文件{filename}{0}解析错误
        /// </summary>
        public const uint ERR_PRASE_DEVICE_OID_FILE_FAILED = ERR_DEVICE_SERVICE + 0x18;

        /// <summary>
        /// 取消添加设备失败
        /// </summary>
        public const uint ERR_DEVICE_CANCEL_ADD_EXP = ERR_DEVICE_SERVICE + 0x19;

        /// <summary>
        /// 设备连接句柄错误
        /// </summary>
        public const uint ERR_DEVICE_CONNECTION_ERR = ERR_DEVICE_SERVICE + 0x1A;

        /// <summary>
        /// 设备服务异常，设备容器不存在
        /// </summary>
        public const uint ERR_DEVICE_CONTAINER_ERR = ERR_DEVICE_SERVICE + 0x1B;

        /// <summary>
        /// 设备已经删除或不存在
        /// </summary>
        public const uint ERR_DEVICE_IS_NOT_EXIST = ERR_DEVICE_SERVICE + 0x1C;

        /// <summary>
        /// MCU存在预约会议不能删除
        /// </summary>
        public const uint ERR_DEL_DEVICE_IS_EXIST_SCH = ERR_DEVICE_SERVICE + 0x1D;

        /// <summary>
        /// MCU存在预约会议不能修改
        /// </summary>
        public const uint ERR_EDITE_DEVICE_IS_EXIST_SCH = ERR_DEVICE_SERVICE + 0x1E;

        /// <summary>
        /// 操作批量设备配置恢复时，部分设备恢复失败
        /// </summary>
        public const uint ERR_DEVICE_PARTIAL_RESTORE_FAILED = ERR_DEVICE_SERVICE + 0x1F;

        /// <summary>
        /// MCU为接入MCU不能删除
        /// </summary>
        public const uint ERR_DEL_DEVICE_IS_IVRMCU = ERR_DEVICE_SERVICE + 0x20;

        /// <summary>
        /// 该地址只能为IPV4，IPV6或URI
        /// </summary>
        public const uint ERR_DEVICE_IPV4_IPV6_URI_FAILED = ERR_DEVICE_SERVICE + 0x21;

        /// <summary>
        /// MCU存在会议或者有预约会议，不允许恢复配置
        /// </summary>
        public const uint ERR_DEVICE_IS_EXIST_SCH_HOLD_CONF = ERR_DEVICE_SERVICE + 0x22;

        /// <summary>
        /// 预发放类型不匹配
        /// </summary>
        public const uint ERR_DEVICE_PRE_DEFINE_MODEL_NOT_MATCH = ERR_DEVICE_SERVICE + 0x23;

        /// <summary>
        /// 预发放异常
        /// </summary>
        public const uint ERR_DEVICE_PRE_DEFINE_EXCEPTION = ERR_DEVICE_SERVICE + 0x24;

        /// <summary>
        /// 设备已经存在,与{DeviceName}{0} IP地址冲突。
        /// </summary>
        public const uint ERR_DEVICE_IP_EXIT = ERR_DEVICE_SERVICE + 0x25;

        /// <summary>
        /// 设备已经存在,与{DeviceName}{0} Mac 地址冲突。
        /// </summary>
        public const uint ERR_DEVICE_MAC_EXIT = ERR_DEVICE_SERVICE + 0x26;

        /// <summary>
        /// 设备MAC地址改变，确认继续配置恢复
        /// </summary>
        public const uint ERR_DEVICE_MAC_CHANGED_CONTINUTE_RESTORE = ERR_DEVICE_SERVICE + 0x27;

        /// <summary>
        /// 与IP为{deviceIp}{0}的设备Mac地址冲突
        /// </summary>
        public const uint ERR_DEVICE_MAC_CONFLICT = ERR_DEVICE_SERVICE + 0x28;

        /// <summary>
        /// 存在未确认告警的设备，不能备份设备配置
        /// </summary>
        public const uint ERR_DEVICE_ALARM_CONFIG_CANNOT_BACKUP = ERR_DEVICE_SERVICE + 0x29;

        /// <summary>
        /// 连接设备失败
        /// </summary>
        public const uint ERR_DEVICE_CONNECT_FAILED = ERR_DEVICE_SERVICE + 0x2A;

        #endregion

        #region 日志码
        /// <summary>
        /// 修改设备"{devicename}{0}"的连接失败，原因:{exception}{0}
        /// </summary>
        public const uint ERR_DEVICE_EDIT_CONNECTION_EXCEPTION = ERR_DEVICE_SERVICE + 0x504;

        /// <summary>
        /// 修改设备"{devicename}{0}"的连接失败，原因:{err}{0}
        /// </summary>
        public const uint ERR_DEVICE_EDIT_CONNECTION_ERROR = ERR_DEVICE_SERVICE + 0x505;

        /// <summary>
        /// 修改设备"{devicename}{0}"的连接成功
        /// </summary>
        public const uint ERR_DEVICE_SAVE_CONNECTION = ERR_DEVICE_SERVICE + 0x506;

        /// <summary>
        /// 添加设备"{devicename}{0}"失败，原因:{err}{0}
        /// </summary>
        public const uint ERR_DEVICE_ADD_FAILURE_ERR = ERR_DEVICE_SERVICE + 0x509;

        /// <summary>
        /// 添加设备"{devicename}{0}"失败，原因:{exception}{0}
        /// </summary>
        public const uint ERR_DEVICE_ADD_FAILURE_EXP = ERR_DEVICE_SERVICE + 0x50A;

        /// <summary>
        /// 添加设备"{devicename}{0}"成功
        /// </summary>
        public const uint ERR_DEVICE_ADD_SUCCESS = ERR_DEVICE_SERVICE + 0x50B;

        /// <summary>
        /// 删除设备"{devicename}{0}"成功
        /// </summary>
        public const uint ERR_DEVICE_DEL_SUCCESS = ERR_DEVICE_SERVICE + 0x50E;

        /// <summary>
        /// 删除设备"{devicename}{0}"失败，原因:{err}{0}
        /// </summary>
        public const uint ERR_DEVICE_DEL_FAILURE_ERR = ERR_DEVICE_SERVICE + 0x510;

        /// <summary>
        /// 删除设备"{devicename}{0}"失败，原因:{exception}{0}
        /// </summary>
        public const uint ERR_DEVICE_DEL_FAILURE_EXP = ERR_DEVICE_SERVICE + 0x511;

        /// <summary>
        /// 导出设备"{devicename}{0}"的日志成功
        /// </summary>
        public const uint ERR_DEVICE_EXPORT_LOG = ERR_DEVICE_SERVICE + 0x512;

        /// <summary>
        /// 导出设备"{devicename}{0}"的日志失败，原因:{err}{0}
        /// </summary>
        public const uint ERR_DEVICE_EXPORT_LOG_ERR = ERR_DEVICE_SERVICE + 0x513;

        /// <summary>
        /// 导出设备"{devicename}{0}"的日志失败，原因:{exception}{0}
        /// </summary>
        public const uint ERR_DEVICE_EXPORT_LOG_EXP = ERR_DEVICE_SERVICE + 0x514;

        /// <summary>
        /// 从数据库加载数据成功
        /// </summary>
        public const uint ERR_DEVICE_LOAD = ERR_DEVICE_SERVICE + 0x515;

        /// <summary>
        /// 从数据库加载数据失败，原因:{err}{0}
        /// </summary>
        public const uint ERR_DEVICE_LOAD_ERR = ERR_DEVICE_SERVICE + 0x516;

        /// <summary>
        /// 从数据库加载数据失败，原因:{exception}{0}
        /// </summary>
        public const uint ERR_DEVICE_LOAD_EXP = ERR_DEVICE_SERVICE + 0x517;

        /// <summary>
        /// 备份设备"{devicename}{0}"的配置成功
        /// </summary>
        public const uint ERR_DEVICE_CONFIG_BACK = ERR_DEVICE_SERVICE + 0x51B;

        /// <summary>
        /// 备份设备"{devicename}{0}"的配置失败，原因:{err}{0}
        /// </summary>
        public const uint ERR_DEVICE_CONFIG_BACK_ERR = ERR_DEVICE_SERVICE + 0x51C;

        /// <summary>
        /// 备份设备"{devicename}{0}"的配置失败，原因:{exception}{0}
        /// </summary>
        public const uint ERR_DEVICE_CONFIG_BACK_EXP = ERR_DEVICE_SERVICE + 0x51D;

        /// <summary>
        /// 恢复设备"{devicename}{0}"的配置成功
        /// </summary>
        public const uint ERR_DEVICE_CONFIG_RESTORE = ERR_DEVICE_SERVICE + 0x51E;

        /// <summary>
        /// 恢复设备"{devicename}{0}"的配置失败，原因:{err}{0}
        /// </summary>
        public const uint ERR_DEVICE_CONFIG_RESTORE_ERR = ERR_DEVICE_SERVICE + 0x51F;

        /// <summary>
        /// 恢复设备"{devicename}{0}"的配置失败，原因:{exception}{0}
        /// </summary>
        public const uint ERR_DEVICE_CONFIG_RESTORE_EXP = ERR_DEVICE_SERVICE + 0x520;

        /// <summary>
        /// 查询设备"{devicename}{0}"的日志成功
        /// </summary>
        public const uint ERR_DEVICE_QUERY_DEVICE_LOG = ERR_DEVICE_SERVICE + 0x521;

        /// <summary>
        /// 查询设备"{devicename}{0}"的日志失败，原因:{err}{0}
        /// </summary>
        public const uint ERR_DEVICE_QUERY_DEVICE_LOG_ERR = ERR_DEVICE_SERVICE + 0x522;

        /// <summary>
        /// 查询设备"{devicename}{0}"的日志失败，原因:{exception}{0}
        /// </summary>
        public const uint ERR_DEVICE_QUERY_DEVICE_LOG_EXP = ERR_DEVICE_SERVICE + 0x523;

        /// <summary>
        /// 设备"{devicename}{0}"下线
        /// </summary>
        public const uint ERR_DEVICE_LOG_OFFLINE = ERR_DEVICE_SERVICE + 0x524;

        /// <summary>
        /// 设备"{devicename}{0}"上线
        /// </summary>
        public const uint ERR_DEVICE_LOG_ONLINE = ERR_DEVICE_SERVICE + 0x525;

        /// <summary>
        /// 修改设备"{devicename}{0}"失败，原因:{err}{0}
        /// </summary>
        public const uint ERR_DEVICE_EDIT_FAILURE_ERR = ERR_DEVICE_SERVICE + 0x526;

        /// <summary>
        /// 修改设备"{devicename}{0}"失败，原因:{exception}{0}
        /// </summary>
        public const uint ERR_DEVICE_EDIT_FAILURE_EXP = ERR_DEVICE_SERVICE + 0x527;

        /// <summary>
        /// 修改设备"{devicename}{0}"成功
        /// </summary>
        public const uint ERR_DEVICE_EDIT_SUCCESS = ERR_DEVICE_SERVICE + 0x528;
		
		/// <summary>
        /// MCU"{devicename}{0}"暂停预约会议成功
        /// </summary>
        public const uint ERR_MCU_FORBID_SCHEDUAL_CONF_SUCC = ERR_DEVICE_SERVICE + 0x52F;

        /// <summary>
        /// MCU"{devicename}{0}"暂停预约会议失败，原因：{exception}{0}
        /// </summary>
        public const uint ERR_MCU_FORBID_SCHEDUAL_CONF_FAIL = ERR_DEVICE_SERVICE + 0x530;

        /// <summary>
        /// MCU"{devicename}{0}"恢复预约会议成功
        /// </summary>
        public const uint ERR_MCU_RESTORE_SCHEDUAL_CONF_SUCC = ERR_DEVICE_SERVICE + 0x532;

        /// <summary>
        /// MCU"{devicename}{0}"恢复预约会议失败，原因：{exception}{0}
        /// </summary>
        public const uint ERR_MCU_RESTORE_SCHEDUAL_CONF_FAIL = ERR_DEVICE_SERVICE + 0x533;

        /// <summary>
        /// 会场"{devicename}{0}"呼叫"{caller}{0}"成功
        /// </summary>
        public const uint ERR_DEVICE_CALL_SUCC = ERR_DEVICE_SERVICE + 0x538;

        /// <summary>
        /// 会场"{devicename}{0}"呼叫"{caller}{0}"失败，原因: {exception}{0}
        /// </summary>
        public const uint ERR_DEVICE_CALL_FAIL = ERR_DEVICE_SERVICE + 0x539;

        /// <summary>
        /// 会场"{devicename}{0}"挂断成功
        /// </summary>
        public const uint ERR_DEVICE_HANGUP_SUCC = ERR_DEVICE_SERVICE + 0x53B;

        /// <summary>
        /// 会场"{devicename}{0}"挂断失败，原因{err}{0}
        /// </summary>
        public const uint ERR_DEVICE_HANGUP_FAIL = ERR_DEVICE_SERVICE + 0x53C;

        /// <summary>
        /// 添加GK节点"{devicename}{0}"成功
        /// </summary>
        public const uint ERR_DEVICE_ADD_GK_EP_SUCC = ERR_DEVICE_SERVICE + 0x53E;

        /// <summary>
        /// 添加GK节点"{devicename}{0}"失败，原因:{err}{0}
        /// </summary>
        public const uint ERR_DEVICE_ADD_GK_EP_FAIL = ERR_DEVICE_SERVICE + 0x53F;

        /// <summary>
        /// 修改GK节点"{devicename}{0}"成功
        /// </summary>
        public const uint ERR_DEVICE_EDIT_GK_EP_SUCC = ERR_DEVICE_SERVICE + 0x541;

        /// <summary>
        /// 修改GK节点"{devicename}{0}"失败，原因:{err}{0}
        /// </summary>
        public const uint ERR_DEVICE_EDIT_GK_EP_FAIL = ERR_DEVICE_SERVICE + 0x542;

        /// <summary>
        /// 删除GK节点成功
        /// </summary>
        public const uint ERR_DEVICE_DEL_GK_EP_SUCC = ERR_DEVICE_SERVICE + 0x544;

        /// <summary>
        /// 删除GK节点失败，原因:{err}{0}
        /// </summary>
        public const uint ERR_DEVICE_DEL_GK_EP_FAIL = ERR_DEVICE_SERVICE + 0x545;

        /// <summary>
        /// GK"{devicename}{0}"上线
        /// </summary>
        public const uint ERR_DEVICE_GK_UPLINE = ERR_DEVICE_SERVICE + 0x547;

        /// <summary>
        /// GK"{devicename}{0}"离线
        /// </summary>
        public const uint ERR_DEVICE_GK_DOWNLINE = ERR_DEVICE_SERVICE + 0x548;

        /// <summary>
        /// GK挂断成功
        /// </summary>
        public const uint ERR_DEVICE_GK_HANGUP_SUCC = ERR_DEVICE_SERVICE + 0x549;

        /// <summary>
        /// GK挂断失败，原因：{err}{0}
        /// </summary>
        public const uint ERR_DEVICE_GK_HANGUP_FAIL = ERR_DEVICE_SERVICE + 0x54A;

        /// <summary>
        /// GK"{devicename}{0}"注销节点成功
        /// </summary>
        public const uint ERR_DEVICE_GK_UNREGISTER_SUCC = ERR_DEVICE_SERVICE + 0x54C;

        /// <summary>
        /// GK"{devicename}{0}"注销节点失败，原因：{err}{0}
        /// </summary>
        public const uint ERR_DEVICE_GK_UNREGISTER_FAIL = ERR_DEVICE_SERVICE + 0x54D;

        /// <summary>
        /// 设备服务启动
        /// </summary>
        public const uint ERR_DEVICE_START_SUCCESS = ERR_DEVICE_SERVICE + 0x54F;

        /// <summary>
        /// 设备服务停止
        /// </summary>
        public const uint ERR_DEVICE_STOP_SUCCESS = ERR_DEVICE_SERVICE + 0x550;

        /// <summary>
        /// MCU"{devicename}{0}"主板"{mainip}{0}"与备板"{backip}{0}"倒换成功
        /// </summary>
        public const uint ERR_DEVICE_MCU_MAIN_BAKC_Board_SUCCESS = ERR_DEVICE_SERVICE + 0x551;
        #endregion
        #endregion

        #region 设备配置模版的错误码(从0x50B01000开始)
        #region 错误码
        /// <summary>
        /// 设备配置管理模块的错误码0x50B01000
        /// </summary>
        public const uint ERR_DEVICE_CONFIGTEMPLATE = 0x50B01000;

        /// <summary>
        /// 没有查询到特定的配置模版详细信息
        /// </summary>
        public const uint ERR_CONFIGTEMPLATE_COM_CONFIGTEMPLATEINFO_QUERY = ERR_DEVICE_CONFIGTEMPLATE + 0x01;

        /// <summary>
        /// 没有查询到配置模版列表
        /// </summary>
        public const uint ERR_CONFIGTEMPLATE_COM_CONFIGTEMPLATELIST_QUERY = ERR_DEVICE_CONFIGTEMPLATE + 0x02;

        /// <summary>
        /// 没有找到待删除的配置模版
        /// </summary>
        public const uint ERR_CONFIGTEMPLATE_COM_CONFIGTEMPLATE_DELETE_PARAM = ERR_DEVICE_CONFIGTEMPLATE + 0x03;

        /// <summary>
        /// 没有找到待编辑的配置模版
        /// </summary>
        public const uint ERR_CONFIGTEMPLATE_COM_CONFIGTEMPLATE_BEGINEDIT = ERR_DEVICE_CONFIGTEMPLATE + 0x04;

        /// <summary>
        ///  配置模版的参数配置项不能为空
        /// </summary>
        public const uint ERR_CONFIGTEMPLATE_COM_CONFIGTEMPLATE_CHECKALL = ERR_DEVICE_CONFIGTEMPLATE + 0x05;

        /// <summary>
        /// 没有找到待查询的配置模版
        /// </summary>
        public const uint ERR_CONFIGTEMPLATE_COM_CONFIGTEMPLATE_FIND = ERR_DEVICE_CONFIGTEMPLATE + 0x06;

        /// <summary>
        /// 提交配置模版的操作出现异常
        /// </summary>
        public const uint ERR_CONFIGTEMPLATE_COM_CONFIGTEMPLATE_COMMIT = ERR_DEVICE_CONFIGTEMPLATE + 0x07;

        /// <summary>
        /// 待取消的配置模版没有找到
        /// </summary>
        public const uint ERR_CONFIGTEMPLATE_COM_CONFIGTEMPLATE_CANCLE = ERR_DEVICE_CONFIGTEMPLATE + 0x08;

        /// <summary>
        /// 待删除的配置模版没有找到
        /// </summary>
        public const uint ERR_CONFIGTEMPLATE_COM_CONFIGTEMPLATE_DELETE = ERR_DEVICE_CONFIGTEMPLATE + 0x09;

        /// <summary>
        /// 配置项校验不通过
        /// </summary>
        public const uint ERR_CONFIGTEMPLATE_COM_CONFIGTEMPLATE_CHECK_PARAM = ERR_DEVICE_CONFIGTEMPLATE + 0x0A;

        /// <summary>
        /// 开始添加配置模版出错
        /// </summary>
        public const uint ERR_CONFIGTEMPLATE_COM_CONFIGTEMPLATE_BEGINADD = ERR_DEVICE_CONFIGTEMPLATE + 0x0B;

        /// <summary>
        /// 配置模版名称不能为空
        /// </summary>
        public const uint ERR_CONFIGTEMPLATE_COM_CONFIGTEMPLATE_NAME_EMPTY = ERR_DEVICE_CONFIGTEMPLATE + 0x0C;

        /// <summary>
        /// 配置模版名称长度不能超过{max}{0}
        /// </summary>
        public const uint ERR_CONFIGTEMPLATE_COM_CONFIGTEMPLATE_NAME_OVER_MAX = ERR_DEVICE_CONFIGTEMPLATE + 0x0D;

        /// <summary>
        /// 配置模版参数到通信库校验失败
        /// </summary>
        public const uint ERR_CONFIGTEMPLATE_COM_CONFIGTEMPLATE_FIELDKEY = ERR_DEVICE_CONFIGTEMPLATE + 0x0E;

        /// <summary>
        /// 配置模版的配置项不能为空
        /// </summary>
        public const uint ERR_CONFIGTEMPLATE_COM_CONFIGTEMPLATE_NO_ITEMS = ERR_DEVICE_CONFIGTEMPLATE + 0x0F;

        /// <summary>
        /// 配置模版的名称不能重复
        /// </summary>
        public const uint ERR_CONFIGTEMPLATE_NAME_REPEATE = ERR_DEVICE_CONFIGTEMPLATE + 0x10;

        /// <summary>
        /// 不能找到临时配置模版ID
        /// </summary>
        public const uint ERR_CONFIGTEMPLATE_TEMPORARY_NOT_FOUND = ERR_DEVICE_CONFIGTEMPLATE + 0x11;

        /// <summary>
        /// 配置模版数据已经不存在
        /// </summary>
        public const uint ERR_CONFIGTEMPLATE_NO_EXIST = ERR_DEVICE_CONFIGTEMPLATE + 0x12;

        /// <summary>
        /// 该参数不可用
        /// </summary>
        public const uint ERR_CONFIGTEMPLATE_PARAM_NO_ENABLE = ERR_DEVICE_CONFIGTEMPLATE + 0x13;

        /// <summary>
        /// GK地址不能为空
        /// </summary>
        public const uint ERR_CONFIGTEMPLATE_PARAM_H323ADDRESS_NO_EMPTY = ERR_DEVICE_CONFIGTEMPLATE + 0x14;

        /// <summary>
        /// SIP服务器地址不能为空
        /// </summary>
        public const uint ERR_CONFIGTEMPLATE_PARAM_SIPADDRESS_NO_EMPTY = ERR_DEVICE_CONFIGTEMPLATE + 0x15;

        /// <summary>
        /// SIP代理服务器地址不能为空
        /// </summary>
        public const uint ERR_CONFIGTEMPLATE_PARAM_SIPAGENTADDRESS_NO_EMPTY = ERR_DEVICE_CONFIGTEMPLATE + 0x16;

        /// <summary>
        /// MCU的DNS服务器地址1不能为空
        /// </summary>
        public const uint ERR_CONFIGTEMPLATE_PARAM_MCU_DNSADDRESS1_NO_EMPTY = ERR_DEVICE_CONFIGTEMPLATE + 0x17;

        /// <summary>
        /// MCU的DNS服务器地址2不能为空
        /// </summary>
        public const uint ERR_CONFIGTEMPLATE_PARAM_MCU_DNSADDRESS2_NO_EMPTY = ERR_DEVICE_CONFIGTEMPLATE + 0x18;

        /// <summary>
        /// 语音网关地址不能为空
        /// </summary>
        public const uint ERR_CONFIGTEMPLATE_PARAM_VGAADDRESS_NO_EMPTY = ERR_DEVICE_CONFIGTEMPLATE + 0x19;

        /// <summary>
        /// MCU的Trap主机地址1不能为空
        /// </summary>
        public const uint ERR_CONFIGTEMPLATE_TRAP_ADDRESS1_NO_EMPTY = ERR_DEVICE_CONFIGTEMPLATE + 0x1A;

        /// <summary>
        /// MCU的Trap主机地址2不能为空
        /// </summary>
        public const uint ERR_CONFIGTEMPLATE_TRAP_ADDRESS2_NO_EMPTY = ERR_DEVICE_CONFIGTEMPLATE + 0x1B;

        /// <summary>
        /// MCU的Trap主机地址3不能为空
        /// </summary>
        public const uint ERR_CONFIGTEMPLATE_TRAP_ADDRESS3_NO_EMPTY = ERR_DEVICE_CONFIGTEMPLATE + 0x1C;

        /// <summary>
        /// MCU的Trap主机地址4不能为空
        /// </summary>
        public const uint ERR_CONFIGTEMPLATE_TRAP_ADDRESS4_NO_EMPTY = ERR_DEVICE_CONFIGTEMPLATE + 0x1D;

        /// <summary>
        /// MCU的Trap主机地址5不能为空
        /// </summary>
        public const uint ERR_CONFIGTEMPLATE_TRAP_ADDRESS5_NO_EMPTY = ERR_DEVICE_CONFIGTEMPLATE + 0x1E;

        /// <summary>
        /// NTP服务器地址不能为空
        /// </summary>
        public const uint ERR_CONFIGTEMPLATE_NTP_ADDRESS5_NO_EMPTY = ERR_DEVICE_CONFIGTEMPLATE + 0x1F;

        /// <summary>
        /// MCU的Trunk地址1不能为空
        /// </summary>
        public const uint ERR_CONFIGTEMPLATE_TRUNK_ADDRESS1_NO_EMPTY = ERR_DEVICE_CONFIGTEMPLATE + 0x20;

        /// <summary>
        /// MCU的Trunk地址2不能为空
        /// </summary>
        public const uint ERR_CONFIGTEMPLATE_TRUNK_ADDRESS2_NO_EMPTY = ERR_DEVICE_CONFIGTEMPLATE + 0x21;

        /// <summary>
        /// MCU的Trunk地址3不能为空
        /// </summary>
        public const uint ERR_CONFIGTEMPLATE_TRUNK_ADDRESS3_NO_EMPTY = ERR_DEVICE_CONFIGTEMPLATE + 0x22;

        /// <summary>
        /// MCU的Trunk地址4不能为空
        /// </summary>
        public const uint ERR_CONFIGTEMPLATE_TRUNK_ADDRESS4_NO_EMPTY = ERR_DEVICE_CONFIGTEMPLATE + 0x23;

        /// <summary>
        /// MCU的Trunk地址5不能为空
        /// </summary>
        public const uint ERR_CONFIGTEMPLATE_TRUNK_ADDRESS5_NO_EMPTY = ERR_DEVICE_CONFIGTEMPLATE + 0x24;

        /// <summary>
        /// MCU的Trunk地址6不能为空
        /// </summary>
        public const uint ERR_CONFIGTEMPLATE_TRUNK_ADDRESS6_NO_EMPTY = ERR_DEVICE_CONFIGTEMPLATE + 0x25;
        #endregion

        #region 日志码
        /// <summary>
        /// 配置模版{templatename}{0}删除成功
        /// </summary>
        public const uint ERR_CONFIGTEMPLATE_DEL = ERR_DEVICE_CONFIGTEMPLATE + 0x500;

        /// <summary>
        /// 没有权限操作配置模版
        /// </summary>
        public const uint ERR_CONFIGTEMPLATE_CANNOT = ERR_DEVICE_CONFIGTEMPLATE + 0x501;

        /// <summary>
        /// 配置模版{templatename}{0}删除失败。失败原因：{err}{1}
        /// </summary>
        public const uint ERR_CONFIGTEMPLATE_DEL_FAIL = ERR_DEVICE_CONFIGTEMPLATE + 0x502;

        /// <summary>
        /// 取消配置模版成功
        /// </summary>
        public const uint ERR_CONFIGTEMPLATE_CANCEL_SUCC = ERR_DEVICE_CONFIGTEMPLATE + 0x503;

        /// <summary>
        /// 取消配置模版失败
        /// </summary>
        public const uint ERR_CONFIGTEMPLATE_CANCEL_FAIL = ERR_DEVICE_CONFIGTEMPLATE + 0x504;

        /// <summary>
        /// 查询配置模版成功
        /// </summary>
        public const uint ERR_CONFIGTEMPLATE_QUERY_SUCC = ERR_DEVICE_CONFIGTEMPLATE + 0x505;

        /// <summary>
        /// 查询配置模版失败
        /// </summary>
        public const uint ERR_CONFIGTEMPLATE_QUERY_FAIL = ERR_DEVICE_CONFIGTEMPLATE + 0x506;

        /// <summary>
        /// 配置模版{templatename}{0}添加成功
        /// </summary>
        public const uint ERR_CONFIGTEMPLATE_ADD_SUCC = ERR_DEVICE_CONFIGTEMPLATE + 0x507;

        /// <summary>
        /// 配置模版{templatename}{0}添加失败。失败原因：{err}{1}
        /// </summary>
        public const uint ERR_CONFIGTEMPLATE_ADD_FAIL = ERR_DEVICE_CONFIGTEMPLATE + 0x508;

        /// <summary>
        /// 配置模版{templatename}{0}编辑成功
        /// </summary>
        public const uint ERR_CONFIGTEMPLATE_EDIT_SUCC = ERR_DEVICE_CONFIGTEMPLATE + 0x509;

        /// <summary>
        /// 配置模版{templatename}{0}编辑失败。失败原因：{err}{1}
        /// </summary>
        public const uint ERR_CONFIGTEMPLATE_EDIT_FAIL = ERR_DEVICE_CONFIGTEMPLATE + 0x50a;

        /// <summary>
        /// 配置模版{templatename}{0}添加异常。异常原因：{err}{1}
        /// </summary>
        public const uint ERR_CONFIGTEMPLATE_ADD_EXP = ERR_DEVICE_CONFIGTEMPLATE + 0x50b;

        /// <summary>
        /// 配置模版{templatename}{0}编辑异常。异常原因：{err}{1}
        /// </summary>
        public const uint ERR_CONFIGTEMPLATE_EDIT_EXP = ERR_DEVICE_CONFIGTEMPLATE + 0x50c;

        /// <summary>
        /// 配置模版{templatename}{0}删除异常。异常原因：{err}{1}
        /// </summary>
        public const uint ERR_CONFIGTEMPLATE_DEL_EXP = ERR_DEVICE_CONFIGTEMPLATE + 0x50d;
        #endregion
        #endregion

        #region 告警管理的错误码(从0x50B02000开始)
        #region 错误码
        /// <summary>
        /// 告警管理模块的错误码
        /// </summary>
        public const uint ERR_DEVICE_ALARM = 0x50B02000;

        /// <summary>
        /// 加载告警定位信息失败
        /// </summary>
        public const uint ERR_ALARM_COM_LOAD_ALARMORIENTATIONMAP = ERR_DEVICE_ALARM + 0x01;

        /// <summary>
        /// 加载告警信息失败
        /// </summary>
        public const uint ERR_ALARM_COM_LOAD_ALARMS = ERR_DEVICE_ALARM + 0x02;

        /// <summary>
        /// 添加告警操作异常
        /// </summary>
        public const uint ERR_ALARM_COM_ADD = ERR_DEVICE_ALARM + 0x03;

        /// <summary>
        /// 删除告警操作异常
        /// </summary>
        public const uint ERR_ALARM_COM_DELETE = ERR_DEVICE_ALARM + 0x04;

        /// <summary>
        /// 确认告警操作异常
        /// </summary>
        public const uint ERR_ALARM_COM_CONFIRM = ERR_DEVICE_ALARM + 0x05;

        /// <summary>
        /// 通过告警GUID没有查询到对应的告警信息
        /// </summary>
        public const uint ERR_ALARM_COM_QUERY_BY_GUID = ERR_DEVICE_ALARM + 0x06;

        /// <summary>
        /// 没有查询到告警信息列表
        /// </summary>
        public const uint ERR_ALARM_COM_QUERY_LIST = ERR_DEVICE_ALARM + 0x07;

        /// <summary>
        /// 告警恢复操作异常
        /// </summary>
        public const uint ERR_ALARM_COM_RESTORE = ERR_DEVICE_ALARM + 0x08;

        /// <summary>
        /// 没有找到需要恢复的告警
        /// </summary>
        public const uint ERR_ALARM_COM_RESTORE_NOT_FOUND = ERR_DEVICE_ALARM + 0x09;

        /// <summary>
        /// 告警类别数量统计操作出现错误。
        /// </summary>
        public const uint ERR_ALARM_COM_ALARM_STAT_COUNT = ERR_DEVICE_ALARM + 0x0a;
        #endregion

        #region 告警日志码
        /// <summary>
        /// 没有权限操作告警
        /// </summary>
        public const uint ERR_ALARM_CANNOT = ERR_DEVICE_ALARM + 0x500;

        /// <summary>
        /// 清除设备"{devicename}{0}"的告警信息：{err}{0}
        /// </summary>
        public const uint ERR_ALARM_DEL_SUCC = ERR_DEVICE_ALARM + 0x501;

        /// <summary>
        /// 清除设备"{devicename}{0}"的告警信息：{err}{0}，失败,原因：{err}{1}
        /// </summary>
        public const uint ERR_ALARM_DEL_FAIL = ERR_DEVICE_ALARM + 0x502;

        /// <summary>
        /// 确认设备"{devicename}{0}"的告警信息：{err}{0}
        /// </summary>
        public const uint ERR_ALARM_CONFIRM_SUCC = ERR_DEVICE_ALARM + 0x503;

        /// <summary>
        /// 确认设备"{devicename}{0}"的告警信息：{err}{0}，失败,原因：{err}{1}
        /// </summary>
        public const uint ERR_ALARM_CONFIRM_FAIL_ERR = ERR_DEVICE_ALARM + 0x504;

        /// <summary>
        /// 告警定位成功
        /// </summary>
        public const uint ERR_ALARM_ORIEN_SUCC = ERR_DEVICE_ALARM + 0x505;

        /// <summary>
        /// 告警定位失败
        /// </summary>
        public const uint ERR_ALARM_ORIEN_FAIL = ERR_DEVICE_ALARM + 0x506;

        /// <summary>
        /// 设备{devicename}{0}恢复告警{err}{0}，成功
        /// </summary>
        public const uint ERR_ALARM_RESTORE_SUCC = ERR_DEVICE_ALARM + 0x507;

        /// <summary>
        /// 恢复告警失败
        /// </summary>
        public const uint ERR_ALARM_RESTORE_FAIL = ERR_DEVICE_ALARM + 0x508;

        /// <summary>
        /// 设备{devicename}{0}上报告警信息：{err}{0}，成功
        /// </summary>
        public const uint ERR_ALARM_ADD_SUCC = ERR_DEVICE_ALARM + 0x509;

        /// <summary>
        /// 设备{devicename}{0}上报告警信息：{err}{0}，失败。失败原因：{err}{1}
        /// </summary>
        public const uint ERR_ALARM_ADD_FAIL_ERR = ERR_DEVICE_ALARM + 0x50a;

        /// <summary>
        /// 设备{devicename}{0}上报告警信息：{err}{0}，失败。失败原因：{exception}{1}
        /// </summary>
        public const uint ERR_ALARM_ADD_FAIL_EXP = ERR_DEVICE_ALARM + 0x50b;

        /// <summary>
        /// 确认设备{devicename}{0}告警信息时，发生异常：{exception}{1}
        /// </summary>
        public const uint ERR_ALARM_CONFIRM_FAIL_EXP = ERR_DEVICE_ALARM + 0x50c;

        /// <summary>
        /// 会议{confname}{0}上报告警信息：{err}{0}，失败。失败原因：{exception}{1}
        /// </summary>
        public const uint ERR_ALARM_ADD_CONF_FAIL_EXP = ERR_DEVICE_ALARM + 0x50d;

        /// <summary>
        /// 会议{confname}{0}上报告警信息：{err}{0}，成功
        /// </summary>
        public const uint ERR_ALARM_ADD_CONF_SUCC = ERR_DEVICE_ALARM + 0x50e;

        /// <summary>
        /// 会议{confname}{0}上报告警信息：{err}{0}，失败。失败原因：{err}{1}
        /// </summary>
        public const uint ERR_ALARM_ADD_CONF_FAIL_ERR = ERR_DEVICE_ALARM + 0x50f;

        /// <summary>
        /// 确认会议{confname}{0}告警信息时，发生异常：{exception}{1}
        /// </summary>
        public const uint ERR_ALARM_CONFIRM_CONF_FAIL_EXP = ERR_DEVICE_ALARM + 0x510;

        /// <summary>
        /// 会议{confname}{0}告警信息：{err}{0}，已确认
        /// </summary>
        public const uint ERR_ALARM_CONFIRM_CONF_SUCC = ERR_DEVICE_ALARM + 0x511;

        /// <summary>
        /// 会议{confname}{0}告警信息：{err}{0}，确认失败,原因：{err}{1}
        /// </summary>
        public const uint ERR_ALARM_CONFIRM_CONF_FAIL_ERR = ERR_DEVICE_ALARM + 0x512;

        /// <summary>
        /// 清除会议{confname}{0}告警信息：{err}{0}，发生异常,原因：{err}{1}
        /// </summary>
        public const uint ERR_ALARM_DEL_CONF_FAIL_EXP = ERR_DEVICE_ALARM + 0x513;

        /// <summary>
        /// 会议{confname}{0}告警信息：{err}{0}，清除失败,原因：{err}{1}
        /// </summary>
        public const uint ERR_ALARM_DEL_CONF_FAIL = ERR_DEVICE_ALARM + 0x514;

        /// <summary>
        /// 会议{confname}{0}告警信息：{err}{0}，已清除
        /// </summary>
        public const uint ERR_ALARM_DEL_CONF_SUCC = ERR_DEVICE_ALARM + 0x515;
        #endregion
        #endregion

        #region 软件管理模块的错误码(从0x50B03000开始)
        #region 错误码
        /// <summary>
        /// 软件管理模块的错误码0x50B03000
        /// </summary>
        public const uint ERR_DEVICE_SOFTWARE = 0x50B03000;

        /// <summary>
        /// 加载软件数据异常。
        /// </summary>
        public const uint ERR_SOFTWARE_LOAD_SOFTWARE = ERR_DEVICE_SOFTWARE + 0x01;

        /// <summary>
        /// 加载升级数据异常。
        /// </summary>
        public const uint ERR_SOFTWARE_LOAD_UPGRADE = ERR_DEVICE_SOFTWARE + 0x02;

        /// <summary>
        /// 添加软件失败，缺少文件路径信息。
        /// </summary>
        public const uint ERR_SOFTWARE_PARAM_ADD_SOFTWARE_PATH = ERR_DEVICE_SOFTWARE + 0x03;
        /// <summary>
        /// 添加软件失败，缺少文件版本信息。
        /// </summary>
        public const uint ERR_SOFTWARE_PARAM_ADD_SOFTWARE_VERSION = ERR_DEVICE_SOFTWARE + 0x04;

        /// <summary>
        /// 删除软件失败，未指定删除软件标识。
        /// </summary>
        public const uint ERR_SOFTWARE_PARAM_DEL_SOFTWARE = ERR_DEVICE_SOFTWARE + 0x05;

        /// <summary>
        /// 编辑设备升级任务，未指定设备标识。
        /// </summary>
        public const uint ERR_SOFTWARE_PARAM_BEGIN_UPGRADE = ERR_DEVICE_SOFTWARE + 0x06;

        /// <summary>
        /// 编辑软件时检查验证错误，未知的验证选项。
        /// </summary>
        public const uint ERR_SOFTWARE_PARAM_SOFTWARE = ERR_DEVICE_SOFTWARE + 0x07;

        /// <summary>
        /// 编辑软件描述信息错误。
        /// </summary>
        public const uint ERR_SOFTWARE_PARAM_SOFTWARE_DESCRIPTION = ERR_DEVICE_SOFTWARE + 0x08;

        /// <summary>
        ///编辑软件版本号错误。
        /// </summary>
        public const uint ERR_SOFTWARE_PARAM_SOFTWARE_VERSION = ERR_DEVICE_SOFTWARE + 0x09;

        /// <summary>
        /// 上传软件文件错误。
        /// </summary>
        public const uint ERR_SOFTWARE_PARAM_SOFTWARE_FILE = ERR_DEVICE_SOFTWARE + 0x0a;

        /// <summary>
        /// 设备升级过程中，升级进度更新失败，未知的设备标识或升级状态。
        /// </summary>
        public const uint ERR_SOFTWARE_PARAM_UPGRADING = ERR_DEVICE_SOFTWARE + 0x0b;

        /// <summary>
        /// 取消等待升级时未指定取消的设备标识。
        /// </summary>
        public const uint ERR_SOFTWARE_PARAM_CANCEL_UPGRADE = ERR_DEVICE_SOFTWARE + 0x0c;

        /// <summary>
        /// 升级数据中缺少修改标识、软件标识或设备标识的相关信息。
        /// </summary>
        public const uint ERR_SOFTWARE_PARAM_LOAD_UPGRADE = ERR_DEVICE_SOFTWARE + 0x0d;

        /// <summary>
        /// 软件数据中缺少软件标识、设备类型、软件型号系列、软件版本等相关信息。
        /// </summary>
        public const uint ERR_SOFTWARE_PARAM_LOAD_SOFTWARE = ERR_DEVICE_SOFTWARE + 0x0e;

        /// <summary>
        /// 编辑升级信息时检查验证错误，不确定的检查验证选项。
        /// </summary>
        public const uint ERR_SOFTWARE_PARAM_UPGRADE = ERR_DEVICE_SOFTWARE + 0x0f;

        /// <summary>
        /// 编辑升级信息时检查验证错误，不确定的升级软件选项。
        /// </summary>
        public const uint ERR_SOFTWARE_PARAM_UPGRADE_COMBOX = ERR_DEVICE_SOFTWARE + 0x10;

        /// <summary>
        /// 内存处理异常，向内存中添加数据失败。
        /// </summary>
        public const uint ERR_SOFTWARE_ADD_MEMORY = ERR_DEVICE_SOFTWARE + 0x11;

        /// <summary>
        /// 内存处理异常，获取内存数据失败。
        /// </summary>
        public const uint ERR_SOFTWARE_GET_MEMORY = ERR_DEVICE_SOFTWARE + 0x12;

        /// <summary>
        /// 内存处理异常，更新内存数据失败。
        /// </summary>
        public const uint ERR_SOFTWARE_UPDATE_MEMORY = ERR_DEVICE_SOFTWARE + 0x13;

        /// <summary>
        /// 内存处理异常，删除内存数据失败。
        /// </summary>
        public const uint ERR_SOFTWARE_DEL_MEMORY = ERR_DEVICE_SOFTWARE + 0x14;

        /// <summary>
        /// 添加软件时检查存在软件版本冲突。
        /// </summary>
        public const uint ERR_SOFTWARE_COMMIT_CONFLICT = ERR_DEVICE_SOFTWARE + 0x15;

        /// <summary>
        /// 升级任务冲突。
        /// </summary>
        public const uint ERR_SOFTWARE_COMMIT_UPGRADE = ERR_DEVICE_SOFTWARE + 0x16;

        /// <summary>
        /// 设备即将升级，不能修改。
        /// </summary>
        public const uint ERR_SOFTWARE_UPDATE_UPGRADE = ERR_DEVICE_SOFTWARE + 0x17;

        /// <summary>
        /// 软件描述信息内容超长。
        /// </summary>
        public const uint ERR_SOFTWARE_CHECK_DESCRIPTION = ERR_DEVICE_SOFTWARE + 0x18;

        /// <summary>
        /// 软件版本号超长。
        /// </summary>
        public const uint ERR_SOFTWARE_CHECK_VERSION = ERR_DEVICE_SOFTWARE + 0x19;

        /// <summary>
        /// 升级软件选择范围错误。
        /// </summary>
        public const uint ERR_SOFTWARE_CHECK_SOFTWARE_RANGE = ERR_DEVICE_SOFTWARE + 0x1a;

        /// <summary>
        /// 升级任务开始时间不能大于结束时间。
        /// </summary>
        public const uint ERR_SOFTWARE_CHECK_BEGIN_TIME = ERR_DEVICE_SOFTWARE + 0x1b;

        /// <summary>
        /// 升级任务开始时间不能小于当前时间。
        /// </summary>
        public const uint ERR_SOFTWARE_CHECK_NOW_TIME = ERR_DEVICE_SOFTWARE + 0x1c;

        /// <summary>
        /// 升级任务结束时间不能小于开始时间。
        /// </summary>
        public const uint ERR_SOFTWARE_CHECK_END_TIME = ERR_DEVICE_SOFTWARE + 0x1d;

        /// <summary>
        /// 不支持上传的软件文件格式。
        /// </summary>
        public const uint ERR_SOFTWARE_CHECK_UNSUPPORTED_FORMAT = ERR_DEVICE_SOFTWARE + 0x1e;

        /// <summary>
        /// 上传的软件文件不存在。
        /// </summary>
        public const uint ERR_SOFTWARE_CHECK_TEMPFILE_LOST = ERR_DEVICE_SOFTWARE + 0x1f;

        /// <summary>
        /// 上传的软件压缩文件，解压缩后文件不存在。
        /// </summary>
        public const uint ERR_SOFTWARE_CHECK_DECOMPRESSFILE_LOST = ERR_DEVICE_SOFTWARE + 0x20;

        /// <summary>
        /// 上传的软件文件校验信息后拷贝失败。
        /// </summary>
        public const uint ERR_SOFTWARE_CHECK_MOVEFILE = ERR_DEVICE_SOFTWARE + 0x21;

        /// <summary>
        /// 上传的压缩文件解压缩路径不存在。
        /// </summary>
        public const uint ERR_SOFTWARE_CHECK_DECOMPRESS_PATH = ERR_DEVICE_SOFTWARE + 0x22;

        /// <summary>
        /// 上传的压缩文件解压时创建解压路径失败。
        /// </summary>
        public const uint ERR_SOFTWARE_CHECK_CREATE_PATH = ERR_DEVICE_SOFTWARE + 0x23;

        /// <summary>
        /// 上传的压缩文件解压失败。
        /// </summary>
        public const uint ERR_SOFTWARE_CHECK_DECOMPRESS = ERR_DEVICE_SOFTWARE + 0x24;

        /// <summary>
        /// 解压密码错误。
        /// </summary>
        public const uint ERR_SOFTWARE_CHECK_DECOMPRESS_PASSWORD = ERR_DEVICE_SOFTWARE + 0x25;

        /// <summary>
        /// 验证升级文件失败，缺少升级文件。
        /// </summary>
        public const uint ERR_SOFTWARE_CHECK_FILE = ERR_DEVICE_SOFTWARE + 0x26;
        /// <summary>
        /// 验证升级文件失败，缺少BIN格式升级文件。
        /// </summary>
        public const uint ERR_SOFTWARE_CHECK_BINFILE = ERR_DEVICE_SOFTWARE + 0x27;
        /// <summary>
        /// 验证升级文件失败，缺少DAT格式升级文件。
        /// </summary>
        public const uint ERR_SOFTWARE_CHECK_DATFILE = ERR_DEVICE_SOFTWARE + 0x28;
        /// <summary>
        /// 验证升级文件失败，缺少智真主机升级文件。
        /// </summary>
        public const uint ERR_SOFTWARE_CHECK_TPMAINFILE = ERR_DEVICE_SOFTWARE + 0x29;
        /// <summary>
        /// 验证升级文件CRC校验失败，文件可能已损坏。
        /// </summary>
        public const uint ERR_SOFTWARE_CHECK_FILE_CRC = ERR_DEVICE_SOFTWARE + 0x2a;
        /// <summary>
        /// 验证升级文件失败，缺少或错误的产品名称信息。
        /// </summary>
        public const uint ERR_SOFTWARE_CHECK_FILE_PNAME = ERR_DEVICE_SOFTWARE + 0x2b;
        /// <summary>
        /// 验证升级文件失败，文件解读异常。
        /// </summary>
        public const uint ERR_SOFTWARE_CHECK_FILE_EXP = ERR_DEVICE_SOFTWARE + 0x2c;

        /// <summary>
        /// 执行设备升级时读取升级文件失败。
        /// </summary>
        public const uint ERR_SOFTWARE_READFILE = ERR_DEVICE_SOFTWARE + 0x2d;

        /// <summary>
        /// 执行设备升级时，升级文件不存在。
        /// </summary>
        public const uint ERR_SOFTWARE_FILE_LOST = ERR_DEVICE_SOFTWARE + 0x2e;

        /// <summary>
        /// 执行设备升级时升级文件路径不存在。
        /// </summary>
        public const uint ERR_SOFTWARE_FILEURL_EMPTY = ERR_DEVICE_SOFTWARE + 0x2f;

        /// <summary>
        /// 删除软件时，文件不存在。
        /// </summary>
        public const uint ERR_SOFTWARE_DEL_FILE_LOST = ERR_DEVICE_SOFTWARE + 0x30;

        /// <summary>
        /// 删除软件时，软件被升级任务使用。
        /// </summary>
        public const uint ERR_SOFTWARE_DEL_OCCUPATION = ERR_DEVICE_SOFTWARE + 0x31;

        /// <summary>
        /// 取消升级任务错误，只允许操作取消等待升级状态的升级任务。
        /// </summary>
        public const uint ERR_SOFTWARE_CANCEL_STATE = ERR_DEVICE_SOFTWARE + 0x32;

        /// <summary>
        /// 升级文件校验失败。
        /// </summary>
        public const uint ERR_SOFTWARE_CHECK_CRC = ERR_DEVICE_SOFTWARE + 0x33;

        /// <summary>
        /// 设备正在升级中，不能执行删除操作，请等待升级成功或超时。
        /// </summary>
        public const uint ERR_SOFTWARE_DEVICE_CANNOTDEL_WHILE_UPGRADING = ERR_DEVICE_SOFTWARE + 0x34;

        /// <summary>
        /// 生成UPF文件失败。
        /// </summary>
        public const uint ERR_SOFTWARE_CREATE_UPF_FAIL = ERR_DEVICE_SOFTWARE + 0x35;

        /// <summary>
        /// 升级失败，未知错误。
        /// </summary>
        public const uint ERR_SOFTWARE_UPGRADE_FAIL_UNKNOWN_ERROR = ERR_DEVICE_SOFTWARE + 0x36;

        /// <summary>
        /// HTTP帐户登录MCU失败。
        /// </summary>
        public const uint ERR_SOFTWARE_UPGRADE_HTTP_STATUS_ERROR = ERR_DEVICE_SOFTWARE + 0x37;

        /// <summary>
        /// 打包升级文件失败
        /// </summary>
        public const uint ERR_SOFTWARE_PACKAGE_ERROR = ERR_DEVICE_SOFTWARE + 0x38;
        #endregion

        #region 日志码
        /// <summary>
        /// 完成加载软件数据
        /// </summary>
        public const uint ERR_SOFTWARE_INITIALIZED_SOFT = ERR_DEVICE_SOFTWARE + 0x501;

        /// <summary>
        /// 添加软件版本号"{version}{0}"成功
        /// </summary>
        public const uint ERR_SOFTWARE_ADD_SOFT = ERR_DEVICE_SOFTWARE + 0x506;

        /// <summary>
        /// 添加软件版本号失败，原因:{err}{0}
        /// </summary>
        public const uint ERR_SOFTWARE_ADD_SOFT_WRONG = ERR_DEVICE_SOFTWARE + 0x508;

        /// <summary>
        /// 删除软件版本号"{version}{0}"成功
        /// </summary>
        public const uint ERR_SOFTWARE_DEL_SOFT = ERR_DEVICE_SOFTWARE + 0x50a;

        /// <summary>
        /// 删除软件版本号"{version}{0}"失败，原因:{err}{0}
        /// </summary>
        public const uint ERR_SOFTWARE_DEL_SOFT_WRONG = ERR_DEVICE_SOFTWARE + 0x50c;

        /// <summary>
        /// 添加设备"{devicename}{0}"的升级任务成功
        /// </summary>
        public const uint ERR_SOFTWARE_ADD_UPGRADE = ERR_DEVICE_SOFTWARE + 0x512;

        /// <summary>
        /// 添加设备"{devicename}{0}"的升级任务失败，原因:{err}{0}
        /// </summary>
        public const uint ERR_SOFTWARE_ADD_UPGRADE_WRONG = ERR_DEVICE_SOFTWARE + 0x514;

        /// <summary>
        /// 取消设备"{devicename}{0}"的升级任务成功
        /// </summary>
        public const uint ERR_SOFTWARE_CANCELED_UPGRADE = ERR_DEVICE_SOFTWARE + 0x516;

        /// <summary>
        /// 取消设备"{devicename}{0}"的升级任务失败，原因:{err}{0}
        /// </summary>
        public const uint ERR_SOFTWARE_CANCEL_UPGRADE_WRONG = ERR_DEVICE_SOFTWARE + 0x518;

        /// <summary>
        /// 设备"{devicename}{0}"开始执行升级任务：开始时间"{begintime}{1}"，结束时间"{endtime}{2}"，目标软件版本"{version}{3}"
        /// </summary>
        public const uint ERR_SOFTWARE_BEGIN_UPGRADE = ERR_DEVICE_SOFTWARE + 0x51b;

        /// <summary>
        /// 升级设备"{devicename}{0}"成功
        /// </summary>
        public const uint ERR_SOFTWARE_UPGRADE_SUCCESS = ERR_DEVICE_SOFTWARE + 0x51c;

        /// <summary>
        /// 升级设备"{devicename}{0}"失败，原因:{err}{0}
        /// </summary>
        public const uint ERR_SOFTWARE_UPGRADE_FAIL = ERR_DEVICE_SOFTWARE + 0x51d;

        #endregion
        #endregion

        #region 通用通讯库的错误码(从0x50B04000开始)
        /// <summary>
        /// 通用通讯库的错误码
        /// </summary>
        public const uint ERR_DEVICE_COM = 0x50B04000;

        /// <summary>
        /// 通讯库未知错误
        /// </summary>
        public const uint ERR_DEVICE_COM_UNKNOW = ERR_DEVICE_COM + 0x01;

        /// <summary>
        /// HTTP响应超时
        /// </summary>
        public const uint ERR_HTTP_TIMEOUT = ERR_DEVICE_COM + 0x02;

        /// <summary>
        /// 返回的消息类型不匹配
        /// </summary>
        public const uint ERR_DEVICE_CONVERT_MSG = ERR_DEVICE_COM + 0x03;

        /// <summary>
        /// 通讯库命令配置文件不存在
        /// </summary>
        public const uint ERR_DEVICE_COMMAND_FIEL_NOT_EXIST = ERR_DEVICE_COM + 0x04;

        /// <summary>
        /// 通讯库命令配置文件解析出错
        /// </summary>
        public const uint ERR_DEVICE_COMMAND_FILE_PARSE_FAILED = ERR_DEVICE_COM + 0x05;

        /// <summary>
        /// 通讯命令生成失败
        /// </summary>
        public const uint ERR_DEVICE_COMMAND_GEN_FAILED = ERR_DEVICE_COM + 0x06;

        /// <summary>
        /// 数据转换失败(FieldId 转Enum失败)
        /// </summary>
        public const uint ERR_DEVICE_FIELDID_CONVERT_FAILED = ERR_DEVICE_COM + 0x07;

        /// <summary>
        /// 保存配置到设备失败
        /// </summary>
        public const uint ERR_DEVICE_SETCONFIGS_FAILED = ERR_DEVICE_COM + 0x08;

        /// <summary>
        /// 设备呼叫失败
        /// </summary>
        public const uint ERR_DEVICE_CALL_FAILED = ERR_DEVICE_COM + 0x09;

        /// <summary>
        /// 获取呼叫列表失败
        /// </summary>
        public const uint ERR_DEVICE_GET_CALLLIST_FAILED = ERR_DEVICE_COM + 0x0A;

        /// <summary>
        /// 挂断呼叫失败
        /// </summary>
        public const uint ERR_DEVICE_DISCONNECT_CALL_FAILED = ERR_DEVICE_COM + 0x0B;

        /// <summary>
        /// 握手失败
        /// </summary>
        public const uint ERR_DEVICE_SHAKEHAND_FAILED = ERR_DEVICE_COM + 0x0C;

        /// <summary>
        /// 备份设备配置失败
        /// </summary>
        public const uint ERR_DEVICE_BACKUP_FILE_FAILED = ERR_DEVICE_COM + 0x0D;

        /// <summary>
        /// MCU通信库不支持的协议
        /// </summary>
        public const uint ERR_DEVICE_UNSUPPORT_PROTOCOL = ERR_DEVICE_COM + 0x0E;

        /// <summary>
        /// 状态机不能处理的消息
        /// </summary>
        public const uint ERR_UNHANDLED_MSG = ERR_DEVICE_COM + 0x0F;

        /// <summary>
        /// 命令配置文件定义不完整
        /// </summary>
        public const uint ERR_COMMAND_CONFIG_FILE_NOT_ENOUGH = ERR_DEVICE_COM + 0x10;

        /// <summary>
        /// 设备用户名密码错误。
        /// </summary>
        public const uint ERR_UNAUTHORIZED = ERR_DEVICE_COM + 0x11;

        /// <summary>
        /// 通讯库不存在该配置项
        /// </summary>
        public const uint ERR_COM_CONFIG_NOT_FOUND = ERR_DEVICE_COM + 0x12;

        /// <summary>
        /// 设备消息返回超时
        /// </summary>
        public const uint ERR_COM_ASYNC_CALL_BACK_TIME_OUT = ERR_DEVICE_COM + 0x13;

        /// <summary>
        /// 设备达到最大连接数
        /// </summary>
        public const uint ERR_UNAUTH_LIMIT = ERR_DEVICE_COM + 0x14;

        /// <summary>
        /// 该输入项必须以点开始，不能包含全角字符
        /// </summary>
        public const uint ERR_UNAUTH_LIMIT_DIAN = ERR_DEVICE_COM + 0x15;

        /// <summary>
        /// 当前连接请求模式(http\https)未启用
        /// </summary>
        public const uint ERR_DEVICE_UNENABLE_CONNECT_MODE = ERR_DEVICE_COM + 0x16;

        /// <summary>
        /// HTTP响应异常
        /// </summary>
        public const uint ERR_HTTP_EXCEPTION = ERR_DEVICE_COM + 0x17;

        /// <summary>
        /// 设备正在呼叫，不允许修改配置
        /// </summary>
        public const uint ERR_DEVICE_MEETING = ERR_DEVICE_COM + 0x18;
        #endregion

        #region VCT通讯模块的错误码(从0x50B05000开始)
        /// <summary>
        /// VCT通讯模块的错误码
        /// </summary>
        public const uint ERR_DEVICE_COM_VCT = 0x50B05000;

        /// <summary>
        /// 会场不存在
        /// </summary>
        public const uint ERR_VCT_NOT_EXIST = ERR_DEVICE_COM_VCT + 0x01;

        /// <summary>
        /// 会场已存在
        /// </summary>
        public const uint ERR_VCT_EXIST = ERR_DEVICE_COM_VCT + 0x02;

        /// <summary>
        /// 会场不在线
        /// </summary>
        public const uint ERR_VCT_STATE_OUTLINE = ERR_DEVICE_COM_VCT + 0x03;

        /// <summary>
        ///  输入参数为空
        /// </summary>
        public const uint ERR_VCT_PARAM_NULL = ERR_DEVICE_COM_VCT + 0x04;

        /// <summary>
        ///  配置属性映射字典为空
        /// </summary>
        public const uint ERR_VCT_CONFIGDIC_NULL = ERR_DEVICE_COM_VCT + 0x05;

        /// <summary>
        /// 同步请求超时
        /// </summary>
        public const uint ERR_VCT_SYNCREQUEST_TIMEOUT = ERR_DEVICE_COM_VCT + 0x06;

        /// <summary>
        /// VCT设备返回异常
        /// </summary>
        public const uint ERR_VCT_DEVICE_EXCEPTION = ERR_DEVICE_COM_VCT + 0x07;

        /// <summary>
        /// VCT已经在升级中
        /// </summary>
        public const uint ERR_VCT_UPADTING = ERR_DEVICE_COM_VCT + 0x08;

        /// <summary>
        /// VCT不在升级中
        /// </summary>
        public const uint ERR_VCT_UN_UPADTING = ERR_DEVICE_COM_VCT + 0x09;

        /// <summary>
        /// VCT通信异常
        /// </summary>
        public const uint ERR_VCT_COM_EXCEPTION = ERR_DEVICE_COM_VCT + 0x0a;

        /// <summary>
        /// 设备升级密码错误
        /// </summary>
        public const uint ERR_VCT_UPG_PWD_WRONG = ERR_DEVICE_COM_VCT + 0x0b;

        #region 设备错误码0x50B05100 -- 0x50B05A00
        #endregion

        #region vct参数验证 0x50B05A00开始
        public const uint ERR_VCT_CONFIGCHECK = ERR_DEVICE_COM_VCT + 0x0A00;

        /// <summary>
        /// 参数{0}无法转换
        /// </summary>
        public const uint ERR_VCT_CONFIG_CONVERT_NOT = ERR_VCT_CONFIGCHECK + 0x00;

        /// <summary>
        /// 参数类型错误
        /// </summary>
        public const uint ERR_VCT_CONFIG_TYPE_ERROR = ERR_VCT_CONFIGCHECK + 0x01;

        /// <summary>
        ///  该配置项不能为空
        /// </summary>
        public const uint ERR_VCT_CONFIG_NULLOREMPTY = ERR_VCT_CONFIGCHECK + 0x02;

        #endregion
        #endregion

        #region 下一代终端通讯模块的错误码(从0x50B06000开始)
        /// <summary>
        /// 下一代终端通讯模块的错误码
        /// </summary>
        public const uint ERR_DEVICE_COM_TXT0 = 0x50B06000;
        #endregion

        #region 智真通讯模块的错误码(从0x50B07000开始)
        /// <summary>
        /// 智真通讯模块的错误码
        /// </summary>
        public const uint ERR_DEVICE_COM_TP = 0x50B07000;

        /// <summary>
        /// 通讯异常
        /// </summary>
        public const uint ERR_DEVICE_COM_TP_UNKNOW_EXCEPTION = ERR_DEVICE_COM_TP + 0x01;

        /// <summary>
        /// 登录失败
        /// </summary>
        public const uint ERR_DEVICE_COM_TP_LOGIN_FAILED = ERR_DEVICE_COM_TP + 0x02;

        /// <summary>
        /// 加载配置文件出错
        /// </summary>
        public const uint ERR_DEVICE_COM_TP_LOAD_COMMAND_FAILED = ERR_DEVICE_COM_TP + 0x03;

        /// <summary>
        /// 加载资源文件出错
        /// </summary>
        public const uint ERR_DEVICE_COM_TP_LOAD_RESOURCES_FAILED = ERR_DEVICE_COM_TP + 0x04;

        /// <summary>
        /// 响应消息失败
        /// </summary>
        public const uint ERR_DEVICE_COM_TP_RESPONSE_MSG_FAILED = ERR_DEVICE_COM_TP + 0x05;

        /// <summary>
        /// 参数转换出错
        /// </summary>
        public const uint ERR_DEVICE_COM_TP_PARAM_CONVERT_FAILED = ERR_DEVICE_COM_TP + 0x06;

        /// <summary>
        /// 参数检验出错
        /// </summary>
        public const uint ERR_DEVICE_COM_TP_PARAM_CHECK_FAILED = ERR_DEVICE_COM_TP + 0x07;

        /// <summary>
        /// 参数不能为空
        /// </summary>
        public const uint ERR_DEVICE_COM_TP_PARAM_NOT_NULL = ERR_DEVICE_COM_TP + 0x08;

        /// <summary>
        /// 参数含有非法字符
        /// </summary>
        public const uint ERR_DEVICE_COM_TP_PARAM_INVALID = ERR_DEVICE_COM_TP + 0x09;

        /// <summary>
        /// 参数长度范围[0,32]
        /// </summary>
        public const uint ERR_DEVICE_COM_TP_PARAM_LENGTH_MAX_32 = ERR_DEVICE_COM_TP + 0x0A;

        /// <summary>
        /// 参数长度范围[0,128]
        /// </summary>
        public const uint ERR_DEVICE_COM_TP_PARAM_LENGTH_MAX_128 = ERR_DEVICE_COM_TP + 0x0B;
        #endregion

        #region Tandberg C通讯模块的错误码(从0x50B08000开始)

        /// <summary>
        /// Tandberg C通讯模块的错误码
        /// </summary>
        public const uint ERR_DEVICE_COM_TANDBERGC = 0x50B08000;

        /// <summary>
        /// 输入字符串中含有非法字符
        /// </summary>
        public const uint ERR_TANDBERG_CHAR_INVALID = ERR_DEVICE_COM_TANDBERGC + 0x01;

        /// <summary>
        /// 未找到对应指令
        /// </summary>
        public const uint ERR_DEVICE_COMMAND_XPATH_NODE_NOT_EXIST = ERR_DEVICE_COM_TANDBERGC + 0x02;

        /// <summary>
        /// (指令描述不完整)找不到属性
        /// </summary>
        public const uint ERR_DEVICE_COMMAND_XPATH_ATTRIBUTE_NOT_EXIST = ERR_DEVICE_COM_TANDBERGC + 0x03;

        /// <summary>
        /// 指令信息不完整(找不到xpath值)
        /// </summary>
        public const uint ERR_DEVICE_COMMAND_XPATH_VALUE_EMPTY = ERR_DEVICE_COM_TANDBERGC + 0x04;

        /// <summary>
        /// 指令格式不正确
        /// </summary>
        public const uint ERR_DEVICE_XPATH_NODE_FORMAT_INVALID = ERR_DEVICE_COM_TANDBERGC + 0x05;

        /// <summary>
        /// 配置文件中未定义订阅告警
        /// </summary>
        public const uint ERR_DEVICE_NO_SUBSCRIBE_ITEM = ERR_DEVICE_COM_TANDBERGC + 0x06;

        /// <summary>
        /// 配置文件中未定义feedback url
        /// </summary>
        public const uint ERR_DEVICE_NO_FEEDBACK_URL = ERR_DEVICE_COM_TANDBERGC + 0x07;

        /// <summary>
        /// 配置文件中未定义SLOT
        /// </summary>
        public const uint ERR_DEVICE_NO_SLOT = ERR_DEVICE_COM_TANDBERGC + 0x08;

        /// <summary>
        /// 告警解析出错
        /// </summary>
        public const uint ERR_DEVICE_DESERIALIZE_TRAP_FAILED = ERR_DEVICE_COM_TANDBERGC + 0x09;

        /// <summary>
        /// NTP Server Address Length must be from 0 to 64
        /// </summary>
        public const uint ERR_DEVICE_TANDBERG_NTP_ADDRESS_OVER_LENGTH = ERR_DEVICE_COM_TANDBERGC + 0x0A;

        /// <summary>
        /// NTP Server Address Length must be from 0 to 64
        /// </summary>
        public const uint ERR_DEVICE_TANDBERG_VOLUME_OVER_MAX = ERR_DEVICE_COM_TANDBERGC + 0x0B;

        ///// <summary>
        ///// Length must be from 0 to 64
        ///// </summary>
        //public const uint ERR_DEVICE_TANDBERG_SNMP_HOST_ADDRESS_OVER_LENGTH = ERR_DEVICE_COM_TANDBERGC + 0x0C;

        ///// <summary>
        ///// Length must be from 0 to 50
        ///// </summary>
        //public const uint ERR_DEVICE_TANDBERG_SNMP_CONTECT_OVER_LENGTH = ERR_DEVICE_COM_TANDBERGC + 0x0D;

        ///// <summary>
        ///// Length must be from 0 to 50
        ///// </summary>
        //public const uint ERR_DEVICE_TANDBERG_SNMP_LOCATION_OVER_LENGTH = ERR_DEVICE_COM_TANDBERGC + 0x0E;

        ///// <summary>
        ///// Length must be from 0 to 50
        ///// </summary>
        //public const uint ERR_DEVICE_TANDBERG_SIP_PASSWORD_OVER_LENGTH = ERR_DEVICE_COM_TANDBERGC + 0x0F;

        /// <summary>
        /// Length must be from 0 to 50
        /// </summary>
        public const uint ERR_DEVICE_TANDBERG_SIP_USER_NAME_OVER_LENGTH = ERR_DEVICE_COM_TANDBERGC + 0x10;

        /// <summary>
        /// Length must be from 0 to 255
        /// </summary>
        public const uint ERR_DEVICE_TANDBERG_SIP_URI_OVER_LENGTH = ERR_DEVICE_COM_TANDBERGC + 0x11;

        /// <summary>
        /// Length must be from 0 to 255
        /// </summary>
        public const uint ERR_DEVICE_TANDBERG_SIP_AGENT_ADDRESS_OVER_LENGTH = ERR_DEVICE_COM_TANDBERGC + 0x12;

        /// <summary>
        /// Length must be from 0 to 50
        /// </summary>
        public const uint ERR_DEVICE_TANDBERG_SC_PASSWORD_OVER_LENGTH = ERR_DEVICE_COM_TANDBERGC + 0x13;

        /// <summary>
        /// Length must be from 0 to 49
        /// </summary>
        public const uint ERR_DEVICE_TANDBERG_SC_H323_ID_OVER_LENGTH = ERR_DEVICE_COM_TANDBERGC + 0x14;

        /// <summary>
        /// Length must be from 0 to 30
        /// </summary>
        public const uint ERR_DEVICE_TANDBERG_SC_H323_ALIAS_OVER_LENGTH = ERR_DEVICE_COM_TANDBERGC + 0x15;

        /// <summary>
        /// Length must be from 0 to 255
        /// </summary>
        public const uint ERR_DEVICE_TANDBERG_SC_ADDRESS_OVER_LENGTH = ERR_DEVICE_COM_TANDBERGC + 0x16;

        /// <summary>
        /// Length must be from 0 to 50
        /// </summary>
        public const uint ERR_DEVICE_TANDBERG_DEVICE_NAME_OVER_LENGTH = ERR_DEVICE_COM_TANDBERGC + 0x17;

        /// <summary>
        /// Can't find the enum type
        /// </summary>
        public const uint ERR_DEVICE_TANDBERG_TYPE_NOT_FOUND = ERR_DEVICE_COM_TANDBERGC + 0x18;

        /// <summary>
        /// doesn't define this enum value
        /// </summary>
        public const uint ERR_DEVICE_TANDBERG_ENUM_VALUE_NOT_DEFINE = ERR_DEVICE_COM_TANDBERGC + 0x19;

        /// <summary>
        /// 未定义xpath节点
        /// </summary>
        public const uint ERR_NOT_DEFINED_XPATH_COMMAND = ERR_DEVICE_COM_TANDBERGC + 0x1A;

        /// <summary>
        /// 重复定义命令
        /// </summary>
        public const uint ERR_DUPLICATE_COMMAND = ERR_NOT_DEFINED_XPATH_COMMAND + 0x1B;
        #endregion

        #region Polycom HDX通讯模块的错误码(从0x50B09000开始)
        /// <summary>
        /// Polycom HDX通讯模块的错误码
        /// </summary>
        public const uint ERR_DEVICE_COM_POLYCOMHDX = 0x50B09000;

        /// <summary>
        /// 通讯发生异常
        /// </summary>
        public const uint ERR_DEVICE_COM_POLYCOMHDX_UNKNOW_EXCEPTION = ERR_DEVICE_COM_POLYCOMHDX + 0x01;

        /// <summary>
        /// 设备连接失败
        /// </summary>
        public const uint ERR_DEVICE_COM_POLYCOMHDX_CONNECT_FAILED = ERR_DEVICE_COM_POLYCOMHDX + 0x02;

        /// <summary>
        /// 设备连接认证失败
        /// </summary>
        public const uint ERR_DEVICE_COM_POLYCOMHDX_CONNECT_AUTH_FAILED = ERR_DEVICE_COM_POLYCOMHDX + 0x03;

        /// <summary>
        /// 设备连接已断开
        /// </summary>
        public const uint ERR_DEVICE_COM_POLYCOMHDX_CONNECT_CLOSED = ERR_DEVICE_COM_POLYCOMHDX + 0x04;

        /// <summary>
        /// 设备不支持当前呼叫速率
        /// </summary>
        public const uint ERR_DEVICE_COM_POLYCOMHDX_UNSUPPORT_SPEED = ERR_DEVICE_COM_POLYCOMHDX + 0x05;

        /// <summary>
        /// 备份文件已损坏
        /// </summary>
        public const uint ERR_DEVICE_COM_POLYCOMHDX_BACKUP_DESTROY = ERR_DEVICE_COM_POLYCOMHDX + 0x06;

        /// <summary>
        /// 获取指令失败
        /// </summary>
        public const uint ERR_DEVICE_COM_POLYCOMHDX_GET_COMMAND_FAILED = ERR_DEVICE_COM_POLYCOMHDX + 0x07;

        /// <summary>
        /// 发送指令返回消息失败
        /// </summary>
        public const uint ERR_DEVICE_COM_POLYCOMHDX_RESPONSE_MSG_FAILED = ERR_DEVICE_COM_POLYCOMHDX + 0x08;

        /// <summary>
        /// 参数转换失败
        /// </summary>
        public const uint ERR_DEVICE_COM_POLYCOMHDX_PARAM_CONVERT_FAILED = ERR_DEVICE_COM_POLYCOMHDX + 0x09;

        /// <summary>
        /// 参数不能为空
        /// </summary>
        public const uint ERR_DEVICE_COM_POLYCOMHDX_PARAM_NOT_NULL = ERR_DEVICE_COM_POLYCOMHDX + 0x0A;

        /// <summary>
        /// 参数含有非法字符
        /// </summary>
        public const uint ERR_DEVICE_COM_POLYCOMHDX_PARAM_INVALID = ERR_DEVICE_COM_POLYCOMHDX + 0x0B;

        /// <summary>
        /// 参数长度范围[0,30]
        /// </summary>
        public const uint ERR_DEVICE_COM_POLYCOMHDX_PARAM_LENGTH_MAX_30 = ERR_DEVICE_COM_POLYCOMHDX + 0x0C;

        /// <summary>
        /// 参数长度范围[0,50]
        /// </summary>
        public const uint ERR_DEVICE_COM_POLYCOMHDX_PARAM_LENGTH_MAX_50 = ERR_DEVICE_COM_POLYCOMHDX + 0x0D;

        /// <summary>
        /// 参数数值范围[0,10]
        /// </summary>
        public const uint ERR_DEVICE_COM_POLYCOMHDX_PARAM_NUM_MAX_10 = ERR_DEVICE_COM_POLYCOMHDX + 0x0E;
        #endregion

        #region 8系列MCU通讯模块的错误码(从0x50B0A000开始)
        /// <summary>
        /// 8系列MCU通讯模块的错误码
        /// </summary>
        public const uint ERR_DEVICE_COM_MCU8X = 0x50B0A000;

        /// <summary>
        /// 8系列MCU Snmp通信输入为空
        /// </summary>
        public const uint DEVICE_COM_MCU8X_SNMP_INPUT_NULL = ERR_DEVICE_COM_MCU8X + 0x01;

        /// <summary>
        /// Snmp写团体名为空
        /// </summary>
        public const uint DEVICE_COM_MCU8X_SNMP_WRITECOMMUNITY_NULL = ERR_DEVICE_COM_MCU8X + 0x02;

        /// <summary>
        /// 开会或者预约会议中，不允许修改配置
        /// </summary>
        public const uint DEVICE_COM_MCU_ERR_MEETING = ERR_DEVICE_COM_MCU8X + 0x03;

        /// <summary>
        /// 未导入根证书不允许配置sip协议类型
        /// </summary>
        public const uint DEVICE_COM_MCU_ERR_ROOT_CONFIG = ERR_DEVICE_COM_MCU8X + 0x04;

        /// <summary>
        /// 请开启DNS服务
        /// </summary>
        public const uint DEVICE_COM_MCU_ERR_DNS_DISABLE = ERR_DEVICE_COM_MCU8X + 0x05;

        /// <summary>
        /// 备份配置
        /// </summary>
        public const uint DEVICE_STORE_CONFIGATION_ERROR = ERR_DEVICE_COM_MCU8X + 0x06;

        /// <summary>
        /// snmp读写团体名不能相同
        /// </summary>
        public const uint DEVICE_COM_MCU_READWRITECOMMUNITY_NOT_EQUAL = ERR_DEVICE_COM_MCU8X + 0x07;

        /// <summary>
        /// snmp写团体名不能为"public"
        /// </summary>
        public const uint DEVICE_COM_MCU_WRITECOMMUNITY_NOT_EQUAL_PUBLIC = ERR_DEVICE_COM_MCU8X + 0x08;

        #region 待删除
        /// <summary>
        /// UDP 端口范围无效
        /// </summary>
        public const uint DEVICE_COM_MCU_UDP_PORT_RANGDE_INVALID = ERR_DEVICE_COM_MCU8X + 0x09;

        /// <summary>
        /// IDO 已经使用端口
        /// </summary>
        public const uint DEVICE_COM_MCU_UDP_PORT_IDO_PORT = ERR_DEVICE_COM_MCU8X + 0x0A;

        /// <summary>
        /// ip地址扫描已经使用该端口
        /// </summary>
        public const uint DEVICE_COM_MCU_UDP_PORT_IP_SCAN = ERR_DEVICE_COM_MCU8X + 0x0B;

        /// <summary>
        /// 日志服务器已使用该端口
        /// </summary>
        public const uint DEVICE_COM_MCU_UDP_PORT_LOG_SERVER = ERR_DEVICE_COM_MCU8X + 0x0C;

        /// <summary>
        /// RTP已使用该端口
        /// </summary>
        public const uint DEVICE_COM_MCU_UDP_PORT_RTP_PORT = ERR_DEVICE_COM_MCU8X + 0x0D;

        /// <summary>
        /// rtp端口必须大于10010
        /// </summary>
        public const uint DEVICE_COM_MCU_UDP_PORT_RTP_BASE_INVALID = ERR_DEVICE_COM_MCU8X + 0x0E;

        /// <summary>
        /// rtp端口范围无效
        /// </summary>
        public const uint DEVICE_COM_MCU_UDP_PORT_RTP_RANGDE_INVALID = ERR_DEVICE_COM_MCU8X + 0x0F;

        /// <summary>
        /// h225 Ras端口已使用该端口
        /// </summary>
        public const uint DEVICE_COM_MCU_UDP_H225_PORT_USED = ERR_DEVICE_COM_MCU8X + 0x10;

        /// <summary>
        /// 9001端口已经被系统预留
        /// </summary>
        public const uint DEVICE_COM_MCU_UDP_9001_PORT_USED = ERR_DEVICE_COM_MCU8X + 0x11;

        /// <summary>
        /// h245端口小于10000
        /// </summary>
        public const uint DEVICE_COM_MCU_TCP_H245_BASE_INVALID = ERR_DEVICE_COM_MCU8X + 0x12;

        /// <summary>
        /// h245已使用该端口
        /// </summary>
        public const uint DEVICE_COM_MCU_TCP_H245_USED = ERR_DEVICE_COM_MCU8X + 0x13;

        /// <summary>
        /// FTP 已使用该端口
        /// </summary>
        public const uint DEVICE_COM_MCU_TCP_FTP_USED = ERR_DEVICE_COM_MCU8X + 0x14;

        /// <summary>
        /// Telnet已使用该端口
        /// </summary>
        public const uint DEVICE_COM_MCU_TCP_TELNET_USED = ERR_DEVICE_COM_MCU8X + 0x15;

        /// <summary>
        /// RM已使用该端口
        /// </summary>
        public const uint DEVICE_COM_MCU_TCP_RM_USED = ERR_DEVICE_COM_MCU8X + 0x16;

        /// <summary>
        /// h225监听端口已使用该端口
        /// </summary>
        public const uint DEVICE_COM_MCU_TCP_H225_USED = ERR_DEVICE_COM_MCU8X + 0x17;

        /// <summary>
        /// 主叫呼集已使用该端口700
        /// </summary>
        public const uint DEVICE_COM_MCU_TCP_SITE_CALL_USED = ERR_DEVICE_COM_MCU8X + 0x18;

        /// <summary>
        /// ip地址不合法
        /// </summary>
        public const uint DEVICE_COM_MCU_CMP_ERR_IP_ADDR = ERR_DEVICE_COM_MCU8X + 0x19;

        /// <summary>
        /// 端口不是偶数，请输入偶数端口
        /// </summary>
        public const uint DEVICE_COM_MCU_UDP_PORT_NOT_EVEN = ERR_DEVICE_COM_MCU8X + 0x1A;

        /// <summary>
        /// http已使用该端口
        /// </summary>
        public const uint DEVICE_COM_MCU_CONNECT_PORT_HTTP_USED = ERR_DEVICE_COM_MCU8X + 0x1B;

        /// <summary>
        /// https已使用该端口
        /// </summary>
        public const uint DEVICE_COM_MCU_CONNECT_PORT_HTTPS_USED = ERR_DEVICE_COM_MCU8X + 0x1C;

        /// <summary>
        /// 端口无效
        /// </summary>
        public const uint DEVICE_COM_MCU_PORT_RANGDE_INVALID = ERR_DEVICE_COM_MCU8X + 0x1D;

        /// <summary>
        /// sip已使用该端口
        /// </summary>
        public const uint DEVICE_COM_MCU_TCP_SIP_USED = ERR_DEVICE_COM_MCU8X + 0x1E;

        /// <summary>
        /// MCU不允许配置
        /// </summary>
        public const uint DEVICE_COM_MCU_NOT_ALLOW_CONFIG = ERR_DEVICE_COM_MCU8X + 0x1F;

        /// <summary>
        /// 端口已经被使用，请输入其它端口
        /// </summary>
        public const uint DEVICE_COM_MCU_SIP_TLS_USED = ERR_DEVICE_COM_MCU8X + 0x20;

        /// <summary>
        /// 另外一个用户正在ping
        /// </summary>
        public const uint DEVICE_COM_MCU_ANOTHER_USER_PING_NOW = ERR_DEVICE_COM_MCU8X + 0x21;
        #endregion

        /// <summary>
        /// 该输入项只能输入英文字母、数字、空格和常见符号
        /// </summary>
        public const uint DEVICE_COM_MCU8X_FAMILIAR_CHAR = ERR_DEVICE_COM_MCU8X + 0x22;

        /// <summary>
        /// 该输入项必须以.开始
        /// </summary>
        public const uint DEVICE_COM_MCU8X_BEGIN_DOT = ERR_DEVICE_COM_MCU8X + 0x23;

        /// <summary>
        /// 该输入项不能出现连续点
        /// </summary>
        public const uint DEVICE_COM_MCU8X_SERIES_DOT = ERR_DEVICE_COM_MCU8X + 0x24;

        /// <summary>
        /// 该输入项只能输入英文字母、数字和_
        /// </summary>
        public const uint DEVICE_COM_MCU8X_HOST = ERR_DEVICE_COM_MCU8X + 0x25;

        /// <summary>
        /// 该项只能输入英文字母、数字和以下符号.-_
        /// </summary>
        public const uint DEVICE_COM_MCU8X_SUFFIX = ERR_DEVICE_COM_MCU8X + 0x26;

        /// <summary>
        /// 请使用主板IP地址进行搜索
        /// </summary>
        public const uint DEVICE_COM_MCU8X_USE_BACKADDR = ERR_DEVICE_COM_MCU8X + 0x27;

        /// <summary>
        /// MCU HTTP 连接失败
        /// </summary>
        public const uint DEVICE_COM_HTTP_CONNECT_FAILED = ERR_DEVICE_COM_MCU8X + 0x28;

        /// <summary>
        /// 启用内置gk后无法修改gk地址
        /// </summary>
        public const uint DEVICE_COM_MCU_GK_ADDRESS_MODIFY_FAILED = ERR_DEVICE_COM_MCU8X + 0x29;

        /// <summary>
        /// 下载文件{filename}{0}，进度{fileprocess}{1}%
        /// </summary>
        public const uint DEVICE_COM_MCU8X_HTTP_PROGRESS_TYPE_DOWNLOAD = ERR_DEVICE_COM_MCU8X + 0x500;

        /// <summary>
        /// 连接ftp服务器,文件{filename}{0}，进度{fileprocess}{1}%
        /// </summary>
        public const uint DEVICE_COM_MCU8X_HTTP_PROGRESS_CONNECT_FTP_SERVER = ERR_DEVICE_COM_MCU8X + 0x501;

        /// <summary>
        /// 文件{filename}{0}合法性校验，进度{fileprocess}{1}%
        /// </summary>
        public const uint DEVICE_COM_MCU8X_HTTP_PROGRESS_TYPE_DOWLOAD_VERIFY = ERR_DEVICE_COM_MCU8X + 0x502;

        /// <summary>
        /// 写入文件{filename}{0}，进度{fileprocess}{1}%
        /// </summary>
        public const uint DEVICE_COM_MCU8X_HTTP_PROGRESS_TYPE_FILE_WRITE = ERR_DEVICE_COM_MCU8X + 0x503;

        /// <summary>
        /// 上传文件{filename}{0}，进度{fileprocess}{1}%
        /// </summary>
        public const uint DEVICE_COM_MCU8X_HTTP_PROGRESS_TYPE_UPLOAD = ERR_DEVICE_COM_MCU8X + 0x504;

        /// <summary>
        /// 读出文件{filename}{0}，进度{fileprocess}{1}%
        /// </summary>
        public const uint DEVICE_COM_MCU8X_HTTP_PROGRESS_TYPE_FILE_READ = ERR_DEVICE_COM_MCU8X + 0x505;

        /// <summary>
        /// 回读合法性校验文件{filename}{0}，进度{fileprocess}{1}%
        /// </summary>
        public const uint DEVICE_COM_MCU8X_HTTP_PROGRESS_TYPE_READ_VERIFY = ERR_DEVICE_COM_MCU8X + 0x506;

        /// <summary>
        /// 擦除flash文件{filename}{0}，进度{fileprocess}{1}%
        /// </summary>
        public const uint DEVICE_COM_MCU8X_HTTP_PROGRESS_TYPE_FLASH_CLEAR = ERR_DEVICE_COM_MCU8X + 0x507;

        /// <summary>
        /// 解压文件文件文件{filename}{0}，进度{fileprocess}{1}%
        /// </summary>
        public const uint DEVICE_COM_MCU8X_HTTP_PROGRESS_TYPE_DECOMPRESSING = ERR_DEVICE_COM_MCU8X + 0x508;

        /// <summary>
        /// 文件{filename}{0}类型错误，进度{fileprocess}{1}%
        /// </summary>
        public const uint DEVICE_COM_MCU8X_HTTP_PROGRESS_TYPE_ERROR = ERR_DEVICE_COM_MCU8X + 0x509;

        #endregion

        #region 9系列MCU通讯模块的错误码（从0x50B0B000开始）
        /// <summary>
        /// 9系列MCU通讯模块的错误码
        /// </summary>
        public const uint ERR_DEVICE_COM_MCU9X = 0x50B0B000;
        #endregion

        #region SNMP模块错误码（从0x50B0C000开始）
        /// <summary>
        /// SNMP模块的错误码
        /// </summary>
        public const uint ERR_DEVICE_COM_SNMP = 0x50B0C000;

        /// <summary>
        /// SNMP通信错误
        /// </summary>
        public const uint DEVICE_COM_SNMP_ERROR = ERR_DEVICE_COM_SNMP + 0x01;

        /// <summary>
        /// SNMP返回超时
        /// </summary>
        public const uint DEVICE_COM_SNMP_TIMEOUT = ERR_DEVICE_COM_SNMP + 0x02;

        /// <summary>
        /// 端口162不支持
        /// </summary>
        public const uint DEVICE_COM_SNMP_TRAPPORT_NONSUPPORT = ERR_DEVICE_COM_SNMP + 0x03;

        /// <summary>
        /// 设备mac地址重复
        /// </summary>
        public const uint DEVICE_MAC_ADDRESS_SNAME = ERR_DEVICE_COM_SNMP + 0x04;

        /// <summary>
        /// <div>搜索设备失败，可能原因：</div><div>1、设备ping失败；</div><div>2、设备不支持snmp通信；</div><div>3、输入snmp读团体名和设备上不一致。</div>
        /// </summary>
        public const uint DEVICE_COM_SNMP_SEARCH_FAILED = ERR_DEVICE_COM_SNMP + 0x05;

        /// <summary>
        /// 存在相同EngineID和SecurityName的Trap用户
        /// </summary>
        public const uint DEVICE_COM_TRAP_USER_EXIST = ERR_DEVICE_COM_SNMP + 0x06;

        /// <summary>
        /// 找不到指定的Trap用户信息
        /// </summary>
        public const uint DEVICE_COM_TRAP_USER_NOT_EXIST = ERR_DEVICE_COM_SNMP + 0x07;

        /// <summary>
        /// EngineID长度为[2,64]
        /// </summary>
        public const uint DEVICE_COM_TRAP_USER_ENGINEID_LENGTH = ERR_DEVICE_COM_SNMP + 0x08;

        /// <summary>
        /// EngineID长度必须是偶数
        /// </summary>
        public const uint DEVICE_COM_TRAP_USER_ENGINEID_LENGTH_EVEN = ERR_DEVICE_COM_SNMP + 0x09;

        /// <summary>
        /// EngineID必须是16进制字符串
        /// </summary>
        public const uint DEVICE_COM_TRAP_USER_ENGINEID_HEXSTRING = ERR_DEVICE_COM_SNMP + 0x0A;

        /// <summary>
        /// Trap用户名长度为[1,32]
        /// </summary>
        public const uint DEVICE_COM_TRAP_USER_NAME_LENGTH = ERR_DEVICE_COM_SNMP + 0x0B;

        /// <summary>
        /// 认证密码长度为[1,48]
        /// </summary>
        public const uint DEVICE_COM_TRAP_USER_AUTHPASSWORD_LENGTH = ERR_DEVICE_COM_SNMP + 0x0C;

        /// <summary>
        /// 加密密码长度为[1,48]
        /// </summary>
        public const uint DEVICE_COM_TRAP_USER_PRIVPASSWORD_LENGTH = ERR_DEVICE_COM_SNMP + 0x0D;
        #endregion


        /// <summary>
        /// 9系列MCU通讯模块的错误码
        /// </summary>
        public const uint ERR_HTTP_COM_MCU = 0x50B0D000;

        /// <summary>
        /// HTTP日志下载异常
        /// </summary>
        public const uint ERR_DOWNLOAD_LOGLIST = ERR_HTTP_COM_MCU + 0x01;

        /// <summary>
        /// HTTP请求MCU异常
        /// </summary>
        public const uint ERR_POST_REQUEST = ERR_HTTP_COM_MCU + 0x02;

        /// <summary>
        /// 导出日志文件不存在
        /// </summary>
        public const uint ERR_LOG_NOT_EXITES = ERR_HTTP_COM_MCU + 0x03;

        /// <summary>
        /// 接口返回数据序列化异常
        /// </summary>
        public const uint ERR_INTFACE_EXCEPA = ERR_HTTP_COM_MCU + 0x04;

        /// <summary>
        /// 从MCU导出配置文件为空
        /// </summary>
        public const uint ERR_CONFIG_FILE_NOTEXITES = ERR_HTTP_COM_MCU + 0x05;


        /// <summary>
        /// MCU升级上传文件失败
        /// </summary>
        public const uint ERR_UPDATE_SEND_FILE = ERR_HTTP_COM_MCU + 0x06;



    }
}
