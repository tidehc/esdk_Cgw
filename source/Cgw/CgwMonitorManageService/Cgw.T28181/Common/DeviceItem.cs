/*-----------------------------------------------------------------------
//文件名：DeviceItem.cs
//版权：Copyright 2011-2012 Huawei Tech. Co. Ltd. All Rights Reserved. 
//作者：w00206574
//日期：2014-3-06
//描述：T28181查询设备列表返回的设备类，用于保存设备列表、解析返回的XML数据
//---------------------------------------------------------------------*/
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml.Serialization;

namespace CgwMonitorManage.T28181
{
    [Serializable]
    [XmlRoot("Item")]
    public class DeviceItem
    {
        /// <summary>
        /// 设备/区域/系统编码
        /// </summary>
        [XmlElement(ElementName = "DeviceID")]
        public string DeviceID { get; set; }

        /// <summary>
        /// 设备/区域/系统名称
        /// </summary>
        [XmlElement(ElementName = "Name")]
        public string Name { get; set; }

        /// <summary>
        /// 当为设备时，设备厂商
        /// </summary>
        [XmlElement(ElementName = "Manufacturer")]
        public string Manufacturer { get; set; }

        /// <summary>
        /// 当为设备时，设备型号
        /// </summary>
        [XmlElement(ElementName = "Model")]
        public string Model { get; set; }

        /// <summary>
        /// 当为设备时，设备归属。默认值：HuaWei
        /// </summary>
        [XmlElement(ElementName = "Owner")]
        public string Owner { get; set; }

        /// <summary>
        /// 行政区域，资源编码的前六位
        /// </summary>
        [XmlElement(ElementName = "CivilCode")]
        public string CivilCode { get; set; }

        /// <summary>
        /// 区
        /// </summary>
        [XmlIgnore]
        public string Block { get; set; }

        /// <summary>
        /// 当为设备时，安装地址（必选）
        /// </summary>
        [XmlElement(ElementName = "Address")]
        public string Address { get; set; }

        /// <summary>
        /// 当为设备时，是否有子设备（必选）1有，0没有。默认都是0
        /// </summary>
        [XmlElement(ElementName = "Parental")]
        public string Parental { get; set; }

        /// <summary>
        /// 父设备/区域/系统ID
        /// </summary>
        [XmlElement(ElementName = "ParentID")]
        public string ParentID { get; set; }

        /// <summary>
        /// 信令安全模式（可选）缺省为0；  0：不采用；2：S/MIME签名方式；3：S/MIME加密签名同时采用方式；4：数字摘要方式。默认值：0
        /// </summary>
        [XmlElement(ElementName = "SafetyWay")]
        public string SafetyWay { get; set; }

        /// <summary>
        /// （必选）缺省为1； 1： 符合sip3261标准的认证注册模式；2： 基于口令的双向认证注册模式；3： 基于数字证书的双向认证注册模式。默认值：1
        /// </summary>
        [XmlElement(ElementName = "RegisterWay")]
        public string RegisterWay { get; set; }

        /// <summary>
        /// 证书序列号（有证书的设备必选）
        /// </summary>
        [XmlIgnore]
        public string CertNum { get; set; }

        /// <summary>
        /// 证书有效标识（有证书的设备必选）
        /// </summary>
        [XmlIgnore]
        public string Certifiable { get; set; }

        /// <summary>
        /// 无效原因码（有证书且证书无效的设备必选）
        /// </summary>
        [XmlIgnore]
        public string ErrCode { get; set; }

        /// <summary>
        /// 证书终止有效期（有证书的设备必选）
        /// </summary>
        [XmlIgnore]
        public string EndTime { get; set; }

        /// <summary>
        /// 保密属性（必选）缺省为0；0：不涉密，1：涉密。默认值：0
        /// </summary>
        [XmlElement(ElementName = "Secrecy")]
        public string Secrecy { get; set; }

        /// <summary>
        /// IP地址
        /// </summary>
        [XmlElement(ElementName = "IPAddress")]
        public string IPAddress { get; set; }

        /// <summary>
        /// 端口
        /// </summary>
        [XmlElement(ElementName = "Port")]
        public string Port { get; set; }

        /// <summary>
        /// 设备口令
        /// </summary>
        //[XmlElement(ElementName = "Password")]
        //public string Password { get; set; }

        /// <summary>
        /// 设备状态（必选），ON，OFF
        /// </summary>
        [XmlElement(ElementName = "Status")]
        public string Status { get; set; }

        /// <summary>
        /// 经度
        /// </summary>
        [XmlElement(ElementName = "Longitude")]
        public string Longitude { get; set; }

        /// <summary>
        /// 维度
        /// </summary>
        [XmlElement(ElementName = "Latitude")]
        public string Latitude { get; set; }

        /// <summary>
        /// LoginName
        /// </summary>
        [XmlElement(ElementName = "LoginName")]
        public string LoginName { get; set; }

        /// <summary>
        /// ChannelNO
        /// </summary>
        [XmlElement(ElementName = "ChannelNO")]
        public string ChannelNO { get; set; }
    }

    /// <summary>
    /// 设备比较器,相同的ID为同一设备 
    /// </summary>
    public class DeviceItemCompare : IEqualityComparer<DeviceItem>
    {
        public bool Equals(DeviceItem x, DeviceItem y)
        {
            return x.DeviceID == y.DeviceID;
        }

        public int GetHashCode(DeviceItem obj)
        {
            return obj.ToString().GetHashCode();
        }
    }
}

