/********************************************************************
filename	: 	buff_pool.h
author		:	s90003596
created		:	2012/09/21
description	:	缓存池类头文件
copyright	:	Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history		:	2012/03/27 初始版本
*********************************************************************/

#ifndef _buff_pool_h
#define _buff_pool_h


#include "bp_namespace.h"
#include "bp_mutex.h"
#include "bp_list.h"
#include "bp_map.h"

NAMESPACE_BP_BEGIN

/**
 * @class CBuffPool
 * @brief 缓存池类
 *        实现不同大小级别的缓存池
 */
class CBuffPool
{
private:
    CBuffPool(const CBuffPool& obj);
    CBuffPool& operator=(const CBuffPool& obj);
protected:
    //构造函数，只能作为单体使用
    CBuffPool();
public:
    ~CBuffPool();

    static CBuffPool& instance();

    enum BUFF_POOL_CONST
    {
        BUFF_HDR_SIZE = sizeof(int),    //缓存区头部大小，用于保存缓冲区大小。
    };

    /**
     * 创建一个级别的缓存链表，该链表最大元素数位<max_buf_num>, 每个缓存的大小为<buf_size>字节.
     * 成功返回0，失败返回错误码。
     */
    int CreatePool(int max_buf_num, int buf_size);

    /**
     * 获取大小为<buf_size>字节的缓存.
     * 成功返回缓存指针（缓存第1字节的地址），失败返回NULL。
     */
    char *GetBuff(int buf_size);

    /**
     * 释放缓存<buff>.
     */
    void ReleaseBuff(char *buff);
protected:
    static CBuffPool static_instance;

    typedef CList<char *> BUF_LIST_T;

#pragma pack(push, 1)
    struct BuffList
    {
        BUF_LIST_T BufList;   //缓存列表
        int BufSize;          //缓存大小, 单位字节
        int MaxNum;           //最大元素数
        int BufNum;           //已分配的总数 
    };
#pragma pack(pop)

    typedef CMap<int, BuffList *> BUF_MAP_T;

    /**
     * 缓存链表映射：主键为缓存大小，值为缓存链表.
     */
    BUF_MAP_T m_BuffListMap;

    BP_Thread_Mutex m_Mutex;
};

NAMESPACE_BP_END

#endif //_buff_pool_h


