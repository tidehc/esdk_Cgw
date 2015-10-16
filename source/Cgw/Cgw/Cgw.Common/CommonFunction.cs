using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Security.Cryptography;
using System.ServiceModel;
using System.ServiceModel.Channels;
using System.Configuration;
using System.Xml;

namespace Cgw.Common
{
    public class CommonFunction
    {
        /// <summary>
        /// 获取web.config的AppSettings的项
        /// </summary>
        /// <param name="key"></param>
        /// <returns></returns>
        public static string GetAppSetting(string key)
        {
            string value = ConfigurationManager.AppSettings.Get(key);

            if (value == null)
            {
                //TODO,记录日志
                value = "";
            }

            return value;
        }

        /// <summary>
        /// SHA256加密
        /// </summary>
        /// <param name="str"></param>
        /// <returns></returns>
        public static string SHA256Encrypt(string str)
        {
            SHA256 s256 = new SHA256Managed();
            byte[] bytes;
            bytes = s256.ComputeHash(ASCIIEncoding.Default.GetBytes(str));
            s256.Clear();
            // 创建一个新的Stringbuilder来保存字节，并且创建一个字符串。
            StringBuilder sBuilder = new StringBuilder();

            // 循环哈希值的每一个字节，并以十六进制格式保存为字符串
            for (int i = 0; i < bytes.Length; i++)
            {
                sBuilder.Append(bytes[i].ToString("x2"));
            }
            return sBuilder.ToString();
        }
        /// <summary>
        /// 解析绑定方式
        /// </summary>
        /// <param name="bindname">绑定方式字符串</param>
        /// <param name="bind">绑定的句柄</param>
        public static void PraseBindManner(string bindname, out Binding bind)
        {
            Binding binding = null;

            switch (bindname)
            {

                case "BasicHttpBinding":
                    binding = new BasicHttpBinding();
                    break;


                case "WSHttpBinding":
                    //binding = new WSHttpBinding();
                    WSHttpBinding wsHttpBinding = new WSHttpBinding("WSHttpBinding_ISMC");
                    //wsHttpBinding.MaxReceivedMessageSize = 2147483647;
                    //wsHttpBinding.MaxBufferPoolSize = 2147483647;
                    //wsHttpBinding.ReceiveTimeout = new TimeSpan(1, 0, 0);
                    binding = wsHttpBinding;
                    break;

                default:
                    //todo
                    binding = new WSHttpBinding();
                    break;
            }

            bind = binding;
        }


        /// <summary>
        /// 用户密码加密
        /// </summary>
        /// <param name="userPassword">用户密码.明文</param>
        /// <returns></returns>
        public static string EncryptPassword(string userPassword,string nodeEntropy)
        {
            byte[] toEncryptArray = UTF8Encoding.UTF8.GetBytes(userPassword);
            byte[] encryptArray = ProtectedData.Protect(toEncryptArray, Encoding.Default.GetBytes(nodeEntropy), DataProtectionScope.CurrentUser);
            return "!@#" + Convert.ToBase64String(encryptArray);

        }

        /// <summary>
        /// 用户密码解密
        /// </summary>
        /// <param name="userPwd">用户密码.密文</param>
        /// <param name="nodeEntropy">增加加密效果，节点名称</param>
        /// <returns></returns>
        public static string DecryptPassword(string userPwd,string nodeEntropy)
        {

            string pwd = userPwd.Remove(0, 3);
            byte[] userPwdByte = Convert.FromBase64String(pwd);
            byte[] DecryptArray = ProtectedData.Unprotect(userPwdByte, Encoding.Default.GetBytes(nodeEntropy), DataProtectionScope.CurrentUser);
            return UTF8Encoding.UTF8.GetString(DecryptArray);

        }

        /// <summary>
        /// 更新配置文件
        /// </summary>
        /// <param name="userPassword"></param>
        public static void UpdateConfig(string userPassword)
        {
            Configuration config = ConfigurationManager.OpenExeConfiguration(ConfigurationUserLevel.None);
            if (userPassword != null)
            {
                config.AppSettings.Settings["ConnectPassword"].Value = EncryptPassword(userPassword, "ConnectPassword");
                config.Save(ConfigurationSaveMode.Modified);
                ConfigurationManager.RefreshSection("appSettings");

            }
        }

        /// <summary>
        /// 读取Node节点值
        /// </summary>
        /// <param name="xmlElement"></param>
        /// <param name="nodeName"></param>
        /// <returns></returns>
        public static string GetSingleNodeValue(XmlElement xmlElement, string nodeName)
        {
            XmlNode node = xmlElement.SelectSingleNode(nodeName);
            if (node == null)
            {
                throw new Exception(string.Format("The element is not contain the child node.Node:{0}.", nodeName));
            }

            return node.InnerText.Trim();
        }

        /// <summary>
        /// 读取节点信息
        /// </summary>
        /// <param name="xmlElement"></param>
        /// <param name="nodeName"></param>
        /// <param name="attributeName"></param>
        /// <returns></returns>
        public static string GetAttribute(XmlElement xmlElement, string nodeName, string attributeName)
        {
            XmlNode node = xmlElement.SelectSingleNode(nodeName);
            if (node == null)
            {
                throw new Exception(string.Format("The node is not contain the attribute.Node:{0},attribute:{1}.", nodeName, attributeName));
            }

            XmlElement element = node as XmlElement;
            if (element == null)
            {
                throw new Exception(string.Format("The node is not contain the attribute.Node:{0},attribute:{1}.", nodeName, attributeName));
            }
            string attributeValue = element.GetAttribute(attributeName).Trim();
            if (string.IsNullOrEmpty(attributeValue))
            {
                throw new Exception(string.Format("The node is not contain the attribute.Node:{0},attribute:{1}.", nodeName, attributeName));
            }

            return attributeValue;
        }

        /// <summary>
        /// 读取节点信息
        /// </summary>
        /// <param name="xmlElement"></param>
        /// <param name="nodeName"></param>
        /// <param name="attributeName"></param>
        /// <returns></returns>
        public static void SetAttribute(XmlElement xmlElement, string nodeName, string attributeName, string attributeValue)
        {
            XmlNode node = xmlElement.SelectSingleNode(nodeName);
            if (node == null)
            {
                throw new Exception(string.Format("The node is not contain the attribute.Node:{0},attribute:{1}.", nodeName, attributeName));
            }

            XmlElement element = node as XmlElement;
            if (element == null)
            {
                throw new Exception(string.Format("The node is not contain the attribute.Node:{0},attribute:{1}.", nodeName, attributeName));
            }
            element.SetAttribute(attributeName, attributeValue);
        }

        /// <summary>
        /// 将code@ip的通道标识中code提取出来
        /// </summary>
        /// <param name="channelLabel">通道标识</param>
        /// <returns>code</returns>
        public static string ConvertChannelLabel(string channelLabel)
        {
            // 判断输入通道号码是否为空
            if (string.IsNullOrEmpty(channelLabel))
            {
                return string.Empty;
            }

            // 如果channelLabel没有@，原字符串返回
            bool isFind = channelLabel.Contains("@");
            if (!isFind)
            {
                return channelLabel;
            }

            // 通过@将通道标识分成几段
            string[] strArray = channelLabel.Split(new string[] { "@" }, StringSplitOptions.None);
            if (null == strArray || null == strArray[0])
            {
                return string.Empty;
            }

            return strArray[0];
        }
    }
}
