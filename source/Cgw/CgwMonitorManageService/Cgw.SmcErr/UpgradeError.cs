using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;

namespace CgwMonitorManage.SmcError
{
    /// <summary>
    /// 升级状态信息扩展错误码
    /// </summary>
    [DataContract]
    public class UpgradeError : SmcErr
    {
        public UpgradeError()
        { 
        }

        public UpgradeError(uint errNo)
            :base(errNo)
        {
        }
    }
}
