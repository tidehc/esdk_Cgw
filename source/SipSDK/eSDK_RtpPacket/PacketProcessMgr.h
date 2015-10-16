#ifndef _PACKET_PROCESS_MGR_
#define	_PACKET_PROCESS_MGR_

#include <map>
#include "ace_header.h"
enum EM_CONVERT_TYPE
{
	PS2ES	=	0,
	ES2PS	=	1
};
class CPacketChangeProcessor;

#define PCAKETPROCESSORMGR() ESDK::CPacketProcessMgr::GetInstance()
namespace ESDK
{
	typedef std::map<UINT32,CPacketChangeProcessor*> PROCESSOR_MAP;
	typedef std::map<UINT32,CPacketChangeProcessor*>::iterator PROCESSOR_MAP_ITER;
	class CPacketProcessMgr
	{
	private:
		CPacketProcessMgr(void);
		~CPacketProcessMgr(void);
	public:
		static CPacketProcessMgr& GetInstance(void);
		static CPacketProcessMgr* m_Instance;
	public:
		CPacketChangeProcessor* GetProcessor(UINT32);
		CPacketChangeProcessor* OpenChannel(UINT32 uiChannel,int iConvertType);
		int CloseChannel(UINT32 uiChannel);
	private:
		PROCESSOR_MAP m_ProcessMap;	//处理器表
		ACE_Mutex m_Mutex;			//处理器m_ProcessMap的查询修改锁

	};
}

#endif

