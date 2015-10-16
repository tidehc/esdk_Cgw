using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.ServiceModel;
using System.Runtime.Serialization;

namespace Cgw.Common
{
    /// <summary>
    /// 通道信息
    /// </summary>
    [DataContract]
    public class ChannelInfo
    {
        /// <summary>
        /// 通道标识
        /// </summary>
        [DataMember]
        public string ChannelLabel;

        /// <summary>
        /// 关联会议号
        /// </summary>
        [DataMember]
        public string AccessCode;

        /// <summary>
        /// 是否被使用
        /// license无效时，该项被置为false
        /// </summary>
        [DataMember]
        public bool IsValid;

        /// <summary>
        /// 无参构造函数
        /// </summary>
        public ChannelInfo()
        {
            this.IsValid = true;
        }

        /// <summary>
        /// 测试时使用
        /// </summary>
        /// <param name="ChannelLabel"></param>
        /// <param name="AccessCode"></param>
        public ChannelInfo(string ChannelLabel, string AccessCode)
        {
            this.ChannelLabel = ChannelLabel;
            this.AccessCode = AccessCode;
            this.IsValid = true;
        }


    }
}
