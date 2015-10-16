/********************************************************************
filename    :    MainCtrl.h
author      :    s00191067
created     :    2012/03/27
description :    主线程控制模块
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2012/03/27 初始版本
*********************************************************************/
#ifndef MAIN_CTRL_H
#define MAIN_CTRL_H

#include "IService.h"
#include "bp_namespace.h"
#include "bp_mem.h"
#include "ThreadMgr.h"
#include "DumpProcess.h"
#include "HandleSignal.h"
#include "bp_memoutput.h"
#include "ILink.h"
#include "bp_config_alarm.h"
#include "Onlyone_Process.h"

#define WRITE_STATFILE_INTERVAL 5
#define PRINT_STAT_INFO_INTERVAL 60

NAMESPACE_BP_BEGIN // namespace begin


const unsigned int WARN_PATH_NAME_LENGTH = 256;
const unsigned int WARN_FILE_NAME_LENGTH = 96; /* Moudle_Type(32) + Node_Code(32) + Other(32) */

typedef struct LogAlarmInfo
{
    uint32_t      uiErrorCode;                       // 错误码
    char          szPathname[WARN_PATH_NAME_LENGTH]; // 出错时正在操作的文件路径名
    char          szFilename[WARN_FILE_NAME_LENGTH]; // 出错时正在操作的文件名
} LOG_ALARM_INFO;

class EXPORT_DLL CMainCtrl : public IService
{
public:
    CMainCtrl(void);
    virtual ~CMainCtrl(void);

    static void SetMainCtrl(CMainCtrl* pMainCtrl);
    static CMainCtrl* GetMainCtrl();

	void   SetIsMonitor(bool bIsMonitor){m_bIsMonitor = bIsMonitor;};

    virtual void SetParam(int argc, char* argv[]);

    // 播发命令，具体实现类需要处理命令
    virtual int HandleCmd(CCmd* pCmd);

    /**************************************************************************
    * name       : Routing
    * description: 定期检查，具体任务实现类处理定期检查任务
    * input      : NA
    * output     : NA
    * return     : IVS_SUCCEED - 成功，IVS_FAIL - 失败
    * remark     : NA    
    **************************************************************************/
    virtual uint32_t Routing(void);

    /**************************************************************************
    * name       : Init
    * description: 初始化主线程公共逻辑
    * input      : NA
    * output     : NA
    * return     : IVS_SUCCEED - 成功，IVS_FAIL - 失败
    * remark     : NA    
    **************************************************************************/
    virtual int Init();

    /**************************************************************************
    * name       : UnInit
    * description: 销毁操作
    * input      : NA
    * output     : NA
    * return     : NA
    * remark     : NA    
    **************************************************************************/
    virtual void UnInit();

    /**************************************************************************
    * name       : UnInit
    * description: 初始化日志
    * input      : pszLogPath - 日志文件根目录, NULL时取用配置中的设定目录
    * output     : NA
    * return     : NA
    * remark     : NA    
    **************************************************************************/
    virtual int InitLog(const char * pszLogPath = NULL);

    /**************************************************************************
    * name       : UnInit
    * description: 初始化线程监控
    * input      : NA
    * output     : NA
    * return     : NA
    * remark     : NA    
    **************************************************************************/
    virtual int InitMon(void);


    /**************************************************************************
    * name       : LoadNetService
    * description: 加载网元服务
    * input      : NA
    * output     : NA
    * return     : NA
    * remark     : NA    
    **************************************************************************/
    virtual int LoadNetService(void);

    /**************************************************************************
    * name       : UnInit
    * description: 加载业务服务
    * input      : NA
    * output     : NA
    * return     : NA
    * remark     : NA    
    **************************************************************************/
    virtual int LoadBusinessService(bool bConfig = true);

    /**************************************************************************
    * name       : LoadLogService
    * description: 加载日志入库服务
    * input      : NA
    * output     : NA
    * return     : NA
    * remark     : NA    
    **************************************************************************/
    virtual int LoadLogDbService(bool bEnableOperationLog, bool bEnableSecurityLog);

protected:
    /**************************************************************************
    * name       : ProcessStat
    * description: 进程定时写文件，用于Daemon进程监控子进程是否僵死
    * input      : NA
    * output     : NA
    * return     : NA
    * remark     : NA    
    **************************************************************************/
    void ProcessStat(void); 

    /**************************************************************************
    * name       : PrintStatInfo
    * description: 打印统计信息，如TPS，队列长度等;
    * input      : NA
    * output     : NA
    * return     : NA
    * remark     : NA    
    **************************************************************************/
    void PrintStatInfo(void); 

	/**************************************************************************
    * name       : LinkStatusReport
    * description: 处理LINK状态上报事件
    * input      : pCmd  --- LINK状态事件命令
    * output     : NA
    * return     : NA
    * remark     : NA    
    **************************************************************************/
    void LinkStatusReport(CCmd* pCmd);

    virtual void OnLinkOpened(const std::string& strDevName, int iNetElementType, const std::string& strLinkID, const std::string& strIP, int iPort);

    virtual void OnLinkClosed(const std::string& strDevName, int iNetElementType, const std::string& strLinkID, const std::string& strIP, int iPort);

	virtual void OnLinkTimeout(const std::string& strDevName, int iNetElementType, const std::string& strLinkID, const std::string& strIP, int iPort);

	virtual void OnLinkOpened(int iNetElementType, LINK_STATUS_REPORT_INFO *pInfo){};

	virtual void OnLinkClosed(int iNetElementType, LINK_STATUS_REPORT_INFO *pInfo){};

	virtual void OnLinkTimeout(int iNetElementType, LINK_STATUS_REPORT_INFO *pInfo){};

	virtual void OnLinkStatusWarn(int iNetElementType, LINK_STATUS_REPORT_INFO *pInfo){};

	void FluxControlWarn(CCmd* pCmd);

	virtual void OnFluxControlWarn(FLUX_CONTROL_WARN_INFO *pInfo){};

	virtual void OnFluxControlResume(FLUX_CONTROL_WARN_INFO *pInfo){};

	void LogWarn(CCmd *pCmd);

	void DBWarn(CCmd *pCmd);

	void ConfigWarn(CCmd *pCmd);

	virtual void OnLogWarn(LOG_ALARM_INFO *pLogAlarmInfo){};

	virtual void OnDBWarn(DB_ALARM_INFO *pDBAlarmInfo){};

	virtual void OnConfigWarn(CONFIG_ALARM_INFO *pConfigAlarmInfo){};

    int LoadDBMonitorService();
private:
    /**************************************************************************
    * name       : ProcessLinkStatusAlarm
    * description: 处理LINK状态告警事件
    * input      : NA
    * output     : NA
    * return     : NA
    * remark     : NA    
    **************************************************************************/
	void ProcessLinkStatusAlarm();

    /**************************************************************************
    * name       : CleanAlarmMsgStorage
    * description: 处理缓存在告警消息仓库中的告警
    * input      : NA
    * output     : NA
    * return     : NA
    * remark     : NA    
    **************************************************************************/
    void CleanAlarmMsgStorage();

protected:
    int           m_iArgc;

    char **       m_pszArgv;

private:
    time_t         m_iLastWriteStatTime;        // 上次进程文件写入时间

    CMemOutput     m_MemOutput;                 // 操作日志和安全日志缓存类

    std::string    m_ModuleType;                // 模块 类型

    time_t         m_tLastCheckTransactionTime; // 上次检测Transaction的时间

	bool           m_bIsMonitor;                // 是否是守护进程
#ifdef ARM_HIS3531
    time_t         m_tLastRoutingTime; // 上次运行Routing的时间
#endif
};


#define BP_MAIN(classname)                                                         \
do                                                                                 \
{                                                                                  \
	printf("Service Start to Run....\n");                                          \
	if(1 < argc && (0 == strcmp("-v", argv[1]) || 0 == strcmp("-V", argv[1])))     \
    {                                                                              \
	    printf("Compile time: %s %s.\n"                                            \
                  , __DATE__, __TIME__);                                           \
	    break;                                                                     \
    }                                                                              \
    if(!Onlyone_Process::onlyone(argv[0]))                                         \
    {                                                                              \
        break;                                                                     \
    }                                                                              \
    ACE::init();                                                                   \
    InitExceptionFilter();                                                         \
    classname *pMainService = NULL;                                                \
    HW_NEW(pMainService, classname);                                               \
    if (NULL == pMainService)                                                      \
    {                                                                              \
        return -1;                                                                 \
    }                                                                              \
    pMainService->SetParam(argc, argv);                                            \
    int iRet = pMainService->InitLog();                                            \
    if (IVS_SUCCEED != iRet)                                                       \
    {                                                                              \
        return iRet;                                                               \
    }                                                                              \
    CHandleSignal::Init();                                                         \
    CThread* pMainThread = NULL;                                                   \
    HW_NEW(pMainThread, CThread);                                                  \
    if (NULL == pMainThread)                                                       \
    {                                                                              \
        IVS_LogCleanOutput();                                                      \
        IVS_LogClean();                                                            \
        HW_DELETE(pMainService);                                                   \
        ACE::fini();                                                               \
        return -1;                                                                 \
    }                                                                              \
    CThread::SetMainThread(pMainThread);                                           \
    pMainThread->open("main", false);                                              \
    CThreadMgr::instance().AddThread(pMainThread);                                 \
    if (BP_OK != pMainService->Init())                                             \
    {                                                                              \
        pMainService->UnInit();                                                    \
        HW_DELETE(pMainService);                                                   \
        ACE::fini();                                                               \
        return -1;                                                                 \
    }                                                                              \
    CMainCtrl::SetMainCtrl(pMainService);                                          \
    pMainThread->AddService(pMainService);                                         \
	printf("Service Runing....\n");                                                \
    pMainThread->svc();                                                            \
    CMainCtrl* pMainCtrl = CMainCtrl::GetMainCtrl();                               \
    if (NULL != pMainCtrl)                                                         \
    {                                                                              \
        pMainCtrl->UnInit();                                                       \
    }                                                                              \
    ACE::fini();                                                                   \
	printf("Service Close....\n");                                                 \
} while(false)

NAMESPACE_BP_END   // namespace end

#endif //MAIN_CTRL_H
