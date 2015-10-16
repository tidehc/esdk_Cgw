using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Cgw.SmcError;
using Cgw.Common;
using System.ServiceModel;
using System.Threading;

namespace Cgw.Service
{
    public class CgwServiceInit : ServiceInit
    {


        /// <summary>
        /// 用户管理服务端口
        /// </summary>
        private ServiceHost serviceHost;

        /// <summary>
        /// 
        /// </summary>
        public CgwServiceInit()
        {
        }

        /// <summary>
        /// 停止服务
        /// </summary>
        public override void StopService()
        {
            if (serviceHost != null)
            {
                NLog.Logger log = NLog.LogManager.GetCurrentClassLogger();
                NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);

                CgwService.Instance().StopAllStream();//stop all stream.
                SmcErr err = CgwService.Instance().DisconnectMonitor();
                logEx.Info("CGW service.StopService DisconnectMonitor.ErrNo ={0}!", err.ErrNo);

                serviceHost.Close();
                logEx.Info("CGW service.StopService Close Success.");
            }
        }

        /// <summary>
        /// 启动本服务的业务服务与接口服务
        /// </summary>
        public override void RunService()
        {
            //
            serviceHost.Open();

            mainThread.Join();
        }

        /// <summary>
        /// 从脚本里读取本服务的接口参数以及注册服务器的接口参数
        /// </summary>
        /// <param name="configName">脚本名字</param>
        /// <returns>windows服务的名字</returns>
        public override void InitServiceInfo()
        {

            serviceHost = new ServiceHost(CgwService.Instance());
        }
    }

    class Program
    {
        /// <summary>
        /// 记录日志操作类.
        /// </summary>
        public static NLog.Logger log = NLog.LogManager.GetCurrentClassLogger();

        static void Main(string[] args)
        {
            SmcErr smcErr = new SmcErr();
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            try
            {
                logEx.Info("Enter: Start Cgw Service");
                CgwServiceInit cgwService = new CgwServiceInit();
                smcErr = CgwService.Instance().InitService();
                if (smcErr.IsSuccess())
                {

                    cgwService.InitServiceInfo();
                    string LocalServiceStartMode = CommonFunction.GetAppSetting("LocalServiceStartMode");

                    if (string.IsNullOrEmpty(LocalServiceStartMode))
                    {
                        LocalServiceStartMode = "1";
                    }
                    CgwService.Instance().LicenseInit();
                    Thread dbConnectionThread = new Thread(new ThreadStart(LoadMonitorTimer_Elapsed));
                    dbConnectionThread.Start();

                    ServiceRun.StarUpService(LocalServiceStartMode, "HUAWEI SMC 2.0 ConvergeGateway", cgwService);
                    //logEx.Info("CGW service Start Successful!");
                }
                else
                {
                    logEx.Error("CGW service Start failed! Errno :{0}", smcErr.ErrNo);
                    NLog.LogManager.Flush();
                    System.Diagnostics.Process.GetCurrentProcess().Kill();

                }

            }
            catch (System.Exception e)
            {
                logEx.Fatal(e, "CGW service Start Failed!");
                NLog.LogManager.Flush();
            }
        }

        /// <summary>
        /// 线程加载监控平台
        /// </summary>
        private static void LoadMonitorTimer_Elapsed()
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            System.Threading.Thread.Sleep(1000);
            SmcErr smcErr = new SmcErr();
            try
            {
                smcErr = CgwService.Instance().Load();
                if (false == smcErr.IsSuccess())
                {
                    logEx.Error("CGW service Start failed! Errno :{0}", smcErr.ErrNo);
                    NLog.LogManager.Flush();
                    System.Diagnostics.Process.GetCurrentProcess().Kill();
                }
                else
                {
                    logEx.Info("CGW service Start Success");
                }
            }
            catch (Exception ex)
            {
                logEx.Fatal(ex, "CGW service LoadMonitor Failed!");
                NLog.LogManager.Flush();
            }
        }
    }
}
