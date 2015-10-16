using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Configuration;
using System.Xml;
using System.Windows.Forms;
using System.Security.Cryptography;

namespace Cgw.Common
{
    public class ConfigSettings
    {
        /// <summary>
        /// 日志
        /// </summary>
        public static NLog.Logger log = NLog.LogManager.GetCurrentClassLogger();

        /// <summary>
        /// XmlDocument对象
        /// </summary>
        private static XmlDocument doc = null;

        /// <summary>
        /// XmlWriterSettings对象
        /// </summary>
        private static XmlWriterSettings settings = null;

        /// <summary>
        /// 构造函数
        /// </summary>
        static ConfigSettings()
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);

            settings = new XmlWriterSettings();
            settings.Indent = true;
            settings.IndentChars = "    ";
            settings.Encoding = Encoding.UTF8;

            try
            {
                doc = new XmlDocument();
                XmlReaderSettings setting = new XmlReaderSettings();
                setting.DtdProcessing = DtdProcessing.Prohibit;
                setting.XmlResolver = null;
                setting.ValidationType = ValidationType.None;
                XmlReader reader = XmlReader.Create(CgwConst.CGW_CONFIG_FILE_NAME, setting);
                doc.Load(reader);
                reader.Close();
                //doc.Load(CgwConst.CGW_CONFIG_FILE_NAME);
            }
            catch (Exception ex)
            {
                logEx.Error(ex, "File [{0}] is not exists!", CgwConst.CGW_CONFIG_FILE_NAME);
            }

        }

        #region monitor节点信息
        /// <summary>
        /// monitor节点信息
        /// </summary>
        public static XmlElement MonitorSettings
        {
            get
            {
                NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);

                try
                {
                    XmlNode monitorNode = doc.SelectSingleNode(CgwConst.MONITORS_CONFIG_ROOT_TAG);
                    XmlElement eleMonitor = (XmlElement)monitorNode;

                    //克隆一份Node，进行解密返回
                    XmlNode cloneMonitorNode = monitorNode.Clone();

                    //再克隆一份Node，用来判断配置文件是否需要保存
                    XmlNode xmlNodemonitor = monitorNode.Clone();

                    //解密
                    DecryptTerm(cloneMonitorNode);
                    XmlElement eleCloneMonitor = (XmlElement)cloneMonitorNode;

                    //加密判断
                    EncryptTerm(monitorNode);

                    //进行加密判断后，与原有文件进行对比，如果相同，直接返回，不需要保存配置文件
                    if (xmlNodemonitor.InnerXml == monitorNode.InnerXml)
                    {
                        return eleCloneMonitor;
                    }
                    else
                    {
                        using (XmlWriter xmlWriter = XmlWriter.Create(CgwConst.CGW_CONFIG_FILE_NAME, settings))
                        {
                            doc.Save(xmlWriter);
                        }

                        return eleCloneMonitor;
                    }
                }
                catch (Exception ex)
                {
                    logEx.Error(ex, "XmlNode [{0}] GetValueByKey failed.", CgwConst.MONITORS_CONFIG_ROOT_TAG);
                    return null;
                }
            }
        }

        #endregion

        #region 根据节点名称获取、修改对应的节点值
        /// <summary>
        /// 根据节点名称获取对应的节点值
        /// </summary>
        private static string GetValueByKey(string key)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            string keyValue = null;

            try
            {
                XmlNode node = doc.SelectSingleNode(CgwConst.SERVICES_SETTINGS_TAG + "/" + key);

                string term = node.InnerText;

                //属性已加密
                if (term.StartsWith(CgwConst.ENCRYPTED_PREFIX))
                {
                    keyValue = Decrypt(term,key);
                }
                //属性未加密
                else
                {
                    //属性需要加密
                    if (node.Attributes.Count > 0 && node.Attributes[CgwConst.NEEDENCRYPT_TAG].Value.ToUpper() == CgwConst.ISNEEDENCRYPT)
                    {
                        keyValue = term;
                        //加密
                        node.InnerText = Encrypt(term,key);
                        //回写
                        using (XmlWriter xmlWriter = XmlWriter.Create(CgwConst.CGW_CONFIG_FILE_NAME, settings))
                        {
                            doc.Save(xmlWriter);
                        }
                    }
                    //属性不需要加密
                    else
                    {
                        keyValue = term;
                    }
                }
            }
            catch (Exception ex)
            {
                logEx.Error(ex, "XmlNode [{0}] GetValueByKey failed.", key);
            }

            if (keyValue == null)
            {
                logEx.Error("XmlNode [{0}] is null. GetValueByKey failed.", key);
            }

            return keyValue;
        }

        /// <summary>
        /// 根据节点名称修改对应的节点值
        /// </summary>
        private static void SetValueByKey<T>(string key, T value)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);

            if (value == null)
            {
                logEx.Error("XmlNode [{0}] is null. SetValueByKey failed.", key);
            }
            else
            {
                string keyString = value.ToString();

                try
                {
                    XmlNode node = doc.SelectSingleNode(CgwConst.SERVICES_SETTINGS_TAG + "/" + key);

                    if (node.InnerText.StartsWith(CgwConst.ENCRYPTED_PREFIX))
                    {
                        node.InnerText = Encrypt(keyString,key);
                    }
                    else
                    {
                        if (node.Attributes.Count > 0 && node.Attributes[CgwConst.NEEDENCRYPT_TAG].Value.ToUpper() == CgwConst.ISNEEDENCRYPT)
                        {
                            node.InnerText = Encrypt(keyString,key);
                        }
                        else
                        {
                            node.InnerText = keyString;
                        }
                    }

                    //回写
                    using (XmlWriter xmlWriter = XmlWriter.Create(CgwConst.CGW_CONFIG_FILE_NAME, settings))
                    {
                        doc.Save(xmlWriter);
                    }
                }
                catch (Exception ex)
                {
                    logEx.Error(ex, "XmlNode [{0}] SetValueByKey failed.", key);
                }
            }

        }
        #endregion

        #region 连接密码
        /// <summary>
        /// 连接密码
        /// </summary>
        //public static string ConnectPassword
        //{
        //    get
        //    {
        //        return GetValueByKey(CgwConst.CONNECT_PASSWORD_TAG);
        //    }
        //    set
        //    {
        //        SetValueByKey(CgwConst.CONNECT_PASSWORD_TAG, value);
        //    }
        //}
        #endregion

        #region 获取连接监控管理服务密码
        /// <summary>
        /// 连接密码
        /// </summary>
        //public static string ConnectMonitorManagePassword
        //{
        //    get
        //    {
        //        return GetValueByKey(CgwConst.CONNECT_MONITOR_MANAGE_PASSWORD_TAG);
        //    }
        //    set
        //    {
        //        SetValueByKey(CgwConst.CONNECT_MONITOR_MANAGE_PASSWORD_TAG, value);
        //    }
        //}
        #endregion

        #region session超时时间
        /// <summary>
        /// session超时时间
        /// </summary>
        public static string SessionOverTime
        {
            get
            {
                return GetValueByKey(CgwConst.SESSION_OVERTIME_TAG);
            }
            set
            {
                SetValueByKey(CgwConst.SESSION_OVERTIME_TAG, value);
            }
        }
        #endregion

        #region PTZ控制级别
        /// <summary>
        /// PTZ控制级别
        /// </summary>
        public static string PtzControlLevel
        {
            get
            {
                return GetValueByKey(CgwConst.PTZ_CONTROL_LEVEL_TAG);
            }
            set
            {
                SetValueByKey(CgwConst.PTZ_CONTROL_LEVEL_TAG, value);
            }
        }
        #endregion

        #region 融合网关服务IP
        /// <summary>
        /// 融合网关服务IP
        /// </summary>
        public static string CgwServiceIp
        {
            get
            {
                return GetValueByKey(CgwConst.CGWSERVICE_IP_TAG);
            }
            set
            {
                SetValueByKey(CgwConst.CGWSERVICE_IP_TAG, value);
            }
        }
        #endregion

        #region 监控通道参数
        /// <summary>
        /// 监控通道RTP起始端口
        /// </summary>
        public static int CgwChannelRTPStartPort
        {
            get
            {
                NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);

                //起始端口默认为11000
                int cgwChannelRTPStartPort = 11000;

                try
                {
                    return Convert.ToInt32(GetValueByKey(CgwConst.CGW_CHANNEL_RTP_START_PORT_TAG));
                }
                catch (Exception ex)
                {
                    logEx.Error("CgwChannelRTPStartPort Convert is failed,Exception:{0},XmlNode:{1}", ex, CgwConst.CGW_CHANNEL_RTP_START_PORT_TAG);
                    return cgwChannelRTPStartPort;
                }
            }
            set
            {
                SetValueByKey(CgwConst.CGW_CHANNEL_RTP_START_PORT_TAG, value);
            }
        }

        /// <summary>
        /// 监控通道RTP端口总数
        /// </summary>
        public static int CgwChannelRTPPortCount
        {
            get
            {
                NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);

                //端口总数默认为1000
                int cgwChannelRTPPortCount = 1000;

                try
                {
                    return Convert.ToInt32(GetValueByKey(CgwConst.CGW_CHANNEL_RTP_PORT_COUNT_TAG));
                }
                catch (Exception ex)
                {
                    logEx.Error("CgwChannelRTPPortCount Convert is failed,Exception:{0},XmlNode:{1}", ex, CgwConst.CGW_CHANNEL_RTP_PORT_COUNT_TAG);
                    return cgwChannelRTPPortCount;
                }
            }
            set
            {
                SetValueByKey(CgwConst.CGW_CHANNEL_RTP_PORT_COUNT_TAG, value);
            }
        }

        /// <summary>
        /// 监控通道呼叫端口
        /// </summary>
        public static int CgwChanelCallPort
        {
            get
            {
                NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);

                try
                {
                    return Convert.ToInt32(GetValueByKey(CgwConst.CGW_CHANNEL_CALL_PORT));
                }
                catch (System.Exception ex)
                {
                    logEx.Debug("CgwChanelCallPort falied,Exception = {0} ", ex.ToString());
                    return 0;
                }


                //DTS2013070405672，因为mcu侧配置呼叫端口不生效，我们修改cgw的呼叫监听端口也没有用，所以去掉该配置项；如果mcu增加了配置端口工作，再添加该配置项
                //try
                //{
                //    return Convert.ToInt32(GetValueByKey(CgwConst.CGW_CHANNEL_CALL_PORT));
                //}
                //catch (Exception ex)
                //{
                //    logEx.Error("CgwChanelCallPort Convert is failed,Exception:{0},XmlNode:{1}", ex, CgwConst.CGW_CHANNEL_CALL_PORT);
                //    return cgwChanelCallPort;
                //}
            }
            set
            {
                //DTS2013070405672，因为mcu侧配置呼叫端口不生效，我们修改cgw的呼叫监听端口也没有用，所以去掉该配置项；如果mcu增加了配置端口工作，再添加该配置项
                SetValueByKey(CgwConst.CGW_CHANNEL_CALL_PORT, value);
            }
        }

        /// <summary>
        /// 监控通道持续无数据的次数
        /// </summary>
        public static int CgwChanelNoData
        {
            get
            {
                NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);

                try
                {
                    return Convert.ToInt32(GetValueByKey(CgwConst.CGW_CHANNEL_NODATA_COUNT));
                }
                catch (System.Exception ex)
                {
                    logEx.Debug("CGW_CHANNEL_NODATA_COUNT falied,Exception = {0} ", ex.ToString());
                    return 10;
                }
            }
            set
            {
                SetValueByKey(CgwConst.CGW_CHANNEL_NODATA_COUNT, value);
            }
        }

        /// <summary>
        /// 融合网关音频协议
        /// </summary>
        public static StreamType CgwAudioProtocol
        {
            get
            {
                StreamType cgwAudioProtocol = StreamType.AUDIO_G711A;
                string audioProtocol = GetValueByKey(CgwConst.CGW_Audio_Protocol);

                switch (audioProtocol)
                {
                    case "G711A":
                        cgwAudioProtocol = StreamType.AUDIO_G711A;
                        break;
                    case "G711U":
                        cgwAudioProtocol = StreamType.AUDIO_G711U;
                        break;
                }

                return cgwAudioProtocol;
            }
            set
            {
                SetValueByKey(CgwConst.CGW_Audio_Protocol, value);
            }

        }

        /// <summary>
        /// eLTEPlatformCameraGroupName
        /// </summary>
        public static string eLTEPlatformCameraGroupName
        {
            get
            {
                return GetValueByKey(CgwConst.ELTE_PLATFORM_CAMERA_GROUP_NAME);
            }
        }

        /// <summary>
        /// MonitorPlatformCameraGroupName
        /// </summary>
        public static string MonitorPlatformCameraGroupName
        {
            get
            {
                return GetValueByKey(CgwConst.MONITOR_PLATFORM_CAMERA_GROUP_NAME);
            }
        }

        #endregion

        #region 节点加密、解密
        /// <summary>
        /// 节点加密
        /// </summary>
        /// <param name="node">节点</param>
        private static void EncryptTerm(XmlNode node)
        {
            foreach (XmlNode xmlNode in node.ChildNodes)
            {
                if (xmlNode.Attributes != null)
                {
                    foreach (XmlAttribute attr in xmlNode.Attributes)
                    {
                        if (attr.Name == CgwConst.NEEDENCRYPT_TAG && attr.Value.ToUpper() == CgwConst.ISNEEDENCRYPT && !xmlNode.InnerText.StartsWith(CgwConst.ENCRYPTED_PREFIX))
                        {
                            xmlNode.InnerText = Encrypt(xmlNode.InnerText,xmlNode.Name);
                        }
                    }
                }
                if (xmlNode.HasChildNodes)
                {
                    EncryptTerm(xmlNode);
                }
            }
        }

        /// <summary>
        /// 节点解密
        /// </summary>
        /// <param name="node">节点</param>
        private static void DecryptTerm(XmlNode node)
        {
            foreach (XmlNode xmlNode in node.ChildNodes)
            {
                if (xmlNode.Attributes != null)
                {
                    foreach (XmlAttribute attr in xmlNode.Attributes)
                    {
                        if (xmlNode.InnerText.StartsWith(CgwConst.ENCRYPTED_PREFIX))
                        {
                            xmlNode.InnerText = Decrypt(xmlNode.InnerText,xmlNode.Name);
                        }
                    }
                }
                if (xmlNode.HasChildNodes)
                {
                    DecryptTerm(xmlNode);
                }
            }
        }
        #endregion

        #region 加密.解密方法
        /// <summary>
        /// 加密
        /// </summary>
        /// <param name="userPassword">用户密码.明文</param>
        /// <param name="nodeEntropy">增加加密效果，节点名称</param>
        /// <returns></returns>
        private static string Encrypt(string needEncryptTerm,string nodeEntropy)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);

            try
            {
                byte[] toEncryptArray = UTF8Encoding.UTF8.GetBytes(needEncryptTerm);
                byte[] encryptArray = ProtectedData.Protect(toEncryptArray, Encoding.Default.GetBytes(nodeEntropy), DataProtectionScope.LocalMachine);
                return CgwConst.ENCRYPTED_PREFIX + Convert.ToBase64String(encryptArray);
            }
            catch (Exception ex)
            {
                logEx.Trace("Encrypt failed Exception:{0},needEncryptTerm:{1}", ex, needEncryptTerm);
                return null;
            }

        }

        /// <summary>
        /// 解密
        /// </summary>
        /// <param name="userPassword">用户密码.密文</param>
        /// <param name="nodeEntropy">增加加密效果，节点名称</param>
        /// <returns></returns>
        private static string Decrypt(string needDecryptTerm,string nodeEntropy)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);

            try
            {
                string term = needDecryptTerm.Remove(0, CgwConst.ENCRYPTED_PREFIX.Length);
                byte[] toDecryptArray = Convert.FromBase64String(term);
                byte[] DecryptArray = ProtectedData.Unprotect(toDecryptArray, Encoding.Default.GetBytes(nodeEntropy), DataProtectionScope.LocalMachine);
                return UTF8Encoding.UTF8.GetString(DecryptArray);
            }
            catch (Exception ex)
            {
                logEx.Trace("Decrypt failed Exception:{0},needDecryptTerm:{1}", ex, needDecryptTerm);
                return null;
            }

        }
        #endregion
    }
}
