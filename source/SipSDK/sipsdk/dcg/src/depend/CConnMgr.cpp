/******************************************************************************
   版权所有 (C), 2001-2011, 华为技术有限公司

 ******************************************************************************
  文件名          : CConnMgr.cpp
  版本号          : 1.0
  作者            : duijiying 24362
  生成日期        : 2007-4-10
  最近修改        : 
  功能描述        : 连接管理模块
  函数列表        : 
  修改历史        : 
  1 日期          : 
    作者          : 
    修改内容      : 
*******************************************************************************/

#ifndef WIN32
#include <sys/epoll.h>
#include "CConnMgr.h"
#endif

#ifdef WIN32
#include <ws2tcpip.h>
#include "CConnMgr.h"
#endif

#include "vos.h"
#include "vos_common.h"
#include "ace/OS_NS_arpa_inet.h"
#include "Log.h"

#define CONN_SECOND_IN_MS   1000
#define CONN_MS_IN_US       1000

#ifndef TCP_NODELAY
#define TCP_NODELAY 0x0001
#endif

#pragma warning(push)
#pragma warning(disable:4996)

//ling -e1732
//ling -e1733
//ling -e438

namespace SipStack
{
	/*******************************************************************************
	Function:       CNetworkAddr::CNetworkAddr()
	Description:    构造函数
	Calls:            
	Called By:      
	Input:          无
	Output:         无 
	Return:         无
	*******************************************************************************/
	CNetworkAddr::CNetworkAddr()
	{
		m_lIpAddr = InvalidIp;
		m_usPort = Invalidport;
	}

	/*******************************************************************************
	  Function:       CNetworkAddr::~CNetworkAddr()
	  Description:    析构函数
	  Calls:            
	  Called By:      
	  Input:          无
	  Output:         无 
	  Return:         无
	*******************************************************************************/
	CNetworkAddr::~CNetworkAddr()
	{
	}

	/*******************************************************************************
	  Function:       CHandle::CHandle()
	  Description:    构造函数
	  Calls:            
	  Called By:      
	  Input:          无
	  Output:         无 
	  Return:         无
	*******************************************************************************/
	CHandle::CHandle()
	{
		m_lSockFD = InvalidSocket;
		m_pHandleNode = NULL;
		m_ulEvents = EPOLLIN;
	#if VOS_APP_OS == VOS_OS_LINUX
		m_lEpfd = InvalidFd;
	#endif //#if

	#if VOS_APP_OS == VOS_OS_WIN32
		m_bReadSelected = VOS_FALSE;
		m_bWriteSelected = VOS_FALSE;
	#endif  //#if

		m_pMutexHandle = VOS_CreateMutex();
		printf("-------------m_pMutexHandle = %p -----------\n", m_pMutexHandle );
		if (NULL == m_pMutexHandle)
		{
			try{
				m_pMutexHandle = new VOS_Mutex;
			}
			catch(...)
			{
				m_pMutexHandle = NULL;
			}

			if (NULL == m_pMutexHandle)
			{
				printf("-------------m_pMutexHandle new fail , errno:%d -----------\n", errno);
			}
			else
			{
				printf("-------------m_pMutexHandle new sucess , errno:%d -----------\n", errno);
	#if VOS_APP_OS == VOS_OS_LINUX
				uint32_t ulResult = (ULONG)pthread_mutex_init( &m_pMutexHandle->mutex, 0);
				if (0 != ulResult)
				{
					printf("-------------pthread_mutex_init != 0, errno:%d -----------\n", errno);
				}
	#endif
			}
		}

	}

	/*******************************************************************************
	  Function:       CHandle::~CHandle()
	  Description:    析构函数
	  Calls:            
	  Called By:      
	  Input:          无
	  Output:         无 
	  Return:         无
	*******************************************************************************/
	CHandle::~CHandle()
	{
		if(NULL != m_pHandleNode)
		{
			delete(m_pHandleNode);
			m_pHandleNode = NULL;
		}

		if(NULL != m_pMutexHandle)
		{
			(void)VOS_DestroyMutex(m_pMutexHandle);
			m_pMutexHandle = NULL;
		}
	}

	/*******************************************************************************
	  Function:       CHandle::initHandle()
	  Description:    初始化函数
	  Calls:            
	  Called By:      
	  Input:          无
	  Output:         无 
	  Return:  
	  VOS_SUCCESS: init success
	  VOS_FAIL: init fail
	*******************************************************************************/
	long CHandle::initHandle(void)
	{
		//防止重用handle时未关闭连接
		if(NULL != m_pMutexHandle)
		{
			this->close();
		}
    
		m_lSockFD = InvalidSocket;
		m_pHandleNode = NULL;
	#if VOS_APP_OS == VOS_OS_LINUX
		m_lEpfd = InvalidFd;
	#endif
		m_ulEvents = EPOLLIN;
		if(NULL == m_pMutexHandle)
		{
			m_pMutexHandle = VOS_CreateMutex();
		}

		if(NULL == m_pMutexHandle)
		{
			WARN_LOG("initHandle fail, pMutexHandle is NULL");
			return VOS_FAIL;
		}
    
		return VOS_SUCCESS;
	}

	/*******************************************************************************
	  Function:       CHandle::setHandleSend()
	  Description:    设置是否检测写事件
	  Calls:            
	  Called By:      
	  Input:          bHandleSend: VOS_TRUE表示检测，VOS_FALSE表示不检测
	  Output:         无 
	  Return:         无 
	*******************************************************************************/
	void CHandle::setHandleSend(VOS_BOOLEAN bHandleSend)
	{
		if(m_pMutexHandle != NULL)
		{
			if(VOS_OK != VOS_MutexLock(m_pMutexHandle))
			{
				return;
			}
		}

		//设置要处理的事件类型 
		if(VOS_FALSE == bHandleSend)
		{
			m_ulEvents = m_ulEvents & (~EPOLLOUT);
		}
		else
		{
			m_ulEvents = m_ulEvents | EPOLLOUT;
		}

		if((m_pHandleNode != NULL) && (m_lSockFD != InvalidSocket))
		{
	#if VOS_APP_OS == VOS_OS_LINUX
			//将handle添加到epoll的集合中
			struct epoll_event epEvent; 
			memset(&epEvent, 0, sizeof(epEvent));
			//设置与要处理的事件相关的文件描述符 
			epEvent.data.ptr = (void *)m_pHandleNode; 
			//设置要处理的事件类型 
			epEvent.events = m_ulEvents;
			//修改注册的epoll事件 
			if ( 0 != epoll_ctl(m_lEpfd, EPOLL_CTL_MOD, m_lSockFD, &epEvent))
			{
				WARN_LOG("CHandle::setHandleSend: modify event fail, m_lSockFD = %d",m_lSockFD);
			}
	#endif
		}

		if(m_pMutexHandle != NULL)
		{
			(void)VOS_MutexUnlock(m_pMutexHandle);
		}
	}

	/*******************************************************************************
	  Function:       CHandle::setHandleRecv()
	  Description:    设置是否检测读事件
	  Calls:            
	  Called By:      
	  Input:          bHandleRecv: VOS_TRUE表示检测，VOS_FALSE表示不检测
	  Output:         无 
	  Return:         
	  VOS_SUCCESS: init success
	  VOS_FAIL: init fail
	*******************************************************************************/
	void CHandle::setHandleRecv(VOS_BOOLEAN bHandleRecv)
	{
		if(m_pMutexHandle != NULL)
		{
			if(VOS_OK != VOS_MutexLock(m_pMutexHandle))
			{
				return;
			}
		}

		//设置要处理的事件类型 
		if(VOS_FALSE == bHandleRecv)
		{
			m_ulEvents = m_ulEvents & (~EPOLLIN);
		}
		else
		{
			m_ulEvents = m_ulEvents | EPOLLIN;
		}

		if((m_pHandleNode != NULL) && (m_lSockFD != InvalidSocket))
		{
	#if VOS_APP_OS == VOS_OS_LINUX
			//将handle添加到epoll的集合中
			struct epoll_event epEvent; 
			memset(&epEvent, 0, sizeof(epEvent));
			//设置与要处理的事件相关的文件描述符 
			epEvent.data.ptr = (void *)m_pHandleNode; 
			//设置要处理的事件类型 
			epEvent.events = m_ulEvents;
			//修改注册的epoll事件 
			if ( 0 != epoll_ctl(m_lEpfd, EPOLL_CTL_MOD, m_lSockFD, &epEvent))
			{
				WARN_LOG("CHandle::setHandleRecv: modify event fail, m_lSockFD = %d",m_lSockFD);
			}
	#endif
		}
    
		if(m_pMutexHandle != NULL)
		{
			(void)VOS_MutexUnlock(m_pMutexHandle);
		}
	}

	/*******************************************************************************
	  Function:       CHandle::close()
	  Description:    关闭网络连接
	  Calls:            
	  Called By:      
	  Input:          无
	  Output:         无 
	  Return:         无 
	*******************************************************************************/
	void CHandle::close(void)
	{
		if (InvalidSocket != m_lSockFD)
		{
			(void)CLOSESOCK((SOCKET)m_lSockFD);
			m_lSockFD = InvalidSocket;
		}

		return;
	}

	/*******************************************************************************
	  Function:       CNetworkHandle::CNetworkHandle()
	  Description:    构在函数
	  Calls:            
	  Called By:      
	  Input:          无
	  Output:         无 
	  Return:         无 
	*******************************************************************************/
	CNetworkHandle::CNetworkHandle()
	{
		m_lSockFD = InvalidSocket;
	}

	/*******************************************************************************
	  Function:       CNetworkHandle::initHandle()
	  Description:    初始化网络连接
	  Calls:            
	  Called By:      
	  Input:          无
	  Output:         无 
	  Return:         无 
	*******************************************************************************/
	long CNetworkHandle::initHandle(void)
	{
		if (VOS_SUCCESS != CHandle::initHandle())
		{
			return VOS_FAIL;
		}
		m_lSockFD = InvalidSocket;

		return VOS_SUCCESS;
	}

	#if VOS_APP_OS == VOS_OS_LINUX
	/*******************************************************************************
	  Function:       CNetworkHandle::sendMsg()
	  Description:    发送矢量数据
	  Calls:            
	  Called By:      
	  Input:          矢量数据
	  Output:         无 
	  Return:         参见系统调用sendmsg
	*******************************************************************************/
	long CNetworkHandle::sendMsg(const struct msghdr *pMsg)
	{
		if (InvalidSocket == m_lSockFD)
		{
			return SendRecvError;
		}
    
		return ::sendmsg(m_lSockFD, pMsg, 0);
	}
	#endif

	/*******************************************************************************
	  Function:       CTcpConnHandle::CTcpConnHandle()
	  Description:    构造函数
	  Calls:            
	  Called By:      
	  Input:          无
	  Output:         无 
	  Return:         无 
	*******************************************************************************/
	CTcpConnHandle::CTcpConnHandle()
	{
		m_lConnStatus = enIdle;
	}

	/*******************************************************************************
	  Function:       CTcpConnHandle::~CTcpConnHandle()
	  Description:    析构函数
	  Calls:            
	  Called By:      
	  Input:          无
	  Output:         无 
	  Return:         无 
	*******************************************************************************/
	CTcpConnHandle::~CTcpConnHandle()
	{
		if (InvalidSocket != m_lSockFD)
		{
			WARN_LOG("handle not released, m_lSockFD = %d,peer_ip=%d,peer_port=%d",m_lSockFD,ntohl((ULONG)(m_peerAddr.m_lIpAddr)),ntohs(m_peerAddr.m_usPort));
			(void)CLOSESOCK((SOCKET)m_lSockFD);
			m_lSockFD = InvalidSocket;
		}
	}

	/*******************************************************************************
	  Function:       CTcpConnHandle::initHandle()
	  Description:    初始化函数
	  Calls:            
	  Called By:      
	  Input:          无
	  Output:         无 
	  Return:         
	  VOS_SUCCESS: init success
	  VOS_FAIL: init fail 
	*******************************************************************************/
	long CTcpConnHandle::initHandle(void)
	{
		if (VOS_SUCCESS != CNetworkHandle::initHandle())
		{
			return VOS_FAIL;
		}

		m_lConnStatus = enIdle;
		return VOS_SUCCESS;
	}

	// BS_Start
	long CTcpConnHandle::connBind(const CNetworkAddr *pLocalAddr)
	{
		m_lSockFD = (long)socket(AF_INET, SOCK_STREAM, 0);
		if(m_lSockFD < 0)
		{
			WARN_LOG("opening client socket error = %d",CONN_ERRNO);
			return VOS_FAIL;
		}

		if(((ULONG)(pLocalAddr->m_lIpAddr) != InvalidIp) || ( pLocalAddr->m_usPort != Invalidport)) // 指定IP或Port都OK
		{
			struct sockaddr_in  serverAddr;
			memset((char *)&serverAddr, 0, (long)sizeof(serverAddr));     
			serverAddr.sin_family = AF_INET;
			serverAddr.sin_addr.s_addr = (unsigned long)pLocalAddr->m_lIpAddr;
			serverAddr.sin_port = pLocalAddr->m_usPort;
			errno = 0;
			if (0 > bind ((SOCKET)m_lSockFD, (struct sockaddr *)(void*)&serverAddr, sizeof(serverAddr)))
			{
				WARN_LOG("Can not Bind Data_Sock,ip;port=%d:%d",ntohl((unsigned int)(pLocalAddr->m_lIpAddr)),ntohs(pLocalAddr->m_usPort));
				(void)CLOSESOCK((SOCKET)m_lSockFD);
				m_lSockFD = InvalidFd;
				return VOS_FAIL;
			}
			DEBUG_LOG("CTcpConnHandle::connBind OK, localPort:%d",ntohs(serverAddr.sin_port));
			return VOS_OK;
		}
		else
		{
			ERROR_LOG("using CTcpConnHandle::connBind without setting pLocalAddr"); 
			return VOS_FAIL;
		}
	}

	long CTcpConnHandle::connAfterBind(const CNetworkAddr *pLocalAddr, const CNetworkAddr *pPeerAddr, const EnumSyncAsync bSyncConn, ULONG ulTimeOut)
	{
		m_lConnStatus = enConnFailed;

		//如果是异步连接，设置为非阻塞模式
		errno = 0;
		if((enAsyncOp == bSyncConn) || (ulTimeOut > 0))
		{
			/* BEGIN: Modified by xuhongyun, 2010/8/3   问题单号:圈复杂度修改*/
			setSockBlock(false);
			/* END:   Modified by xuhongyun, 2010/8/3 */
			setHandleSend(VOS_TRUE);
		}

		//连接对端
		struct sockaddr_in  peerAddr;
		memset((char *)&peerAddr, 0, (long)sizeof(peerAddr));     
		peerAddr.sin_family = AF_INET;
		peerAddr.sin_addr.s_addr = (UINT)pPeerAddr->m_lIpAddr;
		peerAddr.sin_port = pPeerAddr->m_usPort;
		long lRetVal = ::connect((SOCKET)m_lSockFD,(struct sockaddr*)(void*)&peerAddr, sizeof(peerAddr));
		if( lRetVal < 0)
		{
			if (VOS_OK != handleConnFail(bSyncConn, ulTimeOut))
			{
				return VOS_FAIL;
			}
		}
		else
		{
			DEBUG_LOG("connect server OK. socket id = %d",m_lSockFD);
		}

		if (VOS_OK != setSockOpt())
		{
			return VOS_FAIL;
		}

		//如果设置为非阻塞模式，恢复为阻塞模式
		if((enAsyncOp == bSyncConn) || (ulTimeOut > 0))
		{
			/* BEGIN: Modified by xuhongyun, 2010/8/3   问题单号:圈复杂度修改*/
			setSockBlock(true);
			/* END:   Modified by xuhongyun, 2010/8/3 */
			if(enAsyncOp == bSyncConn)
			{
				m_lConnStatus = enConnecting;
			}
		}
		else
		{
			m_lConnStatus = enConnected;
		}

		m_peerAddr.m_lIpAddr = pPeerAddr->m_lIpAddr;
		m_peerAddr.m_usPort= pPeerAddr->m_usPort;

		DEBUG_LOG("CTcpConnHandle::conn: connect success, m_lSockFD = %d, peer_ip(0x%x), peer_port(%d)",
			m_lSockFD,
			ntohl((ULONG)(m_peerAddr.m_lIpAddr)), ntohs(m_peerAddr.m_usPort));
    

		return VOS_SUCCESS;
	}
	// BS_End

	/*******************************************************************************
	  Function:       CTcpConnHandle::conn()
	  Description:    创建连接函数
	  Calls:            
	  Called By:      
	  Input:          pLocalAddr: 本地地址，pPeerAddr: 对端地址，
					  bSyncConn: VOS_TRUE表示同步连接，VOS_FALSE表示异步连接
	  Output:         无 
	  Return:         
	  VOS_SUCCESS: connect success
	  VOS_FAIL: connect fail 
	*******************************************************************************/
	long CTcpConnHandle::conn(const CNetworkAddr *pLocalAddr, const CNetworkAddr *pPeerAddr, const EnumSyncAsync bSyncConn, ULONG ulTimeOut)
	{
		m_lConnStatus = enConnFailed;

		m_lSockFD = (long)socket(AF_INET, SOCK_STREAM, 0);
		if(m_lSockFD < 0)
		{
			WARN_LOG("opening client socket error=%d",CONN_ERRNO);
			return VOS_FAIL;
		}

		/* BEGIN: Modified by xuhongyun, 2010/8/3   问题单号:圈复杂度修改*/
		if (VOS_OK != setSockOpt())
		{
			return VOS_FAIL;
		}
		/* END:   Modified by xuhongyun, 2010/8/3 */
		//绑定本地地址
		if(((ULONG)(pLocalAddr->m_lIpAddr) != InvalidIp) && ( pLocalAddr->m_usPort != Invalidport))
		{
			struct sockaddr_in  serverAddr;
			memset((char *)&serverAddr, 0, (long)sizeof(serverAddr));     
			serverAddr.sin_family = AF_INET;
			serverAddr.sin_addr.s_addr = (unsigned long)pLocalAddr->m_lIpAddr;
			serverAddr.sin_port = pLocalAddr->m_usPort;
			errno = 0;
			if (0 > bind ((SOCKET)m_lSockFD, (struct sockaddr *)(void*)&serverAddr, sizeof(serverAddr)))
			{
				WARN_LOG("Can not Bind Data_Sock,ip;port=%d:%d",ntohl((unsigned int)(pLocalAddr->m_lIpAddr)),ntohs(pLocalAddr->m_usPort));
				(void)CLOSESOCK((SOCKET)m_lSockFD);
				m_lSockFD = InvalidFd;
				return VOS_FAIL;
			}
		}

		//如果是异步连接，设置为非阻塞模式
		errno = 0;
		if((enAsyncOp == bSyncConn) || (ulTimeOut > 0))
		{
			/* BEGIN: Modified by xuhongyun, 2010/8/3   问题单号:圈复杂度修改*/
			setSockBlock(false);
			/* END:   Modified by xuhongyun, 2010/8/3 */
			setHandleSend(VOS_TRUE);
		}

		//连接对端
		struct sockaddr_in  peerAddr;
		memset((char *)&peerAddr, 0, (long)sizeof(peerAddr));
		peerAddr.sin_family = AF_INET;
		peerAddr.sin_addr.s_addr = (UINT)pPeerAddr->m_lIpAddr;
		peerAddr.sin_port = pPeerAddr->m_usPort;
		long lRetVal = ::connect((SOCKET)m_lSockFD,(struct sockaddr*)(void*)&peerAddr, sizeof(peerAddr));
		if( lRetVal < 0)
		{
			if (VOS_OK != handleConnFail(bSyncConn, ulTimeOut))
			{
				return VOS_FAIL;
			}
		}
		else
		{
			DEBUG_LOG("connect server OK. socket id = %d",m_lSockFD);
		}

		//如果设置为非阻塞模式，恢复为阻塞模式
		if((enAsyncOp == bSyncConn) || (ulTimeOut > 0))
		{
			/* BEGIN: Modified by xuhongyun, 2010/8/3   问题单号:圈复杂度修改*/
			setSockBlock(true);
			/* END:   Modified by xuhongyun, 2010/8/3 */
			if(enAsyncOp == bSyncConn)
			{
				m_lConnStatus = enConnecting;
			}
		}
		else
		{
			m_lConnStatus = enConnected;
		}
    
		m_peerAddr.m_lIpAddr = pPeerAddr->m_lIpAddr;
		m_peerAddr.m_usPort= pPeerAddr->m_usPort;

		DEBUG_LOG("CTcpConnHandle::conn: connect success, m_lSockFD = %d, peer_ip(0x%x), peer_port(%d)",m_lSockFD,ntohl((ULONG)(m_peerAddr.m_lIpAddr)), ntohs(m_peerAddr.m_usPort));

		//连接成功后，我们用一个bool标志去标示 2011-04-20
		return VOS_SUCCESS;
	}

	/*******************************************************************************
	  Function:       CTcpConnHandle::send()
	  Description:    发送函数
	  Calls:            
	  Called By:      
	  Input:          pArrayData: 数据buffer，ulDataSize: 数据长度，
					  bSyncSend: VOS_TRUE表示同步发送，VOS_FALSE表示异步发送
	  Output:         无 
	  Return:         
	  lBytesSent: 发送字节数(>0)
	  SendRecvError: 发送失败
	*******************************************************************************/
	long CTcpConnHandle::send(const char *pArrayData, const ULONG ulDataSize, const EnumSyncAsync bSyncSend)
	{
		if (InvalidSocket == m_lSockFD)
		{
			//delete start by limi 2011-03-16
			/*WARN_LOG((char *)
				"CTcpConnHandle::send: socket is invalid, send fail");*/
			//delete end by limi 2011-03-16
			return SendRecvError;
		}

		errno = 0;
		long lBytesSent = 0;
		if(enSyncOp == bSyncSend)
		{
			//同步发送
	#if VOS_APP_OS == VOS_OS_WIN32
			ULONG ulBlock = VOS_FALSE;
			if (SOCKET_ERROR == ioctlsocket((SOCKET)m_lSockFD,(long)FIONBIO,&ulBlock))
			{
				WARN_LOG("Set Socket Block fail.");
				return SendRecvError;
			}
	#endif
			lBytesSent = ::send((SOCKET)m_lSockFD, pArrayData, (datalen_t)ulDataSize, MSG_NOSIGNAL);
		}
		else
		{
			//异步发送
	#if VOS_APP_OS == VOS_OS_WIN32
			ULONG ulBlock = VOS_TRUE;
			if (SOCKET_ERROR == ioctlsocket((SOCKET)m_lSockFD,(long)FIONBIO,&ulBlock))
			{
				WARN_LOG("Set Socket NoBlock fail.");
				return SendRecvError;
			}
	#endif
			lBytesSent = ::send((SOCKET)m_lSockFD, pArrayData, (datalen_t)ulDataSize, MSG_DONTWAIT|MSG_NOSIGNAL);//lint !e835
			//开始检测是否可以发送数据
			//setHandleSend(VOS_TRUE);
		}

		//如果发送失败，首先判断是否是因为阻塞，是的话返回发送0字节，否则关闭连接
		if (lBytesSent < 0)
		{
			if((EWOULDBLOCK == CONN_ERRNO) || (EAGAIN == CONN_ERRNO))
			{
				return 0;
			}
			(void)CLOSESOCK((SOCKET)m_lSockFD);
			m_lSockFD = InvalidSocket;

			WARN_LOG("Can not Bind Data_Sock,ip;port=%d:%d,Error(%d):%s",ntohl((unsigned int)(m_peerAddr.m_lIpAddr)),ntohs(m_peerAddr.m_usPort),CONN_ERRNO,strerror(CONN_ERRNO));

			return SendRecvError;
		}

		return lBytesSent;
	}

	/*******************************************************************************
	Function:       CTcpConnHandle::send_n()
	Description:    发送全部函数
	Calls:            
	Called By:      
	Input:          pArrayData: 数据buffer，ulDataSize: 数据长度，
	Output:         无 
	Return:         
	lBytesSent: 发送字节数(>0)
	SendRecvError: 发送失败
	*******************************************************************************/
	long CTcpConnHandle::send_n(const char *pArrayData, const ULONG ulDataSize)
	{
		unsigned long lTotalSend = 0;

		while((ulDataSize -lTotalSend)  > 0)//循环发送
		{
			unsigned long lSend = (unsigned long)this->send(pArrayData + lTotalSend,ulDataSize-lTotalSend,enSyncOp);
			//if(lSend < 0)
			//{
			//    return SendRecvError;
			//}

			lTotalSend += lSend;
		}

		return (long)lTotalSend;
	}
	/*******************************************************************************
	  Function:       CTcpConnHandle::recv()
	  Description:    接收函数
	  Calls:            
	  Called By:      
	  Input:          pArrayData: 数据buffer，ulDataSize: 数据长度，
					  bSyncRecv: VOS_TRUE表示同步发送，VOS_FALSE表示异步发送
	  Output:         pArrayData: 数据buffer，pPeerAddr: 对端地址，
	  Return:         
	  lBytesSent: 发送字节数(>0)
	  SendRecvError: 发送失败
	*******************************************************************************/
	long CTcpConnHandle::recv(char *pArrayData, CNetworkAddr *pPeerAddr, 
		const ULONG ulDataSize, const EnumSyncAsync bSyncRecv)
	{
		WARN_LOG("*************Enter recv funciton");
		if (InvalidSocket == m_lSockFD)
		{
			WARN_LOG("CTcpConnHandle::recv: socket is invalid, recv fail");
			return SendRecvError;
		}

		errno = 0;
		long lBytesRecv = 0;
		if(enSyncOp == bSyncRecv)
		{
			//同步接收
	#if VOS_APP_OS == VOS_OS_WIN32
			ULONG ulBlock = VOS_FALSE;
			if (SOCKET_ERROR == ioctlsocket((SOCKET)m_lSockFD,(long)FIONBIO,&ulBlock))
			{
				WARN_LOG("Set Socket Block fail.");
				return SendRecvError;
			}
	#endif
			lBytesRecv = ::recv((SOCKET)m_lSockFD, pArrayData, (datalen_t)ulDataSize, MSG_WAITALL);
		}
		else
		{
			//异步接收
	#if VOS_APP_OS == VOS_OS_WIN32
			ULONG ulBlock = VOS_TRUE;
			if (SOCKET_ERROR == ioctlsocket((SOCKET)m_lSockFD,(long)FIONBIO,&ulBlock))
			{
				WARN_LOG("Set Socket NoBlock fail.");
				return SendRecvError;
			}
	#endif
			lBytesRecv = ::recv((SOCKET)m_lSockFD, pArrayData, (datalen_t)ulDataSize, MSG_DONTWAIT);
		}

		//如果返回0，表示已经断连
		if (0 == lBytesRecv)
		{
			DEBUG_LOG("recv EOF!");
			return SendRecvError;
		}

		//如果小于0，首先判断是否是因为阻塞，是的话返回接收0字节，否则断连处理
		if (lBytesRecv < 0)
		{
			if((EWOULDBLOCK == CONN_ERRNO) || (EAGAIN == CONN_ERRNO))
			{
				return 0;
			}
			//delete start by limi
			/*DEBUG_LOG("recv error. Error(%d): %s",CONN_ERRNO, strerror(CONN_ERRNO));*/
			//delete end by limi
			return SendRecvError;
		}

		if(NULL != pPeerAddr)
		{
			pPeerAddr->m_lIpAddr = m_peerAddr.m_lIpAddr;
			pPeerAddr->m_usPort = m_peerAddr.m_usPort;
		}

		return lBytesRecv;
	}

	/*******************************************************************************
	  Function:       CTcpConnHandle::recvWithTimeout()
	  Description:    接收函数
	  Calls:            
	  Called By:      
	  Input:          pArrayData: 数据buffer，ulDataSize: 数据长度，
					  ulTimeOut: 等待时长, ulSleepTime: 检测间隔(ms)
	  Output:         pArrayData: 数据buffer，pPeerAddr: 对端地址，
	  Return:         
	  lBytesSent: 发送字节数(>0)
	  SendRecvError: 发送失败
	*******************************************************************************/
	long CTcpConnHandle::recvWithTimeout(char *pArrayData, CNetworkAddr *pPeerAddr, 
		const ULONG ulDataSize, const ULONG ulTimeOut, const ULONG ulSleepTime)
	{
		(void)ulSleepTime;//过PC-LINT
		long lRecvBytes = 0;
		ULONG ulTotalRecvBytes = 0;
		ULONG ulWaitTime = ulTimeOut;
		errno = 0;
		//设置socket超时时间
	#if VOS_APP_OS == VOS_OS_WIN32

		if(setsockopt((SOCKET)m_lSockFD, SOL_SOCKET, SO_RCVTIMEO, 
			(char *) &ulWaitTime, sizeof(int)) < 0)
		{
			(void)CLOSESOCK((SOCKET)m_lSockFD);
			WARN_LOG("setsockopt socket SO_RCVTIMEO  error:%d",CONN_ERRNO);
			return SendRecvError;
		}

	#elif VOS_APP_OS == VOS_OS_LINUX

		struct timeval recvWaitTime;
		recvWaitTime.tv_sec = (long)ulWaitTime/CONN_SECOND_IN_MS;
		recvWaitTime.tv_usec = (ulWaitTime%CONN_SECOND_IN_MS)*CONN_MS_IN_US;
		if(setsockopt((SOCKET)m_lSockFD, SOL_SOCKET, SO_RCVTIMEO, 
			(char *) &recvWaitTime, sizeof(recvWaitTime)) < 0)
		{
			(void)CLOSESOCK((SOCKET)m_lSockFD);
			WARN_LOG("setsockopt socket SO_RCVTIMEO  error(%d)\n",CONN_ERRNO);
			return SendRecvError;
		}

	#endif

		//windows这个地方无法全部接收完，改为循环接收
		//lRecvBytes = this->recv(pArrayData, pPeerAddr, ulDataSize, enSyncOp);

		//windows 循环接收 linux此处只经过一次循环
		if(NULL == pArrayData)
		{
			return SendRecvError;
		}

		while(ulTotalRecvBytes<ulDataSize)
		{
			lRecvBytes = this->recv(pArrayData+ulTotalRecvBytes,pPeerAddr, ulDataSize-ulTotalRecvBytes, enSyncOp);
	/***BEGIN  V100R001C01 MODIFY For 自发现问题 Modify 2009-06-22 c60705    **********/
	//        if(lRecvBytes <= 0) //recv函数不可能返回0，由于函数内部已经对0返回值做了处理
			if(lRecvBytes < 0)
	/***END    V100R001C01 MODIFY For 自发现问题 Modify 2009-06-22 c60705    **********/
			{
				break;
			}

			ulTotalRecvBytes += (unsigned long)lRecvBytes;
		}


		if(lRecvBytes < 0)
		{
			DEBUG_LOG( "CTcpConnHandle::recvWithTimeout: socket closed when receive.m_lSockFD = %d, peer_ip(0x%x), peer_port(%d) errno = %d, error: %s", m_lSockFD, 
				ntohl((ULONG)(m_peerAddr.m_lIpAddr)), ntohs(m_peerAddr.m_usPort),
				CONN_ERRNO, strerror(CONN_ERRNO) );
                
			if(CONN_ERR_TIMEO == CONN_ERRNO)
			{
				return SendRecvErrorTIMEO;
			}
			if(CONN_ERR_EBADF == CONN_ERRNO)
			{
				return SendRecvErrorEBADF;
			}
			return SendRecvError;
		}

	/***BEGIN  V100R001C01 DELETE For 自发现问题 Modify 2009-06-22 c60705    **********/
	//    if(0 == lRecvBytes)
	//    {
	//        DEBUG_LOG("FILE(%s)LINE(%d): "
	//            "CTcpConnHandle::recvWithTimeout: recv time out. "
	//            "m_lSockFD = %d, peer_ip(0x%x), peer_port(%d) recv_msg_len(%lu)"
	//            "ulDataSize(%lu) errno = %d, error: %s", _FL_, m_lSockFD, 
	//            ntohl((ULONG)(m_peerAddr.m_lIpAddr)), ntohs(m_peerAddr.m_usPort),
	//            ulTotalRecvBytes, ulDataSize,CONN_ERRNO, strerror(CONN_ERRNO) );   
	//        return SendRecvErrorEOF;                    
	//    }
	/***END    V100R001C01 DELETE For 自发现问题 Modify 2009-06-22 c60705    **********/
    
		if(ulTotalRecvBytes <  ulDataSize)//说明接受超时
		{
			DEBUG_LOG( "CTcpConnHandle::recvWithTimeout: recv time out. m_lSockFD = %d, peer_ip(0x%x), peer_port(%d) recv_msg_len(%lu) ulDataSize(%lu) errno = %d, error: %s", m_lSockFD, 
				ntohl((ULONG)(m_peerAddr.m_lIpAddr)), ntohs(m_peerAddr.m_usPort),
				ulTotalRecvBytes, ulDataSize,CONN_ERRNO, strerror(CONN_ERRNO));
                 
			return SendRecvError;
		}

		//设置socket超时时间为0表示永久等待
	#if VOS_APP_OS == VOS_OS_WIN32

		ulWaitTime = 0;
		if(setsockopt((SOCKET)m_lSockFD, SOL_SOCKET, SO_RCVTIMEO, (char *) &ulWaitTime, sizeof(int)) < 0)
		{
			(void)CLOSESOCK((SOCKET)m_lSockFD);
			WARN_LOG("setsockopt socket SO_RCVTIMEO  error(%d)",CONN_ERRNO);
			return SendRecvError;
		}

	#elif VOS_APP_OS == VOS_OS_LINUX

		//设置socket超时时间为0表示永久等待
		recvWaitTime.tv_sec = 0;
		recvWaitTime.tv_usec = 0;
		if(setsockopt((SOCKET)m_lSockFD, SOL_SOCKET, SO_RCVTIMEO, 
			(char *) &recvWaitTime, sizeof(recvWaitTime)) < 0)
		{
			(void)CLOSESOCK((SOCKET)m_lSockFD);
			WARN_LOG((char *)"setsockopt socket SO_RCVTIMEO  error(%d)\n",CONN_ERRNO);
			return SendRecvError;
		}
	#endif

		return (long)ulTotalRecvBytes;

	}
	/* BEGIN: Added by xuhongyun, 2010/8/3   问题单号:圈复杂度修改*/
	int CTcpConnHandle::setSockOpt()
	{
		//setSendBufSize
		long lSendBufSize = DEFAULT_SENDRECV_SIZE;
		socklen_t lSendBufLength = sizeof(lSendBufSize);
		if(setsockopt((SOCKET)m_lSockFD, SOL_SOCKET, SO_SNDBUF, (char*)&lSendBufSize, 
			lSendBufLength) < 0)
		{
			(void)CLOSESOCK((SOCKET)m_lSockFD);
			WARN_LOG("setSendBufSize client socket error(%d)",CONN_ERRNO);
			return VOS_FAIL;
		}
		//setRecBufSize
		long lRecvBufSize = DEFAULT_SENDRECV_SIZE;
		socklen_t lRecvBufLength = sizeof(lRecvBufSize);
		if(setsockopt((SOCKET)m_lSockFD, SOL_SOCKET, SO_RCVBUF, (char*)&lRecvBufSize,
			lRecvBufLength) < 0)
		{
			(void)CLOSESOCK((SOCKET)m_lSockFD);
			WARN_LOG("setRecvBufSize client socket error(%d)",CONN_ERRNO);
			return VOS_FAIL;
		}

		long flag = 1;
		if(setsockopt((SOCKET)m_lSockFD, IPPROTO_TCP, TCP_NODELAY, (char*)&flag, sizeof(flag)) < 0)
		{
			(void)CLOSESOCK((SOCKET)m_lSockFD);
			WARN_LOG("set TCP_NODELAY client socket error(%d)",CONN_ERRNO);
			return VOS_FAIL;
		}

		//setReuseAddr();
		long lReuseAddrFlag = 1;
		if(setsockopt((SOCKET)m_lSockFD, SOL_SOCKET, SO_REUSEADDR, (char*)&lReuseAddrFlag, sizeof(lReuseAddrFlag)) < 0)
		{
			(void)CLOSESOCK((SOCKET)m_lSockFD);
			WARN_LOG("setsockopt client socket error(%d)",CONN_ERRNO);
			return VOS_FAIL;
		}

		return VOS_OK;
	}

	int CTcpConnHandle::handleConnFail(const EnumSyncAsync bSyncConn, ULONG ulTimeOut)
	{
		if((enSyncOp == bSyncConn) && (0 == ulTimeOut))
		{
			(void)CLOSESOCK((SOCKET)m_lSockFD);
			WARN_LOG("SyncConn server fail. error(%d):%s",CONN_ERRNO,strerror(CONN_ERRNO));
			return VOS_FAIL;
		}

		if((EINPROGRESS != CONN_ERRNO) && (EWOULDBLOCK != CONN_ERRNO))
		{
			(void)CLOSESOCK((SOCKET)m_lSockFD);
			WARN_LOG("AsyncConn server fail. error(%d):%s",CONN_ERRNO,strerror(CONN_ERRNO));
			return VOS_FAIL;
		}

		if(enSyncOp == bSyncConn)
		{
			fd_set    fdWriteReady;
			struct timeval waitTime;
			waitTime.tv_sec = (long)ulTimeOut;
			waitTime.tv_usec = 0;
			FD_ZERO(&fdWriteReady);                   /*lint !e661 !e662*/
			FD_SET((SOCKET)m_lSockFD, &fdWriteReady); /*lint !e573 !e713 !e737 !e661 !e662*/
			long lSelectResult = select(FD_SETSIZE, (fd_set*)0, &fdWriteReady, (fd_set*)0, &waitTime);
			if(lSelectResult <= 0)
			{
				WARN_LOG("wait client socket(%d) time out",m_lSockFD);
				(void)CLOSESOCK((SOCKET)m_lSockFD);
				return VOS_FAIL;
			}
			long lErrorNo = 0;
			socklen_t len = sizeof(lErrorNo);
			if (getsockopt((SOCKET)m_lSockFD, SOL_SOCKET, SO_ERROR, 
				(SOCK_OPT_TYPE *)&lErrorNo, &len) < 0)
			{
				WARN_LOG("getsockopt of sockFD(%d) has wrong when wait client",m_lSockFD);
				(void)CLOSESOCK((SOCKET)m_lSockFD);
				return VOS_FAIL;
			}
			else if (lErrorNo != 0)
			{
				WARN_LOG("wait client: socket(%d) connect fail",m_lSockFD);
				(void)CLOSESOCK((SOCKET)m_lSockFD);
				return VOS_FAIL;
			}

			DEBUG_LOG("connect server OK. socket id = %d",m_lSockFD);
			m_lConnStatus = enConnected;
		}

		return VOS_OK;
	}

	void CTcpConnHandle::setSockBlock(bool bBlocked)
	{
	   if (bBlocked)
	   {
		#if VOS_APP_OS == VOS_OS_LINUX
			//恢复为阻塞模式
			if(fcntl(m_lSockFD, F_SETFL, fcntl(m_lSockFD, F_GETFL&(~O_NONBLOCK))) < 0) 
		#elif VOS_APP_OS == VOS_OS_WIN32
			ULONG ulBlock = 0;
			if (SOCKET_ERROR == ioctlsocket((SOCKET)m_lSockFD,(long)FIONBIO,&ulBlock))
		#endif
			{
				WARN_LOG("fcntl client socket error(%d)",CONN_ERRNO);
				//(void)CLOSESOCK((SOCKET)lSockFd);
				//return VOS_FAIL;
			}
	   }
	   else
	   {
	   #if VOS_APP_OS == VOS_OS_LINUX
			//设置为非阻塞
			if(fcntl(m_lSockFD, F_SETFL, fcntl(m_lSockFD, F_GETFL)|O_NONBLOCK) < 0) 
		#elif VOS_APP_OS == VOS_OS_WIN32
			ULONG ulNoBlock = VOS_TRUE;
			if (SOCKET_ERROR == ioctlsocket((SOCKET)m_lSockFD,(long)(long)FIONBIO,&ulNoBlock))
		#endif
			{
				WARN_LOG("fcntl client socket error(%d)",CONN_ERRNO);
				//(void)CLOSESOCK((SOCKET)lSockFd);
				//return VOS_FAIL;
			}
	   }

	   return;
	}
	/* END:   Added by xuhongyun, 2010/8/3 */
	/*******************************************************************************
	  Function:       CTcpConnHandle::close()
	  Description:    关闭连接
	  Calls:            
	  Called By:      
	  Input:          无
	  Output:         无 
	  Return:         无
	*******************************************************************************/
	void CTcpConnHandle::close(void)
	{
		if(m_pMutexHandle != NULL)
		{
			if(VOS_OK != VOS_MutexLock(m_pMutexHandle))
			{
				return;
			}
		}

		if (InvalidSocket != m_lSockFD)
		{
			DEBUG_LOG( "CTcpConnHandle::close: close connection, m_lSockFD = %d, peer_ip(0x%x), peer_port(%d) this(0x%x) m_pHandleNode(0x%x)",
				m_lSockFD,
				ntohl((ULONG)(m_peerAddr.m_lIpAddr)), ntohs(m_peerAddr.m_usPort), this,
				this->m_pHandleNode);

			//The close of an fd will cause it to be removed from 
			//all epoll sets automatically.   
	#if VOS_APP_OS == VOS_OS_LINUX
			struct epoll_event epEvent; 
			memset(&epEvent, 0, sizeof(epEvent));
			epEvent.data.ptr = (void *)NULL; 
			//设置要处理的事件类型 
			epEvent.events = ((unsigned long)EPOLLIN | (unsigned long)EPOLLOUT);
			//修改注册的epoll事件 
			if ( 0 != epoll_ctl(m_lEpfd, EPOLL_CTL_MOD, m_lSockFD, &epEvent))
			{
				WARN_LOG((char *)"CHandle::setHandleSend: modify event fail, m_lSockFD = %d",  m_lSockFD);
			}
			//设置与要处理的事件相关的文件描述符 
			//epEvent.data.ptr = (void *)m_pHandleNode; 
			//设置要处理的事件类型 
			//epEvent.events = EPOLLIN; 
			if ( 0 != epoll_ctl(m_lEpfd, EPOLL_CTL_DEL, m_lSockFD, &epEvent))
			{
				WARN_LOG((char *)"CTcpConnHandle::close: epoll_ctl EPOLL_CTL_DEL fail, m_lSockFD = %d",m_lSockFD);
			}
	#endif
			(void)CLOSESOCK((SOCKET)m_lSockFD);
			m_lSockFD = InvalidSocket;
		}
		m_lConnStatus = enClosed;

		CHandle::close();

		if(m_pMutexHandle != NULL)
		{
			(void)VOS_MutexUnlock(m_pMutexHandle);
		}

		return;
	}

	/*******************************************************************************
	  Function:       CUdpSockHandle::createSock()
	  Description:    创建UDP socket
	  Calls:            
	  Called By:      
	  Input:          pLocalAddr:本地地址
	  Output:         无 
	  Return:         
	  VOS_SUCCESS: init success
	  VOS_FAIL: init fail
	*******************************************************************************/
	long CUdpSockHandle::createSock(const CNetworkAddr *pLocalAddr, const CNetworkAddr *pMultiAddr)
	{
		long lSockFd = (long)InvalidSocket;
		if ((lSockFd = (long)socket(AF_INET, SOCK_DGRAM, 0)) < 0)//lint !e838
		{
			WARN_LOG("create udp socket failed, errno =%d, Msg = %s",CONN_ERRNO,strerror(CONN_ERRNO));
			return VOS_FAIL;
		}

		sockaddr_in  localAddr;
		memset((char *)&localAddr, 0, (long)sizeof(localAddr));
		localAddr.sin_family = AF_INET;
		if(NULL != pMultiAddr)
		{
			localAddr.sin_addr.s_addr =  INADDR_ANY;
		}
		else
		{
			localAddr.sin_addr.s_addr = (UINT)pLocalAddr->m_lIpAddr;
		}
		localAddr.sin_port = pLocalAddr->m_usPort;

		//绑定本地地址
		if (0 > bind ((SOCKET)lSockFd, (struct sockaddr *)(void*)&localAddr, sizeof (localAddr)))
		{
	#if VOS_APP_OS == VOS_OS_LINUX
			char szLocalAddr[INET_ADDRSTRLEN];
			if (NULL != inet_ntop(AF_INET, &localAddr.sin_addr, szLocalAddr, 
				sizeof(szLocalAddr)))
	#elif VOS_APP_OS == VOS_OS_WIN32
			char *szLocalAddr = inet_ntoa(localAddr.sin_addr);
			if (NULL == szLocalAddr)
	#endif
			{
				WARN_LOG("Can not Bind Data_Sock,szLocalAddr is NULL.");
			}
			else
			{
				const unsigned char* addr_ipv4_p = (unsigned char*)&(localAddr.sin_addr.s_addr);
				WARN_LOG("Can not Bind Socket %u.%u.%u.%u:%d",
					(unsigned int)addr_ipv4_p[0],
					(unsigned int)addr_ipv4_p[1],
					(unsigned int)addr_ipv4_p[2],
					(unsigned int)addr_ipv4_p[3],
					ntohs(localAddr.sin_port));
			}
			(void)CLOSESOCK((SOCKET)lSockFd);
			return VOS_FAIL;
		}

	#if VOS_APP_OS == VOS_OS_LINUX
		long lReuseAddrFlag = 1;
		if(setsockopt((SOCKET)lSockFd, SOL_SOCKET, SO_REUSEADDR, (char*)&lReuseAddrFlag, 
			sizeof(lReuseAddrFlag)) < 0)
		{
			(void)CLOSESOCK((SOCKET)lSockFd); WARN_LOG((char *)"setSendBufSize client socket error(%d)", CONN_ERRNO);
			return VOS_FAIL;
		}
	#endif

		long lSendBufSize = DEFAULT_SENDRECV_SIZE;
		if(setsockopt ((SOCKET)lSockFd, SOL_SOCKET, SO_SNDBUF, (char*)&lSendBufSize,  sizeof(lSendBufSize)) < 0)
		{
			(void)CLOSESOCK((SOCKET)lSockFd);
			WARN_LOG("setSendBufSize client socket error(%d)",CONN_ERRNO);
			return VOS_FAIL;
		}
		//setRecBufSize
		long lRecvBufSize = DEFAULT_SENDRECV_SIZE;
		if(setsockopt((SOCKET)lSockFd, SOL_SOCKET, SO_RCVBUF, (char*)&lRecvBufSize, sizeof(lRecvBufSize)) < 0)
		{
			(void)CLOSESOCK((SOCKET)lSockFd);
			WARN_LOG("setRecvBufSize client socket error(%d)",CONN_ERRNO);
			return VOS_FAIL;
		}
    
	#if VOS_APP_OS == VOS_OS_LINUX
		//如果有组播地址，则表明要发送组播
		if(NULL != pMultiAddr)
		{
			struct ip_mreq mreq;
			mreq.imr_multiaddr.s_addr = (unsigned int)pMultiAddr->m_lIpAddr;
			mreq.imr_interface.s_addr = (unsigned int)pLocalAddr->m_lIpAddr;
			if(setsockopt((SOCKET)lSockFd, IPPROTO_IP, IP_ADD_MEMBERSHIP,
								 (char *)&mreq, sizeof(mreq))< 0)
			{
				(void)CLOSESOCK((SOCKET)lSockFd);
				WARN_LOG((char *)"set IPPROTO_IP IP_ADD_MEMBERSHIP error(%d)", CONN_ERRNO);
				return VOS_FAIL;
			}
		}
	#endif
		m_lSockFD = lSockFd;

		return VOS_SUCCESS;

	}

	/*******************************************************************************
	  Function:       CUdpSockHandle::send()
	  Description:    发送函数
	  Calls:            
	  Called By:      
	  Input:          pPeerAddr: 对端地址，pArrayData: 发送缓冲区，ulDataSize:数据长度
					  bSyncSend: VOS_TRUE表示同步发送，VOS_FALSE表示异步发送
	  Output:         无 
	  Return:         
	  VOS_SUCCESS: connect success
	  VOS_FAIL: connect fail 
	*******************************************************************************/
	long CUdpSockHandle::send(const CNetworkAddr *pPeerAddr, const char *pArrayData,
			 const ULONG ulDataSize, const EnumSyncAsync bSyncSend)    
	{
		if (InvalidSocket == m_lSockFD)
		{
			WARN_LOG("CUdpSockHandle::send: socket is invalid, send fail");
			return SendRecvError;
		}

		struct sockaddr_in peerAddr;

		peerAddr.sin_family = AF_INET;
		peerAddr.sin_addr.s_addr = (UINT)pPeerAddr->m_lIpAddr;
		peerAddr.sin_port = pPeerAddr->m_usPort;

		errno = 0;
		long lBytesSent = 0;
		if(enSyncOp == bSyncSend)
		{
			//同步发送
	#if VOS_APP_OS == VOS_OS_WIN32
			ULONG ulBlock = VOS_FALSE;
			if (SOCKET_ERROR == ioctlsocket((SOCKET)m_lSockFD,(long)FIONBIO,&ulBlock))
			{
				WARN_LOG("Set Socket Block fail.");
				return SendRecvError;
			}
	#endif
			lBytesSent = ::sendto( (SOCKET)m_lSockFD, pArrayData, (datalen_t)ulDataSize, 
								   (long)MSG_NOSIGNAL,
								   (const struct sockaddr *)(void*)&peerAddr, sizeof(peerAddr)
								 );
		}
		else
		{
			//异步发送
	#if VOS_APP_OS == VOS_OS_WIN32
			ULONG ulBlock = VOS_TRUE;
			if (SOCKET_ERROR == ioctlsocket((SOCKET)m_lSockFD,(long)FIONBIO,&ulBlock))
			{
				WARN_LOG("Set Socket NoBlock fail.");
				return SendRecvError;
			}
	#endif
			lBytesSent = ::sendto( (SOCKET)m_lSockFD, pArrayData, (datalen_t)ulDataSize,(long)(MSG_DONTWAIT|MSG_NOSIGNAL),(const struct sockaddr *)(void*)&peerAddr, sizeof(peerAddr));//lint !e835
			//setHandleSend(VOS_TRUE);
		}
    
		//如果发送失败，首先判断是否是因为阻塞，是的话返回发送0字节，否则关闭连接
		if (lBytesSent < 0)
		{
			if((EWOULDBLOCK == CONN_ERRNO) || (EAGAIN == CONN_ERRNO))
			{
				return 0;
			}
			WARN_LOG("CUdpSockHandle::send (%d)bytes to peer(IP:0x%x, Port:%d) Error(%d): %s", 
				ulDataSize, ntohl((ULONG)(pPeerAddr->m_lIpAddr)),
				ntohs(pPeerAddr->m_usPort),
				CONN_ERRNO, strerror(CONN_ERRNO));

			return SendRecvError;
		}

		return lBytesSent;
	}

	/*******************************************************************************
	  Function:       CUdpSockHandle::recv()
	  Description:    接收函数
	  Calls:            
	  Called By:      
	  Input:          pArrayData: 数据buffer，ulDataSize: 数据长度，
					  bSyncRecv: VOS_TRUE表示同步发送，VOS_FALSE表示异步发送
	  Output:         pArrayData: 数据buffer，pPeerAddr: 对端地址，
	  Return:         
	  lBytesSent: 发送字节数(>0)
	  SendRecvError: 发送失败
	*******************************************************************************/
	long CUdpSockHandle::recv(char *pArrayData, CNetworkAddr *pPeerAddr, 
		const ULONG ulDataSize, const EnumSyncAsync bSyncRecv)
	{
		if (InvalidSocket == m_lSockFD)
		{
			WARN_LOG("CUdpSockHandle::recv: socket is invalid, recv fail");
			return SendRecvError;
		}

		errno = 0; 
		struct sockaddr_in  peerAddr;
		socklen_t iFromlen = sizeof(peerAddr);
		long lBytesRecv = 0;
		if(enSyncOp == bSyncRecv)
		{
			//同步接收
	#if VOS_APP_OS == VOS_OS_WIN32
			ULONG ulBlock = VOS_FALSE;
			if (SOCKET_ERROR == ioctlsocket((SOCKET)m_lSockFD,(long)FIONBIO,&ulBlock))
			{
				WARN_LOG("Set Socket Block fail.");
				return SendRecvError;
			}
	#endif
			lBytesRecv = recvfrom( (SOCKET)m_lSockFD, pArrayData, (datalen_t)ulDataSize, 
								   MSG_WAITALL, (struct sockaddr *)(void*)&peerAddr, &iFromlen
								 );
		}
		else
		{
			//异步接收
	#if VOS_APP_OS == VOS_OS_WIN32
			ULONG ulBlock = VOS_TRUE;
			if (SOCKET_ERROR == ioctlsocket((SOCKET)m_lSockFD,(long)FIONBIO,&ulBlock))
			{
				WARN_LOG("Set Socket NoBlock fail.");
				return SendRecvError;
			}
	#endif
			lBytesRecv = recvfrom( (SOCKET)m_lSockFD, pArrayData, (datalen_t)ulDataSize, 0, 
								   (struct sockaddr *)(void*)&peerAddr, &iFromlen
								 );
		}

		//如果返回0，表示已经断连
		if (0 == lBytesRecv)
		{
			DEBUG_LOG("recv EOF!");
			return SendRecvError;
		}

		//如果小于0，首先判断是否是因为阻塞，是的话返回接收0字节，否则断连处理
		if (lBytesRecv < 0)
		{
			if((EWOULDBLOCK == CONN_ERRNO) || (EAGAIN == CONN_ERRNO))
			{
				return 0;
			}
			//delete start by limi 2011-03-16
			/*DEBUG_LOG("recv error. Error(%d): %s", CONN_ERRNO, strerror(CONN_ERRNO));*/
			//delete end by limi 2011-03-16
			return SendRecvError;
		}

		pPeerAddr->m_lIpAddr = (LONG)peerAddr.sin_addr.s_addr;
		pPeerAddr->m_usPort = peerAddr.sin_port;

		//连接管理调用接收操作后不再恢复检测读事件，改为由应用程序恢复
		//setHandleRecv(VOS_TRUE);
    
		return lBytesRecv;
	}

	/*******************************************************************************
	  Function:       CUdpSockHandle::recvWithTimeout()
	  Description:    接收函数
	  Calls:            
	  Called By:      
	  Input:          pArrayData: 数据buffer，ulDataSize: 数据长度，
					  ulTimeOut: 等待时长, ulSleepTime: 检测间隔(ms)
	  Output:         pArrayData: 数据buffer，pPeerAddr: 对端地址，
	  Return:         
	  lBytesSent: 发送字节数(>0)
	  SendRecvError: 发送失败
	*******************************************************************************/
	long CUdpSockHandle::recvWithTimeout(char *pArrayData, CNetworkAddr *pPeerAddr, 
		const ULONG ulDataSize, const ULONG ulTimeOut, const ULONG ulSleepTime)
	{
		(void)ulSleepTime;//过PC-LINT
		long lRecvBytes = 0;
		ULONG ulTotalRecvBytes = 0;
		ULONG ulWaitTime = ulTimeOut;
		errno = 0;
		//设置socket超时时间
	#if VOS_APP_OS == VOS_OS_WIN32

		ULONG recvWaitTime;
		recvWaitTime = ulWaitTime;
		if(setsockopt((SOCKET)m_lSockFD, SOL_SOCKET, SO_RCVTIMEO, 
					  (char *) &recvWaitTime, sizeof(recvWaitTime)
					 ) < 0
		  )
		{
			(void)CLOSESOCK((SOCKET)m_lSockFD);
			WARN_LOG("setsockopt socket SO_RCVTIMEO  error(%d)",CONN_ERRNO);
			return SendRecvError;
		}

	#elif VOS_APP_OS == VOS_OS_LINUX

		struct timeval recvWaitTime;
		recvWaitTime.tv_sec = (long)ulWaitTime/CONN_SECOND_IN_MS;
		recvWaitTime.tv_usec = (ulWaitTime%CONN_SECOND_IN_MS)*CONN_MS_IN_US;             
		if(setsockopt((SOCKET)m_lSockFD, SOL_SOCKET, SO_RCVTIMEO, 
					  (char *) &recvWaitTime, sizeof(recvWaitTime)
					 ) < 0
		  )
		{
			(void)CLOSESOCK((SOCKET)m_lSockFD);
			WARN_LOG((char *)"setsockopt socket SO_RCVTIMEO  error(%d)\n", CONN_ERRNO);
			return SendRecvError;
		}

	#endif

		lRecvBytes = this->recv(pArrayData, pPeerAddr, ulDataSize, enSyncOp);//lint !e838
		if(lRecvBytes < 0)
		{
			DEBUG_LOG( "CUdpSockHandle::recvWithTimeout: socket closed when receive. m_lSockFD = %d, peer_ip(0x%x), peer_port(%d) errno = %d, error: %s",  m_lSockFD, 
				ntohl((ULONG)(pPeerAddr->m_lIpAddr)), ntohs(pPeerAddr->m_usPort),
				CONN_ERRNO, strerror(CONN_ERRNO) );
                 
			return SendRecvError;
		}

		ulTotalRecvBytes += (ULONG)lRecvBytes;

		//设置socket超时时间为0表示永久等待
	#if VOS_APP_OS == VOS_OS_WIN32

		recvWaitTime = 0;
		if(setsockopt((SOCKET)m_lSockFD, SOL_SOCKET, SO_RCVTIMEO, 
					  (char *) &recvWaitTime, sizeof(recvWaitTime)
					 ) < 0
		  )
		{
			(void)CLOSESOCK((SOCKET)m_lSockFD);
			WARN_LOG("setsockopt socket SO_RCVTIMEO  error(%d)",CONN_ERRNO);
			return SendRecvError;
		}

	#elif VOS_APP_OS == VOS_OS_LINUX

		//设置socket超时时间为0表示永久等待
		recvWaitTime.tv_sec = 0;
		recvWaitTime.tv_usec = 0;
		if(setsockopt((SOCKET)m_lSockFD, SOL_SOCKET, SO_RCVTIMEO, 
					  (char *) &recvWaitTime, sizeof(recvWaitTime)
					 ) < 0
		  )
		{
			(void)CLOSESOCK((SOCKET)m_lSockFD);
			WARN_LOG((char *)"setsockopt socket SO_RCVTIMEO  error(%d)\n", CONN_ERRNO);
			return SendRecvError;
		}
	#endif

		return (long)ulTotalRecvBytes;
	}

	/*******************************************************************************
	  Function:       CUdpSockHandle::close()
	  Description:    关闭连接
	  Calls:            
	  Called By:      
	  Input:          无
	  Output:         无 
	  Return:         无
	*******************************************************************************/
	void CUdpSockHandle::close(void)
	{
		if(m_pMutexHandle != NULL)
		{
			if(VOS_OK != VOS_MutexLock(m_pMutexHandle))
			{
				return;
			}
		}
		if (InvalidSocket != m_lSockFD)
		{
			//The close of an fd will cause it to be removed from 
			//all epoll sets automatically.   
			(void)CLOSESOCK((SOCKET)m_lSockFD);
			m_lSockFD = InvalidSocket;
		}

		CHandle::close();
    
		if(m_pMutexHandle != NULL)
		{
			(void)VOS_MutexUnlock(m_pMutexHandle);
		}
    
		return;
	}

	/*******************************************************************************
	  Function:       CTcpServerHandle::listen()
	  Description:    启动等待对端连接
	  Calls:            
	  Called By:      
	  Input:          pLocalAddr: 本地地址
	  Output:         无 
	  Return:         
	  VOS_SUCCESS: listen success
	  VOS_FAIL: listen fail 
	*******************************************************************************/
	long CTcpServerHandle::listen(const CNetworkAddr *pLocalAddr)
	{
		long lSockFd = (long)socket(AF_INET, SOCK_STREAM, 0);
		if(lSockFd < 0)
		{
			WARN_LOG("opening client socket error(%d)",CONN_ERRNO);
			return VOS_FAIL;
		}

		//setSendBufSize
		long lSendBufSize = DEFAULT_SENDRECV_SIZE;
		socklen_t lSendBufLength = sizeof(lSendBufSize);
		if(setsockopt((SOCKET)lSockFd, SOL_SOCKET, SO_SNDBUF, (char*)&lSendBufSize, lSendBufLength) < 0)
		{
			(void)CLOSESOCK((SOCKET)lSockFd);
			WARN_LOG("setSendBufSize client socket error(%d)",CONN_ERRNO);
			return VOS_FAIL;
		}

		//setRecBufSize
		long lRecvBufSize = DEFAULT_SENDRECV_SIZE;
		socklen_t lRecvBufLength = sizeof(lRecvBufSize);
		if(setsockopt((SOCKET)lSockFd, SOL_SOCKET, SO_RCVBUF, (char*)&lRecvBufSize, lRecvBufLength) < 0)
		{
			(void)CLOSESOCK((SOCKET)lSockFd);
			WARN_LOG("setRecvBufSize client socket error(%d)",CONN_ERRNO);
			return VOS_FAIL;
		}

		//setReuseAddr();
		long lReuseAddrFlag = 1;
		if(setsockopt((SOCKET)lSockFd, SOL_SOCKET, SO_REUSEADDR, (char*)&lReuseAddrFlag, sizeof(lReuseAddrFlag)) < 0)
		{
			(void)CLOSESOCK((SOCKET)lSockFd);
			WARN_LOG( "setsockopt client socket error(%d)",CONN_ERRNO);
			return VOS_FAIL;
		}


		//绑定本地地址
		struct sockaddr_in  serverAddr;
		memset((char *)&serverAddr, 0, (long)sizeof(serverAddr));     
		serverAddr.sin_family = AF_INET;
		serverAddr.sin_addr.s_addr = (UINT)pLocalAddr->m_lIpAddr;
		serverAddr.sin_port = pLocalAddr->m_usPort;
		errno = 0;
		if (0 > bind ((SOCKET)lSockFd, (struct sockaddr *)(void*)&serverAddr, sizeof(serverAddr)))
		{
	#if VOS_APP_OS == VOS_OS_LINUX
			char szServerAddr[INET_ADDRSTRLEN];
			if (NULL != inet_ntop(AF_INET, &serverAddr.sin_addr, szServerAddr, sizeof(szServerAddr)))
	#elif VOS_APP_OS == VOS_OS_WIN32
			char *szServerAddr = inet_ntoa(serverAddr.sin_addr);
			if (NULL != szServerAddr)
	#endif
			{
				WARN_LOG("Can not Bind Data_Sock %s:%d",szServerAddr,ntohs(serverAddr.sin_port));
			}
			else
			{
				WARN_LOG("Can not Bind Data_Sock %d:%d", serverAddr.sin_addr.s_addr,ntohs(serverAddr.sin_port));
			}
			(void)CLOSESOCK((SOCKET)lSockFd);
			return VOS_FAIL;
		}

		//启动侦听
		errno = 0;
		if(::listen((SOCKET)lSockFd, MAX_LISTEN_QUEUE_SIZE) < 0)
		{
			WARN_LOG("listen Error(%d):%s",CONN_ERRNO,strerror(CONN_ERRNO));
			(void)CLOSESOCK((SOCKET)lSockFd);
			return VOS_FAIL;
		}

		m_lSockFD = lSockFd;

		return VOS_SUCCESS;
	}

	/*******************************************************************************
	  Function:       CTcpServerHandle::close()
	  Description:    关闭连接
	  Calls:          
	  Called By:      
	  Input:          无
	  Output:         无
	  Return:         无
	*******************************************************************************/
	void CTcpServerHandle::close(void)
	{
		if(m_pMutexHandle != NULL)
		{
			if(VOS_OK != VOS_MutexLock(m_pMutexHandle))
			{
				return;
			}
		}
		if (InvalidSocket != m_lSockFD)
		{
			//The close of an fd will cause it to be removed from 
			//all epoll sets automatically.   
			(void)CLOSESOCK((SOCKET)m_lSockFD);
			m_lSockFD = InvalidSocket;
		}

		CHandle::close();

		if(m_pMutexHandle != NULL)
		{
			(void)VOS_MutexUnlock(m_pMutexHandle);
		}

		return;
	}

	// BEGIN  V100R002C01 ADD  2010-12-06 w68813 连接后处理,默认为空
	void CTcpServerHandle::after_accept
	(
		const CNetworkAddr* pRemoteAddr,
		CTcpConnHandle*  pTcpConnHandle
	)
	{
		pRemoteAddr = pRemoteAddr;
		pTcpConnHandle = pTcpConnHandle;
	}//lint !e438
	// END    V100R002C01 ADD  2010-12-06 w68813

	/*******************************************************************************
	  Function:       CHandleManager::CHandleManager()
	  Description:    构造函数
	  Calls:            
	  Called By:      
	  Input:          无
	  Output:         无 
	  Return:         无
	*******************************************************************************/
	CHandleManager::CHandleManager()
	{
		m_pMutexListOfHandle = NULL;
	#if VOS_APP_OS == VOS_OS_LINUX
		m_lEpfd = InvalidFd;
		memset(m_epEvents, 0, sizeof(m_epEvents));
	#elif VOS_APP_OS == VOS_OS_WIN32
		//初始化select集
		FD_ZERO(&m_readSet);
		FD_SET(0,&m_readSet);
		FD_ZERO(&m_writeSet);
		FD_SET(0,&m_writeSet);
		m_stSelectPeriod.tv_sec = 0;
		m_stSelectPeriod.tv_usec = 0;
	#endif
		m_ulSelectPeriod = DEFAULT_SELECT_PERIOD;
		m_pVosThread = NULL;
		m_bExit = VOS_FALSE;
		m_pMutexExit = NULL;
		memset(m_szMgrType, 0, sizeof(m_szMgrType));
	}

	/*******************************************************************************
	  Function:       CHandleManager::~CHandleManager()
	  Description:    析构函数
	  Calls:            
	  Called By:      
	  Input:          无
	  Output:         无 
	  Return:         无
	*******************************************************************************/
	CHandleManager::~CHandleManager()
	{
	#if VOS_APP_OS == VOS_OS_LINUX
		INFO_LOG("manager type: %s. thread = 0X%x,m_lEpfd = %d",m_szMgrType,VOS_pthread_self(),m_lEpfd);
	#elif VOS_APP_OS == VOS_OS_WIN32
		INFO_LOG("manager type: %s. thread = 0X%x",m_szMgrType,VOS_pthread_self());
	#endif

		ListOfHandleIte itListOfHandle = m_listHandle.begin();
		while(itListOfHandle != m_listHandle.end())
		{
			if((*itListOfHandle)->m_pHandle != NULL)
			{
				(*itListOfHandle)->m_pHandle->close();
			}
	#if VOS_APP_OS == VOS_OS_LINUX

	#endif
			//将对应的HandleNode删除
			delete(*itListOfHandle);
			++itListOfHandle;
		}

	#if VOS_APP_OS == VOS_OS_LINUX
		if (m_lEpfd != InvalidFd)
		{
			(void)CLOSESOCK(m_lEpfd);
			m_lEpfd = InvalidFd;
		}
	#endif

		if(m_pVosThread != NULL)
		{
			VOS_free(m_pVosThread);
		}
		m_pVosThread = NULL;

		if(m_pMutexListOfHandle != NULL)
		{
			if(VOS_OK == VOS_DestroyMutex(m_pMutexListOfHandle))
			{
				m_pMutexListOfHandle = NULL;
			}
		}

		m_pMutexListOfHandle = NULL;

		if(m_pMutexExit != NULL)
		{
			if(VOS_OK == VOS_DestroyMutex(m_pMutexExit))
			{
				m_pMutexExit = NULL;
			}
		}
	}

	/*******************************************************************************
	  Function:       CHandleManager::init()
	  Description:    初始化函数
	  Calls:            
	  Called By:      
	  Input:          ulSelectPeriod: 事件检测间隔，单位为ms
	  Output:         无 
	  Return:         
	  VOS_SUCCESS: init success
	  VOS_FAIL: init fail 
	*******************************************************************************/
	long CHandleManager::init(const ULONG ulSelectPeriod)
	{
		if (0 == ulSelectPeriod)
		{
			m_ulSelectPeriod = DEFAULT_SELECT_PERIOD;
		}
		else
		{
			m_ulSelectPeriod = ulSelectPeriod;
		}

	#if VOS_APP_OS == VOS_OS_LINUX
		m_lEpfd = epoll_create(MAX_EPOLL_FD_SIZE);

		if(m_lEpfd < 0)
		{
			m_lEpfd = InvalidFd;
			WARN_LOG((char *)"CHandleManager::init: create file handle for epoll fail. manager type: %s", m_szMgrType);
			return VOS_FAIL;
		}
	#elif VOS_APP_OS == VOS_OS_WIN32
		//将ulSelectPeriod转换成timeval结构
		m_stSelectPeriod.tv_sec = (long)(ulSelectPeriod / CONN_SECOND_IN_MS);
		m_stSelectPeriod.tv_usec = (ulSelectPeriod % CONN_SECOND_IN_MS) * CONN_MS_IN_US;

		//初始化select集
		FD_ZERO(&m_readSet);
		FD_ZERO(&m_writeSet);
	#endif

		m_pMutexListOfHandle = VOS_CreateMutex();
		if(NULL == m_pMutexListOfHandle)
		{

	#if VOS_APP_OS == VOS_OS_LINUX
			close(m_lEpfd);
			m_lEpfd = InvalidFd;
	#endif
			WARN_LOG("CHandleManager::init: create m_pMutexListOfHandle fail. manager type: %s" ,m_szMgrType);
			return VOS_FAIL;
		}
				
		m_pMutexExit = VOS_CreateMutex();
		if(NULL == m_pMutexExit)
		{
			WARN_LOG("CHandleManager::init: create m_pMutexExit fail. manager type: %s" ,m_szMgrType);
			return VOS_FAIL;
		}

		return VOS_SUCCESS;
	}

	/*******************************************************************************
	  Function:       CHandleManager::run()
	  Description:    创建线程，启动事件检测主循环
	  Calls:            
	  Called By:      
	  Input:          无 
	  Output:         无 
	  Return:         
	  VOS_SUCCESS: init success
	  VOS_FAIL: init fail 
	*******************************************************************************/
	long CHandleManager::run()
	{
		errno = 0;
		if (VOS_OK != VOS_CreateThread((VOS_THREAD_FUNC)invoke, (void *)this, 
			&m_pVosThread, VOS_DEFAULT_STACK_SIZE))
		{
			WARN_LOG("Create play thread failed. manager type: %s,error(%d):%s",m_szMgrType,CONN_ERRNO,strerror(CONN_ERRNO));
			return VOS_FAIL;
		}
		INFO_LOG("VOS_CreateThread: manager type: %s. create thread :0X%x",m_szMgrType,m_pVosThread->pthead);
		return VOS_SUCCESS;
	}

	/*******************************************************************************
	  Function:       CHandleManager::invoke()
	  Description:    创建线程，启动事件检测主循环
	  Calls:            
	  Called By:      
	  Input:          argc: 保存对象实例指针
	  Output:         无 
	  Return:         
	  VOS_SUCCESS: init success
	  VOS_FAIL: init fail 
	*******************************************************************************/
	void *CHandleManager::invoke(void *argc)
	{
		CHandleManager *pHandleManager = (CHandleManager *)argc;
		INFO_LOG("%s invoke mainLoop",pHandleManager->m_szMgrType);
		pHandleManager->mainLoop();
		VOS_pthread_exit(NULL);

		return NULL;
	}
	#if VOS_APP_OS == VOS_OS_WIN32
	void CHandleManager::setSelectFd()
	{
		//加锁
		if(VOS_OK != VOS_MutexLock(m_pMutexListOfHandle))
		{
			return;
		}

		FD_ZERO(&m_readSet);
		FD_ZERO(&m_writeSet);

		//将ulSelectPeriod转换成timeval结构
		m_stSelectPeriod.tv_sec = (long)(m_ulSelectPeriod / CONN_SECOND_IN_MS);
		m_stSelectPeriod.tv_usec = (m_ulSelectPeriod % CONN_SECOND_IN_MS) * CONN_MS_IN_US; 

		ListOfHandleIte itListOfHandle = m_listHandle.begin();
		while(itListOfHandle != m_listHandle.end())
		{
			CHandleNode *pHandleNode = NULL;
			CHandle *pHandle = NULL;
			long lSockFd = InvalidSocket;

			if(VOS_TRUE == (*itListOfHandle)->m_bRemoved)
			{
				itListOfHandle = m_listHandle.erase(itListOfHandle);
				delete(pHandleNode);
				continue;
			}
			else
			{
				pHandleNode = *itListOfHandle;
				pHandle = pHandleNode->m_pHandle;
				lSockFd = pHandle->m_lSockFD;
				pHandle->m_bReadSelected = VOS_FALSE;
				pHandle->m_bWriteSelected = VOS_FALSE;
				if(lSockFd != InvalidSocket)
				{
					ULONG ulEvent = pHandle->getEvents();
					if (EPOLLIN == (ulEvent & EPOLLIN))
					{
						if(!FD_ISSET(lSockFd,&m_readSet))
						{
							FD_SET((SOCKET)lSockFd,&m_readSet);
							pHandle->m_bReadSelected = VOS_TRUE;
						}
					}

					if (EPOLLOUT == (ulEvent & EPOLLOUT))
					{
						if(!FD_ISSET(lSockFd,&m_writeSet))
						{
							FD_SET((SOCKET)lSockFd,&m_writeSet);
							pHandle->m_bWriteSelected = VOS_TRUE;
						}
					}
				}

			}
			++itListOfHandle;
		}
		//解锁
		(void)VOS_MutexUnlock(m_pMutexListOfHandle);

		return;
	}

	void CHandleManager::checkSelectEvent()
	{
		INFO_TRACE("");
		//加锁
		if(VOS_OK != VOS_MutexLock(m_pMutexListOfHandle))
		{
		   return;
		}

		CHandleNode *pHandleNode = NULL;
		for(ListOfHandleIte it = m_listHandle.begin();it != m_listHandle.end(); ++it)
		{
			pHandleNode = *it;
			if (VOS_TRUE == pHandleNode->m_bRemoved)
			{
				continue;
			}

			//检查是否为读操作
			CHandle *pHandle = pHandleNode->m_pHandle;
			if (pHandle->m_lSockFD != InvalidSocket)
			{
				if (FD_ISSET(pHandle->m_lSockFD,&m_readSet) && (VOS_TRUE == pHandle->m_bReadSelected))
				{
					this->checkSelectResult(enEpollRead, pHandleNode->m_pHandle);
				}

				//检查是否为写操作
				if (FD_ISSET(pHandle->m_lSockFD,&m_writeSet) && (VOS_TRUE == pHandle->m_bWriteSelected))
				{
					this->checkSelectResult(enEpollWrite, pHandleNode->m_pHandle);
				}
			}
		}
		//解锁
		(void)VOS_MutexUnlock(m_pMutexListOfHandle);

		return;
	}
	/*******************************************************************************
	  Function:       CHandleManager::mainLoop()
	  Description:    事件检测主循环
	  Calls:            
	  Called By:      
	  Input:          无 
	  Output:         无 
	  Return:         无 
	*******************************************************************************/
	void CHandleManager::mainLoop()
	{
		while(1)
		{
			VOS_MutexLock(m_pMutexExit);
			if(VOS_TRUE == m_bExit)
			{
				INFO_LOG("Exit mainLoop");
				VOS_MutexUnlock(m_pMutexExit);
				break;
			}
			VOS_MutexUnlock(m_pMutexExit);
			errno = 0;
			long lWaitFds = 0;

			/* BEGIN: Modified by xuhongyun, 2010/8/3   问题单号:圈复杂度修改*/
			setSelectFd();
			/* END:   Modified by xuhongyun, 2010/8/3 */

			//还没有要检测的socket
			if ((0 == m_readSet.fd_count) && (0 == m_writeSet.fd_count))
			{
				Sleep(m_ulSelectPeriod);
				continue;
			}
			else
			{
				if (0 == m_readSet.fd_count)
				{
					lWaitFds = select(0,NULL,&m_writeSet,NULL,&m_stSelectPeriod);
				}
				else
				{
					if (0 == m_writeSet.fd_count)
					{
						lWaitFds = select(0,&m_readSet,NULL,NULL,&m_stSelectPeriod);
					}
					else
					{
						lWaitFds = select(0,&m_readSet,&m_writeSet,NULL,&m_stSelectPeriod);
					}
				}
			}

			if (0 == lWaitFds)
			{
				continue;
			}
			if (SOCKET_ERROR == lWaitFds)
			{
				DEBUG_LOG("select failed: manager type: %s. errno = %d",m_szMgrType, CONN_ERRNO); 
				//如果在select操作之前FD_SET集合中的socket被close，
				//select操作会报WSAENOTSOCK（10038）错误, break会导致线程退出
				//break;
				continue;
			}

			/* BEGIN: Modified by xuhongyun, 2010/8/3   问题单号:圈复杂度修改*/
			checkSelectEvent();
			/* END:   Modified by xuhongyun, 2010/8/3 */
		}
 
		return;
	}
	#endif

	#if VOS_APP_OS == VOS_OS_LINUX
	/*******************************************************************************
	  Function:       CHandleManager::mainLoop()
	  Description:    事件检测主循环
	  Calls:            
	  Called By:      
	  Input:          无 
	  Output:         无 
	  Return:         无 
	*******************************************************************************/
	void CHandleManager::mainLoop()
	{
		while(VOS_FALSE == m_bExit)
		{
			errno = 0;
			long lWaitFds = epoll_wait(m_lEpfd, m_epEvents, EPOLL_MAX_EVENT, (long)m_ulSelectPeriod);
			if (0 == lWaitFds )
			{
				continue;
			}

			if (0 > lWaitFds )
			{
				if(EINTR == CONN_ERRNO)
				{
					continue;
				}

				WARN_LOG((char *)"epoll_wait failed: manager type: %s. errno(%d):%s", 
							m_szMgrType, CONN_ERRNO, strerror(CONN_ERRNO)
						 );
				break;
			}

			//加锁
			if(VOS_OK != VOS_MutexLock(m_pMutexListOfHandle))
			{
			   break;
			}

			CHandleNode *pHandleNode = NULL;
			for(long i = 0; i < lWaitFds; ++i) 
			{ 
				pHandleNode = (CHandleNode *)(m_epEvents[i].data.ptr);
				if(NULL == pHandleNode)
				{
					WARN_LOG( (char *)"pHandleNode is NULL, sequence = %d", i);
					continue;
				}
				//正常情况下m_bRemoved的值只有2种，为了防止内存被改写成其它值，
				//增加条件:(VOS_FALSE != pHandleNode->m_bRemoved)
				if((VOS_TRUE == pHandleNode->m_bRemoved) || (VOS_FALSE != pHandleNode->m_bRemoved))
				{
					continue;
				}
				CHandle *pHandle = pHandleNode->m_pHandle;
				if(NULL == pHandle)
				{
					WARN_LOG( (char *)"pHandle is NULL, sequence = %d",  i);
					continue;
				}

				//通过事件类型检查是否为读操作
				if(m_epEvents[i].events & EPOLLIN) 
				{
					this->checkSelectResult(enEpollRead, pHandle);
				}

				//通过事件类型检查是否为写操作
				if(m_epEvents[i].events & EPOLLOUT) 
				{
					this->checkSelectResult(enEpollWrite, pHandle); 
				}
			}

			ListOfHandleIte itListOfHandle = m_listHandle.begin();
			while(itListOfHandle != m_listHandle.end())
			{
				if(VOS_TRUE == (*itListOfHandle)->m_bRemoved)
				{
					DEBUG_LOG( "(*itListOfHandle) removed, pHandleNode = 0X%x",(long)(*itListOfHandle));

					//将对应的HandleNode删除
					pHandleNode = *itListOfHandle;
					itListOfHandle = m_listHandle.erase(itListOfHandle);
					delete(pHandleNode);
					pHandleNode = NULL;
					continue;
				}
				++itListOfHandle;
			}

			//解锁
			(void)VOS_MutexUnlock(m_pMutexListOfHandle);
		}
		return;
	}
	#endif

	/*******************************************************************************
	  Function:       CHandleManager::exit()
	  Description:    发送退出通知给事件检测主循环，退出线程
	  Calls:            
	  Called By:      
	  Input:          无 
	  Output:         无 
	  Return:         无 
	*******************************************************************************/
	void CHandleManager::exit()
	{
		if(NULL == m_pVosThread)
		{
			WARN_LOG("handler manager - exit - invalid param");
			return;
		}

		VOS_MutexLock(m_pMutexExit);
		this->m_bExit = VOS_TRUE;		
		VOS_MutexUnlock(m_pMutexExit);
		INFO_LOG("Ready to Exit Recv Thread.");
		errno = 0;
		long ret_val = VOS_ThreadJoin(m_pVosThread);
		if (ret_val != VOS_OK)
		{
			WARN_LOG("handler manager - exit - failure to join - Result_Value=%d, Error_Number=%d, Error=%s",ret_val,CONN_ERRNO,strerror(CONN_ERRNO)); 
		}

		INFO_LOG("handler manager - exit - success - Manage=%s",m_szMgrType);

		return;
	}

	/*******************************************************************************
	  Function:       CHandleManager::addHandle()
	  Description:    注册需要检测事件的handle
	  Calls:            
	  Called By:      
	  Input:          pHandle: 需要检测事件的handle
	  Output:         无 
	  Return:         无 
	*******************************************************************************/
	long CHandleManager::addHandle(CHandle *pHandle, 
									  VOS_BOOLEAN bIsListOfHandleLocked)
	{
		if (NULL == pHandle )
		{
			WARN_LOG( "CHandleManager::addHandle: pHandle is NULL");
			return VOS_FAIL;
		}
		if(InvalidSocket == pHandle->m_lSockFD)
		{
			WARN_LOG( "CHandleManager::addHandle: pHandle's socket is invalid");
			return VOS_FAIL;
		}
		CHandleNode *pHandleNode = NULL;
		if (NULL == VOS_NEW(pHandleNode))
		{
			WARN_LOG("CHandleManager::addHandle: new pHandleNode fail");
			return VOS_FAIL;
		}

		/* BEGIN: Modified by xuhongyun, 2010/8/3   问题单号:圈复杂度修改*/
		// 尝试先取锁
		VOS_BOOLEAN bLocked = getMutex(bIsListOfHandleLocked);
		/* END:   Modified by xuhongyun, 2010/8/3 */
    
	#if VOS_APP_OS == VOS_OS_LINUX
		//将handle添加到epoll的集合中
		struct epoll_event epEvent; 
		memset(&epEvent, 0, sizeof(epEvent));
		//设置与要处理的事件相关的文件描述符 
		epEvent.data.ptr = (void *)pHandleNode; 
		//设置要处理的事件类型 
		epEvent.events = pHandle->getEvents(); 
		//注册epoll事件 
		errno = 0;
		if ( 0 != epoll_ctl(m_lEpfd, EPOLL_CTL_ADD, pHandle->m_lSockFD, &epEvent))
		{
			WARN_LOG((char *)"CHandleManager::addHandle: add event fail, errno = %d, error: %s", CONN_ERRNO, strerror(CONN_ERRNO));
			delete(pHandleNode);
			pHandleNode = NULL;

			//解锁
			if(VOS_TRUE == bLocked)
			{
				if (VOS_OK != VOS_MutexUnlock(m_pMutexListOfHandle))
				{
					WARN_LOG( (char *)"CHandleManager::addHandle: release lock failed");
				}
			}

			return VOS_FAIL;
		}
	#endif
		pHandle->m_pHandleNode = pHandleNode;

	#if VOS_APP_OS == VOS_OS_LINUX
		pHandle->m_lEpfd = m_lEpfd;
	#endif
		pHandleNode->m_pHandle = pHandle;
		char szlog[1024] = {0};
	#if VOS_APP_OS == VOS_OS_LINUX
		DEBUG_LOG( "CHandleManager::addHandle: new pHandleNode(0x%x) m_pHandle(0x%x) fd(%d) Epfd(%d) peer_ip(0x%x) peer_port(%d)",
			pHandleNode, pHandleNode->m_pHandle, 
			pHandleNode->m_pHandle->m_lSockFD, pHandleNode->m_pHandle->m_lEpfd,
			pHandleNode->m_pHandle->m_localAddr.m_lIpAddr,
			ntohs(pHandleNode->m_pHandle->m_localAddr.m_usPort));
	#elif VOS_APP_OS == VOS_OS_WIN32
		DEBUG_LOG( "CHandleManager::addHandle: new pHandleNode(0x%x) m_pHandle(0x%x) fd(%d) peer_ip(0x%x) peer_port(%d)", 
			pHandleNode, pHandleNode->m_pHandle, 
			pHandleNode->m_pHandle->m_lSockFD, 
			pHandleNode->m_pHandle->m_localAddr.m_lIpAddr,
			ntohs(pHandleNode->m_pHandle->m_localAddr.m_usPort));

	#endif
		m_listHandle.push_back(pHandleNode);

		//解锁
		if(VOS_TRUE == bLocked)
		{
			/* BEGIN: Modified by xuhongyun, 2010/8/3   问题单号:圈复杂度修改*/
			releaseMutex();
			/* END:   Added by xuhongyun, 2010/8/3 */
		}

		return VOS_SUCCESS;
	}

	/* BEGIN: Added by xuhongyun, 2010/8/3   问题单号:圈复杂度修改*/
	VOS_BOOLEAN CHandleManager::getMutex(VOS_BOOLEAN bIsListOfHandleLocked)
	{
		//加锁(如果和mainloop不是同一线程不需要加锁)
		VOS_BOOLEAN bNeedLock = VOS_FALSE;
		VOS_BOOLEAN bLocked = VOS_FALSE;
		if(VOS_FALSE == bIsListOfHandleLocked)//没有加过锁需要加锁
		{
			if (NULL == m_pVosThread)
			{
				bNeedLock = VOS_TRUE;
			}
			else
			{
				if(VOS_pthread_self() != m_pVosThread->pthead)
				{
					bNeedLock = VOS_TRUE;
				}
			}
        
			if(VOS_TRUE == bNeedLock)
			{
				if (VOS_OK != VOS_MutexLock(m_pMutexListOfHandle))
				{
					WARN_LOG("CHandleManager::removeHandle: get lock failed");
				}
				else
				{
				   bLocked = VOS_TRUE;
				}
			}
		}

		return bLocked;
	}

	void CHandleManager::releaseMutex()
	{
		if (VOS_OK != VOS_MutexUnlock(m_pMutexListOfHandle))
		{
			WARN_LOG("CHandleManager::addHandle: release lock failed");
		}

		return;
	}

	/* END:   Added by xuhongyun, 2010/8/3 */
	/*******************************************************************************
	  Function:       CHandleManager::removeHandle()
	  Description:    取消注册需要检测事件的handle
	  Calls:            
	  Called By:      
	  Input:          pHandle: 需要检测事件的handle
	  Output:         无 
	  Return:         无 
	*******************************************************************************/
	void CHandleManager::removeHandle(CHandle *pHandle)
	{
		if(NULL == pHandle)
		{
			WARN_LOG("CHandleManager::removeHandle: pHandle is NULL");
			return;
		}

		//加锁(如果和mainloop不是同一线程不需要加锁)
		VOS_BOOLEAN bNeedLock = VOS_FALSE;
		VOS_BOOLEAN bLocked = VOS_FALSE;
		if (NULL == m_pVosThread)
		{
			bNeedLock = VOS_TRUE;
		}
		else
		{
			if(VOS_pthread_self() != m_pVosThread->pthead)
			{
				bNeedLock = VOS_TRUE;
			}
		}

		if(VOS_TRUE == bNeedLock)
		{
			if (VOS_OK != VOS_MutexLock(m_pMutexListOfHandle))
			{
				WARN_LOG( "CHandleManager::removeHandle: get lock failed");
			}
			else
			{
				bLocked = VOS_TRUE;
			}
		}

		if(NULL == pHandle->m_pHandleNode)
		{
			WARN_LOG( "pHandle->m_pHandleNode is NULL");
		}
		else
		{
			if(pHandle->m_pHandleNode->m_bRemoved != VOS_TRUE)
			{
				pHandle->close();
				pHandle->m_pHandleNode->m_bRemoved = VOS_TRUE;
				pHandle->m_pHandleNode->m_pHandle = NULL;
			}
			else
			{
				WARN_LOG("pHandle removed more than once");
			}

			pHandle->m_pHandleNode = NULL;
		}

		//解锁(如果不是同一线程)
		if(VOS_TRUE == bLocked)
		{
			if (VOS_OK != VOS_MutexUnlock(m_pMutexListOfHandle))
			{
				WARN_LOG("CHandleManager::removeHandle: release lock failed");
			}
		}

		return;
	}

	/*******************************************************************************
	  Function:       CTcpConnMgr::lockListOfHandle()
	  Description:    对List Handle 加锁
	  Calls:            
	  Called By:      
	  Input:          NA
	  Output:         无 
	  Return:         无 
	*******************************************************************************/
	void CTcpConnMgr::lockListOfHandle()
	{
		if (VOS_OK != VOS_MutexLock(m_pMutexListOfHandle))
		{
			WARN_LOG("CTcpConnMgr::lockListOfHandle: get lock failed");
		}
	}
	/*******************************************************************************
	  Function:       CTcpConnMgr::unlockListOfHandle()
	  Description:    对List Handle 解锁
	  Calls:            
	  Called By:      
	  Input:          NA
	  Output:         无 
	  Return:         无 
	*******************************************************************************/
	void CTcpConnMgr::unlockListOfHandle()
	{
		if (VOS_OK != VOS_MutexUnlock(m_pMutexListOfHandle))
		{
			WARN_LOG("CTcpConnMgr::unlockListOfHandle: release lock failed");
		}
	}

	/* BEGIN: Added by xuhongyun, 2010/8/3   问题单号:圈复杂度修改*/
	void CTcpConnMgr::checkConnectStatus(CTcpConnHandle *pTcpConnHandle) const
	{
		long lErrorNo = 0;
		socklen_t len = sizeof(lErrorNo);
		if (getsockopt((SOCKET)(pTcpConnHandle->m_lSockFD), SOL_SOCKET, SO_ERROR, (SOCK_OPT_TYPE *)&lErrorNo, &len) < 0)
		{
			WARN_LOG("getsockopt of sockfd has wrong");
			pTcpConnHandle->close();
			pTcpConnHandle->m_lConnStatus = enConnFailed;
		}
		else if (lErrorNo != 0)
		{
			WARN_LOG("getsockopt says sockfd is wrong");
			pTcpConnHandle->close();
			pTcpConnHandle->m_lConnStatus = enConnFailed;
		}
		else
		{
			pTcpConnHandle->m_lConnStatus = enConnected;
		}

		return;
	}
	/* END:   Added by xuhongyun, 2010/8/3 */
	/*******************************************************************************
	  Function:       CTcpConnMgr::checkSelectResult()
	  Description:    根据得到的事件调用相应的tcp handle处理事件
	  Calls:            
	  Called By:      
	  Input:          enEpEvent:检测到的事件，pHandle: 需要检测事件的handle
	  Output:         无 
	  Return:         无 
	*******************************************************************************/
	void CTcpConnMgr::checkSelectResult(const EpollEventType enEpEvent,
		CHandle *pHandle)
	{
		if(NULL == pHandle)
		{
			WARN_LOG("CHandleManager::checkSelectResult: pHandle is NULL");
			return;
		}

		CTcpConnHandle *pTcpConnHandle = dynamic_cast<CTcpConnHandle *>(pHandle);
		if(NULL == pTcpConnHandle)
		{
			return;
		}

		//处理读事件
		if(enEpollRead == enEpEvent)
		{
			//清除读事件检测
			pTcpConnHandle->setHandleRecv(VOS_FALSE);
			//调用handle处理接收事件
			pTcpConnHandle->handle_recv();
		}

		//处理写事件
		if(enEpollWrite == enEpEvent)
		{
			//清除写事件检测
			pTcpConnHandle->setHandleSend(VOS_FALSE);

			//检测是否连接成功
			if(pTcpConnHandle->getStatus() == enConnecting)
			{
				/* BEGIN: Modified by xuhongyun, 2010/8/3   问题单号:圈复杂度修改*/
				checkConnectStatus(pTcpConnHandle);
				/* END:   Added by xuhongyun, 2010/8/3 */
			}

			//调用handle处理写事件
			pTcpConnHandle->handle_send();
		}

	}

	/*******************************************************************************
	  Function:       CUdpSockMgr::checkSelectResult()
	  Description:    根据得到的事件调用相应的udp handle处理事件
	  Calls:            
	  Called By:      
	  Input:          enEpEvent:检测到的事件，pHandle: 需要检测事件的handle
	  Output:         无 
	  Return:         无 
	*******************************************************************************/
	void CUdpSockMgr::checkSelectResult(const EpollEventType enEpEvent,
		CHandle *pHandle)
	{
		INFO_TRACE("");
		if(NULL == pHandle)
		{
			WARN_LOG("CHandleManager::checkSelectResult: pHandle is NULL");
			return;
		}

		CUdpSockHandle *pUdpSockHandle = dynamic_cast<CUdpSockHandle *>(pHandle);
		if(NULL == pUdpSockHandle)
		{
			return;
		}

		//处理读事件
		if(enEpollRead == enEpEvent)
		{
			//清除读事件检测
			pUdpSockHandle->setHandleRecv(VOS_FALSE);
			//调用handle处理接收事件
			pUdpSockHandle->handle_recv();
		}

		//处理写事件
		if(enEpollWrite == enEpEvent)
		{
			//清除写事件检测
			pUdpSockHandle->setHandleSend(VOS_FALSE);
			//调用handle处理写事件
			pUdpSockHandle->handle_send();
		}
	}

	/*******************************************************************************
	  Function:       CTcpServerMgr::checkSelectResult()
	  Description:    根据得到的事件调用相应的handle处理事件
	  Calls:            
	  Called By:      
	  Input:          enEpEvent:检测到的事件，pHandle: 需要检测事件的handle
	  Output:         无 
	  Return:         无 
	*******************************************************************************/
	void CTcpServerMgr::checkSelectResult(const EpollEventType enEpEvent,
		CHandle *pHandle)
	{
		if(NULL == pHandle)
		{
			WARN_LOG("CHandleManager::checkSelectResult: pHandle is NULL");
			return;
		}

		if(NULL == m_pTcpConnMgr)
		{
			WARN_LOG("CTcpServerMgr::checkSelectResult: m_pTcpConnMgr is NULL.");
			return;
		}

		CTcpServerHandle *pTcpServerHandle = dynamic_cast<CTcpServerHandle *>(pHandle);
		if(NULL == pTcpServerHandle)
		{
			return;
		}

		//处理连接到来事件
		if(enEpollRead == enEpEvent)
		{
			struct sockaddr_in peerAddr;
			memset(&peerAddr, 0, sizeof(struct sockaddr_in));

			//接受连接
			socklen_t len = sizeof(struct sockaddr_in);
			long lClientSockfd = InvalidFd;
			errno = 0;
			lClientSockfd = (long)::accept( (SOCKET)(pTcpServerHandle->m_lSockFD),
											(struct sockaddr *)(void*)&peerAddr, &len
										  );//lint !e838
			if( 0 > lClientSockfd)
			{
				if((EWOULDBLOCK != CONN_ERRNO) && (CONN_ERRNO != EAGAIN))
				{
					WARN_LOG("accept Error(%d):%s",CONN_ERRNO,strerror(CONN_ERRNO));
				}
				return;
			}
			/* BEGIN: Deleted by xuhongyun, 2010/8/3   问题单号:圈复杂度修改*/
			// 设置 TCP连接属性操作被删除，后面直接调用CTcpConnHandle接口实现
			/* END: Deleted by xuhongyun, 2010/8/3 */
        
			//调用server handle处理连接到来
			CNetworkAddr clientAddr;
			clientAddr.m_lIpAddr = (LONG)peerAddr.sin_addr.s_addr;
			clientAddr.m_usPort = peerAddr.sin_port;
			CTcpConnHandle *pTcpConnHandle = NULL;
        
			/*此处加锁,使得新生成的pTcpConnHandle,与removeTcpClient互斥*/
			m_pTcpConnMgr->lockListOfHandle();
			if (VOS_SUCCESS != pTcpServerHandle->handle_accept(&clientAddr, pTcpConnHandle))
			{
				(void)CLOSESOCK((SOCKET)lClientSockfd);
				WARN_LOG("CTcpServerMgr::checkSelectResult: accept fail.");
				m_pTcpConnMgr->unlockListOfHandle();
				return;
			}
        
			if (NULL == pTcpConnHandle)
			{
				(void)CLOSESOCK((SOCKET)lClientSockfd);
				WARN_LOG("CTcpServerMgr::checkSelectResult: return NULL arg.");
				m_pTcpConnMgr->unlockListOfHandle();
				return;
			}
			if(VOS_SUCCESS != pTcpConnHandle->initHandle())
			{
				(void)CLOSESOCK((SOCKET)lClientSockfd);
				WARN_LOG( "CTcpServerMgr::checkSelectResult: pTcpConnHandle init fail");
				m_pTcpConnMgr->unlockListOfHandle();
				return;
			}
			pTcpConnHandle->m_lSockFD = lClientSockfd;
			pTcpConnHandle->m_localAddr.m_lIpAddr = pTcpServerHandle->m_localAddr.m_lIpAddr;
			pTcpConnHandle->m_localAddr.m_usPort = pTcpServerHandle->m_localAddr.m_usPort;
			pTcpConnHandle->m_lConnStatus = enConnected;
			pTcpConnHandle->m_peerAddr.m_lIpAddr = clientAddr.m_lIpAddr;
			pTcpConnHandle->m_peerAddr.m_usPort = clientAddr.m_usPort;

			/* BEGIN: Added by xuhongyun, 2010/8/3   问题单号:圈复杂度修改*/
			if (VOS_OK != pTcpConnHandle->setSockOpt())
			{
				pTcpConnHandle->close();
				m_pTcpConnMgr->unlockListOfHandle();
				return;
			}
			/* END:   Added by xuhongyun, 2010/8/3 */
        
			VOS_BOOLEAN bIsListOfHandleLocked = VOS_TRUE;
			if (VOS_SUCCESS != m_pTcpConnMgr->addHandle(pTcpConnHandle, 
														bIsListOfHandleLocked))
			{
				WARN_LOG("CTcpServerMgr::checkSelectResult: addHandle fail.");
				pTcpConnHandle->close();
			}
			m_pTcpConnMgr->unlockListOfHandle();
			DEBUG_LOG("CTcpServerMgr::checkSelectResult: accept connect, m_lSockFD = %d, peer_ip(0x%x), peer_port(%d)", 
				pTcpConnHandle->m_lSockFD, 
				ntohl((ULONG)(pTcpConnHandle->m_peerAddr.m_lIpAddr)), 
				ntohs(pTcpConnHandle->m_peerAddr.m_usPort));
                 

			// BEGIN  V100R002C01 ADD  2010-12-06 w68813 增加连接后的处理
			pTcpServerHandle->after_accept(&clientAddr, pTcpConnHandle);
			// END    V100R002C01 ADD  2010-12-06 w68813

		}

		//不应该检测到写事件
		if(enEpollWrite == enEpEvent)
		{
			WARN_LOG( "CTcpServerMgr should not process write event");
		}
	}


	CConnMgr * CConnMgr::g_pManager = NULL;

	/*******************************************************************************
	  Function:       CConnMgr::CConnMgr()
	  Description:    构造函数
	  Calls:            
	  Called By:      
	  Input:          无
	  Output:         无 
	  Return:         无
	*******************************************************************************/
	CConnMgr::CConnMgr()
	{
		m_lLocalIpAddr = InvalidIp;
		m_pTcpConnMgr = NULL;
		m_pUdpSockMgr = NULL;
		m_pTcpServerMgr = NULL;
    
	#if VOS_APP_OS == VOS_OS_WIN32
		m_bWSAStartup = VOS_FALSE;
	#endif
	}

	/*******************************************************************************
	  Function:       CConnMgr::~CConnMgr()
	  Description:    析构函数
	  Calls:            
	  Called By:      
	  Input:          无
	  Output:         无 
	  Return:         无
	*******************************************************************************/
	CConnMgr::~CConnMgr()
	{
		delete(m_pTcpConnMgr);
		delete(m_pUdpSockMgr);
		delete(m_pTcpServerMgr);
	}

	/*******************************************************************************
	  Function:       CConnMgr::init()
	  Description:    初始化函数
	  Calls:            
	  Called By:      
	  Input:          无
	  Output:         无 
	  Return:         
	  VOS_SUCCESS: init success
	  VOS_FAIL: init fail 
	*******************************************************************************/
	long CConnMgr::init(const ULONG ulSelectPeriod, const VOS_BOOLEAN bHasUdpSock,
				const VOS_BOOLEAN bHasTcpClient, const VOS_BOOLEAN bHasTcpServer)  
	{
	#if VOS_APP_OS == VOS_OS_WIN32
		WSAData wsaData;
		if (SOCKET_ERROR == WSAStartup(MAKEWORD(2,2),&wsaData))
		{
			WARN_LOG("WSAStartup error.");
			return VOS_ERR_SYS;
		}
		m_bWSAStartup = VOS_TRUE;
	#endif
		if(VOS_TRUE == bHasUdpSock)
		{
			if(NULL == VOS_NEW(m_pUdpSockMgr))
			{
				WARN_LOG("CConnMgr::init: create m_pUdpSockMgr fail");
				return VOS_FAIL;
			}
			if(VOS_SUCCESS != m_pUdpSockMgr->init(ulSelectPeriod))
			{
				WARN_LOG("CConnMgr::init: init m_pUdpSockMgr fail");
				return VOS_FAIL;
			}
		}

		if((VOS_TRUE == bHasTcpClient) || (VOS_TRUE == bHasTcpServer))
		{
			if(NULL == VOS_NEW(m_pTcpConnMgr))
			{
				WARN_LOG("CConnMgr::init: create m_pTcpConnMgr fail");
				return VOS_FAIL;
			}
			if(VOS_SUCCESS != m_pTcpConnMgr->init(ulSelectPeriod))
			{
				WARN_LOG("CConnMgr::init: init m_pTcpConnMgr fail");
				return VOS_FAIL;
			}
		}

		if(VOS_TRUE == bHasTcpServer)
		{
			if(NULL == VOS_NEW(m_pTcpServerMgr))
			{
				WARN_LOG( "CConnMgr::init: create m_pTcpServerMgr fail");
				return VOS_FAIL;
			}
			if(VOS_SUCCESS != m_pTcpServerMgr->init(ulSelectPeriod))
			{
				WARN_LOG( "CConnMgr::init: init m_pTcpServerMgr fail");
				return VOS_FAIL;
			}
			m_pTcpServerMgr->setTcpClientMgr(m_pTcpConnMgr);
		}    

		return VOS_SUCCESS;
	}

	/*******************************************************************************
	  Function:       CConnMgr::run()
	  Description:    启动各个manager
	  Calls:            
	  Called By:      
	  Input:          无 
	  Output:         无 
	  Return:         
	  VOS_SUCCESS: start success
	  VOS_FAIL: start fail 
	*******************************************************************************/
	long CConnMgr::run(void)
	{
		if(NULL != m_pUdpSockMgr)
		{
			if(VOS_SUCCESS != m_pUdpSockMgr->run())
			{
				WARN_LOG("CConnMgr::run: run m_pUdpSockMgr fail");
				return VOS_FAIL;
			}
		}

		if(NULL != m_pTcpConnMgr)
		{
			if(VOS_SUCCESS != m_pTcpConnMgr->run())
			{
				WARN_LOG("CConnMgr::run: run m_pTcpConnMgr fail");
				return VOS_FAIL;
			}
		}

		if(NULL != m_pTcpServerMgr)
		{
			if(VOS_SUCCESS != m_pTcpServerMgr->run())
			{
				WARN_LOG("CConnMgr::run: run m_pTcpServerMgr fail");
				return VOS_FAIL;
			}
		}

		return VOS_SUCCESS;

	}


	/*******************************************************************************
	  Function:       CConnMgr::exit()
	  Description:    退出各个manager
	  Calls:            
	  Called By:      
	  Input:          无 
	  Output:         无 
	  Return:         无 
	*******************************************************************************/
	void CConnMgr::exit(void)
	{
		if(NULL != m_pUdpSockMgr)
		{
			m_pUdpSockMgr->exit();
			delete(m_pUdpSockMgr);
			m_pUdpSockMgr = NULL;
		}

		if(NULL != m_pTcpServerMgr)
		{
			m_pTcpServerMgr->exit();
			delete(m_pTcpServerMgr);
			m_pTcpServerMgr = NULL;
		}

		if(NULL != m_pTcpConnMgr)
		{
			m_pTcpConnMgr->exit();
			delete(m_pTcpConnMgr);
			m_pTcpConnMgr = NULL;
		}

	#if VOS_APP_OS == VOS_OS_WIN32
		if(VOS_TRUE == m_bWSAStartup)
		{
			(void)WSACleanup();
			m_bWSAStartup = VOS_FALSE;
		}
	#endif
		return;
	}

	/*******************************************************************************
	  Function:       CConnMgr::setDefaultLocalAddr()
	  Description:    设置本地缺省地址
	  Calls:            
	  Called By:      
	  Input:          szLocalIpAddr: 本地地址
	  Output:         无 
	  Return:         无  
	*******************************************************************************/
	void CConnMgr::setDefaultLocalAddr(const char *szLocalIpAddr)
	{
		if(szLocalIpAddr != NULL)
		{
			long lLocalIp = (long)ACE_OS::inet_addr(szLocalIpAddr);
			if ((ULONG)lLocalIp != InvalidIp)
			{
				m_lLocalIpAddr = (long)ACE_OS::inet_addr(szLocalIpAddr);
			}
		}

		return;
	} 

	/*******************************************************************************
	  Function:       CConnMgr::regTcpClient()
	  Description:    创建TCP客户端
	  Calls:            
	  Called By:      
	  Input:          pLocalAddr: 本地地址，pPeerAddr: 对端地址，
					  pTcpConnHandle: 连接对应的handle
					  bSyncConn: VOS_TRUE表示同步连接，VOS_FALSE表示异步连接
	  Output:         无 
	  Return:         
	  VOS_SUCCESS: connect success
	  VOS_FAIL: connect fail 
	*******************************************************************************/
	long CConnMgr::regTcpClient( const CNetworkAddr *pLocalAddr, 
		const CNetworkAddr *pPeerAddr, CTcpConnHandle *pTcpConnHandle, 
		const EnumSyncAsync bSyncConn, ULONG ulTimeOut)    
	{
		if(NULL == pLocalAddr)
		{
			WARN_LOG("CConnMgr::regTcpClient: pLocalAddr is NULL");
			return VOS_FAIL;
		}
    
		if(NULL == pPeerAddr)
		{
			WARN_LOG("CConnMgr::regTcpClient: pPeerAddr is NULL");
			return VOS_FAIL;
		}
    
		if(NULL == pTcpConnHandle)
		{
			WARN_LOG("CConnMgr::regTcpClient: pTcpConnHandle is NULL");
			return VOS_FAIL;
		}
    
		if(VOS_SUCCESS != pTcpConnHandle->initHandle())
		{
			WARN_LOG(  "CConnMgr::regTcpClient: pTcpConnHandle init fail");
			return VOS_FAIL;
		}
    
		if(NULL == m_pTcpConnMgr)
		{
			WARN_LOG("CConnMgr::regTcpClient: m_pTcpConnMgr is NULL");
			return VOS_FAIL;
		}

		CNetworkAddr localAddr;
		if (InvalidIp == (ULONG)(pLocalAddr->m_lIpAddr))
		{
			localAddr.m_lIpAddr = this->m_lLocalIpAddr;
		}
		else
		{
			localAddr.m_lIpAddr = pLocalAddr->m_lIpAddr;
		}
		localAddr.m_usPort = pLocalAddr->m_usPort;

		pTcpConnHandle->m_localAddr.m_lIpAddr = pLocalAddr->m_lIpAddr;
		pTcpConnHandle->m_localAddr.m_usPort = pLocalAddr->m_usPort;
    
		long lRetVal = pTcpConnHandle->conn(&localAddr, pPeerAddr, bSyncConn, ulTimeOut);

		if(lRetVal != VOS_SUCCESS)
		{
			WARN_LOG("CConnMgr::regTcpClient: connect peer fail(0x%x:%d)",
				ntohl((ULONG)(pPeerAddr->m_lIpAddr)), ntohs(pPeerAddr->m_usPort));
			return lRetVal;
		}

		lRetVal = m_pTcpConnMgr->addHandle(pTcpConnHandle);
		if(lRetVal != VOS_SUCCESS)
		{
			pTcpConnHandle->close();
			WARN_LOG("CConnMgr::regTcpClient: register connection fail");
			return lRetVal;
		}

		return VOS_SUCCESS;
	}

	// BS_Start
	long CConnMgr::regTcpClientBind(const CNetworkAddr *pLocalAddr, CTcpConnHandle *pTcpConnHandle)
	{
		if(NULL == pLocalAddr)
		{
			WARN_LOG("CConnMgr::regTcpClient: pLocalAddr is NULL");
			return VOS_FAIL;
		}

		if(NULL == pTcpConnHandle)
		{
			WARN_LOG( "CConnMgr::regTcpClient: pTcpConnHandle is NULL");
			return VOS_FAIL;
		}

		if(VOS_SUCCESS != pTcpConnHandle->initHandle())
		{
			WARN_LOG("CConnMgr::regTcpClient: pTcpConnHandle init fail");
			return VOS_FAIL;
		}

		if(NULL == m_pTcpConnMgr)
		{
			WARN_LOG("CConnMgr::regTcpClient: m_pTcpConnMgr is NULL");
			return VOS_FAIL;
		}

		CNetworkAddr localAddr;
		if (InvalidIp == (ULONG)(pLocalAddr->m_lIpAddr))
		{
			localAddr.m_lIpAddr = this->m_lLocalIpAddr;
		}
		else
		{
			localAddr.m_lIpAddr = pLocalAddr->m_lIpAddr;
		}
		localAddr.m_usPort = pLocalAddr->m_usPort;

		pTcpConnHandle->m_localAddr.m_lIpAddr = pLocalAddr->m_lIpAddr;
		pTcpConnHandle->m_localAddr.m_usPort = pLocalAddr->m_usPort;

		// long lRetVal = pTcpConnHandle->conn(&localAddr, pPeerAddr, bSyncConn, ulTimeOut);
		long lRetVal = pTcpConnHandle->connBind(pLocalAddr);

		if(lRetVal != VOS_SUCCESS)
		{
			WARN_LOG("CConnMgr::regTcpClient: connBind fail(0x%x:%d)",
				ntohl((ULONG)(pLocalAddr->m_lIpAddr)), ntohs(pLocalAddr->m_usPort));
			return lRetVal;
		}
		return VOS_OK;
	}

	long CConnMgr::regTcpClientConn(const CNetworkAddr *pPeerAddr, CTcpConnHandle *pTcpConnHandle, const EnumSyncAsync bSyncConn, ULONG ulTimeOut)
	{
		long lRetVal = pTcpConnHandle->connAfterBind(&(pTcpConnHandle->m_localAddr), pPeerAddr, bSyncConn, ulTimeOut);

		if(lRetVal != VOS_SUCCESS)
		{
			WARN_LOG("CConnMgr::regTcpClient: connAfterBind peer fail(0x%x:%d)",
				ntohl((ULONG)(pPeerAddr->m_lIpAddr)), ntohs(pPeerAddr->m_usPort));
			return lRetVal;
		}

		lRetVal = m_pTcpConnMgr->addHandle(pTcpConnHandle);
		if(lRetVal != VOS_SUCCESS)
		{
			pTcpConnHandle->close();
			WARN_LOG("CConnMgr::regTcpClient: register connection fail");
			return lRetVal;
		}

		return VOS_SUCCESS;
	}
	// BS_End

	/*******************************************************************************
	  Function:       CConnMgr::removeTcpClient()
	  Description:    注销连接函数
	  Calls:            
	  Called By:      
	  Input:          pTcpConnHandle: 连接对应的handle
	  Output:         无 
	  Return:         无 
	*******************************************************************************/
	void CConnMgr::removeTcpClient(CTcpConnHandle *pTcpConnHandle)
	{
		if(NULL == pTcpConnHandle)
		{
			WARN_LOG( "CConnMgr::removeTcpClient: pTcpConnHandle is NULL");
			return;
		}
		DEBUG_LOG( "CConnMgr::removeTcpClient: remove pTcpConnHandle(0x%x) pHandleNode(0x%x) fd(%d) m_lIpAddr(0x%x) m_usPort(%d)", 
			pTcpConnHandle, pTcpConnHandle->m_pHandleNode, 
			pTcpConnHandle->m_lSockFD, pTcpConnHandle->m_localAddr.m_lIpAddr,
			pTcpConnHandle->m_localAddr.m_usPort );

		//此处不能关闭socket，原因如下:
		//调用通信平台中得CConnMgr::removeTcpClient函数时，
		//由于先关闭了socket，导致通信平台的socket扫描线程监控到socket有读事件，
		//但是此时socket已经被关闭了,socket 上报的读事件是非法的。
		//所以关闭socket动作要和监控socket事件操作，在时序上要互斥.
		//pTcpConnHandle->close();
    
		if(NULL == m_pTcpConnMgr)
		{
			WARN_LOG("CConnMgr::removeTcpClient: m_pTcpConnMgr is NULL");
			return;
		}

		m_pTcpConnMgr->removeHandle(pTcpConnHandle);
    
		return;
	} 

	/*******************************************************************************
	  Function:       CConnMgr::regTcpServer()
	  Description:    创建TCP服务器
	  Calls:            
	  Called By:      
	  Input:          pLocalAddr: 本地地址
					  pTcpServerHandle: TCP服务器对应的handle
	  Output:         无 
	  Return:         
	  VOS_SUCCESS: listen success
	  VOS_FAIL: listen fail 
	*******************************************************************************/
	long CConnMgr::regTcpServer(const CNetworkAddr *pLocalAddr, 
		CTcpServerHandle *pTcpServerHandle)
	{
		if(NULL == pLocalAddr)
		{
			WARN_LOG("CConnMgr::regTcpServer: pLocalAddr is NULL");
			return VOS_FAIL;
		}

		if(NULL == pTcpServerHandle)
		{
			WARN_LOG("CConnMgr::regTcpServer: pTcpServerHandle is NULL");
			return VOS_FAIL;
		}

		if(VOS_SUCCESS != pTcpServerHandle->initHandle())
		{
			WARN_LOG("CConnMgr::regTcpServer: pTcpServerHandle init fail");
			return VOS_FAIL;
		}

		if(NULL == m_pTcpConnMgr)
		{
			WARN_LOG("CConnMgr::regTcpServer: m_pTcpConnMgr is NULL");
			return VOS_FAIL;
		}

		if(NULL == m_pTcpServerMgr)
		{
			WARN_LOG("CConnMgr::regTcpServer: m_pTcpServerMgr is NULL");
			return VOS_FAIL;
		}
    
		CNetworkAddr localAddr;
		if (InvalidIp == (ULONG)(pLocalAddr->m_lIpAddr))
		{
			localAddr.m_lIpAddr = this->m_lLocalIpAddr;
		}
		else
		{
			localAddr.m_lIpAddr = pLocalAddr->m_lIpAddr;
		}
		localAddr.m_usPort = pLocalAddr->m_usPort;

		pTcpServerHandle->m_localAddr.m_lIpAddr = pLocalAddr->m_lIpAddr;
		pTcpServerHandle->m_localAddr.m_usPort = pLocalAddr->m_usPort;
    
		long lRetVal = pTcpServerHandle->listen(&localAddr);

		if(lRetVal != VOS_SUCCESS)
		{
			WARN_LOG("CConnMgr::regTcpServer: listen fail");
			return lRetVal;
		}

		lRetVal = m_pTcpServerMgr->addHandle(pTcpServerHandle);
		if(lRetVal != VOS_SUCCESS)
		{
			pTcpServerHandle->close();
			WARN_LOG("CConnMgr::regTcpClient: register tcp server fail");
			return lRetVal;
		}
    
		return VOS_SUCCESS;
	}

	/*******************************************************************************
	  Function:       CConnMgr::regTcpClient()
	  Description:    注销TCP服务器
	  Calls:            
	  Called By:      
	  Input:          pTcpServerHandle: TCP服务器对应的handle
	  Output:         无 
	  Return:         无 
	*******************************************************************************/
	void CConnMgr::removeTcpServer(CTcpServerHandle *pTcpServerHandle)
	{
		if(NULL == pTcpServerHandle)
		{
			WARN_LOG("CConnMgr::removeTcpServer: pTcpServerHandle is NULL");
			return;
		}
    
		//此处不能关闭socket，原因如下:
		//调用通信平台中得CConnMgr::removeTcpClient函数时，
		//由于先关闭了socket，导致通信平台的socket扫描线程监控到socket有读事件，
		//但是此时socket已经被关闭了,socket 上报的读事件是非法的。
		//所以关闭socket动作要和监控socket事件操作，在时序上要互斥.
		//pTcpServerHandle->close();
   
		if(NULL == m_pTcpServerMgr)
		{
			WARN_LOG("CConnMgr::removeTcpServer: m_pTcpServerMgr is NULL");
			return;
		}

		m_pTcpServerMgr->removeHandle(pTcpServerHandle);

		return;
	} 

	/*******************************************************************************
	  Function:       CConnMgr::regUdpSocket()
	  Description:    创建UDP socket
	  Calls:            
	  Called By:      
	  Input:          pLocalAddr: 本地地址，
					  pUdpSockHandle: 连接对应的handle
	  Output:         无 
	  Return:         
	  VOS_SUCCESS: create success
	  VOS_FAIL: create fail 
	*******************************************************************************/
	long CConnMgr::regUdpSocket(const CNetworkAddr *pLocalAddr, 
									 CUdpSockHandle *pUdpSockHandle,
									 const CNetworkAddr *pMultiAddr)
	{
		if(NULL == pLocalAddr)
		{
			WARN_LOG("CConnMgr::regUdpSocket: pUdpSockHandle is NULL");
			return VOS_FAIL;
		}
    
		if(NULL == pUdpSockHandle)
		{
			WARN_LOG("CConnMgr::regUdpSocket: pUdpSockHandle is NULL");
			return VOS_FAIL;
		}
    
		if(VOS_SUCCESS != pUdpSockHandle->initHandle())
		{
			WARN_LOG("CConnMgr::regUdpSocket: pUdpSockHandle init fail");
			return VOS_FAIL;
		}
    
		if(NULL == m_pUdpSockMgr)
		{
			WARN_LOG("CConnMgr::regUdpSocket: m_pUdpSockMgr is NULL");
			return VOS_FAIL;
		}
    
		CNetworkAddr localAddr;
		if (InvalidIp == (ULONG)(pLocalAddr->m_lIpAddr))
		{
			localAddr.m_lIpAddr = this->m_lLocalIpAddr;
		}
		else
		{
			localAddr.m_lIpAddr = pLocalAddr->m_lIpAddr;
		}
		localAddr.m_usPort = pLocalAddr->m_usPort;

		pUdpSockHandle->m_localAddr.m_lIpAddr = pLocalAddr->m_lIpAddr;
		pUdpSockHandle->m_localAddr.m_usPort = pLocalAddr->m_usPort;
   
		long lRetVal = pUdpSockHandle->createSock(&localAddr, pMultiAddr);
		if(lRetVal != VOS_SUCCESS)
		{
			WARN_LOG("CConnMgr::regUdpSocket: create UDP socket fail");
			return lRetVal;
		}

		lRetVal = m_pUdpSockMgr->addHandle(pUdpSockHandle);
		if(lRetVal != VOS_SUCCESS)
		{
			pUdpSockHandle->close();
			WARN_LOG("CConnMgr::regUdpSocket: register UDP socket fail");
			return lRetVal;
		}

		return VOS_SUCCESS;
	}

	/*******************************************************************************
	  Function:       CConnMgr::removeUdpSocket()
	  Description:    删除UDP socket
	  Calls:            
	  Called By:      
	  Input:          pUdpSockHandle: 连接对应的handle
	  Output:         无 
	  Return:         无 
	*******************************************************************************/
	void CConnMgr::removeUdpSocket(CUdpSockHandle *pUdpSockHandle)
	{
		if(NULL == pUdpSockHandle)
		{
			WARN_LOG("CConnMgr::removeUdpSocket: pUdpSockHandle is NULL");
			return;
		}
		pUdpSockHandle->close();

		if(NULL == m_pUdpSockMgr)
		{
			WARN_LOG("CConnMgr::removeUdpSocket: m_pUdpSockMgr is NULL");
			return;
		}

		m_pUdpSockMgr->removeHandle(pUdpSockHandle);

		return;
	}

}//end namespace
//ling +e438
//ling +e1733
//ling +e1732

#pragma warning(pop)



