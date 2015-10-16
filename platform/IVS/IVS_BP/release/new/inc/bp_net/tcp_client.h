#ifndef _tcp_client_h
#define _tcp_client_h


#include "net_element.h"


NAMESPACE_BP_BEGIN

// TCP通讯网元类
class EXPORT_DLL CTCPClient : public INetElement
{
public:
    CTCPClient(void);
    virtual ~CTCPClient(void);

    // 命令处理
    virtual int HandleCmd(CCmd* pCmd);

    // 列程处理
    virtual uint32_t Routing(void);

    // 判断是否客户端
    virtual bool IsClient(void);

    // 注册检查网络事件的IO
    virtual void RegisterHandle(ACE_Handle_Set& rdHandleSet, ACE_Handle_Set& wtHandleSet, ACE_Handle_Set& exHandleSet);
};


NAMESPACE_BP_END

#endif //
