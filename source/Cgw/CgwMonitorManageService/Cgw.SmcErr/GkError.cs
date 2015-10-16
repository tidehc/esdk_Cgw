//-----------------------------------------------------------------------
//文件名：SmcError.cs
//版权：Copyright 2010-2011 Huawei Tech. Co. Ltd. All Rights Reserved. 
//作者：mKF29742
//日期：2011-10-14
//描述：GK相关错误码。
//      不需要代码解析错误码的值，直接从数据库读取需要的字符串。
//-----------------------------------------------------------------------
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;

namespace CgwMonitorManage.SmcError
{
    /// <summary>
    /// GK错误信息
    /// </summary>
    [DataContract]
    public class GkErr : SmcErr
    {
        /// <summary>
        /// 构造函数
        /// </summary>
        /// <param name="result">结果</param>
        public GkErr(uint result)
            : base(result)
        {
        }
        
        /// <summary>
        /// 构造函数
        /// </summary>
        public GkErr()
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
