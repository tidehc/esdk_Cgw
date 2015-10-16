/*************************************************************************
Copyright (C), 1988-2008, Huawei Tech. Co., Ltd.
File Name       : NSS_Onlyone_Process.h
Author          :
Description     : 定义Onlyone_Process类
Others          :
Function List   :
History:        : // 历史修改记录
<author>    <ID>        <time>      <version>   <desc>
周昌平    ZKF14440     2009-5-12               修改成员函数注释。
*************************************************************************/

#ifndef _Onlyone_Process_h
#define _Onlyone_Process_h

#include "bp_namespace.h"
#include "bp_def.h"

#define FILE_LOCK_TYPE
#define  MAX_FILENAME_LEN 512

NAMESPACE_BP_BEGIN
const int SEM_PRMS = 0644; //信号量操作权限，0644即主用户(属主)可读写、组成员及其它成员可读不可写

/**
 * @class Onlyone_Process
 * @brief 进程唯一性检查类
 *        LINUX: 利用文件锁实现
 *        WIN32: 利用命名EVENT实现.
 */
class EXPORT_DLL Onlyone_Process
{
private:
    Onlyone_Process(const Onlyone_Process& obj);
    Onlyone_Process& operator =(const Onlyone_Process& obj);
protected:
    Onlyone_Process();
    ~Onlyone_Process();
public:
    /**
     * 检查进程是否运行.
     * 已经运行返回false, 否则返回true.
     */
    static bool onlyone(const char *strFileName);

#ifndef WIN32
protected:    
    bool  lockfile(const char *strcfgfile);
#endif  //WIN32
};
NAMESPACE_BP_END

#endif //_NSS_Onlyone_Process_h
