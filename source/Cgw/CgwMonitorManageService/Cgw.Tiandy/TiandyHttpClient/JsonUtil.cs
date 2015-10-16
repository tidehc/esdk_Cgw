/*-----------------------------------------------------------------------
//文件名：JsonUtil.cs
//版权：Copyright 2011-2012 Huawei Tech. Co. Ltd. All Rights Reserved. 
//作者：kf70422
//日期：2013-2-25
//描述：json数据解析工具
//---------------------------------------------------------------------*/
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;
using System.Runtime.Serialization.Json;
using System.IO;

namespace CgwMonitorManage.Tiandy
{
    /// <summary>
    /// json数据解析工具类
    /// </summary>
    internal class JsonUtil
    {

        /// <summary>
        /// 将天地伟业登陆接口返回的json数据转换成对象
        /// </summary>
        /// <param name="jsonString">天地伟业http接口返回的json字符串</param>
        /// <returns></returns>
        public static LoginInfo ParseLoginInfo(string jsonString)
        {
            return ParseJsonString<LoginInfo>(jsonString);
        }

        /// <summary>
        /// 将天地伟业获取设备列表接口返回的json数据转换成对象
        /// </summary>
        /// <param name="jsonString">天地伟业http接口返回的json字符串</param>
        /// <returns></returns>
        public static List<Resource> ParseResource(string jsonString)
        {
            return ParseJsonString<List<Resource>>(jsonString);
        }


        /// <summary>
        /// 将json字符串转换成对象
        /// </summary>
        /// <typeparam name="T">将要转换成的对象</typeparam>
        /// <param name="jsonString">json字符串</param>
        /// <returns></returns>
        public static T ParseJsonString<T>(string jsonString) where T : class,new()
        {

            T jsonObject = null;
            MemoryStream memoryStream = null;
            try
            {
                //解析JSON字符串
                DataContractJsonSerializer jsonSerializer = new DataContractJsonSerializer(typeof(T));
                memoryStream = new MemoryStream(Encoding.UTF8.GetBytes(jsonString));
                jsonObject = (T)jsonSerializer.ReadObject(memoryStream);
            }
            catch
            {
                //记录日志，待日志模块合入后补充日志信息
                //如果解析失败返回空
                jsonObject = null;
            }
            finally
            {
                if (memoryStream != null)
                {
                    memoryStream.Close();
                }

            }
            return jsonObject;

        }
    }
}
