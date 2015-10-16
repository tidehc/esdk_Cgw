using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
//using NLog;

namespace Cgw.SmcError
{
    /// <summary>
    /// 错误码字典key类
    /// </summary>
    public class SmcErrKey
    {
        /// <summary>
        /// 模块名
        /// </summary>
        public string Model { get; set; }

        /// <summary>
        /// 语言ID
        /// </summary>
        public int LanguageId { get; set; }

        /// <summary>
        /// 错误码号
        /// </summary>
        public int ErrNo { get; set; }

        /// <summary>
        /// 构造函数
        /// </summary>
        public SmcErrKey()
        {
        }

        /// <summary>
        /// 获取哈希值
        /// </summary>
        /// <returns>哈希值</returns>
        public override int GetHashCode()
        {
            int hashCode = 0;
            hashCode = this.ErrNo * this.LanguageId + this.Model.GetHashCode();
            return hashCode;
        }

        /// <summary>
        /// 判断两个是否相同
        /// </summary>
        /// <param name="obj">object</param>
        /// <returns>bool</returns>
        public override bool Equals(object obj)
        {
            bool result = false;
            if (null == obj)
            {
                return false;
            }

            SmcErrKey errKey = obj as SmcErrKey;
            if (errKey != null)
            {
                if (errKey.ErrNo == this.ErrNo && errKey.LanguageId == this.LanguageId && errKey.Model == this.Model)
                {
                    result = true;
                }
            }
            return result;
        }
    }

    /// <summary>
    /// ResHelper
    /// </summary>
    public class ResHelper
    {
        /// <summary>
        /// 声明用于存放全部错误码的全局变量
        /// </summary>
        public static Dictionary<SmcErrKey, string> errNoDesc = new Dictionary<SmcErrKey, string>();

        //public static NLog.Logger log = NLog.LogManager.GetCurrentClassLogger();

        /// <summary>
        /// 对象
        /// </summary>
        private static ResHelper resInstance;

        /// <summary>
        /// 单例模式实例
        /// </summary>
        public static ResHelper Instance()
        {
            if (resInstance == null)
            {
                resInstance = new ResHelper();
            }

            return resInstance;
        }

        /// <summary>
        /// 构造函数
        /// </summary>
        public ResHelper()
        {
        }

        /// <summary>
        /// 初始化res
        /// </summary>
        /// <param name="ErrDesc">错误列表</param>
        public void InitialRes(List<ErrNoDesc> ErrDesc)
        {
            if (null != ErrDesc && ErrDesc.Count != 0)
            {
                errNoDesc.Clear();
                //log.Trace("clear errDesc success!");
                foreach (ErrNoDesc errDesc in ErrDesc)
                {
                    SmcErrKey errKey = new SmcErrKey();
                    errKey.Model = errDesc.Model;
                    errKey.ErrNo = errDesc.ErrNo;
                    errKey.LanguageId = errDesc.Languageid;
                    try
                    {
						//Link_
                        if (!errNoDesc.ContainsKey(errKey))
                        {
                            errNoDesc.Add(errKey, errDesc.ErrDesc);
                        }
                    }
                    catch (Exception)
                    {
                        throw new Exception("Repeat to init smcErr key,the key is " + errKey.ErrNo + "!the smcErr desc is ：" + errDesc.ErrDesc);
                    }
                }
            }
            else
            {
                //log.Info("Init errDesc failed,the errdesc is null!");
            }
        }

        /// <summary>
        /// 查询指定错误码类和语言的错误码描述
        /// </summary>
        /// <param name="errNo">错误码</param>
        /// <param name="modelName">错误码模块名称</param>
        /// <param name="languageId">语种ID</param>
        /// <returns>ErrDes</returns>
        public string GetErrDesc(uint errNo, string modelName, int languageId)
        {
            string ErrDes = string.Format("{0,10:X}", errNo);
            if (null != errNoDesc && errNoDesc.Count>0)
            {
                //log.Trace("try get err desc,The errNo:{0},the modelName:{1},the languageId:{2},the total error is:{3}", errNo, modelName, languageId, errNoDesc.Count);
                SmcErrKey errKey = new SmcErrKey();
                string tempErrDes = string.Empty;
                errKey.ErrNo = Convert.ToInt32(errNo);
                errKey.Model = modelName;
                errKey.LanguageId = languageId;
                if (errNoDesc.TryGetValue(errKey, out tempErrDes))
                {
                    ErrDes = tempErrDes.Replace("'", "\'");
                }
                //log.Trace("the err desc result is:{0}", ErrDes);
            }
            else
            {
                //log.Trace("The errDes is null!");
            }

            return ErrDes;
        }
    }
}