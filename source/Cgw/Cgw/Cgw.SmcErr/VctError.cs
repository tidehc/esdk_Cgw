using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;

namespace Cgw.SmcError
{
    /// <summary>
    /// VCT错误信息
    /// </summary>
    [DataContract]
    public class VctError : SmcErr
    {
        /// <summary>
        /// 构造函数
        /// </summary>
        /// <param name="result">结果</param>
        public VctError(uint result)
            : base(result)
        {
        }

        /// <summary>
        /// 构造函数
        /// </summary>
        public VctError()
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
