using System;
using System.IO;
using System.Xml;
using System.Xml.Serialization;

namespace Cgw.SmcError
{
    public class PublicFuction
    {
        /// <summary>
        /// 反序列化
        /// </summary>
        public const string TYPE_FORMAT_SMCERROR = "Smc20.Common.SmcError.ErrorInfo,SmcError";

        /// <summary>
        /// 数据序列化到xml文档中
        /// </summary>
        /// <param name="tData">tData</param>
        /// <param name="XmlResult">XmlResult</param>
        public static void SerializeXML<T>(T tData, out XmlNode XmlResult)
        {
            XmlResult = null;
            XmlDocument xmldoc = new XmlDocument();

            //利用xml序列化器和MemoryStream，将数据序列化到xml文档中
            XmlSerializer s = new XmlSerializer(tData.GetType());
            MemoryStream stream = new MemoryStream();
            try
            {
                s.Serialize(stream, tData);
                stream.Position = 0; //注意这个位置，需要将Memorystream重新定位到起点。

                XmlReaderSettings setting = new XmlReaderSettings();
                setting.DtdProcessing = DtdProcessing.Prohibit;
                setting.XmlResolver = null;
                setting.ValidationType = ValidationType.None;
                XmlReader reader = XmlReader.Create(stream, setting);
                xmldoc.Load(reader);
                // xmldoc.Load(stream); //将xml流加载到xml文档对象。

                XmlResult = xmldoc.LastChild;
                reader.Close();
            }
            catch(Exception)
            {
				//int m = 100;
            }
            finally
            {
                stream.Close();
                stream.Dispose();
            }
        }

        /// <summary>
        /// 反序列化
        /// </summary>
        /// <param name="typeNameFormat">类型的格式字符串</param>
        /// <param name="tData">tData</param>
        /// <param name="XmlResult">XmlResult</param>
        /// <remarks>typeNameFormat的格式为"命名空间.{0},程序集名称"</remarks>
        public static void DesSerializeXML<T>(string typeNameFormat, XmlNode tData, out T XmlResult)
        {
            Type type = Type.GetType(string.Format(typeNameFormat, tData.Name));

            XmlSerializer serial = new XmlSerializer(type);
            XmlNodeReader reader = new XmlNodeReader(tData);

            T test = (T)serial.Deserialize(reader);
            XmlResult = test;
        }
    }
}
