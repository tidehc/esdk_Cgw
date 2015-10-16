using System;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.Configuration.Install;
using System.Linq;
using System.ServiceProcess;

namespace Cgw.Service
{
    [RunInstaller(true)]
    public partial class CgwInstall : Installer
    {

        /// <summary>
        /// 
        /// </summary>
        private ServiceProcessInstaller process;

        /// <summary>
        /// 
        /// </summary>
        private ServiceInstaller service;
        public CgwInstall()
        {
            InitializeComponent();
            process = new ServiceProcessInstaller();
            process.Account = ServiceAccount.LocalSystem;
            service = new ServiceInstaller();
            //windows服务的名字。运行在windows服务下，不考虑在同一台电脑上有主从服务器，console模式可以使用。
            service.ServiceName = "HUAWEI SMC 2.0 ConvergeGateway";
            service.StartType = ServiceStartMode.Automatic;
            Installers.Add(process);
            Installers.Add(service);
        }
    }
}
