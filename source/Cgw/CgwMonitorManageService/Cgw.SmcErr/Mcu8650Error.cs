using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;
namespace CgwMonitorManage.SmcError
{
    [DataContract]
    public class Mcu8650Error : SmcErr
    {
        /// <summary>
        /// asn.1解码失败
        /// </summary>
        public const uint MCU_8650_OK = 0x00000001;
        /// <summary>
        /// asn.1解码失败
        /// </summary>
        public const uint ASN_DECODE_ERR = 0x00000002;

        /// <summary>
        /// 停止召集会议失败
        /// </summary>
        public const uint STOP_CONF_ERROR = 0x00000003;

        /// <summary>
        /// 离会
        /// </summary>
        public const uint ERR_CALLSITEFAIL_BASE = 0x06000000;





        /// <summary>
        /// 构造函数
        /// </summary>
        /// <param name="result">结果</param>

        public Mcu8650Error(uint result)
            : base(result)
        {
        }

        /// <summary>
        /// 构造函数
        /// </summary>
        public Mcu8650Error()
        {
        }

        /// <summary>
        /// 当前是否成功状态
        /// </summary>
        /// <returns>成功状态</returns>
        public override bool IsSuccess()
        {
            return (ErrNo == SMC_ERR_SUCCESS || ErrNo == STR_CONFSTART_RESULT || ErrNo == ERR_CALLSITEFAIL_BASE || ErrNo == STR_ADDSITE_RESULT);
        }

        /// <summary>
        /// 取模块名称
        /// </summary>
        /// <returns>模块名</returns>
        public override string GetModelName()
        {
            return this.GetType().Name;
        }
    }
}
