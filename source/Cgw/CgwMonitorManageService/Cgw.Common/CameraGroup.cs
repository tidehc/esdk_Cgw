/*-----------------------------------------------------------------------
//文件名：CameraGroup.cs
//版权：Copyright 2011-2012 Huawei Tech. Co. Ltd. All Rights Reserved. 
//作者：kf70422
//日期：2013-2-27
//描述：组信息
//---------------------------------------------------------------------*/
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;
using System.ServiceModel;

namespace CgwMonitorManage.Common
{
    [DataContract]
    public class CameraGroup
    {
        /// <summary>
        /// 组编号
        /// </summary>
        [DataMember]
        public string No { get; set; }

        /// <summary>
        /// 组名称
        /// </summary>
        [DataMember]
        public string Name { get; set; }

        /// <summary>
        /// 父节点ID
        /// </summary>
        [DataMember]
        public string ParentID { get; set; }

        public CameraGroup(string no, string name)
        {
            this.No = no;
            this.Name = name;
        }
    }
}
