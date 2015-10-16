/********************************************************************
 filename    :    RtpFrameOrganizer.h
 author      :    shilei 00194833
 created     :    2013-02-21
 description :    实现一个PS流转换成ES流的处理器
 copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
 history     :    2013-02-21 初始版本
*********************************************************************/
#ifndef __RTPFRAMEORGANIZER_H__
#define __RTPFRAMEORGANIZER_H__

#include <deque>
#include <list>
using namespace std;

// ACE系统通用头
#include "ace_header.h"
#include "eSDK_RtpPacket.h"

#define MAX_RTP_FRAME_CACHE_NUM     (3)                // 处理缓存的最大帧数
#define MAX_RTP_SEQ                 65535              // RTPseq最大值
#define RTP_FRAME_CACHE_SIZE        (1500 * 1024)      // 一帧数据的最大长度1.5M
#define MAX_PACKETS_PER_SEND        10
#define MIN_TIME_INTERVAL           5
#define MAX_LEFT_PACKET_COUNT       200
#define SELECT_TIME_INTERVAL        (8 * 1000)
#define MAX_AUDIO_LENGTH            (10 * 1024)
#define MAX_RTP_PACKET_COUNT        (1024)
#define MAX_RTP_QUEUE_NUM           (MAX_RTP_PACKET_COUNT * MAX_RTP_FRAME_CACHE_NUM)

typedef std::list<ACE_Message_Block*>       RTP_FRAME_LIST_T;
typedef RTP_FRAME_LIST_T::iterator            RTP_FRAME_LIST_T_ITER;

typedef struct _stRTP_PACK_INFO_S
{
    unsigned short      usSeq;
    unsigned int        unTimestamp;
    bool                bMarker;
    ACE_Message_Block*  pRtpMsgBlock;
}RTP_PACK_INFO_S;
typedef std::deque<RTP_PACK_INFO_S>         RTP_PACK_QUEUE;

// 媒体流回调
typedef  void (*fafterPacketProtockChangeDataCallBack)(void  *pUser,ACE_Message_Block *pRtpBlock,ST_FRAME_DATA* pFreameData);

// RTP包处理器
class CPacketChangeProcessor
{
private:
    CPacketChangeProcessor(const CPacketChangeProcessor&);
    CPacketChangeProcessor& operator=(const CPacketChangeProcessor&);
public:
    CPacketChangeProcessor():m_ucAudioEncodeMode(0),
		 m_bAppendExtInfo(false),
		 m_fDataCallBack(NULL),
		 m_pUser(NULL),
		 m_Channel(0),
		 m_CallBack(NULL)
    {}
    virtual ~CPacketChangeProcessor()
	{
		m_ucAudioEncodeMode = 0;
		m_bAppendExtInfo = false;
		m_fDataCallBack = NULL;
		m_pUser = NULL;
		m_Channel = 0;
		m_CallBack = NULL;
	}

    // 发送接口
    virtual int init(std::string strServiceID) = 0;
    virtual void receviePacket(const ACE_Message_Block* pMb)=0;
	virtual int  setIsNeedAudioFrame(bool isNeedAudioFrame)=0;
    void setDataCallBack(fafterPacketProtockChangeDataCallBack fDataCallBack, void *pUser)
    {
        m_fDataCallBack =  fDataCallBack;
        m_pUser = pUser;
    }

    void setAudioEncodeMode(unsigned char ucAudioEncodeMode);
    void setAppednExtInfo(bool bAppendExtInfo){ m_bAppendExtInfo = bAppendExtInfo; };
	void SetChannel(unsigned int channelID) {m_Channel = channelID;}
	unsigned int GetChannel(void)const  {return m_Channel;}
	void SetFrameDataCallBack(FrameDataCallBack pCallBack) {m_CallBack = pCallBack;}
	const FrameDataCallBack GetFrameDataCallBack(void)const  {return m_CallBack;}
protected:
    unsigned char           m_ucAudioEncodeMode;           // 音频编码格式
    bool                    m_bAppendExtInfo;              // 回调数据之前，添加附加信息

    fafterPacketProtockChangeDataCallBack m_fDataCallBack; //一帧数据处理完回调发送
    void *m_pUser; //数据处理对象
private:
	unsigned int m_Channel;
	FrameDataCallBack m_CallBack; 

};

// 发送一帧数据接口
class IRtpFrameHandler
{
public:
    IRtpFrameHandler(){}

    virtual ~IRtpFrameHandler(){}

    /*****************************************************************************
     Function:    handleMediaFrame
     Description: 拼完一帧数据后处理媒体数据
     Input:       rtpFrameList ES数据包列表
     Output:      
     Return:      N/A
    *****************************************************************************/
    virtual void handleMediaFrame(RTP_FRAME_LIST_T &rtpFrameList) = 0;

};

// 组帧类
class CRtpFrameOrganizer
{
public:
    CRtpFrameOrganizer();
    virtual ~CRtpFrameOrganizer();

    int init(IRtpFrameHandler* pHandler, const std::string& strServiceID, unsigned int unMaxFrameCache = MAX_RTP_FRAME_CACHE_NUM);

    int insertRtpPacket(const ACE_Message_Block* pRtpBlock);

    bool checkLostPacket()const;

    void release();

    int startThread();

    //设置发送线程运行状态
    void SetSendMediaFunFlag(const bool bRunFlag);

    void acquireSendThreadSemaphore();

private:
    int insert(const RTP_PACK_INFO_S &info);

    int insertRange(const RTP_PACK_INFO_S &info, unsigned int unStartPos, unsigned int unStopPos);

    void checkFrame();

    void handleFinishedFrame(unsigned int unEndPos);

    void releaseRtpPacket(unsigned int unEndPos);
private:
    unsigned int            m_unMaxCacheFrameNum;       // 最大缓冲帧数(按包来计算太不准了)
    IRtpFrameHandler*       m_pRtpFrameHandler;         // RTP帧回调处理接口

    RTP_PACK_QUEUE          m_RtpQueue;
    RTP_PACK_QUEUE          m_RtpAudioQueue;
    unsigned int            m_unCurFrameNum;            // 已缓存的帧数
    unsigned int            m_unLastDequeueSeq;         // 最后一次出队列的RTP包序号
    bool                    m_bSeqReversal;             // 序列号翻转标识
    unsigned int            m_unSeqReversalPos;         // 序列号发生翻转的位置
    std::string             m_strServiceID;
    int                     m_iFrameNumber;
    bool                    m_bIsLostPacket;

    bool    m_bRunFlag;
    bool    m_bThreadExit;

    ACE_Thread_Semaphore * m_pSendThreadSemaphore;     //PS转ES线程信号量

    int                    m_iRtpQueueLogThreshold;
};

#endif /* __RTPFRAMEORGANIZER_H__ */

