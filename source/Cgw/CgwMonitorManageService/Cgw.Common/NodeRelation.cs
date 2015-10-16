/*-----------------------------------------------------------------------
//文件名：NodeRelation.cs
//版权：Copyright 2011-2012 Huawei Tech. Co. Ltd. All Rights Reserved. 
//作者：kf70422
//日期：2013-2-27
//描述：节点关系，描述摄像头和组之间的关联
//---------------------------------------------------------------------*/
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;
using System.ServiceModel;

namespace CgwMonitorManage.Common
{
    /// <summary>
    /// 摄像头分组关系表
    /// </summary>
    [DataContract]
    public class NodeRelation
    {
        /// <summary>
        /// 节点编号
        /// </summary>
        [DataMember]
        public string No { set; get; }

        /// <summary>
        /// 分组各层节点（父节点路径）
        /// </summary>
        [DataMember]
        public List<string> Path { set; get; }

        /// <summary>
        /// 分组各层节点
        /// </summary>
        [DataMember]
        public NodeType Type { set; get; }

        public NodeRelation(string no, List<string> path, NodeType type)
        {
            this.No = no;
            this.Path = path;
            this.Type = type;
        }
    }

    /// <summary>
    /// 节点类型
    /// </summary>
    [DataContract]
    public enum NodeType
    {
        [EnumMember]
        NONE = 0,
        [EnumMember]
        CAMERA = 1,
        [EnumMember]
        GROUP = 2,
    }
}
