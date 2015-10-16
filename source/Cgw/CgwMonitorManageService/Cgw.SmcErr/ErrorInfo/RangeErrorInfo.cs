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
    public class RangeErrorInfo : ErrorInfo
    {
        /// <summary>
        /// 构造函数
        /// </summary>
        public RangeErrorInfo()
            : base()
        {
        }

        /// <summary>
        /// 构造函数重载
        /// </summary>
        /// <param name="max">最大</param>
        /// <param name="min">最小</param>
        public RangeErrorInfo(string max, string min)
        {
            ParametersDictionary = new List<List<ValueDictionary>>();
            List<ValueDictionary> singleObject = new List<ValueDictionary>();
            singleObject.Add(new ValueDictionary("min", min));
            singleObject.Add(new ValueDictionary("max", max));
            ParametersDictionary.Add(singleObject);
        }
    }
}
