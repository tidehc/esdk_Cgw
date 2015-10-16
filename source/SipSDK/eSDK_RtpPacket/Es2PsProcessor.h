/********************************************************************
 filename    :    Es2PsProcessor.h
 author      :    shilei 00194833
 created     :    2013-02-22
 description :    实现一个ES流转换成PS流的处理器
 copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
 history     :    2013-02-22 初始版本
*********************************************************************/

#ifndef _ES_TO_PS_PROCESSOR_H_
#define _ES_TO_PS_PROCESSOR_H_

#include <string>

#include "MediaBlockBuffer.h"
#include "RtpFrameOrganizer.h"
//#include "PsRtpEncap.h"
//CPsRtpEncapHandle
class CEs2PsProcessor: public CPacketChangeProcessor, public IRtpFrameHandler
{
public:
    CEs2PsProcessor();

    virtual ~CEs2PsProcessor();

    int init(std::string strServiceID);
    
	int setIsNeedAudioFrame(bool isNeedAudioFrame);
    /*****************************************************************************
     Function:    receviePacket
     Description: 接收一包ES媒体数据
     Input:       pMb 媒体流数据块
     Output:      
     Return:      N/A
    *****************************************************************************/
    void receviePacket (const ACE_Message_Block* pMb);

    /*****************************************************************************
     Function:    handleMediaFrame
     Description: 拼完一帧ES数据后处理媒体数据
     Input:       rtpFrameList ES数据包列表
     Output:      
     Return:      N/A
    *****************************************************************************/
    void handleMediaFrame(RTP_FRAME_LIST_T &rtpFrameList);


    void SetFrameOrganiaerSendMediaFunFlag(const bool bFunFlag);
    void acquireFrameOrganiaerSendThreadSemaphore();

private:
    
    /*****************************************************************************
     Function:    handleAudioRtpFrame
     Description: 拼完一帧ES数据后处理媒体数据
     Input:       rtpFrameList ES数据包列表音频
     Output:      
     Return:      N/A
    *****************************************************************************/
    void handleAudioRtpFrame(RTP_FRAME_LIST_T &rtpFrameList);

    /*****************************************************************************
     Function:    handleVideoRtpFrame
     Description: 拼完一帧ES数据后处理媒体数据
     Input:       rtpFrameList ES数据包列表视频
     Output:      
     Return:      N/A
    *****************************************************************************/
    void handleVideoRtpFrame(RTP_FRAME_LIST_T &rtpFrameList);

    /*****************************************************************************
     Function:    parseSingleNalu
     Description: 处理单个Nalu
     Input:       rtpFrameList ES数据包列表视频
     Output:      
     Return:      IVS_SUCCEED 成功
                  其他 失败
    *****************************************************************************/
    int parseSingleNalu(RTP_FRAME_LIST_T &rtpFrameList);

    /*****************************************************************************
     Function:    sendPsRtpPacket
     Description: 接收一包ES媒体数据
     Input:       pRtpData  解析成PS流后的一包数据
                  unDataSize PS流的一包数据的长度
     Output:      
     Return:      N/A
    *****************************************************************************/
    void sendPsRtpPacket(const char* pRtpData, unsigned int unDataSize,
            void* pUserData);

    /*****************************************************************************
     Function:    handleSingleNalu
     Description: 处理单个Nalu
     Input:       pRtpBlock 数据块
                  rtpPacket rtp包信息
                  unCacheSize 缓存的剩余空间
     Output:      
     Return:      
    *****************************************************************************/
    void handleSingleNalu(ACE_Message_Block* pRtpBlock,const CRtpPacket &rtpPacket,
            unsigned int &unCacheSize);

    void ReleasePktList();
private:    
    std::string             m_strServiceID;         // 业务ID，主要用于打印日志

    CRtpFrameOrganizer      m_RtpFrameOrganizer;
    //CPsRtpEncap             m_PsRtpEncap;

    ACE_Recursive_Thread_Mutex       m_listMutex;             //RTSP会话MAP表锁
    std::list <ACE_Message_Block*>                  m_curPacketList;
    char*                   m_pRtpFrameCache;       // 存储一帧数据的缓存，注意内存释放
    char*                   m_pWritePos;            // 操作m_pRtpFrameCache内存指针

    ACE_Message_Block*      m_pExtendHeader;                
    suseconds_t             m_usecLastSendTime;
    time_t                  m_tmLastSendTime;
    std::string             m_strSessionID;

	bool                    m_bIsNeedAudioFrame;   // 是否输出音频帧
	char					m_VideoStreamType;

};

#endif //_ES_TO_PS_PROCESSOR_H_
