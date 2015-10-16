/********************************************************************
filename    :    IService.h
author      :    s00191067
created     :    2012/03/27
description :    任务接口类
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2012/03/27 初始版本
*********************************************************************/
#ifndef BP_ISERVICE_H
#define BP_ISERVICE_H

#include <map>
#include "ace_header.h"
#include "bp_def.h"
#include "bp_stdint.h"


NAMESPACE_BP_BEGIN // namespace begin
class CCmd;
class EXPORT_DLL IService
{
public:
    IService(void);
    virtual ~IService(void);

public:
    virtual int Init();

    virtual int Close();

    /**************************************************************************
    * name       : HandleCmd
    * description: 播发命令，具体任务实现类需要处理命令
    * input      : pCmd 命令指针
    * output     : NA
    * return     : IVS_SUCCEED - 成功，IVS_FAIL - 失败
    * remark     : NA
    **************************************************************************/
    virtual int HandleCmd(CCmd* pCmd) = 0;

    /**************************************************************************
    * name       : Routing
    * description: 定期检查，具体任务实现类处理定期检查任务
				   注意：一定要统计本次routing完成的事件数，线程的轮询依赖于此统计！！！
    * input      : NA
    * output     : NA
    * return     : 返回本次rouint完成的事件数，便于后面统计
    * remark     : NA
    **************************************************************************/
    virtual uint32_t Routing(void) = 0;

    /**************************************************************************
    * name       : GetServicePayload
    * description: 获取Service当前负载权重
    * input      : NA
    * output     : NA
    * return     : 负载权重
    * remark     : 默认情况下Service 权重为0, MeidaService 需考虑session 权重值
    **************************************************************************/
    virtual unsigned int GetServicePayload();
   

    void SetServiceID(uint32_t uiServiceID);

    uint32_t GetServiceID() const;
	virtual uint32_t GetSessionCount(){return 0;}

    void SetResendIntervalOffset(uint32_t uiResendIntervalOffset);

    uint32_t GetResendIntervalOffset() const;

    void SetResendTimes(uint32_t uiResendTimes);

    uint32_t GetResendTimes() const;

	std::string GetServiceName() const;

	void SetServiceName(const std::string &strServiceName);

protected:
    uint32_t m_uiResendIntervalOffset;
    uint32_t m_uiResendTimes;

private:
    uint32_t m_uiServiceID;
	std::string m_strServiceName;
};

class EXPORT_DLL CServiceSet
{
public:
    CServiceSet();
    ~CServiceSet();

    /**************************************************************************
    * name       : GetServiceNum
    * description: 获取Service个数
    * input      : NA
    * output     : NA
    * return     : Service个数
    * remark     : NA
    **************************************************************************/
    unsigned int GetServiceNum(void);

    /**************************************************************************
    * name       : AddService
    * description: 增加Service到Service集中
    * input      : pService Service对象
    * output     : NA
    * return     : IVS_SUCCEED - 成功，IVS_FAIL - 失败
    * remark     : NA
    **************************************************************************/
    int AddService(IService* pService);

    /**************************************************************************
    * name       : DelService
    * description: 删除Service对象
    * input      : pService Service对象
    * output     : NA
    * return     : IVS_SUCCEED - 成功，IVS_FAIL - 失败
    * remark     : NA
    **************************************************************************/
    int DelService(IService*& pService);

    /**************************************************************************
    * name       : FindService
    * description: 根据ServiceID查找Service对象
    * input      : enServiceID ServiceID
    * output     : NA
    * return     : Service对象指针
    * remark     : NA
    **************************************************************************/
    IService* FindService(uint32_t enServiceID);

    /**************************************************************************
    * name       : RoutingAll
    * description: 播发所有Service的Routing，执行Set集中所有Service的Routing
    * input      : NA
    * output     : NA
    * return     : 返回本接口中完成的事情，用于后面统计
    * remark     : NA
    **************************************************************************/
    uint32_t RoutingAll();

    /**************************************************************************
    * name       : DelAllService
    * description: 销毁所有Service
    * input      : NA
    * output     : NA
    * return     : NA
    * remark     : NA
    **************************************************************************/
    void DelAllService(void);


    /**************************************************************************
    * name       : GetTotalPayload
    * description: 获取service集中所有serverice的负载权重和
    * input      : NA
    * output     : NA
    * return     : 所有service负载权重和
    * remark     : NA
    **************************************************************************/
    unsigned int GetTotalPayload();
	uint32_t GetSessionCount();

private:
    typedef std::map<uint32_t, IService*>   CServiceMap;
    typedef CServiceMap::iterator           ServiceIter;
    CServiceMap                             m_mapService;
    ACE_Thread_Mutex                        m_mutex;              // 线程负载列表锁;
};

NAMESPACE_BP_END   // namespace end
#endif //BP_ISERVICE_H
