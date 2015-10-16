using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;

namespace CgwMonitorManage.SmcError
{
    /// <summary>
    /// 错误码描述信息
    /// </summary>
    [DataContract]
    public class ErrNoDesc
    {
        /// <summary>
        /// 错误码模块
        /// </summary>
        [DataMember]
        public string Model;

        /// <summary>
        /// 语言编号
        /// </summary>
        [DataMember]
        public int Languageid;

        /// <summary>
        /// 错误描述
        /// </summary>
        [DataMember]
        public int ErrNo;

        /// <summary>
        /// 错误码描述
        /// </summary>
        [DataMember]
        public string ErrDesc;
    }
}
