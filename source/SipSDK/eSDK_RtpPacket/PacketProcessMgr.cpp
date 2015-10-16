#include "StdAfx.h"
#include "PacketProcessMgr.h"
#include "RtpFrameOrganizer.h"
#include "Ps2EsProcessor.h"
#include "Es2PsProcessor.h"
#include "Log.h"

using namespace ESDK;
//lint -e438

CPacketProcessMgr* CPacketProcessMgr::m_Instance = NULL;

CPacketProcessMgr::CPacketProcessMgr(void)
{
}


CPacketProcessMgr::~CPacketProcessMgr(void)
{
}


CPacketProcessMgr& CPacketProcessMgr::GetInstance()
{
	if(NULL == m_Instance)
	{
		m_Instance = new CPacketProcessMgr;
	}

	return *m_Instance;
}
	
CPacketChangeProcessor* CPacketProcessMgr::GetProcessor(UINT32 uiChannel)
{
	m_Mutex.lock();
	if(m_ProcessMap.end() == m_ProcessMap.find(uiChannel))
	{
		m_Mutex.release();
		WARN_LOG("Not Find Processor.");
		return NULL;
	}

	CPacketChangeProcessor* processor = m_ProcessMap[uiChannel];
	m_Mutex.release();
	return processor;
}

CPacketChangeProcessor* CPacketProcessMgr::OpenChannel(UINT32 uiChannel,int iConvertType)
{
	m_Mutex.lock();
	if(m_ProcessMap.end() != m_ProcessMap.find(uiChannel))
	{	
		m_Mutex.release();		
		return m_ProcessMap[uiChannel];
	}

	CPacketChangeProcessor* processor = NULL;
	if(PS2ES == iConvertType)
	{
		processor = new CPs2EsProcessor();
	}
	else
	{
		processor = new CEs2PsProcessor();
	}
	(void)processor->init("ID");
	m_ProcessMap[uiChannel] = processor;
	m_Mutex.release();
	processor->SetChannel(uiChannel);

	return processor;
}
int CPacketProcessMgr::CloseChannel(UINT32 uiChannel)
{
	m_Mutex.lock();
	if(m_ProcessMap.end() == m_ProcessMap.find(uiChannel))
	{
		m_Mutex.release();
		WARN_LOG("Not Find Processor.");
		return 1;
	}

	CPacketChangeProcessor* processor = m_ProcessMap[uiChannel];
	m_ProcessMap.erase(uiChannel);
	m_Mutex.release();
	processor->SetChannel(0);
	delete processor;
	processor = NULL;
	return 0;
}

//lint +e438
