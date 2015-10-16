/*
 * RtpPacket.h
 *
 *  Created on: 2010-12-28
 *      Author: x00106873
 */

#ifndef RTPPACKET_H_
#define RTPPACKET_H_

#include <map>

#define RTP_PACKET_VERSION      2
#define RTP_CSRC_LEN            4
#define RTP_EXTEND_PROFILE_LEN  4

// PayloadType定义
#define IVS_PT_MJPEG         26
#define IVS_PT_MPEG4         98
#define IVS_PT_H264          99
#define IVS_PT_G711A          8
#define IVS_PT_G711U          0
#define IVS_PT_G723           4
#define IVS_PT_G722           9
#define IVS_PT_G726		     16
#define	IVS_PT_AAC 		     17
#define ELTE_PT_AMR          114

//NSS协议定义音频编码值
#define NSS_AUDIO_G711A 0x0001
#define NSS_AUDIO_G711U 0x0002
#define NSS_AUDIO_G726  0x0003
#define NSS_AUDIO_AAC   0x0004

typedef struct
{
    /**//* byte 0 */
    unsigned char csrc_len:4;       /**//* expect 0 */
    unsigned char extension:1;      /**//* expect 1, see RTP_OP below */
    unsigned char padding:1;        /**//* expect 0 */
    unsigned char version:2;        /**//* expect 2 */
    /**//* byte 1 */
    unsigned char payload:7;        /**//* RTP_PAYLOAD_RTSP */
    unsigned char marker:1;         /**//* expect 1 */
    /**//* bytes 2, 3 */
    unsigned short seq_no;
    /**//* bytes 4-7 */
    unsigned int timestamp;
    /**//* bytes 8-11 */
    unsigned int ssrc;             /**//* stream number is used here. */
} RTP_FIXED_HEADER;

typedef struct
{
    unsigned short  usProfile;
    unsigned short  usLength;
}RTP_EXTEND_HEADER;

//数字水印类型
typedef enum  _enWATER_MARK_TYPE
{
    WATER_MARK   = 0,   //数字水印
    WATER_MARK_NULL = 1,   //不使用数字水印
    WATER_MARK_MAX
}WATER_MARK_TYPE_E;


//rtp前端扩展头.旧摄像头仍使用此扩展头结构，故保留
typedef struct _enRTP_EXTENSION_DATA
{
    unsigned short  usMultiLayerCodeType;  //多层编码类型 //>>MULTI_LAYER_CODE_TYPE_E
    unsigned short  usWaterMarkType;       //数字水印类型 //>>WATER_MARK_TYPE_E
    unsigned int    ulWaterMarkValue;      //数字水印值
}RTP_EXTENSION_DATA_S;

//扩展头增加一个字段"补齐16字节对齐的额外数据量".swx164794 add/modify. 2013-03-06. begin
//rtp前端扩展头
typedef struct _stRTP_EXTENSION_DATA_AES_ALL_S
{
	unsigned short usMultiLayerCodeType;    //多层编码类型 //>>MULTI_LAYER_CODE_TYPE_E
	unsigned short usWaterMarkType;         //数字水印类型 //>>WATER_MARK_TYPE_E
	unsigned int   ulWaterMarkValue;        //数字水印值
	unsigned int   ulEncryptExLen;          //补齐16字节对齐的额外数据量
}RTP_EXTENSION_DATA_S_EX;

//rtp平台扩展头,比前端多三个字段. MU组的rtp包都含有补齐信息字段
typedef struct _stRTP_EXTENSION_DATA_MU
{
    unsigned short  usMultiLayerCodeType;   //多层编码类型 //>>MULTI_LAYER_CODE_TYPE_E
    unsigned short  usWaterMarkType;        //数字水印类型 //>>WATER_MARK_TYPE_E
    unsigned int    ulWaterMarkValue;       //数字水印值
    unsigned int    ulEncryptExLen;         //补齐16字节对齐的额外数据量
    unsigned short  usLostFrame;            //是否丢包，0没有丢包，1丢包，无水印时无效
    unsigned short  usGmtMillsecond;        // 绝对时间毫秒部分
    unsigned int    uiGmtSecond;            // 绝对时间秒部分
}RTP_EXTENSION_DATA_MU_S;
//扩展头增加一个字段"补齐16字节对齐的额外数据量".swx164794 add/modify. 2013-03-06. end

//水印帧帧体
typedef struct
{   
	unsigned short  usLostFrame;           //是否丢包，0没有丢包，1丢包，无水印时无效
	unsigned short  usWaterMarkType;       //数字水印类型 //>>WATER_MARK_TYPE_E
	unsigned int    ulWaterMarkValue;      //数字水印值

}WarterMark_MEDIA_BODY;

class CRtpPacket
{
public:
    CRtpPacket();
    virtual ~CRtpPacket();

    int ParsePacket
    (
        char* pRtpData, 
        unsigned int ulLen
    );

    int GeneratePacket
    (
        char*           pRtpPacket, 
        unsigned int    ulLen
    );

    int GenerateExtensionPacket
    (
        char*           pRtpPacket, 
        unsigned int    ulLen
    );

    // 获取属性值，分析包时用
    unsigned short  GetSeqNum() const;
    unsigned int   GetTimeStamp() const;
    char            GetPayloadType() const;
    bool            GetMarker() const;
    char            GetExtension() const;       // 是否含扩展部分    

    unsigned int    GetFixedHeadLen() const;
    unsigned int    GetHeadLen() const;
    unsigned short  GetPacketType() const;
    unsigned int    GetPacketLen() const;
	
	RTP_EXTENSION_DATA_S* GetExtData() const;
	RTP_EXTENSION_DATA_S_EX* GetExtHaveFillInData() const;
	RTP_EXTENSION_DATA_MU_S* GetMuExtData() const;
	char* GetRtpData() const;
	RTP_FIXED_HEADER* GetFixedHead() const;
	
    // 设置属性值，打包时用
    int SetSeqNum(unsigned short usSeqNum);
    int SetTimeStamp(unsigned int ulTimeStamp);
    int SetPayloadType(unsigned char ucPayloadType);
    int SetMarker(bool bMarker);
    int SetExtension(bool bExtension);
    void SetSSRC(unsigned int unSsrc);

    // 获取"补齐16字节对齐的额外数据量"信息.
    unsigned int GetEncryptExLen() const;   

private:
    int GetVersion(char& cVersion)const;
    int CheckVersion()const;
    int SetVersion(unsigned char ucVersion);
    int ParsePacketType();
private:
    char*                   m_pRtpData;
    RTP_FIXED_HEADER*       m_pFixedHead;
    RTP_EXTEND_HEADER*      m_pExtHead;

    //扩展头增加一个字段"补齐16字节对齐的额外数据量".swx164794 add. 2013-03-09
    RTP_EXTENSION_DATA_S_EX*       m_pExtDataHaveFillIn;   //带补齐信息的扩展内容.
    
    RTP_EXTENSION_DATA_S*   m_pExtData;//扩展内容    
    RTP_EXTENSION_DATA_MU_S*   m_pMuExtData;//mu自定义扩展内容

    unsigned int            m_ulPacketLen;
    unsigned int            m_ulHeadLen;    // 包含所有的头
    unsigned int            m_ulFixedHeadLen;  //固定头长度
};

#endif /* RTPPACKET_H_ */
