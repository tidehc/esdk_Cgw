/******************************************************************************
   版权所有 (C), 2001-2011, 华为技术有限公司

 ******************************************************************************
  文 件 名 : rtsp_client_datatype.h
  作    者 : y00182336
  功能描述 : 组装和解析SDP
  修改历史 :
    1 日期 : 2011-09-26
      作者 : y00182336
      修改 : 创建

    2 日期 :
      作者 :
      修改 :
 ******************************************************************************/
#ifndef __RTSP_CLIENT_DATATYPE_H_
#define __RTSP_CLIENT_DATATYPE_H_
#include <time.h>

namespace RTSP_SDK{

/// 定义模块名
#define RTSP_CLIENT_SERVICE_NAME  "rtsp_client"
/// 日志目录
#define RTSP_CLIENT_LOG_PATH      "../log"

/// 定义日志路径长度
#define RTSP_CLIENT_PATH_LEN      (100)

// 交织通道号
#define     RTSP_INTER_LEAVE_NUM    (0)

#define     RTSP_AUTH_LEN          (32)

#define     RTSP_REALM_NONCE_LEN   (64)

#define     RTSP_DEFAULT_TIMEOUT    (3)

// 鉴权认证
typedef struct _stAUTHENTICA
{
    char        szUserName[RTSP_AUTH_LEN];
    char        szPassword[RTSP_AUTH_LEN];
    char        szRealm[RTSP_REALM_NONCE_LEN];
    char        szNonce[RTSP_REALM_NONCE_LEN];
}AUTHENTICA;

// 地址与端口
typedef struct _stINET_ADDR
{
    unsigned int        unAddrIp;
    unsigned short      unAddrPort;
}INET_ADDR;

// 定义媒体地址
typedef struct _stMEDIA_ADDR
{
    unsigned int        unMediaIp;          // 媒体IP
    unsigned short      unMediaVideoPort;   // 媒体视频RTP端口，RTCP端口+1 若不使用，需置为0
    unsigned short      unMediaAudioPort;   // 媒体音频RTP端口，RTCP端口+1 若不使用，需置为0
}MEDIA_ADDR;

// 定义特定的几个时间偏移量
#define OFFSET_BEGIN  ((unsigned int)-3)  // 表示媒体最开始的位置
#define OFFSET_CUR    ((unsigned int)-2)  // 表示媒体当前位置
#define OFFSET_END    ((unsigned int)-1)  // 表示媒体结束位置

// 媒体播放范围的时间表示类型
typedef enum
{
    RANGE_TYPE_NPT = 0, // NormalPlayTime(相对时间)
    RANGE_TYPE_UTC,     // UTC AbsoluteTime(UTC绝对时间)
    RANGE_TYPE_SMPTE    // SMPTE时间，目前暂不支持
}MEDIA_RANGE_TYPE_E;

// 媒体播放范围的时间表示类型
typedef enum
{
    BUSYNESS_TYPE_LIVEVIDEO = 0, // 实况业务
    BUSYNESS_TYPE_RECORD_REPLAY     // 回放业务
}MEDIA_BUSYNESS_TYPE;

// 表示媒体播放范围的结构体
typedef struct
{
    unsigned int  MediaBeginOffset;   // 开始播放位置(单位:秒)
    unsigned int  MediaEndOffset;     // 结束播放位置(单位:秒)
    MEDIA_RANGE_TYPE_E enRangeType;   // Range的时间类型
    MEDIA_BUSYNESS_TYPE enBusyType; // 业务类型
}MEDIA_RANGE_S;

// 日志级别
typedef enum
{
    RTSP_LOG_LEVEL_EMERGENCY = 0,   // Emergency 级别的运行日志；
    RTSP_LOG_LEVEL_ALART,           // Alert 级别的运行日志；
    RTSP_LOG_LEVEL_CRITICAL,        // Critical 级别的运行日志；
    RTSP_LOG_LEVEL_ERROR,           // Error 级别的运行日志；
    RTSP_LOG_LEVEL_WARNING,         // Warning 级别的运行日志；
    RTSP_LOG_LEVEL_NOTICE,          // Notice 级别的运行日志；
    RTSP_LOG_LEVEL_INFO,            // Info 级别的运行日志。
    RTSP_LOG_LEVEL_DEBUG            // Debug 级别的运行日志。
}RTSP_LOG_LEVEL_E;

// 定义Session状态
typedef enum _enRTSP_SESSION_STATUS
{
    RTSP_SESSION_STATUS_INIT,
    RTSP_SESSION_STATUS_DESCRIBE,
    RTSP_SESSION_STATUS_READY,
    RTSP_SESSION_STATUS_PLAYING,
    RTSP_SESSION_STATUS_STOPPING,
    RTSP_SESSION_STATUS_STOPPED,
    RTSP_SESSION_STATUS_ABNORMAL,        // 异常状态
    RTSP_SESSION_STATUS_DELETE
}RTSP_SESSION_STATUS;

// 定义Rtsp消息类型
typedef enum _enRTSP_METHOD_TYPE
{
    RTSP_METHOD_SETUP = 0,     // SETUP请求
    RTSP_METHOD_PLAY,          // PLAY请求
    RTSP_METHOD_PAUSE,         // PAUSE请求
    RTSP_METHOD_TEARDOWN,      // TEARDOWN请求
    RTSP_METHOD_ANNOUNCE,      // ANNOUNCE请求
    RTSP_METHOD_DESCRIBE,      // DESCRIBE请求
    RTSP_METHOD_SETPARAMETER,  // SETPARAMETER请求
    RTSP_METHOD_OPTIONS,       // OPTION请求
    RTSP_METHOD_NUM
}RTSP_METHOD_TYPE;

// RTSP Client返回码
typedef enum _enRTSP_CLIENT_RET_CODE
{
    RTSP_ERR_NOT_EXIST          = 70001,    /// 连接句柄不存在
    RTSP_ERR_DISCONNECT         = 70002,    /// 连接失败
    RTSP_ERR_RESPONSE           = 70003,    /// 响应消息错误
    RTSP_ERR_TIMEOUT            = 70004,    /// 响应超时
    RTSP_ERR_STAUTS_ABNORMAL    = 70005,    /// 连接状态异常
    RTSP_ERR_SEND_FAIL          = 70006,    /// 发送出错
    RTSP_ERR_RECV_FAIL          = 70007,    /// 接收出错
    RTSP_ERR_PARAMETER          = 70008,    /// 参数检查出错
    RTSP_ERR_NORMAL_STOP        = 70009,    /// 正常结束，ANNOUNCE正常结束请求
    RTSP_ERR_ABNORMAL_STOP      = 70010,    /// 异常结束，ANNOUNCE异常结束请求

    RTSP_ERR_NOTENOUGH_BANDWIDTH= 70011,    /// 没有足够的带宽，对应453错误码
    RTSP_ERR_UNAUTHORIZED       = 70012,    /// 未鉴权

    RTSP_ERR_RET_SYS            = 70030,    /// 通用系统错误
    RTSP_ERR_ANNOUNCE_STOP      = 70031     /// 收到回放的announce,表示流停止，然后产生异常，通知MU
}RTSP_CLIENT_RET_CODE;

// 业务类型
typedef enum _enRTSP_CLIENT_BUSINESS_TYPE
{
    RTSP_CLIENT_BUSINESS_TYPE_LIVE       = 1,  /// 实况
    RTSP_CLIENT_BUSINESS_TYPE_TALK       = 2,  /// 语音对讲
    RTSP_CLIENT_BUSINESS_TYPE_BROADCAST  = 3   /// 语音广播
}RTSP_CLIENT_BUSINESS_TYPE;

// Windows需要__stdcall，Liunx不需要
#ifdef WIN32
#define RTSPAPI __stdcall
#else
#define RTSPAPI
#endif

// 异常回调函数 //__stdcall
typedef  void (RTSPAPI *fExceptionCallBack)(long handle,
                                    unsigned int ulMsgType,
                                    unsigned int ulErrCode,           // 回调函数错误码，详见_enRtspClientRetCode
                                    void *pParameter,
                                    void *pUser);

// 媒体流回调//__stdcall
typedef  void (RTSPAPI *fDataCallBack)(long handle,
                               char *pBuffer,
                               unsigned int ulBufSize,
                               void *pUser, //设置到client
                               void* pMediaInfo);//回调给用户
}

#endif //__RTSP_CLIENT_DATATYPE_H_
