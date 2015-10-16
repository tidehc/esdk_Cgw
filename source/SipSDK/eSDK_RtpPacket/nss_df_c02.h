#ifndef _NSS_DF_C02_HEADER_
#define _NSS_DF_C02_HEADER_

#include <vector>

#define	MAC_ADDR_LEN	      17

//lint -e830
#ifndef UNDEF_ERRCODE
#define UNDEF_ERRCODE 0
#endif

#define IVS_DATE_TIME_FORMAT  "YYYYMMDDThhmmssZ"  // 日期&时间长度

const unsigned int          ID_LEN                      = 32;                   // 通用ID
const unsigned int          TRANS_ID_LEN                = 64;                   // 事务编号长
const unsigned int          RESERVE_ID_LEN              = 32;                   // 预留字段长
const unsigned int          DESCRIPTION_LEN             = 256;
const unsigned int          DEVICE_ID_LEN               = 32;                   // 设备ID 长度
const unsigned int          SERVER_DEV_CODE_LEN         = 40;                   // 网元设备code 长度，如MU#xxxx
const unsigned int          USER_ID_LEN                 = 40;                   // 用户ID  长
const unsigned int          DOMAIN_CODE_LEN             = 32;                   // 域ID
const unsigned int          FILE_NAME_LEN               = 128;                  // 文件名长
const unsigned int          IVS_DATE_TIME_LEN           = 20;                   // 时间日期长度 YYYYMMDDThhmmssZ
const unsigned int          IVS_TIME_LEN                = 15;                   // 时间长度
const unsigned int          HEARTBEAT_INTERVAL          = 10;                   // 心跳间隔
const unsigned int          HEARTBEAT_TIMEOUT_TIMES     = 3;                    // 允许的心跳超时次数
const unsigned int          IVS_NSS_URL_LEN             = 512;                  // URL 长
const unsigned int          IP_LEN                      = 32;                   // IP len
const unsigned int          SERVICE_ID_LEN              = 32;                   // 业务ID
const unsigned int			SESSION_ID_LEN			    = 32;					// 事务ID长度  
const unsigned int          IVS_SDP_LEN                 = 2048;                 // SDP信息长度
const unsigned int          REC_FILE_NAME_LEN           = 64;                   // 录像文件名
const unsigned int          CAMERA_NAME_LEN             = 20;                   // 摄像机名称长度
const unsigned int          LABEL_ID_LEN                = 32;                   // 书签ID长度
const unsigned int          IVS_NVS_TIME_LEN            = 20;
const unsigned int          SSRC_CODE_LEN               = 32;                   // SSRC长度


const unsigned int          NSS_DEV_CODE_LEN            = 32;                   // 
const unsigned int          LABEL_NAME_LEN              = 64;                   // 书签名称长度
const unsigned int          PRESET_ID_LEN               = 32;                   //预置位ID
const unsigned int          PRESET_NAME_LEN             = 64;                   // 预置位名称长度
const unsigned int          LOCK_ID_LEN                 = 32;                   // 锁定ID长度
const unsigned int          LOCK_DESC_LEN               = 64;                   // 锁定描述长度
const unsigned int          OPERATOR_NAME_LEN           = 32;                   // 锁定ID长度

const unsigned int          SECURITY_KEY_PLAINTEXT_LEN  = 48;            		// 媒体安全明文长度
const unsigned int          MEDIA_SECURITY_KEY_LEN      = 90;                   // 视频安全密码密文长度
const unsigned int          AUDIO_SECURITY_KEY_LEN      = 90;                   // 音频安全密码密文长度

const unsigned int          INT_MAX_LENGTH              = 10;                   // int长度
	
const unsigned short        NSS_PROTOCOL_VERSION        = 0x0100;               // 信令协议版本

const unsigned short        RELIABLE_TRANSMISSION_LEN 	= 4;               		// 可靠传输类型长度

// tlv tag 定义
enum _INVS_TLV_TAG
{
    TAG_XMLCONTENT                                      = 0x0001,               // 通过NSS发送xml文件时，xml内容字段的Tag值
    TAG_USERID                                          = 0x0002,               // 用户ID
    TAG_ATTACHMENT                                      = 0x0003,               // 二进制文件
    TAG_ALARMDESC                                       = 0x0004,               // 告警描述信息
    TAG_EXTPARAM                                        = 0x0005,               // 告警扩展参数
    TAG_OPERATEINFO                                     = 0x0006                // 告警确认/撤销处理信息
        
};


typedef struct
_RTSP_OPTION_MSG_TIMESTAMP
{
	unsigned int                 TimeUTCSec;       // 视频对应UTC时间，秒
    unsigned int                 TimeUTCMS;        // 视频对应的UTC时间，毫秒
    unsigned int                 RTPTimeTick;      // 该视频帧对应的RTP时间戳
}RTSP_OPTION_MSG_TIMESTAMP, *PRTSP_OPTION_MSG_TIMESTAMP;

//录像参数
typedef struct _RECORD_POLICY_PARA
{
    unsigned int    RecordMode;                            //存储类型,FULL-全量存储,FRAME_EXTRACT-抽帧存储,MOVE_DECTION-移动侦测智能存储
    unsigned int    AllocMethod;                           //分配方式,TIME-留存期,CAPACITY-容量 
    unsigned int    Time;                                  //天数,留存期分配方式时必选
    unsigned int    FrameExtractRecordTime;                //抽帧存储天数
    unsigned int    AlarmTime;                             //有告警录像存储天数
    unsigned int    KeyframeTime;                          //关键帧天数
	unsigned int    BeforeAlarm;                           //移动侦测警前时间
	unsigned int    AfterAlarm;                            //移动侦测警后时间
	unsigned int    PlanStreamID;                          //计划录像使用的流ID
	unsigned int    AlarmStreamID;                         //告警录像使用的流ID
	unsigned int    AlarmRecordTTL;                        //告警录像留存期
	unsigned int    ManualRecordTTL;                       //手动录像留存期
	unsigned int    PreRecord;                         	   //预录设置
	unsigned int    PreRecordTime;                         //预录时长
	unsigned int    AssociatedAudio;                       //随路音频叠加选项
}RECORD_POLICY_PARA;

//录像参数
typedef struct _RECORD_FAULT_UPDATE_PARA
{
	char 	lensId[DEVICE_ID_LEN];
	long 	currTime;
	long 	beginTime;
	long 	endTime;
	int 	update_flag;
    bool    bLoadStrateyFromDB;
}RECORD_FAULT_UPDATE_PARA;

//录像时长
typedef struct _RECORD_PLAN_SLICE
{
	long 	beginTime;
	long 	endTime;
}RECORD_PLAN_SLICE;

typedef struct _OMU_RECORD_MODULEINFO
{
    char devCode[DEVICE_ID_LEN + 1];
	unsigned int devType;
	char nodeCode[DOMAIN_CODE_LEN + 1];
	char devName[CAMERA_NAME_LEN + 1];
}OMU_RECORD_MODULEINFO;

// 录像检索查询方式 QueryType;
enum UPDATE_RECORD_FAULT_FLAG
{
    UPDATE_RECORD_FAULT_DELETE                  = 0x00,       // 更新当前时间后的录像计划
    UPDATE_RECORD_FAULT_ADD                     = 0x01,       // 新增录像计划，更新故障数据
    UPDATE_RECORD_FAULT_MODIFY                  = 0x02,       // 删除、调整录像，更新故障数据
    UPDATE_RECORD_FAULT_MAX
};

// 录像检索查询方式 QueryType;
enum QUERY_TYPE
{
    QUERY_TYPE_TIME              = 0x00,       // 按时间
    QUERY_TYPE_BOOKMARK          = 0x01,       // 按书签
    QUERY_TYPE_PRESET            = 0x02,       // 按预置位
    QUERY_TYPE_LOCK              = 0x03,       // 按锁定状态
    QUERY_TYPE_MAX
};

// 业务类型 ServiceType;
typedef enum
{
    IVS_SERVICE_TYPE_INVALID                = 0,
    IVS_SERVICE_TYPE_REALVIDEO              = 0x0001,       // 实况;
    IVS_SERVICE_TYPE_RECORD                 = 0x0002,       // 平台录像;
    IVS_SERVICE_TYPE_DOWNLOAD               = 0x0003,       // 平台录像下载;
    IVS_SERVICE_TYPE_PLAYBACK               = 0x0004,       // 平台录像回放;
    IVS_SERVICE_TYPE_AUDIO_CALL             = 0x0005,       // 音频对讲;
    IVS_SERVICE_TYPE_AUDIO_BROADCAST        = 0x0006,       // 音频广播;
    IVS_SERVICE_TYPE_PU_DOWNLOAD	        = 0x0007,	    // 前端录像下载;
    IVS_SERVICE_TYPE_PU_PLAYBACK            = 0x0008,	    // 前端录像回放;
    
    IVS_SERVICE_TYPE_REALVIDEO_BACKUP		= 0x0014,		// 实况备份; mbu使用，mu收到后转换成实况处理
    IVS_SERVICE_TYPE_PLATRECORD_BACKUP		= 0x0015,		// 平台录像备份;mbu使用，mu收到后转换成平台录像下载处理
    IVS_SERVICE_TYPE_PURECORD_BACKUP		= 0x0016,		// 前端录像备份;mbu使用，mu收到后转换成前段录像下载处理
    IVS_SERVICE_TYPE_BACKUPRECORD_PLAYBACK	= 0x0017,		// 录像备份回放; 收到后转换成平台录像回放处理
    IVS_SERVICE_TYPE_BACKUPRECORD_DOWNLOAD	= 0x0018,		// 录像备份下载; 收到后转换成平台录像下载处理
    
    IVS_SERVICE_TYPE_DISASTEBACKUP_PLAYBACK = 0x0019,       // 容灾录像回放
    IVS_SERVICE_TYPE_DISASTEBACKUP_DOWNLOAD = 0x001A,       // 容灾录像下载
    IVS_SERVICE_TYPE_PU_UPLOAD              = 0x001B,       // 前端缓存补录
    IVS_SERVICE_TYPE_PU_PRECORD				= 0x001C,		// 前端预录
    
    IVS_SERVICE_TYPE_MAX
}NVS_SERVICE_TYPE;

/*
 * 媒体分发方式 TransType
 */
enum TRANS_TYPE
{
    TRANS_TYPE_INVALID      = -1,
    TRANS_TYPE_TRANS_MDU    = 0x0000,        // 通过MDU中转
    TRANS_TYPE_DIRECT_CU    = 0x0001,        // 直连CU

    TRANS_TYPE_MAX
};

#if 0 // todo l00201416 早晚要删，请勿使用
enum PAY_LOAD_TYPE
{
    PAY_LOAD_TYPE_PCMU  = 0,    ///< G711的u率
    PAY_LOAD_TYPE_G723  = 4,    ///< G723
    PAY_LOAD_TYPE_PCMA  = 8,    ///< G711的a率
    PAY_LOAD_TYPE_G722  = 9,    ///< G722
    PAY_LOAD_TYPE_MJPEG = 24,   ///< MJPEG
    PAY_LOAD_TYPE_AMR_MB= 97,   ///< AMR_NB
    PAY_LOAD_TYPE_MPEG4 = 98,   ///< MPEG4
    PAY_LOAD_TYPE_H264  = 99,   ///< H264
    PAY_LOAD_TYPE_AVS   = 100,  ///< AVS
    PAY_LOAD_TYPE_MP3   = 101,  ///< MP3
    PAY_LOAD_TYPE_3GPP  = 102,  ///< 3GPP
};
#endif

/*
 * 传输方向TRANS_DIRECTION
 */
enum TRANS_DIRECTION
{
    TRANS_DIRECTION_RECVONLY     = 0x1,     // 仅接收
    TRANS_DIRECTION_SENDONLY     = 0x2,     // 仅发送
    TRANS_DIRECTION_SENDRECV     = 0x3,     // 即发送又接收
    TRANS_DIRECTION_MAX
};

enum CONN_SETUP_TYPE
{
    CONN_SETUP_TYPE_ACTIVE       = 0x1,     // 主动建立连接
    CONN_SETUP_TYPE_PASSIVE      = 0x2,     // 被动建立连接
    CONN_SETUP_TYPE_MAX
};

/*
*复用流标志StreamReused
*/
enum STREAM_REUSED_FLAG
{
    STREAM_UNREUSED    = 0x0,       // 流不复用
    STREAM_REUSED      = 0x1      // 流复用
};

/*
 * 网络传输承载类型 TransProtocol
 */

enum TRANS_PROTOCOL
{
    TRANS_PROTOCAL_MIN     = 0,
    TRANS_PROTOCAL_RTP_UDP = 0x01, // RTP over UDP
    TRANS_PROTOCAL_RTP_TCP = 0x02, // RTP over TCP
    TRANS_PROTOCAL_UDP     = 0x03, // UDP
    TRANS_PROTOCAL_TCP     = 0x04, // TCP
    TRANS_PROTOCAL_MAX
};

enum PACKET_PROTOCAL
{
    PACKET_PROTOCAL_MIN = 0,
    PACKET_PROTOCAL_ES  = 0x01, //ES
    PACKET_PROTOCAL_PS  = 0x02, //PS
    PACKET_PROTOCAL_TS  = 0x03, //TS
    PACKET_PROTOCAL_ES2PS = 0x04,
    PACKET_PROTOCAL_PS2ES = 0x05,
    PACKET_PROTOCAL_ES2TS = 0x06,
    PACKET_PROTOCAL_TS2ES = 0x07,
    PACKET_PROTOCAL_PS2TS = 0x08,
    PACKET_PROTOCAL_TS2PS = 0x09,
    PACKET_PROTOCAL_MAX
};

#define PACKET_PROTOCAL_NUMBER 3

/// MDU消息块相关定义
enum _enMDU_MSG_BLOCK_DEF
{
    MDU_MSG_BLOCK_SIZE          = 2 * 1024,     /// 定义消息块大小
    MDU_NSS_BLOCK_SIZE          = 1800,         /// 定义一个NSS包大小
    MDU_BLOCK_NUM_PER_CHANNEL   = 200,          /// 每路视频分配的消息块个数
    MDU_MAX_QUEUE_MSG_NUM       = 1 * 1024,     /// 定义消息队列最大消息个数
    MDU_MAX_QUEUE_BYTE_SIZE     = (MDU_MSG_BLOCK_SIZE * MDU_MAX_QUEUE_MSG_NUM)  /// 定义消息队列最大SIZE
};

/*
 * 是否支持QOS SupportQoSFlag
 */
enum SUPPORT_QOS_FLAG
{
    SUPPORT_QOS_FLAG_TRUE    = 0x01, // 支持
    SUPPORT_QOS_FLAG_FALSE   = 0x02, // 不支持
    SUPPORT_QOS_FLAG_MAX
};

/*
 * 摄像头类型
 */
enum RECORD_CAMERA_TYPE
{
    RECORD_CAMERA_TYPE_TEYES        = 0,    // 千里眼协议 
    RECORD_CAMERA_TYPE_ONVIF        = 1,    //  Onvif协议
    RECORD_CAMERA_TYPE_HUAWEI       = 2,    // 华为摄像头
    RECORD_CAMERA_TYPE_HKSDK        = 3,    // 海康SDK
    RECORD_CAMERA_TYPE_MAX
};

/*
*媒体数据类型
*/
enum MEDIA_DATA_TYPE
{
    VIDEO_RTP_PACKET             = 0, // 视频RTP包
    VIDEO_RTCP_PACKET            = 1, // 视频RTCP包
    AUDIO_RTP_PACKET             = 2, // 音频RTP包
    AUDIO_RTCP_PACKET            = 3  // 音频RTCP包
};

/*
 * 媒体类型 MediaType
 */
enum NSS_MEDIA_TYPE
{
    MEDIA_TYPE_VIDEO_AUDIO = 0x01, // 音视频编码
    MEDIA_TYPE_VIDEO       = 0x02, // 视频编码
    MEDIA_TYPE_AUDIO       = 0x03, // 音频编码
    MEDIA_TYPE_ERROR       = 0x04  // 其它表示错误
};

/*
 * 流控类型
 * 0x01：
 * 0x02：快退，PlayRate为快退速度；
 * 0x03：播放，PlayTime为当前要播放的开始时间点，即客户端界面进度条上游标所在的时间点；
 * 0x04：暂停；
 */
enum PLAYBACK_CTRL_CODE
{
    PLAYBACK_CTRL_CODE_MIN      = 0,
    PLAYBACK_CTRL_CODE_SPEED    = 0x01,     // 快进，PlayRate为快进速度；
    PLAYBACK_CTRL_CODE_BACK     = 0x02,     // 快退，PlayRate为快退速度；
    PLAYBACK_CTRL_CODE_DRAG     = 0x03,     // 播放，PlayTime为当前要播放的开始时间点；
    PLAYBACK_CTRL_CODE_PAUSE    = 0x04,     // 暂停；
    PLAYBACK_CTRL_GET_BASE_TIME = 0x20,     // 获取播放基准时间
    PLAYBACK_CTRL_CODE_MAX
};

/************************************************************************/
/*                           平台服务器类型定义                         */
/************************************************************************/
enum NSS_PLATFORM_SERVER_TYPE_CONST
{
    NSS_PLATFORM_SERVER_TYPE_NONE           =0x00,    /* 无效的服务器类型*/
    NSS_PLATFORM_SERVER_TYPE_SMU            =0x01,    /* SMU*/
    NSS_PLATFORM_SERVER_TYPE_SCU            =0x02,    /* SCU*/
    NSS_PLATFORM_SERVER_TYPE_CMU            =0x03,    /* CMU*/
    NSS_PLATFORM_SERVER_TYPE_OMU            =0x04,    /* OMU*/
    NSS_PLATFORM_SERVER_TYPE_MU             =0x05,    /* MU*/
    NSS_PLATFORM_SERVER_TYPE_DCG            =0x06,    /* DCG */
    NSS_PLATFORM_SERVER_TYPE_PCG            =0x07,    /* PCG*/
    NSS_PLATFORM_SERVER_TYPE_MAUS           =0x08,    /* MAUS*/
    NSS_PLATFORM_SERVER_TYPE_MTU            =0x09,    /* MTU*/
    
    //请在MAX前增加新类型，且值保证连续
    NSS_PLATFORM_SERVER_TYPE_MAX
};


#define NSS_NAME_MAP_TYPE \
{ \
    {NSS_PLATFORM_SERVER_TYPE_NONE,         "\0"        },   \
    {NSS_PLATFORM_SERVER_TYPE_SMU,          "NVS_SMU"   },   \
    {NSS_PLATFORM_SERVER_TYPE_SCU,          "IVS_SCU"   },   \
    {NSS_PLATFORM_SERVER_TYPE_CMU,          "NVS_CMU"   },   \
    {NSS_PLATFORM_SERVER_TYPE_OMU,          "IVS_OMU"   },   \
    {NSS_PLATFORM_SERVER_TYPE_MU,           "NVS_MU"    },   \
    {NSS_PLATFORM_SERVER_TYPE_DCG,          "NVS_DCG"   },   \
    {NSS_PLATFORM_SERVER_TYPE_PCG,          "NVS_PCG"   },   \
    {NSS_PLATFORM_SERVER_TYPE_MAUS,         "IVS_MAUS"  },   \
    {NSS_PLATFORM_SERVER_TYPE_MTU,          "NVS_MTU"   }    \
}


/// 定义网络连接状态
typedef enum _enNETWORK_HANDLE_STATUS
{
    NETWORK_HANDLE_STATUS_INIT = 0,            /// 初始状态，对象刚创建时的状态
    NETWORK_HANDLE_STATUS_OPEN,            /// 连接已打开，占用了端口，但此时收到数据或连接会直接丢弃
    NETWORK_HANDLE_STATUS_START,           /// 开始接收数据，正常处理
    NETWORK_HANDLE_STATUS_DISCONNECT,      /// 接收数据过程中连接异常，TCP连接才有此状态
    NETWORK_HANDLE_STATUS_ABNORMAL         /// 其它异常
}NETWORK_HANDLE_STATUS;

/*
 * 录像类型
 */
typedef enum _E_RECORD_TYPE
{
    RECORD_TYPE_NONE            = -1,    

    MANUAL_RECORD               = 0x00,     // 手动录像
    ALARM_RECORD               	= 0x01,     // 告警录像
    PLAN_RECORD                 = 0x02,     // 计划录像
    PRE_RECORD                  = 0x03,     // 平台预录
    MEND_RECORD                 = 0x04,     // 补录
    LOCK_RECORD                 = 0x05,     // 锁定录像
    PRE_TO_RECORD               = 0x06,     // 预录转录像
    
    RECORD_TYPE_MAX
}E_RECORD_TYPE;

/*
 * 录像状态flags 
 */
enum RECORD_FLAGS
{
    RECORD_FLAG_NONE            = 0,    

    RECORD_FLAG_MANUAL          = 1,     // 手动录像
    RECORD_FLAG_ALARM          	= 2,     // 告警录像
    RECORD_FLAG_PLAN            = 4,     // 计划录像
    RECORD_FLAG_PRE             = 8,     // 平台预录

    RECORD_FLAG_MASK            = 15
};

/*
 * 码流类型
 */
enum STREAM_ID_
{
    STREAM_ID_INVALID           = -1,  
    STREAM_ID_NOT_ASSIGNED      = 0,            // 不指定码流
    STREAM_ID_MAIN,                             // 主码流 
    STREAM_ID_SUB1,                             // 子码流1
    STREAM_ID_SUB2,                             // 子码流2

    STREAM_ID_MAX               = 17
};

/*
 * 媒体单组播类型
 */
enum CAST_MODE_
{
    CAST_MODE_INVALID           = -1,
    CAST_MODE_UNICAST           = 0,        // 单播
    CAST_MODE_MULTICAST         = 1,        // 组播

    CAST_MODE_MAX
};

/*
 * 录像方式
 */
typedef enum MU_RECORD_METHOD_
{
    MU_RECORD_METHOD_INVALID       = -1,
    MU_RECORD_METHOD_PLAT          = 0,        // 录像服务器
    MU_RECORD_METHOD_PU            = 1,        // 前端
	MU_RECORD_METHOD_MBU           = 2,        // 备份服务器
	MU_RECORD_METHOD_RECOVERY	   = 3,		   // 容灾服务器
    MU_RECORD_METHOD_MAX
}MU_RECORD_METHOD;

// 手动录像状态
typedef enum _MANUAL_REC_STATE
{
    MANUAL_REC_STATE_INVALID    = -1,        
    MANUAL_REC_STATE_END        = 0,        // 停止
    MANUAL_REC_STATE_START      = 1,        // 启动

    MANUAL_REC_STATE_MAX
}MANUAL_REC_STATE;

// 手动录像停止原因
typedef enum _MANUAL_REC_STOP_REASON
{
    STOP_REASON_INVALID         = -1,
    STOP_REASON_MANUAL          = 0,        // 手动停止
    STOP_REASON_TIMEOUT         = 1,        // 超时停止
    STOP_REASON_UNEXCPETED      = 2,        // 异常停止

    STOP_REASON_MAX
}MANUAL_REC_STOP_REASON;

// 查询手动录像任务响应的镜头信息结构
typedef struct _REC_TASK_CAMERAINFO
{    
    // char            DomainCode[DOMAIN_CODE_LEN];                // 消息目标域
    char            CameraCode[DEVICE_ID_LEN];                  // 摄像机编码
    unsigned int    RecordMethod;                               // 录像方式：0-平台录像1-前端录像
}REC_TASK_CAMERAINFO;
typedef std::vector<REC_TASK_CAMERAINFO> CAMERA_LIST;

// 标记当前录像状态相关
typedef struct _ST_RECORD_STATUS
{
	volatile E_RECORD_TYPE	record_type;					// 录像类型
	volatile unsigned char	record_flag;					// 录像标识
	volatile bool			recording_flag;					// 写录像标识
	volatile unsigned int	total_manual;					// 总的手动录像数
	volatile unsigned int	total_event;					// 总告警录像数
	volatile unsigned int	total_plan;						// 计划起止时间重叠时使用
	char					alarm_type[ID_LEN];				// 告警类型，告警录像时有效
	char					user_id[USER_ID_LEN];			// 用户ID，手动录像时有效
	char					user_domain[DOMAIN_CODE_LEN];	// 用户域ID, 手动录像时有效
	int						tbl_event_id;					// 事件录像表中的策略ID
	bool					restart_flag;					// 重启录像标记	
	bool					prealarm_sync_flag;				// 告警预录同步标识
}ST_RECORD_STATUS;

// 本域外域  临时定义，SDK里定义
typedef enum
{
    HOME_DOMAIN           = 0x0000,
    EX_DOMAIN             = 0x0001
}DOMAINTYPE;

enum NVR_TYPE
{
	NSS_NVR_TYPE_INVALID	= -1,
    NSS_NORMAL_NVR_TYPE 	= 0,	//普通NVR
    NSS_AGENT_NVR_TYPE 		= 1,  	//代理NVR
    NSS_MBU_NVR_TYPE 		= 2    	//备份服务器
};

typedef enum _enNET_HANDLE_TYPE
{
    NET_HANDLE_RTP,            /// RTP
    NET_HANDLE_RTCP,            /// RTCP

    HANDLE_RELEASE_SESSION 
}NET_HANDLE_TYPE;

// 发送、接收消息前封装的头信息
typedef struct _NSS_SOCKET_BIRTH
{
	long seconds_;
	long microseconds_;
}NSS_SOCKET_BIRTH;


#endif // _NSS_DF_C02_HEADER_

