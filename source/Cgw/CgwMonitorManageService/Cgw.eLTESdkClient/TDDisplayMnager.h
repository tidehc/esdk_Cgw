//===================================================================
//	Copyright(C)2013-2020, Huawei Tech. Co., Ltd.
//
//	文件名	：TDDisplayMnager.h
//	作  者	：w00206574
//	完成日期：2014-04-08
//	说  明	：实现eLTE接口C++ 和C#之间转换
//	其  它	：
//	历史记录：
//===================================================================

#include <list>
#include <string>
#include "notifyStruct_t.h"

using namespace std;

class TDDisplayMnager
{
private:
	TDDisplayMnager(void);
public:
	~TDDisplayMnager(void);

	//获取TDDisplayMnager静态实例
	static TDDisplayMnager& Instance();

	//通知组呼的状态变更信息
	void notifyGroupStatus(const string& strEventBuf) const;

	//通知用户和群组资源的状态变更信息
	void notifyResourceStatus(const string& strEventBuf);

	//通知点对点呼叫的状态变更信息
	void notifyP2pcallStatus(const string& strEventBuf) const;

	//通知视频呼叫的状态变更信息（包括视频回传），需要YC实现
	void notifyP2pvideocallStatus(const string& strEventBuf) const;

	void notifyUserStatus(const string& strEventBuf) const;

	void notifyUserSdsStatus(const string& strEventBuf) const;
	void notifyModuleStatus(const string& strEventBuf) const;

	//短数据信息上报给GUI
	void notifySMS(const string& strEventBuf) const;

	//通知配置对象的变更信息， 参数为一个多态指针， 使用者需要downcast 为对应的变更子类
	void notifyProvisionChange(const string& strEventBuf) const {;};

	//通知配置对象的全同步， 参数为一个多态指针， 使用者需要downcast 为对应的变更子类
	void notifyProvisionAllResync();

	//通知状态对象的全同步， 参数为一个多态指针， 使用者需要downcast 为对应的变更子类
	void notifyStatusAllResync(const string& strEventBuf) const;

	//获取登录事件对象
	void* GetLoginEventHandle() {return m_EventLogin;};

	//设置当前登录的用户id
	void SetResID(const std::string& strResID){m_strResID = strResID;};

protected:

private:
	void*       m_EventLogin;
	std::string  m_strResID;
};

#ifdef ESDK_EXPORTS
#define ESDK_API __declspec(dllexport)
#else
#define ESDK_API __declspec(dllimport)
#endif

#ifdef WIN32
#define _SDK_CALL __stdcall
#else
#define _SDK_CALL
#endif

#ifdef __cplusplus
extern "C"
{
#endif

 ESDK_API void _SDK_CALL SetGrpStaCallBack(notifyGroupStatus_t notiGrpSta);

 ESDK_API void _SDK_CALL SetResStaCallBack(notifyResourceStatus_t notiResSta);

 ESDK_API void _SDK_CALL SetP2pStaCallBack(notifyP2pcallStatus_t notiP2pSta);

 ESDK_API void _SDK_CALL SetUserStaCallBack(notifyUserStatus_t notiUserSta);

 //ESDK_API void _SDK_CALL SetSMSCallBack2(notifySMS2_t notiSMS);

 ESDK_API void _SDK_CALL SetModStaCallBack(notifyModuleStatus_t notifyModSta);

 ESDK_API void _SDK_CALL SetP2pVideoCallStaCallBack(notifyP2pvideocallStatus_t notifyP2pvideocallSta);

 ESDK_API void _SDK_CALL SetProvisionAllResyncCallBack(notifyProvisionAllResync_t notifyProSta);
#ifdef __cplusplus
}
#endif 


