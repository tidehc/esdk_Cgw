#ifndef _HW_IVS_SDK_H_
#define _HW_IVS_SDK_H_

#pragma pack(push, 1)

// 类型定义
typedef  int                IVS_INT32;
typedef  unsigned int       IVS_UINT32; 
typedef  void*				IVS_LPVOID;
typedef  long               IVS_LONG;
typedef  unsigned long      IVS_ULONG;

#ifdef _MSC_VER
typedef __int64             IVS_INT64;
typedef unsigned __int64    IVS_UINT64;
#else
typedef long long           IVS_INT64;
typedef unsigned long long  IVS_UINT64;
#endif 

typedef  int	            IVS_BOOL;
typedef  char               IVS_CHAR;
typedef  unsigned char      IVS_UCHAR;
typedef  float              IVS_FLOAT;
typedef  double             IVS_DOUBLE;

typedef  void               IVS_VOID;

typedef  short              IVS_SHORT;
typedef  unsigned short     IVS_USHORT;

#ifdef WIN32
#define __SDK_CALL __stdcall
#else
#define __SDK_CALL
#endif

#ifdef WIN32
#define __SDK_CALLBACK CALLBACK
#else
#define __SDK_CALLBACK
#endif

#ifndef WIN32

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

#ifndef BYTE
#define BYTE unsigned char
#endif

#define HWND void*

#endif

//定义NULL
#ifndef NULL
#ifdef __cplusplus
#define NULL    0
#else
#define NULL    ((void *)0)
#endif
#endif



// 消息定义(消息上报)
#define IVS_EVENT_AUTO_CONNECT_SUCCEED                  10001   // 重新登录成功事件上报
#define IVS_EVENT_KEEP_ALIVE_FAILED                     10002   // 保活失败事件上报
#define IVS_EVENT_REALPLAY_FAILED                       10003   // 实时浏览异常事件上报
#define IVS_EVENT_RECORD_FAILED                         10004   // 录像异常事件上报
#define IVS_EVENT_DOWNLOAD_FAILED                       10005   // 录像下载异常事件上报
#define IVS_EVENT_REMOTE_PLAYBACK_FAILED                10006   // 远程录像回放异常事件上报
#define IVS_EVENT_LOCAL_PLAYBACK_FAILED                 10007   // 本地录像回放异常事件上报
#define IVS_EVENT_TALKBACK_FAILED                       10008   // 语音对讲异常事件上报
#define IVS_EVENT_BROADCAST_FAILED		                10009   // 语音广播异常事件上报
//#define IVS_EVENT_START_MULT_SNAPSHOT                   10010   // 开始连续抓拍
//#define IVS_EVENT_STOP_MULT_SNAPSHOT                    10011   // 连续抓拍异常停止
//#define IVS_EVENT_FINISHED_MULT_SNAPSHOT                10012   // 连续抓拍完成
#define IVS_EVENT_REPORT_ALARM                          10013   // 告警上报
#define IVS_EVENT_FILE_BROADCAST_END                    10014   // 本地文件广播结束事件上报

//#define IVS_EVENT_REMOTE_RECORD_STOP                    10015   // 远程录像停止
//#define IVS_EVENT_REPORT_DEVICE                         10016   // 设备状态上报
#define IVS_EVENT_REPORT_ALARM_STATUS                   10017   // 告警状态上报

#define IVS_EVENT_USER_OFFLINE                          10019   // 用户下线通知
#define IVS_EVENT_RESUME_REALPLAY                       10020   // 启动实况重连
#define IVS_EVENT_RESUME_REALPLAY_OK                    10021   // 恢复实况浏览

#define IVS_EVENT_LOCAL_RECORD_SUCCESS                  10022   // 本地录像成功事件
#define IVS_EVENT_LOCAL_RECORD_EGENERIC                 10023   // 录像文件写入失败事件
#define IVS_EVENT_DO_LINKAGE_ACTION                     10024   // 告警联动动作执行通知

#define IVS_EVENT_FIND_DEVICE                           10025   // 发现前端设备事件
#define IVS_EVENT_OMU_REPORT_ALARM                      10026   // OMU将设备类告警发给CU的接口消息
#define IVS_EVENT_REPORT_WATERMARK_TIP                  10027   // 发现水印篡改提示（只有篡改开始时间）
#define IVS_EVENT_REPORT_WATERMARK_ALARM                10028   // 水印告警记录产生通知（有篡改开始时间和停止时间）

#define IVS_EVENT_PTZ_LOCK                              10029   // 云镜锁定上报（云镜操作结果返回时告知客户端）

//#define IVS_EVENT_OTHER                               10030   // 其他模式
#define IVS_EVENT_MANUAL_RECORD_STATE                   10031   // 手动录像状态通知
#define IVS_EVENT_STOP_LINKAGE_ACTION                   10032   // 告警联动动作停止通知
#define IVS_EVENT_DOWNLOAD_SUCCESS                      10033   // 录像下载完成事件上报
//#define IVS_EVENT_PWD_EXPIRE_DAYS                       10034   // 用户密码过期天数
#define IVS_EVENT_PLAYER_BUFFER                         10035   // 客户端缓存通知
#define IVS_EVENT_PLAY_SOUND_FAILED                     10036   // 随路语音开启失败（已有语音对讲不可开启随路语音）

#define IVS_EVENT_PLAYER_WATER_MARK_EXCEPTION           10037   // 水印异常事件上报

#define IVS_EVENT_IA_BA_LEARNING_SCHEDULE_REPORT        10100   // 智能分析学习进度上报
#define IVS_EVENT_IA_FR_SCHEDULE_REPORT                 10101   // 智能分析人脸批量注册进度上报
#define IVS_EVENT_IA_PUSH_ALARM_LOCUS                   10102   // 智能分析轨迹推送
#define IVS_EVENT_IA_DRAW_OVER                          10103   // 智能分析绘图结束
#define IVS_EVENT_IA_ADDPLAN_SCHEDULE_REPORT            10104   // 智能分析添加计划进度上报 

#define IVS_EVENT_MODE_CRUISE_RECORD_OVER               10203   // 模式轨迹录制结束通知

#define IVS_EVENT_LOGIN_FAILED                          10301   // 客户端登录失败（例如，重连时密码改变）

#define IVS_EVENT_SHUTDOWN_STREAM				10302	//客户端关闭视频业务通知

// 通用常量定义
#define IVS_TIME_LEN			20          // 时间长度
#define IVS_NAME_LEN			128			// 名称长度
#define IVS_TICKET_LEN			64			// 单点登录的Ticket长度
#define IVS_PWD_LEN		        64			// 密码长度(网络传输使用密文)
#define IVS_GROUP_CODE_LEN		64			// 组编号长度
#define IVS_DESCRIBE_LEN		256			// 描述长度
#define IVS_IP_LEN				64			// IP长度
#define IVS_URL_LEN				256			// URL长度
#define IVS_FILE_NAME_LEN		256			// 文件名最大长度
#define IVS_MAX_NAME_LIST_LEN   1024        // 名称列表最大长度，用逗号隔开

// 用户
#define IVS_MACHINE_NAME_LEN	128			// PC机器名长度
#define IVS_DOMAIN_LEN			64			// Windows域名长度
#define IVS_PHONE_NUMBER_LEN	64			// 电话号码最大长度
#define IVS_MAX_OPERATION_RIGHT	128			// 操作权限个数上线
#define	IVS_EMAILE_LEN			128			// 用户Email长度

// 设备信息
#define IVS_DEV_SERIALNO_LEN	64		    // 设备序列号长度
#define IVS_DEV_CODE_LEN        64		    // 设备编码最大长度
#define IVS_DEVICE_GROUP_LEN    128         // 设备组编码最大长度
#define IVS_TAS_CODE_LEN        32          // TAS编码长度
#define IVS_NVR_CODE_LEN        32          // NVR编码长度
#define IVS_CLUSTER_CODE_LEN    32          // 集群编码长度
#define IVS_DOMAIN_CODE_LEN     32		    // 域编码最大长度
#define IVS_DEV_MODEL_LEN	    32		    // 设备型号最大长度

#define IVS_MRU_CODE_LEN        64          //MRU分配方式

#define IVS_MAX_STREAM_LIST_NUM      3           //码流信息列表最大3条
#define IVS_MAX_ENCODE_LIST_NUM      2           //编码类型信息列表最大2条
#define IVS_MAX_RESOLUTION_LIST_NUM  30          //分辨率列表最大30条

// 快捷键
#define IVS_HOTKEY_CTRL         0x01
#define IVS_HOTKEY_SHIFT        0x02
#define IVS_HOTKEY_ALT          0x04
#define IVS_HOTKEY_WIN          0x08

#define IVS_VERSION_LEN  32      // 客户端版本号长度
#define IVS_MD5_LEN      32      // MD5值长度
#define IVS_TOKEN_LEN      128      // MD5值长度



/*流异常结构体*/
typedef struct ST_STREAM_EXCEPTION_INFO
{
    IVS_INT32 iSessionID;       //登录返回的会话ID
    IVS_ULONG ulHandle;         //句柄
    IVS_INT32 iExceptionCode;   //错误码
    IVS_INT32 iReseved1;        //备用
    IVS_CHAR szDeviceCode[IVS_DEV_CODE_LEN];

}STREAM_EXCEPTION_INFO;

typedef enum 
{
    CU_LEFTTOP = 0, //CU左上角
    CU_LOGINABOUT,  //CU登录/关于
    MOBILE_PAD,     //移动PAD
    MOBILE_PHONE,   //移动Phone
    BS_LEFTTOP,     //BS左上角
    BS_LOGINABOUT   //BS登录/关于
}IVS_LOGO_TYPE;

typedef enum
{
    KEY_WEAK=0,
    KEY_MIDDLE,
    KEY_STRONG,
    KEY_STRONGER
}IVS_KEY_RELIABILITY;

// 账号签退类型
typedef enum
{
    USER_OFFLINE_LOCK=0,    // 账号被管理员锁定
    USER_OFFLINE_DELETE=1,  // 账号被管理员删除
    USER_OFFLINE_SIGNOUT=2, // 登录会话被管理员签退
}IVS_USER_OFFLINE_TYPE;

// 时间片段结构
typedef struct
{
    IVS_CHAR	cStart[IVS_TIME_LEN];   // 格式如yyyyMMddHHmmss
    IVS_CHAR	cEnd[IVS_TIME_LEN];     // 格式如yyyyMMddHHmmss
}IVS_TIME_SPAN;

// 本地录像文件上报通知
typedef struct
{
    IVS_CHAR    cCameraCode[IVS_DEV_CODE_LEN];      // 摄像机编码
    IVS_CHAR    cFileName[IVS_FILE_NAME_LEN];       // 文件名
    IVS_CHAR    cFilePath[IVS_FILE_NAME_LEN];       // 文件存放路径
    IVS_CHAR    cOperationTime[IVS_TIME_LEN];       // 操作时间
    IVS_TIME_SPAN   stRecordTime;                   // 录像起始、结束时间
    IVS_UINT32  uiRecordType;                       // 录像类型：0-本地录像，1-平台录像下载
    IVS_BOOL    bIsEncrypt;                         // 录像是否加密：0-不加密，1-加密    
}IVS_LOCAL_RECORD_SUCCESS_INFO;

// 用户下线通知
typedef struct
{
    IVS_INT32   iSessionID;     // 下线用户的SessionID
    IVS_UINT32  uiOfflineType;  // 下线类型，值参考 IVS_USER_OFFLINE_TYPE
}IVS_USER_OFFLINE_INFO;

// 本地录像文件上报通知
typedef struct
{
    IVS_LONG    lErrorCode;     // 错误码
    IVS_ULONG   ulHandle;       // 播放句柄
    IVS_CHAR    cReserve[32];   // 保留字段
}IVS_LOCAL_RECORD_EGENERIC;

// 基本数据结构
typedef struct
{
	IVS_FLOAT	left;
	IVS_FLOAT	top;
	IVS_FLOAT	right;
	IVS_FLOAT	bottom;
}IVS_RECT_FLOAT;

typedef struct
{
	IVS_LONG	left;
	IVS_LONG	top;
	IVS_LONG	right;
	IVS_LONG	bottom;
}IVS_RECT;

typedef enum
{
	WIND_LAYOUT_1_1 = 11,   // 窗口数1,布局1
	WIND_LAYOUT_3_1 = 31,   // 窗口数3,布局1
	WIND_LAYOUT_3_2 = 32,   // 窗口数3,布局2
	WIND_LAYOUT_3_3 = 33,   // 窗口数3,布局3
	WIND_LAYOUT_4_1 = 41,   // 窗口数4,布局1
	WIND_LAYOUT_4_2 = 42,   // 窗口数4,布局2
	WIND_LAYOUT_4_3 = 43,   // 窗口数4,布局3
	WIND_LAYOUT_6_1 = 61,   // 窗口数6,布局1
	WIND_LAYOUT_6_2 = 62,   // 窗口数6,布局2
	WIND_LAYOUT_6_3 = 63,   // 窗口数6,布局3
    WIND_LAYOUT_6_4 = 64,   // 窗口数6,布局4
	WIND_LAYOUT_8_1 = 81,   // 窗口数8,布局1
	WIND_LAYOUT_9_1 = 91,   // 窗口数9,布局1
    WIND_LAYOUT_9_2 = 92,   // 窗口数9,布局2
	WIND_LAYOUT_10_1 = 101, // 窗口数10,布局1
	WIND_LAYOUT_10_2 = 102, // 窗口数10,布局2
	WIND_LAYOUT_12_1 = 121, // 窗口数12,布局1
	WIND_LAYOUT_12_2 = 122, // 窗口数12,布局2
	WIND_LAYOUT_13_1 = 131, // 窗口数13,布局1
	WIND_LAYOUT_13_2 = 132, // 窗口数13,布局2
	WIND_LAYOUT_16_1 = 161, // 窗口数16,布局1
	WIND_LAYOUT_16_2 = 162, // 窗口数16,布局2
	WIND_LAYOUT_17_1 = 171, // 窗口数17,布局1
	WIND_LAYOUT_20_1 = 201, // 窗口数20,布局1
	WIND_LAYOUT_25_1 = 251, // 窗口数25,布局1
	WIND_LAYOUT_26_1 = 261, // 窗口数26,布局1
	WIND_LAYOUT_36_1 = 361, // 窗口数36,布局1
	WIND_LAYOUT_49_1 = 491, // 窗口数49,布局1
	WIND_LAYOUT_64_1 = 641, // 窗口数64,布局1

	WIND_LAYOUT_CUSTOM_1 = 651, //自定义布局1
	WIND_LAYOUT_CUSTOM_2 = 652, //自定义布局2
	WIND_LAYOUT_CUSTOM_3 = 653, //自定义布局3
	WIND_LAYOUT_CUSTOM_4 = 654, //自定义布局4

    WIND_LAYOUT_TVW = 200,      // 电视墙
}IVS_WIND_LAYOUT;

// OCX工具条按钮
const IVS_UINT32 IVS_TOOLBAR_SNAPSHOT       = 0x00000001;   // 抓拍
const IVS_UINT32 IVS_TOOLBAR_LOCAL_RECORD   = 0x00000002;   // 本地录像
const IVS_UINT32 IVS_TOOLBAR_BOOKMARK       = 0x00000004;   // 书签
const IVS_UINT32 IVS_TOOLBAR_ZOOM           = 0x00000008;   // 局部放大
const IVS_UINT32 IVS_TOOLBAR_PLAY_RECORD    = 0x00000010;   // 即时回放
const IVS_UINT32 IVS_TOOLBAR_PLAY_SOUND     = 0x00000020;   // 声音
const IVS_UINT32 IVS_TOOLBAR_TALKBACK       = 0x00000040;   // 对讲
const IVS_UINT32 IVS_TOOLBAR_VIDEO_TVW      = 0x00000080;   // 视频上墙
const IVS_UINT32 IVS_TOOLBAR_ALARM_WIN      = 0x00000100;   // 设置告警窗口
const IVS_UINT32 IVS_TOOLBAR_PTZ            = 0x00000200;   // 云镜控制
const IVS_UINT32 IVS_TOOLBAR_IA             = 0x00000400;   // 叠加智能分析
const IVS_UINT32 IVS_TOOLBAR_OPEN_MAP       = 0x00000800;   // 打开电子地图
const IVS_UINT32 IVS_TOOLBAR_WINDOW_MAIN    = 0x00001000;   // 一主多辅
const IVS_UINT32 IVS_TOOLBAR_PLAY_QUALITY   = 0x00002000;   // 网速优先/画质优先
const IVS_UINT32 IVS_TOOLBAR_PTZ3D          = 0x00004000;   // 3D定位
const IVS_UINT32 IVS_TOOLBAR_EXIT_PLAY_RECORD= 0x00008000;   // 退出即时回放，返回正常播放

// OCX智能分析工具条
const IVS_UINT32 IVS_IA_TOOLBAR_DRAW_RECT       = 0x00000001;   // 矩形
const IVS_UINT32 IVS_IA_TOOLBAR_DRAW_POLYGON    = 0x00000002;   // 多边形
const IVS_UINT32 IVS_IA_TOOLBAR_ZOOM_IN         = 0x00000004;   // 放大
const IVS_UINT32 IVS_IA_TOOLBAR_ZOOM_OUT        = 0x00000008;   // 缩小
const IVS_UINT32 IVS_IA_TOOLBAR_DRAW_MOVE       = 0x00000010;   // 移动
const IVS_UINT32 IVS_IA_TOOLBAR_DISPLAY_MODE    = 0x00000020;   // 屏幕视频显示方式
const IVS_UINT32 IVS_IA_TOOLBAR_PLAY            = 0x00000040;   // 播放视频
const IVS_UINT32 IVS_IA_TOOLBAR_DELETE          = 0x00000080;   // 删除

// 录像状态
const IVS_UINT32 IVS_RECORD_STATE_MANUAL        = 0x00000001;   // 手动录像
const IVS_UINT32 IVS_RECORD_STATE_ALARM         = 0x00000002;   // 告警录像
const IVS_UINT32 IVS_RECORD_STATE_PLAN          = 0x00000004;   // 计划录像

// 视频轮巡窗口按钮状态
const IVS_UINT32 IVS_CAMERA_SWITCH_BUTTON_NULL  = 0x00000000;   // 无轮巡快速切换按钮
const IVS_UINT32 IVS_CAMERA_SWITCH_BUTTON_LEFT  = 0x00000001;   // 轮巡快速切换左按钮
const IVS_UINT32 IVS_CAMERA_SWITCH_BUTTON_RIGHT = 0x00000002;   // 轮巡快速切换右按钮

// 云镜控制码
typedef enum
{
    // 云台转动控制，包括停止、上、下、左、右等
    PTZ_STOP = 1,
    PTZ_UP,
    PTZ_DOWN,
    PTZ_LEFT,
    PTZ_UP_LEFT,
    PTZ_DOWN_LEFT,
    PTZ_RIGHT,
    PTZ_UP_RIGHT,
    PTZ_DOWN_RIGHT,
    PTZ_AUTO,
    PTZ_PREFAB_BIT_RUN,
    PTZ_CRUISE_RUN,
    PTZ_CRUISE_STOP,
    PTZ_MODE_CRUISE_RUN,
    PTZ_MODE_CRUISE_STOP,
    PTZ_MENU_OPEN,
    PTZ_MENU_EXIT,
    PTZ_MENU_ENTER,
    PTZ_FLIP,
    PTZ_START,
    // 镜头控制，包括光圈、缩放、聚焦
    PTZ_LENS_APERTURE_OPEN,
    PTZ_LENS_APERTURE_CLOSE,
    PTZ_LENS_ZOOM_IN,
    PTZ_LENS_ZOOM_OUT,
    PTZ_LENS_FOCAL_NEAT,
    PTZ_LENS_FOCAL_FAR,
    PTZ_AUX_OPEN,
    PTZ_AUX_STOP,
    // 模式
    MODE_SET_START,
    MODE_SET_STOP,
    // 增强操作
    PTZ_FAST_LOCATE,
    PTZ_HORIZONTAL_SCAN,
    PTZ_VERTICAL_SCAN,
    PTZ_LOCK,
	PTZ_UNLOCK,
	PTZ_ABSOLUTE_MOVE,
	PTZ_DIRECTION_MOVE
	//end
}IVS_PTZ_CODE;

//摄像机云台位置信息
typedef struct{
	IVS_DOUBLE dX;		//摄像机云台水平位置
	IVS_DOUBLE dY;		//摄像机云台垂直位置
	IVS_DOUBLE dZ;		//摄像机变倍倍数
}IVS_PTZ_POSITION_INFO;

//摄像机云台位置范围和运动速率范围信息
typedef struct{
	IVS_DOUBLE dXMax;		//摄像机云台水平位置最大值
	IVS_DOUBLE dXMin;		//摄像机云台水平位置最小值
	IVS_DOUBLE dYMax;		//摄像机云台垂直位置最大值
	IVS_DOUBLE dYMin;		//摄像机云台垂直位置最小值
	IVS_DOUBLE dZMax;		//摄像机变倍倍数最大值
	IVS_DOUBLE dZMin;		//摄像机变倍倍数最小值
	IVS_DOUBLE dVXMax;		//摄像机云台水平速度最大值
	IVS_DOUBLE dVXMin;		//摄像机云台水平速度最小值
	IVS_DOUBLE dVYMax;		//摄像机云台垂直速度最大值
	IVS_DOUBLE dVYMin;		//摄像机云台垂直速度最小值
	IVS_DOUBLE dVZMax;		//摄像机变倍速度最大值
	IVS_DOUBLE dVZMin;		//摄像机变倍速度最小值
}IVS_PTZ_RANGE_INFO;

//摄像机云台绝对位置信息
typedef struct{
	IVS_CHAR cCameraCode[IVS_DEV_CODE_LEN];		//摄像机编码
	IVS_PTZ_POSITION_INFO stPTZPosition;		//摄像机云台位置信息
	IVS_PTZ_RANGE_INFO stPTZRange;				//摄像机云台位置范围和运动速率范围信息
}IVS_PTZ_ABSPOSITION;
//end

// OCX窗口工具栏图标状态
// 巡航、告警、手动录像、单帧
typedef enum
{
    OCX_TITLEBAR_CRUISE = 0,  // 巡航
    OCX_TITLEBAR_WARNING= 1,  // 告警
    OCX_TITLEBAR_RECORD = 2,  // 手动录像
    OCX_TITLEBAR_FRAME  = 3,  // 单帧
}IVS_TITLEBAR_ICON;

// OCX窗口工具栏按钮状态
typedef enum
{
    OCX_TOOLBAR_STATUS_DISABLE= 0,  // 置灰
    OCX_TOOLBAR_STATUS_NORMAL,      // 正常
}IVS_TOOLBAR_STATUS;

// 分页信息
typedef struct
{
	IVS_UINT32 uiFromIndex; // 开始索引
	IVS_UINT32 uiToIndex;   // 结束索引
}IVS_INDEX_RANGE;

// IP类型
typedef enum
{
    IP_V4= 0,
    IP_V6= 1
}IVS_IP_TYPE;

// IP信息
typedef struct
{
	IVS_UINT32	uiIPType;				// IP类型：参考 IVS_IP_TYPE
	IVS_CHAR	cIP[IVS_IP_LEN];		// IP地址
}IVS_IP;

typedef struct  
{
    IVS_UINT32	uiCUType;// 客户端类型：5-PC客户端 6-iPad 7-AndroidPhone 8-AndroidPad 9-iPhone
    IVS_CHAR	   cCUVersion[IVS_VERSION_LEN];// 客户端版本号
    IVS_CHAR     cReserve[64];// 保留字段
}IVS_CU_INFO;

// 客户端类型
typedef enum
{
	CLIENT_PC       =0, // PC客户端
	CLIENT_WEB      =1, // web浏览器
	CLIENT_MOBILE   =2, // 移动客户端
	CLIENT_PC_CHILD =3, // PC客户端子进程
    CLIENT_TV_WALL  =4 // 电视墙进程
}IVS_CLIENT_TYPE;

//升级信息
typedef struct
{
    IVS_BOOL	bIsUpgrade;//是否需要升级，0-否，1-是
    IVS_CHAR	cSetupFileMd5[IVS_MD5_LEN];// 安装压缩包MD5值,ios客户端时为空
    IVS_UINT64      uiFileSize;     // 安装压缩包大小，字节数，ios客户端时为空
    // IVS_CHAR        cIosSetupUrl[IVS_URL_LEN];// ios客户端下载URL
    IVS_CHAR        cHttpURL[IVS_URL_LEN];//上传下载文件流URL信息
    // IVS_CHAR        cToken[IVS_TOKEN_LEN];//用户凭证
    IVS_CHAR        cReserve[64];// 保留字段
}IVS_UPGRADE_INFO;

// 登陆类型
typedef enum
{
	LOGIN_BASIC= 0,			// 基本身份验证
	LOGIN_WINDOWS_CURRENT,  // Windows域身份验证—当前用户
	LOGIN_WINDOWS_OTHER     // Windows域身份验证—其他用户
}IVS_LOGIN_TYPE;

// 用户登录信息
typedef struct
{
	IVS_CHAR cUserName[IVS_NAME_LEN];		        // 用户名
	IVS_CHAR pPWD[IVS_PWD_LEN];                     // 密码明文，登录类型为0（基本账号）时，不能为空

	IVS_IP	stIP;			                        // 登录服务器IP
	IVS_UINT32 uiPort;		                        // 登录服务器端口

	IVS_UINT32 uiLoginType;							// 登录类型：参考 IVS_LOGIN_TYPE 0–基本身份验证,	1–Windows域身份验证—当前用户，2–Windows域身份验证—其他用户
	IVS_CHAR cDomainName[IVS_DOMAIN_LEN];			// windows域名
	IVS_CHAR cMachineName[IVS_MACHINE_NAME_LEN];	// PC机器名

    IVS_UINT32 uiClientType;						// 客户端类型：参考 IVS_CLIENT_TYPE 0-PC客户端, 1-web浏览器, 2-移动客户端, 3-PC客户端子进程，4-电视墙进程
    IVS_CHAR cReserve[32];                          //保留字段
}IVS_LOGIN_INFO;

// 用户单点登录信息;
typedef struct
{
	IVS_CHAR cTicketID[IVS_TICKET_LEN];		        // TicketID;

	IVS_IP	stIP;			                        // 登录服务器IP;
	IVS_UINT32 uiPort;		                        // 登录服务器端口;

	IVS_UINT32 uiLoginType;							// 登录类型：参考 IVS_LOGIN_TYPE 0–基本身份验证,	1–Windows域身份验证—当前用户，2–Windows域身份验证—其他用户;
	IVS_CHAR cDomainName[IVS_DOMAIN_LEN];			// windows域名;
	IVS_CHAR cMachineName[IVS_MACHINE_NAME_LEN];	// PC机器名;

	IVS_UINT32 uiClientType;						// 客户端类型：参考 IVS_CLIENT_TYPE 0-PC客户端, 1-web浏览器, 2-移动客户端, 3-PC客户端子进程，4-电视墙进程;
	IVS_CHAR cReserve[32];                          //保留字段;
}IVS_LOGIN_INFO_EX;


// 在线用户信息
typedef struct
{
	IVS_UINT32	uiUserID;			                // 用户ID
	IVS_UINT32	uiUserOnlineID;		                // 在线用户信息ID （一个用户名可以多次同时登录）
    IVS_CHAR    cUserName[IVS_NAME_LEN];            // 登录账号
   	IVS_CHAR    cDomainName[IVS_DOMAIN_LEN];		// 登录域名信息
    IVS_UINT32  uiClientType;					    // 登录客户端类型 值参考 IVS_CLIENT_TYPE
    IVS_IP	    stIP;			                    // 登录的客户端IP
    IVS_CHAR    cMachineName[IVS_MACHINE_NAME_LEN];	// PC机器名
	IVS_CHAR	cLoginTime[IVS_TIME_LEN];	        // 用户登录时间
    IVS_CHAR    cReserve[64];                       // 保留字段
}IVS_ONLINE_USER;

typedef struct
{
	IVS_UINT32   uiTotal;               // 总记录数
	IVS_INDEX_RANGE stIndexRange;       // 分页信息
	IVS_ONLINE_USER stOnlineUser[1];    // 在线用户信息
}IVS_ONLINE_USER_LIST;

// 密码复杂性要求
typedef struct
{
	IVS_BOOL	bCapital;		// 大写字母，是否选中
	IVS_BOOL	bLowercase;		// 小写字母，是否选中
	IVS_BOOL	bFigure;		// 数字，是否选中
	IVS_BOOL	bSpecialChar;	// 特殊字符，是否选中
    IVS_CHAR    cReserve[32];   // 保留字段
}IVS_PWD_CONSIST;

// 账户规则
typedef struct
{
	IVS_BOOL	bEnable;			// 是否启用
	IVS_PWD_CONSIST	stPswConsist;	// 密码复杂性要求
	IVS_BOOL	bFirstModify;		// 首次登录修改密码，是否选中
	IVS_BOOL	bCheckOldPwd;		// 修改密码时验证旧密码，是否选中
	IVS_BOOL	bAccountNotFit;		// 口令不能和帐号一样，是否选中
	IVS_BOOL	bAccountBackNotFit;	// 口令不能和帐号的倒写一样，是否选中
	IVS_BOOL	bDomainAccCheckPwd;	// Windows本地账号登录需要输入密码，是否选中
	IVS_UINT32	uiPwdMinLen;        // 密码长度最小值，数值范围在6-64位。
	IVS_UINT32	uiPwdDifferentNum;	// 新旧密码不相同字符，数值范围在0-密码长度
	IVS_UINT32	uiAccountLockNum;	// 账号锁定阈值，0表示无锁定限制，最大999
	IVS_UINT32	uiAccountLockElapse;// 账号锁定时长，数值范围在1-1440分钟

    IVS_UINT32	uiAccountLockInterval;  // 账号锁定间隔，数值范围在0-999分钟，0表示不自动清零，锁定后需要管理员手动解锁
    IVS_UINT32	uiPWDExpireTime;        // 密码过期时间，密码创建或修改后开始计时，单位：天，范围：0-999
    IVS_UINT32	uiPWDExpireNotifyTime;  // 密码过期提醒时间，密码过期时间到达前开始提醒，单位：天，范围：0-99
    IVS_BOOL	bPWDExpireForceNotify;  // 密码过期强制修改，是否选中。0-不强制修改，1-强制修改
    IVS_CHAR    cReserve[64];           // 保留字段
}IVS_ROLE_ACCOUNT_RULE;

typedef enum
{
	// 默认管理角色
	OR_MANAGER_DEVICE= 1,		// 设备管理：设备的增、删、改、查和子设备启动、设备组管理、设备参数配置（包含：高级参数配置（码率、分辨率、重启等）；基本参数配置（OSD、亮度、对比度等））
	OR_MANAGER_USER,			// 用户管理：用户管理、角色管理、权限管理、用户签退、告警订阅（其他用户）
	OR_MANAGER_RECORD,			// 录像管理：录像空间、计划配置、预录、补录、录像归档管理（视频归档）
	OR_MANAGER_RECORD_BACKUP,	// 录像备份管理：平台、前端录像备份管理、配置备份策略、手动录像备份
	OR_MANAGER_IA,				// 智能分析管理：智能分析规则设置
	//OR_MANAGER_STORAGE,		// 存储管理：存储空间管理
	OR_MANAGER_ALARM=7,			// 告警管理：告警订阅、联动策略、防区管理、告警级别
	OR_MANAGER_NET,				// 网管：客户端网管（含设备类告警通知）
    OR_RECORD_LOCK,				// 录像锁定管理
    OR_RECORD_BOOKMARK=10,      // 录像书签管理
	//OR_TRANSPARENT_CHANNEL,	// 透明通道
    OR_MANAGER_OUT_DOMAIN=12,   // 外域管理权限
    OR_MANAGER_MAP,             // 电子地图管理权限

    // 特殊业务角色
    OR_TV_WALL_HANDLE=20,       // 电视墙：上墙实况、上墙回放
    OR_ALARM_HANDLE,		    // 告警处理：告警确认、查询告警、导出告警、撤销、授权告警、本地告警参数配置
    OR_IA_HANDLE,			    // 智能分析操作
    OR_DEFENCE_HANDLE,		    // 防区布撤防
    OR_WATERMARK_HANDLE,	    // 水印篡改校验
    OR_MOBILE_CAMERA,	        // 手机模拟前端权限

    // 一般业务角色
    OR_SNAPSHOT_MANAGER=40,	    // 平台抓拍管理权限（配置、下载、预览、删除）
    OR_LOG_QUERY,			    // 日志查询、导出
    OR_MAP,					    // 电子地图操作
    OR_REALPLAY,			    // 软解、叠加智能分析结果、局部放大、轮巡/组轮巡、轮巡计划、布局管理、场景管理
    OR_RECORD_PLAY,				// 平台、前端录像回放、局部放大、同步回放、录像检索（包括备份录像）、VCR控制、录像书签
    OR_RECORD_DOWNLOAD,			// 平台、前端录像下载、录像检索
    OR_VOICE,				    // 语音：对讲和广播
    OR_PTZ,					    // 云台控制：云台预置位、巡航、云台控制、看守位设置、云台加解锁、3D定位、本地云台参数配置
    OR_RECORD_MANUAL,			// 手动录像：平台、前端、本地手动录像
    OR_SNAPSHOT_LOCAL,		    // 抓拍操作(包括本地、平台)
}IVS_OPERATION_RIGHT;

// 平台支持能力集信息
#define IVS_ABILITY_IA_FR   0x01    // 智能分析-人脸
#define IVS_ABILITY_IA_BA   0x02    // 智能分析-行为
#define IVS_ABILITY_IA_QD   0x04    // 智能分析-质量诊断

// 权限组信息
#define IVS_MAX_RIGHT_GROUP_NAME_LEN	64	// 权限组最大英文名长度（多语言由客户端自己映射实现）
typedef struct
{
    IVS_CHAR	cRightGroupName[IVS_MAX_RIGHT_GROUP_NAME_LEN];	// 权限组名称
    IVS_CHAR	cRight[IVS_MAX_OPERATION_RIGHT];                // 角色权限数组：0-没权限，1-有权限，权限值参考 IVS_OPERATION_RIGHT
    IVS_CHAR    cReserve[32];                                   // 保留字段
}IVS_OPERATION_RIGHT_GROUP;

// 权限组列表
#define IVS_MAX_RIGHT_GROUP_NUM	8	// 权限组最大个数
typedef struct
{
    IVS_UINT32   uiTotal;           // 总记录数
    IVS_INDEX_RANGE stIndexRange;   // 分页信息
    IVS_OPERATION_RIGHT_GROUP stOperationRightGroup[1];    // 权限组信息
}IVS_OPERATION_RIGHT_GROUP_LIST;

// 角色类型
typedef enum
{
    ROLE_TYPE_CUSTOM=0,     // 用户自定义角色
    ROLE_TYPE_DEFAULT=1,    // 系统预置默认角色
    ROLE_TYPE_ADMIN=2,      // 超级管理员角色
}IVS_ROLE_TYPE;

// 角色
#define IVS_MAX_ROLE_INFO_NUM   128 // 最大角色个数
typedef struct
{
    IVS_CHAR	cDomainCode[IVS_DOMAIN_CODE_LEN];	// 角色所属域编码
	IVS_UINT32	uiRoleID;                           // 角色ID
	IVS_CHAR	cRoleName[IVS_NAME_LEN];            // 角色名称
	IVS_CHAR	cRoleDescribe[IVS_DESCRIBE_LEN];    // 角色描述
    IVS_UINT32	uiRoleType;                         // 角色类型：值参考 IVS_ROLE_TYPE
	IVS_CHAR	cRight[IVS_MAX_OPERATION_RIGHT];    // 角色权限数组：0-没权限，1-有权限，权限值参考 IVS_OPERATION_RIGHT
	IVS_ROLE_ACCOUNT_RULE	stAccountRule;          // 账号规则ID：参考 IVS_ROLE_ACCOUNT_RULE
    IVS_CHAR    cReserve[64];                       // 保留字段
}IVS_ROLE_INFO;

typedef struct
{
	IVS_UINT32   uiTotal;           // 总记录数
	IVS_INDEX_RANGE stIndexRange;   // 分页信息
	IVS_CHAR cReserve[32];          // 保留字段
	IVS_ROLE_INFO stRoleInfo[1];    // 角色信息
}IVS_ROLE_INFO_LIST;

// 用户组
#define IVS_MAX_USER_GROUP_LEVEL	    4	// 用户组层级最大个数
#define IVS_MAX_USER_GROUP_DESC_LEN 	256	// 用户组备注
typedef struct
{
    IVS_CHAR	cDomainCode[IVS_DOMAIN_CODE_LEN];	// 用户组所属域编码
	IVS_UINT32	uiID;					            // 用户组ID
	IVS_CHAR	cName[IVS_NAME_LEN];	            // 用户组名称
	IVS_UINT32	uiParentID;				            // 父用户组ID
	IVS_UINT32	uiLevel;				            // 用户组层级，最多支持4层（包含根组在内）
    IVS_CHAR	cGroupDesc[IVS_MAX_USER_GROUP_DESC_LEN];	// 用户组备注，键盘可见字符和中文，0~64字符。值可以为空
    IVS_CHAR    cReserve[32];                       // 保留字段
}IVS_USER_GROUP;

//用户组列表
typedef struct
{
	IVS_UINT32   uiTotal;           // 总记录数
	IVS_INDEX_RANGE stIndexRange;   // 分页信息
	IVS_CHAR cReserve[32];          //保留字段
	IVS_USER_GROUP stUserGroup[1];  //用户组列表
}IVS_USER_GROUP_LIST;

// 账号状态
typedef enum
{
	ACCOUNT_STATUS_ACTIVE= 1,           // 激活
	ACCOUNT_STATUS_STOPPED= 2,          // 停用
	ACCOUNT_STATUS_INVALID_LOGIN= 3,    // 无效登录锁定
}IVS_ACCOUNT_STATUS;

// 用户信息
typedef struct
{
    IVS_CHAR	cDomainCode[IVS_DOMAIN_CODE_LEN];	// 用户所属域编码
	IVS_UINT32	uiUserID;						// 用户ID，系统分配
	IVS_BOOL	bFirstLogin;					// 是否是第一次登录
	// 添加用户需要的信息
	IVS_UINT32	uiRoleID;						// 角色ID，管理员配置：结构定义参考 IVS_USER_ROLE
	IVS_UINT32	uiGroupID;						// 用户组ID：结构定义参考 IVS_USER_GROUP
	IVS_CHAR	cLoginName[IVS_NAME_LEN];		// 登录账号：登录账号，字母（a-z和A-Z）,数字，中划线和下划线，1~20个字符

	IVS_CHAR	cPWD[IVS_PWD_LEN];	// 登录密码：登录密码，键盘可见字符，1~16个字符。
	IVS_CHAR	cPwdModifyDate[IVS_TIME_LEN];	// 密码修改日期

	IVS_UINT32	uiUserType;						// 用户类型：0：IVS普通用户，1：Windows域用户
	IVS_CHAR	cUserDomain[IVS_DOMAIN_LEN];	// Windows域名
	IVS_UINT32	uiStatus;						// 用户状态：参考 IVS_ACCOUNT_STATUS
	IVS_CHAR	cRegiterDate[IVS_TIME_LEN];		// 创建日期
	IVS_CHAR	cUserDesc[IVS_DESCRIBE_LEN];	// 用户描述，键盘可见字符和中文，0~64字符。
	IVS_UINT32	uiPTZLevel;						// 云镜控制优先级：0~9
	IVS_UINT32	uiMaxSessionCnt;				// 多点登录数,最大长度3字符，数字
	IVS_UINT32	uiMaxVideoCnt;					// 最大视频路数，最大长度3字符，数字 
	IVS_CHAR	cLockTime[IVS_TIME_LEN];		// 账号被锁定的时间

	// 个人基本信息，用于个人修改
	IVS_CHAR	cUserName[IVS_NAME_LEN];		// 真实姓名，汉字和字母（a-z和A-Z），数字，中划线和下划线，1~20个字符
	IVS_CHAR	cEmail[IVS_EMAILE_LEN];			// 邮箱，参考网易163邮箱限制。
	IVS_CHAR	cPhone[IVS_PHONE_NUMBER_LEN];	// 电话，0~25位数字，中划线
	IVS_UINT32	uiSex;							// 性别：0-女性，1-男性
    IVS_UINT32	uiValidDateFlag;        //是否有账户有效时间
    IVS_CHAR	cValidDateStart[IVS_TIME_LEN];   //账户有效起始日期
    IVS_CHAR	cValidDateEnd[IVS_TIME_LEN];     //账户有效结束日期

    IVS_CHAR    cReserve[64];                       // 保留字段
}IVS_USER_INFO;

typedef struct
{
	IVS_UINT32   uiTotal;           // 总记录数
	IVS_INDEX_RANGE stIndexRange;   // 分页信息
	IVS_CHAR cReserve[32];          //保留字段
	IVS_USER_INFO stUserInfo[1];    // 用户信息
}IVS_USER_INFO_LIST;

// 设备用户信息
typedef struct
{
    IVS_UINT32	uiUserID;						// 用户ID，系统分配
    IVS_CHAR	cLoginName[IVS_NAME_LEN];		// 登录账号：登录账号，字母（a-z和A-Z）,数字，中划线和下划线，1~20个字符
    IVS_CHAR	cUserName[IVS_NAME_LEN];		// 真实姓名，汉字和字母（a-z和A-Z），数字，中划线和下划线，1~20个字符
	IVS_UINT32	uiGroupID;						// 用户组ID：结构定义参考 IVS_USER_GROUP
    IVS_CHAR	cReserve[8];
}IVS_DEVICE_USER_INFO;

// 设备用户列表
typedef struct
{
    IVS_UINT32   uiTotal;           // 总记录数
    IVS_INDEX_RANGE stIndexRange;   // 分页信息
    IVS_DEVICE_USER_INFO stDeviceUserInfo[1];    // 设备用户信息
}IVS_DEVICE_USER_INFO_LIST;

// 查询的条件类型
typedef enum
{
	QUERY_INVALID=0,	                // 查询条件无

    QUERY_TIME=1,	                    // 默认按照数据库记录生成时间进行排序

    QUERY_USER_ID=100,                  // 用户ID
    QUERY_LOGIN_NAME,                   // 用户的登录账号
    QUERY_USER_NAME,                    // 用户真实姓名
    QUERY_USER_STATUS,                  // 用户状态
    QUERY_USER_NAME_TYPE,               // 用户状态

    QUERY_CAMERA_CODE=200,              // 摄像机编码

	QUERY_DEV_NVR_CODE=300,				// 设备所属NVR编码
	QUERY_DEV_DEV_NAME,				    // 设备名称
	QUERY_DEV_TYPE,					    // 具体设备类型
	QUERY_DEV_VENDOR_TYPE,			    // 具体厂商类型
	QUERY_DEV_IP,					    // 具体的设备IP地址
	QUERY_DEV_ONLINE,				    // 是否在线
    QUERY_DEV_SN,				        // 设备SN
    QUERY_DEV_MODEL_TYPE,               // 设备SN

    QUERY_DEV_GROUP_CODE=500,             // 设备组编码
    QUERY_DEV_DOMAIN_CODE,              // 设备所属域编码

    QUERY_RECORD_QUERY_TYPE=600,        // 录像查询方式：0-按时间，1-按书签，2-按预置位，3-按锁定信息
	QUERY_RECORD_BOOKMARK_NAME,			// 书签名，模糊匹配
	QUERY_RECORD_PTZ_PRESET_NAME,		// 预置位名称，模糊匹配，查询预置位查询录像时有效
	QUERY_RECORD_LOCK_DESC,				// 锁定描述，模糊匹配，查询锁定录像时有效
    QUERY_RECORD_LOCK_OPERATOR_TYPE,	// 锁定操作用户类型（查询锁定录像时有效），0-所有用户，1-当前用户
	QUERY_RECORD_METHOD,    	        // 按存储位置查询录像：值参考 IVS_RECORD_METHOD
	QUERY_RECORD_TYPE,			        // 值参考 IVS_RECORD_TYPE
    QUERY_RECORD_ALARM_TYPE,            // 录像告警类型
    QUERY_RECORD_USER_DOMAIN,           // 录像操作用户域（录像检索、书签、锁定等）

    QUERY_OPERATION_ACCOUNT=700,        // 管理员账号
    QUERY_OPERATION_RESULT,             // 操作结果：0-成功，1是失败
    QUERY_OPERATION_START_TIME,         // 开始时间，yyyyMMddhhmmss，查询操作时间
    QUERY_OPERATION_END_TIME,           // 结束时间，yyyyMMddhhmmss，查询操作时间
    QUERY_OPERATION_CODE,               // 操作功能代码
    QUERY_OPERATION_OBJECT_CODE,        // 操作对象编码

    QUERY_ALARM_CATEGORY=800,           // 告警类型级别
    QUERY_ALARM_TYPE,                   // 告警类型，条件必选（例如录像查询，为0表示所有）
    QUERY_ALARM_IN_TYPE,                // 告警源类型
    QUERY_ALARM_LEVEL_ID,               // 告警级别

    QUERY_ALARM_IN_CODES,               // 告警源编码；多选，用逗号“,”分隔；无此条件或为空表示所有
    QUERY_ALARM_IN_NAME,                // 告警源名称
    QUERY_ALARM_LEVEL_MIN,              // 告警级别下限
    QUERY_ALARM_LEVEL_MAX,              // 告警级别上限
    QUERY_ALARM_LEVEL_VALUE,            // 告警级别：同时传递多个告警级别，以逗号“,”隔开
    QUERY_ALARM_STATUS,                 // 告警信息状态；多选，按“,”分割；无此条件或为空表示所有

    QUERY_ALARM_LOCATION_INFO,          // 告警定位信息
    QUERY_ALARM_CONFIRM_OPERATOR,       // 确认人ID；多选，按“,”分割；无此条件或为空表示所有
    QUERY_ALARM_DESC,                   // 告警源描述，模糊查询
    QUERY_ALARM_TYPE_LIST,              // 告警类型列表，用“,”分割，可以表示同时查询多个告警类型，例如录像查询中
    QUERY_ALARM_TYPE_FILTER,            // 需要过滤的告警类型类别，以逗号隔开，010201：行为分析；    010202：人脸识别；    010203：质量诊断

    QUERY_DICTIONARY_TYPE=1000,         // 字典类型，精确查询，必选
    QUERY_DICTIONARY_NAME,              // 字典名称，精确查询，可选
    QUERY_DICTIONARY_PARENT_ID,         // 父节点ID，精确查询，可选

    QUERY_OPERATOR_ID=1100,             //执行查询操作的用户ID
    QUERY_NODE_CODE,				    // 网元编码(如果一次查一个网元的，该字段必填)

    QUERY_WATERMARK_ALARM_STATUS=1300,  // 水印告警状态：0x01-未确认 0x02-已确认 0x04-已撤销；异或值表示多选，如0x07表示查询三种状态，0x06表示查询已确认和已撤销；
    QUERY_WATERMARK_USERID_DOMAIN,      // 当前用户所属域编码

    // 外域目录共享
    QUERY_SHARE_EXDOMAIN_CODE=1400,     // 互联域编码，精确查询，必选
    QUERY_SHARE_DIRECT,                 // 共享方向，精确查询，必选：1-本域共享给外域 2-外域共享给本域
    // DeviceType 共用 QUERY_DEV_TYPE   // 共享设备资源类型，精确查询：00：主设备 01：摄像机 02：报警输入设备 03：报警输出设备 99：区域
    QUERY_SHARE_PARENT_ID,              // 父设备编码，精确查询，
    QUERY_SHARE_DEVICE_ID,              // 设备互联编码，精确查询，
    // DevName    共用 QUERY_DEV_DEV_NAME // 设备名称，模糊查询
    QUERY_SHARE_ISLATEST,               // 是否查询最新：0-否，只查询库中的数据 1-是，与外域进行同步后，查询最新的数据

}IVS_QUERY_FIELD_TYPE;

//查询的结构体
#define IVS_QUERY_VALUE_LEN 2048    // 查询字段大小
typedef struct
{
    IVS_UINT32  eFieID;                         //查询字段的类型 参考 IVS_QUERY_FIELD_TYPE
    IVS_CHAR    cValue[IVS_QUERY_VALUE_LEN];    //字段的值
	IVS_BOOL    bExactQuery;                    // 是否精确查询：0-否，1-是
}IVS_QUERY_FIELD;

//排序的结构体
typedef struct
{
	IVS_BOOL    bEnableOrder;       // 是否启用排序 0 - 不排序 1 - 排序
	IVS_UINT32  eFieID;             //排序的字段 参考 IVS_QUERY_FIELD_TYPE
    IVS_BOOL    bUp;                // 是否升序：0-降序，1-升序
}IVS_ORDER_COND;

// 查询通用格式
typedef struct 
{
    IVS_INDEX_RANGE stIndex;    // 查询范围索引
	IVS_ORDER_COND stOrderCond; // 排序信息
    IVS_INT32 iFieldNum;        // 字段的数量为0表示无查询限制条件
    IVS_QUERY_FIELD stQueryField[1];
}IVS_QUERY_UNIFIED_FORMAT;

// 设备操作结果
typedef struct
{
	IVS_UINT32	uiSequence;             // 批量操作序号
	IVS_INT32	iResult;                // 错误返回代码（批量添加时）
	IVS_CHAR cCode[IVS_DEV_CODE_LEN];   // 设备编码
}IVS_RESULT_INFO;

// 设备批量操作结果列表
typedef struct
{
	IVS_UINT32	uiNum;          // 返回的操作结果个数
	IVS_RESULT_INFO	stInfo[1];
}IVS_RESULT_LIST;

// 摄像机计划信息
#define IVS_MAX_CAMERA_PLAN_NUM     64  // 摄像机最大计划种类数
typedef struct
{
    IVS_CHAR	cCameraCode[IVS_DEV_CODE_LEN];  // 摄像头编码
    IVS_INT32	iResult;                        // 错误返回代码（批量时，确认每个摄像机是否成功返回）
    IVS_BOOL    bSetPlan;                       // 是否设置计划 0-未设置 1-已设置
    IVS_CHAR    cReserve[32];                   // 保留字段
}IVS_CAMERA_PLAN_SET_INFO;

// 摄像机计划信息列表
typedef struct
{
    IVS_UINT32	uiNum;                      // 返回的操作结果个数
    IVS_CAMERA_PLAN_SET_INFO    stInfo[1];  // 摄像机计划信息列表
}IVS_CAMERA_PLAN_SET_INFO_LIST;

// 录像计划日类型
typedef enum
{
	DAY_TYPE_EVERYDAY = 0,	// 每天（所有天指定相同时间段录像）
	DAY_TYPE_MONDAY=1,		// 周一
	DAY_TYPE_TUESDAY,		// 周二
	DAY_TYPE_WEDNESDAY,		// 周三
	DAY_TYPE_THURSDAY,		// 周四
	DAY_TYPE_FRIDAY,		// 周五
	DAY_TYPE_SATURDAY,		// 周六
	DAY_TYPE_SUNDAY,		// 周日
}IVS_DAY_TYPE;

// 回放时间信息
typedef struct
{
    IVS_CHAR    cPlayBackTime[IVS_TIME_LEN];        // 当前回放时间，格式为yyyyMMddHHmmss
    IVS_CHAR    cPlayBackStartTime[IVS_TIME_LEN];   // 当前开始时间，格式为yyyyMMddHHmmss（本地文件年月日为空）
    IVS_CHAR    cPlayBackEndTime[IVS_TIME_LEN];     // 当前结束时间，格式为yyyyMMddHHmmss（本地文件年月日为空）
    IVS_CHAR    cReserve[64];                       // 保留字段
}IVS_PLAYBACK_TIME;

#define IVS_MAX_TIMESPAN_NUM    24  // 时间段最大数量
#define IVS_MAX_PLAN_INFO_NUM   7   // 最大计划信息个数（一周7天）
// 计划时间信息
typedef struct
{
	IVS_UINT32      uiDayType;								//星期几：参考 IVS_DAY_TYPE 0 – 每天 1 – 周一 2 – 周二 3 – 周三 4 – 周四 5 – 周五 6 – 周六 7 – 周日
	IVS_UINT32      uiTimeSpanInfoNum;						//时间段数量
    IVS_CHAR        cReserve[32];                           // 保留字段
	IVS_TIME_SPAN   stTimeSpanInfo[IVS_MAX_TIMESPAN_NUM];   //时间段信息
}IVS_PLAN_INFO;

// 录像计划类型
typedef enum
{
	RECORD_PLAN_WEEK=0,		// 周计划
	RECORD_PLAN_DAY=1,		// 日计划
	RECORD_PLAN_ALLDAY=2,	// 全程录像
}IVS_RECORD_PLAN_TYPE;

// 录像存储位置
typedef enum
{
	RECORD_METHOD_PLATFORM = 0,	// 平台录像
	RECORD_METHOD_PU,			// 前端录像
    RECORD_METHOD_MBU,          // 备份服务器
    RECORD_METHOD_DISASTER_RECOVERY,    // 容灾录像
}IVS_RECORD_METHOD;

// 录像计划
typedef struct
{
	IVS_UINT32	uiRecordMethod;         // 录像存储位置：参考 IVS_RECORD_METHOD
	IVS_BOOL	bEnableRecordPlan;      // 是否启用录像计划 0-未启用，1-启用
	IVS_UINT32	uiRecordPlanType;		// 计划方式：参考 IVS_RECORD_PLAN_TYPE 0-(WEEK)周计划,1(DAY)-日计划，周一到周日计划相同，2-全程录像，
    IVS_CHAR    cReserve[64];           // 保留字段

	IVS_UINT32	uiPlanInfoNum;			// 录像计划信息条数：周计划-7，日计划-1，全程录像-0
	IVS_PLAN_INFO	stPlanInfo[IVS_MAX_PLAN_INFO_NUM];	// 录像计划信息：周计划-最多7个计划信息，日计划-1一个计划信息，全程录像-无
}IVS_RECORD_PLAN;

// 录像计划类型
typedef enum
{
	PLAN_WEEK=0,	// 周计划
	PLAN_DAY=1,		// 日计划
	PLAN_ALLDAY=2,	// 全程
}IVS_PLAN_TYPE;

// 防区布防计划
typedef struct
{
	IVS_UINT32		uiAlarmAreaID;          // 防区ID  
	IVS_BOOL		bEnablePlan;            // 是否启用录像计划 0-未启用，1-启用
	IVS_UINT32		uiRecordPlanType;		// 计划方式：参考 IVS_PLAN_TYPE
    IVS_CHAR        cReserve[64];           // 保留字段

	IVS_UINT32		uiPlanInfoNum;			// 计划信息条数：周计划-7，日计划-1，全程录像-0
	IVS_PLAN_INFO	stPlanInfo[IVS_MAX_PLAN_INFO_NUM];	// 录像计划信息：周计划-最多7个计划信息，日计划-1一个计划信息，全程录像-无
}IVS_ALARM_AREA_PLAN;

// 本地录像文件分割方式
typedef enum
{
    RECORD_SPLITE_TIME      =1,   // 按时间分割
    RECORD_SPLITE_CAPACITY  =2,   // 按容量分割
}IVS_RECORD_SPLITE_TYPE;

// 本地录像文件命名规则
typedef enum
{
    RULE_NAME_NUM_TIME=1,       // 摄像机名_序号_时间（开始时间-结束时间，YYYY-MM-DD-hh-mm-ss）
    RULE_NAME_TIME_NUM=2,       // 摄像机名_时间（开始时间-结束时间，YYYY-MM-DD-hh-mm-ss）_序号
    RULE_TIME_NAME_NUM=3,       // 时间（开始时间-结束时间，YYYY-MM-DD-hh-mm-ss）_摄像机名_序号
}IVS_RECORD_NAME_RULE;

// 本地录像文件格式
typedef enum
{
    IVS_FILE_MP4=1,       // MP4文件
    IVS_FILE_AVI=2,       // AVI文件
}IVS_RECORD_FILE_TYPE;

// 本地录像参数
#define IVS_MIN_TIME_SPLITTER_VALUE     5       // 最小时间分割值，5分钟
#define IVS_MAX_TIME_SPLITTER_VALUE     720     // 最大时间分割值，720分钟
#define IVS_MIN_SPACE_SPLITTER_VALUE    50      // 最小容量分割值，50M
#define IVS_MAX_SPACE_SPLITTER_VALUE    3072    // 最大容量分割值，3072M
#define IVS_MIN_RECORD_TIME             300     // 最小录像时长，300，单位秒
#define IVS_MAX_RECORD_TIME             43200   // 最大录像时长，43200，单位秒（12小时）
#define IVS_LOCAL_RECORD_PWD_LEN        36      // 本地录像密码最大字节数，包含'\0'
typedef struct
{
    IVS_UINT32  uiRecordFormat;                 // 录像文件格式，参考 IVS_RECORD_FILE_TYPE
    IVS_UINT32  uiSplitterType;                 // 录像分割方式，参考 IVS_RECORD_SPLITE_TYPE
    IVS_UINT32  uiSplitterValue;                // 录像分割值，文件分割方式是时间时，填入时间，对应单位为分钟，5-60分钟，同时满足文件大小不超过2048MB的限制，文件分割方式是容量时，填入容量，对应单位为M，10-2048MB

    IVS_UINT32  uiDiskWarningValue;             // 本地录像，磁盘空间小于此值告警，单位M（进行“本地录像通用事件上报”2-本地录像告警）
    IVS_UINT32  uiStopRecordValue;              // 本地录像，磁盘空间小于此值停止录像，单位M（进行“本地录像通用事件上报”3-本地录像磁盘满停止）

    IVS_UINT32  uiRecordTime;                   // 录像时长，单位秒，整数，300 ~ 43200（12小时）

    IVS_BOOL    bEncryptRecord;                 // 录像是否加密
    IVS_CHAR    cRecordPWD[IVS_LOCAL_RECORD_PWD_LEN];   // 本地录像密码

    IVS_UINT32  uiNameRule;                     // 录像文件命名规则，参考 IVS_RECORD_NAME_RULE
    IVS_CHAR    cSavePath[IVS_FILE_NAME_LEN];   // 本地录像存放路径，加上文件名长度不超过256字节
    IVS_CHAR    cReserve[64];                   // 保留字段
}IVS_LOCAL_RECORD_PARAM;

// 录像下载参数
typedef struct
{
    IVS_UINT32  uiRecordFormat;                 // 录像文件格式，参考 IVS_RECORD_FILE_TYPE
    IVS_UINT32  uiSplitterType;                 // 录像分割方式，参考 IVS_RECORD_SPLITE_TYPE
    IVS_UINT32  uiSplitterValue;                // 录像分割值，文件分割方式是时间时，填入时间，对应单位为分钟，5-60分钟，同时满足文件大小不超过2048MB的限制，文件分割方式是容量时，填入容量，对应单位为M，1-2048MB

    IVS_UINT32  uiDiskWarningValue;             // 磁盘空间小于此值告警，单位M（进行“本地录像通用事件上报”2-本地录像告警）
    IVS_UINT32  uiStopRecordValue;              // 磁盘空间小于此值停止录像，单位M（进行“本地录像通用事件上报”3-本地录像磁盘满停止）

    IVS_UINT32  uiDownloadSpeed;                // 录像下载速度：255-不限速（全速），1-1倍速（前端只支持：1）
    IVS_TIME_SPAN stTimeSpan;                   // 录像下载时间段

    IVS_BOOL    bEncryptRecord;                 // 录像下载是否加密
    IVS_CHAR    cRecordPWD[IVS_LOCAL_RECORD_PWD_LEN];   // 录像下载密码

    IVS_UINT32  uiNameRule;                     // 录像文件命名规则，参考 IVS_RECORD_NAME_RULE
    IVS_CHAR    cSavePath[IVS_FILE_NAME_LEN];   // 录像存放路径，加上文件名长度不超过256字节

    IVS_CHAR    cFileName[IVS_FILE_NAME_LEN];   // 本地录像文件名，绝对路径，长度为256字节（包括后缀和结束符），多份录像文件用序号区分，为空时，根据uiNameRule的命名规则和cSavePath的路径确认文件名
    IVS_CHAR    cReserve[64];                   // 保留字段
}IVS_DOWNLOAD_PARAM;

// 录像下载信息
typedef struct
{
    IVS_UINT32  uiDownloadStatus;   // 下载状态，0-暂停 1-下载
    IVS_UINT32  uiDownloadSpeed;    // 下载速度，单位KB/S
    IVS_UINT64  uiDownloadSize;     // 已下载大小，单位KB
    IVS_UINT32  uiTimeLeft;         // 剩余下载时间：单位秒
    IVS_UINT32  uiProgress;         // 当前下载进度：0-100，表示百分比
}IVS_DOWNLOAD_INFO;

// 媒体播放信息
typedef struct
{
	IVS_UINT32	uiWinID;                // 窗口ID
	IVS_UINT32	uiAudioStatus;          // 声音状态

	IVS_UINT32  uiVideoFrameRate;		// 视频当前帧率，单位(fps)，范围1-60

    IVS_UINT32	uiVideoBitrate;         //	视频当前码率
	IVS_UINT32	uiVideoAverageBitrate;  //	视频平均码率

	IVS_UINT32	uiAudioBitRate;			// 音频当前码率，单位(kbps)
	IVS_UINT32	uiAudioAverageBitRate;	// 音频平均码率，单位(kbps)

	IVS_UINT32	uiVideoCodec;           // 视频编码格式：参考 IVS_VIDEO_DEC_TYPE
	IVS_UINT32	uiVideoWidth;           // 视频画面宽
	IVS_UINT32	uiVideoHeight;          // 视频画面高
	IVS_FLOAT	fLossTolerance;         // 丢包率

	IVS_UINT32	uiInfoLocationX;        // i按钮在屏幕上的右下角X坐标
	IVS_UINT32	uiInfoLocationY;        // i按钮在屏幕上的右下角Y坐标

    IVS_IP      stMediaSourceIP;        // 媒体流发送端IP
    IVS_CHAR    cReserve[64];           // 保留字段
}IVS_MEDIA_INFO;

// 存储类型
typedef enum
{
	RECORD_MODE_FULL=0,					// 全量存储
	RECORD_MODE_FRAME_EXTRACTION=1,		// 抽帧存储
	RECORD_MODE_VIDEO_COMPRESS=2,		// 运动检测智能存储
}IVS_RECORD_MODE;

// 预录设置
typedef enum
{
	PRERECORD_MODE_NONE=0,	// 不预录
	PRERECORD_MODE_PLAT=1,	// 平台预录
	PRERECORD_MODE_PU=2,	// 前端预录
}IVS_PRERECORD_MODE;

// 录像策略（时间）
#define IVS_MAX_PRERECORD_TIME	300     // 最大预录时间，单位：秒

typedef struct
{
	IVS_UINT32	uiRecordMode;				// 存储类型：参考 IVS_RECORD_MODE

	IVS_UINT32	uiTime;						// 视频留存天数，单位：天
	IVS_UINT32	uiFrameExtractRecordTime;	// 抽帧存储天数，单位：天
	IVS_UINT32	uiAlarmTime;				// 有告警录像存储天数，单位：天
	IVS_UINT32	uiKeyframeTime;				// 关键帧天数，单位：天

	IVS_UINT32	uiBeforeAlarm;				// 运动检测警前时间，单位：秒（视频归档时有效）
	IVS_UINT32	uiAfterAlarm;				// 运动检测警后时间，单位：秒（视频归档时有效）

	IVS_UINT32	uiPlanStreamType;			// 计划录像使用的流：参考 IVS_STREAM_TYPE
	IVS_UINT32	uiAlarmStreamType;			// 告警录像使用的流：参考 IVS_STREAM_TYPE

	IVS_UINT32	uiAlarmRecordTTL;			// 告警录像留存期，非负整数，0–永久，	其他–留存期，单位：天
	IVS_UINT32	uiManualRecordTTL;			// 手动录像留存期，非负整数，0–永久，	其他–留存期，单位：天

	IVS_UINT32	uiPreRecord;				// 预录设置：参考 IVS_PRERECORD_MODE
	IVS_UINT32	uiPreRecordTime;			// 预录时长，单位：秒，范围(1～IVS_MAX_PRERECORD_TIME)

	IVS_BOOL	bAssociatedAudio;			// 随路音频叠加选项：	0-不叠加 1-叠加

	IVS_CHAR	cReserve[32];
}IVS_RECORD_POLICY_TIME;

// 录像策略（容量）
typedef struct
{
    IVS_UINT32	uiRecordMode;				// 存储类型：参考 IVS_RECORD_MODE

    IVS_UINT32	uiBeforeAlarm;				// 运动检测警前时间，单位：秒（视频归档时有效）
    IVS_UINT32	uiAfterAlarm;				// 运动检测警后时间，单位：秒（视频归档时有效）

    IVS_UINT32	uiPlanStreamType;			// 计划录像使用的流：参考 IVS_STREAM_TYPE
    IVS_UINT32	uiAlarmStreamType;			// 告警录像使用的流：参考 IVS_STREAM_TYPE

    IVS_UINT32	uiAlarmRecordTTL;			// 告警录像留存期，非负整数，0–永久，	其他–留存期，单位：天
    IVS_UINT32	uiManualRecordTTL;			// 手动录像留存期，非负整数，0–永久，	其他–留存期，单位：天

    IVS_UINT32	uiPreRecord;				// 预录设置：参考 IVS_PRERECORD_MODE
    IVS_UINT32	uiPreRecordTime;			// 预录时长，单位：秒，范围(1～IVS_MAX_PRERECORD_TIME)

    IVS_BOOL	bAssociatedAudio;			// 随路音频叠加选项：	0-不叠加 1-叠加

    IVS_UINT64	uiCapacity;                 // 容量，MB为单位
    IVS_UINT32  uiFullPolicy;               // 录像空间满处理策略：0-满覆盖所有录像  1-满覆盖计划录像 2-满停止

    IVS_CHAR	cReserve[32];
}IVS_RECORD_POLICY_CAPACITY;

#define IVS_RECORD_TYPE_LEN      8	// 录像类型长度
typedef enum
{
	//modify by zwx211831，去掉“智能分析录像	”。Date：20140520
	RECORD_TYPE_ALARM=1,	// 普通告警录像
	RECORD_TYPE_PLAN=2,		// 计划录像
	RECORD_TYPE_MANUAL=4,	// 手动录像
}IVS_RECORD_TYPE;

// 录像抽帧次数
typedef enum
{
	FRAME_EXTRACTION_0= 0,	// 完整
	FRAME_EXTRACTION_1= 1,	// 不完整（抽帧一次）
	FRAME_EXTRACTION_2= 2,	// 不完整（抽帧二次）
	FRAME_EXTRACTION_3= 3,	// 不完整（抽帧三次）
}IVS_FRAME_EXTRACTION_TIMES;

// 录像标签信息，查询方式为按书签时有效
#define IVS_BOOKMARK_NAME_LEN 612
typedef struct
{
	IVS_UINT32	uiBookmarkID;                           // 录像标签ID
    IVS_CHAR	cBookmarkName[IVS_BOOKMARK_NAME_LEN];   // 标签名，长度最大150字符
	IVS_CHAR	cBookmarkTime[IVS_TIME_LEN];            // 标签时间：yyyyMMddHHmmss

	IVS_CHAR	cCameraCode[IVS_DEV_CODE_LEN];          // 摄像头编码
	IVS_CHAR	cCameraName[IVS_NAME_LEN];              // 摄像头名称

    IVS_CHAR	cUserDomain[IVS_DOMAIN_CODE_LEN];       // 操作用户所在域编码
    IVS_UINT32  uiBookmarkCreatorID;                    // 创建书签的用户ID
    IVS_CHAR    cBookmarkCreatorName[IVS_NAME_LEN];     // 创建书签的用户名

    IVS_CHAR	cNvrCode[IVS_NVR_CODE_LEN];             // NVR编码
    IVS_CHAR	cMBUDomain[IVS_DOMAIN_CODE_LEN];        // 备份服务器域编码备份录像检索结果中带此字段

    IVS_CHAR	cReserve[32];
}IVS_RECORD_BOOKMARK_INFO;

// 录像书签列表
typedef struct
{
	IVS_UINT32   uiTotal;                           // 总记录数
	IVS_INDEX_RANGE stIndexRange;                   // 分页信息
	IVS_CHAR cRes[32];                              //保留字段
	IVS_RECORD_BOOKMARK_INFO stBookmarkInfo[1];     // 录像书签信息
}IVS_RECORD_BOOKMARK_INFO_LIST;

// 录像锁定信息，查询方式为按锁定状态时有效
#define IVS_RECORD_LOCK_DESC_LEN 260
typedef struct
{
	IVS_UINT32	uiLockID;						        // 锁定记录ID
	IVS_CHAR	cLockTime[IVS_TIME_LEN];		        // 执行锁定操作的时间
	IVS_TIME_SPAN	stLockTimeSpan;				        // 锁定的录像时段
	IVS_CHAR	cLockDesc[IVS_RECORD_LOCK_DESC_LEN];	// 锁定描述

	IVS_UINT32	uiOperatorID;						    // 执行锁定操作的用户ID
	IVS_CHAR	cOperatorName[IVS_NAME_LEN];            // 执行锁定操作的用户名
    IVS_CHAR    cReserve[64];                           // 保留字段
}IVS_RECORD_LOCK_INFO;

// 录像预置位信息，查询方式为按预置位查询时有效
#define IVS_RECORD_PTZ_PRESET_NAME_LEN 64
typedef struct
{
	IVS_UINT32	uiID;	// 预置位ID
	IVS_CHAR	cName[IVS_RECORD_PTZ_PRESET_NAME_LEN];	// 预置位名称
    IVS_CHAR    cReserve[32];                           // 保留字段
}IVS_RECORD_PTZ_PRESET_INFO;

// 录像检索信息
typedef struct
{
	IVS_CHAR	cRecordFileName[IVS_FILE_NAME_LEN];	    // 录像文件名

	IVS_UINT32	uiRecordMethod;		                    // 录像存储位置：参考 IVS_RECORD_METHOD
    IVS_CHAR	cNvrCode[IVS_NVR_CODE_LEN];             // NVR编码，仅在平台录像检索结果中带此字段（内部处理）
    IVS_CHAR	cMBUDomain[IVS_DOMAIN_CODE_LEN];        // 备份服务器域编码备份录像检索结果中带此字段

	IVS_UINT32	uiRecordType;		                    // 录像类型：参考 IVS_RECORD_TYPE
    IVS_CHAR    cAlarmType[IVS_QUERY_VALUE_LEN];		// 告警类型，录像类型为告警录像时有效（英文字符串）
	IVS_TIME_SPAN	stTime;                             // 录像起止时间
	IVS_UINT32	uiFrameExtractionTimes;	                // 录像抽帧次数：参考 IVS_FRAME_EXTRACTION_TIMES
    
	IVS_RECORD_BOOKMARK_INFO	stBookmarkInfo;         // 录像标签信息，查询方式为按书签时有效
	IVS_RECORD_LOCK_INFO	    stLockInfo;		        // 锁定信息，查询方式为按锁定状态时有效
	IVS_RECORD_PTZ_PRESET_INFO	stPtzPresetInfo;        // 录像预置位信息，查询方式为按预置位查询时有效
    IVS_CHAR    cReserve[64];                           // 保留字段
}IVS_RECORD_INFO;

// 录像信息列表
typedef struct
{
	IVS_UINT32   uiTotal;               // 总记录数
	IVS_INDEX_RANGE stIndexRange;       // 分页信息
	IVS_CHAR cRes[32];			        //保留字段
	IVS_RECORD_INFO stRecordInfo[1];    // 录像检索信息
}IVS_RECORD_INFO_LIST;

// 运动幅度数据信息
typedef struct
{
    IVS_CHAR	StartTime[IVS_TIME_LEN];    // 统计间隔起始时间，国际标准时间, 只有时分秒
    IVS_CHAR	EndTime[IVS_TIME_LEN];      // 统计间隔结束时间，国际标准时间, 只有时分秒
    IVS_UINT64	uiMotionData;	            // [OUT]在查询时间段内按时间间隔统计的移动数据
    IVS_CHAR    cReserve[32];               // 保留字段
}IVS_MOTION_RANGE_DATA_INFO;

// 运动幅度数据列表
typedef struct
{
    IVS_TIME_SPAN	stTime;			// [OUT]运动幅度数据起止时间
    IVS_UINT32		uiTimeInterval;	// [OUT]运动幅度数据统计时间间隔

    IVS_UINT32  uiTotal;			// [OUT]总记录数
    IVS_INDEX_RANGE stIndexRange;	// [IN]分页信息
    IVS_MOTION_RANGE_DATA_INFO	stMotionRangeDataInfo[1];	// [OUT]运动幅度数据信息，可多条
}IVS_MOTION_RANGE_DATA_LIST;

// 通用设备类型（查询设备时获取）
typedef enum
{
	DEVICE_TYPE_MAIN=1,		// 主设备
	DEVICE_TYPE_CAMERA,		// 摄像头设备
	DEVICE_TYPE_VOICE,		// 语音设备
	DEVICE_TYPE_ALARM,	    // 告警设备（同时获取告警输入、告警输出）
    DEVICE_TYPE_SERIAL,	    // 串口设备

    DEVICE_TYPE_EXTEND_ALARM=9,   // 外域告警设备
}IVS_DEVICE_TYPE;

// 主设备形态类型
typedef enum
{
	DEVICE_IPC= 1,	// 摄像机
	DEVICE_DVS,		// DVS
	DEVICE_DVR,		// DVR
    DEVICE_ENVR,    // eNVR
}IVS_MAIN_DEVICE_TYPE;

// 子设备形态类型
typedef enum
{
	DEVICE_CAMERA= 1,	// 摄像头
	DEVICE_ALARM_IN,	// 告警输入子设备
	DEVICE_ALARM_OUT,	// 告警输出子设备
	DEVICE_AUDIO_IN,	// 音频输入子设备
	DEVICE_AUDIO_OUT,	// 音频输出子设备
	DEVICE_SERIAL,		// 串口子设备
}IVS_CHANNEL_TYPE;

// 厂商类型
typedef enum
{
    VENDOR_NONE=0,      // 厂商无
	VENDOR_HUAWEI=1,	// 华为
	VENDOR_HIKVISION,	// 海康威势
	VENDOR_DAHUA,		// 大华
	VENDOR_AXIS,		// 安讯视
	VENDOR_BOSCH,		// Bosch
	VENDOR_SONY,		// 索尼
	VENDOR_PANASONIC	// 松下
}IVS_VENDOR_TYPE;

// IPC位置信息
typedef struct
{
    IVS_DOUBLE	dLongitude;             // 安装经度	20	最多12位小数，-180~180
    IVS_DOUBLE	dLatitude;				// 安装纬度	20	最多12位小数，-180~180
	IVS_DOUBLE	dHeight;                // 安装高度	5	单位m，0-99.99
	IVS_CHAR	cCameraLocation[IVS_DESCRIBE_LEN];	// 摄像机安装位置描述
    IVS_CHAR    cReserve[32];                       // 保留字段
}IVS_CAMERA_POSITION;

//码率类型
typedef enum
{
	BITRATE_CBR = 0,	//定码率
	BITRATE_VBR			//变码率
}IVS_BITRATE_TYPE;

// 设备网络地址信息
typedef struct
{
	IVS_IP		stIPV4;
	IVS_CHAR	cIPV4Gateway[IVS_IP_LEN];	// ipv4网关
	IVS_IP		stIPV6;
	IVS_CHAR	cIPV6Gateway[IVS_IP_LEN];	// ipv6网关
}IVS_DEV_IP_INFO;

// 设备状态
typedef enum
{
	DEV_STATUS_OFFLINE= 0,	// 离线
	DEV_STATUS_ONLINE,		// 在线
	DEV_STATUS_SLEEP,		// 休眠
}IVS_DEV_STATUS;

// 视频制式
typedef enum
{
	VIDEO_STANDART_NONE= 0,
	VIDEO_STANDART_PAL,
	VIDEO_STANDART_NTSC,
}IVS_VIDEO_STANDARD;

// 设备时间同步信息
typedef struct
{
	IVS_UINT32	uiNtpType;		// NTP同步方式：0-DHCP，1-手动指定服务器

	IVS_IP		stNtpServerIP;	// NTP服务器IP，当NTP同步方式uiNtpType为1时有效
	IVS_IP		stDnsIP;		// DNS地址，当NTP同步方式uiNtpType为1时有效
	IVS_CHAR	cRes[32];
}IVS_NTP_INFO;

// 主设备网络参数
typedef struct
{
    IVS_IP		stPUIp;	        // 前端IP
	IVS_IP		stPUGateway;	// 前端网关(onvif设备)
    IVS_UINT32  uiPUPort;       // 设备端口
	IVS_UINT32	uiMaxMtu;		// 最大MTU值
    IVS_IP		stSubNetMask;   // 子网掩码
    IVS_CHAR	cReserve[32];
}IVS_DEVICE_NET_CFG;

// 摄像机安全参数
typedef struct
{
    IVS_BOOL    bEnableMediaSecurity;       // 是否启用媒体安全：0-关闭 1-开启
    IVS_BOOL    bEnableSecureStorage;       // 是否启用安全存储：0-关闭 1-开启
    IVS_UINT32  uiMediaSecurityAlgorithm;   // 媒体安全算法：1-AES256 2-媒体扰码
    IVS_CHAR	cReserve[32];
}IVS_MEDIA_SECURITY_CFG;

// 可靠传输类型
typedef enum
{
    RELIABLE_TRANSMISSION_NONE,
    RELIABLE_TRANSMISSION_FEC,
    RELIABLE_TRANSMISSION_ARQ,
    RELIABLE_TRANSMISSION_HARQ,
}IVS_RELIABLE_TRANSMISSION_TYPE;

// 摄像机扩展参数
typedef struct
{
    IVS_BOOL	bEnableWatermark;	        // 是否启用水印：0-关闭 1-开启
    IVS_UINT32  uiReliableTransmission;     // 可靠传输类型，值参考 IVS_RELIABLE_TRANSMISSION_TYPE
    IVS_UINT32  uiFECMatrix;                // 可靠传输类型FEC模式下，矩阵模式 
    /*
    0:  5x5矩阵 
    1:  7x7矩阵 
    2:  8x4矩阵 
    3:  9x6矩阵 
    4:  10x4矩阵
    5:  11x4矩阵
    6:  12x6矩阵
    7:  15x5矩阵
    8:  18x6矩阵
    9:  19x5矩阵
    10: 24x7矩阵
    11: 26x6矩阵
    12: 34x1矩阵
    */
    IVS_BOOL    bEnableBandwidthAdaptive;   // 是否启用带宽自适应：0-关闭 1-开启
    IVS_BOOL    bEnableCache;               // 是否启用缓存：0-关闭 1-开启

    IVS_MEDIA_SECURITY_CFG stMediaSecurityCfg;  // 安全相关参数
    IVS_CHAR	cReserve[32];
}IVS_CAMERA_EXTEND_CFG;

// 摄像机FEC参数
typedef struct
{
    IVS_UINT32  uiReliableTransmission;     // 可靠传输类型，值参考 IVS_RELIABLE_TRANSMISSION_TYPE
    IVS_UINT32  uiDataDropPercent;           // 可靠传输类型FEC模式下，丢包率    1:  1%丢包率   2:  2%丢包率  3:  3%丢包率   4:  4%丢包率   5:  5%丢包率
    IVS_CHAR	cReserve[32];
}IVS_CAMERA_FEC_CFG;

// 摄像机抓拍参数
typedef struct
{
    IVS_UINT32  uiSnapCount;    // 抓拍数量，范围1-5
    IVS_UINT32  uiSnapInterval; // 抓拍时间间隔，范围1-5秒
    IVS_CHAR	cReserve[8];
}IVS_CAMERA_SNAPSHOT_CFG;

// 摄像机前端录像码流参数
typedef struct
{
    IVS_UINT32	uiStreamType;	    // 码流类型：参考 IVS_STREAM_TYPE
    IVS_CHAR	cReserve[8];
}IVS_CAMERA_RECORD_STREAM_CFG;

// 主设备扩展属性
typedef struct
{
	IVS_BOOL	bEnableWatermark;	// 是否启用水印 0：否 1:是？视频子设备参数
	IVS_BOOL	bEnableEncrypt;		// 是否启用加密 0: 否 1:是？
	IVS_BOOL	bEnableCache;		// 是否支持缓存补录 0:否 1:是
    IVS_CHAR    cReserve[32];       // 保留字段
}IVS_DEVICE_EXCFG;

// 主设备通道信息
typedef struct
{
	IVS_UINT32	uiMaxVideoNum;		// 最大视频通道数
	IVS_UINT32	uiMaxAudioInNum;	// 最大音频输入通道数
	IVS_UINT32	uiMaxAudioOutNum;	// 最大音频输出通道数
	IVS_UINT32  uiMaxAlarmInNum;	// 最大告警输入通道数
	IVS_UINT32  uiMaxAlarmOutNum;	// 最大告警输出通道数
	IVS_UINT32  uiMaxSerialNum;		// 最大串口通道数
    IVS_CHAR    cReserve[32];       // 保留字段
}IVS_DEVICE_CHANNEL_CFG;

// 主设备时间参数
typedef struct
{
    IVS_BOOL	    bEnableNtp;	    // 是否启用NTP同步：0-不启用（手动设置时间），1-启用（自动同步时间）
	IVS_NTP_INFO    stNTP;	        // NTP信息(单独接口实现) 时间参数（当bEnableNtp值为1，自动同步时间时有效）
	IVS_INT32	    iTimezone;	    // 设备所在时区，以时区偏移量记录-50400~50400秒 （当bEnableNtp值为0，手动设置时间时有效）
	IVS_BOOL	    bEnableDst;	    // 是否启用夏令时 0:不启用,1:启用 （当bEnableNtp值为0，手动设置时间时有效）
    IVS_CHAR        cReserve[32];   // 保留字段
}IVS_DEVICE_TIME_CFG;

// 通用设备信息（NVR/TAS）（主设备 DVR/DVS/IPC）
#define IVS_MAX_VENDOR_TYPE_NAME_LEN    32  // 厂商名称最大长度
#define IVS_DEVICE_NAME_LEN             128 // 主设备名称长度
typedef struct
{
	IVS_UINT32	uiType;									// 主设备类型：参考 IVS_MAIN_DEVICE_TYPE
	IVS_CHAR	cSerialNumber[IVS_DEV_SERIALNO_LEN];	// 序列号(SN/IMEI)

	IVS_CHAR	cCode[IVS_DEV_CODE_LEN];	        // 设备编码
	IVS_CHAR	cName[IVS_DEVICE_NAME_LEN];		    // 设备名称

	IVS_CHAR	cParentCode[IVS_DEV_CODE_LEN];	    // 父设备编码，如果不为空，表示通过eNVR接入
	IVS_CHAR	cDomainCode[IVS_DOMAIN_CODE_LEN];	// 域编码

    IVS_CHAR	cVendorType[IVS_MAX_VENDOR_TYPE_NAME_LEN];  // 设备提供商类型: "HUAWEI" "HIK"

	IVS_CHAR	cModel[IVS_DEV_MODEL_LEN];	// 设备型号

	IVS_IP		stIP;						// 设备IP信息
	IVS_UINT32	uiPort;						// 设备连接端口

	IVS_CHAR	cReserve[32];
}IVS_DEVICE_BASIC_INFO;

// 设备编码信息
typedef struct
{
	IVS_CHAR	cCode[IVS_DEV_CODE_LEN];	// 设备编码
	IVS_CHAR	cNvrCode[IVS_NVR_CODE_LEN];	// 设备所属NVR编码, 表示该设备被该NVR管理	

	IVS_UINT32	uiSequence;	// 序号
	IVS_UINT32	uiResult;	// 操作结果

    IVS_CHAR    cReserve[32];   // 保留字段
}IVS_DEVICE_OPER_RESULT;

// 设备编码列表
typedef struct
{
    IVS_UINT32	uiNum;
    IVS_DEVICE_OPER_RESULT	stCodeInfo[1];
}IVS_DEVICE_OPER_RESULT_LIST;

// 设备编码
typedef struct
{
	IVS_CHAR cDevCode[IVS_DEV_CODE_LEN];	// 设备编码
}IVS_DEVICE_CODE;

// 认证类型
typedef enum
{
	LOGINTYPE_NONE= 0,
	LOGINTYPE_HTTP_DIGEST,		// HTTP Digest
	LOGINTYPE_USERNAME_TOKEN,	// WS-Usernametoken Authentication
}IVS_LOGINTYPE;

// 主设备配置参数
#define IVS_MAX_PROTOCOLTYPE_NAME_LEN   64  // 协议类型描述最大长度
typedef struct
{
	IVS_DEVICE_BASIC_INFO	stDeviceInfo;	        // 主设备基本信息
	//
	IVS_CHAR	cProtocolType[IVS_MAX_PROTOCOLTYPE_NAME_LEN];		// 主设备协议类型: "TEYES"-千里眼   "ONVIF"  "HWSDK"-华为SDK "HIKSDK"-海康SDK

	IVS_UINT32	iLoginType;			                // 认证类型：参考 IVS_LOGINTYPE 驱动为ONVIF时认证类型可设置，认证类型有三种（0:None、1:HTTP Digest、2:WS-Usernametoken Authentication）

	IVS_BOOL	bEnableAlarm;		                // 设备告警使能 1-启用（ON） 0-停用（OFF）参考：IVS_VIDEO_STANDARD
	IVS_UINT32	uiVideoFormat;		                // 视频制式 1-PAL 2-NTSC

	IVS_UINT32	uiDirectConnectFirst;	            // 是否直连优先： 0-非直连优先 1-直连优先
	IVS_UINT32	uiMaxDirectConnectNum;	            // 最大直连数目

	IVS_BOOL	bEnableSchedule;	                // 是否启用集群调度：0-否, 1-是(NVR CODE 不为空)
	IVS_CHAR	cNvrCode[IVS_NVR_CODE_LEN];			// 设备所属NVR编码, 表示该设备被该NVR管理
	IVS_CHAR	cTasCode[IVS_TAS_CODE_LEN];			// 设备所属TAS编码, 当是被通过独立部署的DCG接入时使用
	IVS_IP      stTasIP;                               // TASIP
	IVS_CHAR	cDevUser[IVS_NAME_LEN];				// 设备登录用户名
	IVS_CHAR	cDevPasswd[IVS_PWD_LEN];		// 设备登录密码
	IVS_CHAR	cDevRegPasswd[IVS_PWD_LEN];	// 设备向平台注册密码

    IVS_DEVICE_CHANNEL_CFG  stDeviceChannel;        // 设备通道信息

	IVS_CHAR	cReserve[32];
}IVS_DEVICE_CFG;

// 主设备操作信息
typedef struct
{
	IVS_UINT32	uiSequence;			// 设备序号（如添加操作等）
	IVS_DEVICE_CFG	stDeviceCfg;	// 主设备配置参数
    IVS_CHAR    cReserve[32];       // 保留字段
}IVS_DEVICE_OPER_INFO;

// 摄像机显示参数
typedef struct
{
    IVS_UINT32	uiBrightness;	// 亮度，范围：0-255
    IVS_UINT32	uiContrast;		// 对比度，范围：0-255
    IVS_UINT32	uiHue;			// 色度，范围：0-255
    IVS_UINT32	uiSaturation;	// 饱和度，范围：0-255
    IVS_CHAR    cReserve[32];   // 保留字段
}IVS_CAMERA_DISPLAY_CFG;

// 背光补偿参数
typedef struct
{
    IVS_UINT32  uiBacklightMode;    // 背光补偿模式 0-关闭；1-开启
    IVS_UINT32  uiBacklightLevel;   // 背光补偿等级 0-3
    IVS_CHAR    cReserve[32];       // 保留字段
}IVS_BACKLIGHT;

// 曝光参数
typedef struct
{
    IVS_UINT32  uiExposureMode;     // 曝光模式 0-自动；1-手动
    IVS_UINT32  uiExposurePriority; // 曝光优先模式 0-低噪优先；1-帧率优先
    IVS_RECT_FLOAT    rectWindow;   // 曝光窗口
    IVS_UINT32  uiExposureTime;     // 曝光时间（单位微秒us）
    IVS_UINT32  uiExposureGain;     // 曝光增益（单位dB）
    IVS_UINT32  uiExposureIris;     // 光圈
    IVS_CHAR    cReserve[32];       // 保留字段
}IVS_EXPOSURE;

// 变焦参数
typedef struct
{
    IVS_UINT32  uiFocusMode;        // 变焦模式 0-自动；1-手动
    IVS_UINT32  uiFocusSpeed;       // 变焦速度
    IVS_UINT32  uiNearLimit;        // 最小变焦范围
    IVS_UINT32  uiFarLimit;         // 最大变焦范围
    IVS_CHAR    cReserve[32];       // 保留字段
}IVS_FOCUS;

// 宽动态参数
typedef struct
{
    IVS_UINT32  uiWideDynamicMode;  // 宽动态模式 0-关闭；1-开启
    IVS_UINT32  uiWideDynamicLevel; // 宽动态等级 0-20
    IVS_CHAR    cReserve[32];       // 保留字段
}IVS_WIDE_DYNAMIC;

// 白平衡参数
typedef struct
{
    IVS_UINT32  uiWhiteBalanceMode; // 白平衡模式 0-自动；1-手动
    IVS_UINT32  uiCrGain;           // 红色增益
    IVS_UINT32  uiCbGain;           // 蓝色增益
    IVS_CHAR    cReserve[32];       // 保留字段
}IVS_WHITE_BALANCE;

// 防抖动参数
typedef struct
{
    IVS_UINT32	uiStabilizationMode;	// 防抖动模式 0-关闭；1-打开；2-自动；3-扩展
    IVS_UINT32	uiStabilizationLevel;   // 防抖动等级
    IVS_CHAR    cReserve[32];           // 保留字段
}IVS_STABILIZATION;

// 摄像机图像参数
typedef struct
{
    IVS_BACKLIGHT       stBacklight;        // 背光
    IVS_EXPOSURE        stExposure;         // 曝光
    IVS_FOCUS           stFocus;            // 变焦
    IVS_WIDE_DYNAMIC    stWideDynamic;      // 宽动态
    IVS_WHITE_BALANCE   stWhiteBalance;     // 白平衡
    IVS_STABILIZATION   stStabilization;    // 防抖动
    IVS_UINT32          uiIrCutFilter;      // 红外过滤 0-关闭；1-打开；2-自动
    IVS_UINT32          uiSharpness;        // 锐度
    IVS_CHAR            cReserve[32];       // 保留字段
}IVS_CAMERA_IMAGING_CFG;

#define IVS_CAMERA_OSD_NUM_MAX (128)

// 前端OSD时间
typedef struct
{
    IVS_BOOL	bEnableOSDTime;     // 是否显示时间：0-不显示，1-显示
    IVS_UINT32	uiTimeFormat;       // 时间显示格式：？1: XXXX-XX-XX XX:XX:XX(如2009-09-09 09:09:09), 2: XXXX年XX月XX日 XX :XX :XX(2009年09月09日 09 :09 :09；3: UTC时间
    IVS_FLOAT   fTimeX;             // 时间X坐标，以左上角为原点
    IVS_FLOAT   fTimeY;             // 时间Y坐标，以左上角为原点
    IVS_CHAR    cReserve[32];       // 保留字段
}IVS_OSD_TIME;

// 前端OSD文字
#define IVS_MAX_OSD_TEXT_LEN	256 // OSD文字最大长字节数
typedef struct
{
    IVS_BOOL	bEnableOSDName;                     // 是否显示文字：0-不显示，1-显示
    IVS_CHAR	cOSDNameText[IVS_MAX_OSD_TEXT_LEN]; // 文字内容
    IVS_RECT_FLOAT    rectText;                     // 文字区域

    IVS_BOOL	bEnableSwitch;		                // 是否交替显示：0-不交替，1-交替
    IVS_UINT32	uiSwitchInterval;                   // 交替显示时间间隔，单位为秒

    IVS_BOOL	bEnableTextBlink;		            // 是否允许闪烁：0-不闪烁，1-闪烁
    IVS_BOOL	bEnableTextTranslucent;	            // 是否允许透明：0-不透明，1-透明
    IVS_UINT32	uiTextTranslucentPercent;           // 文字透明度：0-100

    IVS_CHAR	cReserve[64];
}IVS_OSD_NAME;

// 摄像机OSD参数
typedef struct
{
    IVS_BOOL        bEnableOSD; // 是否启用：0-停用 1-启用
    IVS_OSD_TIME    stOSDTime;  // OSD时间信息
    IVS_OSD_NAME    stOSDName;  // OSD文字信息

    IVS_CHAR	cReserve[32];
}IVS_CAMERA_OSD_CFG;

// 多行OSD参数配置信息
typedef struct
{
	IVS_UINT32		uiTotal;														//总记录数
	IVS_UINT32		uiOSDTimeNum;										//OSD时间信息个数
	IVS_UINT32		uiOSDNameNum;									//OSD文字信息个数
	IVS_BOOL			bEnableOSD;											//是否启用：0-停用 1-启用
	IVS_CHAR			cReserve[32];											//保留字段
	IVS_CAMERA_OSD_CFG stCameraOSDCfg[1];				//多条OSD参数信息
}IVS_CAMERA_MULTI_OSD_CFG;


// 运动检测
#define IVS_MAX_MOTION_DETECTION_AREA_NUM   8   // 运动检测区域个数
typedef struct
{
    IVS_BOOL    bMotionDetectionEnable;     // 设备运动检测告警使能 0-停用运动检测 1-启用运动检测
    IVS_BOOL    bPlatformMotionDetection;   // 平台运动检测告警使能 0-停用运动检测 1-启用运动检测

    IVS_UINT32  uiDetectInterval;       // 运动监测的时间间隔，在该间隔内的运动都作为同一个运动，取值范围：1~30秒 单位:秒 默认为：10秒
    IVS_UINT32  uiSensitivity;          // 运动监测的灵敏度，共分5级：1-5，数字越高表示灵敏度越高
    IVS_UINT32  uiAutoClearAlarmTime;   // 告警检测时间间隔1-255，单位：秒。默认为120秒，即每隔120秒检测一次是否在告警状态，并上报

    IVS_UINT32  uiRectNum;              // 有效区域信息的个数
    IVS_RECT_FLOAT    rectDetection[IVS_MAX_MOTION_DETECTION_AREA_NUM];   // 运动检测区域列表
        
    IVS_CHAR	cReserve[32];
}IVS_MOTION_DETECTION;

// 图像遮挡
#define IVS_MAX_VIDEO_HIDE_AREA_NUM 5   // 最大图像遮挡告警区域个数
#define IVS_MAX_VIDEO_HIDE_GUARD_TIME_NUM   12  // 最大视频遮挡布防时间段个数
typedef struct
{
    IVS_BOOL    bEnableVideoHide;       // 0-关闭视频遮挡告警，1-启用视频遮挡告警
    IVS_UINT32  uiSensitivity;          // 图像遮挡监测的灵敏度，共分5级：1-5，数字越高表示灵敏度越高
    IVS_UINT32  uiAutoClearAlarmTime;   // 告警检测时间间隔1-255，单位：秒。默认为120秒，即每隔120秒检测一次是否在告警状态，并上报

    IVS_UINT32  uiGuardTimeNum;                                         // 实际告警时间个数
    IVS_TIME_SPAN   stGuardTime[IVS_MAX_VIDEO_HIDE_GUARD_TIME_NUM];     // 视频遮挡布防时间段

    IVS_UINT32  uiAreaNum;                              // 实际设置的图像遮挡告警区域个数
    IVS_RECT_FLOAT    rectArea[IVS_MAX_VIDEO_HIDE_AREA_NUM];  // 图像遮挡告警区域数组

    IVS_CHAR	cReserve[32];
}IVS_VIDEO_HIDE_ALARM;

// 隐私保护
#define IVS_MAX_VIDEO_MASK_AREA_NUM 5   // 最大隐私保护区域个数
typedef struct
{
    IVS_BOOL    bEnableVideoMask;       // 0-关闭隐私保护，1-启用隐私保护

    IVS_UINT32  uiAreaNum;                              // 实际设置的图像遮挡告警区域个数
    IVS_RECT_FLOAT    rectArea[IVS_MAX_VIDEO_MASK_AREA_NUM];  // 图像遮挡告警区域数组

    IVS_CHAR	cReserve[32];
}IVS_VIDEO_MASK;

// 告警输入子设备
#define IVS_ALARM_IN_NAME_LEN   260      // 告警输入子设备名称长度
typedef struct
{
    IVS_CHAR    cAlarmInName[IVS_ALARM_IN_NAME_LEN];    // 告警输入设备名称，汉字和字母（a-z和A-Z），数字，中划线和下划线，1~20个字符。
    IVS_UINT32  uiAlarmInChanIndex;                     // 告警输入通道索引
    IVS_UINT32  uiSwitchType;                           // 设备开关类型 0-常闭 1-常开 
    IVS_UINT32  uiAutoClearAlarmTime;                   // 告警检测时间间隔1-255，单位：秒。默认为120秒，即每隔120秒检测一次是否在告警状态，并上报，数字。

    IVS_CHAR	cReserve[32];
}IVS_ALARM_IN_CFG;

// 告警输出子设备
#define IVS_ALARM_OUT_NAME_LEN   128      // 告警输出子设备名称长度
typedef struct
{
    IVS_CHAR    cAlarmOutName[IVS_ALARM_OUT_NAME_LEN];  // 告警输出设备名称，汉字和字母（a-z和A-Z），数字，中划线和下划线，1~20个字符。
    IVS_UINT32  uiAlarmOutChanIndex;                    // 告警输出通道索引
    IVS_UINT32  uiSwitchType;                           // 设备开关类型 0-常闭 1-常开
    IVS_UINT32  uiAlarmOutMode;                         // 告警输出模式：0-单稳态，1-双稳态    
    IVS_UINT32  uiHoldTime;                             // 延迟时间，1秒~600秒；默认：10秒（单稳态时有效）

    IVS_CHAR	cReserve[32];
}IVS_ALARM_OUT_CFG;

typedef struct
{
    IVS_UINT32  uiPitch;        // 定调
    IVS_UINT32  uiVolume;       // 声道
    IVS_UINT32  uiInputType;    // 输入类型
    IVS_UINT32  uiDataFormat;   // 数据格式，参考 IVS_PAYLOAD_TYPE
    IVS_UINT32  uiSampleRate;   // 采样率
    IVS_UINT32  uiChannel;      // 频道
    IVS_UINT32  uiGain;         // 增益
    IVS_UINT32  uiSampleBits;   // 比特率

    IVS_CHAR	cReserve[32];
}IVS_AUDIO_PARAM;

// 音频子设备参数
#define IVS_AUDIO_NAME_LEN   128      // 音频子设备名称长度
typedef struct
{
    IVS_CHAR    cAudioName[IVS_AUDIO_NAME_LEN];     // 语音通道名称，汉字和字母（a-z和A-Z），数字，中划线和下划线，1~20个字符。
    IVS_UINT32  uiChannelType;                      // 通道类型 0-语音输入 1-语音输出
    IVS_UINT32  uiAudioIndex;                       // 语音通道索引，正整数。

    IVS_AUDIO_PARAM stAudioParam;                   // 音频参数

    IVS_CHAR	cReserve[32];
}IVS_AUDIO_CFG;

// 语音设备信息
typedef struct
{
	IVS_CHAR	cDevGroupCode[IVS_DEVICE_GROUP_LEN];           // 设备组编码
    IVS_CHAR    cAudioCode[IVS_DEV_CODE_LEN];       // 语音通道编码长度
    IVS_CHAR	cDomainCode[IVS_DOMAIN_CODE_LEN];	// 所属域编码
    IVS_CHAR    cAudioName[IVS_AUDIO_NAME_LEN];     // 语音通道名称，汉字和字母（a-z和A-Z），数字，中划线和下划线，1~20个字符。
    IVS_UINT32  uiChannelType;                      // 通道类型 0-语音输入 1-语音输出

    IVS_CHAR	cReserve[32];
}IVS_AUDIO_BRIEF_INFO;

// 语音设备列表信息
typedef struct
{
    IVS_UINT32   uiTotal;           // 总记录数
    IVS_INDEX_RANGE stIndexRange;   // 分页信息
    IVS_CHAR cRes[32];              //保留字段
    IVS_AUDIO_BRIEF_INFO stAudioBriefInfo[1];  // 语音设备列表信息
}IVS_AUDIO_BRIEF_INFO_LIST;

// 告警设备信息
#define IVS_AUDIO_CODE_LEN  64      // 语音通道编码长度
typedef struct
{
    //IVS_UINT32	uiDevGroupID;                       // 设备组ID
    IVS_CHAR	cDevGroupCode[IVS_DEVICE_GROUP_LEN];// 所属设备组编码
    IVS_CHAR    cAlarmCode[IVS_DEV_CODE_LEN];       // 告警设备编码
    IVS_CHAR	cDomainCode[IVS_DOMAIN_CODE_LEN];	// 所属域编码
    IVS_CHAR    cAlarmName[IVS_ALARM_IN_NAME_LEN];  // 语音通道名称，汉字和字母（a-z和A-Z），数字，中划线和下划线，1~20个字符。
    IVS_UINT32  uiChannelType;                      // 通道类型 0-告警输入 1-告警输出
    IVS_CHAR    cParentCode[IVS_DEV_CODE_LEN];      // 主设备编码
    IVS_BOOL	bIsExDomain;                        // 是否为外部域 0-否  1-是

    IVS_CHAR	cReserve[32];
}IVS_ALARM_BRIEF_INFO;

// 告警设备列表信息
typedef struct
{
    IVS_UINT32   uiTotal;           // 总记录数
    IVS_INDEX_RANGE stIndexRange;   // 分页信息
    IVS_CHAR cRes[32];              //保留字段
    IVS_ALARM_BRIEF_INFO stAlarmBriefInfo[1];  // 语音设备列表信息
}IVS_ALARM_BRIEF_INFO_LIST;

typedef enum
{
    CONFIG_DEVICE_CFG=1,                    // 主设备基本参数 IVS_DEVICE_CFG
    CONFIG_DEVICE_NET_CFG,                  // 主设备网络参数 IVS_DEVICE_NET_CFG
    CONFIG_DEVICE_TIME_CFG,                 // 主设备时间参数（NTP、时区）IVS_DEVICE_TIME_CFG

    CONFIG_CAMERA_CFG,                      // 摄像机基本参数设置 IVS_CAMERA_CFG
    CONFIG_CAMERA_STREAM_CFG,               // 摄像机码流参数设置 IVS_CAMERA_STREAM_CFG
    CONFIG_CAMERA_DISPLAY_CFG,              // 摄像机基本显示参数设置 IVS_CAMERA_DISPLAY_CFG
    CONFIG_CAMERA_IMAGING_CFG,              // 摄像机图像参数设置 IVS_CAMERA_IMAGING_CFG
    CONFIG_CAMERA_OSD_CFG,                  // 摄像机OSD参数设置 IVS_CAMERA_OSD_CFG
    CONFIG_CAMERA_MOTION_DETECTION_CFG,     // 摄像机运动检测参数设置 IVS_MOTION_DETECTION
    CONFIG_CAMERA_VIDEO_HIDE_ALARM_CFG,     // 摄像机遮挡告警参数设置 IVS_VIDEO_HIDE_ALARM
    CONFIG_CAMERA_VIDEO_MASK_CFG,           // 摄像机隐私保护参数设置 IVS_VIDEO_MASK
    CONFIG_CAMERA_AUDIO_CFG,                // 摄像机音频参数设置 IVS_AUDIO_CFG
    
    CONFIG_SERIAL_CHANNEL_CFG,              // 摄像机串口参数设置 IVS_SERIAL_CHANNEL_CFG
    CONFIG_ALARM_IN_CFG,                    // 告警输入子设备设置 IVS_ALARM_IN_CFG
    CONFIG_ALARM_OUT_CFG,                   // 告警输出子设备设置 IVS_ALARM_OUT_CFG
    
    CONFIG_DEVICE_PTZ_CFG,                  // 云台参数设置 IVS_PTZ_CFG
    CONFIG_CAMERA_EXTEND_CFG,               // 摄像机扩展参数设置 IVS_CAMERA_EXTEND_CFG

    CONFIG_CAMERA_SNAPSHOT_CFG,             // 摄像机抓拍参数设置 IVS_CAMERA_SNAPSHOT_CFG
    CONFIG_CAMERA_RECORD_STREAM_CFG,        // 摄像机前端录像码流设置 IVS_CAMERA_RECORD_STREAM_CFG
	CONFIG_CAMERA_FEC_CFG,                  // 摄像机FEC参数 IVS_CAMERA_FEC_CFG

	CONFIG_CAMERA_STREAM_EVENTEX_CFG,	// 摄像机码流参数设置 IVS_CAMERA_STREAM_EVENTEX_CFG（包含事件触发图像质量调整信息）
	CONFIG_CAMERA_MULTI_OSD_CFG,				//摄像机设备的OSD显示信息设置 IVS_CAMERA_MULTI_OSD_CFG(支持设置多行OSD行信息)
}IVS_CONFIG_TYPE;

typedef enum
{
    DEVICE_ENCODE_CAPABILITY=1,             // 摄像机码流参数
}IVS_DEVICE_CAPABILITY_TYPE;

// 摄像机接入协议
typedef enum
{
    DEVICE_PRO_TEYES = 0,	// 千里眼
    DEVICE_PRO_ONVIF,		// ONVIF
    DEVICE_PRO_HUAWEI,		// 华为SDK
    DEVICE_PRO_HIK,			// 海康SDK
	DEVICE_PRO_T28181,		// T28181
    DEVICE_PRO_DHSDK		// DHSDK
}IVS_DEVICE_PROTOCOL;

// 查询主设备列表信息（NVR/TAS）（主设备 DVR/DVS/IPC）
typedef struct
{
	IVS_DEVICE_BASIC_INFO	stDeviceInfo;	// 主设备基本信息

    IVS_CHAR	cProtocolType[IVS_MAX_PROTOCOLTYPE_NAME_LEN];		// 主设备协议类型: "TEYES"-千里眼   "ONVIF"  "HWSDK"-华为SDK "HIKSDK"-海康SDK

    IVS_UINT32	iLoginType;			// 认证类型：参考 IVS_LOGINTYPE 驱动为ONVIF时认证类型可设置，认证类型有三种（0:None、1:HTTP Digest、2:WS-Usernametoken Authentication）

	IVS_BOOL	bEnableSchedule;	// 是否启用集群调度：0-否, 1-是(NVR CODE 不为空)
	IVS_UINT32	uiStatus;			// 设备状态：0-离线，1-在线，2-休眠 参考 IVS_DEV_STATUS

	IVS_CHAR	cReserve[32];
}IVS_DEVICE_BRIEF_INFO;

typedef struct
{
	IVS_UINT32   uiTotal;           // 总记录数
	IVS_INDEX_RANGE stIndexRange;   // 分页信息
	IVS_CHAR cRes[32];              //保留字段
	IVS_DEVICE_BRIEF_INFO stDeviceBriefInfo[1];  // 主设备列表信息
}IVS_DEVICE_BRIEF_INFO_LIST;

// 主设备详细信息（DVR/DVS/IPC）
typedef struct
{
	IVS_DEVICE_CFG	stDeviceCfg;            // 主设备参数
	IVS_DEVICE_EXCFG	stDeviceExCfg;	    // 主设备扩展参数 
	IVS_DEVICE_NET_CFG	stDeviceNetCfg;	    // 主设备网络参数
	IVS_DEVICE_CHANNEL_CFG	stChannelInfo;	// 通道数配置
	IVS_DEVICE_TIME_CFG	stTimeInfo;	        // 时间参数（NTP、时区）

	IVS_UINT32	uiStatus;			        // 设备状态：0-离线，1-在线，2-休眠 参考 IVS_DEV_STATUS

	IVS_CHAR	cReserve[32];
}IVS_DEVICE_INFO;

#define IVS_MAX_DEV_GROUP_NUM   5000    // 最大设备组数目
// 设备组
typedef struct
{
	IVS_UINT32	uiID;					// 设备组ID
	IVS_CHAR	cName[IVS_NAME_LEN];	// 设备组名称
	IVS_UINT32	uiParentID;				// 父设备组ID
	IVS_CHAR	cDomainCode[IVS_DOMAIN_CODE_LEN];	// 域编码
    IVS_CHAR	cReserve[32];
}IVS_DEV_GROUP;

typedef struct
{
    IVS_CHAR	cGroupCode[IVS_DEVICE_GROUP_LEN];           // 设备组编码
    IVS_CHAR	cGroupName[IVS_NAME_LEN];	                // 设备组名称
    //IVS_UINT32	uiParentGroupID;                    // 父设备组ID
    IVS_CHAR	cParentGroupCode[IVS_DEVICE_GROUP_LEN];     // 父设备组编码
    IVS_CHAR	cDomainCode[IVS_DOMAIN_CODE_LEN];	        // 域编码
    IVS_BOOL	bIsExDomain;	                            // 是否外部域设备/组   0：否，1：是
    IVS_CHAR	cReserve[32];
}IVS_DEVICE_GROUP;

// 设备组列表
typedef struct
{
    IVS_UINT32          uiTotal;				// 总记录数
    IVS_INDEX_RANGE     stIndexRange;			// 分页信息
    IVS_DEVICE_GROUP    stDeviceGroup[1];		// 用户设备组信息列表
}IVS_DEVICE_GROUP_LIST;

// 设备组列表
typedef struct
{
	IVS_UINT32   uiTotal;				// 总记录数
	IVS_INDEX_RANGE stIndexRange;       // 分页信息
	IVS_DEV_GROUP stDeviceGroup[1];		// 用户设备组信息列表
}IVS_DEV_GROUP_LIST;

// 设备组摄像头信息(组操作使用)
typedef struct
{
	IVS_CHAR	cCameraCode[IVS_DEV_CODE_LEN];          // 摄像头编码
	IVS_CHAR	cCameraDomainCode[IVS_DOMAIN_CODE_LEN];    // 摄像头所属域编码

	IVS_UINT32	uiDevGroupID;					// 摄像头所属设备组ID
	//	IVS_CHAR	cDevGroupDomainCode[IVS_DOMAIN_CODE_LEN];	// 设备组所属域编码
	IVS_CHAR	cReserve[64];
}IVS_DEV_GROUP_CAMERA;

// 设备组
typedef struct
{
	IVS_UINT32   uiTotal;				// 总记录数
	IVS_INDEX_RANGE stIndexRange;			// 分页信息
	IVS_DEV_GROUP_CAMERA stDevGroupCamera[1];		// 设备组摄像头信息列表
}IVS_DEV_GROUP_CAMERA_LIST;

// 目标设备组信息
typedef struct
{
	IVS_UINT32	uiID;                               // 目标设备组ID
	IVS_CHAR	cDomainCode[IVS_DOMAIN_CODE_LEN];	// 目标域编码
    IVS_CHAR	cReserve[32];
}IVS_TARGET_DEV_GROUP;

// 组播信息
typedef struct
{
	IVS_IP		stIP;	// 组播IP地址
	IVS_UINT32	uiPort;	// 组播端口
	IVS_UINT32	uiTTL;  // (Time To Live ) 生存时间
    IVS_CHAR	cReserve[32];
}IVS_BROADCAST_INFO;

// 云台参数
typedef struct ST_PTZ_PARA
{
	IVS_UINT32  uiDecoderType;                          // 云台解码器类型
	IVS_UINT32  uiDecoderAddress;                       // 云台解码器地址(1,2...) ，最大长度4，正整数
	IVS_BOOL    bPosNegDirectControl;                   // 正反向控制，取值范围：0-正向 1-反向
	IVS_CHAR    cSerialCode[IVS_DEV_SERIALNO_LEN];      // 串口编码

	IVS_CHAR	cReserve[32];
}IVS_PTZ_CFG;

// 云台控制返回信息结构体
typedef struct
{    
    IVS_UINT32  uiLockStatus;              // 云台锁定状态：0-解锁，1-锁定    
    IVS_UINT32	uiUserID;			       // 用户ID    
    IVS_CHAR	cUserName[IVS_NAME_LEN];   // 用户登录账号    
    IVS_IP	    stIP;			           // 登录的客户端IP    
    IVS_UINT32  uiReleaseTimeRemain;       // 释放时长，以秒为单位    
    IVS_CHAR	cReserve[32];
}IVS_PTZ_CONTROL_INFO;

typedef enum
{
    IMAGING_QUALITY_HIGH=0,     // 图像质量高，默认值
    IMAGING_QUALITY_MIDDLE=1,   // 图像质量中
    IMAGING_QUALITY_LOW=2,      // 图像质量低
}IVS_IMAGING_QUALITY;


// 分辨率
#define IVS_RESOLUTION_LEN  	16	// 分辨率字符串长度

// 视频流信息
typedef struct
{
	IVS_UINT32	uiStreamType;	    // 码流类型：参考 IVS_STREAM_TYPE
	IVS_UINT32	uiEncodeType;	    // 编码格式：参考 IVS_VIDEO_DEC_TYPE
    IVS_UINT32	uiProfile;          // 根据uiEncodeType确定，如果是H.264：参考IVS_H264_PROFILE；如果是MJPEG：参考IVS_MJPEG_PROFILE
    IVS_UINT32	uiFrameInterval;	// I帧间隔
    IVS_CHAR	cResolution[IVS_RESOLUTION_LEN];    // 分辨率，如“177*144”

    IVS_UINT32	uiPicQuality;	    // 图像质量 （0-高 1-中 2-低） IVS_IMAGING_QUALITY

    IVS_UINT32	uiTransProtocol;    // 传输协议：参考 IVS_PROTOCOL_TYPE
    IVS_UINT32	uiPktProtocol;      // 打包协议：参考 IVS_PACK_PROTOCOL_TYPE
	IVS_UINT32	uiBitRateType;	    // 码率类型：参考 IVS_BITRATE_TYPE
	IVS_UINT32	uiBitRate;		    // 码率，单位(kbps)
	IVS_UINT32  uiFrameRate;		// 帧率，单位(fps)，范围1-60

	IVS_CHAR	cReserve[32];
}IVS_STREAM_INFO;

// 视频流信息（包含事件触发图像质量调整信息）
typedef struct
{
	IVS_UINT32	uiStreamType;				// 码流类型：参考 IVS_STREAM_TYPE
	IVS_UINT32	uiEncodeType;				// 编码格式：参考 IVS_VIDEO_DEC_TYPE
	IVS_UINT32	uiProfile;						// 根据uiEncodeType确定，如果是H.264：参考IVS_H264_PROFILE；如果是MJPEG：参考IVS_MJPEG_PROFILE
	IVS_UINT32	uiFrameInterval;			// I帧间隔
	IVS_UINT32	uiPicQuality;					// 图像质量 （0-高 1-中 2-低） IVS_IMAGING_QUALITY
	IVS_UINT32	uiTransProtocol;			// 传输协议：参考 IVS_PROTOCOL_TYPE
	IVS_UINT32	uiPktProtocol;				// 打包协议：参考 IVS_PACK_PROTOCOL_TYPE
	IVS_UINT32	uiBitRateType;				// 码率类型：参考 IVS_BITRATE_TYPE
	IVS_UINT32	uiBitRate;						// 码率，单位(kbps)
	IVS_UINT32  uiFrameRate;				// 帧率，单位(fps)，范围1-60

	IVS_UINT32 uiEventRecordAdapt;	//事件触发图像质量调整，0：不触发，1：触发
	IVS_UINT32 uiEventBitRate;				//事件触发时的码率 单位(kbps)
	IVS_UINT32 uiEventFrameRate;		//事件触发时的帧率

	IVS_CHAR	cResolution[IVS_RESOLUTION_LEN];    // 分辨率，如“177*144”

	IVS_CHAR	cReserve[32];
}IVS_STREAM_INFO_EVENTEX;

// 视频子设备配置参数
#define IVS_MAX_STREAM_NUM	8
#define IVS_CAMERA_NAME_LEN 192 // 摄像机名称长度
typedef struct
{
    IVS_UINT32	uiVideoChannel;	                // 视频输入通道号

	IVS_CHAR    cCameraCode[IVS_DEV_CODE_LEN];	// 设备编码
	IVS_CHAR    cCameraName[IVS_CAMERA_NAME_LEN];		// 摄像机名称

	IVS_CAMERA_POSITION	stPosition;	            // 摄像头位置信息

	IVS_BOOL	bSupportBroadcast;		        // 是否支持组播 0: 否 1:是
	IVS_BROADCAST_INFO	stBroadcast;	        // 组播信息

	IVS_BOOL	bEnableVoice;	                    // 随路音频是否启用	0-停用 1-启用
    IVS_CHAR    cVoiceOutCode[IVS_DEV_CODE_LEN];    // 摄像机绑定的音频输出通道，只能为同主设备上的音频输出通道。

    // 非用户修改信息
    IVS_CHAR    cParentCode[IVS_DEV_CODE_LEN];  // 主设备编码
    IVS_CHAR	cNvrCode[IVS_NVR_CODE_LEN];     // 设备所属NVR编码, 表示该设备被该NVR管理

    IVS_UINT32	uiCameraType;	                // 摄像机类型 IVS_CAMERA_TYPE
    IVS_UINT32	uiCameraStatus;		            // 摄像机扩展状态：1 – 正常	2 – 视频丢失

	IVS_CHAR	cReserve[32];
}IVS_CAMERA_CFG;

// 摄像机码流信息
typedef struct
{
    IVS_UINT32		uiStreamInfoNum;	// 有效视频流信息数目
    IVS_STREAM_INFO	stStreamInfo[IVS_MAX_STREAM_NUM];	// 视频流信息

    IVS_CHAR	cReserve[32];
}IVS_CAMERA_STREAM_CFG;

// 摄像机码流信息（包含事件触发图像质量调整信息）
typedef struct
{
	IVS_UINT32		uiStreamInfoNum;																		// 有效视频流信息数目
	IVS_STREAM_INFO_EVENTEX	stStreamInfo[IVS_MAX_STREAM_NUM];	// 视频流信息（包含事件触发图像质量调整信息）

	IVS_CHAR	cReserve[32];
}IVS_CAMERA_STREAM_EVENTEX_CFG;

typedef enum
{
    CAMERA_FIX=0,       // 固定枪机 
    CAMERA_PTZ,         // 有云台枪机 
    CAMERA_DOME,        // 球机
    CAMERA_FIX_DOME,    // 半球-固定摄像机
}IVS_CAMERA_TYPE;

// 串口类型
typedef enum
{
    SERIAL_RS232=0, // RS232
    SERIAL_RS485=1, // RS485
    SERIAL_RS422=2  // RS422
}IVS_SERIAL_TYPE;

// 校验类型
typedef enum
{
    PARITY_TYPE_NONE=0,     // 无校验
    PARITY_TYPE_ODD=1,      // 奇校验
    PARITY_TYPE_EVEN=2,     // 偶校验
    PARITY_TYPE_MARK=3,     // 标记校验
    PARITY_TYPE_SPACE=4,    // 空校验
}IVS_PARITY_TYPE;

// 流控类型
typedef enum
{
    FLOW_CONTROL_NONE=0,    // 无流控
    FLOW_CONTROL_SOFT=1,    // 软流控
    FLOW_CONTROL_HARD=2,    // 硬流控
}IVS_FLOW_CONTROL_TYPE;

// 串口子设备信息
#define IVS_SERIAL_NAME_LEN 128  // 串口设备名称长度
typedef struct
{
    IVS_CHAR cSerialName[IVS_SERIAL_NAME_LEN];  // 串口设备名称，汉字和字母（a-z和A-Z），数字，中划线和下划线，1~20个字符。
    IVS_UINT32  uiSerialIndex;                  // 串口索引
    IVS_UINT32  uiSerialType;                   // 串口类型，值参考 IVS_SERIAL_TYPE
    IVS_UINT32  uiBaudRate;                     // 波特率(bps) (50, 75, 110, 150, 300, 600, 1200, 2400, 4800, 9600, 19200 38400, 57600, 115200)
    IVS_UINT32  uiDataBit;                      // 数据位 （5、6、7、8）
    IVS_UINT32  uiParity;                       // 校验，值参考IVS_PARITY_TYPE（0－无校验，1－奇校验，2－偶校验，3: 标记校验，4:空校验）
    IVS_UINT32  uiStopBit;                      // 停止位 （1、2）
    IVS_UINT32  uiFlowControl;                  // 流控，值参考 IVS_FLOW_CONTROL_TYPE

    IVS_CHAR	cReserve[32];
}IVS_SERIAL_CHANNEL_CFG;

// 主设备子设备通道（查询）
#define IVS_MAX_DEVICE_CHANNEL_NUM  256 //  设备通道最大数目
#define IVS_CHANNEL_NAME_LEN  128 //  通道名称长度
typedef struct
{
	IVS_CHAR	cChannelCode[IVS_DEV_CODE_LEN];	// 子设备编码
	IVS_UINT32	uiChannelType;		            // 通道类型：参考 IVS_CHANNEL_TYPE
    IVS_CHAR	cChannelName[IVS_CHANNEL_NAME_LEN];	// 通道名称
	IVS_CHAR	cReserve[32];
}IVS_DEV_CHANNEL_BRIEF_INFO;

// 主设备子设备列表
typedef struct
{
    IVS_UINT32   uiTotal;				        // 总记录数
    IVS_INDEX_RANGE stIndexRange;			    // 分页信息
    IVS_DEV_CHANNEL_BRIEF_INFO stDevChannelBriefInfo[1];   // 主设备子设备列表
}IVS_DEV_CHANNEL_BRIEF_INFO_LIST;

// 用户绑定设备信息
typedef struct
{
//	IVS_UINT32	uiDevGroupID;                               // 设备组ID
    IVS_CHAR	cDevGroupCode[IVS_DEVICE_GROUP_LEN];        // 设备组编码
	IVS_CHAR	cDevGroupDomainCode[IVS_DOMAIN_CODE_LEN];	// 设备组域编码
    IVS_CHAR	cDevCode[IVS_DEV_CODE_LEN];	                // 设备编码
    IVS_CHAR	cDevDomainCode[IVS_DOMAIN_CODE_LEN];	    // 设备域编码
    IVS_BOOL	bIsExDomain;	                            // 是否外部域设备/组   0：否，1：是
	IVS_BOOL	bIsTotalGroup;	// 0：绑定的是设备组中的设备	1：绑定整个设备组（设备组扩容后自动更新组内设备），此时：DevCode、DomainCode、ChanelType为空

    IVS_CHAR	cReserve[64];
}IVS_USER_DEVICE_INFO;

// 用户绑定设备信息列表
typedef struct
{
	IVS_UINT32   uiTotal;				        // 总记录数
	IVS_INDEX_RANGE stIndexRange;			    // 分页信息
	IVS_USER_DEVICE_INFO stUserDeviceInfo[1];   // 用户绑定设备信息列表
}IVS_USER_DEVICE_INFO_LIST;

// 用户绑定设备信息列表
typedef enum
{
    NET_TYPE_LAN = 0,   // 有线
    NET_TYPE_WAN = 1,   // 无线
}IVS_NET_TYPE;

// 视频子设备列表简要信息（查询视频子设备列表）
typedef struct
{
	IVS_CHAR	cCode[IVS_DEV_CODE_LEN];	    // 设备编码
	IVS_CHAR	cName[IVS_CAMERA_NAME_LEN];		// 摄像机名称

    IVS_CHAR	cDevGroupCode[IVS_DEVICE_GROUP_LEN];// 所属设备组编码
	IVS_CHAR	cParentCode[IVS_DEV_CODE_LEN];	    // 父设备编码
	IVS_CHAR	cDomainCode[IVS_DOMAIN_CODE_LEN];	// 域编码
    IVS_CHAR	cDevModelType[IVS_DEV_MODEL_LEN];	// 主设备型号

    IVS_CHAR	cVendorType[IVS_MAX_VENDOR_TYPE_NAME_LEN];  // 设备提供商类型: "HUAWEI" "HIK" "DAHUA"
    IVS_UINT32	uiDevFormType;                              // 主设备类型：参考 IVS_MAIN_DEVICE_TYPE

	IVS_UINT32	uiType;								// 摄像机类型 IVS_CAMERA_TYPE
	IVS_CHAR	cCameraLocation[IVS_DESCRIBE_LEN];	// 摄像机安装位置描述
	IVS_UINT32	uiCameraStatus;		                // 摄像机扩展状态：1 – 正常	2 – 视频丢失

	IVS_UINT32	uiStatus;			                // 设备状态：0-离线，1-在线，2-休眠 参考 IVS_DEV_STATUS

    IVS_UINT32  uiNetType;                          // 网络类型 0-有线  1-无线, 参考 IVS_NET_TYPE
    IVS_BOOL    bSupportIntelligent;                // 是否支持智能分析  0-不支持 1-支持

	IVS_BOOL	bEnableVoice;		                // 随路音频是否启用	0-停用 1-启用
	IVS_CHAR	cNvrCode[IVS_NVR_CODE_LEN];	        // 设备所属NVR编码, 表示该设备被该NVR管理

    IVS_CHAR    cDevCreateTime[IVS_TIME_LEN];       // 设备创建时间
    IVS_BOOL	bIsExDomain;                        // 是否为外部域 0-否  1-是
    IVS_CHAR    cDevIp[IVS_IP_LEN];                 // 前端IP

	IVS_CHAR	cReserve[32];
}IVS_CAMERA_BRIEF_INFO;

// 视频子设备列表
typedef struct
{
	IVS_UINT32   uiTotal;           // 总记录数
	IVS_INDEX_RANGE stIndexRange;   // 分页信息
	IVS_CAMERA_BRIEF_INFO stCameraBriefInfo[1];
}IVS_CAMERA_BRIEF_INFO_LIST;

//扩展的查询操作日志条件
typedef struct
{
	IVS_CHAR cLoginID[IVS_NAME_LEN];					//登录OMU的ID
	IVS_CHAR cNVRCode[IVS_NVR_CODE_LEN];		//待查询的NVRCode
	IVS_CHAR cModuleName[IVS_DESCRIBE_LEN];	//模块类型,譬如"SMU","SCU"和"OMU"三选一
	IVS_CHAR cPortalClientIP[IVS_IP_LEN];					//门户客户端IP
}IVS_QUERY_OPTLOG_EX;

// 操作日志
typedef struct
{
	IVS_CHAR    cAccount[IVS_NAME_LEN];                 //操作员账号
	IVS_CHAR    cClientIp[IVS_IP_LEN];                  //客户端IP
	IVS_UINT32  iGrade;                                 //日志级别
	IVS_CHAR    cServiceCode[IVS_DESCRIBE_LEN];         //业务代码，IVS平台写"IVS"
	IVS_CHAR    cModuleType[IVS_DESCRIBE_LEN];          //模块类型,譬如"SMU","SCU"
	IVS_CHAR    cModuleCode[IVS_DEV_CODE_LEN];          //模块唯一代码，指当前模块对应的机器编号
	IVS_CHAR    cOperationCode[IVS_DEV_CODE_LEN];       //操作代码用来唯一标示一个操作
	IVS_CHAR    cOperationObjectCode[IVS_DESCRIBE_LEN]; //操作对象代码，用来标示本次操作实施的对象
	IVS_CHAR    cOccurTime[IVS_TIME_LEN];               //日志发生时间
	IVS_CHAR    cErrorCode[IVS_DESCRIBE_LEN];           //错误码
	IVS_UINT32  iResult;                                //操作结果
	IVS_CHAR    cAdditionalInfo[IVS_DESCRIBE_LEN];      //附加信息
    IVS_CHAR	cReserve[32];
}IVS_OPERATION_LOG_INFO;

// 操作日志列表
typedef struct
{
	IVS_UINT32   uiTotal;           // 总记录数
	IVS_INDEX_RANGE stIndexRange;   // 分页信息
	IVS_OPERATION_LOG_INFO stOperationInfo[1];
}IVS_OPERATION_LOG_LIST;

// 功能代码
typedef enum
{
	FUNC_LOGIN= 1,	// 登录
	FUNC_MAIN_UI,	// 主界面
	FUNC_GROUP_MANAGER,	// 组织管理
	// 待添加..<eSpace IVS V100R001C02 用户操作功能和代码列表.xlsx>
}IVS_FUNCTION_CODE;

/////////////////////////////////////////////////告警///////////////////////////////////////////
#define IVS_ALARM_DESCRIPTION_LEN 1024
#define IVS_ALARM_CODE_LEN		64
#define IVS_ALARM_NAME_LEN		260
#define IVS_NODE_TYPE_LEN       8

// 告警级别
#define IVS_MAX_ALARM_LEVEL_NUM	20	// 告警级别最大个数
typedef struct 
{
	IVS_UINT32  uiAlarmLevelId;		// 告警级别ID

	IVS_CHAR    cName[IVS_NAME_LEN];	// 告警级别名称,汉字和字母（a-z和A-Z），数字，中划线和下划线，1~20个字符
	IVS_UINT32  uiValue;				// 告警级别权值,1~100
	IVS_CHAR    cColor[16];			    // 告警级别颜色,使用颜色字符串表示ARGB,例如#FFFF0000 表示红色，不透明
	IVS_CHAR    cDescription[IVS_ALARM_DESCRIPTION_LEN];	// 告警级别描述,键盘可见字符和中文，0~256个字符
	IVS_BOOL    bIsUserDefined;		    // 是否为用户自定义 0-否，表示“系统定义” 1-是

	IVS_CHAR	cReserver[32];
}IVS_ALARM_LEVEL;

// 告警类型
#define IVS_ALARM_TYPE_CATEGORY_LEN  8
typedef struct
{
    IVS_UINT32  uiID;                   // 告警ID
	IVS_CHAR cType[IVS_ALARM_CODE_LEN];	// 告警类型编码
	IVS_CHAR cName[IVS_ALARM_NAME_LEN];	// 告警类型名称，汉字和字母（a-z和A-Z），数字，中划线和下划线，1~64个字符

	IVS_UINT32	uiType;					// 告警源类型 IVS_ALARM_IN_TYPE
	IVS_CHAR cCategory[IVS_ALARM_TYPE_CATEGORY_LEN];				// 告警类型类别
/*    
告警类型类别（支持四级类别）
01：业务类告警
0101：一般业务告警
0102：智能分析告警
010201：行为分析告警
010202：人脸识别告警
0103：第三方接入告警
02：设备类告警
0201：一般设备告警
0202：智能分析告警
*/
    IVS_BOOL    bIsUserDefined;		    // 是否为用户自定义 0-否，表示“系统定义” 1-是

	IVS_ALARM_LEVEL stAlarmLevel;	    // 告警级别ID

    IVS_BOOL    bEnable;                // 是否启用 0-否 1-是
	IVS_CHAR	cReserver[32];
}IVS_ALARM_TYPE;

// 告警类型分页查询列表
typedef struct
{
	IVS_UINT32   iTotal;                // 总记录数
	IVS_INDEX_RANGE stIndexRange;       // 分页信息
	IVS_CHAR cReserve[32];              //保留字段
	IVS_ALARM_TYPE stSubscribeInfo[1];  // 告警类型信息列表
}IVS_ALARM_TYPE_LIST;

typedef enum
{
	ALARM_IN_CAMERA=1,      // 摄像头
	ALARM_IN_DI=2,          // 开关量输入设备
    ALARM_IN_EXTERN_DI=9,   // 第三方开关量输入
}IVS_ALARM_IN_TYPE;

// 开关量输出设备
typedef struct
{
	IVS_CHAR cCode[IVS_ALARM_CODE_LEN];	// 开关量输出编码
	IVS_CHAR cName[IVS_ALARM_NAME_LEN]; // 开关量输出名称

	IVS_CHAR	cReserver[32];
}IVS_ALARM_OUT;

// 告警处理信息
typedef struct
{
	IVS_UINT32	uiOperatorID;	                // 处理人ID 参考 IVS_USER_INFO
    IVS_CHAR    cOperatorName[IVS_NAME_LEN];	// 处理人名
	IVS_CHAR    cOperateTime[IVS_TIME_LEN];		// 告警处理时间

    IVS_CHAR	cReserver[32];
    IVS_CHAR    cOperateInfo[IVS_ALARM_DESCRIPTION_LEN];	// 告警处理人员输入的信息
}IVS_ALARM_OPERATE_INFO;

// 告警状态
typedef enum
{
	ALARM_STATUS_NO_OPERATE=0,	// 未操作
	ALARM_STATUS_TOBE_CONFIRM,	// 待确认
	ALARM_STATUS_CONFIRMED,	    // 已确认
	ALARM_STATUS_CANCELED,	    // 已撤销
	ALARM_STATUS_RECOVERED,	    // 已恢复
}IVS_ALARM_STATUS;

// 告警源设备
typedef struct
{
	IVS_CHAR cAlarmInCode[IVS_ALARM_CODE_LEN];	// 告警源编码
	IVS_CHAR cAlarmInName[IVS_ALARM_NAME_LEN];  // 告警源名称
	IVS_UINT32	uiAlarmInType;					// 告警源类型 IVS_ALARM_IN_TYPE

	IVS_UINT32	uiDeviceType;	                // 参考 IVS_CAMERA_TYPE 设备类型（目前只有摄像机区分设备类型，其余设备留空）
	IVS_BOOL	bOnline;		                // 是否在线：1-在线	0-离线
	IVS_BOOL	bEnable;		                // 是否启用：1-启用	0-停用
	IVS_CAMERA_POSITION	stLocation;		        // 	设备安装位置，对于没有位置信息的显示为空

	IVS_CHAR	cReserver[32];
}IVS_ALARM_IN;

// 告警源设备列表
typedef struct
{
    IVS_UINT32   iTotal;            // 总记录数
    IVS_INDEX_RANGE stIndexRange;   // 分页信息
    IVS_CHAR cReserve[32];          //保留字段
    IVS_ALARM_IN stAlarmInInfo[1];  // 告警源设备列表
}IVS_ALARM_IN_LIST;

// 告警上报信息(客户端等模块上报平台)
#define IVS_ALARM_EX_PARAM_LEN  2048    // 告警上报扩展参数
#define IVS_ALARM_REPORT_ID_LEN 32      // 告警上报ID
#define IVS_ALARM_TYPE_LEN  64          // 告警类型长度
typedef struct 
{
    // 告警源设备
    IVS_CHAR    cAlarmInCode[IVS_ALARM_CODE_LEN];	// 告警源编码
    IVS_CHAR	cDevDomainCode[IVS_DOMAIN_CODE_LEN];// 设备所属域编码
    IVS_CHAR    cAlarmType[IVS_ALARM_TYPE_LEN];		// 告警源类型 英文字符串
    IVS_CHAR    cOccurTime[IVS_TIME_LEN];           // 告警发生时间：yyyyMMddHHmmss
    IVS_CHAR    cReportTime[IVS_TIME_LEN];          // 告警上报时间：yyyyMMddHHmmss
    
    IVS_BOOL    bAction;                                // 0-告警消除 1-告警
    IVS_CHAR    cAlarmID[IVS_ALARM_REPORT_ID_LEN];      // 告警ID，“告警消除”与“告警”时的告警ID相同

    IVS_UINT32  uiOccurNumber;                          // 告警发生次数，默认值1
    IVS_CHAR    cNvrCode[IVS_NVR_CODE_LEN];             // NVR编码，可用于更新NVR路由

    IVS_CHAR    cReserve[32];                           // 保留字段
    IVS_CHAR    cAlarmDesc[IVS_ALARM_DESCRIPTION_LEN];  // 告警描述信息，键盘可见字符和中文，1024字节
    IVS_CHAR    cExtParam[IVS_ALARM_EX_PARAM_LEN];      // 扩展参数
}IVS_ALARM_REPORT;

#define IVS_ALARM_LEVEL_COLOR_LEN   16
#define IVS_ALARM_CATEGORY_LEN   8

// 业务告警通知信息(平台通知客户端)
typedef struct 
{
    IVS_UINT64	ullAlarmEventID;                    // 告警事件ID
    IVS_CHAR    cAlarmInCode[IVS_ALARM_CODE_LEN];	// 告警源编码
    IVS_CHAR	cDevDomainCode[IVS_DOMAIN_CODE_LEN];// 设备所属域编码
    IVS_UINT32  uiAlarmInType;					    // 告警源类型 IVS_ALARM_IN_TYPE
    IVS_CHAR    cAlarmInName[IVS_ALARM_NAME_LEN];   // 告警源名称

    IVS_UINT32  uiAlarmLevelValue;		        // 告警级别权值,1~100
    IVS_CHAR    cAlarmLevelName[IVS_NAME_LEN];	// 告警级别名称,汉字和字母（a-z和A-Z），数字，中划线和下划线，1~20个字符
    IVS_CHAR    cAlarmLevelColor[IVS_ALARM_LEVEL_COLOR_LEN];			// 告警级别颜色,使用颜色字符串表示ARGB,例如#FFFF0000 表示红色，不透明

    IVS_CHAR    cAlarmType[IVS_ALARM_TYPE_LEN];	    // 告警类型
    IVS_CHAR    cAlarmTypeName[IVS_ALARM_NAME_LEN];	// 告警类型名称，汉字和字母（a-z和A-Z），数字，中划线和下划线，1~64个字符
    IVS_CHAR    cAlarmCategory[IVS_ALARM_CATEGORY_LEN];				// 告警类型类别

    IVS_CHAR    cOccurTime[IVS_TIME_LEN];       // 告警发生时间：yyyyMMddHHmmss
    IVS_UINT32 uiOccurNumber;		            // 告警发生次数
    IVS_UINT32  uiAlarmStatus;		            // 告警状态 参考 IVS_ALARM_STATUS

    IVS_BOOL	bIsCommission;	                // 是否为授权告警信息：0-否，1-是
    IVS_CHAR    cPreviewUrl[IVS_URL_LEN];	    // 在存在联动抓拍或者智能分析时的图片预览URL

    IVS_BOOL	bExistsRecord;	                // 是否存在告警录像：0-否，1-是
    IVS_CHAR    cNvrCode[IVS_NVR_CODE_LEN];     // NVR编码，可用于更新NVR路由

    IVS_CHAR	cReserver[32];                  // 保留字段
    IVS_CHAR    cAlarmDesc[IVS_ALARM_DESCRIPTION_LEN];	// 告警描述信息，键盘可见字符和中文，0~256字符。
    IVS_CHAR    cExtParam[IVS_ALARM_EX_PARAM_LEN];      // 扩展参数
}IVS_ALARM_NOTIFY;

// 告警信息（告警事件查询）
typedef struct 
{
    IVS_ALARM_NOTIFY        stAlarmNotify;      // 告警信息
    IVS_ALARM_OPERATE_INFO	stOperateInfo;	    // 告警处理信息
}IVS_ALARM_EVENT;

// 告警信息状态通知
typedef struct 
{
    IVS_UINT64	ullAlarmEventID;                        // 告警事件ID
    IVS_CHAR    cAlarmInCode[IVS_ALARM_CODE_LEN];	    // 告警源编码
    IVS_CHAR	cDevDomainCode[IVS_DOMAIN_CODE_LEN];    // 告警源域编码
    IVS_CHAR    cAlarmType[IVS_ALARM_TYPE_LEN];	        // 告警类型

    IVS_UINT32  uiOccurNumber;		            // 告警发生次数
    IVS_UINT32  uiAlarmStatus;		            // 告警状态 参考 IVS_ALARM_STATUS    

    IVS_CHAR	cReserve[32];                   // 保留字段

    IVS_ALARM_OPERATE_INFO	stOperateInfo;	    // 告警处理信息
}IVS_ALARM_STATUS_NOTIFY;

// 告警上报信息分页查询列表
typedef struct
{
    IVS_UINT32   iTotal;                    // 总记录数
    IVS_INDEX_RANGE stIndexRange;           // 分页信息
    IVS_CHAR cReserve[32];                  //保留字段
    IVS_ALARM_EVENT stSubscribeInfo[1];    // 告警通知信息
}IVS_ALARM_EVENT_LIST;

// 设备告警通知信息(OMU告警通知信息)
#define IVS_DEVICE_ALARM_LOCATION_INFO_LEN  256
#define IVS_NET_ELEMENT_ID_LEN  32
typedef struct 
{
    IVS_UINT64	ullAlarmEventID;                    // 告警事件ID
    IVS_CHAR    cAlarmInCode[IVS_ALARM_CODE_LEN];	// 告警源编码
    IVS_UINT32  uiAlarmInType;					    // 告警源类型 IVS_ALARM_IN_TYPE
    IVS_CHAR    cAlarmInName[IVS_ALARM_NAME_LEN];   // 告警源名称
    IVS_CHAR    cNodeType[IVS_NODE_TYPE_LEN];       // 告警所属网元类型,如30858

    IVS_UINT32  uiAlarmLevelValue;		        // 告警级别权值，只有四种：1-提示；50-一般；80-重要；100-紧急；

    IVS_CHAR    cAlarmType[IVS_ALARM_CODE_LEN];	// 告警类型
    IVS_CHAR    cAlarmCategory[IVS_ALARM_CATEGORY_LEN];				// 告警类型类别 01：业务类告警 02：网管类告警 03：状态类告警

    IVS_CHAR    cOccurFirstTime[IVS_TIME_LEN];     // 告警首次发生时间：yyyyMMddHHmmss（告警抑制的第一次告警产生时间）
    IVS_CHAR    cOccurEndTime[IVS_TIME_LEN];       // 告警最后发生时间：yyyyMMddHHmmss（告警抑制的最后一次告警产生时间）

    IVS_UINT32 uiOccurNumber;		            // 告警发生次数

    IVS_CHAR	cReserver[32];                  // 保留字段
    // 告警定位信息。定位信息字段有格式要求，需要是“以逗号分隔的键值对”的形式，譬如：ip=10.137.97.123,hostname=linux,...可以为空，不能有中文
    IVS_CHAR    cLocationInfo[IVS_DEVICE_ALARM_LOCATION_INFO_LEN];
    IVS_CHAR    cNetEleID[IVS_NET_ELEMENT_ID_LEN];      // 告警产生模块归属的网元ID

	IVS_CHAR    cNetName[IVS_NAME_LEN];                 // 所属网元名称
    IVS_CHAR    cAlarmDest[IVS_ALARM_DESCRIPTION_LEN];  // 告警描述信息，键盘可见字符和中文，0~256字符。
    IVS_CHAR    cExtParam[IVS_ALARM_EX_PARAM_LEN];      // 扩展参数
}IVS_DEVICE_ALARM_NOTIFY;

// 设备告警信息分页查询列表
typedef struct
{
    IVS_UINT32   iTotal;                    // 总记录数
    IVS_INDEX_RANGE stIndexRange;           // 分页信息
    IVS_CHAR cReserve[32];                  //保留字段
    IVS_DEVICE_ALARM_NOTIFY stDeviceAlarmNotify[1];    // 告警通知信息
}IVS_DEVICE_ALARM_EVENT_LIST;

// 用户ID信息
typedef struct
{
    IVS_UINT32	uiUserID;	                        // 被授权用户ID
    IVS_CHAR	cDomainCode[IVS_DOMAIN_CODE_LEN];	// 用户所属域编码
}IVS_USER_ID_INFO;

// 告警授权
typedef struct 
{
    IVS_UINT64	ullAlarmEventID;	// 告警事件ID
    IVS_UINT32  uiUserNum;          // 被授权用户数
    IVS_USER_ID_INFO    stUserInfo[1];  // 被授权用户ID列表
}IVS_ALARM_COMMISSION;

// 订阅者类型
typedef enum
{
	SUBSCRIBER_TYPE_USER= 1,	// 用户订阅
	SUBSCRIBER_TYPE_DOMAIN= 2,	// 外域向本域订阅
}IVS_SUBSCRIBE_USER_TYPE;

// 告警订阅者信息
typedef struct 
{
	IVS_UINT32	uiSubscriberType;	// 订阅者类型：参考 IVS_SUBSCRIBE_USER_TYPE
	IVS_UINT32	uiSubscriberId;		// 订阅者ID（用户ID/域ID）

	IVS_CHAR	cReserve[32];
}IVS_ALARM_SUBSCRIBER;

// 订阅方式
typedef enum
{
	SUBSCRIBE_ALL= 1,       // 所有
	SUBSCRIBE_BY_TYPE= 2,   // 按告警类型
	SUBSCRIBE_BY_LEVEL= 3,  // 按告警级别
}IVS_SUBSCRIBE_TYPE;

// 告警订阅信息（一个用户可以订阅多个告警源，每个告警源可以订阅多个告警类型）
#define IVS_MAX_ALARM_TYPE_NUM  32  // 最大告警类型个数
typedef struct 
{
	IVS_CHAR cCode[IVS_ALARM_CODE_LEN];	// 告警源编码
	IVS_UINT32	uiSubscribeType;        // 订阅方式：参考 IVS_SUBSCRIBE_TYPE
	IVS_UINT32	uiAlarmLevelValueMin;	// 告警级别下限值，订阅方式为“3-按告警级别”必须，可以为空
	IVS_UINT32	uiAlarmLevelValueMax;	// 告警级别上限值，订阅方式为“3-按告警级别”必须，可以为空

	IVS_CHAR	cReserve[32];

	IVS_UINT32	uiAlarmTypeNumber;	// 实际告警类型订阅个数
	IVS_ALARM_TYPE	stAlaryType[IVS_MAX_ALARM_TYPE_NUM];	// 告警类型数据
}IVS_ALARM_SUBSCRIBE_INFO;

// 告警订阅信息分页查询列表
typedef struct
{
	IVS_UINT32   iTotal;            // 总记录数
	IVS_INDEX_RANGE stIndexRange;   // 分页信息
	IVS_CHAR cReserve[32];                          //保留字段
	IVS_ALARM_SUBSCRIBE_INFO stSubscribeInfo[1];    // 告警订阅信息
}IVS_ALARM_SUBSCRIBE_INFO_LIST;

typedef enum
{
	HELP_SOURCE_AREA= 1,	// 防区
	HELP_SOURCE_TYPE= 2,	// 告警类型
}IVS_ALARM_HELP_SOURCE;

// 防区信息
#define IVS_MAX_ALARM_IN_NUM	512	// 最大防区个数
typedef struct 
{
	IVS_UINT32	uiAlarmAreaID;	                    // 防区的ID
	IVS_CHAR	cAlarmAreaName[IVS_ALARM_NAME_LEN];	// 防区名称
	IVS_BOOL	bGuard;			                    // 是否布防	0-撤防	1-布防
    IVS_CHAR	cReserve[32];

	IVS_UINT32      uiAlarmInNum;	                    // 防区告警源实际个数
	IVS_ALARM_IN    stAlarmIn[IVS_MAX_ALARM_IN_NUM];	// 防区告警源列表
}IVS_ALARM_AREA;

// 告警订阅信息分页查询列表
typedef struct
{
	IVS_UINT32   iTotal;                // 总记录数
	IVS_INDEX_RANGE stIndexRange;       // 分页信息
	IVS_CHAR cReserve[32];              //保留字段
	IVS_ALARM_AREA stAlarmAreaInfo[1];  // 告警订阅信息
}IVS_ALARM_AREA_LIST;

#define IVS_MAX_HELP_LEN	40960
// 帮助信息
typedef struct 
{
	IVS_UINT32	uiSourceType;					// 帮助信息源类型：参考 IVS_ALARM_HELP_SOURCE
	IVS_UINT32	uiSourceId;						// 防区/告警类型ID	
	IVS_CHAR	cSourceName[IVS_ALARM_NAME_LEN];// 防区/告警类型名称
	IVS_CHAR	cHelpInfo[IVS_MAX_HELP_LEN];	// 帮助信息正文，支持HTML，最大支持10000文字
	IVS_CHAR	cUserName[IVS_NAME_LEN];		// 更新帮助信息用户名
	IVS_CHAR	cUpdateTime[IVS_TIME_LEN];		// 编辑时间
    IVS_CHAR	cReserve[64];
}IVS_HELP_INFO;

// 帮助信息列表
typedef struct
{
    IVS_UINT32   uiTotal;           // 总记录数
    IVS_INDEX_RANGE stIndexRange;   // 分页信息
    IVS_HELP_INFO stHelpInfo[1];    // 帮助信息
}IVS_HELP_INFO_LIST;

// 联动策略>告警动作类型
typedef enum
{
    ACTION_TYPE_INTERACTIVE=1,      // 用户交互
    ACTION_TYPE_PLATFORM_RECORD,    // 联动录像
    ACTION_TYPE_PU_RECORD,          // 联动前端录像
    ACTION_TYPE_SNAP,               // 联动抓拍
	ACTION_TYPE_PRESET=5,           // 联动预置位

	ACTION_TYPE_IO,                 // 联动开关量输出
    ACTION_TYPE_LIVE,               // 联动客户端弹出实况
    ACTION_TYPE_TV,                 // 联动实况上墙
    ACTION_TYPE_EMAIL,              // 联动EMAIL
    ACTION_TYPE_SMS=10,             // 联动短信
	ACTION_TYPE_MMS,                // 联动彩信
    ACTION_TYPE_BACKUP_RECORD,      // 联动录像备份
}IVS_LINKAGE_ACTION_TYPE;

// 水印告警信息
typedef struct
{
    IVS_CHAR    cCameraCode[IVS_DEV_CODE_LEN]; // 摄像机编码
    IVS_UINT64  ullAlarmEventID;               // 告警ID
    IVS_CHAR    cStartTime[IVS_TIME_LEN];      // 告警开始时间
    IVS_CHAR    cEndTime[IVS_TIME_LEN];        // 告警结束时间
    IVS_CHAR	cReserve[32];
}IVS_ALARM_WATERMARK_INFO;

// 联动策略>执行分支
typedef enum
{
	ACTION_BRANCH_ALWAYS = 0,		// 总是执行
	ACTION_BRANCH_AFTER_CONFIRM,	// 确认后执行
	ACTION_BRANCH_AFTER_TIMEOUT,	// 延时未确认执行
}IVS_LINKAGE_ACTION_BRANCH;

// 联动策略>设备信息
#define IVS_LINKAGE_DEV_PARAM_LEN     128  // 联动设备动作参数长度
typedef struct
{
	IVS_CHAR	cDevCode[IVS_DEV_CODE_LEN];	            // 设备编码
    IVS_CHAR    cDevDomainCode[IVS_DOMAIN_CODE_LEN];    // 设备所在域编码
	IVS_CHAR    cParam[IVS_LINKAGE_DEV_PARAM_LEN];      // 预置位 动作参数
    IVS_CHAR	cReserve[32];
}IVS_LINKAGE_DEV_INFO;

// 联动策略>用户信息
typedef struct
{
	IVS_UINT32  uiUserId;       // 用户ID
    IVS_CHAR	cReserve[32];
}IVS_LINKAGE_USER_INFO;

// 告警-》动作-》
//	摄像机
//	用户

// 告警联动策略联动动作
#define IVS_LINKAGE_GLOBAL_PARAM_LEN        1024// 联动全局动作参数长度
#define IVS_MAX_LINKAGE_ACTION_DEVICE_NUM   8   // 执行动作的设备最大个数
#define IVS_MAX_LINKAGE_ACTION_USER_NUM     32  // 告警联动目标用户的最大个数
typedef struct
{
	IVS_UINT32 uiActionID;                                     // 联动动作ID
	IVS_UINT32 uiActionType;                                   // 告警动作类型： 参考 IVS_LINKAGE_ACTION_TYPE
	IVS_CHAR	cGlobalParam[IVS_LINKAGE_GLOBAL_PARAM_LEN];	   // 全局动作参数
	IVS_UINT32	uiActionBranch;                                // 执行分支：参考 IVS_LINKAGE_ACTION_BRANCH
	IVS_UINT32  uiDevInfoNumber;                               // 执行动作的设备实际个数
	IVS_LINKAGE_DEV_INFO stDevInfo[IVS_MAX_LINKAGE_ACTION_DEVICE_NUM];  // 执行动作的设备列表
	IVS_UINT32  uiUserInfoNumber;								// 执行动作的目标实际用户数
	IVS_UINT32 uiUserID[IVS_MAX_LINKAGE_ACTION_USER_NUM];		// 动作目标用户列表
    IVS_CHAR	cReserve[32];
}IVS_LINKAGE_ACTION;

// 告警联动策略
typedef struct
{
	IVS_UINT32 uiID;                            // 联动策略ID
//	IVS_CHAR cLinkageName[IVS_ALARM_NAME_LEN];	// 联动策略名称
	IVS_CHAR cAlarmInCode[IVS_ALARM_CODE_LEN];	// 告警源编码
	IVS_CHAR cAlarmType[IVS_ALARM_CODE_LEN];	// 告警类型编码
	IVS_CHAR cReserve[32];
	IVS_UINT32 uiActionNumber;                  // 联动动作个数
	IVS_LINKAGE_ACTION stAction[1];             // 联动动作列表
}IVS_LINKAGE_POLICY;

// 告警联动策略列表
typedef struct
{
    IVS_UINT32   uiTotal;               // 总记录数
    IVS_INDEX_RANGE stIndexRange;       // 分页信息
    IVS_LINKAGE_POLICY stLinkagePolicy[1];  // 告警联动策略列表
}IVS_LINKAGE_POLICY_LIST;

// 告警联动动作停止通知
typedef struct 
{
    IVS_CHAR    cSrcDomainCode[IVS_DOMAIN_CODE_LEN];            // 动作源域编码，为可选，不带该字段时表示为本域
    IVS_UINT64	ullAlarmEventID;                                // 告警事件ID，和SrcDomainCode组成唯一值
    IVS_UINT32  uiActionType;                                   // 告警动作类型： 参考 IVS_LINKAGE_ACTION_TYPE
    IVS_CHAR	cGlobalParam[IVS_LINKAGE_GLOBAL_PARAM_LEN];	    // 全局动作参数

    IVS_UINT32  uiDevInfoNumber;                                // 执行动作的设备实际个数
    IVS_LINKAGE_DEV_INFO stDevInfo[IVS_MAX_LINKAGE_ACTION_DEVICE_NUM];  // 执行动作的设备列表
    IVS_UINT32  uiUserInfoNumber;								// 执行动作的目标实际用户数
    IVS_UINT32  uiUserID[IVS_MAX_LINKAGE_ACTION_USER_NUM];		// 动作目标用户列表
    IVS_CHAR	cReserve[32];
}IVS_STOP_LINKAGE_ACTION;

// 手动录像状态通知
typedef struct 
{
    IVS_CHAR    cCameraCode[IVS_DEV_CODE_LEN];      // 摄像机编码
    IVS_UINT32	uiUserID;                           // 用户ID
    IVS_UINT32	uiRecordMethod;                     // 录像方式：值参考IVS_RECORD_METHOD
    IVS_UINT32	uiManualRecordState;                // 手动录像状态：0-停止1-启动
    IVS_UINT32	uiNotifInfo;                        // 停止原因，停止时有效：0-手动停止 1-超时停止 2-异常停止
    IVS_CHAR	cReserve[32];
}IVS_MANUAL_RECORD_STATUS_NOTIFY;

// 策略属性
typedef struct
{
//	char          Id[POLICYID_LEN + 1]; // 策略ID
	unsigned char Type;        			// 0x00: 禁止，0x01: 连续，0x02: 事件触发(加一等于数据库表里的值)
	unsigned char Week;        			// 周标志, 1 ~ 8分别对应周日 ~ 周六
	unsigned char Once;        			// 表示该策略只执行一次
	IVS_CHAR	StartTime[IVS_TIME_LEN];// 开始时间，国际标准时间, 只有时分秒
	IVS_CHAR	EndTime[IVS_TIME_LEN];  // 结束时间，国际标准时间, 只有时分秒
	unsigned char isEnable;     		// 是否启用：0-未启用，1-启用
    IVS_CHAR	cReserve[32];
}IVS_STRAT_PROPERTY;

// 字典信息
#define IVS_DICTIONARY_NAME_LEN     64      // 字段名称长度
#define IVS_DICTIONARY_VALUE_LEN    256     // 字典值长度
#define IVS_DICTIONARY_TYPE_LEN     32      // 字典类型长度
#define IVS_DICTIONARY_REMARK_LEN   256      // 备注长度
typedef struct
{
    IVS_UINT32  uiID;                                   // 记录唯一ID
    IVS_CHAR    cDictName[IVS_DICTIONARY_NAME_LEN];     // 字典名称（英文字母和符号）
    IVS_CHAR    cDictValue[IVS_DICTIONARY_VALUE_LEN];   // 字典值，支持中文、英文
    IVS_CHAR    cDictType[IVS_DICTIONARY_TYPE_LEN];     // 字典类型
    IVS_UINT32  uiParentID;                             // 父节点ID
    IVS_CHAR    cRemark[IVS_DICTIONARY_REMARK_LEN];     // 备注

    IVS_CHAR cReserve[32];                              //保留字段
}IVS_DICTIONARY_INFO;

// 字典信息列表
typedef struct
{
    IVS_UINT32   uiTotal;                       // 总记录数
    IVS_INDEX_RANGE stIndexRange;               // 分页信息
    IVS_DICTIONARY_INFO stDictionaryInfo[1];    // 字典信息
}IVS_DICTIONARY_INFO_LIST;

////////////////////////////////////////MLib相关的头文件////////////////////////////////////////////
// 业务类型 ServiceType;
typedef enum
{
    SERVICE_TYPE_INVALID            = 0,
    SERVICE_TYPE_REALVIDEO          = 0x0001,   // 实况;
    SERVICE_TYPE_RECORD             = 0x0002,   // 平台录像;
    SERVICE_TYPE_DOWNLOAD           = 0x0003,   // 本地录像下载;
    SERVICE_TYPE_PLAYBACK           = 0x0004,   // 录像回放;
    SERVICE_TYPE_AUDIO_CALL         = 0x0005,   // 音频对讲;
    SERVICE_TYPE_AUDIO_BROADCAST    = 0x0006,   // 音频广播; 
    SERVICE_TYPE_PU_DOWNLOAD        = 0x0007,   // 前端录像下载
    SERVICE_TYPE_PU_PLAYBACK        = 0x0008,   // 前端录像回放

    SERVICE_TYPE_REALVIDEO_BACKUP		= 0x0014,		// 实况备份
    SERVICE_TYPE_PLATRECORD_BACKUP		= 0x0015,		// 平台录像备份
    SERVICE_TYPE_PURECORD_BACKUP		= 0x0016,		// 前端录像备份
    SERVICE_TYPE_BACKUPRECORD_PLAYBACK	= 0x0017,		// 录像备份回放
    SERVICE_TYPE_BACKUPRECORD_DOWNLOAD	= 0x0018,		// 录像备份下载
    SERVICE_TYPE_DISASTEBACKUP_PLAYBACK = 0x0019,       // 容灾录像回放
    SERVICE_TYPE_DISASTEBACKUP_DOWNLOAD = 0x001A,       // 容灾录像下载
    SERVICE_TYPE_PU_UPLOAD              = 0x001B,       // 前端缓存补录

    SERVICE_TYPE_MAX
}IVS_SERVICE_TYPE;

// 播发类型;
typedef enum
{
    BROADCAST_UNICAST   = 0x0000,   // 单播;
    BROADCAST_MULTICAST = 0x0001    // 组播;
}IVS_BROADCAST_TYPE;

// 连接类型;
typedef enum
{
    MEDIA_TRANS     = 0x0000, // 中转
    MEDIA_DIRECT    = 0x0001, // 直连;    
}IVS_MEDIA_TRANS_MODE;

// 传输协议;
typedef enum
{
    PROTOCOL_RTP_OVER_UDP = 0x0001, //RTP over UDP
    PROTOCOL_RTP_OVER_TCP = 0x0002, //RTP over TCP
    PROTOCOL_UDP          = 0x0003, //UDP
    PROTOCOL_TCP          = 0x0004  //TCP
}IVS_PROTOCOL_TYPE;

// 打包协议类型;
typedef enum
{
    PACK_PROTOCOL_ES     = 0x0001,
    PACK_PROTOCOL_PS     = 0x0002,
    PACK_PROTOCOL_TS     = 0x0003
}IVS_PACK_PROTOCOL_TYPE;

// 视频解码类型;
typedef enum
{
    VIDEO_DEC_H264              = 0x0001,
    VIDEO_DEC_MJPEG             = 0x0002,
    VIDEO_DEC_MPEG4             = 0x0003,  
    VIDEO_DEC_MPEG2             = 0x0004,
    VIDEO_DEC_AVS               = 0x0005
}IVS_VIDEO_DEC_TYPE;

// H264 Profile
typedef enum
{
    H264_PROFILE_BASELINE= 1,	// BP
    H264_PROFILE_EXTENDED,		// EP
    H264_PROFILE_MAIN,			// MP
    H264_PROFILE_HIGH,			// HP
}IVS_H264_PROFILE;

// MJPEG Profile
typedef enum
{
	MJPEG_PROFILE_SP= 1,	// SP
	MJPEG_PROFILE_ASP,		// ASP
}IVS_MJPEG_PROFILE;

// 音频解码类型;
typedef enum
{
    AUDIO_DEC_G711A             = 0x0001,
    AUDIO_DEC_G711U             = 0x0002,
    AUDIO_DEC_G726              = 0x0003 
}IVS_AUDIO_DEC_TYPE;

// 码流类型
typedef enum
{
    STREAM_TYPE_UNSPECIFIED=0,  // 不指定流
    STREAM_TYPE_MAIN=1, // 主码流
    STREAM_TYPE_SUB1,	// 辅码流1
    STREAM_TYPE_SUB2,	// 辅码流2
}IVS_STREAM_TYPE;

// 媒体参数
typedef struct
{
    IVS_VENDOR_TYPE         DecoderCompany; // 解码器厂商，1-HUAWEI，默认为1
    IVS_STREAM_TYPE         StreamType;     // 码流类型,1-主码流 2-辅码流1 3-辅码流2，默认为1
    IVS_PROTOCOL_TYPE       ProtocolType;   // 协议类型，1-UDP 2-TCP，默认为1
    IVS_MEDIA_TRANS_MODE    TransMode;      // 是否直连优先,0-否 1-是，默认为0
    IVS_BROADCAST_TYPE      BroadCastType;  // 是否支持组播，0-不支持 1-支持，默认为0 
    IVS_UINT32	            VideoDecType;   // 视频编码格式：参考 IVS_VIDEO_DEC_TYPE 1-H.264，默认为1
    IVS_AUDIO_DEC_TYPE      AudioDecType;   // 音频编码格式，1-PCMA 2-PCMU，默认为1
    IVS_CHAR	cReserve[64];
}IVS_MEDIA_PARA;


// 视频质量参数
typedef struct
{
    IVS_UINT32  VideoDecType;           // 视频编码格式：参考 IVS_VIDEO_DEC_TYPE

	IVS_UINT32  VideoCodeRate;          // 视频码率
    IVS_UINT32  VideoAverageCodeRate;   // 视频平均码率

	IVS_UINT32  VideoFPS;        // 视频帧率
	IVS_UINT32  VideoWidth;      // 视频宽
	IVS_UINT32  VideoHeight;     // 视频高
	IVS_UINT32  CurrPlayTime;    // 当前播放时间

	IVS_UINT32  Reserved1;       //保留1
	IVS_UINT32  Reserved2;       //保留2
}IVS_VEDIO_QUALITY_PARA;

// 音频质量参数
typedef struct
{	
	IVS_AUDIO_DEC_TYPE      AudioDecType;           // 音频编码方式
	IVS_UINT32              AudioCodeRate;          // 音频码率
    IVS_UINT32              AudioAverageCodeRate;   // 音频平均码率
    IVS_UINT32              AudioState;             // 音频状态    0-关闭  1-打开

	IVS_UINT32              Reserved1;              // 保留1
	IVS_UINT32              Reserved2;              // 保留2
}IVS_AUDIO_QUALITY_PARA;

typedef enum
{
	DOMAIN_TYPE_STACK=1,	// 堆叠
	DOMAIN_TYPE_CLUSTER=2,	// 集群
    DOMAIN_TYPE_SINGLE=3,	// 单机
}IVS_DOMAIN_TYPE;

// 域路由信息
#define IVS_MAX_DOMAIN_ROUTE_NUM    128     // 域路由最大记录数
typedef struct
{
	IVS_UINT32	uiDomainType;                       // SMU工作模式，参考 IVS_DOMAIN_TYPE 1-堆叠 2-集群 3-单机
	IVS_BOOL	bIsAgent;	                        // 是否代理
	IVS_CHAR	cDomainCode[IVS_DOMAIN_CODE_LEN];	// 域编码
	IVS_CHAR	cDomainName[IVS_NAME_LEN];		    // 域名称
	IVS_IP		stIP;                               // 域IP
	IVS_UINT32	uiPort;                             // 域端口
    IVS_UINT32	uiStatus;			                // 设备状态：0-离线，1-在线，2-休眠 参考 IVS_DEV_STATUS
	IVS_CHAR	cSuperDomain[IVS_DOMAIN_CODE_LEN];	// 上级域编码
    IVS_BOOL    bIsLocalDomain;                     // 是否本域 0-不是 1-是
    IVS_CHAR	cReserve[64];
}IVS_DOMAIN_ROUTE;

// 域路由信息列表
// 系统域路由最大个数 128
typedef struct
{
	IVS_UINT32   uiTotal;               // 系统实际域路由个数
	//IVS_INDEX_RANGE stIndexRange;     // 分页信息
	IVS_DOMAIN_ROUTE stDomainRoute[1];  // 域路由信息
}IVS_DOMAIN_ROUTE_LIST;

// 视频显示比例
typedef enum
{
    DISPLAY_SCALE_ORIGINAL=1,   // 原始比例
    DISPLAY_SCALE_FULL=2,       // 铺满窗口
}IVS_DISPLAY_SCALE;

// 视频播放效果
typedef enum
{
    PLAY_QUALITY_FLUENCY=1,    // 流畅优先
    PLAY_QUALITY_DEFINITION=2, // 清晰度优先
}IVS_PLAY_QUALITY_MODE;

// 密码过期信息
typedef struct
{
    IVS_BOOL    bIsNeedWarning;     // 是否需要提示用户：0-不需要；1-需要
    IVS_UINT32  uiRemainDays;       // 密码还剩多少天过期
    IVS_UINT32  uiExpiredDays;      // 密码已过期的天数（平台可设置过期不锁定账号）
    IVS_CHAR    cReserve[32];
}IVS_PWD_EXPIRE_INFO;

typedef enum
{
    PAY_LOAD_NO_TYPE     = -1,
    PAY_LOAD_TYPE_PCMU   = 0,   // G711的u率
    PAY_LOAD_TYPE_PCM    = 1,
    PAY_LOAD_TYPE_G723   = 4,   // G723
    PAY_LOAD_TYPE_PCMA   = 8,   // G711的a率
    PAY_LOAD_TYPE_G722   = 9,   // G722

    PAY_LOAD_TYPE_G726   = 16,  // G726
    PAY_LOAD_TYPE_AAC    = 17,  // AAC

    PAY_LOAD_TYPE_MJPEG  = 26,  // MJPEG
    PAY_LOAD_TYPE_H264_1 = 96,  // H264(非标准定义)
    PAY_LOAD_TYPE_AMR_MB = 97,  // AMR_NB
    PAY_LOAD_TYPE_MPEG4  = 98,  // MPEG4
    PAY_LOAD_TYPE_H264   = 99,  // H264
    PAY_LOAD_TYPE_AVS    = 100, // AVS
    PAY_LOAD_TYPE_MP3    = 101, // MP3
    PAY_LOAD_TYPE_3GPP   = 102,  // 3GPP
    PAY_LOAD_TYPE_TRACK  = 107,  // 轨迹
}IVS_PAYLOAD_TYPE;

/*****************************未接码原始数据结构****************************/
typedef struct
{
    IVS_UINT32	uiStreamType;		// 编码格式，参考：IVS_PAYLOAD_TYPE
    IVS_UINT32  uiFrameType;		// 帧数据类型，SPS、PPS、IDR、P（视频数据有效）
    IVS_UINT32  uiTimeStamp;		// 时间戳
    IVS_UINT64  ullTimeTick;		// 绝对时间戳
    IVS_UINT32  uiWaterMarkValue;	// 水印数据 ，0表示无水印数据（视频数据有效）

    IVS_UINT32	uiWidth;			// 视屏宽度
    IVS_UINT32	uiHeight;			// 视频高度

    IVS_UINT32	uiGopSequence;		// GOP序列
}IVS_RAW_FRAME_INFO;

/*****************************解码数据结构*********************************/
typedef enum
{
	PICTURE_FORMAT_YUV420 = 0,
	PICTURE_FORMAT_YUV422,
	PICTURE_FORMAT_YUV444,
	PICTURE_FORMAT_YUV422V,
	PICTURE_FORMAT_YUV400,
	PICTURE_FORMAT_YV12,
	PICTURE_FORMAT_NV12,
    PICTURE_FORMAT_YUV422H,
}IVS_PICTURE_FORMAT;

typedef struct
{
	IVS_UCHAR*  pY;
	IVS_UCHAR*  pU;
	IVS_UCHAR*  pV;
	IVS_UINT32  uiPitch;            // 平面的内存占用长度(也叫做跨度Stride)
	IVS_UINT32  uiWidth;            // 画面宽
	IVS_UINT32  uiHeight;           // 画面高
	IVS_UINT32  uiPictureFormat;    // 画面格式,参考IVS_PICTURE_FORMAT
	IVS_UINT32  uiTimeStamp;        // RTP时间戳
	IVS_UINT64  ullTimeTick;        // 时标信息，单位毫秒
    IVS_UINT32	uiGopSequence;		// GOP序列
    IVS_CHAR	cReserve[32];
    IVS_UINT32  uiFrameType;        //帧类型(I P)
}IVS_VIDEO_FRAME;

typedef struct
{
	IVS_UCHAR*  pFrame;         // 音频帧首地址
	IVS_UINT32  uiFrameSize;    // 音频帧长度
	IVS_UINT32  uiSampleRate;   // 采样率
	IVS_UINT32  uiChannels;     // 音频声道数
	IVS_UINT32  uiBits;         // 样位率
	IVS_UINT32  uiTimeStamp;    // RTP时间戳
	IVS_UINT64  ullTimeTick;    // 时标信息，单位毫秒
    IVS_UINT32  uiAudioFormat;  // 音频格式，直接使用payload Type定义的值，参考 IVS_PAYLOAD_TYPE
    IVS_CHAR	cReserve[32];
}IVS_AUDIO_FRAME;

// 媒体流类型;
typedef enum
{
    MEDIA_STREAM_VIDEO = 0,       // 视频;
    MEDIA_STREAM_AUDIO            // 音频;
}IVS_MEDIA_STREAM;

// 设备状态;
typedef struct
{
    IVS_CHAR	cCode[IVS_DEV_CODE_LEN];	// 设备编码
    IVS_UINT32	uiStatus;			        // IVS_DEV_STATUS
    IVS_IP		stIP;						// 设备IP信息
    IVS_UINT32	uiPort;						// 设备连接端口
    IVS_CHAR	cReserve[32];
}IVS_DEVICE_STATE;

// 语音对讲参数
typedef struct
{
    IVS_UINT32      uiProtocolType;     // 协议类型，1-UDP
    IVS_BOOL        bDirectFirst;       // 是否直连优先，0-否 1-是，默认为0
    IVS_CHAR	    cReserve[32];
}IVS_VOICE_TALKBACK_PARAM;

// 预置位信息
#define IVS_MAX_PRESET_NUM      256     // 预置位
#define IVS_PRESET_NAME_LEN     84      // 预置位名称
typedef struct
{
    IVS_UINT32  uiPresetIndex;   // 预置位索引
    IVS_CHAR    cPresetName[IVS_PRESET_NAME_LEN];   // 预置位名称，1~20个字符。
    IVS_CHAR	cReserve[32];
}IVS_PTZ_PRESET;

// 看守位信息
typedef struct
{
    IVS_UINT32  uiPresetIndex;          // 预置位索引
    IVS_BOOL    bEnableGuardPosition;   // 启用标识：0-不启用，1-启用
    IVS_UINT32  uiWaitTime;             // 自动归位等待时长，30秒~3600秒
    IVS_CHAR	cReserve[32];
}IVS_GUARD_POS_INFO;

// 巡航类型
typedef enum
{
    CRUISE_TRACK = 1,       // 轨迹巡航
    CRUISE_MODE,            // 模式巡航
    CRUISE_HORIZONTAL,      // 水平巡航
    CRUISE_VERTICAL,        // 垂直巡航
}IVS_CRUISE_TYPE;

// 云镜巡航轨迹点
typedef struct
{
    IVS_UINT32  uiPresetIndex;  // 预置位索引, 最多20个点
    IVS_UINT32  uiDwellTime;    // 预置位停留时间，秒为单位，1~3600秒 
    IVS_UINT32  uiSpeed;        // 云台速度，1~10 
    IVS_CHAR	cReserve[32];
}IVS_CRUISE_TRACK_POINT;

// 云镜巡航轨迹
#define IVS_MAX_CURISE_TRACK_NUM    16  // 单个摄像机最大巡航轨迹个数
#define IVS_CURISE_TRACK_NAME_LEN   84  // 轨迹名称
#define IVS_MAX_CURISE_TRACK_POINT  20  // 巡航轨迹预置位最大个数
typedef struct
{
    IVS_UINT32  uiCruiseNO;                                             // 轨迹索引
    IVS_UINT32  uiCruiseType;                                           // 巡航类型，值参考 IVS_CRUISE_TYPE
    IVS_CHAR    cTrackName[IVS_CURISE_TRACK_NAME_LEN];                  // 轨迹名称
    IVS_UINT32  uiTrackPointNum;                                        // 轨迹点个数
    IVS_CRUISE_TRACK_POINT  stTrackPoint[IVS_MAX_CURISE_TRACK_POINT];   // 轨迹点列表
    IVS_CHAR	cReserve[32];
}IVS_CRUISE_TRACK;

// 时间片段结构
typedef struct
{
    IVS_CHAR	cStart[IVS_TIME_LEN];   // 格式如yyyyMMddHHmmss
    IVS_CHAR	cEnd[IVS_TIME_LEN];     // 格式如yyyyMMddHHmmss
    IVS_UINT32  uiCruiseNO;             // 轨迹索引
    IVS_UINT32  uiCruiseType;           // 巡航类型，值参考 IVS_CRUISE_TYPE
    IVS_CHAR	cReserve[32];
}IVS_CRUISE_TIME_SPAN;

// 巡航计划信息
typedef struct
{
    IVS_UINT32      uiDayType;								//星期几：参考 IVS_DAY_TYPE 0 – 每天 1 – 周一 2 – 周二 3 – 周三 4 – 周四 5 – 周五 6 – 周六 7 – 周日
    IVS_UINT32      uiTimeSpanInfoNum;						//时间段数量
    IVS_CRUISE_TIME_SPAN   stTimeSpanInfo[IVS_MAX_TIMESPAN_NUM];   //时间段信息
}IVS_CRUISE_PLAN_INFO;

// 巡航计划
typedef struct
{
    IVS_BOOL	bEnable;        // 是否启用计划：0-停用，1-启用
    IVS_UINT32	uiPlanType;		// 计划方式：0-(WEEK)周计划,1(DAY)-日计划，周一到周日计划相同
    IVS_CHAR	cReserve[32];
    IVS_UINT32	uiPlanInfoNum;			                // 计划信息条数：周计划-7，日计划-1
    IVS_CRUISE_PLAN_INFO	stPlanInfo[IVS_MAX_PLAN_INFO_NUM];	// 计划信息：周计划-最多7个计划信息，日计划-1一个计划信息
}IVS_CRUISE_PLAN;

// 时间片段结构
typedef struct
{
    IVS_CHAR	cStart[IVS_TIME_LEN];   // 格式如yyyyMMddHHmmss
    IVS_CHAR	cEnd[IVS_TIME_LEN];     // 格式如yyyyMMddHHmmss
    IVS_UINT32  SnapInterval;           // 抓拍时间间隔
}IVS_SNAPSHOT_TIME_SPAN;

// 抓拍计划信息
typedef struct
{
    IVS_UINT32      uiDayType;								//星期几：参考 IVS_DAY_TYPE 0 – 每天 1 – 周一 2 – 周二 3 – 周三 4 – 周四 5 – 周五 6 – 周六 7 – 周日
    IVS_UINT32      uiTimeSpanInfoNum;						//时间段数量
    IVS_SNAPSHOT_TIME_SPAN   stTimeSpanInfo[IVS_MAX_TIMESPAN_NUM];   //时间段信息
}IVS_SNAPSHOT_PLAN_INFO;

// 抓拍计划
typedef struct
{
    IVS_BOOL	bEnable;        // 是否启用计划：0-停用，1-启用
    IVS_UINT32	uiPlanType;		// 计划方式：0-(WEEK)周计划,1(DAY)-日计划，周一到周日计划相同
    IVS_CHAR	cReserve[32];
    IVS_UINT32	uiPlanInfoNum;			                // 计划信息条数：周计划-7，日计划-1
    IVS_SNAPSHOT_PLAN_INFO	stPlanInfo[IVS_MAX_PLAN_INFO_NUM];	// 计划信息：周计划-最多7个计划信息，日计划-1一个计划信息
}IVS_SNAPSHOT_PLAN;
const IVS_UINT32 IVS_SNAPSHOT_TYPE_IA       = 0x00000001;   // 智能分析抓拍
const IVS_UINT32 IVS_SNAPSHOT_TYPE_ALARM    = 0x00000002;   // 告警抓拍
const IVS_UINT32 IVS_SNAPSHOT_TYPE_MANUAL   = 0x00000004;   // 手动抓拍(包括定时抓拍)

// 抓拍图片查询信息
typedef struct
{
    IVS_TIME_SPAN   stTimeSpan;     // 抓拍图片时间范围
    IVS_INDEX_RANGE stIndexRange;   // 抓拍图片索引信息
    IVS_UINT32      uiSnapType;     // 抓拍图片类型：异或标记值，例如0x01 | 0x02=3表示查询类型包括智能分析抓拍和告警抓拍；值参考IVS_SNAPSHOT_TYPE_IA
    IVS_CHAR	    cReserve[64];
}IVS_QUERY_SNAPSHOT_PARAM;

// 抓拍图片信息
typedef struct
{
    IVS_CHAR    cCameraCode[IVS_DEV_CODE_LEN];      // 摄像机编码
    IVS_CHAR	cSnapTime[IVS_TIME_LEN];            // 抓拍图片时间，格式如yyyyMMddHHmmss
    IVS_UINT32  uiSnapType;                         // 抓拍类型：参考IVS_SNAPSHOT_TYPE_IA
    IVS_UINT32	uiPictureID;                        // 抓拍图片ID
    IVS_CHAR    cPictureName[IVS_FILE_NAME_LEN];    // 文件名
    IVS_UINT32	uiPictureSize;                      // 图片文件大小
    IVS_CHAR    cPreviewUrl[IVS_URL_LEN];	        // 抓拍图片缩略图URL
    IVS_CHAR    cPictureUrl[IVS_URL_LEN];	        // 抓怕图片URL
    IVS_CHAR    cReserve[32];
}IVS_SNAPSHOT_INFO;

// 抓拍图片列表信息
typedef struct
{
    IVS_UINT32   uiTotal;                   // 总记录数
    IVS_INDEX_RANGE stIndexRange;           // 分页信息
    IVS_SNAPSHOT_INFO stSnapshotInfo[1];    // 抓拍图片信息
}IVS_SNAPSHOT_INFO_LIST;

// 实况参数
typedef struct
{
    IVS_UINT32      uiStreamType;       // 码流类型，值参考 IVS_STREAM_TYPE
    IVS_UINT32      uiProtocolType;     // 协议类型，1-UDP 2-TCP，默认为1
    IVS_BOOL        bDirectFirst;       // 是否直连优先，0-否 1-是，默认为0
    IVS_BOOL        bMultiCast;         // 是否组播，0-单播，1-组播，默认为0
    IVS_CHAR	    cReserve[32];
}IVS_REALPLAY_PARAM;

// 录像回放参数
typedef struct
{
    IVS_UINT32      uiProtocolType;     // 协议类型，1-UDP 2-TCP，默认为1
    IVS_TIME_SPAN   stTimeSpan;         // 实况启动、结束时间
    IVS_FLOAT       fSpeed;             // 回放速率
    IVS_CHAR	    cReserve[32];
}IVS_PLAYBACK_PARAM;

// 本地录像回放参数
typedef struct
{
    IVS_UINT32      uiStartTime;        // 回放起始时间，单位是秒
    IVS_UINT32      uiEndTime;          // 回放结束时间，单位是秒
    IVS_FLOAT       fSpeed;             // 回放速率
    IVS_CHAR        cPWD[IVS_PWD_LEN];  // 录像密码（为空表示无密码）
    IVS_CHAR	    cReserve[32];
}IVS_LOCAL_PLAYBACK_PARAM;

// 媒体地址
typedef struct
{
    IVS_IP      stIP;		                    // 媒体流目的IP
    IVS_UINT32  uiAudioPort;                    // 媒体流目的音频端口
    IVS_UINT32  uiVideoPort;                    // 媒体流目的视频端口
    IVS_CHAR	cReserve[32];
}IVS_MEDIA_ADDR;

// 电视墙参数
#define IVS_TVWALL_DECODER_CODE_LEN     24  // 电视墙解码器长度
typedef struct
{
    IVS_CHAR    cDecoderCode[IVS_TVWALL_DECODER_CODE_LEN];  // 电视墙解码器编码
    IVS_UINT32  uiChannel;                                  // 解码器通道号
    IVS_CHAR	cReserve[32];
}IVS_TVWALL_PARAM;

// 水印告警确认信息
#define IVS_WATERMARK_ALARM_DESCRIPTION_LEN 1024
typedef struct
{
    IVS_CHAR    cCameraCode[IVS_DEV_CODE_LEN];                      // 摄像机编码
    IVS_UINT64	ullAlarmEventID;                                    // 告警事件ID
    IVS_CHAR    cConfirmInfo[IVS_WATERMARK_ALARM_DESCRIPTION_LEN];	// 告警处理人员输入的信息
    IVS_CHAR    cConfirmTime[IVS_TIME_LEN];				            // 告警处理时间    
    IVS_CHAR	cReserve[64];
}IVS_WATERMARK_ALARM_CONFIRM;

// 水印告警取消信息
typedef struct
{
    IVS_CHAR    cCameraCode[IVS_DEV_CODE_LEN];                      // 摄像机编码
    IVS_UINT64	ullAlarmEventID;                                    // 告警事件ID
    IVS_CHAR    cCancelInfo[IVS_WATERMARK_ALARM_DESCRIPTION_LEN];	// 告警处理人员输入的信息
    IVS_CHAR    cCancelTime[IVS_TIME_LEN];				            // 告警撤销时间
    IVS_CHAR	cReserve[64];
}IVS_WATERMARK_ALARM_CANCEL;

// 水印告警信息
typedef struct
{
    IVS_UINT64	ullAlarmEventID;                                    // 告警事件ID
    IVS_CHAR    cAlarmType[IVS_ALARM_TYPE_LEN];		                // 告警类型：水印告警
    IVS_CHAR    cCheckoutOperator[IVS_NAME_LEN];	                // 水印校验人
    IVS_CHAR    cCameraCode[IVS_DEV_CODE_LEN];                      // 摄像机编码
    IVS_CHAR	cOccurStartTime[IVS_TIME_LEN];                      // 水印告警发生时间：YYYYMMDDHHMMSSmmm
    IVS_CHAR	cOccurStopTime[IVS_TIME_LEN];                       // 水印告警结束时间：YYYYMMDDHHMMSSmmm
    IVS_CHAR    cConfirmOperator[IVS_NAME_LEN];	                    // 告警确认人
    IVS_CHAR    cConfirmTime[IVS_TIME_LEN];	                        // 告警确认时间
    IVS_CHAR    cConfirmInfo[IVS_WATERMARK_ALARM_DESCRIPTION_LEN];	// 告警确认描述信息，键盘可见字符和中文，
    IVS_UINT32  uiWatermarkAlarmStatus;                             // 水印告警状态：0x01-未确认 0x02-已确认 0x04-已撤销；异或值表示多选，如0x07表示查询三种状态，0x06表示查询已确认和已撤销；
    IVS_CHAR	cReserve[64];
}IVS_WATERMARK_ALARM_INFO;

typedef struct
{
    IVS_UINT32   uiTotal;                               // 总记录数
    IVS_INDEX_RANGE stIndexRange;                       // 分页信息
    IVS_WATERMARK_ALARM_INFO stWatermarkAlarmInfo[1];   // 水印告警信息
}IVS_WATERMARK_ALARM_INFO_LIST;

// SFTP信息
#define IVS_FTP_ACCOUNT_LEN			64			// 名称长度
typedef struct
{
    IVS_IP      stIP;		                    // 服务器IP
    IVS_UINT32  uiPort;		                    // 服务器端口
    IVS_CHAR    cAccount[IVS_FTP_ACCOUNT_LEN];  // 服务器账号
    IVS_CHAR    cPWD[IVS_PWD_LEN];              // 服务器账号的密码
    IVS_CHAR	cReserve[32];
}IVS_FTP_INFO;

typedef struct
{
    IVS_FTP_INFO    stFTPInfo;                          // ftp信息
    IVS_CHAR        cMapFilePath[IVS_FILE_NAME_LEN];    // 电子地图路径信息
    IVS_CHAR        cReserve[32];                       //保留字段
}IVS_MAP_FTP_INFO;

/*广播失败异常事件结构体*/
typedef struct ST_BROADCAST_FAILED_INFO
{
    IVS_INT32 iSessionID;                             //登录会话ID
    IVS_CHAR cVoiceCode[IVS_DEV_CODE_LEN + 1];          //广播失败的语音设备编码
}BROADCAST_FAILED_INFO;

/*分辨率信息*/
typedef struct
{
	IVS_CHAR cResolution[IVS_FILE_NAME_LEN];   //分辨率信息
	IVS_CHAR cPFrameList[IVS_MAX_NAME_LIST_LEN]; //帧率列表，以逗号分隔
	IVS_CHAR cNFrameList[IVS_MAX_NAME_LIST_LEN]; //帧率列表，以逗号分隔
	IVS_CHAR cReserve[32]; //保留字段
}IVS_RESOLUTION_INFO;

/*I帧信息*/
typedef struct
{
	IVS_INT32 iPMinIFrameInterval; //P制最小I帧间隔
	IVS_INT32 iPMaxIFrameInterval; //P制最大I帧间隔
	IVS_INT32 iNMinIFrameInterval; //N制最小I帧间隔
	IVS_INT32 iNMaxIFrameInterval; //N制最大I帧间隔
	IVS_CHAR cReserve[32]; //保留字段
}IVS_IFRAME_INFO;

/*编码类型信息*/
typedef struct
{
	IVS_CHAR cEncodeMode[IVS_DEV_MODEL_LEN];    // 编码类型
	IVS_CHAR cProfileList[IVS_MAX_NAME_LIST_LEN];   // Profile列表,以逗号分隔
	IVS_CHAR cBitRateList[IVS_MAX_NAME_LIST_LEN];  //码率列表,以逗号分隔
	IVS_INT32 iRealNum;    // 列表个数，最大30条
	IVS_IFRAME_INFO stIFrameInfo;   //I帧信息
	IVS_RESOLUTION_INFO stResolution[IVS_MAX_RESOLUTION_LIST_NUM];        // 分辨率列表
	IVS_CHAR cReserve[32]; //保留字段
}IVS_ENCODEMODE_INFO;

/*码流类型结构体*/
typedef struct
{
	IVS_CHAR cStreamType[IVS_FILE_NAME_LEN];  //码流类型
	IVS_INT32 iRealNum;     // 列表个数，最大2条
	IVS_ENCODEMODE_INFO stEncodeMode[IVS_MAX_ENCODE_LIST_NUM]; //编码类型列表
	IVS_CHAR cReserve[32]; //保留字段
}IVS_ST_STREAM_INFO;

/*摄像机码流列表结构体*/
typedef struct
{
	IVS_INT32 iRealNum;                   // 列表个数，最大3条
	IVS_ST_STREAM_INFO stStreamInfo[IVS_MAX_STREAM_LIST_NUM];  // 码流信息列表
	IVS_CHAR cReserve[32]; //保留字段
}IVS_STREAM_INFO_LIST;

//客户端关闭视频业务通知信息
typedef struct{
	IVS_CHAR cCameraCode[IVS_DEV_CODE_LEN];					//摄像机编码
	IVS_UINT32 uiStreamType;														//业务类型：1-实况，2-回放
	IVS_UINT32 uiDuration;															//关闭时长，在此时间之内该用户不能再次点播，单位为秒，0为不控制关闭时长
}IVS_SHUTDOWN_NOTIFY_INFO;

#define IVS_MAX_STREAM_USER_INFO_NUM	512           //点播媒体流的用户信息最多512个
#define IVS_MAX_STREAM_CAM_INFO_NUM	512           //单个用户正在调用的摄像机最多512个
//点播媒体流的用户信息
typedef struct{
	IVS_UINT32 uiUserID;																//用户ID
	IVS_CHAR cUserName[IVS_NAME_LEN];								//用户名称
	IVS_CHAR cDomainCode[IVS_DOMAIN_CODE_LEN];		//用户所在域编码
	IVS_UINT32 uiStreamType;														//媒体流类型：1-实况，2-回放
}IVS_STREAM_USER_INFO;

//通过摄像机查询的媒体流列表
typedef struct{
	IVS_UINT32 uiTotal;														//媒体流用户信息记录总数
	IVS_CHAR cCameraCode[IVS_DEV_CODE_LEN];		//摄像机编码
	IVS_STREAM_USER_INFO stStreamUserInfo[1];		//点播媒体流的用户信息，最多IVS_MAX_STREAM_USER_INFO_NUM个
}IVS_STREAM_LIST_BY_CAM;

//用户点播的媒体流的摄像机信息
typedef struct{
	IVS_CHAR cCameraCode[IVS_DEV_CODE_LEN];					//摄像机编码
	IVS_CHAR cCameraName[IVS_NAME_LEN];						//摄像机名称
	IVS_UINT32 uiStreamType;														//媒体流类型：1-实况，2-回放
}IVS_STREAM_CAM_INFO;

//通过用户查询的媒体流列表
typedef struct{
	IVS_UINT32 uiTotal;														//媒体流摄像机信息记录总数
	IVS_UINT32 uiUserID;													//用户ID
	IVS_STREAM_CAM_INFO stStreamCamInfo[1];		//用户点播的媒体流的摄像机信息，最多IVS_MAX_STREAM_CAM_INFO_NUM个
}IVS_STREAM_LIST_BY_USER;

//NVR类型
typedef enum
{
	NVR_MODE_STACK = 1,   // 堆叠
	NVR_MODE_CLUSTER = 2, // 集群
	NVR_MODE_NONE    =3,
}IVS_NVR_MODE;

// 集群信息
#define IVS_MAX_CLUSTER_MEMBER_NUM 16  // 集群最大数目
typedef struct
{
	IVS_CHAR cClusterName[IVS_NAME_LEN];           // 集群名称
	IVS_CHAR cClusterCode[IVS_CLUSTER_CODE_LEN];   // 集群编码
	IVS_CHAR cDomainCode[IVS_DOMAIN_CODE_LEN];
	IVS_UINT32	uiStatus;			               // 设备状态：0-离线，1-在线，2-休眠 参考 IVS_DEV_STATUS
	IVS_CHAR cReserve[32];                         // 保留字段
}IVS_CLUSTER_INFO;

// 集群信息列表
#define IVS_MAX_CLUSTER_NUM 64
typedef struct
{
	IVS_UINT32 uiTotal;
	IVS_CLUSTER_INFO stClusterInfo[IVS_MAX_CLUSTER_NUM];    // 集群信息列表
}IVS_CLUSTER_INFO_LIST;

/******************************************************************
 function   : EventCallBack
 description: 事件回调函数(事件通知，异常回调等)
 input      : NA
 output     : iEventType    事件类型;
              pEventBuf     事件数据;
              uiBufSize     buf长度;
              pUserData     用户数据;
 return     : IVS_VOID
*******************************************************************/
typedef IVS_VOID (__SDK_CALL * EventCallBack)(IVS_INT32 iEventType, IVS_VOID* pEventBuf, IVS_UINT32 uiBufSize, IVS_VOID* pUserData);

/******************************************************************
 function   : DownloadCallBackRaw
 description: 拼帧祼码流回调函数(录像下载)
 input      : NA
 output     : ulHandle      播放句柄;
              pRawFrameInfo 裸码流帧类型参数;
              pBuf          帧数据;
              uiBufSize     buf长度;
              pUserData     用户数据;
 return     : IVS_VOID
*******************************************************************/
typedef IVS_VOID (__SDK_CALL * DownloadCallBackRaw)(IVS_ULONG ulHandle, IVS_RAW_FRAME_INFO* pRawFrameInfo, IVS_VOID* pBuf, IVS_UINT32 uiBufSize, IVS_VOID* pUserData);

/******************************************************************
 function   : RealPlayCallBackRaw
 description: 拼帧祼码流回调函数(实况)
 input      : NA
 output     : ulHandle      播放句柄;
              pRawFrameInfo 裸码流帧类型参数;
              pBuf          帧数据;
              uiBufSize     buf长度;
              pUserData     用户数据;
 return     : IVS_VOID
*******************************************************************/
typedef IVS_VOID (__SDK_CALL * RealPlayCallBackRaw)(IVS_ULONG ulHandle, IVS_RAW_FRAME_INFO* pRawFrameInfo, IVS_VOID* pBuf, IVS_UINT32 uiBufSize, IVS_VOID* pUserData);

/******************************************************************
 function   : PlayBackCallBackRaw
 description: 拼帧祼码流回调函数(录像回放)
 input      : NA
 output     : ulHandle      播放句柄;
              pRawFrameInfo 裸码流帧类型参数;
              pBuf          帧数据;
              uiBufSize     buf长度;
              pUserData     用户数据;
 return     : IVS_VOID
*******************************************************************/
typedef IVS_VOID (__SDK_CALL * PlayBackCallBackRaw)(IVS_ULONG ulHandle, IVS_RAW_FRAME_INFO* pRawFrameInfo, IVS_VOID* pBuf, IVS_UINT32 uiBufSize, IVS_VOID* pUserData);

/******************************************************************
 function   : RealPlayCallBackFrame
 description: 获取解码后码流回调函数(实况)
 input      : NA
 output     : ulHandle      播放句柄;
              uiStreamType  帧类型(音频或视频，参考IVS_MEDIA_STREAM);
              pFrameInfo    帧类型参数(视频类型参考IVS_VIDEO_FRAME，音频类型参考IVS_AUDIO_FRAME);
              pUserData     用户数据;
 return     : IVS_VOID
*******************************************************************/
typedef IVS_VOID (__SDK_CALL * RealPlayCallBackFrame)(IVS_ULONG ulHandle, IVS_UINT32 uiStreamType, IVS_VOID* pFrameInfo, IVS_VOID* pUserData);

/******************************************************************
 function   : PlayBackCallBackFrame
 description: 获取解码后码流回调函数(录像回放)
 input      : NA
 output     : ulHandle      播放句柄;
              uiStreamType  帧类型(音频或视频，参考IVS_MEDIA_STREAM);
              pFrameInfo    帧类型参数(视频类型参考IVS_VIDEO_FRAME，音频类型参考IVS_AUDIO_FRAME);
              pUserData     用户数据;
 return     : IVS_VOID
*******************************************************************/
typedef IVS_VOID (__SDK_CALL * PlayBackCallBackFrame)(IVS_ULONG ulHandle, IVS_UINT32 uiStreamType, IVS_VOID* pFrameInfo, IVS_VOID* pUserData);

// 画图回调
typedef IVS_VOID (__SDK_CALL * DRAW_PROC)(IVS_VOID *hDc, IVS_VOID *rc, IVS_UINT64 ullFrameID, IVS_VOID *pUser);

#pragma pack(pop)

#endif //_HW_IVS_SDK_H_