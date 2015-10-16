using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Linq;
using System.ServiceProcess;
using System.Text;

namespace CgwMonitorManage.Common
{
   
       /// <summary>
    /// 本类为SMC服务寄宿在windows服务时运行时的基类
    /// </summary>
    public class ServiceRun : ServiceBase
    {
        /// <summary>
        /// 服务初始化（服务运行所需要的配置信息）
        /// </summary>
        private ServiceInit SmcInit;

        /// <summary>
        /// 构造函数
        /// </summary>
        /// <param name="init">服务初始化类</param>
        /// <param name="Name">服务的名字</param>
        public ServiceRun(ServiceInit init, string Name)
        {
            this.SmcInit = init;
            this.InitializeComponent(Name);
        }

        /// <summary>
        /// 开始运行服务
        /// </summary>
        /// <param name="args">args</param>
        /// <param name="serviceName">服务名</param>
        /// <param name="serviceInit">服务初始化</param>
        public static void StarUpService(string args, string serviceName, ServiceInit serviceInit)
        {
            if (args == "0")
            {
                //运行在console方式下
                serviceInit.Start();
            }
            else if (args == "1")
            {
                //运行在windows服务方式下
                ServiceBase.Run(new ServiceRun(serviceInit, serviceName));
            }
            else
            {
                Debug.Assert(false);

                //运行在windows服务方式下
                ServiceBase.Run(new ServiceRun(serviceInit, serviceName));
            }
        }

        /// <summary>
        /// 重载windows服务的启动函数，调用服务启动类的Start函数
        /// </summary>
        /// <param name="args">args</param>
        protected override void OnStart(string[] args)
        {
            //启动函数在这里，不要在这里进行任何数据库连接或其他动作。应该直接创建一个新的线程运行我们“自己的主线程”。因为OnStart有30秒的要求
            if (this.SmcInit != null)
            {
                this.SmcInit.Start();
            }
        }

        /// <summary>
        /// 重载windows服务停止函数，调用服务启动类的Stop函数
        /// </summary>
        protected override void OnStop()
        {
            //在这里通知系统退出。注意，还是有30秒的要求。应该均匀分配各个退出场景需要的时间。如果30秒实在不够用，可以调用RequestAdditionalTime这个函数获得更长点的时间。不过不能太长或不能使用动态增长方式，否则最后会导致服务无法停止。
            if (this.SmcInit != null)
            {
                this.SmcInit.Stop();
            }
        }

        /// <summary>
        /// 服务运行的初始化
        /// </summary>
        /// <param name="serviceName">服务的名字</param>
        private void InitializeComponent(string serviceName)
        {
            // Initialize the operating properties for the service.
            this.CanPauseAndContinue = false;
            this.CanShutdown = true;
            this.CanHandleSessionChangeEvent = true;
            this.ServiceName = serviceName;
        }
    }
}
