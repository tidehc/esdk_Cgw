#ifndef _udp_net_element_h
#define _udp_net_element_h

#include "net_element.h"

NAMESPACE_BP_BEGIN

// UDP通讯网元类
class EXPORT_DLL CUDPElement : public INetElement
{
public:
    CUDPElement(void);
    virtual ~CUDPElement(void);

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

#endif 
