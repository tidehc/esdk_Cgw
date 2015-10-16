/********************************************************************
filename    :    GenerateID.h 
author      :    lkf72560
created     :    2012/11/26
description :    生成用户的唯一标示
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd 
history     :    2012/11/26 初始版本
*********************************************************************/

#ifndef BP_GENERATE_ID_H
#define BP_GENERATE_ID_H

#include <string>
#include "bp_namespace.h"
#include "ace/Truncate.h"
#include "ace/UUID.h"

using namespace std;
NAMESPACE_BP_BEGIN // namespace begin

/**********************************************************************
    * name           : GenerateUUID
    * description    : 生成用户的唯一标示
    * input          : NA
    * return         : string - 用户的唯一标示
    * remark         : NA   
*********************************************************************/
static string GenerateUUID()
{
    // 生成用户的唯一标示
    ACE_Utils::UUID_Generator uuidout;
    uuidout.init();
    ACE_Utils::UUID uuID;
    uuidout.generate_UUID(uuID);

    string strUUID;
    const ACE_CString* pUUID = uuID.to_string();
    if (NULL == pUUID)
    {
        return strUUID;
    }
    strUUID = pUUID->c_str();
    transform(strUUID.begin(), strUUID.end(), strUUID.begin(), ::toupper);
    return strUUID;
}

NAMESPACE_BP_END   // namespace end
#endif
