/*-----------------------------------------------------------------------
//文件名：TiandyHttpClient.cs
//版权：Copyright 2011-2012 Huawei Tech. Co. Ltd. All Rights Reserved. 
//作者：kf70422
//日期：2013-2-27
//描述：天地伟业http接口
//---------------------------------------------------------------------*/
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net;
using System.IO;
using CgwMonitorManage.NLogEx;
using CgwMonitorManage.Common;

namespace CgwMonitorManage.Tiandy
{
    /// <summary>
    /// 天地伟业http接口
    /// 
    /// </summary>
    internal class TiandyHttpClient
    {

        /// <summary>
        /// 日志
        /// </summary>
        private static NLog.Logger log = NLog.LogManager.GetCurrentClassLogger();

        /// <summary>
        /// 服务端IP
        /// </summary>
        private string serverUrl;

        private string user;

        private string pwd;

        public TiandyHttpClient(string serverUrl, string user, string password)
        {
            this.serverUrl = serverUrl;
            this.user = user;
            this.pwd = password;
        }

        /// <summary>
        /// 调用天地伟业http接口
        /// </summary>
        /// <param name="requestUrl"></param>
        /// <returns>成功返回json字符串，失败返回null</returns>
        private string CallHttpInterface(string requestUrl)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Trace("Enter: TiandyHttpClient.CallHttpInterface().");
            Stream stream = null;
            string result = null;
            try
            {
                //调用http接口
                WebClient webClient = new WebClient();

                stream = webClient.OpenRead(requestUrl);
                StreamReader streamReader = new StreamReader(stream);

                //调用HTTP接口返回json字符串
                result = streamReader.ReadToEnd();
            }
            catch (Exception e)
            {
                result = null;
                logEx.Error("Call http function failed.Exception message:{0}.", e.Message);
            }
            finally
            {
                //记录日志，待日志模块合入后补充日志信息
                if (stream != null)
                {
                    stream.Close();
                }
            }
            return result;
        }

        /// <summary>
        /// 登陆http服务接口
        /// </summary>
        /// <returns>成功返回登录信息对象，失败返回null</returns>
        public LoginInfo Login()
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Trace("Enter: TiandyHttpClient.Login().");

            LoginInfo loginInfo = null;
            //设置访问天地伟业登陆接口的uri
            string requestUrl = string.Format(
                                "{0}/LogIn.jsp?UserName={1}&Password={2}&LoginType=1",
                                this.serverUrl,
                                this.user,
                                this.pwd);

            //日志不能记密码，单独处理
            string logString = string.Format(
                     "{0}/LogIn.jsp?UserName={1}&Password=******&LoginType=1",
                     this.serverUrl,
                     this.user);

            //调用HTTP接口获取json字符串
            string loginInfoString = CallHttpInterface(requestUrl);

            if (!string.IsNullOrEmpty(loginInfoString))
            {

                loginInfo = JsonUtil.ParseLoginInfo(loginInfoString);
                if (loginInfo == null)
                {
                    logEx.Error("Login to http server failed.Url:{0}", logString);
                    //返回失败设置错误码
                }
                logEx.Info("Login to http server success.Url:{0}", logString);
            }
            else
            {
                //如果字符串为空，说明调用不成功
                logEx.Error("Login to http server failed.Url:{0}", logString);
                loginInfo = null;
            }

            return loginInfo;
        }

        /// <summary>
        /// 获取设备树接口
        /// </summary>
        /// <returns>成功返回对象列表，失败返回null</returns>
        public List<Resource> GetResourceTree()
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Trace("Enter: TiandyHttpClient.GetResourceTree().");

            //设置访问天地伟业登陆接口的uri
            string requestUrl = string.Format(
                                "{0}/GetResourceTree.jsp?CurrentUserId={1}&UserId={2}&Password={3}",
                                this.serverUrl,
                                this.user,
                                this.user,
                                this.pwd);

            //日志不能记密码，单独处理
            string logString = string.Format(
                                "{0}/GetResourceTree.jsp?CurrentUserId={1}&UserId={2}&Password=******",
                                this.serverUrl,
                                this.user,
                                this.user);

            logEx.Trace("Call function TiandyHttpClient.GetResourceTree({0}).", logString);
            List<Resource> resources = GetResourceTree(requestUrl);

            return resources;
        }


        /// <summary>
        /// 获取自定义设备列表
        /// </summary>
        /// <returns>成功返回对象列表，失败返回null</returns>
        public List<Resource> GetCustomTree()
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Trace("Enter: TiandyHttpClient.GetCustomTree().");

            //设置访问天地伟业登陆接口的uri
            string requestUrl = string.Format(
                                "{0}/GetCustomTree.jsp?CurrentUserId={1}&UserId={2}&Password={3}",
                                this.serverUrl,
                                this.user,
                                this.user,
                                pwd);
            //日志不能记密码，单独处理
            string logString = string.Format(
                                "{0}/GetCustomTree.jsp?CurrentUserId={1}&UserId={2}&Password=******",
                                this.serverUrl,
                                this.user,
                                this.user);

            logEx.Trace("Call function TiandyHttpClient.GetResourceTree({0}).", logString);
            List<Resource> resources = GetResourceTree(requestUrl);

            return resources;

        }

        /// <summary>
        /// 获取设备树
        /// </summary>
        /// <param name="requestUrl"></param>
        /// <returns>成功返回对象列表，失败返回null</returns>
        private List<Resource> GetResourceTree(String requestUrl)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            List<Resource> resources = null;

            //调用HTTP接口获取json字符串
            string resourceString = CallHttpInterface(requestUrl);
            if (!string.IsNullOrEmpty(resourceString))
            {
                resources = JsonUtil.ParseResource(resourceString);
                if (resources == null)
                {
                    logEx.Error("Call function GetResourceTree() failed.");
                }
                else
                {
                    logEx.Info("Call function GetResourceTree() success.");
                }
            }
            else
            {
                //如果字符串为空，说明调用不成功
                resources = null;
                logEx.Error("Call function GetResourceTree() failed.");
            }

            return resources;
        }

    }
}
