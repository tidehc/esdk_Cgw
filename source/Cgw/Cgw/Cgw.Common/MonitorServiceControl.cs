/*-----------------------------------------------------------------------
//文件名：MonitorServiceControl.cs
//版权：Copyright 2011-2012 Huawei Tech. Co. Ltd. All Rights Reserved. 
//作者：jwx224908
//日期：2014-7-24
//描述：用于判断监控服务是否在运行；重启监控服务。
//---------------------------------------------------------------------*/

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.ServiceProcess;
using System.Timers;
using System.Threading;

namespace Cgw.Common
{
    public class MonitorServiceControl
    {
        public bool MonitorServiceRun()
        {
            ServiceController cs = new ServiceController();
            cs.ServiceName = "HUAWEI SMC 2.0 MonitorManage";
            cs.Refresh();

            if (cs.Status == ServiceControllerStatus.Running || cs.Status == ServiceControllerStatus.StartPending)
            {
                return true;
            }
            return false;
        }

        public void MonitorServiceStart()
        {
            ServiceController cs = new ServiceController();
            ServiceController cgw = new ServiceController();
            try
            {
                cs.ServiceName = "HUAWEI SMC 2.0 MonitorManage";
                cs.Refresh();

                cgw.ServiceName = "HUAWEI SMC 2.0 ConvergeGateway";
                cgw.Refresh();
                if (cgw.Status == ServiceControllerStatus.Running || cgw.Status == ServiceControllerStatus.StartPending)  //监控服务自启动的前提是CGW服务在线
                {
                    //if (cs.Status != ServiceControllerStatus.Running && cs.Status != ServiceControllerStatus.StartPending)
                    if (cs.Status == ServiceControllerStatus.Stopped)
                    {
                        //Thread.Sleep(1000);
                        TimeSpan timeout = TimeSpan.FromMilliseconds(CgwConst.WAIT_MONITOR_SERVICE_RUNNING_MILLI_SECONDS);
                        cs.Start();
                        cs.WaitForStatus(ServiceControllerStatus.Running, timeout);
                    }
                }
            }
            catch (System.Exception)
            {
                TimeSpan timeout = TimeSpan.FromMilliseconds(CgwConst.WAIT_MONITOR_SERVICE_RUNNING_MILLI_SECONDS);
                cs.Start();
                cs.WaitForStatus(ServiceControllerStatus.Running, timeout);
            }
        }
    }
}
