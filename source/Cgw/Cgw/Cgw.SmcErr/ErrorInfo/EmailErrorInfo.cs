using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;
using System.Xml.Serialization;
using System.ServiceModel;

namespace Cgw.SmcError
{
    /// <summary>
    /// Email错误详细信息结构
    /// </summary>
    [DataContract]
    public class EmailErrorInfo : ErrorInfo
    {
        /// <summary>
        /// 构造函数
        /// </summary>
        public EmailErrorInfo()
            : base()
        {
        }

        /// <summary>
        /// email
        /// </summary>
        public const string EMAIL_ADDRESS = "email";

        /// <summary>
        /// email错误信息
        /// </summary>
        /// <param name="email">email</param>
        public EmailErrorInfo(string email)
        {
            ParametersDictionary = new List<List<ValueDictionary>>();
            List<ValueDictionary> singleObject = new List<ValueDictionary>();
            singleObject.Add(new ValueDictionary(EMAIL_ADDRESS, email));
            ParametersDictionary.Add(singleObject);
        }

        /// <summary>
        /// 添加Email
        /// </summary>
        /// <param name="email">email</param>
        public override void AddEmail(string email)
        {
            List<ValueDictionary> singleObject = ParametersDictionary[0];
            ValueDictionary value = singleObject[0];
            string multipleEmail = value.Value + "," + email;
            value.Value = multipleEmail;
        }

        /// <summary>
        /// Email的最大长度
        /// </summary>
        public const string MAX_EMAIL_LENGTH = "max";

        /// <summary>
        /// 添加Email的长度
        /// </summary>
        /// <param name="length">email长度</param>
        public void AddEmailLength(uint length)
        {
            ////if (null != length)Link_20111229 表达式恒等于True无意义
            ////{
            List<ValueDictionary> singleObject = new List<ValueDictionary>();
            singleObject.Add(new ValueDictionary(MAX_EMAIL_LENGTH, length.ToString()));
            ParametersDictionary.Add(singleObject);
            ////}
        }
    }
}