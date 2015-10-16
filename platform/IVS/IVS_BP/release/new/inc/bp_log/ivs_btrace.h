#ifndef __IVS_BACKTRACE_H__
#define __IVS_BACKTRACE_H__


/* linux 使用backtrace功能 编译时加上 -g  -funwind-tables  -rdynamic 选项 */
/* windows 使用backtrace 需要将相应的 pdb 文件拷贝到运行程序目录下 */

/*
 * 初始化库及进程符号资源
 */
//int  IVS_BackTraceInit();

/*
 * 清除进程符号资源
 */
void IVS_BackTraceClean();

/*
 * Dump 调用栈到缓存中
 */
size_t IVS_DumpBackTrace(char *buffer, size_t len);


#define MAX_BACKTRACE_SIZ  16   // 最大回溯层数

#endif 