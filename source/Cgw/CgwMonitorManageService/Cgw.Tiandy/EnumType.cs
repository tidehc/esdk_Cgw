/*-----------------------------------------------------------------------
//文件名：EnumType.cs
//版权：Copyright 2011-2012 Huawei Tech. Co. Ltd. All Rights Reserved. 
//作者：kf70422
//日期：2013-2-27
//描述：天地伟业监控平台使用的枚举类型定义
//---------------------------------------------------------------------*/
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace CgwMonitorManage.Tiandy
{
    /// <summary>
    /// 节点类型，对应天地伟业http接口返回的json数据中的TypeId
    /// </summary>
    internal enum NodeType
    {
        MEDIASERVER = 3,          /* 流媒体服务器 */
        HOST = 4,                 /* 主机 */
        CAMERA = 5,             /* 摄像头类型 */
        GROUP = 1000,           /* 自定义分组类型 */
    }

    /// <summary>
    /// 节点类型，对应天地伟业http接口返回的json数据中的TypeId
    /// </summary>
    internal enum TiandyDateType
    {
        //码流类型 0主码流 1子码流
        PRIMARY = 0,          /* 主码流 */
        CHILD = 1,             /* 子码流 */
    }

    internal enum TiandyPtzCommandType
    {
        PTZ_CMD_LIGHT_PWRON = 2,            /* 接通灯光电源 */
        PTZ_CMD_WIPER_PWRON = 3,            /* 接通雨刷开关 */
        PTZ_CMD_FAN_PWRON = 4,              /* 接通风扇开关 */
        PTZ_CMD_HEATER_PWRON = 5,           /* 接通加热器开关 */
        PTZ_CMD_AUX_PWRON = 6,              /* 接通辅助设备开关 */
        PTZ_CMD_ZOOM_IN = 11,               /* 焦距以变大(倍率变大) */
        PTZ_CMD_ZOOM_OUT = 12,              /* 焦距变小(倍率变小) */
        PTZ_CMD_FOCUS_IN = 13,              /* 焦点前调 */
        PTZ_CMD_FOCUS_OUT = 14,             /* 焦点后调 */
        PTZ_CMD_IRIS_ENLARGE = 15,          /* 光圈扩大 */
        PTZ_CMD_IRIS_SHRINK = 16,           /* 光圈缩小 */
        PTZ_CMD_UP = 21,                    /* 云台向上, */
        PTZ_CMD_DOWN = 22,                  /* 云台向下 */
        PTZ_CMD_LEFT = 23,                  /* 云台左转 */
        PTZ_CMD_RIGHT = 24,                 /* 云台右转 */
        PTZ_CMD_PAN_AUTO = 29,              /* 云台以SS 的速度左右自动扫描 */
        PTZ_CMD_SET_PRESET = 8,             /* 设置预置点 */
        PTZ_CMD_CLE_PRESET = 9,             /* 清除预置点 */
        PTZ_CMD_GOTO_PRESET = 39,           /* 转到预置点 */
        PTZ_CMD_LEFTUP = 25,                /* 左上 1001*/
        PTZ_CMD_RIGHTUP = 26,               /* 右上 1002*/
        PTZ_CMD_LEFTDOWN = 27,              /* 左上 1003*/
        PTZ_CMD_RIGHTDOWN = 28,             /* 右下 1004*/
        PTZ_CMD_LEFTUP_ZOOMOUT = 40,        /* 左上 变倍小*/
        PTZ_CMD_RIGHTUP_ZOOMOUT = 41,       /* 右上 变倍小*/
        PTZ_CMD_LEFTUP_ZOOMIN = 42,         /* 左上 变倍大*/
        PTZ_CMD_RIGHTUP_ZOOMIN = 43,        /* 右上 变倍大*/
        PTZ_CMD_LEFTDOWN_ZOOMOUT = 44,      /* 左下 变倍小*/
        PTZ_CMD_RIGHTDOWN_ZOOMOUT = 45,     /* 右下 变倍小*/
        PTZ_CMD_LEFTDOWN_ZOOMIN = 46,       /* 左下 变倍大*/
        PTZ_CMD_RIGHTDOWN_ZOOMIN = 47,      /* 右下 变倍大*/
        PTZ_CMD_LEFT_ZOOMOUT = 48,          /* 左 变倍小*/
        PTZ_CMD_RIGHT_ZOOMOUT = 49,         /* 右 变倍小*/
        PTZ_CMD_LEFT_ZOOMIN = 50,           /* 左 变倍大*/
        PTZ_CMD_RIGHT_ZOOMIN = 51,          /* 右 变倍大*/
        PTZ_CMD_UP_ZOOMOUT = 52,            /* 上 变倍小*/
        PTZ_CMD_DOWN_ZOOMOUT = 53,          /* 下 变倍小*/
        PTZ_CMD_UP_ZOOMIN = 54,             /* 上 变倍大*/
        PTZ_CMD_DOWN_ZOOMIN = 55,           /* 下 变倍大*/
        PTZ_CMD_3D = 1010,                  /* 3D定位 */
    }

    /// <summary>
    /// Tiandy码流类型
    /// </summary>
    internal enum TiandyStreamType
    {
        STREAM_TYPE_VIDEO_FRAME_I = 0,		   /* I 帧类型 */
        STREAM_TYPE_VIDEO_FRAME_P = 1,		   /* P帧类型  */
        STREAM_TYPE_VIDEO_FRAME_B = 2,		   /* B帧类型  */

        STREAM_TYPE_AUDIO_G711A = 6,
        STREAM_TYPE_AUDIO_G711U = 7,
        STREAM_TYPE_AUDIO_ADPCM_A = 8,
        STREAM_TYPE_AUDIO_G726 = 9,
        STREAM_TYPE_AUDIO_INFO = 10,           /* 采样率等音频信息*/
    }
}
