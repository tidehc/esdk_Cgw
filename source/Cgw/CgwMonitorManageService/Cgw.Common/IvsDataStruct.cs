/*-----------------------------------------------------------------------
//文件名：IvsDataStruct.cs
//版权：Copyright 2011-2012 Huawei Tech. Co. Ltd. All Rights Reserved. 
//作者：kf70422
//日期：2013-4-8
//描述：IVS监控平台是用的数据结构
//---------------------------------------------------------------------*/
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;

namespace CgwMonitorManage.Common
{
    /// <summary>
    /// IVS 摄像头信息
    /// </summary>
    public class IvsCamera
    {

        /// <summary>
        /// 摄像头编号
        /// </summary>
        public string No { get; set; }

        /// <summary>
        /// 摄像头名称
        /// </summary>
        public string Name { get; set; }

        public IvsCameraType Type { get; set; }

        /// <summary>
        /// 所属设备组编码
        /// </summary>
        public string GroupNo { get; set; }

        /// <summary>
        /// 父设备编码
        /// </summary>
        public string ParentNo { get; set; }

        /// <summary>
        /// 域编码
        /// </summary>
        public string DomainCode { get; set; }

        /// <summary>
        /// 摄像头状态 在线/不在线
        /// </summary>
        public CameraStatus Status { get; set; }

    }

    /// <summary>
    /// 摄像头类型
    /// </summary>
    public enum IvsCameraType
    {
        CAMERA_FIX = 0,         // 固定枪机 
        CAMERA_PTZ = 1,         // 有云台枪机 
        CAMERA_DOME = 2,        // 球机
        CAMERA_FIX_DOME = 3,    // 半球-固定摄像机
    }

    /// <summary>
    /// IVS域信息
    /// </summary>
    public class IvsDomainRoute
    {
        /// <summary>
        /// 域编码
        /// </summary>
        public string DomainCode { get; set; }

        /// <summary>
        /// 域名称
        /// </summary>
        public string DomainName { get; set; }

        /// <summary>
        /// 域IP
        /// </summary>
        public string Ip { get; set; }

        /// <summary>
        /// 域端口
        /// </summary>
        public int Port { get; set; }

        /// <summary>
        /// 上级域编码
        /// </summary>
        public string ParentDomain { get; set; }
    }

    /// <summary>
    /// IVS分组信息
    /// </summary>
    public class IvsCameraGroup
    {
        /// <summary>
        /// 设备组编码
        /// </summary>
        public string GroupNo { get; set; }
        /// <summary>
        /// 设备组名称
        /// </summary>
        public string GroupName { get; set; }

        /// <summary>
        /// 父设备组编码
        /// </summary>
        public string ParentGroupNo { get; set; }

        /// <summary>
        /// 域编码
        /// </summary>
        public string DomainCode { get; set; }
    }

    /// <summary>
    /// IVS登陆信息
    /// </summary>
    public class IvsLoginInfo
    {
        /// <summary>
        /// IP地址
        /// </summary>
        public string Ip { get; set; }

        /// <summary>
        /// 端口
        /// </summary>
        public int Port { get; set; }

        /// <summary>
        /// 用户
        /// </summary>
        public string User { get; set; }

        /// <summary>
        /// 密码
        /// </summary>
        public string Pwd { get; set; }

    }

    /// <summary>
    /// 裸码流帧信息，跟IVS侧数据结构IVS_RAW_FRAME_INFO保持一致
    /// </summary>
    public class IvsRawFrameInfo
    {
        public int StreamType { get; set; }	         // 编码格式，参考：IvsDataType
        public int FrameType { get; set; }	         // 帧数据类型，SPS、PPS、IDR、P（视频数据有效）
        public int TimeStamp { get; set; }		     // 时间戳
        public UInt64 TimeTick { get; set; }	     // 绝对时间戳
        public int WaterMarkValue { get; set; }      // 水印数据 ，0表示无水印数据（视频数据有效）
        public int Width { get; set; }		         // 视屏宽度
        public int Height { get; set; }		         // 视频高度
        public int GopSequence { get; set; }		 // GOP序列
    }

    public enum IVS_USER_OFFLINE_TYPE
    {
        USER_OFFLINE_LOCK = 0,    // 账号被管理员锁定
        USER_OFFLINE_DELETE = 1,  // 账号被管理员删除
        USER_OFFLINE_SIGNOUT = 2, // 登录会话被管理员签退
    }

    public class IvsUserOfflineOff
    {
        public int iSessionID;     // 下线用户的SessionID
        public int uiOfflineType;  // 下线类型，值参考 IVS_USER_OFFLINE_TYPE
    }

    public delegate void IvsRealPlayCallBackRawFunc(int ulHandle, IvsRawFrameInfo pRawFrameInfo, byte[] pBuf, UInt32 uiBufSize, string pUserData);

    /// <summary>
    /// 事件回调函数(事件通知，异常回调等)
    /// </summary>
    /// <param name="ulHandle"></param>
    /// <param name="pRawFrameInfo"></param>
    /// <param name="pBuf"></param>
    /// <param name="uiBufSize"></param>
    /// <param name="pUserData"></param>
    public delegate void IvsEventCallBackFunc(int iEventType, IvsUserOfflineOff pEventBuf, UInt32 uiBufSize, string pUserData);

    /// <summary>
    /// 云镜控制码,值跟IVS侧IVS_PTZ_CODE枚举保持一致，如IVS侧发生变化，需要同步刷新
    /// 为保持融合网关的一致性，将枚举名前缀PTZ_改成PTZ_CMD_，保持跟PtzCommandType一致，方便转换
    /// </summary>
    public enum IvsPtzCommandType
    {
        // 云台转动控制，包括停止、上、下、左、右等
        PTZ_CMD_STOP = 1,
        PTZ_CMD_UP,
        PTZ_CMD_DOWN,
        PTZ_CMD_LEFT,
        PTZ_CMD_UP_LEFT,
        PTZ_CMD_DOWN_LEFT,
        PTZ_CMD_RIGHT,
        PTZ_CMD_UP_RIGHT,
        PTZ_CMD_DOWN_RIGHT,
        PTZ_CMD_AUTO,
        PTZ_CMD_PREFAB_BIT_RUN,
        PTZ_CMD_CRUISE_RUN,
        PTZ_CMD_CRUISE_STOP,
        PTZ_CMD_MODE_CRUISE_RUN,
        PTZ_CMD_MODE_CRUISE_STOP,
        PTZ_CMD_MENU_OPEN,
        PTZ_CMD_MENU_EXIT,
        PTZ_CMD_MENU_ENTER,
        PTZ_CMD_FLIP,
        PTZ_CMD_START,
        // 镜头控制，包括光圈、缩放、聚焦
        //光圈扩大。PTZ_LENS_APERTURE_OPEN修改为PTZ_CMD_IRIS_ENLARGE
        PTZ_CMD_IRIS_ENLARGE,
        //光圈缩小。PTZ_LENS_APERTURE_OPEN修改为PTZ_CMD_IRIS_ENLARGE
        PTZ_CMD_IRIS_SHRINK,
        //PTZ_LENS_ZOOM_IN修改为PTZ_CMD_ZOOM_IN
        PTZ_CMD_ZOOM_IN,
        //PTZ_LENS_ZOOM_OUT修改为PTZ_CMD_ZOOM_OUT
        PTZ_CMD_ZOOM_OUT,
        //焦点前调。PTZ_LENS_FOCAL_NEAT修改为PTZ_CMD_FOCUS_IN，方便跟PtzCommandType转换
        PTZ_CMD_FOCUS_IN,
        //焦点后调。PTZ_LENS_FOCAL_FAR修改为PTZ_CMD_FOCUS_OUT，方便跟PtzCommandType转换
        PTZ_CMD_FOCUS_OUT,
        PTZ_CMD_AUX_OPEN,
        PTZ_CMD_AUX_STOP,
        // 模式
        PTZ_CMD_MODE_SET_START,
        PTZ_CMD_MODE_SET_STOP,
        // 增强操作
        PTZ_CMD_FAST_LOCATE,
        PTZ_CMD_HORIZONTAL_SCAN,
        PTZ_CMD_VERTICAL_SCAN,
        PTZ_CMD_LOCK,
        PTZ_CMD_UNLOCK
    }

    /// <summary>
    /// 数据类型枚举跟IVS侧枚举IVS_PAYLOAD_TYPE保持一致，需要根据IVS侧的变化同步刷新
    /// </summary>
    public enum IvsStreamType
    {
        PAY_LOAD_NO_TYPE = -1,
        PAY_LOAD_TYPE_PCMU = 0,   // G711的u率
        PAY_LOAD_TYPE_PCM = 1,
        PAY_LOAD_TYPE_G723 = 4,   // G723
        PAY_LOAD_TYPE_PCMA = 8,   // G711的a率
        PAY_LOAD_TYPE_G722 = 9,   // G722

        PAY_LOAD_TYPE_G726 = 16,  // G726
        PAY_LOAD_TYPE_AAC = 17,  // AAC

        PAY_LOAD_TYPE_MJPEG = 26,  // MJPEG
        PAY_LOAD_TYPE_H264_1 = 96,  // H264(非标准定义)
        PAY_LOAD_TYPE_AMR_MB = 97,  // AMR_NB
        PAY_LOAD_TYPE_MPEG4 = 98,  // MPEG4
        PAY_LOAD_TYPE_H264 = 99,  // H264
        PAY_LOAD_TYPE_AVS = 100, // AVS
        PAY_LOAD_TYPE_MP3 = 101, // MP3
        PAY_LOAD_TYPE_3GPP = 102,  // 3GPP
        PAY_LOAD_TYPE_TRACK = 107,  // 轨迹
    }

    /// <summary>
    /// H264帧类型枚举跟IVS侧枚举H264_NALU_TYPE保持一致，需要根据IVS侧的变化同步刷新
    /// </summary>
    public enum IvsH264NaluType
    {
        H264_NALU_TYPE_UNDEFINED = 0,
        H264_NALU_TYPE_SLICE = 1,
        H264_NALU_TYPE_DPA = 2,
        H264_NALU_TYPE_DPB = 3,
        H264_NALU_TYPE_DPC = 4,
        H264_IDR_NALU_TYPE = 5,
        H264_SEI_NALU_TYPE = 6,
        H264_SPS_NALU_TYPE = 7,
        H264_PPS_NALU_TYPE = 8,
        H264_NALU_TYPE_AUD = 9,
        H264_NALU_TYPE_EOSEQ = 10,
        H264_NALU_TYPE_EOSTREAM = 11,
        H264_NALU_TYPE_FILL = 12,

        H264_NALU_TYPE_STAP_A = 24,
        H264_NALU_TYPE_STAP_B = 25,
        H264_NALU_TYPE_MTAP16 = 26,
        H264_NALU_TYPE_MTAP24 = 27,
        H264_NALU_TYPE_FU_A = 28,
        H264_NALU_TYPE_FU_B = 29,
        H264_NALU_TYPE_END = 30
    }
}
