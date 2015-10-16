#ifndef IVSCBB_SS_DCGSTACK_HPP
#define IVSCBB_SS_DCGSTACK_HPP

#include <vector>
#include <string>
#include "sipstack_config.hpp"

using namespace std;

namespace SipStack{

class CSipStack;
class SipStackAgentServerImpl;

class CDcgStack
{
private:
	CDcgStack(const CDcgStack& ){}
	CDcgStack& operator=(const CDcgStack&){}	
public:

	CDcgStack();
	~CDcgStack();

	// 初始化
	long Init(SipStackAgentServerImpl *sipImpl);
	void Fini(void);
    
	// 发送消息
	bool setRegInfo(const SipStackAgentCfg &regInfo);
	bool sendRegister (const CSipVar &sipVar);
	bool sendUnRegister(const CSipVar &sipVar);
	bool sendInvite(const string &message);
	bool sendInvite(const string &message, const CSipVar &sipVar);
	bool sendInviteResp (const string &message, unsigned long seq, unsigned long retCode);
	bool sendMsg(const string &message, const CSipVar &sipVar);
	bool sendMsg(const string &message);
	bool sendMsg(const string &message, const CSipVar &sipVar, unsigned long msgSeq);
	bool sendOption (void);
	bool sendOption (const CSipVar &sipVar);

	bool sendMsgEx(const string &message);
	bool sendMsgEx(const string &message, const string userName, const string hostName);

	unsigned long getSipSeq(void);

	long sendResp(const unsigned long seq);

	// 取得Server对象
	SipStackAgentServerImpl *GetServer(void);
	// bool sendReRegister (void);

	//     bool setRegName(string const &regName);
	// 
	//     bool setRegPwd(string const &regPwd);

	///////////////////////////////////////////  add by w00207012 2012-11-02 Begin //////////////////////////////////////////////////
	long setServerAddrEx(bool bServerAddrEx, const string& serverIP, unsigned short serverPort);
	bool sendRegisterResp (unsigned long seq, unsigned long retCode);
	bool sendCommonMsgResp(unsigned long seq, unsigned long retCode);
	bool sendOptionMsgResp(unsigned long seq, unsigned long retCode);
	bool sendInviteAck(unsigned long seq);
	bool sendByeReq(unsigned long seq);
	bool sendCancelReq(unsigned long seq);
	bool sendSubscribeReq(const string &message, const CSipVar &sipVar);
	///////////////////////////////////////////  add by w00207012 2012-11-02 End ////////////////////////////////////////////////////

	//modify-cwx148380-begin
	bool ansMsg(const string& ans_code, const string& data);
	//modify-cwx148380-end

private: 
	CSipStack* m_pSipStack;
	SipStackAgentServerImpl *m_pServer;
	unsigned short m_serverPort;
	unsigned short m_localPort;
};

}


#endif

