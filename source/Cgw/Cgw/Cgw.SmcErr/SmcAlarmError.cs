using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;

namespace Cgw.SmcError
{
    /// <summary>
    /// 告警错误信息
    /// </summary>
    [DataContract]
    public class SmcAlarm : SmcErr
    {
        public SmcAlarm()
        { 
        }

        public SmcAlarm(uint errNo)
            :base(errNo)
        {
        }

        #region VCT告警列表
        /// <summary>
        /// VCT启动初始化告警
        /// </summary>
        public const uint ALARM_VCT_INITIAL = 0x00000001;

        /// <summary>
        /// VCT重启Trap告警
        /// </summary>
        public const uint ALARM_VCT_RESTART = 0x00000002;

        /// <summary>
        /// VCT CPU异常告警
        /// </summary>
        public const uint ALARM_VCT_CPU = 0x00000003;

        /// <summary>
        /// VCT E1线路告警
        /// </summary>
        public const uint ALARM_VCT_E1_LINE = 0x00000004;

        /// <summary>
        /// VCT 丢包发生/恢复时发送
        /// </summary>
        public const uint ALARM_VCT_PACKET_LOSS = 0x00000005;

        /// <summary>
        /// VCT WEB认证失败发送
        /// </summary>
        public const uint ALARM_VCT_WEB_AUTHEN_FAIL = 0x00000006;

        /// <summary>
        /// VCT 开机/重启检测配置参数有误发送
        /// </summary>
        public const uint ALARM_VCT_CONFIGURATION_ERROR = 0x00000007;

        /// <summary>
        /// VCT 终端IP地址修改时发送
        /// </summary>
        public const uint ALARM_VCT_IP_CHANGE = 0x00000008;

        /// <summary>
        /// VCT 终端同步SERVER时间发送
        /// </summary>
        public const uint ALARM_VCT_SYNC_SERVER_TIME = 0x00000009;

        /// <summary>
        /// VCT 接上/断开CAMERA发送SNMP
        /// </summary>
        public const uint ALARM_VCT_CAMERA = 0x0000000a;

        /// <summary>
        /// 设备温度过高。
        /// </summary>
        public const uint ALARM_VCT_TEMPERATURE = 0x0000000b;

        /// <summary>
        /// 后台升级请求。
        /// </summary>
        public const uint ALARM_VCT_UPGRADE_REQUEST = 0x0000000c;

        /// <summary>
        /// 后台升级响应。
        /// </summary>
        public const uint ALARM_VCT_UPGRADE_RESPONSE = 0x0000000d;

        /// <summary>
        /// 接上或者断开MICROPHONE。
        /// </summary>
        public const uint ALARM_VCT_MICROPHONE = 0x0000000e;

        /// <summary>
        /// 终端发生网络抖动时。
        /// </summary>
        public const uint ALARM_VCT_NETWORK_SHAKE = 0x0000000f;

        /// <summary>
        /// GK/SIP状态变化。
        /// </summary>
        public const uint ALARM_VCT_GK_SIP_STATUS_CHANGE = 0x00000011;
        #endregion

        #region HDX告警列表
        /// <summary>
        /// 用户登陆失败
        /// </summary>
        public const uint ALARM_HDX_LOGIN_FAIL = 0x00100001;

        /// <summary>
        /// 呼叫失败
        /// </summary>
        public const uint ALARM_HDX_CALL_FAIL = 0x00100002;

        /// <summary>
        /// 视频连接失败
        /// </summary>
        public const uint ALARM_HDX_VISUAL_CONCERT_OFF = 0x00100003;

        /// <summary>
        /// 麦克风有问题
        /// </summary>
        public const uint ALARM_HDX_MIC_CHANGE = 0x00100004;

        /// <summary>
        /// 自动应答改变
        /// </summary>
        public const uint ALARM_HDX_AUTO_ANSWER_CHANGE = 0x00100005;

        /// <summary>
        /// 时间服务器连接不正确
        /// </summary>
        public const uint ALARM_HDX_TIME_SERVER_DOWN = 0x00100006;

        /// <summary>
        /// 时间服务器不可用
        /// </summary>
        public const uint ALARM_HDX_TIME_SERVER_OFF = 0x00100007;

        /// <summary>
        /// 目录服务器连接不正确
        /// </summary>
        public const uint ALARM_HDX_GDS_DOWN = 0x00100008;

        /// <summary>
        /// 目录服务器不可用
        /// </summary>
        public const uint ALARM_HDX_GDS_OFF = 0x00100009;

        /// <summary>
        /// GK未注册
        /// </summary>
        public const uint ALARM_HDX_GK_DOWN = 0x0010000a;

        /// <summary>
        /// ＧＫ不可用
        /// </summary>
        public const uint ALARM_HDX_GK_OFF = 0x0010000b;

        /// <summary>
        /// ＩＰ地址改变
        /// </summary>
        public const uint ALARM_HDX_IP_CHANGE = 0x0010000c;

        /// <summary>
        /// 网卡改变
        /// </summary>
        public const uint ALARM_HDX_INTERFACE_TYPE_CHANGE = 0x0010000d;

        /// <summary>
        /// ＩＳＤＮ线路不可用
        /// </summary>
        public const uint ALARM_HDX_LINE_DISABLED = 0x0010000e;

        /// <summary>
        /// ISDN功能有误
        /// </summary>
        public const uint ALARM_HDX_LINE_DOWN = 0x0010000f;

        /// <summary>
        /// V.35端口改变
        /// </summary>
        public const uint ALARM_HDX_V35_PORT_ENABLED = 0x00100010;

        /// <summary>
        /// ISDN线路改变
        /// </summary>
        public const uint ALARM_HDX_LINE_COUNT_CHANGE = 0x00100011;

        /// <summary>
        /// 主摄像头不能使用
        /// </summary>
        public const uint ALARM_HDX_MAIN_CAMERA_DOWN = 0x00100012;

        /// <summary>
        /// UPnP状态改变
        /// </summary>
        public const uint ALARM_HDX_UPNP_CHANGE = 0x00100013;

        /// <summary>
        /// 丢包率1分钟1%的告警
        /// </summary>
        public const uint ALARM_HDX_PACKET_LOSS = 0x00100014;

        /// <summary>
        /// 抖动一分钟35毫秒则发告警
        /// </summary>
        public const uint ALARM_HDX_JITT = 0x00100015;

        /// <summary>
        /// 反应时间1分钟150毫秒
        /// </summary>
        public const uint ALARM_HDX_LATENCY = 0x00100016;
        #endregion

        #region TDC告警列表
        /// <summary>
        /// TDC 呼叫失败
        /// </summary>
        public const uint ALARM_TDC_CALL_FAIL = 0x00200001;

        /// <summary>
        /// 自动应答改变
        /// </summary>
        public const uint ALARM_TDC_AUTOREPLY_CHANGE = 0x00200001;

        /// <summary>
        /// 时间服务器不能使用
        /// </summary>
        public const uint ALARM_TDC_NTPSERVER_UNABLE = 0x00200003;

        /// <summary>
        /// GK没有注册
        /// </summary>
        public const uint ALARM_TDC_GK_UNREGISTER = 0x00200004;

        /// <summary>
        /// GK不能使用
        /// </summary>
        public const uint ALARM_TDC_GK_UNABLE = 0x00200005;

        /// <summary>
        /// IP地址改变
        /// </summary>
        public const uint ALARM_TDC_IP_CHANGE = 0x00200006;

        /// <summary>
        /// SIP没有注册
        /// </summary>
        public const uint ALARM_TDC_SIP_UNREGISTER = 0x00200007;

        /// <summary>
        /// SIP不能使用
        /// </summary>
        public const uint ALARM_TDC_SIP_UNABLE = 0x00200008;
        #endregion    

        #region 我司MCU告警
        /// <summary>
        /// CPU通信自检错误
        /// </summary>
        public const uint ALARM_HW_MANAGE_MCU_CPU_ERROR = 0x00300000;

        /// <summary>
        /// IIC检测错误
        /// </summary>
        public const uint ALARM_HW_MANAGE_MCU_IIC_ERROR = 0x00300001;

        /// <summary>
        /// Flash检测错误
        /// </summary>
        public const uint ALARM_HW_MANAGE_MCU_FLASH_ERROR = 0x00300002;

        /// <summary>
        /// 底板逻辑状态检测错误
        /// </summary>
        public const uint ALARM_HW_MANAGE_MCU_MOTHERBOARD_LOGIC_ERROR = 0x00300003;

        /// <summary>
        /// 扣板0逻辑状态检测错误
        /// </summary>
        public const uint ALARM_HW_MANAGE_MCU_BUCKLE_0_LOGIC_ERROR = 0x00300004;

        /// <summary>
        /// 扣板1逻辑状态检测错误
        /// </summary>
        public const uint ALARM_HW_MANAGE_MCU_BUCKLE_1_LOGIC_ERROR = 0x00300005;

        /// <summary>
        /// 扣板2逻辑状态检测错误
        /// </summary>
        public const uint ALARM_HW_MANAGE_MCU_BUCKLE_2_LOGIC_ERROR = 0x00300006;

        /// <summary>
        /// 扣板3逻辑状态检测错误
        /// </summary>
        public const uint ALARM_HW_MANAGE_MCU_BUCKLE_3_LOGIC_ERROR = 0x00300007;

        /// <summary>
        /// XSI SDRAM 检测错误
        /// </summary>
        public const uint ALARM_HW_MANAGE_MCU_XSI_SDRAM_ERROR = 0x00300008;

        /// <summary>
        /// CPP SDRAM 检测错误
        /// </summary>
        public const uint ALARM_HW_MANAGE_MCU_CPP_SDRAM_ERROR = 0x00300009;

        /// <summary>
        /// CF card检测错误
        /// </summary>
        public const uint ALARM_HW_MANAGE_MCU_CF_CARD_ERROR = 0x0030000A;

        /// <summary>
        /// 电源0(左)检测错误
        /// </summary>
        public const uint ALARM_HW_MANAGE_MCU_POWER_0_LEFT_ERROR = 0x0030000B;

        /// <summary>
        /// 电源1(右)检测错误
        /// </summary>
        public const uint ALARM_HW_MANAGE_MCU_POWER_1_RIGHT_ERROR = 0x0030000C;

        /// <summary>
        /// 风扇检测错误
        /// </summary>
        public const uint ALARM_HW_MANAGE_MCU_FAN_ERROR = 0x0030000D;

        /// <summary>
        /// 底板的电压检测错误
        /// </summary>
        public const uint ALARM_HW_MANAGE_MCU_MOTHERBOARD_PRESSURE_ERROR = 0x0030000E;

        /// <summary>
        /// 扣板0的电压检测错误
        /// </summary>
        public const uint ALARM_HW_MANAGE_MCU_BUCKLE_0_PRESSURE_ERROR = 0x0030000F;

        /// <summary>
        /// 扣板1的电压检测错误
        /// </summary>
        public const uint ALARM_HW_MANAGE_MCU_BUCKLE_1_PRESSURE_ERROR = 0x00300016;

        /// <summary>
        /// 扣板2的电压检测错误
        /// </summary>
        public const uint ALARM_HW_MANAGE_MCU_BUCKLE_2_PRESSURE_ERROR = 0x00300017;

        /// <summary>
        /// 扣板3的电压检测错误
        /// </summary>
        public const uint ALARM_HW_MANAGE_MCU_BUCKLE_3_PRESSURE_ERROR = 0x00300018;

        /// <summary>
        /// NP扣板的电压检测错误
        /// </summary>
        public const uint ALARM_HW_MANAGE_MCU_NP_BUCKLE_PRESSURE_ERROR = 0x00300019;

        /// <summary>
        /// 配置初始化
        /// </summary>
        public const uint ALARM_HW_MANAGE_MCU_CONFIGURE_INITIAL_ERROR = 0x00300020;

        /// <summary>
        /// license错误告警
        /// </summary>
        public const uint ALARM_HW_MANAGE_MCU_LICENSE_ERROR = 0x00300021;

        /// <summary>
        /// 槽位号和电子标签单板类型信息匹配结果
        /// </summary>
        public const uint ALARM_HW_MANAGE_MCU_TROUGH_TAG_MATCH_ERROR = 0x00300022;

        /// <summary>
        /// 主控板与业务板握手超时
        /// </summary>
        public const uint ALARM_HW_MANAGE_MCU_MAINCONTROL_BUSINESSCONTROL_HANDSHAKE_OVERTIME_ERROR = 0x00300023;

        /// <summary>
        /// 软件版本不匹配告警
        /// </summary>
        public const uint ALARM_HW_MANAGE_MCU_SOFTWARE_VERSION_MATCH_ERROR = 0x00300024;

        /// <summary>
        /// 主控板下发配置失败
        /// </summary>
        public const uint ALARM_HW_MANAGE_MCU_MAINCONTROL_SEND_CONFIGURE_ERROR = 0x00300025;

        /// <summary>
        /// 主板与备板的license不一致告警
        /// </summary>
        public const uint ALARM_HW_MANAGE_MCU_MAINCONTROL_BACKUPCONTROL_LICENSE_MATCH_ERROR = 0x00300026;

        /// <summary>
        /// E1的IP自检结果
        /// </summary>
        public const uint ALARM_HW_MANAGE_MCU_E1_IP_ERROR = 0x00300027;

        /// <summary>
        /// 实时时钟获取错误
        /// </summary>
        public const uint ALARM_HW_MANAGE_MCU_REAL_TIME_ERROR = 0x00300028;

        /// <summary>
        /// 单板插入不正确，与主控板通信失败
        /// </summary>
        public const uint ALARM_HW_MANAGE_MCU_VENEER_ERROR = 0x00300029;

        /// <summary>
        /// 备板的ESN错误
        /// </summary>
        public const uint ALARM_HW_MANAGE_MCU_BACKUPCONTROL_ESN_ERROR = 0x00300030;

        /// <summary>
        /// 扣板0 DSP设备检测错误
        /// </summary>
        public const uint ALARM_HW_MANAGE_MCU_BUCKLE_0_DSP_ERROR = 0x00300031;

        /// <summary>
        /// 扣板1 DSP设备检测错误
        /// </summary>
        public const uint ALARM_HW_MANAGE_MCU_BUCKLE_1_DSP_ERROR = 0x00300032;

        /// <summary>
        /// 扣板2 DSP设备检测错误
        /// </summary>
        public const uint ALARM_HW_MANAGE_MCU_BUCKLE_2_DSP_ERROR = 0x00300033;

        /// <summary>
        /// 扣板3 DSP设备检测错误
        /// </summary>
        public const uint ALARM_HW_MANAGE_MCU_BUCKLE_3_DSP_ERROR = 0x00300034;

        /// <summary>
        /// Tsi568a配置失败
        /// </summary>
        public const uint ALARM_HW_MANAGE_MCU_TSI568A_ERROR = 0x00300035;

        /// <summary>
        /// 单板在位跟SWITCH连接状态不一致
        /// </summary>
        public const uint ALARM_HW_MANAGE_MCU_SWITCH_CONNECT_ERROR = 0x00300036;

        /// <summary>
        /// 底板的温度大于75度
        /// </summary>
        public const uint ALARM_HW_MANAGE_MCU_MOTHERBOARD_TEMPERATURE_OVER75_ERROR = 0x00300037;

        /// <summary>
        /// 扣板0温度大于75度
        /// </summary>
        public const uint ALARM_HW_MANAGE_MCU_BUCKLE_0_TEMPERATURE_OVER75_ERROR = 0x00300038;

        /// <summary>
        /// 扣板1温度大于75度
        /// </summary>
        public const uint ALARM_HW_MANAGE_MCU_BUCKLE_1_TEMPERATURE_OVER75_ERROR = 0x00300039;

        /// <summary>
        /// 扣板2温度大于75度
        /// </summary>
        public const uint ALARM_HW_MANAGE_MCU_BUCKLE_2_TEMPERATURE_OVER75_ERROR = 0x00300040;

        /// <summary>
        /// 扣板3温度大于75度
        /// </summary>
        public const uint ALARM_HW_MANAGE_MCU_BUCKLE_3_TEMPERATURE_OVER75_ERROR = 0x00300041;

        /// <summary>
        /// NP扣板温度大于75度
        /// </summary>
        public const uint ALARM_HW_MANAGE_MCU_NP_BUCKLE_TEMPERATURE_OVER75_ERROR = 0x00300042;

        /// <summary>
        /// 底板的温度小于0度
        /// </summary>
        public const uint ALARM_HW_MANAGE_MCU_MOTHERBOARD_TEMPERATURE_LESS0_ERROR = 0x00300043;

        /// <summary>
        /// 扣板0温度小于0度
        /// </summary>
        public const uint ALARM_HW_MANAGE_MCU_BUCKLE_0_TEMPERATURE_LESS0_ERROR = 0x00300044;

        /// <summary>
        /// 扣板1温度小于0度
        /// </summary>
        public const uint ALARM_HW_MANAGE_MCU_BUCKLE_1_TEMPERATURE_LESS0_ERROR = 0x00300045;

        /// <summary>
        /// 扣板2温度小于0度
        /// </summary>
        public const uint ALARM_HW_MANAGE_MCU_BUCKLE_2_TEMPERATURE_LESS0_ERROR = 0x00300046;

        /// <summary>
        /// 扣板3温度小于0度
        /// </summary>
        public const uint ALARM_HW_MANAGE_MCU_BUCKLE_3_TEMPERATURE_LESS0_ERROR = 0x00300047;

        /// <summary>
        /// NP扣板温度小于0度
        /// </summary>
        public const uint ALARM_HW_MANAGE_MCU_NP_BUCKLE_TEMPERATURE_LESS0_ERROR = 0x00300048;

        /// <summary>
        /// 跨板SRIO流量超标
        /// </summary>
        public const uint ALARM_HW_MANAGE_MCU_SRIO_ERROR = 0x00300049;

        /// <summary>
        /// 扣板0未插好
        /// </summary>
        public const uint ALARM_HW_MANAGE_MCU_BUCKLE_0_INSERT_ERROR = 0x00300050;

        /// <summary>
        /// 扣板1未插好
        /// </summary>
        public const uint ALARM_HW_MANAGE_MCU_BUCKLE_1_INSERT_ERROR = 0x00300051;

        /// <summary>
        /// 扣板2未插好
        /// </summary>
        public const uint ALARM_HW_MANAGE_MCU_BUCKLE_2_INSERT_ERROR = 0x00300052;

        /// <summary>
        /// 扣板3未插好
        /// </summary>
        public const uint ALARM_HW_MANAGE_MCU_BUCKLE_3_INSERT_ERROR = 0x00300053;

        /// <summary>
        /// 扣板0的CPLD标志错误
        /// </summary>
        public const uint ALARM_HW_MANAGE_MCU_BUCKLE_0_CPLD_ERROR = 0x00300054;

        /// <summary>
        /// 扣板1的CPLD标志错误
        /// </summary>
        public const uint ALARM_HW_MANAGE_MCU_BUCKLE_1_CPLD_ERROR = 0x00300055;

        /// <summary>
        /// 扣板2的CPLD标志错误
        /// </summary>
        public const uint ALARM_HW_MANAGE_MCU_BUCKLE_2_CPLD_ERROR = 0x00300056;

        /// <summary>
        /// 扣板3的CPLD标志错误
        /// </summary>
        public const uint ALARM_HW_MANAGE_MCU_BUCKLE_3_CPLD_ERROR = 0x00300057;

        /// <summary>
        /// 扣板0的50M失败
        /// </summary>
        public const uint ALARM_HW_MANAGE_MCU_BUCKLE_0_50M_ERROR = 0x00300058;

        /// <summary>
        /// 扣板1的50M失败
        /// </summary>
        public const uint ALARM_HW_MANAGE_MCU_BUCKLE_1_50M_ERROR = 0x00300059;

        /// <summary>
        /// 扣板2的50M失败
        /// </summary>
        public const uint ALARM_HW_MANAGE_MCU_BUCKLE_2_50M_ERROR = 0x00300060;

        /// <summary>
        /// 扣板3的50M失败
        /// </summary>
        public const uint ALARM_HW_MANAGE_MCU_BUCKLE_3_50M_ERROR = 0x00300061;

        /// <summary>
        /// 网络被冲击
        /// </summary>
        public const uint ALARM_HW_MANAGE_MCU_NETWORK_STORM_ERROR = 0x00300062;

        /// <summary>
        /// GE0 ip 冲突
        /// </summary>
        public const uint ALARM_HW_MANAGE_MCU_GE0_IP_CONFLICT_ERROR = 0x00300063;

        /// <summary>
        /// GE1 ip 冲突
        /// </summary>
        public const uint ALARM_HW_MANAGE_MCU_GE1_IP_CONFLICT_ERROR = 0x00300064;

        /// <summary>
        /// GE0 未接网线
        /// </summary>
        public const uint ALARM_HW_MANAGE_MCU_GE0_DISCONNECT_ERROR = 0x00300065;

        /// <summary>
        /// GE1 未接网线
        /// </summary>
        public const uint ALARM_HW_MANAGE_MCU_GE1_DISCONNECT_ERROR = 0x00300066;

        /// <summary>
        /// 分配HDPD扣板失败
        /// </summary>
        public const uint ALARM_HW_MANAGE_MCU_HDPD_CONTROL_ERROR = 0x00300067;

        /// <summary>
        /// DSP加载失败
        /// </summary>
        public const uint ALARM_HW_MANAGE_MCU_DSP_LOAD_ERROR = 0x00300068;

        /// <summary>
        /// 下风扇检测错误
        /// </summary>
        public const uint ALARM_HW_MANAGE_MCU_BELOW_FAN_ERROR = 0x00300069;

        /// <summary>
        /// FE0 & GE 网段冲突
        /// </summary>
        public const uint ALARM_HW_MANAGE_MCU_FE0_GE_ERROR = 0x00300070;

        /// <summary>
        /// 音频资源不足
        /// </summary>
        public const uint ALARM_HW_MANAGE_MCU_AUDIO_LACK_ERROR = 0x00300071;

        /// <summary>
        /// 视频资源不足
        /// </summary>
        public const uint ALARM_HW_MANAGE_MCU_VIDEO_LACK_ERROR = 0x00300072;

        /// <summary>
        /// IP资源不足
        /// </summary>
        public const uint ALARM_HW_MANAGE_MCU_IP_RESOURCE_LACK_ERROR = 0x0030004A;

        /// <summary>
        /// CF卡或FLASH不在线
        /// </summary>
        public const uint ALARM_HW_MANAGE_MCU_CF_FLASH_ERROR = 0x00300075;

        /// <summary>
        /// CF卡IVR目录下无文件
        /// </summary>
        public const uint ALARM_HW_MANAGE_MCU_CF_IVR_ERROR = 0x00300076;

        
        #endregion

        #region 会议告警列表
        /// <summary>
        /// MCU不在线
        /// </summary>
        public const uint ALARM_CONF_MCU_NOT_ONLINE_ERROR = 0x00400001;

        /// <summary>
        /// MCU资源不足
        /// </summary>
        public const uint ALARM_CONF_MCU_RESOURCE_LACK_ERROR = 0x00400002;

        /// <summary>
        /// 级联会场呼叫失败
        /// </summary>
        public const uint ALARM_CONF_SITE_CALL_FAIL = 0x00400003;

        /// <summary>
        /// 级联会场离线。
        /// </summary>
        public const uint ALARM_CONF_SITE_NOT_ONLINE = 0x00400004;

        /// <summary>
        /// 会议召开失败。
        /// </summary>
        public const uint ALARM_CONF_START_FAIL = 0x00400005;
        #endregion

        #region GK 告警列表
        /// <summary>
        /// 主备倒换告警
        /// </summary>
        public const uint ALARM_GK_MAIN_BACK_CHANGE_ERROR = 0x00500001;

        /// <summary>
        /// GK与GKM连接断开告警
        /// </summary>
        public const uint ALARM_GK_GKM_CONNECT_ERROR = 0x00500002;

        /// <summary>
        /// CPU使用率过载告警。
        /// </summary>
        public const uint ALARM_GK_CPU_OVERLOAD_ERROR = 0x00500003;

        /// <summary>
        /// 超过设定内存使用率。
        /// </summary>
        public const uint ALARM_GK_MEMORY_OVERLOAD_ERROR = 0x00500004;

        /// <summary>
        ///在线节点数过载。
        /// </summary>
        public const uint ALARM_GK_ONLINE_COUNT_OVERLOAD_ERROR = 0x00500005;

        /// <summary>
        /// 呼叫数过载告警
        /// </summary>
        public const uint ALARM_GK_CALL_COUNT_OVERLOAD_ERROR = 0x00500006;

        /// <summary>
        /// 主备Gk连接告警
        /// </summary>
        public const uint ALARM_GK_MAIN_BACK_CONN_ERROR = 0x00500007;

        /// <summary>
        /// 邻居GK响应告警
        /// </summary>
        public const uint ALARM_GK_LABORGK_REQ_ERROR = 0x00500008;

        /// <summary>
        /// GK超出资源告警
        /// </summary>
        public const uint ALARM_GK_OUTOF_RES_ERROR = 0x00500009;

        /// <summary>
        /// GK网段带宽限制
        /// </summary>
        public const uint ALARM_GK_SUBNETBANDWIDTHOVERFLOW_ERROR = 0x0050000A;

        /// <summary>
        /// 硬盘剩余空间不足
        /// </summary>
        public const uint ALARM_GK_DISKFREEOVERLOAD_ERROR = 0x0050000B;

        /// <summary>
        /// 计费认证服务响应超时
        /// </summary>
        public const uint ALARM_GK_RADIUS_AUTH_REQ_TIMEOUT_ERROR = 0x0050000C;

        /// <summary>
        /// 计费服务响应超时
        /// </summary>
        public const uint ALARM_GK_RADIUS_ACCT_REQ_TIMEOUT_ERROR = 0x0050000D;

        #endregion

        #region 不可管理终端
        /// <summary>
        /// GK注册失败。
        /// </summary>
        public const uint ALARM_UNMANAGE_SITE_GK_REGISTER_FAIL = 0x00600001;
        #endregion

        #region 不可管理MCU
        /// <summary>
        /// GK注册失败。
        /// </summary>
        public const uint ALARM_UNMANAGE_MCU_GK_REGISTER_FAIL = 0x00700001;

        /// <summary>
        /// IVR注册失败。
        /// </summary>
        public const uint ALARM_UNMANAGE_MCU_IVR_REGISTER_FAIL = 0x00700002;

        /// <summary>
        /// 连接失败。
        /// </summary>
        public const uint ALARM_UNMANAGE_MCU_CONNECT_FAIL = 0x00700003;
        #endregion

        #region 预发放终端 PreDefined 0x00800000 开始
        /// <summary>
        /// 预发放告警 开始段 0x00800000
        /// </summary>
        public const uint ALARM_PREDEFINED_SITE_BASE = 0x00800000;

        /// <summary>
        /// 类型不匹配。
        /// </summary>
        public const uint ALARM_PREDEFINED_SITE_TYPE_NOT_MATCH = ALARM_PREDEFINED_SITE_BASE + 0x01;

        /// <summary>
        /// 配置参数失败。
        /// </summary>
        public const uint ALARM_PREDEFINED_SITE_CONFIGURE_FAIL = ALARM_PREDEFINED_SITE_BASE + 0x02;

        /// <summary>
        /// 读取配置失败。
        /// </summary>
        public const uint ALARM_PREDEFINED_SITE_SNMP_FAIL = ALARM_PREDEFINED_SITE_BASE + 0x03;
        #endregion

        #region SMC业务告警
        /// <summary>
        /// SMC业务告警起始段0x00900000
        /// </summary>
        public const uint ALARM_SMC_BUSINESS_BASE = 0x00900000;
        /// <summary>
        /// 升级成功后，没有重启。
        /// </summary>
        public const uint ALARM_DEVICE_BUSINESS_NO_RESTART = 0x00900001;

        /// <summary>
        /// 同步配置项失败。
        /// </summary>
        public const uint ALARM_DEVICE_BUSINESS_CONFIGURE_SYNC_FAIL = 0x00900002;

        /// <summary>
        /// GK注册失败。
        /// </summary>
        public const uint ALARM_DEVICE_BUSINESS_CONFIGURE_GK_REGISTER_FAIL = 0x00900003;

        /// <summary>
        /// SIP注册失败。
        /// </summary>
        public const uint ALARM_DEVICE_BUSINESS_CONFIGURE_SIP_REGISTER_FAIL = 0x00900004;

         /// <summary>
        /// 设备IP地址不可用
        /// </summary>
        public const uint ALARM_IPADDRESS_NOT_AVAILABLE = 0x00900005;

        /// <summary>
        /// 用户选择设备类型与实际设备类型不匹配
        /// </summary>
        public const uint ALARM_DEVICE_TYPE_NOMATCH = 0x00900006;

        /// <summary>
        /// 控制台类型不匹配
        /// </summary>
        public const uint ALARM_BOARD_TYPE_NOMATCH = 0x00900007;

        /// <summary>
        /// 设备已经被其他系统连接
        /// </summary>
        public const uint ALARM_CONNECT_MAX_NUM = 0x00900008;

        /// <summary>
        /// 密码认证失败
        /// </summary>
        public const uint ALARM_DEVICE_AUTHER_ERROR = 0x00900009;

        /// <summary>
        /// 设备{0}{devicename} sip标识与设备实际的标识不一致
        /// </summary>
        public const uint ALARM_DEVICE_SIP_ERROR = 0x0090000A;

        /// <summary>
        /// 设备{0}{devicename} E164标识与设备实际的标识不一致
        /// </summary>
        public const uint ALARM_DEVICE_E164_ERROR = 0x0090000B;

        /// <summary>
        /// 设备{0}{devicename} H323ID标识与设备实际的标识不一致
        /// </summary>
        public const uint ALARM_DEVICE_H323ID_ERROR = 0x0090000C;

        /// <summary>
        /// MAC地址发生变化
        /// </summary>
        public const uint ALARM_DEVICE_MAC_CHANGED = 0x0090000D;

        /// <summary>
        /// POLYCOM,VCT/TEX0 snmp 连接失败告警码
        /// </summary>
        public const uint ALARM_DEVICE_SNMP_CONNECT_ERROR = 0x0090000E;

        /// <summary>
        /// 备份配置失败
        /// </summary>
        public const uint ALARM_DEVICE_BACKUP_FAIL = 0x0090000F;

        /// <summary>
        /// GK跟设备上报的GK状态不一致
        /// </summary>
        public const uint ALARM_DEVICE_GKM_DEVICEREPORT_GK = 0x00900010;

        /// <summary>
        /// MAC地址冲突
        /// </summary>
        public const uint ALARM_DEVICE_MAC_CONFILCT = 0x00900011;

        /// <summary>
        /// 设备{0}{devicename} sip左标识与设备实际的标识不一致
        /// </summary>
        public const uint ALARM_DEVICE_SIP_L_ERROR = 0x00900012;

        /// <summary>
        /// 设备{0}{devicename} sip右标识与设备实际的标识不一致
        /// </summary>
        public const uint ALARM_DEVICE_SIP_R_ERROR = 0x00900013;

        /// <summary>
        /// 设备{0}{devicename} E164左标识与设备实际的标识不一致
        /// </summary>
        public const uint ALARM_DEVICE_E164_L_ERROR = 0x00900014;

        /// <summary>
        /// 设备{0}{devicename} E164右标识与设备实际的标识不一致
        /// </summary>
        public const uint ALARM_DEVICE_E164_R_ERROR = 0x00900015;

        /// <summary>
        /// 设备{0}{devicename} H323ID左标识与设备实际的标识不一致
        /// </summary>
        public const uint ALARM_DEVICE_H323ID_L_ERROR = 0x00900016;

        /// <summary>
        /// 设备{0}{devicename} H323ID右标识与设备实际的标识不一致
        /// </summary>
        public const uint ALARM_DEVICE_H323ID_R_ERROR = 0x00900017;

        /// <summary>
        /// ISDN资源超出阀值
        /// </summary>
        public const uint ISDN_RESOURCE_OVERFLOW_ALARM = 0x00900018;

        /// <summary>
        /// IP资源超出阀值
        /// </summary>
        public const uint IP_RESOURCE_OVERFLOW_ALARM = 0x00900019;

        /// <summary>
        /// 多画面编码资源超出阀值
        /// </summary>
        public const uint CPENCODE_RESOURCE_OVERFLOW_ALARM = 0x0090001A;

        /// <summary>
        /// 多画面解码资源超出阀值
        /// </summary>
        public const uint CPDECODE_RESOURCE_OVERFLOW_ALARM = 0x0090001B;

        /// <summary>
        /// 速率适配资源超出阀值
        /// </summary>
        public const uint RATE_RESOURCE_OVERFLOW_ALARM = 0x0090001C;

        /// <summary>
        /// 音频资源超出阀值
        /// </summary>
        public const uint AUDIO_RESOURCE_OVERFLOW_ALARM = 0x0090001D;

        /// <summary>
        /// 监控资源超出阀值
        /// </summary>
        public const uint MONITER_RESOURCE_OVERFLOW_ALARM = 0x0090001E;

        /// <summary>
        /// 加密资源超出阀值
        /// </summary>
        public const uint ENCRY_RESOURCE_OVERFLOW_ALARM = 0x0090001F;

        /// <summary>
        /// 端口资源超出阀值
        /// </summary>
        public const uint PORT_RESOURCE_OVERFLOW_ALARM = 0x00900020;

        /// <summary>
        /// HTTP登陆失败告警码
        /// </summary>
        public const uint ALARM_HW_MANAGE_MCU_HTTP = 0x00900021;

        /// <summary>
        ///设备恢复配置失败
        /// </summary>
        public const uint ALARM_DEVICE_RESTORE_FAILED = 0x00900022;

        /// <summary>
        /// SNMP参数错误
        /// </summary>
        public const uint ALARM_DEVICE_SNMP_PARAM_ERROR = ALARM_SMC_BUSINESS_BASE + 0x23;
        #endregion
        #region Nlog告警
        /// <summary>
        /// 音频接收丢包超过告警阀值
        /// </summary>
        public const uint NLOG_ALARM_AUDIO_REV = 0x00A00000;

        /// <summary>
        /// 音频发送丢包超过告警阀值
        /// </summary>
        public const uint NLOG_ALARM_AUDIO_SEND = 0x00A00001;

        /// <summary>
        /// 视频接收丢包超过告警阀值
        /// </summary>
 
        public const uint NLOG_ALARM_VIDEO_REV = 0x00A00002;

        /// <summary>
        /// 视频发送丢包超过告警阀值
        /// </summary>
        public const uint NLOG_ALARM_VIDEO_SEND = 0x00A00003;

        /// <summary>
        /// 辅流接收丢包超过告警阀值
        /// </summary> 
        public const uint NLOG_ALARM_AMCVIDEO_REV = 0x00A00004;

        /// <summary>
        /// 辅流发送丢包超过告警阀值
        /// </summary>
        public const uint NLOG_ALARM_AMCVIDEO_SEND = 0x00A00005;

        /// <summary>
        /// CRC
        /// </summary>
        public const uint NLOG_ALARM_CRC = 0x00A00006;

        /// <summary>
        /// 网络延时超过告警阀值
        /// </summary>
        public const uint NLOG_ALARM_NETDELAY = 0x00A00007;

        /// <summary>
        /// 抖动接收丢包超过告警阀值
        /// </summary> 
        public const uint NLOG_ALARM_JITTERE_REV = 0x00A00008;

        /// <summary>
        /// 抖动发送丢包超过告警阀值
        /// </summary> 
        public const uint NLOG_ALARM_JITTERE_SEND = 0x00A00009;

        /// <summary>
        /// I帧发送丢包超过告警阀值
        /// </summary>
        public const uint NLOG_ALARM_IFRAME_SEND = 0x00A0000A;

        /// <summary>
        /// I帧接受丢包超过告警阀值
        /// </summary>
        public const uint NLOG_ALARM_IFRAME_REV = 0x00A0000B;

        /// <summary>
        /// 视频发送方向连续丢包数超过阀值
        /// </summary>
        public const uint NLOG_ALARM_VDNUM_SEND = 0x00A0000C;

        /// <summary>
        /// 视频接收方向连续丢包数超过阀值
        /// </summary>
        public const uint NLOG_ALARM_VDNUM_REV = 0x00A0000D;
        #endregion
        #region TP告警列表
        #region 中控机
        /// <summary>
        /// 外网冲突(0:正常；1：冲突)
        /// </summary>
        public const uint ALARM_TP_MAINHOST_inetipconflict = 0x00B00001;

        /// <summary>
        /// 内网冲突(0:正常；1：冲突)
        /// </summary>
        public const uint ALARM_TP_MAINHOST_lanipconflict = 0x00B00002;

        /// <summary>
        /// 外网连接状态(0：正常；1：断开)
        /// </summary>
        public const uint ALARM_TP_MAINHOST_inetdisconnect = 0x00B00003;

        /// <summary>
        /// 内网连接状态(0：正常；1：断开)
        /// </summary>
        public const uint ALARM_TP_MAINHOST_landisconnect = 0x00B00004;

        /// <summary>
        /// 风扇状态(0：正常；1：堵转)
        /// </summary>
        public const uint ALARM_TP_MAINHOST_hostfanstate = 0x00B00005;

        /// <summary>
        /// POE网口(终端和主机POE网址不在同一网段则告警)
        /// </summary>
        public const uint ALARM_TP_MAINHOST_poenetwarning = 0x00B00006;

        /// <summary>
        /// 温度(0：正常；1：过高)
        /// </summary>
        public const uint ALARM_TP_MAINHOST_hosttemper = 0x00B00007;

        /// <summary>
        /// 视频处理板(0：正常；1：异常)
        /// </summary>
        public const uint ALARM_TP_MAINHOST_vivoboardwarn = 0x00B00008;

        /// <summary>
        /// 输入DVI连线状态(只在端口已配置并且DVI线拔出时告警)
        /// </summary>
        public const uint ALARM_TP_MAINHOST_hostdviconnected = 0x00B00009;

        /* 命名有冲突，后续补充
        /// <summary>
        /// 输出DVI连线状态(只在端口已配置并且DVI线拔出时告警)
        /// </summary>
        public const uint ALARM_TP_MAINHOST_hostdviconnected,*/

        /// <summary>
        /// 视频分辨率不一致(只在视频分辨率格式不一致时告警)
        /// </summary>
        public const uint ALARM_TP_MAINHOST_hostvideoformatwarn = 0x00B0000A;

        /// <summary>
        /// 音频6748系统状态(0：DSP异常；1：MIC数据同步异常；2：DSP重启成功)
        /// </summary>
        public const uint ALARM_TP_MAINHOST_dsp6748state = 0x00B0000B;

        /// <summary>
        /// 音频6455系统状态(0：断开；1：连接)
        /// </summary>
        public const uint ALARM_TP_MAINHOST_dsp6455state = 0x00B0000C;

        /// <summary>
        /// POE网口状态(0：异常；1：正常)
        /// </summary>
        public const uint ALARM_TP_MAINHOST_poestate = 0x00B0000D;

        /// <summary>
        /// 音频处理板不满足规格(0：正常；1：异常)
        /// </summary>
        public const uint ALARM_TP_MAINHOST_apboardwarn = 0x00B0000E;

        /// <summary>
        /// 音频接口板不满足规格(0：正常；1：异常)
        /// </summary>
        public const uint ALARM_TP_MAINHOST_aiboardwarn = 0x00B0000F;
        #endregion

        #region 左终端
        /// <summary>
        /// 主机的辅流输出和主终端检测到的视频分辨率格式不一致(0:正常; 1:异常)
        /// </summary>
        public const uint ALARM_TP_LEFTTERM_assistcodecvideoformatwarn = 0x00B00010;

        /// <summary>
        /// 业务网络连接状态(0:连接; 1:断开)
        /// </summary>
        public const uint ALARM_TP_LEFTTERM_callipdisconnected = 0x00B00011;

        /// <summary>
        /// 版本不匹配(0:正常; 1:异常)
        /// </summary>
        public const uint ALARM_TP_LEFTTERM_codecverwarning = 0x00B00012;

        /// <summary>
        /// 从CPU死机(0: SOFTWARE_SLAVE；2：SOFTWARE_DM6421；3：SOFTWARE_DM6422)
        /// </summary>
        public const uint ALARM_TP_LEFTTERM_cpudownid = 0x00B00013;

        /// <summary>
        /// 终端启动错误(0: 无错误; 1:主板CPU错误; 2:DSPA上CPU1错误; 3:DSPA上CPU2错误; 4:DSPB上CPU1错误; 5:DSPB上CPU2错误)
        /// </summary>
        public const uint ALARM_TP_LEFTTERM_decoderloadfailid = 0x00B00014;

        /// <summary>
        /// 风扇状态(0:正常; 1:异常)
        /// </summary>
        public const uint ALARM_TP_LEFTTERM_fanstate = 0x00B00015;

        /// <summary>
        /// 终端IP地址冲突(0: 正常; 1:冲突)
        /// </summary>
        public const uint ALARM_TP_LEFTTERM_iptype = 0x00B00016;

        /// <summary>
        /// 主机的视频输出和终端收到的视频分辨率格式不一致(0: 正常; 1:异常)
        /// </summary>
        public const uint ALARM_TP_LEFTTERM_maincodecvideoformatwarn = 0x00B00017;

        /// <summary>
        /// 器件异常(0: 正常; 1:异常)
        /// </summary>
        public const uint ALARM_TP_LEFTTERM_sysapparatus = 0x00B00018;

        /// <summary>
        /// 终端telnet连接状态(0: 连接; 1:断开)
        /// </summary>
        public const uint ALARM_TP_LEFTTERM_telconnected = 0x00B00019;

        /// <summary>
        /// 温度(0: 正常; 1:过高)
        /// </summary>
        public const uint ALARM_TP_LEFTTERM_temperature = 0x00B0001A;

        /// <summary>
        /// 4E1状态(0: 正常; 1:异常)
        /// </summary>
        public const uint ALARM_TP_LEFTTERM_4e1warn = 0x00B0001B;

        /// <summary>
        /// 4E1连线(0: 正常; 1:异常)
        /// </summary>
        public const uint ALARM_TP_LEFTTERM_4e1linewarn = 0x00B0001C;
        #endregion

        #region 主终端
        /// <summary>
        /// 主机的辅流输出和主终端检测到的视频分辨率格式不一致(0:正常; 1:异常)
        /// </summary>
        public const uint ALARM_TP_CENTERTERM_assistcodecvideoformatwarn = 0x00B00020;

        /// <summary>
        /// 业务网络连接状态(0:连接; 1:断开)
        /// </summary>
        public const uint ALARM_TP_CENTERTERM_callipdisconnected = 0x00B00021;

        /// <summary>
        /// 版本不匹配(0:正常; 1:异常)
        /// </summary>
        public const uint ALARM_TP_CENTERTERM_codecverwarning = 0x00B00022;

        /// <summary>
        /// 从CPU死机(0: SOFTWARE_SLAVE；2：SOFTWARE_DM6421；3：SOFTWARE_DM6422)
        /// </summary>
        public const uint ALARM_TP_CENTERTERM_cpudownid = 0x00B00023;

        /// <summary>
        /// 终端启动错误(0: 无错误; 1:主板CPU错误; 2:DSPA上CPU1错误; 3:DSPA上CPU2错误; 4:DSPB上CPU1错误; 5:DSPB上CPU2错误)
        /// </summary>
        public const uint ALARM_TP_CENTERTERM_decoderloadfailid = 0x00B00024;

        /// <summary>
        /// 风扇状态(0:正常; 1:异常)
        /// </summary>
        public const uint ALARM_TP_CENTERTERM_fanstate = 0x00B00025;

        /// <summary>
        /// 终端IP地址冲突(0: 正常; 1:冲突)
        /// </summary>
        public const uint ALARM_TP_CENTERTERM_iptype = 0x00B00026;

        /// <summary>
        /// 主机的视频输出和终端收到的视频分辨率格式不一致(0: 正常; 1:异常)
        /// </summary>
        public const uint ALARM_TP_CENTERTERM_maincodecvideoformatwarn = 0x00B00027;

        /// <summary>
        /// 器件异常(0: 正常; 1:异常)
        /// </summary>
        public const uint ALARM_TP_CENTERTERM_sysapparatus = 0x00B00028;

        /// <summary>
        /// 终端telnet连接状态(0: 连接; 1:断开)
        /// </summary>
        public const uint ALARM_TP_CENTERTERM_telconnected = 0x00B00029;

        /// <summary>
        /// 温度(0: 正常; 1:过高)
        /// </summary>
        public const uint ALARM_TP_CENTERTERM_temperature = 0x00B0002A;

        /// <summary>
        /// 4E1状态(0: 正常; 1:异常)
        /// </summary>
        public const uint ALARM_TP_CENTERTERM_4e1warn = 0x00B0002B;

        /// <summary>
        /// 4E1连线(0: 正常; 1:异常)
        /// </summary>
        public const uint ALARM_TP_CENTERTERM_4e1linewarn = 0x00B0002C;
        #endregion

        #region 右终端
        /// <summary>
        /// 主机的辅流输出和主终端检测到的视频分辨率格式不一致(0:正常; 1:异常)
        /// </summary>
        public const uint ALARM_TP_RIGHTTERM_assistcodecvideoformatwarn = 0x00B00030;

        /// <summary>
        /// 业务网络连接状态(0:连接; 1:断开)
        /// </summary>
        public const uint ALARM_TP_RIGHTTERM_callipdisconnected = 0x00B00031;

        /// <summary>
        /// 版本不匹配(0:正常; 1:异常)
        /// </summary>
        public const uint ALARM_TP_RIGHTTERM_codecverwarning = 0x00B00032;

        /// <summary>
        /// 从CPU死机(0: SOFTWARE_SLAVE；2：SOFTWARE_DM6421；3：SOFTWARE_DM6422)
        /// </summary>
        public const uint ALARM_TP_RIGHTTERM_cpudownid = 0x00B00033;

        /// <summary>
        /// 终端启动错误(0: 无错误; 1:主板CPU错误; 2:DSPA上CPU1错误; 3:DSPA上CPU2错误; 4:DSPB上CPU1错误; 5:DSPB上CPU2错误)
        /// </summary>
        public const uint ALARM_TP_RIGHTTERM_decoderloadfailid = 0x00B00034;

        /// <summary>
        /// 风扇状态(0:正常; 1:异常)
        /// </summary>
        public const uint ALARM_TP_RIGHTTERM_fanstate = 0x00B00035;

        /// <summary>
        /// 终端IP地址冲突(0: 正常; 1:冲突)
        /// </summary>
        public const uint ALARM_TP_RIGHTTERM_iptype = 0x00B00036;

        /// <summary>
        /// 主机的视频输出和终端收到的视频分辨率格式不一致(0: 正常; 1:异常)
        /// </summary>
        public const uint ALARM_TP_RIGHTTERM_maincodecvideoformatwarn = 0x00B00037;

        /// <summary>
        /// 器件异常(0: 正常; 1:异常)
        /// </summary>
        public const uint ALARM_TP_RIGHTTERM_sysapparatus = 0x00B00038;

        /// <summary>
        /// 终端telnet连接状态(0: 连接; 1:断开)
        /// </summary>
        public const uint ALARM_TP_RIGHTTERM_telconnected = 0x00B00039;

        /// <summary>
        /// 温度(0: 正常; 1:过高)
        /// </summary>
        public const uint ALARM_TP_RIGHTTERM_temperature = 0x00B0003A;

        /// <summary>
        /// 4E1状态(0: 正常; 1:异常)
        /// </summary>
        public const uint ALARM_TP_RIGHTTERM_4e1warn = 0x00B0003B;

        /// <summary>
        /// 4E1连线(0: 正常; 1:异常)
        /// </summary>
        public const uint ALARM_TP_RIGHTTERM_4e1linewarn = 0x00B0003C;
        #endregion

        #region 触摸屏
        /// <summary>
        /// 触摸屏连接状态(0:正常; 1:异常)
        /// </summary>
        public const uint ALARM_TP_HID_hidconnected = 0x00B00040;

        /// <summary>
        /// 版本不匹配(0:正常; 1:异常)
        /// </summary>
        public const uint ALARM_TP_HID_hidverwarning = 0x00B00041;
        #endregion

        #region 演示
        /// <summary>
        /// 辅流分辨率异常(0:正常; 1:异常)
        /// </summary>
        public const uint ALARM_TP_PRE_L_videoinputwarn = 0x00B00050;

        /// <summary>
        /// 辅流分辨率异常(0:正常; 1:异常)
        /// </summary>
        public const uint ALARM_TP_PRE_R_videoinputwarn = 0x00B00051;
        #endregion

        #region 电视机
        /// <summary>
        /// 连接状态(0:断开; 1:连接)
        /// </summary>
        public const uint ALARM_TP_LEFTTV_tverror = 0x00B00060;

        /// <summary>
        /// 连接状态(0:断开; 1:连接)
        /// </summary>
        public const uint ALARM_TP_CENTERTV_tverror = 0x00B00061;

        /// <summary>
        /// 连接状态(0:断开; 1:连接)
        /// </summary>
        public const uint ALARM_TP_RIGHTTV_tverror = 0x00B00062;
        #endregion

        #region 继电器
        /// <summary>
        /// 连接状态(0:断开; 1:连接)
        /// </summary>
        public const uint ALARM_TP_RELAY_SWICH_powerconnected = 0x00B00070;
        #endregion

        #region 翻转屏
        /// <summary>
        /// 连接状态(0:断开; 1:连接)
        /// </summary>
        public const uint ALARM_TP_DISPLAY_1_displaystate = 0x00B00080;

        /// <summary>
        /// 连接状态(0:断开; 1:连接)
        /// </summary>
        public const uint ALARM_TP_DISPLAY_2_displaystate = 0x00B00081;

        /// <summary>
        /// 连接状态(0:断开; 1:连接)
        /// </summary>
        public const uint ALARM_TP_DISPLAY_3_displaystate = 0x00B00082;

        /// <summary>
        /// 连接状态(0:断开; 1:连接)
        /// </summary>
        public const uint ALARM_TP_DISPLAY_4_displaystate = 0x00B00083;

        /// <summary>
        /// 连接状态(0:断开; 1:连接)
        /// </summary>
        public const uint ALARM_TP_DISPLAY_5_displaystate = 0x00B00084;

        /// <summary>
        /// 连接状态(0:断开; 1:连接)
        /// </summary>
        public const uint ALARM_TP_DISPLAY_6_displaystate = 0x00B00085;

        /// <summary>
        /// 连接状态(0:断开; 1:连接)
        /// </summary>
        public const uint ALARM_TP_DISPLAY_7_displaystate = 0x00B00086;

        /// <summary>
        /// 连接状态(0:断开; 1:连接)
        /// </summary>
        public const uint ALARM_TP_DISPLAY_8_displaystate = 0x00B00087;

        /// <summary>
        /// 连接状态(0:断开; 1:连接)
        /// </summary>
        public const uint ALARM_TP_DISPLAY_9_displaystate = 0x00B00088;
        #endregion
        #endregion

        #region TE30告警列表 0x00C00001开始
        /// <summary>
        /// TE告警列表开始
        /// </summary>
        public const uint ALARM_TE_BLOCK = 0x00C00000;

        /// <summary>
        /// 启动初始化
        /// </summary>
        public const uint ALARM_TE_INITIAL = ALARM_TE_BLOCK + 0x01;

        /// <summary>
        /// 重启
        /// </summary>
        public const uint ALARM_TE_REBOOT = ALARM_TE_BLOCK + 0x02;

        /// <summary>
        /// CPU温度过高
        /// </summary>
        public const uint ALARM_TE_CPU_TEMPERATURE = ALARM_TE_BLOCK + 0x03;

        /// <summary>
        /// E1线路故障
        /// </summary>
        public const uint ALARM_TE_E1_LINE = ALARM_TE_BLOCK + 0x04;

        /// <summary>
        /// 丢包
        /// </summary>
        public const uint ALARM_TE_LOST_PACKET = ALARM_TE_BLOCK + 0x05;

        /// <summary>
        /// WEB认证失败发送
        /// </summary>
        public const uint ALARM_TE_WEB_AUTHERTICATION = ALARM_TE_BLOCK + 0x06;

        /// <summary>
        /// 开机/重启检测配置参数有误发送
        /// </summary>
        public const uint ALARM_TE_CONFIGURATION = ALARM_TE_BLOCK + 0x07;

        /// <summary>
        /// IP地址改变
        /// </summary>
        public const uint ALARM_TE_IP_CHANGE = ALARM_TE_BLOCK + 0x08;

        /// <summary>
        /// 终端同步SERVER时间
        /// </summary>
        public const uint ALARM_TE_NTP = ALARM_TE_BLOCK + 0x09;

        /// <summary>
        /// 接上/断开CAMER
        /// </summary>
        public const uint ALARM_TE_CAMERA = ALARM_TE_BLOCK + 0x0A;

        /// <summary>
        /// 设备温度过高
        /// </summary>
        public const uint ALARM_TE_DEVICE_TEMPERATURE = ALARM_TE_BLOCK + 0x0B;

        /// <summary>
        /// 后台升级请求
        /// </summary>
        public const uint ALARM_TE_BACKGROUND_UPGRADE_REQUEST = ALARM_TE_BLOCK + 0x0C;

        /// <summary>
        /// 后台升级响应
        /// </summary>
        public const uint ALARM_TE_BACKGROUND_UPGRADE_RESPONSE = ALARM_TE_BLOCK + 0x0D;

        /// <summary>
        /// 接上或者断开MICROPHONE
        /// </summary>
        public const uint ALARM_TE_MICROPHONE = ALARM_TE_BLOCK + 0x0E;

        /// <summary>
        /// 终端发生网络抖动
        /// </summary>
        public const uint ALARM_TE_NETWORK_JITTER = ALARM_TE_BLOCK + 0x0F;

        /// <summary>
        /// GK/SIP状态变化
        /// </summary>
        public const uint ALARM_TE_GK_SIP = ALARM_TE_BLOCK + 0x11;

        /// <summary>
        /// 遥控器电池电量过低
        /// </summary>
        public const uint ALARM_TE_REMOTE_CONTROL_BATTERY = ALARM_TE_BLOCK + 0x12;

        /// <summary>
        /// 低音告警
        /// </summary>
        public const uint ALARM_TE_LOW_VOLUME = ALARM_TE_BLOCK + 0x13;

        #region 终端摄像机异常告警 + 原因  0x100 - 0x164
        /// <summary>
        /// 终端摄像机异常
        /// </summary>
        public const uint ALARM_TE_CAMERA_ERROR = ALARM_TE_BLOCK + 0x100;

        /// <summary>
        /// 终端摄像机异常:未检测到信号
        /// </summary>
        public const uint ALARM_TE_CAMERA_ERROR_NO_SIGNAL = ALARM_TE_CAMERA_ERROR + 1;

        /// <summary>
        /// 终端摄像机异常:接口未连接
        /// </summary>
        public const uint ALARM_TE_CAMERA_ERROR_INTERFACE_DISCONNECT = ALARM_TE_CAMERA_ERROR + 2;

        /// <summary>
        /// 终端摄像机异常:电动镜头盖异常
        /// </summary>
        public const uint ALARM_TE_CAMERA_ERROR_SCENE_ADNORMAL = ALARM_TE_CAMERA_ERROR + 3;

        /// <summary>
        /// 终端摄像机异常:升级失败
        /// </summary>
        public const uint ALARM_TE_CAMERA_ERROR_UPDATE_FAILED = ALARM_TE_CAMERA_ERROR + 4;

        /// <summary>
        /// 终端摄像机异常:风扇异常
        /// </summary>
        public const uint ALARM_TE_CAMERA_ERROR_FAN_ADNORMAL = ALARM_TE_CAMERA_ERROR + 5;

        /// <summary>
        /// 终端摄像机异常:温升过高
        /// </summary>
        public const uint ALARM_TE_CAMERA_ERROR_TEMP_HIGH = ALARM_TE_CAMERA_ERROR + 6;

        /// <summary>
        /// 终端摄像机异常:芯片自检失败CY22393(U33)
        /// </summary>
        public const uint ALARM_TE_CAMERA_ERROR_CY = ALARM_TE_CAMERA_ERROR + 21;

        /// <summary>
        /// 终端摄像机异常:芯片自检失败AD9889(U9)
        /// </summary>
        public const uint ALARM_TE_CAMERA_ERROR_AD = ALARM_TE_CAMERA_ERROR + 22;

        /// <summary>
        /// 终端摄像机异常:芯片自检失败THS8200(U8)
        /// </summary>
        public const uint ALARM_TE_CAMERA_ERROR_THS = ALARM_TE_CAMERA_ERROR + 23;

        /// <summary>
        /// 终端摄像机异常:芯片自检失败F-10(Sony)
        /// </summary>
        public const uint ALARM_TE_CAMERA_ERROR_F10 = ALARM_TE_CAMERA_ERROR + 24;

        /// <summary>
        /// 终端摄像机异常:芯片自检失败ST(U6)
        /// </summary>
        public const uint ALARM_TE_CAMERA_ERROR_ST = ALARM_TE_CAMERA_ERROR + 25;

        /// <summary>
        /// 终端摄像机异常:芯片自检失败FPGA(U14)
        /// </summary>
        public const uint ALARM_TE_CAMERA_ERROR_FPGA = ALARM_TE_CAMERA_ERROR + 26;

        /// <summary>
        /// 终端摄像机异常:PAN查找边界失败,电机坏或光耦坏
        /// </summary>
        public const uint ALARM_TE_CAMERA_ERROR_PAN_SIDE = ALARM_TE_CAMERA_ERROR + 41;

        /// <summary>
        /// 终端摄像机异常:PAN转动错误,电机坏或被卡住
        /// </summary>
        public const uint ALARM_TE_CAMERA_ERROR_PAN_TURN = ALARM_TE_CAMERA_ERROR + 42;

        /// <summary>
        /// 终端摄像机异常:PAN光耦跳变,光耦转动不均匀
        /// </summary>
        public const uint ALARM_TE_CAMERA_ERROR_PAN_JUMP = ALARM_TE_CAMERA_ERROR + 43;

        /// <summary>
        /// 终端摄像机异常:TILT查找边界失败,电机坏或光耦坏或被卡住
        /// </summary>
        public const uint ALARM_TE_CAMERA_ERROR_TILT_SIDE = ALARM_TE_CAMERA_ERROR + 61;

        /// <summary>
        /// 终端摄像机异常:TILT转动错误,电机坏或被卡住
        /// </summary>
        public const uint ALARM_TE_CAMERA_ERROR_TILT_TURN = ALARM_TE_CAMERA_ERROR + 62;

        /// <summary>
        /// 终端摄像机异常:TILT光耦跳变,光耦转动不均匀
        /// </summary>
        public const uint ALARM_TE_CAMERA_ERROR_TILT_JUMP = ALARM_TE_CAMERA_ERROR + 63;

        /// <summary>
        /// 终端摄像机异常:预留
        /// </summary>
        public const uint ALARM_TE_CAMERA_ERROR_STAY = ALARM_TE_CAMERA_ERROR + 0x64;
        #endregion

        #endregion
    }

    /// <summary>
    /// 告警定位错误信息
    /// </summary>
    [DataContract]
    public class SmcAlarmOri : SmcErr
    {
    }

    /// <summary>
    /// 告警定位错误信息
    /// </summary>
    [DataContract]
    public class SmcAlarmOrientation : SmcErr
    {
    }
}
