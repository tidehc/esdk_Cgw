/******************************************************************************
   版权所有 (C), 2001-2011, 华为技术有限公司

 ******************************************************************************
  文件名          : CNVSTimer.h
  版本号          : 1.0
  作者            : duijiying 24362
  生成日期        : 2007-4-02
  最近修改        : 
  功能描述        : 
  函数列表        : 
  修改历史        : 
    日期          : 2007-4-02
    作者          : duijiying
    修改内容      : 生成
*******************************************************************************/

#ifndef IVSCBB_SS_NVS_TIMER_H
#define IVSCBB_SS_NVS_TIMER_H

#ifdef WIN32
#pragma warning(disable: 4786)
#endif

#include <list>
#include <map>

#include "vos_config.h"
#include "vos_basetype.h"
#include "vos_common.h"
#include "vos.h"

using namespace std;

namespace SipStack{
const ULONG DefaultTimerScale = 100; //缺省定时精度为100ms
const ULONG MinTimerScale = 1; //定时精度最小为1ms

class ITrigger;
class CTimerItem;

class CCmpTimeOut
{
  public:
    bool operator()(const ULONGLONG ullTimerOut1, const ULONGLONG ullTimerOut2) const
    {
        return (ullTimerOut1 < ullTimerOut2);
    };
};
typedef multimap<ULONGLONG, CTimerItem *, CCmpTimeOut> ListOfTrigger;
typedef pair<ULONGLONG const, CTimerItem *> ListOfTriggerPair;
typedef ListOfTrigger::iterator ListOfTriggerIte;

typedef enum tagTriggerStyle
{
    enOneShot = 0,
    enRepeated = 1
} TriggerStyle;

class ITrigger 
{
  public:
    ITrigger()
    {
        m_pTimerItem = NULL;
    };
    virtual ~ITrigger(){};
    
  public:
    virtual void OnTrigger(void *pArg, ULONGLONG ullScales, TriggerStyle enStyle) = 0;
        
  public:
    CTimerItem *m_pTimerItem;
};

class CTimerItem
{
  public:
    CTimerItem()
    {
        m_pTrigger = NULL;
        m_pArg = NULL;
        m_bRemoved = VOS_FALSE;
		m_ulInitialScales = 0;
		m_ullCurScales = 0;
		m_ulBaseTimeTicks = 0;
		m_enStyle = enOneShot;
    };
    
  public:
    ITrigger *m_pTrigger;
    void *m_pArg;
    ULONG m_ulInitialScales; 
    ULONGLONG m_ullCurScales; 
    TriggerStyle m_enStyle;
    VOS_BOOLEAN m_bRemoved;

    //BEGIN V100R001C05 ADD 2010-02-03 ligengqiang l00124479 for 合入朱玉军解决的黄石解码器下线问题 NP-231 
    ULONG m_ulBaseTimeTicks;    //当前基准时间 
    //END   V100R001C05 ADD 2010-02-03 ligengqiang l00124479 for 合入朱玉军解决的黄石解码器下线问题 NP-231
};

} //end namespace

// 4种日志操作
#define    TIMER_OPERATOR_LOG    16
#define    TIMER_RUN_LOG         17
#define    TIMER_SECURITY_LOG    20
#define    TIMER_USER_LOG        19

// 4种日志级别
enum TIMER_LOG_LEVEL
{
    TIMER_EMERGENCY = 0,
    TIMER_ERROR = 3,
    TIMER_WARNING = 4,
    TIMER_DEBUG = 7
};

namespace SipStack{

//日志打印接口
class ITimerLog 
{
  public:
    virtual ~ITimerLog()
    {
    }
    virtual void writeLog(long iType, long ilevel, 
        const char *szLogDetail, const long iLogLen) = 0;
};

extern ITimerLog *g_pTimerLog;

class CNVSTimer
{
private:
    CNVSTimer()
    {
        m_plistTrigger = NULL;
        m_ulTimerScale = DefaultTimerScale;
        m_ullRrsAbsTimeScales = 0;
        m_pMutexListOfTrigger = NULL;
        m_pVosThread = NULL;
        m_bExit = VOS_FALSE;
    }
    CNVSTimer(const CNVSTimer&);
    CNVSTimer& operator=(const CNVSTimer&);
public:
    virtual ~CNVSTimer();

    static CNVSTimer& instance()
    {
        if (NULL == m_pTimer)
        {
            try
            {
                m_pTimer = new CNVSTimer;
            }
            catch(...)
            {
                static CNVSTimer timer;
                m_pTimer = &timer;
            }
        }

        return (*m_pTimer);
    }
  
    virtual long init(ULONG ulTimerScale);
    void setLogWriter(ITimerLog *pTimerLog)
    {
        g_pTimerLog = pTimerLog;
    };
    virtual long run();
    void exit();

     virtual long registerTimer(ITrigger *pRrsTrigger, void *pArg, ULONG nScales, 
        TriggerStyle enStyle);
     virtual long cancelTimer(ITrigger *pRrsTrigger);

    void clearTimer( );
    static void *invoke(void *argc)
    {
        CNVSTimer *pTimer = (CNVSTimer *)argc;
        pTimer->mainLoop();
        VOS_pthread_exit(NULL);
        return NULL;
    };

    void mainLoop();

private:
    static CNVSTimer* m_pTimer;

    ULONG m_ulTimerScale;
    ULONGLONG m_ullRrsAbsTimeScales;
    ListOfTrigger *m_plistTrigger;
    VOS_Mutex *m_pMutexListOfTrigger;
    VOS_Thread *m_pVosThread;
    volatile VOS_BOOLEAN m_bExit;    
};
}//end namespace

#endif //CTIMER_H_INCLUDE


