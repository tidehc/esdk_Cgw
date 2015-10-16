/*-----------------------------------------------------------------------
//文件名：LoginInfo.cs
//版权：Copyright 2011-2012 Huawei Tech. Co. Ltd. All Rights Reserved. 
//作者：kf70422
//日期：2013-2-25
//描述：登陆信息，用于封装调用天地伟业登陆http服务接口返回的json数据
//---------------------------------------------------------------------*/
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;

namespace CgwMonitorManage.Tiandy
{
    /// <summary>
    /// 登陆信息对象，用于封装调用天地伟业登陆http服务接口返回的json数据，
    /// 字段名跟天地伟业返回的json数据结构保持一致
    /// </summary>
    [DataContract]
    internal class LoginInfo
    {

        [DataMember(Name = "clientuuid")]
        public string clientuuid { get; set; }

        /// <summary>
        /// 当前用户ID
        /// </summary>
        [DataMember(Name = "content")]
        public string content { get; set; }

        /// <summary>
        /// 登陆ip
        /// </summary>
        [DataMember(Name = "ip")]
        public string ip { get; set; }

        /// <summary>
        /// 登陆日期
        /// </summary>
        [DataMember(Name = "loginDate")]
        public string loginDate { get; set; }

        /// <summary>
        /// 登陆结果
        /// </summary>
        [DataMember(Name = "ret")]
        public string ret { get; set; }

        /// <summary>
        /// 登陆次数
        /// </summary>
        [DataMember(Name = "time")]
        public string time { get; set; }

        /// <summary>
        /// 用户名
        /// </summary>
        [DataMember(Name = "username")]
        public string username { get; set; }

        [DataMember(Name = "uuid")]
        public string uuid { get; set; }
    }
}
