using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using Cgw.SmcError;
using System.Threading;
using Cgw.Common;

namespace Cgw.Service
{
    public class SessionManage
    {

        /// <summary>
        /// 读写锁
        /// </summary>
        private ReaderWriterLockSlim rwl = new ReaderWriterLockSlim();

        /// <summary>
        /// 单实例变量
        /// </summary>
        private static SessionManage instance;

        /// <summary>
        /// 会话集合
        /// </summary>
        /// <remarks>会话ID，会话</remarks>
        private Dictionary<Guid, CgwSession> sessionDict;


        /// <summary>
        /// 会话超时定时器
        /// </summary>
        private System.Timers.Timer timeoutTimer;

        /// <summary>
        /// 会话超时时间       
        /// </summary>
        private double sessionTimeout;

        /// <summary>
        /// 构造函数
        /// </summary>
        private SessionManage()
        {
            
            sessionDict = new Dictionary<Guid, CgwSession>();
           
            uint defaultSessionTimeout = 0;

            //获取配置文件中session超时时间（min）
            if (!uint.TryParse(ConfigSettings.SessionOverTime, out defaultSessionTimeout))
            {
                //缺省为1分钟
                defaultSessionTimeout = 1; 
            }
            //会话超时时间
            sessionTimeout = defaultSessionTimeout * 60;

            
            //会话超时定时处理器
            timeoutTimer = new System.Timers.Timer();
            timeoutTimer.AutoReset = true;
            timeoutTimer.Elapsed +=new System.Timers.ElapsedEventHandler(timeoutTimer_Elapsed);
            timeoutTimer.Interval = 1000;
            timeoutTimer.Start();
        }

        /// <summary>
        /// 定时清理过期会话
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void  timeoutTimer_Elapsed(object sender, System.Timers.ElapsedEventArgs e)
        {
            timeoutTimer.Stop();

            if (sessionDict != null && sessionDict.Count > 0 &&
                rwl.TryEnterUpgradeableReadLock(CgwConst.ENTER_LOCK_WAIT_TIME))
            {
                List<CgwSession> deletingCGMSession = new List<CgwSession>();

                try
                {
                    foreach (CgwSession session in sessionDict.Values)
                    {
                        if (session != null
                            && (e.SignalTime - session.LastVisitedTime).TotalSeconds > sessionTimeout)
                        {
                            deletingCGMSession.Add(session);
                        }

                    }
                    //清除已过期的会话
                    if (deletingCGMSession.Count > 0)
                    {
                        try
                        {
                            if (rwl.TryEnterWriteLock(CgwConst.ENTER_LOCK_WAIT_TIME))
                            {
                                CgwSession tempSession;
                                for (int i = 0; i < deletingCGMSession.Count; i++)
                                {
                                    tempSession = deletingCGMSession[i];
                                    if (tempSession != null)
                                    {
                                        //删除会话
                                        sessionDict.Remove(tempSession.SessionGuid);

                                        tempSession = null;
                                    }
                                    
                                }
                            }
                        }
                        finally
                        {
                            rwl.ExitWriteLock();
                        }
                    }
                }
                finally
                {
                    rwl.ExitUpgradeableReadLock();
                }
            }

            timeoutTimer.Start();
        }

        /// <summary>
        /// 单实例模式
        /// </summary>
        /// <returns></returns>
        public static SessionManage Instance()
        {

            if (instance == null)
            {
                instance = new SessionManage();
            }

            return instance;
        }

        /// <summary>
        /// 添加会话
        /// </summary>
        /// <param name="sessionGuid">会话GuidID</param>
        /// <param name="smcSession">会话数据</param>
        /// <returns></returns>
        public SmcErr AddSession(Guid sessionGuid, CgwSession smcSession)
        {
            SmcErr err = new SmcErr();

            if (rwl.TryEnterWriteLock(CgwConst.ENTER_LOCK_WAIT_TIME))
            {
                try
                {
                    if (sessionDict.ContainsKey(sessionGuid))
                    {
                        sessionDict.Remove(sessionGuid);
                    }
                    sessionDict.Add(sessionGuid, smcSession);

                }
                finally
                {
                    rwl.ExitWriteLock();
                }
            }

            return err;
        }

        /// <summary>
        /// 删除会话
        /// </summary>
        /// <param name="sessionGuid">会话Guid</param>
        /// <returns></returns>
        public SmcErr RemoveSession(Guid sessionGuid)
        {
           
            SmcErr err = new SmcErr();

            if (rwl.TryEnterWriteLock(CgwConst.ENTER_LOCK_WAIT_TIME))
            {
                try
                {
                    CgwSession session;
                    if (sessionDict.TryGetValue(sessionGuid, out session))
                    {
                        if (session != null)
                        {
                            sessionDict.Remove(sessionGuid);
                            session = null;
                            
                        }
                    }
                }
                finally
                {
                    rwl.ExitWriteLock();
                }
            }
            return err;
        }

        /// <summary>
        /// 删除会话
        /// </summary>
        /// <param name="sessionGuid">会话Guid</param>
        /// <returns></returns>
        public SmcErr RemoveSessionBySessionId(string sessionId)
        {
            SmcErr err = new SmcErr();
            if (rwl.TryEnterWriteLock(CgwConst.ENTER_LOCK_WAIT_TIME))
            {
                try
                {
                    foreach (CgwSession cgwSession in sessionDict.Values)
                    {
                        if (cgwSession.SessionId == sessionId)
                        {
                            sessionDict.Remove(cgwSession.SessionGuid);
                            break;
                        }
                    }

                }
                finally
                {
                    rwl.ExitWriteLock();
                }
            }
            return err;
        }

        /// <summary>
        /// 删除会话
        /// </summary>
        /// <param name="sessionGuid">会话Guid</param>
        /// <returns></returns>
        public void RemoveAllSession()
        {
            
            if (rwl.TryEnterWriteLock(CgwConst.ENTER_LOCK_WAIT_TIME))
            {
                sessionDict = new Dictionary<Guid, CgwSession>();
                rwl.ExitWriteLock();
            }
        }
        /// <summary>
        /// 根据sessionId获取session
        /// </summary>
        /// <param name="sessionId">sessionId</param>
        /// <param name="sessions">session</param>
        /// <returns></returns>
        public SmcErr GetSessionsBySessionId(string sessionId, out CgwSession sessions)
        {
            SmcErr err = new SmcErr();
            sessions = null;

            if (rwl.TryEnterReadLock(CgwConst.ENTER_LOCK_WAIT_TIME))
            {
                try
                {

                    foreach (CgwSession cgwSession in sessionDict.Values)
                    {
                        if (cgwSession.SessionId == sessionId)
                        {
                            sessions = cgwSession;
                            break;
                        }
                    }
                }
                finally
                {
                    rwl.ExitReadLock();
                }
            }

            return err;
        }

        /// <summary>
        /// 获取Session个数
        /// </summary>
        /// <returns></returns>
        public int GetSessionsCount()
        {
            int Count = 0;
            if (rwl.TryEnterReadLock(CgwConst.ENTER_LOCK_WAIT_TIME))
            {
                try
                {
                    if (sessionDict != null)
                    {
                        Count = sessionDict.Count;
                    }
                    else
                    {
                        Count = 0;
                    }
                }
                finally
                {
                    rwl.ExitReadLock();
                }
            }
            return Count;

        }

    }
}