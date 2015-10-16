/*-----------------------------------------------------------------------
//文件名：VideoPipeManage.cs
//版权：Copyright 2011-2012 Huawei Tech. Co. Ltd. All Rights Reserved. 
//作者：w00206574
//日期：2014-6-12
//描述：实况管道管理类
//---------------------------------------------------------------------*/

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;

namespace Cgw.Common
{
    /// <summary>
    /// 实况管道管理类
    /// </summary>
    public class VideoPipeManage
    {
        /// <summary>
        /// 管道列表操作锁
        /// </summary>
        static private ReaderWriterLockSlim listVideoPipeOperateLock = new ReaderWriterLockSlim();

        /// <summary>
        /// 单实例
        /// </summary>
        private static VideoPipeManage videoPipeManage;

        /// <summary>
        /// 单例模式锁
        /// </summary>
        private static object instanceLocker = new object();

        private Common.DataExceptionCallBack dataExceptionCallBack;

        /// <summary>
        /// 记录日志操作类.
        /// </summary>
        private static NLog.Logger log = NLog.LogManager.GetCurrentClassLogger();

        /// <summary>
        /// 指定码流数据异常回调函数
        /// </summary>
        /// <param name="dataExceptionCallBack">回调函数</param>
        public void SetDataExceptionCallBackFunc(Common.DataExceptionCallBack dataExceptionCallBack)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Trace("Enter: CgwMonitorManageAdapter.SetDataExceptionCallBackFunc({0})", dataExceptionCallBack);

            this.dataExceptionCallBack = dataExceptionCallBack;
        }

        /// <summary>
        /// 系统所有存在的管道
        /// </summary>
        static List<VideoPipe> listVideoPipe = new List<VideoPipe>();
        static public List<VideoPipe> ListVideoPipe
        {
            get
            {
                if (listVideoPipeOperateLock.TryEnterWriteLock(CgwConst.ENTER_PIPE_LOCK_WAIT_TIME))
                {
                    try
                    {
                        return listVideoPipe;
                    }
                    finally
                    {
                        listVideoPipeOperateLock.ExitWriteLock();
                    }
                }
                else
                {
                    return null;
                }
            }
        }

        /// <summary>
        /// 私有构造函数
        /// </summary>
        private VideoPipeManage()
        {

        }

        /// <summary>
        /// 单实例
        /// </summary>
        /// <returns></returns>
        static public VideoPipeManage Instance()
        {
            if (videoPipeManage == null)
            {
                lock (instanceLocker)
                {
                    videoPipeManage = new VideoPipeManage();
                }
            }
            return videoPipeManage;
        }

        /// <summary>
        /// 删除管道
        /// </summary>
        /// <param name="cameraNO"></param>
        public VideoPipe DeleteVideoPipe(string cameraNO)
        {
            if (listVideoPipeOperateLock.TryEnterWriteLock(CgwConst.ENTER_PIPE_LOCK_WAIT_TIME))
            {
                try
                {
                    if (listVideoPipe.Exists((x) => { return x.CameraNo == cameraNO ? true : false; }))
                    {
                        VideoPipe videoPipe = listVideoPipe.Find((x) => { return x.CameraNo == cameraNO ? true : false; });

                        if (videoPipe != null)
                        {
                            listVideoPipe.Remove(videoPipe);
                            return videoPipe;
                        }
                    }
                }
                finally
                {
                    listVideoPipeOperateLock.ExitWriteLock();
                }
            }

            return null;
        }

        /// <summary>
        /// 删除管道
        /// </summary>
        /// <param name="cameraNO"></param>
        public VideoPipe DeleteNoDataVideoPipe(string cameraNO)
        {
            try
            {
                this.dataExceptionCallBack(cameraNO);
            }
            finally
            {

            }
            return null;
        }

        /// <summary>
        /// 增加管道 
        /// </summary>
        /// <param name="cameraNo">摄像机编码</param>
        /// <param name="pipeName">管道名称</param>
        /// <returns>VideoPipe</returns>
        public VideoPipe AddVideoPipe(string cameraNo, string pipeName)
        {
            if (listVideoPipeOperateLock.TryEnterWriteLock(CgwConst.ENTER_PIPE_LOCK_WAIT_TIME))
            {
                try
                {
                    int index = listVideoPipe.FindIndex((x) => { return x.PipeName == pipeName ? true : false; });

                    if (index == -1)
                    {
                        VideoPipe videoPipe = new VideoPipe(pipeName);

                        videoPipe.CameraNo = cameraNo;
                        listVideoPipe.Add(videoPipe);

                        return videoPipe;
                    }
                }
                finally
                {
                    listVideoPipeOperateLock.ExitWriteLock();
                }
            }
            return null;
        }

        /// <summary>
        /// 查询管道
        /// </summary>
        /// <param name="cameraNO">摄像机编码</param>
        /// <returns></returns>
        public VideoPipe GetVideoPipe(string cameraNO)
        {
            if (listVideoPipeOperateLock.TryEnterWriteLock(CgwConst.ENTER_PIPE_LOCK_WAIT_TIME))
            {
                try
                {
                    int index = listVideoPipe.FindIndex((x) => { return x.CameraNo == cameraNO ? true : false; });

                    if (index != -1)
                    {
                        VideoPipe videoPipe = listVideoPipe.Find((x) => { return x.CameraNo == cameraNO ? true : false; });
                        return videoPipe;
                    }
                }
                finally
                {
                    listVideoPipeOperateLock.ExitWriteLock();
                }
            }
            return null;

        }

    }
}
