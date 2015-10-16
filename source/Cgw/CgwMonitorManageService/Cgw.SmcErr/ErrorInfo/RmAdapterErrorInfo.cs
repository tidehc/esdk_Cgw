using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;
using System.Xml.Serialization;
using System.ServiceModel;

namespace CgwMonitorManage.SmcError
{
    /// <summary>
    /// 值比较、值范围错误详细信息结构
    /// </summary>
    [DataContract]
    public class RmAdapterErrorInfo : ErrorInfo
    {
        /// <summary>
        /// 构造函数
        /// </summary>
        public RmAdapterErrorInfo()
            : base()
        {
        }

        public const string RMADAPTER_ERR = "adaptererror";

        /// <summary>
        /// 添加由RmAdapter传上来的参数
        /// </summary>
        /// <param name="errReason">参数值</param>
        public void AddRmAdapterError(string errReason)
        {
            List<ValueDictionary> singleObject = new List<ValueDictionary>();
		    singleObject.Add(new ValueDictionary(RMADAPTER_ERR,errReason));
	        ParametersDictionary.Add(singleObject);
        }
    }
}
