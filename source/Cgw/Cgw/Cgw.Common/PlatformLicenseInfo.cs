/*-----------------------------------------------------------------------
//文件名：PlatformLicenseInfo.cs
//版权：Copyright 2011-2012 Huawei Tech. Co. Ltd. All Rights Reserved. 
//作者：w00206574
//日期：2014-8-06
//描述：监控平台和应急平台的权限
//---------------------------------------------------------------------*/
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Cgw.Common
{
    public class PlatformLicenseInfo
    {
        /// <summary>
        /// 是否有监控平台权限
        /// </summary>
        private bool isMonitorPlatform = false;
        public bool IsMonitorPlatform
        {
            get { return isMonitorPlatform; }
            set { isMonitorPlatform = value; }
        }
        /// <summary>
        /// 是否有应急平台权限
        /// </summary>
        private bool isEltePlatform = false;
        public bool IsEltePlatform
        {
            get { return isEltePlatform; }
            set { isEltePlatform = value; }
        }
    }
    public enum PlatformType
    {
        MONITORPLATFORM = 1,               /*监控平台 */
        ELTEPLATFORM = 2,               /* Elte监控平台 */
    }
}
