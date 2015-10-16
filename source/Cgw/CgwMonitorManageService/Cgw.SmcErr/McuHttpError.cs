//-----------------------------------------------------------------------
//文件名：Mcu9660Error.cs
//版权：Copyright 2010-2011 Huawei Tech. Co. Ltd. All Rights Reserved. 
//作者：l36201 
//日期：2011-6-3
//描述：MCU http通信的错误码。
//-----------------------------------------------------------------------
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;

namespace CgwMonitorManage.SmcError
{
    /// <summary>
    /// MCU http 通讯错误信息
    /// </summary>
    [DataContract]
    public class McuHttpError : SmcErr
    {
        /// <summary>
        /// 构造函数
        /// </summary>
        /// <param name="result">结果</param>
        public McuHttpError(uint result)
            : base(result)
        {

        }

        /// <summary>
        /// 构造函数
        /// </summary>
        public McuHttpError()
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