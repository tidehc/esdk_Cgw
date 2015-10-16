using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Serialization;
using System.ServiceModel;
using System.Text;
using System.Text.RegularExpressions;
using System.Xml.Serialization;

namespace Cgw.SmcError
{
    /// <summary>
    /// 错误信息类
    /// </summary>
    [DataContract]
    [Serializable]
    [KnownType(typeof(RangeErrorInfo))]
    [KnownType(typeof(EmailErrorInfo))]
    [KnownType(typeof(RmAdapterErrorInfo))]
    public class ErrorInfo
    {
        /// <summary>
        /// 构造函数
        /// </summary>
        public ErrorInfo()
        {
            this.ParametersDictionary = new List<List<ValueDictionary>>();
        }

        /// <summary>
        /// 保存参数，保存的结构为{序号{标识名称，标识符替代值}}
        /// </summary>
        [DataMember]
        public List<List<ValueDictionary>> ParametersDictionary;

        /// <summary>
        /// 错误参数包括的扩展错误原因
        /// </summary>
        [DataMember]
        [System.Xml.Serialization.XmlElement(typeof(SmcErr))]
        [System.Xml.Serialization.XmlElement(typeof(GkErr))]
        [System.Xml.Serialization.XmlElement(typeof(Mcu9610Error))]
        [System.Xml.Serialization.XmlElement(typeof(Mcu9660Error))]
        [System.Xml.Serialization.XmlElement(typeof(Mcu8650Error))]
        [System.Xml.Serialization.XmlElement(typeof(McuHttpError))]
        [System.Xml.Serialization.XmlElement(typeof(VctError))]
        [System.Xml.Serialization.XmlElement(typeof(NrsError))]
        [System.Xml.Serialization.XmlElement(typeof(Tex0Error))]
        [System.Xml.Serialization.XmlElement(typeof(EConfError))]
        [System.Xml.Serialization.XmlElement(typeof(UpgradeError))]
        [System.Xml.Serialization.XmlElement(typeof(SmcAlarm))]
        [System.Xml.Serialization.XmlElement(typeof(LicenseError))]
        [System.Xml.Serialization.XmlElement(typeof(TP3206Error))]
        [System.Xml.Serialization.XmlElement(typeof(DataConfError))]
        public List<SmcErr> ExternErrs;

        /// <summary>
        /// 批量操作的错误参数
        /// </summary>
        [DataMember]
        [System.Xml.Serialization.XmlElement(typeof(SmcErr), ElementName = "MultipleErr")]
        [System.Xml.Serialization.XmlElement(typeof(GkErr), ElementName = "MultipleGkErr")]
        [System.Xml.Serialization.XmlElement(typeof(Mcu9610Error), ElementName = "MultipleMcu9610Error")]
        [System.Xml.Serialization.XmlElement(typeof(Mcu8650Error), ElementName = "MultipleMcu8650Error")]
        [System.Xml.Serialization.XmlElement(typeof(Mcu9660Error), ElementName = "MultipleMcu9660Error")]
        [System.Xml.Serialization.XmlElement(typeof(McuHttpError), ElementName = "MultipleMcuHttpError")]
        [System.Xml.Serialization.XmlElement(typeof(VctError), ElementName = "MultipleVctError")]
        [System.Xml.Serialization.XmlElement(typeof(NrsError), ElementName = "MultipleNrsError")]
        [System.Xml.Serialization.XmlElement(typeof(Tex0Error), ElementName = "MultipleTex0Error")]
        [System.Xml.Serialization.XmlElement(typeof(EConfError), ElementName = "MultipleEConfError")]
        [System.Xml.Serialization.XmlElement(typeof(UpgradeError), ElementName = "MultipleUpgradeError")]
        [System.Xml.Serialization.XmlElement(typeof(SmcAlarm), ElementName = "MultipleSmcAlarm")]
        [System.Xml.Serialization.XmlElement(typeof(LicenseError), ElementName = "MultipleGkLicenseError")]
        [System.Xml.Serialization.XmlElement(typeof(TP3206Error), ElementName = "MultipleTP3206Error")]
        public List<SmcErr> MultipleErrs;

        /// <summary>
        /// 嵌套错误码标识符名称，用于错误码内部
        /// </summary>
        private const string err = "err";

        /// <summary>
        /// 保存错误描述包括的扩展错误原因
        /// </summary>
        /// <param name="err">错误信息</param>
        public virtual void AddSmcErrInfo(SmcErr err)
        {
            if (null == this.ExternErrs)
            {
                this.ExternErrs = new List<SmcErr>();
            }

            if (null != err)
            {
                this.ExternErrs.Add(err);
            }
        }

        /// <summary>
        /// 保存批量操作的错误描述
        /// </summary>
        /// <param name="err">错误信息</param>
        public virtual void AddMultipleErrs(SmcErr err)
        {
            if (null == this.MultipleErrs)
            {
                this.MultipleErrs = new List<SmcErr>();
            }

            if (null != err)
            {
                SmcErr oldErr = this.MultipleErrs.Find(delegate(SmcErr target) { return target.ErrNo == err.ErrNo; });
                if (oldErr != null)
                {
                    try
                    {
                        if (err.ErrorInfoParam.ParametersDictionary != null && err.ErrorInfoParam.ParametersDictionary.Count > 0)
                        {
                            for (int i = 0; i < err.ErrorInfoParam.ParametersDictionary.Count; i++)
                            {
                                if (err.ErrorInfoParam.ParametersDictionary[i] != null && err.ErrorInfoParam.ParametersDictionary[i].Count > 0)
                                {
                                    for (int j = 0; j < err.ErrorInfoParam.ParametersDictionary[i].Count; j++)
                                    {
                                        if (err.ErrorInfoParam.ParametersDictionary[i][j].Name == oldErr.ErrorInfoParam.ParametersDictionary[i][j].Name &&
                                            err.ErrorInfoParam.ParametersDictionary[i][j].Value != oldErr.ErrorInfoParam.ParametersDictionary[i][j].Value)
                                        {
                                            //批量操作错误描述对象如果超过3个则用“...”代替
                                            if (oldErr.ErrorInfoParam.ParametersDictionary[i][j].Value.Split(',').Length < 3)
                                            {
                                                oldErr.ErrorInfoParam.ParametersDictionary[i][j].Value += ", " + err.ErrorInfoParam.ParametersDictionary[i][j].Value;
                                            }
                                            else if (oldErr.ErrorInfoParam.ParametersDictionary[i][j].Value.Split(',').Length == 3)
                                            {
                                                oldErr.ErrorInfoParam.ParametersDictionary[i][j].Value += ", ...";
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                    catch (Exception)
                    {
                    }
                }
                else
                {
                    this.MultipleErrs.Add(err);
                }
            }
        }

        /// <summary>
        /// 添加Email
        /// </summary>
        /// <param name="email">email</param>
        public virtual void AddEmail(string email)
        {
        }

        /// <summary>
        /// 格式化错误描述信息，填充错误参数
        /// </summary>
        /// <param name="delegateHandler">委托线程</param>
        /// <param name="langId">langId</param>
        /// <param name="errDesc">带格式的错误描述，需要补充参数</param>
        /// <returns>格式化以后的错误描述内容</returns>
        public string ToFormatedErrDesc(DelegateGetErrorDesc delegateHandler, int langId, string errDesc)
        {
            //获取标识符以及标识符对应的替代值字典
            Dictionary<string, string> errParams = this.GetErrorInfo(delegateHandler, langId, errDesc);

            string formatedString = errDesc;

            if (null != errParams || 0 != errParams.Count)
            {
                Dictionary<string, string>.KeyCollection paramKey = errParams.Keys;

                //填充描述字串参数
                foreach (string s in paramKey)
                {
                    formatedString = errDesc.Replace(s, errParams[s]);
                    errDesc = formatedString;
                }
            }

            //处理批量操作的错误描述
            if (null != this.MultipleErrs && this.MultipleErrs.Count != 0)
            {
                formatedString += "<br />";
                foreach (SmcErr err in this.MultipleErrs)
                {
                    //DTS2012080206396 批量添加失败报错，错误提示有"<br />"
                    //将这里的"<br />"去掉
                    formatedString += err.ToFomatedErrDesc(langId);
                }
            }

            return formatedString;
        }

        /// <summary>
        /// 取出字典{标识符，标识符替代值}
        /// </summary>
        /// <param name="delegateHandler">委托数据库取描述字串</param>
        /// <param name="langId">语种ID</param>
        /// <param name="errDesc">带格式的错误描述，需补充参数</param>
        /// <returns>字典{标识符，标识符替代值}</returns>
        public virtual Dictionary<string, string> GetErrorInfo(DelegateGetErrorDesc delegateHandler, int langId, string errDesc)
        {
            Dictionary<string, string> dictionary = new Dictionary<string, string>();

            //提取描述格式中的参数
            List<SignStruct> properties = this.GetProperties(errDesc);

            //根据参数名，从Dictionary中取值
            foreach (SignStruct o in properties)
            {
                //标识符序号
                int num = Convert.ToInt32(o.number);
                ////标识符名称
                string sign = o.sign;
                //初始化该字典项为“”;
                dictionary[o.data] = string.Empty;
                //嵌套错误码
                if (sign.Equals(err))
                {
                    //将标识符与错误描述填入字典中
                    if (this.ExternErrs.Count > num)
                    {
                        dictionary[o.data] = this.ExternErrs[num].ToFomatedErrDesc(langId);
                    }

                    continue;
                }


                //统一按名称匹配，暂时不处理名称一样，有多个索引的问题
                foreach (List<ValueDictionary> valueList in this.ParametersDictionary)
                {
                    if (valueList == null)
                    {
                        continue;
                    }

                    foreach (ValueDictionary valueDic in valueList)
                    {
                        if (sign.Equals(valueDic.Name))
                        {
                            dictionary[o.data] = valueDic.Value;
                            break;
                        }
                    }
                }
            }

            return dictionary;
        }

        /// <summary>
        /// 识别所有标识符“{标识名称}{序号}”
        /// </summary>
        /// <param name="errDesc">带格式的错误描述，需要补充参数</param>
        /// <returns>返回多个SignStruct，每个SignStruct结构是一个标识符对象，结构为{标识名称，序号，标识符}</returns>
        public List<SignStruct> GetProperties(string errDesc)
        {
            List<SignStruct> signDictionary = new List<SignStruct>();

            string pattern = @"\{[a-z,A-Z]+\}\s*\{[0-9]+\}";
            MatchCollection result = Regex.Matches(errDesc, pattern, RegexOptions.None);

            for (int i = 0; i < result.Count; i++)
            {
                string str = result[i].Value;
                string[] tempString = str.Split('{');

                // 对应标识符“{标识名称}{序号}”中的标识名称
                string sign = tempString[1].Substring(0, tempString[1].Length - 1);

                // 对应标识符“{标识名称}{序号}”中的序号
                string num = tempString[2].Substring(0, tempString[2].Length - 1);

                SignStruct signObject = new SignStruct(sign, num, result[i].Value);
                signDictionary.Add(signObject);
            }

            return signDictionary;
        }

        /// <summary>
        /// 根据参数名，查找字典中所有的指定参数值
        /// </summary>
        /// <param name="param">参数名</param>
        /// <returns>string</returns>
        public string FindParameters(string param)
        {
            string result = string.Empty;

            if (string.IsNullOrEmpty(param))
            {
                return string.Empty;
            }

            for (int i = 0; i < this.ParametersDictionary.Count; i++)
            {
                for (int j = 0; j < this.ParametersDictionary[i].Count; j++)
                {
                    ValueDictionary dic = this.ParametersDictionary[i][j];
                    if (param.Equals(dic.Name))
                    {
                        result += dic.Value + ",";
                    }
                }
            }

            if (!string.IsNullOrEmpty(result))
            {
                string temp = result.Trim(',');
                result = temp;
            }

            return result;
        }
    }
}