//-----------------------------------------------------------------------
//文件名：Mcu9660Error.cs
//版权：Copyright 2010-2011 Huawei Tech. Co. Ltd. All Rights Reserved. 
//作者：l36201 
//日期：2011-6-3
//描述：9610MCU通信的错误码。
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
    /// MCU9610错误信息
    /// </summary>
    [DataContract]
    public class Mcu9660Error : SmcErr
    {
        /// <summary>
        /// 构造函数
        /// </summary>
        /// <param name="result">结果</param>
        public Mcu9660Error(uint result)
            : base(result)
        {
        }

        /// <summary>
        /// 构造函数
        /// </summary>
        public Mcu9660Error()
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