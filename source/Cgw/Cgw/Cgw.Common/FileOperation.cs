using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using Cgw.NLogEx;

namespace Cgw.Common
{
    public class FileOperation
    {
        /// <summary>
        /// 记录日志操作类.
        /// </summary>
        public static NLog.Logger log = NLog.LogManager.GetCurrentClassLogger();

        /// <summary>
        /// 写文件 
        /// </summary>
        /// <param name="path"></param>
        /// <param name="content"></param>
        public void WriteFile(string path,string content)
        {
            LoggerEx logEx = new LoggerEx(log);
            try
            {
                FileStream fs = new FileStream(path, FileMode.Create, FileAccess.Write);
                StreamWriter sw = new StreamWriter(fs, Encoding.Default);
                sw.Write(content);
                sw.Close();
                fs.Close();
            }
            catch (System.Exception ex)
            {
                logEx.Error("FileOperation.WriteFile failed. message:{0}", ex.ToString());
            }
        }
    }
}
