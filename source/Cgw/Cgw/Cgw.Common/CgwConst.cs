/*-----------------------------------------------------------------------
//文件名：CgwConst.cs
//版权：Copyright 2011-2012 Huawei Tech. Co. Ltd. All Rights Reserved. 
//作者：f00204327
//日期：2013-2-27
//描述：常量类
//---------------------------------------------------------------------*/
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Cgw.Common
{
    public class CgwConst
    {
        /// <summary>
        /// 通道模块进入通道关系字典读写锁等待时间 5s
        /// </summary>
        public const int ENTER_LOCK_WAIT_TIME = 5000;


        public static string CGW_CONFIG_FILE_NAME = AppDomain.CurrentDomain.BaseDirectory + "/CgwConfig.xml";

        /// <summary>
        /// 特殊字符---在加密项经过加密后的字符串前加上特殊字符，写入配置文件，读取配置文件时，用来判断是否加密。
        /// </summary>
        public const string ENCRYPTED_PREFIX = "①≠②";

        /// <summary>
        /// 用来判断是否需要加密
        /// </summary>
        public const string ISNEEDENCRYPT = "TRUE";

        /// <summary>
        /// 管道管理读写锁等待时间 2s
        /// </summary>
        public const int ENTER_PIPE_LOCK_WAIT_TIME = 2000;

        #region 配置文件中的servicesSettings节点
        /// <summary>
        /// 配置文件中的servicesSettings标记
        /// </summary>
        public const string SERVICES_SETTINGS_TAG = "/configuration/servicesSettings";

        /// <summary>
        /// 配置文件的connectPassword标记
        /// </summary>
        //public const string CONNECT_PASSWORD_TAG = "connectPassword";

        /// <summary>
        /// 配置文件的CONNECT_MONITOR_MANAGE_PASSWORD_TAG标记
        /// </summary>
        //public const string CONNECT_MONITOR_MANAGE_PASSWORD_TAG = "connectMonitorManagePassword";

        /// <summary>
        /// 配置文件中的sessionOverTime标记
        /// </summary>
        public const string SESSION_OVERTIME_TAG = "sessionOverTime";

        /// <summary>
        /// 配置文件中的monitorPlatformCameraGroupName标记
        /// </summary>
        public const string MONITOR_PLATFORM_CAMERA_GROUP_NAME = "monitorPlatformCameraGroupName";

        /// <summary>
        /// 配置文件中的eLTEPlatformCameraGroupName标记
        /// </summary>
        public const string ELTE_PLATFORM_CAMERA_GROUP_NAME = "eLTEPlatformCameraGroupName";

        /// <summary>
        /// 配置文件中的cgwServiceIp标记
        /// </summary>
        public const string CGWSERVICE_IP_TAG = "cgwServiceIp";

        /// <summary>
        /// 配置文件中的ptzControlLevel节点
        /// </summary>
        public const string PTZ_CONTROL_LEVEL_TAG = "ptzControlLevel";

        /// <summary>
        /// 配置文件中的cgwChannelRTPStartPort节点
        /// </summary>
        public const string CGW_CHANNEL_RTP_START_PORT_TAG = "cgwChannelRTPStartPort";

        /// <summary>
        /// 配置文件中的cgwChannelRTPPortCount节点
        /// </summary>
        public const string CGW_CHANNEL_RTP_PORT_COUNT_TAG = "cgwChannelRTPPortCount";

        /// <summary>
        /// 配置文件中的cgwChanelCallPort节点
        /// </summary>
        public const string CGW_CHANNEL_CALL_PORT = "cgwChanelCallPort";

        /// <summary>
        /// 配置文件中的cgwChanelNoDataCount节点
        /// </summary>
        public const string CGW_CHANNEL_NODATA_COUNT = "cgwChanelNoDataCount";
        

        /// <summary>
        /// 配置文件中的cgwAudioProtocol节点
        /// </summary>
        public const string CGW_Audio_Protocol = "cgwAudioProtocol";

        /// <summary>
        /// 配置文件中needEncrypt属性
        /// </summary>
        public const string NEEDENCRYPT_TAG = "needEncrypt";

        /// <summary>
        /// 连接监控通道的时间间隔
        /// </summary>
        public const int CONNECT_MONITOR_MANAGE_WAIT_TIME = 10000;

        /// <summary>
        /// 连接监控通道的保活时间间隔
        /// </summary>
        public const int MONITOR_MANAGE_KEEPLIVE_WAIT_TIME = 5000;

        /// <summary>
        /// 等待监控服务启动时间
        /// </summary>
        public const int WAIT_MONITOR_SERVICE_RUNNING_MILLI_SECONDS = 10000;
        #endregion

        #region 平台管理及接口适配层使用常量

        /// <summary>
        /// 平台ID及摄像头分隔标记，平台管理统一用“平台id”+“###”+“摄像头编号”，生成唯一的编号，
        /// 防止不同的监控平台摄像头id重复的问题
        /// </summary>
        public const string MONITOR_SEPARATOR_TAG = "###";

        /// <summary>
        /// 摄像头编号用分隔符分隔后的长度是2
        /// </summary>
        public const int ENCODE_CAMERANO_LENGHT = 2;

        /// <summary>
        /// 监控配置文件中monitors节点标记
        /// </summary>
        public const string MONITORS_CONFIG_ROOT_TAG = "/configuration/monitorSettings";

        /// <summary>
        /// 监控配置文件中monitor标记
        /// </summary>
        public const string MONITOR_TAG = "monitor";

        /// <summary>
        /// 监控配置文件中id标记
        /// </summary>
        public const string ID_TAG = "id";

        /// <summary>
        /// 监控配置文件中name标记
        /// </summary>
        public const string NAME_TAG = "name";

        /// <summary>
        /// 监控配置文件中class标记
        /// </summary>
        public const string CLASS_TAG = "class";

        /// <summary>
        /// 监控配置文件中ip的标记
        /// </summary>
        public const string IP_TAG = "ip";

        /// <summary>
        /// 监控配置文件中port的标记
        /// </summary>
        public const string PORT_TAG = "port";

        /// <summary>
        /// 监控配置文件中user的标记
        /// </summary>
        public const string USER_TAG = "user";

        /// <summary>
        /// 监控配置文件中password的标记
        /// </summary>
        public const string PASSWORD_TAG = "password";

        /// <summary>
        /// 监控配置文件中enable的标记
        /// </summary>
        public const string ENABLE_TAG = "enable";

        /// <summary>
        /// true字符串
        /// </summary>
        public const string TRUE_TAG = "true";

        /// <summary>
        /// T28181 SIP_PORT
        /// </summary>
        public const string SIP_PORT = "SipPort";

        /// <summary>
        /// T28181 LOCAL_PORT
        /// </summary>
        public const string LOCAL_PORT = "LocalPort";

        /// <summary>
        /// T28181 QueryDeviceTimeOut
        /// </summary>
        public const string QueryDeviceTimeOut = "QueryDeviceTimeOut";

        /// <summary>
        /// T28181 DeviceID
        /// </summary>
        public const string Device_ID = "DeviceID";

        /// <summary>
        /// T28181查询设备等待时间（毫秒）
        /// </summary>
        public const int Thread_Sleep_Time = 1000;

        /// <summary>
        /// 监控配置文件中T28181 monitors节点标记
        /// </summary>
        public const string T28181_MONITORS_CONFIG_ROOT_TAG = "/configuration/T28181monitorSettings";

        /// <summary>
        /// eLTE FIXEDCAMERA 
        /// </summary>
        public const string FIXED_CAMERA = "UserCategory/FIXEDCAMERA";

        /// <summary>
        /// eLTE PTTUSER 
        /// </summary>
        public const string PTT_USER = "UserCategory/PTTUSER";

        /// <summary>
        /// 视频格式 
        /// </summary>
        public const string FMT_VALUE = "VideoParameter/FmtValue";

        /// <summary>
        /// 表示前置 
        /// </summary>
        public const string CAMERA_TYPE = "VideoParameter/CameraType";

        /// <summary>
        /// 表示不需要用户确认 
        /// </summary>
        public const string USER_CONFIRM_TYPE = "VideoParameter/UserConfirmType";

        /// <summary>
        /// 表示需要伴音 
        /// </summary>
        public const string MUTE_TYPE = "VideoParameter/MuteType";

        /// <summary>
        /// 监控配置文件中Tiandy Easy7平台用户配置项索引
        /// </summary>
        public const string PLATFORM_USER_TAG = "platform/user";

        /// <summary>
        /// 监控配置文件中Tiandy Easy7平台用户密码配置项索引
        /// </summary>
        public const string PLATFORM_PASSWORD_TAG = "platform/password";

        /// <summary>
        /// 监控配置文件中Tiandy Easy7 url配置项索引
        /// </summary>
        public const string PLATFORM_URL_TAG = "platform/url";

        /// <summary>
        /// Tiandy中间件（流媒体）服务器id配置配置项索引
        /// </summary>
        public const string MEDIASERVER_SERVERID_TAG = "mediaServer/serverid";

        /// <summary>
        /// 预览摄像头失败标记，天地伟业返回-1标识失败
        /// </summary>
        public const int START_RECEIVE_VIDEO_DATA_FAILED = -1;

        /// <summary>
        /// 监控平台定时刷新设备列表，时间间隔1小时
        /// 监控平台设备列表变化不会太频繁，时间间隔设置比较长
        /// </summary>
        public const int REFRESH_CAMERA_LIST_WAIT_TIME = 3600000;

        /// <summary>
        /// 天地伟业码流回调，接受视频标记
        /// </summary>
        public const int MESS_VIDEO_ACCEPT = 7;

        /// <summary>
        /// 判断是否成功接收码流等待时间
        /// </summary>
        public const int CHECK_RECEIVE_STATUS_WAIT_TIME = 500;

        /// <summary>
        /// IVS SDK接口，返回值的成功标记，如果为0标识成功，否则返回错误码
        /// </summary>
        public const int IVS_SDK_SUCCESS_TAG = 0;

        /// <summary>
        /// IVS SDK是用的分隔符，分组编号上加上域编码，标识唯一，用#分隔，目前IVS摄像头编号是用#分隔的
        /// </summary>
        public const string IVS_SDK_DOMAINCODE_SEPARATOR_TAG = "#";

        /// <summary>
        /// IVS SDK分组列表根节点,目前IVS摄像头编号是用#分隔的
        /// </summary>
        public const string IVS_SDK_ROOTGROUP_TAG = "0";

        /// <summary>
        /// 第一页开始记录索引
        /// </summary>
        public const int PAGE_FIRST_INDEX = 1;

        /// <summary>
        /// 第一页结束记录索引,一页2000条记录
        /// </summary>
        public const int PAGE_LAST_INDEX = 2000;

        /// <summary>
        /// Tiandy PTZ控制节点配置项索引
        /// </summary>
        public const string PTZCONTROLLEVEL_TAG = "platform/ptzControlLevel";

        /// <summary>
        /// T28181错误的播放通道
        /// </summary>
        public const UInt32 T28181_ERROR_HANDLE = 0;

        /// <summary>
        /// PTZ控制级别默认值，为1
        /// </summary>
        public const int PTZ_CONTROL_LEVEL = 1;

        #endregion

        #region region 共享设备类型
        /**
     * 共享主设备类型
     */
        public const string RESOURCE_TYPE_MAIN = "00";

        /**
         * 共享摄像机类型
         */
        public const string RESOURCE_TYPE_CAMERA = "01";

        /**
         * 共享开关量输入类型
         */
        public const string RESOURCE_TYPE_ALARM_IN = "02";

        /**
         * 共享开关量输出类型
         */
        public const string RESOURCE_TYPE_ALARM_OUT = "03";

        /**
         * 平台设备
         */
        public const string RESOURCE_TYPE_PLAT_DEV = "98";

        /**
         * 共享目录类型
         */
        public static string RESOURCE_TYPE_DIR = "99";

        /**
 * 主设备起始编码
 */
        public const int DEVICE_TYPE_MAIN_START = 111;

        /**
         * 主设备结束编码
         */
        public const int DEVICE_TYPE_MAIN_END = 130;

        /**
         * 摄像机编码
         */
        public const int DEVICE_TYPE_CAMERA = 131;

        /**
         * 网络摄像机编码
         */
        public const int DEVICE_TYPE_NET_CAMERA = 132;

        /**
         * 报警输入设备编码
         */
        public const int DEVICE_TYPE_ALARM_IN = 134;

        /**
         * 报警输出设备编码
         */
        public const int DEVICE_TYPE_ALARM_OUT = 135;

        /**
         * 平台设备起始编码
         */
        public const int DEVICE_TYPE_PLAT_DEV_START = 200;

        /**
         * 平台设备结束编码
         */
        public const int DEVICE_TYPE_PLAT_DEV_END = 299;
        /**
 * 组织编码
 */
        public const int DEVICE_TYPE_ORG = 500;

        #endregion

        public static string LocalServiceStartUpMode = "1";

        public static string SmcIp
        {
            get;
            set;
        }

    }
}
