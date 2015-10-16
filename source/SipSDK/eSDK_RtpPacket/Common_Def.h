/*******************************************************************
Copyright (C), 2008-2009, Huawei Symantec Tech. Co., Ltd.
File name:    Store_Signal.cpp
Author: 石玉清 ID 00003596     Version:  1.0        Date: 2009-09-15
Description:   定义通用的宏: 返回值等.

Others:        
Function List:  
1. ....
History:
1. Date:      2009-09-19
Author:       石玉清
ID:           00003596
Modification: create
2. ...
 ********************************************************************/
#ifndef _Common_Def_h
#define _Common_Def_h

enum NRU_CONST
{
    NRU_SUCCESS                = 0,   //返回成功
    NRU_FAIL                   = -1  //返回失败

    //NRU_CALLER_CAN_DELETE_MEM  = 0,   //调用者能够释放传入的内存
   // NRU_CALLER_CANT_DELETE_MEM = 1,   //调用者不能够释放传入的内存
    //NRU_TASK_ERROR_RETURN    = 0,   //Task的错误返回值

    //NRU_CANDELETE              = 1,   //对象可以删除,引用计数时使用
    //NRU_CANNOTDELETE           = 0,   //对象不可以删除，引用计数时使用

    //NRU_NOUPLOADFILE           = 1,   //没有断网录像需要下载

    //NRU_INVALID_INDEX_VALUE    = -1,  //无效的索引值

    //NRU_LOGIN_AGAIN            = 1,   //需要再次连接登录服务器

    //NRU_INVALID_BSM_SESSION_ID = -1,//BSM无效的会话ID

    //NRU_MAXUINT32              = 4294967295, //unsinged int 的最大值

    //NRU_INVALID_FILE           = -1,  //无效的文件描述符

    //NRU_INFINITE_TIME          = -1  //无限长时间
};

//NRU中set_timer()类函数: 设置定时器时的返回值
enum NRU_TIMER_RET
{
    NRU_TIMER_SET_SUCCESS      = 1,   //设置定时器成功
    NRU_TIMER_SET_LOCK         = 0,   //另一个线程正在设置, 已经锁住该操作
    NRU_TIMER_SET_FAIL         = -1  //设置定时器失败
};


enum _enRTP_HANDLE_TYPE
{
    VIDEO_RTP_HANDLE,
    VIDEO_RTCP_HANDLE,
    AUDIO_RTP_HANDLE,
    AUDIO_RTCP_HANDLE,

    HANDLE_TYPE_MAX
};

enum _enRTP_TRACK_HANDLE_TYPE
{
    TRACK_RTP_HANDLE,
    TRACK_RTCP_HANDLE,

    TRACK_HANDLE_TYPE_MAX
};

typedef struct _REAL_RECORD_TIME
{
    unsigned int uiSecond;
    unsigned int uiMSecond;
}REAL_RECORD_TIME;

#define NRU_NULL_CHAR     '\0' //字符串结束符
#define NRU_ZERO          0    //数值0
#define NRU_ONE           1    //数值1
#define NRU_TWO           2    //数值2
#define NRU_INVALID_PORT  0    //无效端口
#define NRU_KILO         (1024)

#define MAX_SQL_LEN 512

#define HIGH_WATER_M   ((500) * (1024) * (1024))     //录像输出队列的最大缓冲区为500M
#define LOW_WATER_M    ((500) * (1024) * (1024))     //录像输出队列的最大缓冲区为500M
#define DEFAULT_STACK_SIZE   ((128) * (1024))        //子线程栈大小128k

#define MEDIA_DATA_TIMEOUT       (120)                //获取媒体数据超时 
#define MBU_MEDIA_DATA_TIMEOUT   (5)                  //获取媒体数据超时 
#define CONTROL_DATA_TIMEOUT     (15)                 //控制消息入队列超时


#define DEFAULT_GOP_SIZE    25
#define MAX_GOP_SIZE    250

#define LOG_ERR_DISKFULL       500   /* 磁盘满         */
#define LOG_ERR_OPENFAIL       502   /* 打开文件失败    */
#define LOG_ERR_DBIMPORT       503   /* 导入数据库错误   */
#define LOG_ERR_RENAMEFAIL     504   /* 重命名文件失败  */
#define LOG_ERR_ZIPFAIL        505   /* 压缩文件失败    */
#define LOG_ERR_SFTPFAIL       506   /* 日志上传失败    */

/* 异常回调定义---恢复 */
#define LOG_RESUME_DISKFULL       550   /* 磁盘满恢复         */
#define LOG_RESUME_OPENFAIL       552   /* 打开文件失败恢复   */
#define LOG_RESUME_DBIMPORT       553   /* 导入数据库错误恢复 */
#define LOG_RESUME_RENAMEFAIL     554   /* 重命名文件失败恢复 */
#define LOG_RESUME_ZIPFAIL        555   /* 压缩文件失败恢复   */
#define LOG_RESUME_SFTPFAIL       556   /* 日志上传失败恢复   */

const int DAYS_OF_WEEK = 7;   /* 一周的天数   */

//socket缓存区大小
#define SOCKET_BUFFER_2M       (2 * 1024 * 1024)
#define SOCKET_BUFFER_1M       (1024 * 1024)
#define SOCKET_BUFFER_512K     (512 * 1024)
#define SOCKET_BUFFER_256K     (256 * 1024)
#define SOCKET_BUFFER_128K     (128 * 1024)
#define SOCKET_BUFFER_32K      (32  * 1024)
#define SOCKET_BUFFER_8K       ( 8  * 1024)

#ifndef WIN32 
#define UNUSED_PARA(P) (void)P
#else
#define UNUSED_PARA(P) UNREFERENCED_PARAMETER(P)
#endif

//log计数,防止频繁打印
#define LOG_COUNT_THRESHOLD_1000    (1000)

#endif //_Common_Def_h

