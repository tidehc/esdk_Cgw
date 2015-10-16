using System.Threading;
using System.Configuration;
using System.ServiceProcess;

namespace Cgw.Common
{
    /// <summary>
    /// 服务启动的基类。各个普通服务都需要继承该类并实现RunService抽象接口
    /// </summary>
    public abstract class ServiceInit
    {
        /// <summary>
        /// 服务启动的主线程
        /// </summary>
        protected Thread mainThread;

        /// <summary>
        /// 服务启动的构造函数
        /// </summary>
        public ServiceInit()
        {
            this.mainThread = new Thread(this.RunService);
        }

        /// <summary>
        /// 主线程运行函数
        /// </summary>
        public abstract void RunService();

        /// <summary>
        /// 停止线程服务
        /// </summary>
        public abstract void StopService();

        /// <summary>
        /// 初始化线程服务
        /// </summary>
        public abstract void InitServiceInfo();

        /// <summary>
        /// 线程的启动函数
        /// </summary>
        public void Start()
        {
            this.mainThread.Start();
        }

        /// <summary>
        /// 线程的停止函数
        /// </summary>
        public void Stop()
        {
            this.StopService();
            this.mainThread.Abort();
        }
    }
}
