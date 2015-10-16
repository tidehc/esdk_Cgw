/*-----------------------------------------------------------------------
//文件名：PtzCommand.cs
//版权：Copyright 2011-2012 Huawei Tech. Co. Ltd. All Rights Reserved. 
//作者：kf70422
//日期：2013-2-25
//描述：云台控制命令
//---------------------------------------------------------------------*/
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.ServiceModel;
using System.Runtime.Serialization;

namespace CgwMonitorManage.Common
{
    [DataContract]
    public enum PtzCommandType
    {
        [EnumMember]
        PTZ_CMD_ZOOM_IN = 1,               /* 焦距以变大(倍率变大) */
        [EnumMember]
        PTZ_CMD_ZOOM_OUT = 2,              /* 焦距变小(倍率变小) */
        [EnumMember]
        PTZ_CMD_FOCUS_IN = 3,              /* 焦点前调 */
        [EnumMember]
        PTZ_CMD_FOCUS_OUT = 4,             /* 焦点后调 */
        [EnumMember]
        PTZ_CMD_IRIS_ENLARGE = 5,          /* 光圈扩大 */
        [EnumMember]
        PTZ_CMD_IRIS_SHRINK = 6,           /* 光圈缩小 */
        [EnumMember]
        PTZ_CMD_UP = 7,                    /* 云台向上 */
        [EnumMember]
        PTZ_CMD_DOWN = 8,                  /* 云台向下 */
        [EnumMember]
        PTZ_CMD_LEFT = 9,                  /* 云台左转 */
        [EnumMember]
        PTZ_CMD_RIGHT = 10,                /* 云台右转 */
    }
}
