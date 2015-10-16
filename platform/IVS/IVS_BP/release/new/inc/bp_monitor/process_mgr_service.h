/********************************************************************
filename    :     process_mgr_service.h
author      :     s69859
created     :     2012/09/20
description :     监控进程,通过信号量拉起子进程
copyright   :     Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :     2012/09/20 初始版本
*********************************************************************/

#ifndef PROCESS_MGR_SERVICE_H_
#define PROCESS_MGR_SERVICE_H_

#include "MainCtrl.h"
#include "ivs_xml.h"
#include "bp_config_alarmer.h"

#define TRUE_TAG  "True"
#define FALSE_TAG "False"
#define  MAXPROCESS 16
#define MAXPHL 1024


NAMESPACE_BP_BEGIN

typedef struct stProcessStatus
{
	//进程名
	std::string szProcessName;//[MAXPHL];

	//模块名--用于生成告警文件的命名
	std::string szModuleName;//[MAXPHL];

	//是否加载
	std::string szIfLoad;//[MAXPHL];

	//扩展文件路径
	std::string szExeFilePath;//[MAXPHL];

	//是否需要守护
	std::string szIsKill;

	//工作模式，单机和集群:0 ; 双机:1
	std::string szWorkMode;

	//该标志位用于判断是否需要产生告警文件
	bool bifRuning;

	bool bifFristRun;

	pid_t iProcessID;
	
	stProcessStatus()
	{
		  bifRuning = false;
		  bifFristRun = true;
		  iProcessID = 0;
		  szProcessName = "";
		  szModuleName = "";
		  szIfLoad = "";
		  szExeFilePath = "";
		  szWorkMode = "";
		  szIsKill = "";
	}
	
    //用于结构体互斥访问的锁
    BP_Recursive_Thread_Mutex mutex;
	
} PROCESS_STATUS;

class CProcessMgrService : public CMainCtrl, private CConfigAlarmer
{
public:
     CProcessMgrService();
	 virtual ~CProcessMgrService();
	 virtual uint32_t Routing(void);
	 virtual int HandleCmd(CCmd* pCmd);
	 virtual void InitService(void);
	 PROCESS_STATUS* getProcessMap();
	 int getProcessMapSize();
private:
     void runDaemon(void);
     bool initMember(CBB::CXml& oCXml);
	 bool DaemonCreateProcess(PROCESS_STATUS* pProcessMap, int pIndex);
	 bool CreateWarnFile(std::string szFilePath);
#ifdef WIN32
	 int InitWinService();
#endif

private:
	void checkPidFile();

    void CheckProcessItemConfig(
        const std::vector<bool> & vFind, 
        const std::vector<std::string> & vItem, 
        const std::vector<std::string> & vValue, 
        const std::vector<std::string> & vDefault);

private:
    int m_iConfNum;
	PROCESS_STATUS m_oProcessMap[MAXPROCESS];
	std::string m_szWarnFilePath;
	time_t m_iLastCheckStatTime;
};
NAMESPACE_BP_END
#endif /* PROCESS_MGR_SERVICE_H_ */
