using System.Runtime.Serialization;
using System.Xml.Serialization;
using System.Collections.Generic;
using System.ServiceModel;

namespace CgwMonitorManage.SmcError
{
    /// <summary>
    /// 根据错误码查询错误描述字符串
    /// </summary>
    /// <param name="errNo">错误编号</param>
    /// <param name="modelName">模块名</param>
    /// <param name="langId">langid</param>
    /// <returns>string</returns>
    public delegate string DelegateGetErrorDesc(uint errNo, string modelName, int langId);
}