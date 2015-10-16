using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;
using System.Xml.Serialization;

namespace CgwMonitorManage.SmcError
{
    /// <summary>
    /// ValueDictionary
    /// </summary>
    [DataContract]
    public class ValueDictionary
    {
        /// <summary>
        /// 构造函数
        /// </summary>
        public ValueDictionary()
        {
        }

        /// <summary>
        /// 构造函数重载
        /// </summary>
        /// <param name="name">名字</param>
        /// <param name="value">值</param>
        public ValueDictionary(string name, string value)
        {
            this.Name = name;
            this.Value = value;
        }

        /// <summary>
        /// 标识符名称
        /// </summary>
        [DataMember]
        public string Name;

        /// <summary>
        /// 标识符替代值
        /// </summary>
        [DataMember]
        public string Value;
    }

    /// <summary>
    /// 标识符“{标识符名称}{序号}”
    /// </summary>
    public class SignStruct
    {
        /// <summary>
        /// 构造函数
        /// </summary>
        public SignStruct()
        {
        }

        /// <summary>
        /// 构造函数
        /// </summary>
        /// <param name="sign">标识符名称</param>
        /// <param name="number">标识符序号</param>
        /// <param name="data">标识符字串</param>
        public SignStruct(string sign, string number, string data)
        {
            this.sign = sign;
            this.number = number;
            this.data = data;
        }

        /// <summary>
        /// 标识符名称
        /// </summary>
        [DataMember]
        public string sign;

        /// <summary>
        /// 标识符序号
        /// </summary>
        [DataMember]
        public string number;

        /// <summary>
        /// 标识符字串
        /// </summary>
        [DataMember]
        public string data;
    }
}
