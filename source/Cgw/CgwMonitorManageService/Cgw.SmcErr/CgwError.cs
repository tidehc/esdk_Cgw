/*-----------------------------------------------------------------------
//文件名：CgwError.cs
//版权：Copyright 2011-2012 Huawei Tech. Co. Ltd. All Rights Reserved. 
//作者：lizhengping kf70422
//日期：2013-3-13
//描述：融合网关错误码
//---------------------------------------------------------------------*/
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;

namespace CgwMonitorManage.SmcError
{
    /// <summary>
    /// 融合网关错误码
    /// </summary>
    [DataContract]
    public class CgwError : SmcErr
    {
        /// <summary>
        /// 构造函数
        /// </summary>
        /// <param name="result">结果</param>
        public CgwError(uint result)
            : base(result)
        {
        }

        /// <summary>
        /// 构造函数
        /// </summary>
        public CgwError()
        {
        }

        /// <summary>
        /// 取模块名称
        /// </summary>
        /// <returns>模块名</returns>
        public override string GetModelName()
        {
            return this.GetType().Name;
        }

        #region 融合网关错误码
        //////////////////////////////////////////////////////////////////////////
        //融合网关错误码
        //////////////////////////////////////////////////////////////////////////

        /// <summary>
        /// 融合网关错误码0x90000000，待定义
        /// </summary>
        public const uint ERR_CGW_VIDEO_MONITOR = 0x70001000;

        /// <summary>
        /// 监控平台配置文件格式有问题
        /// </summary>
        public const uint MONITOR_LOAD_FAILED = ERR_CGW_VIDEO_MONITOR + 0x01;

        /// <summary>
        /// 监控平台卸载失败
        /// </summary>
        public const uint MONITOR_UDLOAD_FAILED = ERR_CGW_VIDEO_MONITOR + 0x02;

        /// <summary>
        /// 监控平台配置文件不存在
        /// </summary>
        public const uint MONITOR_CONFIG_FILE_NOT_EXIST = ERR_CGW_VIDEO_MONITOR + 0x03;


        /// <summary>
        /// 监控平台配置文件格式有问题
        /// </summary>
        public const uint MONITOR_CONFIG_FILE_INVALID = ERR_CGW_VIDEO_MONITOR + 0x04;

        /// <summary>
        /// 监控平台配置文件格式有问题,ID不能为空
        /// </summary>
        public const uint MONITOR_CONFIG_FILE_INVALID_ID_NULL = ERR_CGW_VIDEO_MONITOR + 0x05;

        /// <summary>
        /// 监控平台的id重复
        /// </summary>
        public const uint MONITOR_CONFIG_FILE_INVALID_ID_EXIST = ERR_CGW_VIDEO_MONITOR + 0x06;

        /// <summary>
        /// 监控平台 {0}的class不能为空
        /// </summary>
        public const uint MONITOR_CONFIG_FILE_INVALID_CLASS_NULL = ERR_CGW_VIDEO_MONITOR + 0x07;

        /// <summary>
        /// 无法加载监控平台 {0}的 class {1}。
        /// </summary>
        public const uint MONITOR_CONFIG_FILE_INVALID_CLASS_INVALID = ERR_CGW_VIDEO_MONITOR + 0x08;

        /// <summary>
        /// 监控平台配置文件，流媒体服务器ID不存在。
        /// </summary>
        public const uint MEDIA_SERVER_ID_NOT_FOUND = ERR_CGW_VIDEO_MONITOR + 0x09;

        /// <summary>
        /// 摄像头不存在。
        /// </summary>
        public const uint CAMERA_NOT_FOUND = ERR_CGW_VIDEO_MONITOR + 0x10;

        /// <summary>
        /// 监控平台ID不存在。
        /// </summary>
        public const uint MONITOR_NOT_FOUND = ERR_CGW_VIDEO_MONITOR + 0x11;

        /// <summary>
        /// 加载监控平台失败
        /// </summary>
        public const uint LOAD_MONITOR_FAILED = ERR_CGW_VIDEO_MONITOR + 0x12;

        /// <summary>
        /// 卸载监控平台失败
        /// </summary>
        public const uint UNLOAD_MONITOR_FAILED = ERR_CGW_VIDEO_MONITOR + 0x13;


        /// <summary>
        /// 获取摄像头列表失败
        /// </summary>
        public const uint GET_ALL_CAMERAS_FAILED = ERR_CGW_VIDEO_MONITOR + 0x14;

        /// <summary>
        /// 启动接收码流失败
        /// </summary>
        public const uint START_RECEIVE_VIDEO_FAILED = ERR_CGW_VIDEO_MONITOR + 0x15;

        /// <summary>
        /// 停止接收码流失败
        /// </summary>
        public const uint STOP_RECEIVE_VIDEO_FAILED = ERR_CGW_VIDEO_MONITOR + 0x16;

        /// <summary>
        /// 开始控制ptz失败
        /// </summary>
        public const uint START_CONTROL_PTZ_FAILED = ERR_CGW_VIDEO_MONITOR + 0x17;

        /// <summary>
        /// 停止控制ptz失败
        /// </summary>
        public const uint STOP_CONTROL_PTZ_FAILED = ERR_CGW_VIDEO_MONITOR + 0x18;


        /// <summary>
        /// 开始控制ptz失败
        /// </summary>
        public const uint MAKE_I_FRAME_FAILED = ERR_CGW_VIDEO_MONITOR + 0x19;

        /// <summary>
        /// 监控管理服务正在尝试恢复
        /// </summary>
        public const uint ERR_MONITOR_MANAGE_SERVICE_RESTARTING = ERR_CGW_VIDEO_MONITOR + 0x20;

        /// <summary>
        /// 获取刷新状态 - 监控和elte都刷新完毕
        /// </summary>
        public const uint ERR_DEVICE_LIST_REFRESH_STATUS_END = ERR_CGW_VIDEO_MONITOR + 0x21;

        /// <summary>
        /// 获取刷新状态 - 监控和elte都刷新执行中
        /// </summary>
        public const uint ERR_DEVICE_LIST_REFRESH_STATUS_EXECUTING = ERR_CGW_VIDEO_MONITOR + 0x22;

        /// <summary>
        /// 获取刷新状态 - 监控和elte都刷新失败
        /// </summary>
        public const uint ERR_DEVICE_LIST_REFRESH_STATUS_FAILED = ERR_CGW_VIDEO_MONITOR + 0x23;

        /// <summary>
        /// 刷新设备列表失败
        /// </summary>
        public const uint ERR_REFRESH_MONITOR_CAMERA_FAIL = ERR_CGW_VIDEO_MONITOR + 0x24;

        /// <summary>
        /// 获取刷新状态 - 监控成功、elte失败 
        /// </summary>
        public const uint ERR_DEVICE_LIST_REFRESH_STATUS_MONITOR_SUCCESS_ELTE_FAIL = ERR_CGW_VIDEO_MONITOR + 0x27;

        /// <summary>
        /// 获取刷新状态 - 监控成功、elte执行中 
        /// </summary>
        public const uint ERR_DEVICE_LIST_REFRESH_STATUS_MONITOR_SUCCESS_ELTE_EXECUTING = ERR_CGW_VIDEO_MONITOR + 0x28;

        /// <summary>
        /// 获取刷新状态 - 监控执行中、elte成功 
        /// </summary>
        public const uint ERR_DEVICE_LIST_REFRESH_STATUS_MONITOR_EXECUTING_ELTE_SUCCESS = ERR_CGW_VIDEO_MONITOR + 0x29;

        /// <summary>
        /// 获取刷新状态 - 监控执行中、elte失败 
        /// </summary>
        public const uint ERR_DEVICE_LIST_REFRESH_STATUS_MONITOR_EXECUTING_ELTE_FAIL = ERR_CGW_VIDEO_MONITOR + 0x30;

        /// <summary>
        /// 获取刷新状态 - 监控失败、elte成功 
        /// </summary>
        public const uint ERR_DEVICE_LIST_REFRESH_STATUS_MONITOR_FAIL_ELTE_SUCCESS = ERR_CGW_VIDEO_MONITOR + 0x31;

        /// <summary>
        /// 获取刷新状态 - 监控失败、elte执行中 
        /// </summary>
        public const uint ERR_DEVICE_LIST_REFRESH_STATUS_MONITOR_FAIL_ELTE_EXECUTING = ERR_CGW_VIDEO_MONITOR + 0x32;


        public const uint ERR_CGW_BUSINESS = 0x70002000;

        /// <summary>
        /// 连接密码错误
        /// </summary>
        public const uint ERR_CGW_BUSINESS_PASSWORD_INCORRECT = ERR_CGW_BUSINESS + 0x01;

        /// <summary>
        /// 密码不能为空
        /// </summary>
        public const uint ERR_CGW_BUSINESS_PASSWORD_NULL = ERR_CGW_BUSINESS + 0x02;

        /// <summary>
        /// 融合网关中不存在此会话
        /// </summary>
        public const uint ERR_CGW_BUSINESS_SESSION_NULL = ERR_CGW_BUSINESS + 0x03;

        /// <summary>
        /// 当前会话为空
        /// </summary>
        public const uint ERR_CGW_BUSINESS_CURRENTSESSION_NULL = ERR_CGW_BUSINESS + 0x04;

        /// <summary>
        /// 当前会话个数超过最大限制
        /// </summary>
        public const uint ERR_CGW_BUSINESS_SESSION_COUNT_MAX = ERR_CGW_BUSINESS + 0x05;

        /// <summary>
        /// 监控平台加载失败
        /// </summary>
        public const uint ERR_CGW_BUSINESS_INIT_FAILED = ERR_CGW_BUSINESS + 0x06;

        /// <summary>
        /// 监控通道号不能为空
        /// </summary>
        public const uint ERR_CGW_BUSINESS_CHANNELNO_NULL = ERR_CGW_BUSINESS + 0x07;

        /// <summary>
        /// 监控通道列表不能为空
        /// </summary>
        public const uint ERR_CGW_BUSINESS_CHANNELLIST_NULL = ERR_CGW_BUSINESS + 0x08;
        /// <summary>
        /// License控制项改变
        /// </summary>
        public const uint ERR_CGW_LICENSE_CHANGE = ERR_CGW_BUSINESS + 0x09;

        /// <summary>
        /// 字符串格式不正确
        /// </summary>
        public const uint ERR_CGW_CONFIGURE_FORMAT = ERR_CGW_BUSINESS + 0x0A;

        /// <summary>
        /// 融合网关正在加载
        /// </summary>
        public const uint ERR_CGW_IS_LOADING = ERR_CGW_BUSINESS + 0x0B;

        #region 通道模块错误码
        /// <summary>
        /// 通道模块错误码
        /// </summary>
        public const uint ERR_CGW_CHANNEL = 0x70003000;

        /// <summary>
        /// 输入参数非法
        /// </summary>
        public const uint ERR_CGW_CHANNEL_INPUT_ERROR = ERR_CGW_CHANNEL + 0x01;

        /// <summary>
        /// 找不到指定的通道号码
        /// </summary>
        public const uint ERR_CGW_CHANNEL_NOT_FIND_CHANNELNO = ERR_CGW_CHANNEL + 0x02;

        /// <summary>
        /// 通道没有入会，不能关联摄像头
        /// </summary>
        public const uint ERR_CGW_CHANNEL_NOT_IN_CONF = ERR_CGW_CHANNEL + 0x03;

        /// <summary>
        /// 获取读写锁失败
        /// </summary>
        public const uint ERR_CGW_CHANNEL_GET_LOCK_FAIL = ERR_CGW_CHANNEL + 0x04;

        /// <summary>
        /// 通道模块服务异常
        /// </summary>
        public const uint ERR_CGW_CHANNEL_SERVICE_ADNORMAL = ERR_CGW_CHANNEL + 0x05;

        /// <summary>
        /// 监控通道不可用
        /// </summary>
        public const uint ERR_CGW_CHANNEL_CHANNEL_INVALID = ERR_CGW_CHANNEL + 0x06;

        #region 软终端
        /// <summary>
        /// 加载软终端dll失败
        /// </summary>
        public const uint ERR_CGW_CHANNEL_SOT_LOAD_FAIL = ERR_CGW_CHANNEL + 0x100;

        /// <summary>
        /// 设置软终端参数失败
        /// </summary>
        public const uint ERR_CGW_CHANNEL_SOT_SET_INITPARAM_FAIL = ERR_CGW_CHANNEL + 0x101;

        /// <summary>
        /// 初始化软终端失败
        /// </summary>
        public const uint ERR_CGW_CHANNEL_SOT_INIT_FAIL = ERR_CGW_CHANNEL + 0x102;

        /// <summary>
        /// 注册软终端回调失败
        /// </summary>
        public const uint ERR_CGW_CHANNEL_SOT_REGIST_CALLBACK_FAIL = ERR_CGW_CHANNEL + 0x103;

        /// <summary>
        /// 监控管理服务正确
        /// </summary>
        public const uint ERR_MONITOR_MANAGE_SERVICE_SUCCESS = 0x0;

        /// <summary>
        /// 监控管理服务保活失败
        /// </summary>
        public const uint ERR_MONITOR_MANAGE_SERVICE_KEEPLIVE_FAIL = ERR_CGW_CHANNEL + 0x104;

        #endregion
        #endregion

        #endregion
    }
}
