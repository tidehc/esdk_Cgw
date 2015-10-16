/*-----------------------------------------------------------------------
//文件名：Resource.cs
//版权：Copyright 2011-2012 Huawei Tech. Co. Ltd. All Rights Reserved. 
//作者：kf70422
//日期：2013-2-25
//描述：资源树信息，用于封装调用天地伟业获取资源树的http接口返回的json数据
//---------------------------------------------------------------------*/
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;

namespace CgwMonitorManage.Tiandy
{
    /// <summary>
    /// 资源树信息，跟接口返回的json数据结构保持一致
    /// 天地伟业返回的json数据格式样例
    ///[
    ///    {
    ///        "id": "25387a42-d3d1-478e-9df2-fe40caf8bcf5", 
    ///        "parentId": "null", 
    ///        "typeId": 1, 
    ///        "caption": "设备管理", 
    ///        "authority": [
    ///            1, 
    ///            2, 
    ///            3, 
    ///            12
    ///        ], 
    ///        "items": [
    ///            {
    ///                "id": "dev1", 
    ///                "parentId": "25387a42-d3d1-478e-9df2-fe40caf8bcf5", 
    ///                "typeId": 3, 
    ///                "caption": "dev1", 
    ///                "ip": "10.11.170.130", 
    ///                "port": 8000, 
    ///                "username": "", 
    ///                "password": "", 
    ///                "authority": [], 
    ///                "items": [
    ///                    {
    ///                        "id": "dd90adb5-3052-4039-960a-63b90d607745", 
    ///                        "parentId": "dev1", 
    ///                        "typeId": 4, 
    ///                        "caption": "10.11.170.102", 
    ///                        "hostType": 3, 
    ///                        "authority": [], 
    ///                        "items": [
    ///                            {
    ///                                "id": "e316db53-3e2a-4e3f-ac95-4e61445b85de", 
    ///                                "parentId": "dd90adb5-3052-4039-960a-63b90d607745", 
    ///                                "typeId": 5, 
    ///                                "caption": "通道1", 
    ///                                "ch": 0, 
    ///                                "description": "", 
    ///                                "authority": [], 
    ///                                "items": [ ]
    ///                            }
    ///                        ]
    ///                    }, 
    ///                    {
    ///                        "id": "e0af0c12-cce6-4cf4-aee1-123251db6cb4", 
    ///                        "parentId": "dev1", 
    ///                        "typeId": 4, 
    ///                        "caption": "192.168.1.2", 
    ///                        "hostType": 3, 
    ///                        "authority": [], 
    ///                        "items": []
    ///                    }
    ///                ]
    ///            }
    ///        ]
    ///    }
    ///]
    /// </summary>
    /// 
    [DataContract]
    internal class Resource
    {
        /// <summary>
        /// 树节点id
        /// </summary>
        [DataMember(Name = "id")]
        public string Id { get; set; }

        /// <summary>
        /// 父节点id
        /// </summary>
        [DataMember(Name = "parentId")]
        public string ParentId { get; set; }

        /// <summary>
        /// 节点类型，针对可视化调度台，5表示摄像头，1000表示自定义组
        /// </summary>
        [DataMember(Name = "typeId")]
        public string TypeId { get; set; }

        /// <summary>
        /// 节点名称
        /// </summary>
        [DataMember(Name = "caption")]
        public string Caption { get; set; }

        /// <summary>
        /// 权限类型
        /// </summary>
        [DataMember(Name = "authority")]
        public string[] Authority { get; set; }

        /// <summary>
        /// IP地址
        /// </summary>
        [DataMember(Name = "ip")]
        public string Ip { get; set; }

        /// <summary>
        /// 端口
        /// </summary>
        [DataMember(Name = "port")]
        public int Port { get; set; }

        /// <summary>
        /// 用户名
        /// </summary>
        [DataMember(Name = "username")]
        public string Username { get; set; }

        /// <summary>
        /// 密码
        /// </summary>
        [DataMember(Name = "password")]
        public string Pwd { get; set; }

        /// <summary>
        /// 主机类型
        /// </summary>
        [DataMember(Name = "hostType")]
        public string HostType { get; set; }

        /// <summary>
        /// 通道号
        /// </summary>
        [DataMember(Name = "ch")]
        public int Ch { get; set; }

        /// <summary>
        /// 描述
        /// </summary>
        [DataMember(Name = "description")]
        public string Description { get; set; }

        /// <summary>
        /// 子节点
        /// </summary>
        [DataMember(Name = "items")]
        public List<Resource> items { get; set; }
    }
}
