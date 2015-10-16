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

namespace CgwMonitorManage.Common
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
        /// 增加管道、成功返回管道，失败返回空
        /// </summary>
        /// <param name="cameraNO">摄像机编码</param>
        public VideoPipe AddVideoPipe(string cameraNO)
        {
            if (listVideoPipeOperateLock.TryEnterWriteLock(CgwConst.ENTER_PIPE_LOCK_WAIT_TIME))
            {
                try
                {
                    int index = listVideoPipe.FindIndex((x) => { return x.CameraNo == cameraNO ? true : false; });

                    if (index == -1)
                    {
                        string pipeName = GetVideoPipeName(cameraNO);
                        VideoPipe pipe = new VideoPipe(pipeName);
                        pipe.PipeName = pipeName;
                        pipe.CameraNo = cameraNO;

                        if (pipeName != string.Empty)
                        {
                            listVideoPipe.Add(pipe);
                            return pipe;
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

        /// <summary>
        /// 获取管道名称 
        /// </summary>
        /// <returns></returns>
        static private string GetVideoPipeName(string cameraNO)
        {
            return cameraNO + System.DateTime.Now.ToString("yyyyMMddhhmmss");
        }
    }
}
