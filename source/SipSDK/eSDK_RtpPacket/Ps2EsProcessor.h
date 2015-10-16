/********************************************************************
 filename    :    Ps2Es.h
 author      :    
 created     :    
 description :    实现一个PS流转换成ES流的处理器
 copyright   :    
 history     :    
*********************************************************************/
#ifndef _PS_TO_ES_ 
#define _PS_TO_ES_ 

#include "RtpFrameOrganizer.h"
#include "HstPESParse.h"
#include "MediaBlockBuffer.h"
#include "RtpPacket.h"
#include "Lock.h"

class CPs2EsProcessor : public CPacketChangeProcessor, public IRtpFrameHandler
{
public:
    CPs2EsProcessor();

    ~CPs2EsProcessor();

    int  init(std::string strServiceID);

	int setIsNeedAudioFrame(bool isNeedAudioFrame);

    /*****************************************************************************
     Function:    receviePSPacket
     Description: 接收一包PS媒体数据
     Input:       pMb 媒体流数据块
     Output:      
     Return:      N/A
    *****************************************************************************/
    void receviePacket(const ACE_Message_Block* pMb);

    /*****************************************************************************
     Function:    handleMediaFrame
     Description: 拼完一帧PS数据后处理媒体数据
     Input:       rtpFrameList PS数据包列表
     Output:      
     Return:      N/A
    *****************************************************************************/
    void handleMediaFrame(RTP_FRAME_LIST_T &rtpFrameList);


private:
    /*****************************************************************************
     Function:    parseSingleNalu
     Description: 
     Input:       rtpFrameList ES数据包列表
     Output:      
     Return:      N/A
    *****************************************************************************/
    int parseSingleNalu(char *&pReadPoint, char *&pCopyPoint, int &iLeftLength, unsigned char &ucType);

    /*****************************************************************************
     Function:    checkStartCode
     Description: 校验数据包起始码
     Input:       
     Output:      
     Return:      true 成功
                  其他 失败
    *****************************************************************************/
    bool checkStartCode(char *pReadPoint, unsigned int &uiStartCodeLength);

    /*****************************************************************************
     Function:    HSPspkt2ESFrm
     Description: 拼完一帧PS数据后处理媒体数据
     Input:       pPacket PS帧数据缓存
                  iPktLen PS帧数据长度
     Output:      pFrame 转换后数据存储缓存
                  iVideoLen 视频长度
                  iAudioLen 音频长度
     Return:      IVS_SUCCEED 成功
                  其他 失败
    *****************************************************************************/
    int  HSPspkt2ESFrm(unsigned char *pPacket, int iPktLen, unsigned char *&pFrame,
        int &iVideoLen, int &iAudioLen);

    /*****************************************************************************
     Function:    sendAudioFrame
     Description: 发送ES音频帧
     Input:       pVideobuff ES帧数据缓存
                  iVideobuffLen ES帧数据长度
     Output:      
     Return:      IVS_SUCCEED 成功
                  其他 失败
    *****************************************************************************/
    int  sendAudioFrame(char *pAudiobuff, int iAudiobuffLen);

    /*****************************************************************************
     Function:    sendVideoFrame
     Description: 发送ES视频帧
     Input:       pVideobuff ES帧数据缓存
                  iVideobuffLen ES帧数据长度
     Output:      
     Return:      IVS_SUCCEED 成功
                  其他 失败
    *****************************************************************************/
    int  sendVideoFrame(char *pVideobuff, int iVideobuffLen);

    /*****************************************************************************
     Function:    parseH264frame
     Description: 解析H264帧
     Input:       pVideobuff ES帧数据缓存
                  iBuffLen ES帧数据长度
     Output:      
     Return:      IVS_SUCCEED 成功
                  其他 失败
    *****************************************************************************/
    int parseH264frame(char *pVideobuff, int iBuffLen);

    /*****************************************************************************
     Function:    cutSliceToRtp
     Description: 将一个slice拆分成Rtp包
     Input:       pVideobuff ES帧数据缓存
                  iBuffLen ES帧数据长度
     Output:      
     Return:      IVS_SUCCEED 成功
                  其他 失败
    *****************************************************************************/
    int cutSliceToRtp(char *pVideobuff, int iBuffLen);

    /*****************************************************************************
     Function:    setRtpInfo
     Description: 将一个slice拆分成Rtp包
     Input:       pRtpHead RTP头
                  ucFrameType 帧类型
                  bIsMarker mark标志是否有效
     Output:      
     Return:      
    *****************************************************************************/
    void setRtpInfo(char *pRtpHead, unsigned char ucFrameType, bool bIsMarker);

    /*****************************************************************************
     Function:    sendEsRtpPacket
     Description: 
     Input:       pRtpData  解析成ES流后的一包数据
                  unDataSize ES流的一包数据的长度
     Output:      
     Return:      N/A
    *****************************************************************************/
    void sendEsRtpPacket(const char* pRtpData, unsigned int unDataSize);

    void ReleasePktList();

    void ReleaseFrameList();
private:
    void appendExtInfo(ACE_Message_Block *pMsg);

private:
    std::string             m_strServiceID;         // 业务ID，主要用于打印日志
    
    CRtpFrameOrganizer      m_RtpFrameOrganizer;    // 组帧对象
    CHstPESParse*           m_pHstPESParse;         // PS转换成ES流对象，注意内存释放

    char*                   m_pRtpFrameCache;       // 存储一帧数据的缓存，注意内存释放
    char*                   m_pWritePos;            // 操作m_pRtpFrameCache内存指针

    ACE_Recursive_Thread_Mutex       m_listMutex;             //RTSP会话MAP表锁
    std::list <ACE_Message_Block*>    m_curPacketList;
    std::list <ACE_Message_Block*>    m_curFrameList;  // 这个list是在解析H264是使用

    ACE_Message_Block*      m_pExtendHeader;                

    // RTP包媒体流信息
    unsigned long           m_ulVideoTimeTick;
    unsigned long           m_ulAudioTimeTick;
    unsigned short          m_usVideoRtpSeq;        
    unsigned short          m_usAudioRtpSeq;        
    unsigned char           m_ucVideoPayloadType;
    
    bool                    m_bIsFirstTime;

	bool                    m_bIsNeedAudioFrame;   // 是否输出音频帧


    suseconds_t             m_usecLastSendTime;
    time_t                  m_tmLastSendTime;
    unsigned int            m_uiRealRecordSecond;
    unsigned int            m_uiRealRecordMSecond;
    unsigned int            m_uiReserved;

	CIVSMutex				m_Mutex;
};

#endif //_PS_TO_ES_

