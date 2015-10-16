using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;

namespace CgwMonitorManage.Common
{
    /// <summary>
    /// eLTE设备播放时，播放超时等待的信号量
    /// </summary>
    public class eLTEAutoReset
    {
        /// <summary>
        /// 信号量
        /// </summary>
        public AutoResetEvent eLTEVideoEvent { get; set; }

        /// <summary>
        /// true：播放视频成功，false：播放视频失败
        /// </summary>
        public bool VideoSuccess { get; set; }
    }
}
