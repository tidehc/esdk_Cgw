/*-----------------------------------------------------------------------
//文件名：Program.cs
//版权：Copyright 2011-2012 Huawei Tech. Co. Ltd. All Rights Reserved. 
//作者：w00206574
//日期：2014-6-12
//描述：用于启动监控平台接入、管理服务 
//---------------------------------------------------------------------*/
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.ServiceModel;
using System.ServiceModel.Description;
using System.Threading;
using System.ServiceProcess;
using System.IO;
using System.Security.Cryptography;
using CgwMonitorManage.SmcError;
using CgwMonitorManage.Common;

namespace CgwMonitorManage.Service
{
    class Program
    {
        /// <summary>
        /// 日志
        /// </summary>
        private static NLog.Logger log = NLog.LogManager.GetCurrentClassLogger();

        /// <summary>
        /// 服务启动Main函数
        /// </summary>
        /// <param name="args"></param>
        static void Main(string[] args)
        {
            Thread th = new Thread(new ThreadStart(() =>
            {
                StartService();
            }));
            th.Priority = ThreadPriority.Highest;
            th.Start();
            th.Join();
        }

        /// <summary>
        /// 开启服务函数
        /// </summary>
        static void StartService()
        {
            MonitorManageService inist = MonitorManageService.Instance();
            ServiceHost host = new ServiceHost(inist);
            inist.SetHost(host);
            host.Opened += delegate
            {
                NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
                Thread dbConnectionThread = new Thread(new ThreadStart(LoadMonitorTimer_Elapsed));
                dbConnectionThread.Start();
                logEx.Info("Enter: Start CgwMonitorManage.Service");
            };
            host.Open();
            ServiceBase.Run(inist);
        }
        /// <summary>
        /// 线程加载监控平台
        /// </summary>
        private static void LoadMonitorTimer_Elapsed()
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            System.Threading.Thread.Sleep(CgwConst.Thread_Sleep_Time);
            SmcErr smcErr = new SmcErr();
            try
            {

                smcErr = MonitorManageService.Instance().Load();
                if (false == smcErr.IsSuccess())
                {
                    logEx.Error("MonitorManageService Start failed! Errno :{0}", smcErr.ErrNo);
                    NLog.LogManager.Flush();
                    System.Diagnostics.Process.GetCurrentProcess().Kill();
                }
            }
            catch (Exception ex)
            {
                logEx.Fatal(ex, "MonitorManageService LoadMonitor Failed!");
                NLog.LogManager.Flush();
            }

        }
    }
}
