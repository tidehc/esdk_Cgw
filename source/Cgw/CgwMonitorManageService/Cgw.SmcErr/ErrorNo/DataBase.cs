//-----------------------------------------------------------------------
//文件名：ErrorNo/DataBase.cs
//版权：Copyright 2010-2012 Huawei Tech. Co. Ltd. All Rights Reserved. 
//描述：数据库服务的错误码
//-----------------------------------------------------------------------
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace CgwMonitorManage.SmcError
{
    /// <summary>
    /// SmcErr类：数据库服务错误码，错误码从0x50700000开始
    /// </summary>
    public partial class SmcErr
    {
        #region EF模块错误码0x50704000
        /// <summary>
        /// EF模块错误码0x50704000
        /// </summary>
        public const uint ERR_DB_ENTITYFRAMEWORK = 0x50704000;

        /// <summary>
        /// 数据库服务获取事务失败。
        /// </summary>
        public const uint ERR_DB_GETTRANSACTION = ERR_DB_ENTITYFRAMEWORK;

        /// <summary>
        /// 数据库服务获取表中的主键值失败。
        /// </summary>
        public const uint ERR_DB_GETALLKEY = 0x50704000 + 0x01;

        /// <summary>
        /// 数据库连接异常或者断开。
        /// </summary>
        public const uint ERR_DB_SERVER = 0x50704000 + 0x02;
        #endregion

        #region 数据库关于设备管理错误码0x50705000
        /// <summary>
        /// 数据库关于设备管理错误码0x50705000
        /// </summary>
        public const uint ERR_DB_DEVICE = 0x50705000;

        /// <summary>
        /// 数据库服务创建新事务失败。
        /// </summary>
        public const uint DB_TRAN_CREATE_R2 = 0x50705000;

        /// <summary>
        /// 数据库服务提交新事务失败。
        /// </summary>
        public const uint DB_TRAN_COMMIT_R2 = 0x50705000 + 0x01;

        /// <summary>
        /// 数据库服务回滚新事务失败。
        /// </summary>
        public const uint DB_TRAN_ROLLBACK_R2 = 0x50705000 + 0x02;

        /// <summary>
        /// 数据库服务提交或回滚事务时，未找到此事务。
        /// </summary>
        public const uint DB_DATA_NOTIN_R2 = 0x50705000 + 0x03;

        /// <summary>
        /// 数据库服务添加设备时，传入设备参数有误。
        /// </summary>
        public const uint DB_DOWN_ADDDEVICE = 0x50705000 + 0x04;

        /// <summary>
        /// 数据库服务添加或修改设备时，传入设备参数有误。
        /// </summary>
        public const uint DB_DOWN_DEVICE = 0x50705000 + 0x05;

        /// <summary>
        /// 数据库服务添加设备失败。
        /// </summary>
        public const uint DB_ADD_ADDDEVICE = 0x50705000 + 0x06;

        /// <summary>
        /// 数据库服务修改设备数据时，传入设备参数有误。
        /// </summary>
        public const uint DB_DOWN_UPDATEDEVICE = 0x50705000 + 0x07;

        /// <summary>
        /// 数据库服务修改设备数据失败。
        /// </summary>
        public const uint DB_EDIT_UPDATEDEVICE = 0x50705000 + 0x08;

        /// <summary>
        /// 数据库服务删除设备数据时，传入设备参数有误。
        /// </summary>
        public const uint DB_DOWN_DELDEVICE = 0x50705000 + 0x09;

        /// <summary>
        /// 数据库服务删除设备失败。
        /// </summary>
        public const uint DB_DEL_DELDEVICE = 0x50705000 + 0x0A;

        /// <summary>
        /// 数据库服务加载所有设备失败。
        /// </summary>
        public const uint DB_LOAD_ALLDEVICE = 0x50705000 + 0x0B;

        /// <summary>
        /// 数据库服务更新设备状态时，传入设备参数有误。
        /// </summary>
        public const uint DB_DOWN_UPDATESTATE = 0x50705000 + 0x0C;

        /// <summary>
        /// 数据库服务更新设备状态失败。
        /// </summary>
        public const uint DB_UPDATE_STATEFAIL = 0x50705000 + 0x0D;

        /// <summary>
        /// 数据库服务检查预发放设备IP地址时，传入设备参数有误。
        /// </summary>
        public const uint DB_DOWN_CHECKIP = 0x50705000 + 0x0E;

        /// <summary>
        /// 数据库服务检查预发放设备IP地址失败。
        /// </summary>
        public const uint DB_CHECKIP = 0x50705000 + 0x10;

        /// <summary>
        /// 数据库服务检查可管理设备Mac地址时，传入设备参数有误。
        /// </summary>
        public const uint DB_DOWN_CHECKMAC = 0x50705000 + 0x11;

        /// <summary>
        /// 数据库服务检查可管理设备Mac地址失败。
        /// </summary>
        public const uint DB_CHECKMAC = 0x50705000 + 0x12;

        /// <summary>
        /// 数据库服务查询单个设备信息时，传入设备参数有误。
        /// </summary>
        public const uint DB_DOWN_QUERYSINGLE = 0x50705000 + 0x13;

        /// <summary>
        /// 数据库服务查询单个设备信息失败。
        /// </summary>
        public const uint DB_QUERYSINGLE = 0x50705000 + 0x14;

        /// <summary>
        /// 数据库服务删除预发放设备或自动发现设备时，传入参数有误。
        /// </summary>
        public const uint DB_DOWN_DELETEFAIL = 0x50705000 + 0x15;

        /// <summary>
        /// 数据库服务删除预发放设备或自动发现设备失败。
        /// </summary>
        public const uint DB_DELETEFAIL = 0x50705000 + 0x16;

        /// <summary>
        /// 数据库服务添加告警时，传入告警参数有误。
        /// </summary>
        public const uint DB_DOWN_ADDALARM = 0x50705000 + 0x17;

        /// <summary>
        /// 数据库服务添加告警失败。
        /// </summary>
        public const uint DB_ADDALARM = 0x50705000 + 0x18;

        /// <summary>
        /// 数据库服务确认告警时，传入告警参数有误。
        /// </summary>
        public const uint DB_DOWN_CONFIRMALARM = 0x50705000 + 0x19;

        /// <summary>
        /// 数据库服务确认告警失败。
        /// </summary>
        public const uint DB_CONFIRMALARM = 0x50705000 + 0x1A;

        /// <summary>
        /// 数据库服务删除告警时，传入告警参数有误。
        /// </summary>
        public const uint DB_DOWN_DELETEALARM = 0x50705000 + 0x1B;

        /// <summary>
        /// 数据库服务确认告警失败。
        /// </summary>
        public const uint DB_DELETEALARM = 0x50705000 + 0x1C;

        /// <summary>
        /// 数据库服务加载所有告警失败。
        /// </summary>
        public const uint DB_LOAD_ALARMS = 0x50705000 + 0x1D;

        /// <summary>
        /// 数据库服务加载告警定位信息失败。
        /// </summary>
        public const uint DB_LOADORIENT = 0x50705000 + 0x1E;

        /// <summary>
        /// 数据库服务添加设备配置模板时，传入参数有误。
        /// </summary>
        public const uint DB_DOWN_ADDCONFIGTEMPLATE = 0x50705000 + 0x20;

        /// <summary>
        /// 数据库服务添加设备配置模板失败。
        /// </summary>
        public const uint DB_ADDCONFIGTEMPLATE = 0x50705000 + 0x21;

        /// <summary>
        /// 数据库服务修改设备配置模板时，传入参数有误。
        /// </summary>
        public const uint DB_DOWN_UPDATECONFIGTEMPLATE = 0x50705000 + 0x22;

        /// <summary>
        /// 数据库服务修改设备配置模板失败。
        /// </summary>
        public const uint DB_UPDATECONFIGTEMPLATE = 0x50705000 + 0x23;

        /// <summary>
        /// 数据库服务删除设备配置模板时，传入参数有误。
        /// </summary>
        public const uint DB_DOWN_DELETECONFIGTEMPLATE = 0x50705000 + 0x24;

        /// <summary>
        /// 数据库服务删除设备配置模板失败。
        /// </summary>
        public const uint DB_DELETECONFIGTEMPLATE = 0x50705000 + 0x25;

        /// <summary>
        /// 数据库服务加载设备配置模板失败。
        /// </summary>
        public const uint DB_LOAD_CONFIGTEMPLATE = 0x50705000 + 0x26;

        /// <summary>
        /// 数据库服务查询单个设备配置模板时，传入参数有误。
        /// </summary>
        public const uint DB_DOWN_QUERYCONFIGTEMPLATE = 0x50705000 + 0x27;

        /// <summary>
        /// 数据库服务查询单个设备配置模板失败。
        /// </summary>
        public const uint DB_QUERYCONFIGTEMPLATE = 0x50705000 + 0x28;

        /// <summary>
        /// 数据库服务添加设备软件时，传入参数有误。
        /// </summary>
        public const uint DB_DOWN_ADDDEVICESOFT = 0x50705000 + 0x29;

        /// <summary>
        /// 数据库服务添加设备软件失败。
        /// </summary>
        public const uint DB_ADDDEVICESOFT = 0x50705000 + 0x2A;

        /// <summary>
        /// 数据库服务删除设备软件时，传入参数有误。
        /// </summary>
        public const uint DB_DOWN_DELETEDEVICESOFT = 0x50705000 + 0x2B;

        /// <summary>
        /// 数据库服务删除设备软件失败。
        /// </summary>
        public const uint DB_DELETEDEVICESOFT = 0x50705000 + 0x2C;

        /// <summary>
        /// 数据库服务加载设备软件失败。
        /// </summary>
        public const uint DB_LOAD_DEVICESOFT = 0x50705000 + 0x2D;

        /// <summary>
        /// 数据库服务添加设备升级任务时，传入参数有误。
        /// </summary>
        public const uint DB_DOWN_ADDUPGRADE = 0x50705000 + 0x2E;

        /// <summary>
        /// 数据库服务添加设备升级任务失败。
        /// </summary>
        public const uint DB_ADDUPGRADE = 0x50705000 + 0x30;

        /// <summary>
        /// 数据库服务修改设备升级任务时，传入参数有误。
        /// </summary>
        public const uint DB_DOWN_UPDATEUPGRADE = 0x50705000 + 0x31;

        /// <summary>
        /// 数据库服务修改设备升级任务失败。
        /// </summary>
        public const uint DB_UPDATEUPGRADE = 0x50705000 + 0x32;

        /// <summary>
        /// 数据库服务删除设备升级任务时，传入参数有误。
        /// </summary>
        public const uint DB_DOWN_DELETEUPGRADE = 0x50705000 + 0x33;

        /// <summary>
        /// 数据库服务删除设备升级任务失败。
        /// </summary>
        public const uint DB_DELETEUPGRADE = 0x50705000 + 0x34;

        /// <summary>
        /// 数据库服务加载设备升级任务失败。
        /// </summary>
        public const uint DB_LOAD_UPGRADE = 0x50705000 + 0x35;

        /// <summary>
        /// 数据库服务添加设备配置备份时，传入参数有误。
        /// </summary>
        public const uint DB_DOWN_ADDDEVICEBACKUP = 0x50705000 + 0x36;

        /// <summary>
        /// 数据库服务添加设备配置备份失败。
        /// </summary>
        public const uint DB_ADDDEVICEBACKUP = 0x50705000 + 0x37;

        /// <summary>
        /// 数据库服务提取设备配置备份时，传入参数有误。
        /// </summary>
        public const uint DB_DOWN_QUERYDEVICEBACKUP = 0x50705000 + 0x38;

        /// <summary>
        /// 数据库服务提取设备配置备份时，未找到备份文件。
        /// </summary>
        public const uint DB_DEVICEBACKUP_ISNOT = 0x50705000 + 0x39;

        /// <summary>
        /// 数据库服务提取设备配置备份失败。
        /// </summary>
        public const uint DB_QUERYDEVICEBACKUP = 0x50705000 + 0x3A;

        /// <summary>
        /// 数据库查询设备列表信息时，传入参数有误。
        /// </summary>
        public const uint DB_DOWN_QUERYDEVICELIST = 0x50705000 + 0x3B;

        /// <summary>
        /// 数据库查询设备列表信息失败。
        /// </summary>
        public const uint DB_QUERYDEVICELIST = 0x50705000 + 0x3C;

        /// <summary>
        /// 数据库查询预发放设备列表信息时，传入参数有误。
        /// </summary>
        public const uint DB_DOWN_QUERYPREPAREDEVICELIST = 0x50705000 + 0x3D;

        /// <summary>
        /// 数据库查询预发放设备列表信息失败。
        /// </summary>
        public const uint DB_QUERYPREPAREDEVICELIST = 0x50705000 + 0x3E;

        /// <summary>
        /// 数据库查询自动发现设备列表信息时，传入参数有误。
        /// </summary>
        public const uint DB_DOWN_QUERYAUTODEVICELIST = 0x50705000 + 0x40;

        /// <summary>
        /// 数据库查询自动发现设备列表信息失败。
        /// </summary>
        public const uint DB_QUERYAUTODEVICELIST = 0x50705000 + 0x41;

        /// <summary>
        /// 数据库查询GK列表信息时，传入参数有误。
        /// </summary>
        public const uint DB_DOWN_QUERYGKLIST = 0x50705000 + 0x42;

        /// <summary>
        /// 数据库查询GK列表信息失败。
        /// </summary>
        public const uint DB_QUERYGKLIST = 0x50705000 + 0x43;

        /// <summary>
        /// 数据库查询设备告警列表信息时，传入参数有误。
        /// </summary>
        public const uint DB_DOWN_QUERYDEVICEALARMLIST = 0x50705000 + 0x44;

        /// <summary>
        /// 数据库查询设备告警列表信息失败。
        /// </summary>
        public const uint DB_QUERYDEVICEALARMLIST = 0x50705000 + 0x45;

        /// <summary>
        /// 数据库查询设备历史告警列表信息时，传入参数有误。
        /// </summary>
        public const uint DB_DOWN_QUERYDEVICEHOSTORYALARMLIST = 0x50705000 + 0x46;

        /// <summary>
        /// 数据库查询设备历史告警列表信息失败。
        /// </summary>
        public const uint DB_QUERYDEVICEHOSTORYALARMLIST = 0x50705000 + 0x47;

        /// <summary>
        /// 数据库查询所有告警列表信息时，传入参数有误。
        /// </summary>
        public const uint DB_DOWN_QUERYALLALARMLIST = 0x50705000 + 0x48;

        /// <summary>
        /// 数据库查询所有告警列表信息失败。
        /// </summary>
        public const uint DB_QUERYALLALARMLIST = 0x50705000 + 0x49;

        /// <summary>
        /// 数据库查询所有历史告警列表信息时，传入参数有误。
        /// </summary>
        public const uint DB_DOWN_QUERYHISTORYALARMLIST = 0x50705000 + 0x4A;

        /// <summary>
        /// 数据库查询所有历史告警列表信息失败。
        /// </summary>
        public const uint DB_QUERYHISTORYALARMLIST = 0x50705000 + 0x4B;

        /// <summary>
        /// 数据库查询设备升级列表信息时，传入参数有误。
        /// </summary>
        public const uint DB_DOWN_QUERYUPGRADELIST = 0x50705000 + 0x4C;

        /// <summary>
        /// 数据库查询设备升级列表信息失败。
        /// </summary>
        public const uint DB_QUERYUPGRADELIST = 0x50705000 + 0x4D;

        /// <summary>
        /// 数据库查询设备模板应用的设备列表信息时，传入参数有误。
        /// </summary>
        public const uint DB_DOWN_QUERYTEMPLATEDEVICELIST = 0x50705000 + 0x4E;

        /// <summary>
        /// 数据库查询设备模板应用的设备列表信息失败。
        /// </summary>
        public const uint DB_QUERYTEMPLATEDEVICELIST = 0x50705000 + 0x50;

        /// <summary>
        /// 数据库查询设备或系统日志列表信息时，传入参数有误。
        /// </summary>
        public const uint DB_DOWN_QUERYLOGLIST = 0x50705000 + 0x51;

        /// <summary>
        /// 数据库查询设备或系统日志列表信息失败。
        /// </summary>
        public const uint DB_QUERYLOGLIST = 0x50705000 + 0x52;

        /// <summary>
        /// 数据库查询导出设备或系统日志列表信息时，传入参数有误。
        /// </summary>
        public const uint DB_DOWN_EXPORTLOGLIST = 0x50705000 + 0x53;

        /// <summary>
        /// 数据库查询导出设备或系统日志列表信息失败。
        /// </summary>
        public const uint DB_EXPORTLOGLIST = 0x50705000 + 0x54;

        /// <summary>
        /// 数据库服务查询会场列表信息时，传入参数有误。
        /// </summary>
        public const uint DB_DOWN_SITELIST = 0x50705000 + 0x55;

        /// <summary>
        /// 数据库服务查询会场列表信息失败。
        /// </summary>
        public const uint DB_SITELIST = 0x50705000 + 0x56;

        /// <summary>
        /// 数据库服务加载会场信息失败。
        /// </summary>
        public const uint DB_LOAD_SITERELATION = 0x50705000 + 0x57;

        /// <summary>
        /// 数据库服务加载MCU信息失败。
        /// </summary>
        public const uint DB_LOAD_MCURELATION = 0x50705000 + 0x58;

        /// <summary>
        /// 数据库服务删除组织信息时，传入参数有误。
        /// </summary>
        public const uint DB_DOWN_DELORG = 0x50705000 + 0x59;

        /// <summary>
        /// 数据库服务删除组织信息失败。
        /// </summary>
        public const uint DB_DELORG = 0x50705000 + 0x5A;

        /// <summary>
        /// 数据库服务修改设备与组织关系失败。
        /// </summary>
        public const uint DB_UPDATEORG = 0x50705000 + 0x5B;

        /// <summary>
        /// 数据库服务查询单个告警信息来源失败。
        /// </summary>
        public const uint DB_QUERY_ALARMSOURCENAME = 0x50705000 + 0x5C;

        /// <summary>
        /// 数据库服务修改设备信息时，设备不存在或数据已经被更新。
        /// </summary>
        public const uint DB_DOWN_DATAVER = 0x50705000 + 0x5D;

        /// <summary>
        /// 数据库服务修改录播信息失败。
        /// </summary>
        public const uint DB_LOAD_NRSRELATION = 0x50705000 + 0x5E;

        /// <summary>
        /// 数据库自动清理数据。
        /// </summary>
        public const uint DB_AUTO_DELETE = 0x50705000 + 0x5F;

        /// <summary>
        /// 数据库服务启动失败。
        /// </summary>
        public const uint DB_RUN_FAIL = 0x50705000 + 0x60;

        /// <summary>
        /// 数据库服务成功停止。
        /// </summary>
        public const uint DB_STOP_SUCCESS = 0x50705000 + 0x61;

        /// <summary>
        /// 数据库服务定时清理启动成功。
        /// </summary>
        public const uint DB_AUTO_SUCCESS = 0x50705000 + 0x62;

        /// <summary>
        /// 数据库服务定时清理启动失败。
        /// </summary>
        public const uint DB_AUTO_FAIL = 0x50705000 + 0x63;

        /// <summary>
        /// 数据库服务查询告警状态统计信息失败。
        /// </summary>
        public const uint DB_QUERY_ALARM_STAT = 0x50705000 + 0x64;

        /// <summary>
        /// 数据库服务添加活动告警失败，因为设备已经被删除。
        /// </summary>
        public const uint DB_ADD_ALARM_DEVICE_DEL = 0x50705000 + 0x65;

        /// <summary>
        /// 数据库服务成功开启。
        /// </summary>
        public const uint DB_START_SUCCESS = 0x50705000 + 0x66;

        /// <summary>
        /// 数据库服务自动清除被标记删除会议模板{detail}{0}个。
        /// </summary>
        public const uint DB_AUTO_DEL_CONFTEMPLATE = 0x50705000 + 0x67;

        /// <summary>
        /// 数据库服务自动清除被标记删除用户{detail}{0}个。
        /// </summary>
        public const uint DB_AUTO_DEL_USER = 0x50705000 + 0x68;

        /// <summary>
        /// 数据库服务自动清除被标记删除组织{detail}{0}个。
        /// </summary>
        public const uint DB_AUTO_DEL_ORG = 0x50705000 + 0x69;

        /// <summary>
        /// 数据库服务自动清除被标记删除设备{detail}{0}个。
        /// </summary>
        public const uint DB_AUTO_DEL_DEVICE = 0x50705000 + 0x6A;

        /// <summary>
        /// 数据库服务自动清除被标记删除系统告警{detail}{0}个。
        /// </summary>
        public const uint DB_AUTO_DEL_DEVICEALARM = 0x50705000 + 0x6B;

        /// <summary>
        /// 数据库服务自动清除被标记删除会议{detail}{0}个。
        /// </summary>
        public const uint DB_AUTO_DEL_CONF = 0x50705000 + 0x6C;

        /// <summary>
        /// 数据库服务添加SNMP信息时，传入参数有误。
        /// </summary>
        public const uint DB_DOWN_ADDSNMP = 0x50705000 + 0x6D;

        /// <summary>
        /// 数据库服务添加SNMP信息失败。
        /// </summary>
        public const uint DB_ADD_SNMP = 0x50705000 + 0x6E;

        /// <summary>
        /// 数据库服务修改SNMP信息时，传入参数有误。
        /// </summary>
        public const uint DB_DOWN_EDITSNMP = 0x50705000 + 0x6F;

        /// <summary>
        /// 数据库服务修改SNMP信息失败。
        /// </summary>
        public const uint DB_EDIT_SNMP = 0x50705000 + 0x70;

        /// <summary>
        /// 数据库服务修改SNMP信息失败，未找到数据。
        /// </summary>
        public const uint DB_NOTEXIST_EDITSNMP = 0x50705000 + 0x71;

        /// <summary>
        /// 数据库服务删除SNMP信息失败，未找到数据。
        /// </summary>
        public const uint DB_NOTEXIST_DELSNMP = 0x50705000 + 0x72;

        /// <summary>
        /// 数据库服务加载所有SNMP信息失败。
        /// </summary>
        public const uint DB_LOAD_ALLSNMP = 0x50705000 + 0x73;

        /// <summary>
        /// 数据库加载当前所有告警名称类型时，传入参数有误。
        /// </summary>
        public const uint DB_DOWN_LOAD_ALLDESCRIPTION = 0x50705000 + 0x74;

        /// <summary>
        /// 数据库加载当前所有告警名称类型失败。
        /// </summary>
        public const uint DB_LOAD_ALLDESCRIPTION = 0x50705000 + 0x75;
        #endregion

        #region 新增R1错误码0x50706000
        /// <summary>
        /// 数据库在编辑计费码时，传入参数有误。
        /// </summary>
        public const uint DB_DOWN_EDIT_BILLCODE = 0x50706000;

        /// <summary>
        /// 计费码已经被删除。
        /// </summary>
        public const uint DB_EDIT_BILLCODE_ISNOT = 0x50706000 + 0x01;

        /// <summary>
        /// 计费码已经被修改。
        /// </summary>
        public const uint DB_EDITED_BILLCODE = 0x50706000 + 0x02;
        #endregion 
    }
}
