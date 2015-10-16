#ifndef IVSCBB_SS_SIPSTACK_AGENT_MGR_IMPL_HPP
#define IVSCBB_SS_SIPSTACK_AGENT_MGR_IMPL_HPP

#include "sipstack_agent_mgr.hpp"
#include "sipstack_agent_client_impl.hpp"
#include "sipstack_agent_server_impl.hpp"

namespace SipStack{

class SipStackAgentMgrImpl : public SipStackAgentMgr
{
private:
    SipStackAgentMgrImpl ();

    virtual ~SipStackAgentMgrImpl();

public:
    static SipStackAgentMgrImpl* Instance();

    virtual bool Init ( const SipStackAgentCfg& agent_reg_info, unsigned long max_num_ul, const std::string& strFile );
    virtual void Fini ();

    virtual SipStackAgentClient*    GetAgentClient();
    SipStackAgentServerImpl*        GetAgentServer();

private:
    static SipStackAgentMgrImpl* m_instance_p;

    static unsigned int m_ref_n;

    bool m_bInit;

    SipStackAgentServerImpl* m_server_dflt_p;
    SipStackAgentClientImpl* m_client_dflt_p;
};

}
#endif 

