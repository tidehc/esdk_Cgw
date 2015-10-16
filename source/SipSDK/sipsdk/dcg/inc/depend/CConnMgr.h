/******************************************************************************
   版权所有 (C), 2001-2011, 华为技术有限公司

 ******************************************************************************
  文件名          : CConnMgr.h
  版本号          : 1.0
  作者            : duijiying 24362
  生成日期        : 2007-4-02
  最近修改        : 
  功能描述        : 
  函数列表        : 
  修改历史        : 
  1 日期          : 2007-4-02
    作者          : duijiying
    修改内容      : 生成
*******************************************************************************/

#ifndef IVSCBB_SS_CONN_MGR_H
#define IVSCBB_SS_CONN_MGR_H

#include <list>
#include "vos.h"
#include <cstring>

#define InvalidFd              -1
#define InvalidSocket          -1
#define InvalidIp              INADDR_ANY
#define Invalidport            0
#define DEFAULT_SENDRECV_SIZE (1024 * 1024)

#define SendRecvError       -1//发送或接收错误
#define SendRecvErrorTIMEO  -2//发送或接收超时
#define SendRecvErrorEBADF  -3//socket句柄错误
#define SendRecvErrorEOF    -4//tcp断连

#define MAX_LISTEN_QUEUE_SIZE 2000
#define EPOLL_MAX_EVENT       (MAX_LISTEN_QUEUE_SIZE + 1000)
#define MAX_EPOLL_FD_SIZE     3000
#define LINGER_WAIT_SECONDS   1 //LINGER等待时间(seconds)

#if VOS_APP_OS == VOS_OS_LINUX
	#define SOCKET         int
	//#define socklen_t      int
	#define datalen_t      size_t
	#define CLOSESOCK(x)   ::close(x)
	#define SOCK_OPT_TYPE  void
	#define CONN_ERRNO     errno
#elif VOS_APP_OS == VOS_OS_WIN32
	#define CLOSESOCK(x) closesocket(x)
	#define datalen_t      int
	#define SOCK_OPT_TYPE  char
	#define snprintf       _snprintf
	#define vsnprintf      _vsnprintf
	#define CONN_ERRNO     WSAGetLastError()
	//#define EWOULDBLOCK    WSAEWOULDBLOCK
	//#define EINPROGRESS    WSAEINPROGRESS
#endif


//定义连接管理器得错误码
#if VOS_APP_OS == VOS_OS_LINUX
#define CONN_ERR_TIMEO      ETIMEDOUT
#define CONN_ERR_EBADF      EBADF
#elif VOS_APP_OS == VOS_OS_WIN32
#define CONN_ERR_EBADF      WSAEINTR 
#define CONN_ERR_TIMEO      WSAETIMEDOUT
#undef MSG_WAITALL          //windows 先取消这个宏定义
#define MSG_WAITALL 0       //windows XP不支持这个，导致接受错误
#endif


#if VOS_APP_OS == VOS_OS_WIN32
enum tagSockEvent
{
    EPOLLIN  = 0x1,
    EPOLLOUT = 0x2
};

// #ifndef INET_ADDRSTRLEN
// #define  INET_ADDRSTRLEN 16
// #endif

#ifndef MSG_DONTWAIT
#define MSG_DONTWAIT 0
#endif

#ifndef MSG_NOSIGNAL
#define MSG_NOSIGNAL 0
#endif

#endif //#if win32

#define DEFAULT_SELECT_PERIOD 20

#define MAX_HANDLE_MGR_TYPE_LEN 20
using namespace std;
namespace SipStack
{  
	typedef enum tagConnStatus
	{
		enIdle = 0,
		enConnecting,
		enConnected,
		enConnFailed,
		enClosed
	} ConnStatus;

	typedef enum tagEnumSyncAsync
	{
		enSyncOp = 1,
		enAsyncOp = 0
	} EnumSyncAsync;

	typedef enum tagEpollEventType
	{
		enEpollRead = 0,
		enEpollWrite = 1
	} EpollEventType;


 
	class CHandle;
	class CHandleNode;
	typedef list<CHandleNode *> ListOfHandle;
	typedef ListOfHandle::iterator ListOfHandleIte;

	class CNetworkAddr
	{
	  public:
		CNetworkAddr();
		~CNetworkAddr();
	  public:
		long m_lIpAddr;   //网络序列
		USHORT m_usPort;  //网络序列
	};

	class CHandle
	{
	private:
		CHandle(const CHandle& ){}
		CHandle& operator=(const CHandle&){}	
	public:
		CHandle();
		virtual ~CHandle();
    
	public:
		virtual long initHandle(void);
		virtual void setHandleSend(VOS_BOOLEAN bHandleSend);
		virtual void setHandleRecv(VOS_BOOLEAN bHandleRecv);
		ULONG getEvents(void)
		{
			if(m_pMutexHandle != NULL)
			{
				(void)VOS_MutexLock(m_pMutexHandle);
			}

			ULONG ulEvents = m_ulEvents;

			if(m_pMutexHandle != NULL)
			{
				(void)VOS_MutexUnlock(m_pMutexHandle);
			}
			return ulEvents;
		}; 
		virtual void close(void);
	
		public:
		long m_lSockFD;
		CHandleNode *m_pHandleNode;
		CNetworkAddr m_localAddr;
    
	#if VOS_APP_OS == VOS_OS_WIN32
		VOS_BOOLEAN m_bReadSelected;
		VOS_BOOLEAN m_bWriteSelected;
	#endif  //#if
    
	#if VOS_APP_OS == VOS_OS_LINUX
		long m_lEpfd;
	#endif  //#if
		ULONG m_ulEvents;
		VOS_Mutex *m_pMutexHandle;
	};

	class CHandleNode
	{
	  public:
		CHandleNode()
		{
			m_pHandle = NULL;
			m_bRemoved = VOS_FALSE;
		};
    
	  public:
		CHandle *m_pHandle;
		VOS_BOOLEAN m_bRemoved;
	};

	class CNetworkHandle : public CHandle
	{
	  public:
		CNetworkHandle();
		virtual ~CNetworkHandle(){};
    
	  public:
		virtual long initHandle(void);
		long getSockFD(void) const    /*lint -e1714*///接口函数，本类不调用
		{
			return m_lSockFD;
		};
	#if VOS_APP_OS == VOS_OS_LINUX
		long sendMsg(const struct msghdr *pMsg);
	#endif
		virtual long recv(char *pArrayData, CNetworkAddr *pPeerAddr, const ULONG ulDataSize, 
			const EnumSyncAsync bSyncRecv) = 0;

	  public:
		virtual void handle_recv(void) = 0;
		virtual void handle_send(void) = 0;
	};

	class CTcpConnHandle : public CNetworkHandle
	{
	  public:
		CTcpConnHandle();
		virtual ~CTcpConnHandle();
    
	  public:
		virtual long initHandle(void);
		virtual long conn( const CNetworkAddr *pLocalAddr, const CNetworkAddr *pPeerAddr,
			const EnumSyncAsync bSyncConn, ULONG ulTimeOut);
		// BS_Start
		virtual long connBind(const CNetworkAddr *pLocalAddr);
		virtual long connAfterBind(const CNetworkAddr *pLocalAddr, const CNetworkAddr *pPeerAddr,
			const EnumSyncAsync bSyncConn, ULONG ulTimeOut);
		// BS_End
		virtual long send(const char *pArrayData, const ULONG ulDataSize, 
			const EnumSyncAsync bSyncSend);
		virtual long send_n(const char *pArrayData, const ULONG ulDataSize);

		virtual long recv(char *pArrayData, CNetworkAddr *pPeerAddr, const ULONG ulDataSize, 
			const EnumSyncAsync bSyncRecv);
		virtual long recvWithTimeout(char *pArrayData, CNetworkAddr *pPeerAddr, 
			const ULONG ulDataSize, const ULONG ulTimeOut, const ULONG ulSleepTime);
		virtual ConnStatus getStatus(void) const
		{
			return m_lConnStatus;
		};
		virtual void close(void);
    
	  /* BEGIN: Added by xuhongyun, 2010/8/3   问题单号:圈复杂度修改*/
		// 把 conn函数中的一些操作提取出来
		int setSockOpt();

		int handleConnFail(const EnumSyncAsync bSyncConn, ULONG ulTimeOut);

		void setSockBlock(bool bBlocked);
	 /* END:   Added by xuhongyun, 2010/8/3 */
 
	  public:
		ConnStatus m_lConnStatus;
		CNetworkAddr m_peerAddr;
	};

	class CUdpSockHandle : public CNetworkHandle
	{
	  public:
		virtual long createSock(const CNetworkAddr *pLocalAddr, 
								   const CNetworkAddr *pMultiAddr);
		virtual long send(const CNetworkAddr *pPeerAddr, const char *pArrayData,
			 const ULONG ulDataSize, const EnumSyncAsync bSyncSend);
		virtual long recv(char *pArrayData, CNetworkAddr *pPeerAddr, const ULONG ulDataSize, 
			const EnumSyncAsync bSyncRecv);
		virtual void close(void);
		virtual long recvWithTimeout(char *pArrayData, 
											CNetworkAddr *pPeerAddr, 
											const ULONG ulDataSize, 
											const ULONG ulTimeOut, 
											const ULONG ulSleepTime);
	};

	class CTcpServerHandle : public CHandle
	{
	  public:
		long listen(const CNetworkAddr *pLocalAddr);

	  public:
		virtual long handle_accept(const CNetworkAddr *pRemoteAddr, 
			CTcpConnHandle *&pTcpConnHandle) = 0;

		// BEGIN  V100R002C01 ADD  2010-12-06 w68813 连接后的处理接口
		virtual void after_accept
		(
			const CNetworkAddr* pRemoteAddr,
			CTcpConnHandle*  pTcpConnHandle
		);
		// END    V100R002C01 ADD  2010-12-06 w68813
		virtual void close(void);
	};


	class  CHandleManager
	{
	  public:
		CHandleManager();
		virtual ~CHandleManager();
    
	  public:
		long init(const ULONG ulMilSeconds);
		long run();
		void exit();

	  public:
		long addHandle(CHandle *pHandle, 
						  VOS_BOOLEAN bIsListOfHandleLocked = VOS_FALSE);
		void removeHandle(CHandle *pHandle);
		virtual void checkSelectResult(const EpollEventType enEpEvent,
			CHandle *pHandle) = 0;

	  protected:
		static void *invoke(void *argc);
		void mainLoop();
	  /* BEGIN: Added by xuhongyun, 2010/8/3   问题单号:圈复杂度修改*/
	  // 把addHandle中的锁操作提取出来
	  VOS_BOOLEAN getMutex(VOS_BOOLEAN bIsListOfHandleLocked = VOS_FALSE);
	  void releaseMutex();
	  /* END:   Added by xuhongyun, 2010/8/3 */
	  protected:
		ListOfHandle m_listHandle;
		VOS_Mutex *m_pMutexListOfHandle;

	#if VOS_APP_OS == VOS_OS_LINUX
		long m_lEpfd; //用于epoll的句柄
		struct epoll_event m_epEvents[EPOLL_MAX_EVENT];
	#elif VOS_APP_OS == VOS_OS_WIN32
		fd_set m_readSet;        
		fd_set m_writeSet;
		timeval m_stSelectPeriod;            //select周期

		/* BEGIN: Added by xuhongyun, 2010/8/3   问题单号:圈复杂度修改*/
		// 把Windows下Select相关操作提取出来
		void setSelectFd();
		void checkSelectEvent();
	  /* END:   Added by xuhongyun, 2010/8/3 */
	#endif

		ULONG m_ulSelectPeriod;
		VOS_Thread *m_pVosThread;
		VOS_BOOLEAN m_bExit;
		VOS_Mutex *m_pMutexExit;
		char m_szMgrType[MAX_HANDLE_MGR_TYPE_LEN];
	};

	class CTcpConnMgr : public CHandleManager
	{
	  public:
		CTcpConnMgr()
		{
			(void)strcpy_s(m_szMgrType, "CTcpConnMgr");
		};
		void lockListOfHandle();
		void unlockListOfHandle();
    
	  protected:
		virtual void checkSelectResult(const EpollEventType enEpEvent,
								CHandle *pHandle);  /*lint !e1768*///需要对外屏蔽该接口

		/* BEGIN: Added by xuhongyun, 2010/8/3   问题单号:圈复杂度修改*/
		void checkConnectStatus(CTcpConnHandle *pTcpConnHandle) const;
		/* END:   Added by xuhongyun, 2010/8/3 */
	};

	class CUdpSockMgr : public CHandleManager
	{
	  public:
		CUdpSockMgr()
		{
			(void)strcpy_s(m_szMgrType, "CUdpSockMgr");
		};
    
	  protected:
		virtual void checkSelectResult(const EpollEventType enEpEvent,
			CHandle *pHandle);  /*lint !e1768*///需要对外屏蔽该接口
	};

	class CTcpServerMgr : public CHandleManager
	{
	  public:
		CTcpServerMgr()
		{
			m_pTcpConnMgr = NULL;
			(void)strcpy_s(m_szMgrType, "CTcpServerMgr");
		};
    
	  public:
		void setTcpClientMgr(CTcpConnMgr *pTcpConnMgr)
		{
			m_pTcpConnMgr = pTcpConnMgr;
		};
        
	  protected:
		virtual void checkSelectResult(const EpollEventType enEpEvent,
			CHandle *pHandle);  /*lint !e1768*///需要对外屏蔽该接口
	  protected:
		CTcpConnMgr *m_pTcpConnMgr;
	};

	class CConnMgr
	{
	#ifdef UT_AUTO_MOCK
		friend class CMediaSocketTester;
	#endif

	  public:
		CConnMgr();
		virtual ~CConnMgr();

		static CConnMgr& instance()
		{
			if(NULL == g_pManager)
			{
				g_pManager = new CConnMgr;
			}
			return (*g_pManager);
		}
	  public:
		virtual long init(const ULONG ulSelectPeriod, const VOS_BOOLEAN bHasUdpSock, 
			const VOS_BOOLEAN bHasTcpClient, const VOS_BOOLEAN bHasTcpServer);

		virtual long run(void);
		virtual void exit(void);
    
	  public:
		virtual void setDefaultLocalAddr(const char *szLocalIpAddr); 
		virtual long regTcpClient( const CNetworkAddr *pLocalAddr, 
			const CNetworkAddr *pPeerAddr, CTcpConnHandle *pTcpConnHandle, 
			const EnumSyncAsync bSyncConn, ULONG ulTimeOut);
		// BS_Start
		virtual long regTcpClientBind( const CNetworkAddr *pLocalAddr, CTcpConnHandle *pTcpConnHandle);
		virtual long regTcpClientConn( const CNetworkAddr *pPeerAddr, CTcpConnHandle *pTcpConnHandle, 
			const EnumSyncAsync bSyncConn, ULONG ulTimeOut);
		// BS_End
		virtual void removeTcpClient(CTcpConnHandle *pTcpConnHandle);
		virtual long regTcpServer(const CNetworkAddr *pLocalAddr, 
			CTcpServerHandle *pTcpServerHandle);
		virtual void removeTcpServer(CTcpServerHandle *pTcpServerHandle);
		virtual long regUdpSocket(const CNetworkAddr *pLocalAddr, 
									 CUdpSockHandle *pUdpSockHandle,
									 const CNetworkAddr *pMultiAddr);
		virtual void removeUdpSocket(CUdpSockHandle *pUdpSockHandle);

	  protected:
		long m_lLocalIpAddr;
		CTcpConnMgr *m_pTcpConnMgr;
		CUdpSockMgr *m_pUdpSockMgr;
		CTcpServerMgr *m_pTcpServerMgr;
    
	#if VOS_APP_OS == VOS_OS_WIN32
		VOS_BOOLEAN     m_bWSAStartup;
	#endif


		static CConnMgr * g_pManager;

	};
}//end namespace

#endif //CCONNMGR_H_INCLUDE

