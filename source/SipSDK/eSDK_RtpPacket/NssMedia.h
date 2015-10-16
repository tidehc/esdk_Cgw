#ifndef __NSS_MEDIA_H__
#define __NSS_MEDIA_H__

//#include "nssH/nss_mt_old.h"
//#include "nssH/nss_adt.h"
//#include "nssH/rtvideo/stream_m.h"

#define SAFETY_INFO_LEN 16       // 防盗链信息长度

typedef unsigned long long uint64_nss;

#define MAX_PACKET_NUM_PER_FRAME        1024
#define WATER_MARK_LEN                  4
#define NSS_HEAD_EXTEND_LEN             4
#define FRAME_CACHE_NUM                 5
#define FRAME_ALLOC_NUM                 (FRAME_CACHE_NUM + 10)   // 加上SPS、PPS、IDR

#pragma pack(push, 1)
/*
 * 帧类别 FrameType
 * 1:视频帧；2:音频帧；3:NAT请求消息；4:NAT响应消息；5,6:回放控制；7,8:EOS/BOS消息;其它:错误。
 */
enum FRAME_TYPE
{
    FRAME_TYPE_VIDEO                    = 0x01, // 视频帧
    FRAME_TYPE_AUDIO                    = 0x02, // 音频帧
    FRAME_TYPE_NAT_REQ                  = 0x03, // NAT请求消息
    FRAME_TYPE_NAT_RESP                 = 0x04, // NAT响应消息
    FRAME_TYPE_PLAYBACK_CONTROL_REQ     = 0x05, // 回放控制请求消息
    FRAME_TYPE_PLAYBACK_CONTROL_RESP    = 0x06, // 回放控制应答消息
    FRAME_TYPE_EOS                      = 0x07, // EOS消息
    FRAME_TYPE_BOS                      = 0x08, // BOS消息
    FRAME_TYPE_CLOSE                    = 0x09  // 异常结束消息
};

/*
 * 是否是关键帧标志符 
 * 0：非关键帧；1：关键帧；2：关键帧含有解码参数集；其它：错误。
 */
enum KEY_FRAME_FLAG
{
    KEY_FRAME_FLAG_COMMON               = 0x00, // P帧
    KEY_FRAME_FLAG_KEY                  = 0x01, // 关键帧IDR
    KEY_FRAME_FLAG_SPS                  = 0x02, // 关键帧SPS
    KEY_FRAME_FLAG_PPS                  = 0x03, // 关键帧 PPS
    KEY_FRAME_FLAG_SEI_IFRAME           = 0x04, // SEI(I帧)
    KEY_FRAME_FLAG_SEI_PFRAME           = 0x05, // SEI(P帧)
    KEY_FRAME_FLAG_WATER_MARK           = 0x06  // 水印帧

};

#define WATER_MARK_VALUE 0xF0
#define BIT_LOW_VALUE    0x0F
#define WATER_MARK_LABEL 0x0001

#define H264_START_CODE_LENGTH 0x04
#define H264_START_CODE 0x00000001
#define H264_START_CODE_SHORT_LENGTH 0x03
#define H264_START_CODE_SHORT 0x00000100
#define H264_START_CODE_VALUE 0xFFFFFF00

/*
 * 视频编码格式 VIDEO_CODEC_TYPE
 */
enum VIDEO_CODEC_TYPE
{
    VIDEO_CODEC_TYPE_MIN = 0,
    CODEC_TYPE_H264      = 0x01,
    CODEC_TYPE_MJPEG     = 0x02,
    CODEC_TYPE_MPEG4     = 0x03,
    CODEC_TYPE_MPEG2     = 0x04,
    CODEC_TYPE_AVS       = 0x05,
    VIDEO_CODEC_TYPE_MAX
};

/*
 * 音频编码格式 AUDIO_CODEC_TYPE
 */
enum AUDIO_CODEC_TYPE
{
    CODEC_TYPE_NULL      = 0x00, // PU不支持音频
    CODEC_TYPE_G711A     = 0x01, // G.711A
    CODEC_TYPE_G711U     = 0x02, // G.711U
    AUDIO_CODEC_TYPE_MAX
};

// 扩展帧头
typedef struct _NSS_EXTEND_FRAME_HEADER
{
    unsigned long    FrameSize;      // 帧长度,包含扩展数据帧头、帧头、媒体数据
    unsigned long    AdjustFrameSize;// 校正帧长度, 包含扩展数据帧头、帧头、媒体数据、4K对齐补充数据(如果有的话) 
    unsigned char    ReserveData[4]; // 保留字节
}NSS_EXTEND_FRAME_HEADER;

// 关键帧时需要在帧头后附加关键帧信息
typedef struct _H264_KEY_FRAME_HEADER
{
    unsigned long    SPSOffset;
    unsigned long    PPSOffset;
    unsigned long    SEIOffset;
    unsigned long    IDROffset;
}H264_KEY_FRAME_HEADER;

// 自研摄像头侧定义的多层编码类型, 和我们存储分层信息有区别的
// 如果是关键帧MULTI_LAYER_CODE_0存储在0层,否则存储在1层,MULTI_LAYER_CODE_1和MULTI_LAYER_CODE_2分别存储在第2和3层
typedef enum  MULTI_LAYER_CODE_TYPE
{
    MULTI_LAYER_CODE_0      = 0,   //多层编码类型0，基本层
    MULTI_LAYER_CODE_1      = 1,   //多层编码类型1，扩展层
    MULTI_LAYER_CODE_2      = 2,   //多层编码类型2，增强层
    MULTI_LAYER_CODE_NULL   = 3,   //不使用多层编码，即不分层
    MULTI_LAYER_CODE_MAX
}MULTI_LAYER_CODE_TYPE_E;

typedef struct NSS_EXTEND_WATER
{
    unsigned long ulSize;  //结构体长度
    unsigned short usWaterMarkType;       //数字水印类型 //>>WATER_MARK_TYPE_E
    unsigned long  ulWaterMarkValue;      //数字水印值
}NSS_EXTEND_WATER_S;


// 带扩展帧头的帧头，MRU用存录像文件用
/*typedef struct _FILE_MEDIA_FRAME_HEADER
{
    NSS_EXTEND_FRAME_HEADER ExtendFrameHeader;  // 扩展帧头
    NSS_MEDIA_FRAME_HEADER  FrameHeader;        // 帧头
}FILE_MEDIA_FRAME_HEADER;
*/

// NAT穿越请求消息
/*typedef struct _NSS_MEDIA_NAT_REQ
{
    NSS_MEDIA_FRAME_HEADER FrameHeader;         // 帧头，只用 FrameType = FRAME_TYPE_NAT_REQ
    char            StreamID[ID_LEN];           // 流ID
    unsigned long   MsgReqSeq;                  // 请求消息序列号
    char            SafetyInfo[SAFETY_INFO_LEN];// 认证信息。16字节
    unsigned long   LocalIP;                    // 媒体请求方NAT穿越前的本地IP地址
    unsigned short  LocalPort;                  // 媒体请求方NAT穿越前的本地端口
}NSS_MEDIA_NAT_REQ;
*/
// NAT穿越响应消息
/*
typedef struct _NSS_MEDIA_NAT_RESP
{
    NSS_MEDIA_FRAME_HEADER FrameHeader;        // 帧头，只用 FrameType = FRAME_TYPE_NAT_RESP
    uint64_nss      StreamID;                  // 流ID
    unsigned long   MsgReqSeq;                 // 请求消息序列号
    unsigned long   LocalIP;                   // 媒体请求方NAT穿越前的本地IP地址
    unsigned short  LocalPort;                 // 媒体请求方NAT穿越前的本地端口
    unsigned long   SrcIP;                     // 媒体请求方NAT穿越后的IP地址
    unsigned short  SrcPort;                   // 媒体请求方NAT穿越后的端口
}NSS_MEDIA_NAT_RESP;
*/

typedef enum
{
    H264_NALU_TYPE_UNDEFINED    =0,
    H264_NALU_TYPE_SILCE        =1,
    H264_NALU_TYPE_IDR          =5,
    H264_NALU_TYPE_SEI          =6,
    H264_NALU_TYPE_SPS          =7,
    H264_NALU_TYPE_PPS          =8,    
    H264_NALU_TYPE_AUD          =9,
    H264_NALU_TYPE_STAP_A        =24,
    H264_NALU_TYPE_STAP_B        =25,
    H264_NALU_TYPE_MTAP16        =26,
    H264_NALU_TYPE_MTAP24        =27,
    H264_NALU_TYPE_FU_A         =28,
    H264_NALU_TYPE_FU_B         =29,
    H264_NALU_TYPE_END
}H264_NALU_TYPE;//lint !e751

typedef struct
{
    //byte 0
    unsigned char TYPE:5;
    unsigned char NRI:2;
    unsigned char F:1;
} NALU_HEADER; /**//* 1 BYTES */

typedef struct 
{
    //byte 0
    unsigned char TYPE:5;
    unsigned char NRI:2; 
    unsigned char F:1;    
} FU_INDICATOR; /**//* 1 BYTES */

typedef struct
{
    //byte 0
    unsigned char TYPE:5;
    unsigned char R:1;
    unsigned char E:1;
    unsigned char S:1;    
} FU_HEADER; /**//* 1 BYTES */

#pragma pack(pop)

// 拼帧处理器里面的回调函数
typedef   int (*fAfterCombinFrameCallBack) (void* pUser, 
                                            ACE_Message_Block **pMbArray, 
                                            unsigned int MsgCount, unsigned short usLayerNo);
#endif /*__NSS_MEDIA_H__*/


