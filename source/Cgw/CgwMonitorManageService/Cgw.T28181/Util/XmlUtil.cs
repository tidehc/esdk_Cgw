/*-----------------------------------------------------------------------
//文件名：XmlUtil.cs
//版权：Copyright 2011-2012 Huawei Tech. Co. Ltd. All Rights Reserved. 
//作者：w00206574
//日期：2014-3-06
//描述：解析返回的XML数据返回数据对象、对象列表
//---------------------------------------------------------------------*/
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Xml;
using System.Xml.Serialization;

namespace CgwMonitorManage.T28181
{
    public class XmlUtil
    {
        /// <summary>
        /// 日志
        /// </summary>
        private static NLog.Logger log = NLog.LogManager.GetCurrentClassLogger();

        /// <summary>
        /// 反序列化xml为对象
        /// </summary>
        /// <param name="xml">对象序列化后的Xml字符串</param>
        /// <returns></returns>
        public static T DeserializeToEntity<T>(string xml)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            //logEx.Trace("Enter: XmlUtil.DeserializeToEntity xml ={0}", xml);

            using (var sr = new StringReader(xml))
            {
                var xz = new XmlSerializer(typeof(T));
                try
                {
                    return (T)xz.Deserialize(sr);
                }
                catch (Exception xmle)
                {
                    logEx.Error("XmlUtil.DeserializeToEntity error ={0}", xmle.ToString());
                    return default(T);
                }
            }
        }

        /// <summary>
        /// 反序列化xml为List泛型
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <param name="node">XmlNode</param>
        /// <returns></returns>
        public static List<T> DeserializeToList<T>(XmlNode node)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Trace("Enter: XmlUtil.DeserializeToList XmlNode ={0}", node.Value);

            var list = new List<T>();
            try
            {
                if (node != null)
                {
                    list.AddRange(from XmlNode subNode in node.ChildNodes
                                  where subNode.NodeType != XmlNodeType.Comment
                                  select DeserializeToEntity<T>(subNode.OuterXml));
                }
            }
            catch (Exception xmle)
            {
                logEx.Error("XmlUtil.DeserializeToList error ={0}", xmle.ToString());
            }
            return list;
        }

        /// <summary>
        /// 获取xml某一节点的值
        /// </summary>
        /// <param name="strXml">xml文本</param>
        /// <param name="node">节点名称</param>
        /// <returns></returns>
        public static string GetNodeValue(string strXml, string node)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Trace("Enter: XmlUtil.GetNodeValue");

            var doc = new XmlDocument();
            try
            {
                doc.XmlResolver = null;
                doc.LoadXml(strXml);
            }
            catch (Exception ex)
            {
                logEx.Error("XmlUtil.GetNodeValue error={0}", ex.ToString());
                return string.Empty;
            }

            XmlNode content = doc.SelectSingleNode("Response");
            if (content != null)
            {
                XmlNode nodeCode = content.SelectSingleNode(node);
                if (nodeCode != null)
                {
                    return nodeCode.InnerText;
                }
                else
                {
                    //响应消息体不存在RESULT_CODE节点
                    return string.Empty;
                }
            }
            //响应消息体不存在RESULT_CODE节点
            return string.Empty;
        }

        static public XmlNode ProcessResponseMsg(string strBody, string node)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            //logEx.Trace("Enter: XmlUtil.GetNodeValue strXml ={0}", strBody);

            try
            {
                var doc = new XmlDocument();
                doc.XmlResolver = null;
                doc.LoadXml(strBody);

                XmlNode content = doc.SelectSingleNode("Response");
                if (content != null)
                {
                    XmlNode selectNode = content.SelectSingleNode(node);

                    return selectNode;
                }
                else
                {
                    //响应消息体不存在CV_CONTENT节点
                    return null;
                }
            }
            catch (Exception ex)
            {
                logEx.Error("XmlUtil.ProcessResponseMsg error={0}", ex.ToString());
                return null;
            }
        }
    }
}
