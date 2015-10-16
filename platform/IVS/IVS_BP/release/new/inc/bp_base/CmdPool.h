/********************************************************************
filename    :    CmdPool.h
author      :    j00213308
created     :    2012/05/07
description :    命令对象池
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2012/05/07 初始版本
*********************************************************************/
#ifndef BP_CMD_POOL_H
#define BP_CMD_POOL_H

#include <map>
#include <list>
#include "ace_header.h"
#include "bp_namespace.h"

NAMESPACE_BP_BEGIN

#define BP_CMD_HIGH_WATER_MARK			40960			// 池高水位标示
#define BP_CMD_LOW_WATER_MARK			4096			// 池低水位标示
#define BP_CMD_POOL_GROW_SIZE			50				// 池增长步长
#define BP_CMD_BUFF_LEN				4096			// 池内定长缓存长度
#define BP_CMD_POOL_INIT_SIZE			500				// 池初始大小

class CCmd;

typedef std::list<CCmd*>					ListCmd;
typedef std::list<CCmd*>::iterator			ListCmdIter;
typedef std::map<CCmd*, CCmd*>				MapCmd;
typedef std::map<CCmd*, CCmd*>::iterator	MapCmdIter;
typedef ACE_Recursive_Thread_Mutex	MutexThread;

// 命令对象池
class CCmdPool
{
public:
	static CCmdPool* GetInstance(void);
	~CCmdPool(void);

	/**************************************************************************
	* name       : Init
	* description: 初始化命令对象内存池
	* input      : NA
	* output     : NA
	* return     : NA
	* remark     : NA
	**************************************************************************/
	void Init(void);

	/**************************************************************************
	* name       : Fnit
	* description: 销毁命令对象内存池
	* input      : NA
	* output     : NA
	* return     : NA
	* remark     : NA
	**************************************************************************/
	void Fnit(void);

	/**************************************************************************
	* name       : Alloc
	* description: 从对象池获取命令对象
	* input      : uiBuffLen: 命令对象内缓存大小
	* output     : NA
	* return     : 命令对象指针
	* remark     : NA
	**************************************************************************/
	CCmd* Alloc(unsigned int uiBuffLen);

	/**************************************************************************
	* name       : Release
	* description: 释放命令对象
	* input      : pCmd: 待释放的命令对象指针
	* output     : NA
	* return     : NA
	* remark     : NA
	**************************************************************************/
	void Release(CCmd* pCmd);

private:
	CCmdPool(void);
	// 对象池增长
	void Grow(unsigned int uiNum);

private:
	static CCmdPool*    m_pInstance;		// 池指针
	static MutexThread  m_mutex;			// 池锁

	unsigned int m_uiHighWaterMark;			// 命令对象池高水位标识
	unsigned int m_uiLowWaterMark;			// 命令对象池低水位标识
	unsigned int m_uiGrowSize;				// 池的增长步长
	unsigned int m_uiBuffLen;				// 命令对象内缓存大小

	ListCmd		 m_freeCmdList;				// 可用的命令对象
	MapCmd		 m_usedCmdList;				// 已使用的命令对象
};

NAMESPACE_BP_END

#endif
