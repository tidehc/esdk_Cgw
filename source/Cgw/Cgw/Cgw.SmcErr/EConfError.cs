//-----------------------------------------------------------------------
//文件名：EConfError.cs
//版权：Copyright 2010-2011 Huawei Tech. Co. Ltd. All Rights Reserved. 
//作者：198314 
//日期：2011-12-2
//描述：EConf相关错误码
//      不需要代码解析错误码的值，直接从数据库读取需要的字符串。
//-----------------------------------------------------------------------
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;

namespace Cgw.SmcError
{
    /// <summary>
    /// Econf相关错误代码
    /// </summary>
    [DataContract]
    public class EConfError : SmcErr
    {
        /// <summary>
        /// 构造函数
        /// </summary>
        /// <param name="result">结果</param>
        public EConfError(uint result)
            : base(result)
        {
        }

        /// <summary>
        /// 构造函数
        /// </summary>
        public EConfError()
        {
        }

        /// <summary>
        /// 取模块名称
        /// </summary>
        /// <returns>模块名</returns>
        public override string GetModelName()
        {
            return this.GetType().Name;
        }
    }
}