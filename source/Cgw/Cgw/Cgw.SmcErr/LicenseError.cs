//-----------------------------------------------------------------------
//文件名：Mcu9610Error.cs
//版权：Copyright 2010-2011 Huawei Tech. Co. Ltd. All Rights Reserved. 
//作者：mkf29742 
//日期：2011-6-3
//描述：License适配模块错误码。
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
    /// 认真错误
    /// </summary>
    [DataContract]
    public class LicenseError : SmcErr
    {
        /// <summary>
        /// 构造函数
        /// </summary>
        /// <param name="result">结果</param>
        public LicenseError(uint result)
            : base(result)
        {
        }

        /// <summary>
        /// 构造函数
        /// </summary>
        public LicenseError()
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
