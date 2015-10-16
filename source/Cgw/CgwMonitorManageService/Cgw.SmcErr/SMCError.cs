//-----------------------------------------------------------------------
//文件名：SmcErr.cs
//版权：Copyright 2010-2011 Huawei Tech. Co. Ltd. All Rights Reserved. 
//描述：后台全部服务的错误码
//-----------------------------------------------------------------------

using System;
using System.Runtime.Serialization;
using System.Xml.Serialization;
using System.Collections.Generic;
using System.ServiceModel;
using System.Xml;
using System.Diagnostics;
using System.Text;

namespace CgwMonitorManage.SmcError
{
    /// <summary>
    /// SmcErr类：后台服务错误码，错误码从0x50000000开始，之前的错误码段为老RM使用
    /// 服务的错误码起始位置分配：
    /// 通用错误：  0x50000000
    /// 注册服务：  0x50100000
    /// 用户服务：  0x50200000
    /// 会场服务：  0x50300000
    /// MCU服务：   0x50400000
    /// 会议服务：  0x50500000
    /// GKM服务：   0x50600000
    /// 数据库服务：0x50700000
    /// email服务： 0x50800000
    /// BLL：       0x50900000
    /// 配置模版：  0x50a00000
    /// 错误码命名：服务/模块_错误描述
    /// 服务内部按子模块分段设置错误码，0x1000为一段，即一个服务内部最多分为256个子模块
    /// </summary>
    [DataContract]
    [KnownType(typeof(GkErr))]
    [KnownType(typeof(Mcu9610Error))]
    [KnownType(typeof(VctError))]
    [KnownType(typeof(Tex0Error))]
    [KnownType(typeof(Mcu8650Error))]
    [KnownType(typeof(McuHttpError))]
    [KnownType(typeof(Mcu9660Error))]
    [KnownType(typeof(EConfError))]
    [KnownType(typeof(UpgradeError))]
    [KnownType(typeof(LicenseError))]
    [KnownType(typeof(NrsError))]
    [KnownType(typeof(SmcAlarm))]
    [KnownType(typeof(SmcAlarmOri))]
    [KnownType(typeof(SmcAlarmOrientation))]
    [KnownType(typeof(TP3206Error))]
    [KnownType(typeof(DataConfError))]
    [KnownType(typeof(CgwError))]
    [Serializable]
    public partial class SmcErr
    {
        #region 构造函数和成员
        /// <summary>
        /// 默认构造函数
        /// </summary>
        public SmcErr()
        {
        }

        /// <summary>
        /// 构造函数
        /// </summary>
        /// <param name="err">错误码</param>
        public SmcErr(uint err)
        {
            ErrNo = err;
        }

        /// <summary>
        /// 设置错误码
        /// </summary>
        /// <param name="err">状态</param>
        public void SetErrorNo(uint err)
        {
            ErrNo = err;
        }

        /// <summary>
        /// 当前是否成功状态
        /// </summary>
        /// <returns>成功状态</returns>
        public virtual bool IsSuccess()
        {
            return (ErrNo == SMC_ERR_SUCCESS);
        }

        /// <summary>
        /// 返回错误码对应的字符串描述
        /// </summary>
        /// <returns>string</returns>
        public string GetErrorDesc()
        {
            return ToFomatedErrDesc(LANGUAGE_ENGLISH);
        }

        /// <summary>
        /// 中文语种，用于错误码模块内部
        /// </summary>
        private const int LANGUAGE_CHINESE = 1;

        /// <summary>
        /// 英文语种，用于错误码模块内部
        /// </summary>
        private const int LANGUAGE_ENGLISH = 2;

        /// <summary>
        /// 错误码
        /// </summary>
        [DataMember]
        public uint ErrNo = SMC_ERR_SUCCESS;

        /// <summary>
        /// 错误对应的参数
        /// </summary>
        [DataMember]
        [System.Xml.Serialization.XmlElement(typeof(RangeErrorInfo))]
        [System.Xml.Serialization.XmlElement(typeof(EmailErrorInfo))]
        [System.Xml.Serialization.XmlElement(typeof(ErrorInfo))]
        [System.Xml.Serialization.XmlElement(typeof(RmAdapterErrorInfo))]
        public ErrorInfo ErrorInfoParam { get; set; }

        /// <summary>
        /// 成功
        /// </summary>
        public const uint SMC_ERR_SUCCESS = 0;

        /// <summary>
        /// 失败
        /// </summary>
        public const uint ASN_DECODE_ERR = 1;

        /// <summary>
        /// 获取错误码模块名称
        /// </summary>
        /// <returns>name</returns>
        public virtual string GetModelName()
        {
            return this.GetType().Name;
        }

        /// <summary>
        /// 参数序列化，反序列化处理
        /// </summary>
        public string GetXmlParamter()
        {
            if (null == this.ErrorInfoParam)
            {
                return null;
            }
            else
            {
                System.Xml.XmlNode xmlNode = null;
                PublicFuction.SerializeXML(this.ErrorInfoParam, out xmlNode);
                if (xmlNode == null)
                {
                    return null;
                }
                else
                {
                    return xmlNode.OuterXml;
                }
            }
        }

        /// <summary>
        /// 参数序列化，反序列化处理
        /// </summary>
        public void SetXmlParamter(string value)
        {
            if (string.IsNullOrEmpty(value))
            {
                this.ErrorInfoParam = null;
            }
            else
            {
                XmlDocument xmlstr = new XmlDocument();
                xmlstr.XmlResolver = null;
                xmlstr.LoadXml(value);

                XmlNode xmlNode = xmlstr;
                ErrorInfo errInfo = null;
                PublicFuction.DesSerializeXML<ErrorInfo>(PublicFuction.TYPE_FORMAT_SMCERROR, xmlNode, out errInfo);
                this.ErrorInfoParam = errInfo;
            }
        }
        #endregion

        #region 取格式化错误描述接口
        /// <summary>
        /// 取格式化后的错误描述
        /// </summary>
        /// <param name="langId">语种ID</param>
        /// <returns>格式化后的错误描述字串</returns>
        public string ToFomatedErrDesc(int langId)
        {
            // 获取委托接口
            DelegateGetErrorDesc delegateHandler = GetDelegate();

            // 查询带格式的错误描述
            string errDesc = delegateHandler(ErrNo, this.GetModelName(), langId);

            //错误描述字串为空，返回错误号码
            if (string.IsNullOrEmpty(errDesc))
            {
                return ErrNo.ToString();
            }

            return errDesc;

            //if (null == ErrorInfoParam)
            //{
            //    return errDesc;
            //}

            //// 格式化错误描述，填充错误参数
            //return ErrorInfoParam.ToFormatedErrDesc(delegateHandler, langId, errDesc);
        }

        /// <summary>
        /// 取查询错误描述的委托
        /// </summary>
        /// <returns>DelegateGetErrorDesc</returns>
        public DelegateGetErrorDesc GetDelegate()
        {
            DelegateGetErrorDesc delegateHandler = new DelegateGetErrorDesc(ResHelper.Instance().GetErrDesc);

            return delegateHandler;
        }
        #endregion

        #region 通用错误码0x50000000
        /// <summary>
        /// 通用错误基址
        /// </summary>
        public const uint SMC_COMMON_ERR = 0x50000000;

        /// <summary>
        /// 参数错误
        /// </summary>
        public const uint SMC_INVALID_PARAM = SMC_COMMON_ERR + 1;

        /// <summary>
        /// 服务出现异常
        /// </summary>
        public const uint SMC_CATCH_EXCEPTION = SMC_COMMON_ERR + 2;

        /// <summary>
        /// 服务处于忙状态
        /// </summary>
        public const uint ERR_SERVICES_STATE_BUSY = SMC_COMMON_ERR + 3;

        /// <summary>
        /// 服务离线
        /// </summary>
        public const uint ERR_SERVICES_STATE_OFFLINE = SMC_COMMON_ERR + 4;

        /// <summary>
        /// 服务重连
        /// </summary>
        public const uint ERR_SERVICES_NEED_RECONNECT = SMC_COMMON_ERR + 5;

        /// <summary>
        /// 索引超出范围
        /// </summary>
        public const uint ERR_INDEX_OVERFLOW_ERROR = SMC_COMMON_ERR + 6;

        /// <summary>
        /// 停止服务失败
        /// </summary>
        public const uint ERR_STOP_SERVICE_FAIL = SMC_COMMON_ERR + 7;

        /// <summary>
        /// 启动服务失败
        /// </summary>
        public const uint ERR_START_SERVICE_FAIL = SMC_COMMON_ERR + 8;

        /// <summary>
        /// 该输入项为必输项
        /// 备注：（为后台检查界面参数项不能为空的通用错误码）
        /// </summary>
        public const uint ERR_INPUT_NOT_NULL = SMC_COMMON_ERR + 9;

        /// <summary>
        /// 该输入项必须为1～65535之间的数字
        /// 备注（端口的最大值为1，最小值为65535）
        /// </summary>
        public const uint ERR_PORT_INPUT_VALUE = SMC_COMMON_ERR + 0x0A;

        /// <summary>
        /// 该输入项必须是IP地址，格式如："192.168.0.1"
        /// </summary>
        public const uint ERR_IP_FORMAT_ERROR = SMC_COMMON_ERR + 0x0B;

        /// <summary>
        /// 该输入项只能包含数字
        /// </summary>
        public const uint ERR_ONLY_NUMBER_ERROR = SMC_COMMON_ERR + 0x0C;

        /// <summary>
        /// 用户服务离线
        /// </summary>
        public const uint ERR_SERVICES_USER_STATE_OFFLINE = SMC_COMMON_ERR + 0x0D;

        /// <summary>
        /// 会议服务离线
        /// </summary>
        public const uint ERR_SERVICES_CONF_STATE_OFFLINE = SMC_COMMON_ERR + 0x0E;

        /// <summary>
        /// 会场管理服务离线
        /// </summary>
        public const uint ERR_SERVICES_DEVICE_STATE_OFFLINE = SMC_COMMON_ERR + 0x0F;

        /// <summary>
        /// EMAIL服务离线
        /// </summary>
        public const uint ERR_SERVICES_EMAIL_STATE_OFFLINE = SMC_COMMON_ERR + 0x10;

        /// <summary>
        /// 数据库服务离线
        /// </summary>
        public const uint ERR_SERVICES_DB_STATE_OFFLINE = SMC_COMMON_ERR + 0x11;

        /// <summary>
        /// IIS服务离线
        /// </summary>
        public const uint ERR_SERVICES_IIS_STATE_OFFLINE = SMC_COMMON_ERR + 0x12;

        /// <summary>
        /// 该输入项的最小值、最大值范围是{0}～{1}。
        /// </summary>
        public const uint ERR_SERVICES_PARAM_EXCESS_RANGE = SMC_COMMON_ERR + 0x13;

        /// <summary>
        /// 数字版本号冲突
        /// </summary>
        public const uint ERR_SERVICES_PARAM_DATAVER_CONFLICT = SMC_COMMON_ERR + 0x14;

        /// <summary>
        /// 非法字符
        /// </summary>
        public const uint ERR_CHARATER = SMC_COMMON_ERR + 0x17;
        #endregion

        #region 注册服务和过滤器0x50100000
        //////////////////////////////////////////////////////////////////////////
        // 注册服务错误码0x50100000
        //////////////////////////////////////////////////////////////////////////

        /// <summary>
        /// 参数错误
        /// </summary>
        public const uint ERR_REG_SERVER = 0x50100000;

        /// <summary>
        /// 服务不存在
        /// </summary>
        public const uint REG_SERVICE_NOT_EXIT = ERR_REG_SERVER + 1;

        /// <summary>
        /// 服务已经存在
        /// </summary>
        public const uint REG_SERVICE_ALREADY_EXIT = ERR_REG_SERVER + 2;

        /// <summary>
        /// 不识别的服务
        /// </summary>
        public const uint REG_CREATE_SERVCIE_ERROR = ERR_REG_SERVER + 3;

        /// <summary>
        /// 服务的过滤组件不存在
        /// </summary>
        public const uint REG_SERVICE_FILTER_NOT_EXIT = ERR_REG_SERVER + 4;

        /// <summary>
        /// {servicename}{0}注册成功。
        /// </summary>
        public const uint REG_SERVICE_SUCCESS = ERR_REG_SERVER + 5;

        /// <summary>
        /// {servicename}{0}注册失败，原因：{err}{0}。
        /// </summary>
        public const uint REG_SERVICE_FAILED = ERR_REG_SERVER + 6;

        /// <summary>
        /// 注册服务成功开启
        /// </summary>
        public const uint REG_SERVICE_SUCCESS_START = ERR_REG_SERVER + 7;
        /// <summary>
        /// 注册服务成功停止
        /// </summary>
        public const uint REG_SERVICE_SUCCESS_STOP = ERR_REG_SERVER + 8;

        /// <summary>
        /// "{servicename}{0}"服务离线
        /// </summary>
        public const uint SERVICE_OFFLINE = ERR_REG_SERVER + 9;

        //////////////////////////////////////////////////////////////////////////
        // 过滤器错误码0x50101000
        //////////////////////////////////////////////////////////////////////////

        /// <summary>
        /// 过滤器错误码0x50101000
        /// </summary>
        public const uint ERR_MESSAGE_FILTER = 0x50101000;

        /// <summary>
        /// 过滤器不存在
        /// </summary>
        public const uint FILTER_ERR_FILTER_NOT_EXIST = ERR_MESSAGE_FILTER + 1;

        /// <summary>
        /// 信息解析错误
        /// </summary>
        public const uint FILTER_ERR_DESERIALIZE_ERROR = ERR_MESSAGE_FILTER + 2;

        /// <summary>
        /// 过滤器中该订阅类型不存在
        /// </summary>
        public const uint FILTER_ERR_SUBTYPE_NOT_EXIST = ERR_MESSAGE_FILTER + 3;
        #endregion

        #region 用户服务错误码0x50200000
        //////////////////////////////////////////////////////////////////////////
        // 用户服务：用户数据相关的错误码0x50200000
        //////////////////////////////////////////////////////////////////////////

        /// <summary>
        /// 用户服务：用户数据相关的错误码0x50200000
        /// </summary>
        public const uint ERR_USER_SERVICE = 0x50200000;

        /// <summary>
        /// 用户认证密码错误
        /// </summary>
        public const uint USER_AUHT_ERROR = ERR_USER_SERVICE + 1;

        /// <summary>
        /// 用户认证成功但密码被重置
        /// </summary>
        public const uint USER_AUHT_SUCCES_ADD_RESET_PASSWORD = ERR_USER_SERVICE + 2;

        /// <summary>
        /// 用户权限不足
        /// </summary>
        public const uint USER_NO_PERMISSION_TODO = ERR_USER_SERVICE + 3;

        /// <summary>
        /// 用户名不能为空
        /// </summary>
        public const uint USER_NAME_IS_NULL = ERR_USER_SERVICE + 4;

        /// <summary>
        /// 用户不存在
        /// </summary>
        public const uint USER_ISNOT_EXIT = ERR_USER_SERVICE + 5;

        /// <summary>
        /// 用户名重复
        /// </summary>
        public const uint USER_NAME_REPEAT = ERR_USER_SERVICE + 6;

        /// <summary>
        /// 用户名长度不合法
        /// </summary>
        public const uint USER_NAME_LENGTH_ERR = ERR_USER_SERVICE + 7;

        /// <summary>
        /// admin用户密码不能被普通用户重置
        /// </summary>
        public const uint USER_ADMIN_DENY_RESETPASSWORD = ERR_USER_SERVICE + 0x0A;

        /// <summary>
        /// 用户名不能修改
        /// </summary>
        public const uint USER_NAME_ISNOT_EDIT = ERR_USER_SERVICE + 0x0B;

        /// <summary>
        /// 用户密码被重置
        /// </summary>
        public const uint USER_PASSWORD_RESET = ERR_USER_SERVICE + 0x0C;

        /// <summary>
        /// 用户处于离线状态
        /// </summary>
        public const uint USER_LOGINSTATE_OFFLINE = ERR_USER_SERVICE + 0x0D;

        /// <summary>
        /// 不是owner用户
        /// </summary>
        public const uint USER_ISNOT_OWNER = ERR_USER_SERVICE + 0x0E;

        /// <summary>
        /// 用户不存在任何组织内
        /// </summary>
        public const uint USER_NOT_ANY_ORG = ERR_USER_SERVICE + 0x0F;

        /// <summary>
        /// 用户状态中登录语言不存在
        /// </summary>
        public const uint USER_LOGINLANG_IS_NULL = ERR_USER_SERVICE + 0x10;

        /// <summary>
        /// 用户状态中登录时间不存在
        /// </summary>
        public const uint USER_LOGINTIME_IS_NULL = ERR_USER_SERVICE + 0x11;

        /// <summary>
        /// 用户状态中注销时间不存在
        /// </summary>
        public const uint USER_LOGOUTTIME_IS_NULL = ERR_USER_SERVICE + 0x12;

        /// <summary>
        /// 修改密码时，用户的旧密码不正确
        /// </summary>
        public const uint USER_OLD_PASSWORD_ERROR = ERR_USER_SERVICE + 0x13;

        /// <summary>
        /// 修改密码时，用户两次输入的新密码不一致
        /// </summary>
        public const uint USER_NEW_PASSWORD_DIFFER = ERR_USER_SERVICE + 0x14;

        /// <summary>
        /// 用户已存在
        /// </summary>
        public const uint USER_IS_EXIST = ERR_USER_SERVICE + 0x15;

        /// <summary>
        /// 管理员不能删除
        /// </summary>
        public const uint USER_ADMIN_DENY_DEL = ERR_USER_SERVICE + 0x16;

        /// <summary>
        /// MCU已经存在了
        /// </summary>
        public const uint USER_MCU_IS_EXIST = ERR_USER_SERVICE + 0x17;

        /// <summary>
        /// MCU不存在
        /// </summary>
        public const uint USER_MCU_ISNOT_EXIST = ERR_USER_SERVICE + 0x18;

        /// <summary>
        /// 会场已经存在了
        /// </summary>
        public const uint USER_SITE_IS_EXIST = ERR_USER_SERVICE + 0x19;

        /// <summary>
        /// 会场不存在
        /// </summary>
        public const uint USER_SITE_ISNOT_EXIST = ERR_USER_SERVICE + 0x20;

        /// <summary>
        /// 会议模板已经存在了
        /// </summary>
        public const uint USER_CONFTEMPLATE_IS_EXIST = ERR_USER_SERVICE + 0x21;

        /// <summary>
        /// 会议模板不存在
        /// </summary>
        public const uint USER_CONFTEMPLATE_ISNOT_EXIST = ERR_USER_SERVICE + 0x22;

        /// <summary>
        /// 会议已经存在了
        /// </summary>
        public const uint USER_CONF_IS_EXIST = ERR_USER_SERVICE + 0x23;

        /// <summary>
        /// 会议不存在
        /// </summary>
        public const uint USER_CONF_ISNOT_EXIST = ERR_USER_SERVICE + 0x24;

        /// <summary>
        /// 计费码已经存在了
        /// </summary>
        public const uint USER_BILLCODE_IS_EXIST = ERR_USER_SERVICE + 0x25;

        /// <summary>
        /// 计费码不存在
        /// </summary>
        public const uint USER_BILLCODE_ISNOT_EXIST = ERR_USER_SERVICE + 0x26;

        /// <summary>
        /// 用户数据冲突，数据版本号不同
        /// </summary>
        public const uint USER_DATAVER_ERR = ERR_USER_SERVICE + 0x27;

        /// <summary>
        /// 不能移除MCU、会场原生组织关系
        /// </summary>
        public const uint USER_CANNOT_REMOVE_ORIGINALORG = ERR_USER_SERVICE + 0x28;

        /// <summary>
        /// 不能编辑admin组织关系
        /// </summary>
        public const uint USER_CANNOT_EDIT_ADMIN_ORGRELATION = ERR_USER_SERVICE + 0x29;

        /// <summary>
        /// 用户不能删除自己的组织关系
        /// </summary>
        public const uint USER_CANNOT_EDIT_OWN_ORGRELATION = ERR_USER_SERVICE + 0x2A;

        /// <summary>
        /// 用户不能重置自己密码
        /// </summary>
        public const uint USER_CANNOT_RESET_OWN_PASSWORD = ERR_USER_SERVICE + 0x2B;

        /// <summary>
        /// 管理员密码被重置
        /// </summary>
        public const uint USER_ADMIN_RESET_PASSWORD = ERR_USER_SERVICE + 0x2C;

        /// <summary>
        /// 用户名包含非法字符“,”
        /// </summary>
        public const uint USER_NAME_CONTAIN_INVALID_CHARACTER = ERR_USER_SERVICE + 0x2D;

        /// <summary>
        /// 被删除的用户不存在
        /// </summary>
        public const uint USER_DELETE_USER_ISNOT_EXIST = ERR_USER_SERVICE + 0x2E;

        /// <summary>
        /// 用户个数已经达到最大值
        /// </summary>
        public const uint USER_COUNT_REACH_MAX = ERR_USER_SERVICE + 0x2F;

        /// <summary>
        /// 用户的在线个数达到最大值
        /// </summary>
        public const uint USER_ONLINESTATE_COUNT_REACE_MAX = ERR_USER_SERVICE + 0x30;

        /// <summary>
        /// 无效用户
        /// </summary>
        public const uint USER_IS_INVALID = ERR_USER_SERVICE + 0x31;

        /// <summary>
        /// 用户名或者密码错误
        /// </summary>
        public const uint USER_NAME_IS_INVALID = ERR_USER_SERVICE + 0x32;

        /// <summary>
        /// 录播服务器已经存在。
        /// </summary>
        public const uint USER_NRS_IS_EXIST = ERR_USER_SERVICE + 0x33;

        /// <summary>
        /// 录播服务器不存在
        /// </summary>
        public const uint USER_NRS_ISNOT_EXIST = ERR_USER_SERVICE + 0x34;

        /// <summary>
        /// 用户被锁定
        /// </summary>
        public const uint USER_ISLOCK = ERR_USER_SERVICE + 0x35;

        /// <summary>
        /// 用户服务开启
        /// </summary>
        public const uint USER_SERVICE_START = ERR_USER_SERVICE + 0x36;

        /// <summary>
        /// 用户服务停止
        /// </summary>
        public const uint USER_SERVICE_STOP = ERR_USER_SERVICE + 0x37;

        //////////////////////////////////////////////////////////////////////////
        //用户服务：组织结构错误码0x50201000
        //////////////////////////////////////////////////////////////////////////

        /// <summary>
        /// 组织结构管理错误码
        /// </summary>
        public const uint ERR_ORGANIZATION_SERVICE = 0x50201000;

        /// <summary>
        /// 组织结构名称重复
        /// </summary>
        public const uint ORG_NAME_REPEAT = ERR_ORGANIZATION_SERVICE + 1;

        /// <summary>
        /// 组织结构不存在
        /// </summary>
        public const uint ORG_ISNOT_EXIST = ERR_ORGANIZATION_SERVICE + 2;

        /// <summary>
        /// 删除组织内MCU失败
        /// </summary>
        public const uint ORG_EXIST_ORGIN_MCU = ERR_ORGANIZATION_SERVICE + 3;

        /// <summary>
        /// 删除组织内会场失败
        /// </summary>
        public const uint ORG_EXIST_ORGIN_SITE = ERR_ORGANIZATION_SERVICE + 4;

        /// <summary>
        /// 组织关系已存在
        /// </summary>
        public const uint ORG_RELATION_EXIST = ERR_ORGANIZATION_SERVICE + 5;

        /// <summary>
        /// 组织关系不存在
        /// </summary>
        public const uint ORG_RELATION_NOTEXIST = ERR_ORGANIZATION_SERVICE + 6;

        /// <summary>
        /// 组织结构名称为空
        /// </summary>
        public const uint ORG_NAME_IS_NULL = ERR_ORGANIZATION_SERVICE + 7;

        /// <summary>
        /// 组织结构名称长度错误
        /// </summary>
        public const uint ORG_NAME_LENGTH_ERR = ERR_ORGANIZATION_SERVICE + 8;

        /// <summary>
        /// 组织结构名称已经存在
        /// </summary>
        public const uint ORG_NAME_IS_EXIST = ERR_ORGANIZATION_SERVICE + 9;

        /// <summary>
        /// 组织结构计费码为空
        /// </summary>
        public const uint ORG_BILLCODE_IS_NULL = ERR_ORGANIZATION_SERVICE + 0x0A;

        /// <summary>
        /// 组织结构计费码长度错误
        /// </summary>
        public const uint ORG_BILLCODE_LENGTH_ERR = ERR_ORGANIZATION_SERVICE + 0x0B;

        /// <summary>
        /// 组织结构数据冲突，数据版本号不同
        /// </summary>
        public const uint ORG_DATAVER_ERR = ERR_ORGANIZATION_SERVICE + 0x0C;

        /// <summary>
        /// 根组织结构不能修改
        /// </summary>
        public const uint ORG_ROOT_CANNOT_EDIT = ERR_ORGANIZATION_SERVICE + 0x0D;

        /// <summary>
        /// 根组织不能删除
        /// </summary>
        public const uint ORG_ROOT_CANNOT_DEL = ERR_ORGANIZATION_SERVICE + 0x0E;

        /// <summary>
        /// 待删除组织内存在操作用户，不能被删除
        /// </summary>
        public const uint ORG_EXIST_OPUSER = ERR_ORGANIZATION_SERVICE + 0x0F;

        //////////////////////////////////////////////////////////////////////////
        //用户服务：权限组数据错误码0x50202000
        //////////////////////////////////////////////////////////////////////////

        /// <summary>
        /// 权限组错误码
        /// </summary>
        public const uint ERR_PERMGROUP_SERVICE = 0x50202000;

        /// <summary>
        /// 用户类型名称重复
        /// </summary>
        public const uint PERMGROUP_NAME_REPEAT = ERR_PERMGROUP_SERVICE + 1;

        /// <summary>
        /// 用户类型不存在
        /// </summary>
        public const uint PERMGOUP_ISNOT_EXIT = ERR_PERMGROUP_SERVICE + 2;

        /// <summary>
        /// 用户类型名称不能修改
        /// </summary>
        public const uint PERMGOUP_NAME_DENY_EDIT = ERR_PERMGROUP_SERVICE + 3;

        /// <summary>
        /// 不能修改默认的权限组
        /// </summary>
        public const uint PERMGOUP_DEFAULT_DENY_EDIT = ERR_PERMGROUP_SERVICE + 4;

        /// <summary>
        /// 不能删除默认的权限组
        /// </summary>
        public const uint PERMGOUP_DEFAULT_DENY_DEL = ERR_PERMGROUP_SERVICE + 5;

        /// <summary>
        ///  有属于此用户类型的用户，不能删除该用户类型
        /// </summary>
        public const uint PERMGOUP_NOT_EMPTY = ERR_PERMGROUP_SERVICE + 6;

        /// <summary>
        /// 权限组名称不能为空
        /// </summary>
        public const uint PERMGROUP_NAME_NOT_NULL = ERR_PERMGROUP_SERVICE + 7;

        /// <summary>
        /// 数据版本号冲突
        /// </summary>
        public const uint PERMGROUP_DATAVER_ERR = ERR_PERMGROUP_SERVICE + 8;


        //////////////////////////////////////////////////////////////////////////
        //用户服务：用户服务日志描述0x50203000
        //////////////////////////////////////////////////////////////////////////
        /// <summary>
        /// 用户服务日志记录描述错误码
        /// </summary>
        public const uint ERR_USER_SERVICE_LOG = 0x50203000;

        /// <summary>
        /// 添加用户{username}{0}成功
        /// </summary>
        public const uint ERR_USER_SERVICE_LOG_ADDUSER_SUCCESS = 0x50203000 + 0x1;
        /// <summary>
        /// 添加用户{username}{0}失败，原因：{err}{0}
        /// </summary>
        public const uint ERR_USER_SERVICE_LOG_ADDUSER_FAILED = 0x50203000 + 0x2;

        /// <summary>
        /// 删除用户{username}{0}成功
        /// </summary>
        public const uint ERR_USER_SERVICE_LOG_DELUSER_SUCCESS = 0x50203000 + 0x3;

        /// <summary>
        /// 删除用户{username}{0}失败，原因：{err}{0}
        /// </summary>
        public const uint ERR_USER_SERVICE_LOG_DELUSER_FAILED = 0x50203000 + 0x4;

        /// <summary>
        /// 编辑用户{username}{0}成功
        /// </summary>
        public const uint ERR_USER_SERVICE_LOG_EDITUSER_SUCCESS = 0x50203000 + 0x5;

        /// <summary>
        /// 编辑用户{username}{0}失败，原因：{err}{0}
        /// </summary>
        public const uint ERR_USER_SERVICE_LOG_EDITUSER_FAILED = 0x50203000 + 0x6;


        /// <summary>
        /// 添加组织结构{orgname}{0}成功
        /// </summary>
        public const uint ERR_USER_SERVICE_LOG_ADDORG_SUCCESS = 0x50203000 + 0x7;

        /// <summary>
        /// 添加组织结构{orgname}{0}失败，原因：{err}{0}
        /// </summary>
        public const uint ERR_USER_SERVICE_LOG_ADDORG_FAILED = 0x50203000 + 0x8;

        /// <summary>
        /// 删除组织结构{orgname}{0}成功
        /// </summary>
        public const uint ERR_USER_SERVICE_LOG_DELORG_SUCCESS = 0x50203000 + 0x9;

        /// <summary>
        /// 删除组织结构{orgname}{0}失败，原因：{err}{0}
        /// </summary>
        public const uint ERR_USER_SERVICE_LOG_DELORG_FAILED = 0x50203000 + 0xA;

        /// <summary>
        /// 编辑组织结构{orgname}{0}成功
        /// </summary>
        public const uint ERR_USER_SERVICE_LOG_EDITORG_SUCCESS = 0x50203000 + 0xB;

        /// <summary>
        /// 编辑组织结构{orgname}{0}失败，原因：{err}{0}
        /// </summary>
        public const uint ERR_USER_SERVICE_LOG_EDITORG_FAILED = 0x50203000 + 0xC;

        /// <summary>
        /// 重置用户{username}{0} 密码成功
        /// </summary>
        public const uint ERR_USER_SERVICE_LOG_RESETPWD_SUCCESS = 0x50203000 + 0xD;

        /// <summary>
        /// 重置用户{username}{0} 密码失败，原因：{err}{0}
        /// </summary>
        public const uint ERR_USER_SERVICE_LOG_RESETPWD_FAILED = 0x50203000 + 0xE;

        /// <summary>
        /// 用户{username}{0}认证成功
        /// </summary>
        public const uint ERR_USER_SERVICE_LOG_USERLOGIN_SUCCESS = 0x50203000 + 0xF;

        /// <summary>
        /// 用户{username}{0}认证失败，原因：{err}{0}
        /// </summary>
        public const uint ERR_USER_SERVICE_LOG_USERLOGIN_FAILED = 0x50203000 + 0x10;

        /// <summary>
        /// 第三方用户{username}{0}认证成功
        /// </summary>
        public const uint ERR_USER_SERVICE_LOG_THIRD_USERLOGIN_SUCCESS = 0x50203000 + 0x11;

        /// <summary>
        /// 第三方用户{username}{0}认证失败，原因：{err}{0}
        /// </summary>
        public const uint ERR_USER_SERVICE_LOG_THIRD_USERLOGIN_FAILED = 0x50203000 + 0x12;

        /// <summary>
        /// OA用户{username}{0}认证成功
        /// </summary>
        public const uint ERR_USER_SERVICE_LOG_OA_USERLOGIN_SUCCESS = 0x50203000 + 0x13;

        /// <summary>
        /// OA用户{username}{0}认证失败，原因：{err}{0}
        /// </summary>
        public const uint ERR_USER_SERVICE_LOG_OA_USERLOGIN_FAILED = 0x50203000 + 0x14;

        /// <summary>
        /// 添加用户类型{permissionname}{0}成功
        /// </summary>
        public const uint ERR_USER_SERVICE_LOG_ADDPERMIS_SUCCESS = 0x50203000 + 0x15;

        /// <summary>
        /// 添加用户类型{permissionname}{0}失败，原因：{err}{0}
        /// </summary>
        public const uint ERR_USER_SERVICE_LOG_ADDPERMIS_FAILED = 0x50203000 + 0x16;

        /// <summary>
        /// 删除用户类型{permissionname}{0}成功
        /// </summary>
        public const uint ERR_USER_SERVICE_LOG_DELPERMIS_SUCCESS = 0x50203000 + 0x17;

        /// <summary>
        /// 删除用户类型{permissionname}{0}失败，原因：{err}{0}
        /// </summary>
        public const uint ERR_USER_SERVICE_LOG_DELPERMIS_FAILED = 0x50203000 + 0x18;

        /// <summary>
        /// 编辑用户类型{permissionname}{0}成功
        /// </summary>
        public const uint ERR_USER_SERVICE_LOG_EDITPERMIS_SUCCESS = 0x50203000 + 0x19;

        /// <summary>
        /// 编辑用户类型{permissionname}{0}失败，原因：{err}{0}
        /// </summary>
        public const uint ERR_USER_SERVICE_LOG_EDITPERMIS_FAILED = 0x50203000 + 0x1A;

        /// <summary>
        /// 修改用户{username}{0}的密码成功
        /// </summary>
        public const uint ERR_USER_SERVICE_LOG_EDITPWD_SUCCESS = 0x50203000 + 0x20;

        /// <summary>
        /// 修改用户{username}{0}的密码失败，原因：{err}{0}
        /// </summary>
        public const uint ERR_USER_SERVICE_LOG_EDITPWD_FAILED = 0x50203000 + 0x21;

        /// <summary>
        /// 锁定用户{username}{0} 成功
        /// </summary>
        public const uint ERR_USER_SERVICE_LOG_LOCKUSER_SUCCESS = 0x50203000 + 0x22;

        /// <summary>
        /// 锁定用户{username}{0} 失败，原因：{err}{0}
        /// </summary>
        public const uint ERR_USER_SERVICE_LOG_LOCKUSER_FAILED = 0x50203000 + 0x23;

        /// <summary>
        /// 解锁用户{username}{0} 成功
        /// </summary>
        public const uint ERR_USER_SERVICE_LOG_UNLOCKUSER_SUCCESS = 0x50203000 + 0x24;

        /// <summary>
        /// 解锁用户{username}{0} 失败，原因：{err}{0}
        /// </summary>
        public const uint ERR_USER_SERVICE_LOG_UNLOCKUSER_FAILED = 0x50203000 + 0x25;

        /// <summary>
        /// 查询所有在线人数失败，返回为null或0
        /// </summary>
        public const uint ERR_USER_SERVICE_LOG_QUERYALLUSER_FAILED = 0x50203000 + 0x26;
        /// <summary>
        /// 修改会议模板的管理员为"{siteownername}{1}"
        /// </summary>
        public const uint ERR_USER_SERVICE_LOG_EDITCONFTEMPLATEOWNER_SUCCESS = 0x50203000 + 0x27;
        /// <summary>
        /// 修改会议模板管理员为"{siteownername}{1}失败"，原因：{err}{0}
        /// </summary>
        public const uint ERR_USER_SERVICE_LOG_EDITCONFTEMPLATEOWNER_FAILED = 0x50203000 + 0x28;

        /// <summary>
        /// 修改会议管理员为"{siteownername}{0}"
        /// </summary>
        public const uint ERR_USER_SERVICE_LOG_EDITCONFOWNER_SUCCESS = 0x50203000 + 0x29;
        /// <summary>
        /// 修改会议管理员为"{siteownername}{0}"失败，原因：{err}{0}
        /// </summary>
        public const uint ERR_USER_SERVICE_LOG_EDITCONFOWNER_FAILED = 0x50203000 + 0x2A;

        /// <summary>
        /// 添加MCU"{strname}{2}"到组织"{orgname}{1}"中
        /// </summary>
        public const uint ERR_USER_SERVICE_LOG_ADDMCURELATIONSINORG_SUCCESS = 0x50203000 + 0x2B;
        /// <summary>
        /// 添加MCU"{strname}{2}"到组织"{orgname}{1}"中失败，原因：{err}{0}
        /// </summary>
        public const uint ERR_USER_SERVICE_LOG_ADDMCURELATIONSINORG_FAILED = 0x50203000 + 0x2C;

        /// <summary>
        /// 删除MCU"{strname}{2}"从组织"{orgname}{1}"中
        /// </summary>
        public const uint ERR_USER_SERVICE_LOG_DELMCURELATIONSINORG_SUCCESS = 0x50203000 + 0x2D;

        /// <summary>
        /// 删除MCU"{strname}{2}"从组织"{orgname}{1}"中失败，原因：{err}{0}
        /// </summary>
        public const uint ERR_USER_SERVICE_LOG_DELMCURELATIONSINORG_FAILED = 0x50203000 + 0x2E;

        /// <summary>
        /// 添加录播服务器"{strname}{2}"到组织"{orgname}{1}"中
        /// </summary>
        public const uint ERR_USER_SERVICE_LOG_ADDNRSRELATIONSINORG_SUCCESS = 0x50203000 + 0x2F;

        /// <summary>
        ///添加录播服务器"{strname}{2}"到组织"{orgname}{1}"中失败，原因：{err}{0}
        /// </summary>
        public const uint ERR_USER_SERVICE_LOG_ADDNRSRELATIONSINORG_FAILED = 0x50203000 + 0x30;

        /// <summary>
        /// 添加会场"{strname}{2}"到组织"{orgname}{1}"中
        /// </summary>
        public const uint ERR_USER_SERVICE_LOG_ADDSITERELATIONSINORG_SUCCESS = 0x50203000 + 0x31;

        /// <summary>
        ///添加会场"{strname}{2}"到组织"{orgname}{1}"中失败，原因：{err}{0}
        /// </summary>
        public const uint ERR_USER_SERVICE_LOG_ADDSITERELATIONSINORG_FAILED = 0x50203000 + 0x32;

        /// <summary>
        /// 添加用户"{strname}{2}"到组织"{orgname}{1}"中
        /// </summary>
        public const uint ERR_USER_SERVICE_LOG_ADDUSERRELATIONSINORG_SUCCESS = 0x50203000 + 0x33;

        /// <summary>
        ///添加用户"{strname}{2}"到组织"{orgname}{1}"中失败，原因：{err}{0}
        /// </summary>
        public const uint ERR_USER_SERVICE_LOG_ADDUSERRELATIONSINORG_FAILED = 0x50203000 + 0x34;

        /// <summary>
        /// 删除录播服务器"{strname}{2}"从组织"{orgname}{1}"中
        /// </summary>
        public const uint ERR_USER_SERVICE_LOG_DELNRSRELATIONSINORG_SUCCESS = 0x50203000 + 0x35;

        /// <summary>
        /// 删除录播服务器"{strname}{2}"从组织"{orgname}{1}"中失败，原因：{err}{0}
        /// </summary>
        public const uint ERR_USER_SERVICE_LOG_DELNRSRELATIONSINORG_FAILED = 0x50203000 + 0x36;

        /// <summary>
        /// 删除会场"{strname}{2}"从组织"{orgname}{1}"中
        /// </summary>
        public const uint ERR_USER_SERVICE_LOG_DELSITERELATIONSINORG_SUCCESS = 0x50203000 + 0x37;

        /// <summary>
        /// 删除会场"{strname}{2}"从组织"{orgname}{1}"中失败，原因：{err}{0}
        /// </summary>
        public const uint ERR_USER_SERVICE_LOG_DELSITERELATIONSINORG_FAILED = 0x50203000 + 0x38;

        /// <summary>
        /// 删除用户"{strname}{2}"从组织"{orgname}{1}"中
        /// </summary>
        public const uint ERR_USER_SERVICE_LOG_DELUSERRELATIONSINORG_SUCCESS = 0x50203000 + 0x39;

        /// <summary>
        /// 删除用户"{strname}{2}"从组织"{orgname}{1}"中失败，原因：{err}{0}
        /// </summary>
        public const uint ERR_USER_SERVICE_LOG_DELUSERRELATIONSINORG_FAILED = 0x50203000 + 0x3A;

        /// <summary>
        /// 修改会场{devicename}{0}的组织关系成功
        /// </summary>
        public const uint ERR_USER_SERVICE_LOG_SITE_EDITORGRELATIONS_SUCC = 0x50203000 + 0x3B;

        /// <summary>
        /// 修改会场{devicename}{0}的组织关系失败。失败原因：{exception}{0}
        /// </summary>
        public const uint ERR_USER_SERVICE_LOG_SITE_EDITORGRELATIONS_FAIL = 0x50203000 + 0x3C;

        /// <summary>
        /// 修改会场{devicename}的组织关系发生异常。异常原因：{exception}{1}
        /// </summary>
        public const uint ERR_USER_SERVICE_LOG_SITE_EDITORGRELATIONS_EXP = 0x50203000 + 0x3D;

        /// <summary>
        /// 取消会场{devicename}{0}的组织关系成功
        /// </summary>
        public const uint ERR_USER_SERVICE_LOG_SITE_CANCELORGRELATIONS_SUCC = 0x50203000 + 0x3E;

        /// <summary>
        /// 取消会场{devicename}{0}的组织关系失败。失败原因：{exception}{0}
        /// </summary>
        public const uint ERR_USER_SERVICE_LOG_SITE_CANCELORGRELATIONS_FAIL = 0x50203000 + 0x3F;

        /// <summary>
        /// 取消会场{devicename}的组织关系发生异常。异常原因：{exception}{1}
        /// </summary>
        public const uint ERR_USER_SERVICE_LOG_SITE_CANCELORGRELATIONS_EXP = 0x50203000 + 0x40;

        /// <summary>
        /// 修改MCU{devicename}{0}的组织关系成功
        /// </summary>
        public const uint ERR_USER_SERVICE_LOG_MCU_EDITORGRELATIONS_SUCC = 0x50203000 + 0x41;

        /// <summary>
        /// 修改MCU{devicename}{0}的组织关系失败。失败原因：{exception}{0}
        /// </summary>
        public const uint ERR_USER_SERVICE_LOG_MCU_EDITORGRELATIONS_FAIL = 0x50203000 + 0x42;

        /// <summary>
        /// 修改MCU{devicename}{0}的组织关系发生异常。异常原因：{exception}{0}
        /// </summary>
        public const uint ERR_USER_SERVICE_LOG_MCU_EDITORGRELATIONS_EXP = 0x50203000 + 0x43;

        /// <summary>
        /// 取消MCU{devicename}{0}的组织关系成功
        /// </summary>
        public const uint ERR_USER_SERVICE_LOG_MCU_CANCELORGRELATIONS_SUCC = 0x50203000 + 0x44;

        /// <summary>
        /// 取消MCU{devicename}{0}的组织关系失败。失败原因：{exception}{0}
        /// </summary>
        public const uint ERR_USER_SERVICE_LOG_MCU_CANCELORGRELATIONS_FAIL = 0x50203000 + 0x45;

        /// <summary>
        /// 取消MCU{devicename}{0}的组织关系发生异常。异常原因：{exception}{0}
        /// </summary>
        public const uint ERR_USER_SERVICE_LOG_MCU_CANCELORGRELATIONS_EXP = 0x50203000 + 0x46;

        /// <summary>
        /// 修改录播服务器{devicename}的组织关系成功
        /// </summary>
        public const uint ERR_USER_SERVICE_LOG_NRS_EDITORGRELATIONS_SUCC = 0x50203000 + 0x47;

        /// <summary>
        /// 修改录播服务器{devicename}的组织关系失败。失败原因：{exception}{1}
        /// </summary>
        public const uint ERR_USER_SERVICE_LOG_NRS_EDITORGRELATIONS_FAIL = 0x50203000 + 0x48;

        /// <summary>
        /// 修改录播服务器{devicename}{0}的组织关系发生异常。异常原因：{exception}{0}
        /// </summary>
        public const uint ERR_USER_SERVICE_LOG_NRS_EDITORGRELATIONS_EXP = 0x50203000 + 0x49;

        /// <summary>
        /// 取消录播服务器{devicename}{0}的组织关系成功
        /// </summary>
        public const uint ERR_USER_SERVICE_LOG_NRS_CANCELORGRELATIONS_SUCC = 0x50203000 + 0x4A;

        /// <summary>
        /// 取消录播服务器{devicename}{0}的组织关系失败。失败原因：{exception}{0}
        /// </summary>
        public const uint ERR_USER_SERVICE_LOG_NRS_CANCELORGRELATIONS_FAIL = 0x50203000 + 0x4B;

        /// <summary>
        /// 取消录播服务器{devicename}{0}的组织关系发生异常。异常原因：{exception}{0}
        /// </summary>
        public const uint ERR_USER_SERVICE_LOG_NRS_CANCELORGRELATIONS_EXP = 0x50203000 + 0x4C;

        /// <summary>
        ///添加用户"{strname}{0}"的组织关系"{orgname}{0}"成功
        /// </summary>
        public const uint ERR_USER_SERVICE_LOG_ADDORGRELATIONSINUSER_SUCCESS = 0x50203000 + 0x4D;

        /// <summary>
        /// 添加用户"{strname}{0}"的组织关系"{orgname}{0}"失败，原因：{err}{0}
        /// </summary>
        public const uint ERR_USER_SERVICE_LOG_ADDORGRELATIONSINUSER_FAILED = 0x50203000 + 0x4E;

        /// <summary>
        /// 删除用户"{strname}{0}"的组织关系"{orgname}{0}"成功
        /// </summary>
        public const uint ERR_USER_SERVICE_LOG_DELORGRELATIONSINUSER_SUCCESS = 0x50203000 + 0x4F;

        /// <summary>
        /// 删除用户"{strname}{0}"的组织关系"{orgname}{0}"失败，原因：{err}{0}
        /// </summary>
        public const uint ERR_USER_SERVICE_LOG_DELORGRELATIONSINUSER_FAILED = 0x50203000 + 0x50;

        /// <summary>
        /// 修改用户"{strname}{0}"的类型为"{orgname}{0}"成功
        /// </summary>
        public const uint ERR_USER_SERVICE_LOG_EDITUSERTYPE_SUCCESS = 0x50203000 + 0x51;

        /// <summary>
        /// 修改用户"{strname}{0}"的类型为"{orgname}{0}"失败，原因：{err}{0}
        /// </summary>
        public const uint ERR_USER_SERVICE_LOG_EDITUSERTYPE_FAILED = 0x50203000 + 0x52;
        #endregion

        #region 会场服务 0x50300000
        //////////////////////////////////////////////////////////////////////////
        //会场服务错误码0x50300000
        //////////////////////////////////////////////////////////////////////////

        /// <summary>
        /// 会场服务错误码0x50300000
        /// </summary>
        public const uint ERR_SITE_SERVICE = 0x50300000;

        /// <summary>
        /// 会场不存在
        /// </summary>
        public const uint SITE_ISNOT_EXIST = ERR_SITE_SERVICE + 1;

        /// <summary>
        /// 会场IP地址重复
        /// </summary>
        public const uint SITE_IPADDR_IS_EXIST = ERR_SITE_SERVICE + 2;

        /// <summary>
        /// 会场个数超过最大个数
        /// </summary>
        public const uint SITE_OVER_MAX_NUM = ERR_SITE_SERVICE + 3;

        /// <summary>
        /// E1/4E1会场的IP地址重复
        /// </summary>
        public const uint SITE_MCU_ALREADY_EXIST = ERR_SITE_SERVICE + 4;

        /// <summary>
        /// 会场名称重复
        /// </summary>
        public const uint SITE_NAME_ALREADY_EXIST = ERR_SITE_SERVICE + 5;

        /// <summary>
        /// 会场号码重复
        /// </summary>
        public const uint SITE_CODE_ALREADY_EXIST = ERR_SITE_SERVICE + 6;

        /// <summary>
        /// 会场类型无法识别
        /// </summary>
        public const uint SITE_TYPE_UNKNOW = ERR_SITE_SERVICE + 7;

        /// <summary>
        /// 从数据库获取的会场ID和版本号不正确
        /// </summary>
        public const uint SITE_FAIL_GET_SITEID = ERR_SITE_SERVICE + 8;

        /// <summary>
        /// 会场名称超过最大长度
        /// </summary>
        public const uint SITE_NAME_OVER_MAX_LEN = ERR_SITE_SERVICE + 9;

        /// <summary>
        /// IP格式不正确
        /// </summary>
        public const uint SITE_IPADDRESS_ERR = ERR_SITE_SERVICE + 0x0A;

        /// <summary>
        /// 名称不能为空
        /// </summary>
        public const uint SITE_NAME_EMPTY = ERR_SITE_SERVICE + 0x0B;

        /// <summary>
        /// 会场密码只能为数字
        /// </summary>
        public const uint SITE_PWD_NUMBER_ONLY = ERR_SITE_SERVICE + 0x0C;

        /// <summary>
        /// 密码不为空
        /// </summary>
        public const uint SITE_PASSWORD_EMPTY = ERR_SITE_SERVICE + 0x0D;

        /// <summary>
        /// 参数不匹配
        /// </summary>
        public const uint SITE_PARAM_NOT_MATCH = ERR_SITE_SERVICE + 0x0E;

        /// <summary>
        /// ID错误
        /// </summary>
        public const uint SITE_PARAM_ID_ERR = ERR_SITE_SERVICE + 0x0F;

        /// <summary>
        /// 数据版本号冲突
        /// </summary>
        public const uint SITE_DATAVER_ERR = ERR_SITE_SERVICE + 0x10;

        /// <summary>
        /// 会场号码超过最大长度
        /// </summary>
        public const uint SITE_SITECODE_OVER_MAX_LEN = ERR_SITE_SERVICE + 0x11;

        /// <summary>
        /// 会场URI不能为空
        /// </summary>
        public const uint SITE_URI_NOT_EMPTY = ERR_SITE_SERVICE + 0x12;

        /// <summary>
        /// 会场国家区号超过最大长度
        /// </summary>
        public const uint SITE_COUNTRY_CODE_OVER_MAX_LEN = ERR_SITE_SERVICE + 0x13;

        /// <summary>
        /// 会场区号超过最大长度
        /// </summary>
        public const uint SITE_REGION_CODE_OVER_MAX_LEN = ERR_SITE_SERVICE + 0x14;

        /// <summary>
        /// 会场密码超过最大长度
        /// </summary>
        public const uint SITE_PWD_CODE_OVER_MAX_LEN = ERR_SITE_SERVICE + 0x15;

        /// <summary>
        /// 临时会场模板不存在
        /// </summary>
        public const uint SITE_TEMPSITE_NOT_EXIST = ERR_SITE_SERVICE + 0x16;

        /// <summary>
        /// 会场URI格式不正确
        /// </summary>
        public const uint SITE_URI_SEP_ERR = ERR_SITE_SERVICE + 0x17;

        /// <summary>
        /// 通知方式为EMAIL时，邮件地址不能为空
        /// </summary>
        public const uint SITE_EMAIL_NOT_ALLOW_EMPTY = ERR_SITE_SERVICE + 0x18;

        /// <summary>
        /// tp会场逻辑屏号码不能重复
        /// </summary>
        public const uint SITE_TP_LOGIC_CODE_ALREADY_EXIST = ERR_SITE_SERVICE + 0x19;

        /// <summary>
        /// 不识别的设备型号
        /// </summary>
        public const uint SITE_UNKOW_DEVTYPE = ERR_SITE_SERVICE + 0x1A;

        /// <summary>
        /// 会场号码只允许添加一个
        /// </summary>
        public const uint SITE_NOT_ALLOW_MUTIL_CODE = ERR_SITE_SERVICE + 0x1B;

        /// <summary>
        /// 会场URI只允许添加一个
        /// </summary>
        public const uint SITE_NOT_ALLOW_MUTIL_URI = ERR_SITE_SERVICE + 0x1C;

        /// <summary>
        /// 会场Ip只允许添加一个
        /// </summary>
        public const uint SITE_NOT_ALLOW_MUTIL_IP = ERR_SITE_SERVICE + 0x1D;

        /// <summary>
        /// 电子邮件格式不正确
        /// </summary>
        public const uint SITE_EAMIL_FORMAT_ERR = ERR_SITE_SERVICE + 0x1E;

        /// <summary>
        /// 会场URI重复
        /// </summary>
        public const uint SITE_URI_ALREADY_EXIST = ERR_SITE_SERVICE + 0x1F;

        /// <summary>
        /// tp会场逻辑屏URI不能重复
        /// </summary>
        public const uint SITE_TP_LOGIC_URI_ALREADY_EXIST = ERR_SITE_SERVICE + 0x20;

        /// <summary>
        /// tp会场逻辑屏IP不能重复
        /// </summary>
        public const uint SITE_TP_LOGIC_IP_ALREADY_EXIST = ERR_SITE_SERVICE + 0x21;

        /// <summary>
        /// 电子邮件地址超过最大长度
        /// </summary>
        public const uint SITE_EMAIL_OVER_MAX_LENGTH = ERR_SITE_SERVICE + 0x22;

        /// <summary>
        /// 查询Ldap会场出错
        /// </summary>
        public const uint SITE_QUERY_LDAPSITE_ERROR = ERR_SITE_SERVICE + 0x23;

        /// <summary>
        /// 通知方式为SMS时，联系电话不能为空
        /// </summary>
        public const uint SITE_LINK_MAN_NOT_ALLOW_EMPTY = ERR_SITE_SERVICE + 0x24;

        /// <summary>
        /// 联系电话超过最大长度
        /// </summary>
        public const uint SITE_LINK_MAN_OVER_MAX_LENGTH = ERR_SITE_SERVICE + 0x25;

        /// <summary>
        /// CT会场标识不允许为IP形式
        /// </summary>
        public const uint CTSITE_URI_NOT_ALLOW_IP = ERR_SITE_SERVICE + 0x26;

        /// <summary>
        /// 会场唯一标识检查委托为空
        /// </summary>
        public const uint SITE_UI_WINDOW_CHECKID_NULL = ERR_SITE_SERVICE + 0x27;

        /// <summary>
        /// 不可管理转为可管理，设备型号不同
        /// </summary>
        public const uint SITE_UNMANAGER_TO_MANAGER_MODEL_DIFFERERNT = ERR_SITE_SERVICE + 0x28;

        /// <summary>
        /// 测试Ldap会场配置失败
        /// </summary>
        public const uint SITE_TEST_LDAPSITE_ERROR = ERR_SITE_SERVICE + 0x29;

        /// <summary>
        /// ISDN会场只支持1个主机#分机格式
        /// </summary>
        public const uint ISDN_SITE_SUPPORT_NUMBERFOMAT_ERR = ERR_SITE_SERVICE + 0x2A;

        /// <summary>
        /// 会场号码只能为数字
        /// </summary>
        public const uint ISDN_SITE_CODEFOMAT_ERR = ERR_SITE_SERVICE + 0x2B;

        /// <summary>
        /// 会场号码个数应该为xx
        /// </summary>
        public const uint ISDN_NOT_SUPPOTR_CODE_NUM = ERR_SITE_SERVICE + 0x2C;

        /// <summary>
        /// 会场号码长度不能超过最大值
        /// </summary>
        public const uint ISDN_SITE_CODE_LEGHTH_ERR = ERR_SITE_SERVICE + 0x2D;

        /// <summary>
        /// ISDN会场国家号长度过长
        /// </summary>
        public const uint ISDN_COUNTRYCODE_LENGTH_ERR = ERR_SITE_SERVICE + 0x2E;

        /// <summary>
        /// ISDN会场区号长度过长
        /// </summary>
        public const uint ISDN_AREACODE_LENGTH_ERR = ERR_SITE_SERVICE + 0x2F;

        /// <summary>
        /// ISDN会场国家号或区号只能为数字
        /// </summary>
        public const uint ISDN_COUNTRYCODE_OR_AREACODE_FORMAT_ERR = ERR_SITE_SERVICE + 0x30;


        /// <summary>
        /// 会场号码超过最大长度
        /// </summary>
        public const uint SITE_FULLSITECODE_OVER_MAX_LEN = ERR_SITE_SERVICE + 0x31;

        /// <summary>
        /// URI号码超过最大长度
        /// </summary>
        public const uint SITE_FULLURICODE_OVER_MAX_LEN = ERR_SITE_SERVICE + 0x32;

        /// <summary>
        /// IP号码超过最大长度
        /// </summary>
        public const uint SITE_FULLIPCODE_OVER_MAX_LEN = ERR_SITE_SERVICE + 0x33;

        /// <summary>
        /// 同一个线路的ISDN号码必须相同。
        /// </summary>
        public const uint SITE_ISDN_SAME_NUM_SAME_LINE = ERR_SITE_SERVICE + 0x34;
        #endregion

        #region MCU管理错误码0x50400000
        //////////////////////////////////////////////////////////////////////////
        // MCU管理错误码0x50400000
        //////////////////////////////////////////////////////////////////////////

        /// <summary>
        /// MCU管理错误码0x50400000
        /// </summary>
        public const uint ERR_MCUSERVICE = 0x50400000;

        /// <summary>
        /// MCU不存在
        /// </summary>
        public const uint MCU_ISNOT_EXIST = ERR_MCUSERVICE + 1;

        /// <summary>
        /// MCU已存在
        /// </summary> 
        public const uint MCU_IS_ALREADY_EXIST = ERR_MCUSERVICE + 2;

        /// <summary>
        /// Mcu初始组织错误
        /// </summary>
        public const uint MCU_ORIGIN_ORG_ERROR = ERR_MCUSERVICE + 3;

        /// <summary>
        /// Mcu参数错误
        /// </summary>
        public const uint MCU_PARAM_ERROR = ERR_MCUSERVICE + 4;

        /// <summary>
        /// 参数不能为空
        /// </summary>
        public const uint MCU_PARAM_ISNULL = ERR_MCUSERVICE + 5;

        /// <summary>
        /// MCU中用户ID为空
        /// </summary>
        public const uint MCU_PARAM_USERID_ISNULL = ERR_MCUSERVICE + 6;

        /// <summary>
        /// MCU 参数中ID为空
        /// </summary>
        public const uint MCU_PARAM_ID_ISNULL = ERR_MCUSERVICE + 7;

        /// <summary>
        /// MCU参数中名称为空
        /// </summary>
        public const uint MCU_PARAM_NAME_ISNULL = ERR_MCUSERVICE + 8;

        /// <summary>
        /// MCU参数中GKCODE 为空
        /// </summary>
        public const uint MCU_PARAM_GKCODE_ISNULL = ERR_MCUSERVICE + 9;

        /// <summary>
        /// MCU参数中IP为空
        /// </summary>
        public const uint MCU_PARAM_IP_ISNULL = ERR_MCUSERVICE + 0x0A;

        /// <summary>
        /// MCU参数IP重复
        /// </summary>
        public const uint MCU_PARAM_IP_ALREADY_EXIST = ERR_MCUSERVICE + 0x0B;

        /// <summary>
        /// MCU参数名称重复
        /// </summary>
        public const uint MCU_PARAM_NAME_ALREADY_EXIST = ERR_MCUSERVICE + 0x0C;

        /// <summary>
        /// MCU参数ID重复
        /// </summary>
        public const uint MCU_PARAM_ID_ALREADY_EXIST = ERR_MCUSERVICE + 0x0D;

        /// <summary>
        /// MCU类型错误
        /// </summary>
        public const uint MCU_MCUTYPE_ERROR = ERR_MCUSERVICE + 0x0E;

        /// <summary>
        /// MCU资源为空
        /// </summary>
        public const uint MCU_RESOURCE_ISNULL = ERR_MCUSERVICE + 0x0F;

        /// <summary>
        /// MCU上报的消息不合法
        /// </summary>
        public const uint MCU_REPORT_MSG_INVALID = ERR_MCUSERVICE + 0x10;

        /// <summary>
        /// 导播会议会场端口数和会场个数不一致
        /// </summary>
        public const uint MCU_NAVIGATECONF_PORTCOUNT_ERR = ERR_MCUSERVICE + 0x11;

        /// <summary>
        /// 数据库MCU数据版本不一致
        /// </summary>
        public const uint MCU_DATAVER_ERR = ERR_MCUSERVICE + 0x12;

        /// <summary>
        /// MCU上线
        /// </summary>
        public const uint MCU_CONNECT_STATE_ONLINE = ERR_MCUSERVICE + 0x13;

        /// <summary>
        /// MCU离线
        /// </summary>
        public const uint MCU_CONNECT_STATE_OFFLINE = ERR_MCUSERVICE + 0x14;

        /// <summary>
        /// MCU重启
        /// </summary>
        public const uint MCU_CONNECT_STATE_RESTART = ERR_MCUSERVICE + 0x15;

        /// <summary>
        /// MCU主备倒换
        /// </summary>
        public const uint MCU_BACKUPSWICH = ERR_MCUSERVICE + 0x16;

        /// <summary>
        /// MCU发送会控命令等待超时，请稍后重试
        /// </summary>
        public const uint MCU_TIME_OUT_WAIT = ERR_MCUSERVICE + 0x17;

        /// <summary>
        /// GK前缀检查委托为空
        /// </summary>
        public const uint MCU_UI_WINDOW_PREFIXCHECK_NULL = ERR_MCUSERVICE + 0x18;

        /// <summary>
        /// MCU参数连接密码为空
        /// </summary>
        public const uint MCU_PARAM_CONNECTIONPWD_NULL = ERR_MCUSERVICE + 0x19;

        /// <summary>
        /// 9610MCU通信模块错误码起始地址
        /// </summary>
        public const uint ERR_MCUSERVICE_DRIVER = ERR_MCUSERVICE + 0x2000;

        /// <summary>
        /// 连接错误
        /// </summary>
        public const uint MCU_CONNECTING_ERR = ERR_MCUSERVICE_DRIVER + 0x01;

        /// <summary>
        /// 连接超时（网络繁忙）
        /// </summary>
        public const uint MCU_CONNECT_TIMEOUT_ERR = ERR_MCUSERVICE_DRIVER + 0x03;

        /// <summary>
        /// 认证错误
        /// </summary>
        public const uint MCU_AUTH_ERR = ERR_MCUSERVICE_DRIVER + 0x04;

        /// <summary>
        /// 分配会话ID失败
        /// </summary>
        public const uint MCU_ALLOCATE_SESSIONID_ERR = ERR_MCUSERVICE_DRIVER + 0x05;

        /// <summary>
        /// SOAP协议错误
        /// </summary>
        public const uint MCU_SOAP_PROTOCOL_ERR = ERR_MCUSERVICE_DRIVER + 0x06;

        /// <summary>
        /// SOAP未知的异常
        /// </summary>
        public const uint MCU_UNKNOW_EXCEPTION_ERR = ERR_MCUSERVICE_DRIVER + 0x07;

        /// <summary>
        /// SOAP未知的指示
        /// </summary>
        public const uint MCU_UNKOWN_INDICATION = ERR_MCUSERVICE_DRIVER + 0x08;

        /// <summary>
        /// MCU解码参数错误
        /// </summary>
        public const uint MCU_DECODE_PARAM_ERR = ERR_MCUSERVICE_DRIVER + 0x9;

        /// <summary>
        /// MCU编码参数错误
        /// </summary>
        public const uint MCU_ENCODE_PARAM_ERR = ERR_MCUSERVICE_DRIVER + 0xA;

        /// <summary>
        /// 9610MCU不支持的数据类型
        /// </summary>
        public const uint MCU_DATA_NOT_SUPPORT_ERR = ERR_MCUSERVICE_DRIVER + 0xB;

        /// <summary>
        /// SMC不支持的数据类型
        /// </summary>
        public const uint MCU_SMCDATA_NOT_SUPPORT_ERR = ERR_MCUSERVICE_DRIVER + 0xC;

        /// <summary>
        /// MCU会话ID不存在（SMC握手超时次数范围内，MCU刚启动）
        /// </summary>
        public const uint MCU_SESSIONID_NOT_EXIT_ERR = ERR_MCUSERVICE_DRIVER + 0x0D;

        /// <summary>
        /// 9610MCU类型不匹配
        /// </summary>
        public const uint MCU_TYPE9610_NOT_MATCH_ERR = ERR_MCUSERVICE_DRIVER + 0x0E;

        /// <summary>
        /// MCU个数已经达到了最大值
        /// </summary>
        public const uint MCU_COUNT_REACH_MAX = ERR_MCUSERVICE_DRIVER + 0x0F;

        /// <summary>
        /// MCU的PRI号码只能为数字
        /// </summary>
        public const uint MCU_PRI_IS_NUMBER = ERR_MCUSERVICE_DRIVER + 0x10;

        /// <summary>
        /// 通讯Socket错误
        /// </summary>
        public const uint MCU_SOCKET_ERR = MCU_8650_ERR_BASE + 0x04;


        #region 录播服务器错误码0x50405000

        /// <summary>
        /// 录播错误码起始地址
        /// </summary>
        public const uint ERR_NRS_DRIVER = ERR_MCUSERVICE + 0x5000;

        /// <summary>
        /// 录播服务器ID错误
        /// </summary>
        public const uint NRS_PARAM_ID_ERR = ERR_NRS_DRIVER + 0x01;

        /// <summary>
        /// 录播服务器不存在
        /// </summary>
        public const uint NRS_ISNOT_EXIST = ERR_NRS_DRIVER + 0x02;

        /// <summary>
        /// 录播服务器版本号错误
        /// </summary>
        public const uint NRS_DATAVER_ERR = ERR_NRS_DRIVER + 0x03;

        /// <summary>
        /// 录播服务器类型错误
        /// </summary>
        public const uint NRS_NRSTYPE_ERROR = ERR_NRS_DRIVER + 0x04;

        /// <summary>
        /// 录播服务器指定的IP与端口号已存在
        /// </summary>
        public const uint NRS_IP_PORT_ALREADY_EXIST = ERR_NRS_DRIVER + 0x05;

        /// <summary>
        /// 录播认证失败
        /// </summary>
        public const uint NRS_AUTH_ERR = ERR_NRS_DRIVER + 0x06;

        /// <summary>
        /// 连接录播服务器失败
        /// </summary>
        public const uint NRS_CONNECTING_ERR = ERR_NRS_DRIVER + 0x07;

        /// <summary>
        /// 连接录播服务器超时
        /// </summary>
        public const uint NRS_CONNECT_TIMEOUT_ERR = ERR_NRS_DRIVER + 0x08;

        /// <summary>
        /// 录播服务器未知错误
        /// </summary>
        public const uint NRS_UNKNOW_EXCEPTION_ERR = ERR_NRS_DRIVER + 0x09;

        /// <summary>
        /// 录播服务器参数错误
        /// </summary>
        public const uint NRS_PARAM_ERROR = ERR_NRS_DRIVER + 0x0A;

        /// <summary>
        /// 录播服务器已经存在
        /// </summary>
        public const uint NRS_IS_ALREADY_EXIST = ERR_NRS_DRIVER + 0x0B;

        /// <summary>
        /// 录播服务器开会失败
        /// </summary>
        public const uint NRS_START_CONF_FAILED = ERR_NRS_DRIVER + 0x0C;

        /// <summary>
        /// 未知指示消息
        /// </summary>
        public const uint NRS_UNKOWN_INDICATION = ERR_NRS_DRIVER + 0x0D;

        /// <summary>
        /// 临时录播服务器不存在
        /// </summary>
        public const uint NRS_TEMPNRS_NOT_EXIST = ERR_NRS_DRIVER + 0x0E;

        /// <summary>
        /// 录播服务器名称为空
        /// </summary>
        public const uint NRS_NAME_EMPTY = ERR_NRS_DRIVER + 0x0F;

        /// <summary>
        /// 录播服务器名称超过最大长度
        /// </summary>
        public const uint NRS_NAME_OVER_MAX_LEN = ERR_NRS_DRIVER + 0x10;

        /// <summary>
        /// 录播服务器密码为空
        /// </summary>
        public const uint NRS_PASSWORD_EMPTY = ERR_NRS_DRIVER + 0x11;

        /// <summary>
        /// 录播服务器密码超过最大长度
        /// </summary>
        public const uint NRS_PASSWORD_OVER_MAX_LEN = ERR_NRS_DRIVER + 0x12;

        /// <summary>
        /// 录播服务器IP超过最大长度
        /// </summary>
        public const uint NRS_IP_OVER_MAX_LEN = ERR_NRS_DRIVER + 0x13;

        /// <summary>
        /// 录播服务器IP格式有误
        /// </summary>
        public const uint NRS_IP_ERROR = ERR_NRS_DRIVER + 0x14;

        /// <summary>
        /// 录播服务器的前缀为空
        /// </summary>
        public const uint NRS_PARAM_PREFIX_ISNULL = ERR_NRS_DRIVER + 0x15;

        /// <summary>
        /// 录播服务器服务区已存在
        /// </summary>
        public const uint NRS_IN_DOMAINCODE = ERR_NRS_DRIVER + 0x16;

        /// <summary>
        /// 录播服务器上线
        /// </summary>
        public const uint NRS_CONNECT_STATE_ONLINE = ERR_NRS_DRIVER + 0x17;

        /// <summary>
        /// 录播服务器离线
        /// </summary>
        public const uint NRS_CONNECT_STATE_OFFLINE = ERR_NRS_DRIVER + 0x18;

        /// <summary>
        /// 录播服务器重启
        /// </summary>
        public const uint NRS_CONNECT_STATE_RESTART = ERR_NRS_DRIVER + 0x19;

        /// <summary>
        /// 录播服务器不支持离线转码
        /// </summary>
        public const uint NRS_NOT_SUPPORT_OFFLINE_TRANSCODE = ERR_NRS_DRIVER + 0x1A;

        /// <summary>
        /// 录播错误码起始地址
        /// </summary>
        public const uint ERR_NRS_RCDCTRL_DRIVER = ERR_MCUSERVICE + 0x5100;

        #region
        /// <summary>
        /// 会议不存在
        /// </summary>
        public const uint ERR_NRS_RCDCTRL_CONF_NOEXIST = ERR_NRS_RCDCTRL_DRIVER + 0x01;

        /// <summary>
        /// 录制操作已经启动
        /// </summary>
        public const uint ERR_NRS_RCDCTRL_BE_ON = ERR_NRS_RCDCTRL_DRIVER + 0x02;

        /// <summary>
        /// 录播操作没有启动
        /// </summary>
        public const uint ERR_NRS_RCDCTRL_NOTSTART = ERR_NRS_RCDCTRL_DRIVER + 0x03;

        /// <summary>
        /// 录制操作失败
        /// </summary>
        public const uint ERR_NRS_RCDCTRL_FAIL = ERR_NRS_RCDCTRL_DRIVER + 0x04;

        /// <summary>
        /// 会议不存在
        /// </summary>
        public const uint ERR_NRS_LIVECTRL_CONF_NOEXIST = ERR_NRS_RCDCTRL_DRIVER + 0x05;

        /// <summary>
        /// 直播操作已经启动
        /// </summary>
        public const uint ERR_NRS_LIVECTRL_BE_ON = ERR_NRS_RCDCTRL_DRIVER + 0x06;

        /// <summary>
        /// 直播操作没有启动
        /// </summary>
        public const uint ERR_NRS_LIVECTRL_NOTSTART = ERR_NRS_RCDCTRL_DRIVER + 0x07;

        /// <summary>
        /// 直播操作失败
        /// </summary>
        public const uint ERR_NRS_LIVECTRL_FAIL = ERR_NRS_RCDCTRL_DRIVER + 0x08;

        /// <summary>
        /// 会议不支持直播
        /// </summary>
        public const uint ERR_NRS_LIVECTRL_NOTSUPPORT = ERR_NRS_RCDCTRL_DRIVER + 0x09;

        #endregion

        #region 结束会议结果
        /// <summary>
        /// NRS结束会议不存在
        /// </summary>
        public const uint ERR_NRS_ENDCONF_CONF_NOEXIST = ERR_NRS_RCDCTRL_DRIVER + 0x0F;

        /// <summary>
        /// NRS结束会议成功，会议未启动过录制
        /// </summary>
        public const uint ERR_NRS_ENDCONF_NORECORD = ERR_NRS_RCDCTRL_DRIVER + 0x10;

        /// <summary>
        /// NRS结束会议处理失败
        /// </summary>
        public const uint ERR_NRS_ENDCONF_NRS_FAIL = ERR_NRS_RCDCTRL_DRIVER + 0x11;

        /// <summary>
        /// NRS结束会议成功，返回录制文件路径
        /// </summary>
        public const uint ERR_NRS_ENDCONF_NRS_SUCCESS = ERR_NRS_RCDCTRL_DRIVER + 0x12;

        #endregion

        #region 录播服务器停止录制指示

        /// <summary>
        /// 录播服务器未知原因停止录制指示
        /// </summary>
        public const uint ERR_NRS_STOP_RECORD_IND = ERR_NRS_RCDCTRL_DRIVER + 0x13;

        /// <summary>
        /// 录播服务器可用录制空间低于阀值，会议中止录制
        /// </summary>
        public const uint ERR_NRS_STOP_RECORD_IND_TOTAL_SPACE = ERR_NRS_RCDCTRL_DRIVER + 0x14;

        /// <summary>
        /// 会议录制时长达到阀值，会议中止录制
        /// </summary>
        public const uint ERR_NRS_STOP_RECORD_IND_TOTAL_TIME = ERR_NRS_RCDCTRL_DRIVER + 0x15;

        /// <summary>
        /// 会议录制空间达到阀值，会议中止录制
        /// </summary>
        public const uint ERR_NRS_STOP_RECORD_IND_CONF_SPACE = ERR_NRS_RCDCTRL_DRIVER + 0x16;

        #endregion

        #region 录播服务器返回的开会结果

        /// <summary>
        /// 录播服务器没有可以使用的会议室
        /// </summary>
        public const uint ERR_NRS_START_CONF_NOCONFROOM = ERR_NRS_RCDCTRL_DRIVER + 0x17;

        /// <summary>
        /// 录播服务器磁盘空间不足
        /// </summary>
        public const uint ERR_NRS_START_CONF_NOFREEDIST = ERR_NRS_RCDCTRL_DRIVER + 0x18;

        /// <summary>
        /// 录播服务器开会处理失败
        /// </summary>
        public const uint ERR_NRS_START_CONF_FAIL = ERR_NRS_RCDCTRL_DRIVER + 0x19;

        /// <summary>
        /// 录播服务器上会议已存在
        /// </summary>
        public const uint ERR_NRS_START_CONF_EXIST = ERR_NRS_RCDCTRL_DRIVER + 0x1A;
        #endregion

        #endregion
        #endregion

        #region 会议管理服务错误码0x50500000
        //////////////////////////////////////////////////////////////////////////
        // 会议管理服务的错误码0x50500000
        //////////////////////////////////////////////////////////////////////////


        #region 活动会议
        /// <summary>
        /// 会议管理服务的错误码0x50500000
        /// </summary>
        public const uint ERR_CONF_SERVICE = 0x50500000;

        /// <summary>
        /// 参数错误
        /// </summary>
        public const uint CONF_PARAM_ERROR = ERR_CONF_SERVICE + 1;

        /// <summary>
        /// 会议不存在
        /// </summary>
        public const uint CONF_ISNOT_EXIST = ERR_CONF_SERVICE + 2;

        /// <summary>
        /// 会议已存在
        /// </summary>
        public const uint CONF_IS_ALREADY_EXIT = ERR_CONF_SERVICE + 3;

        /// <summary>
        /// 整个逻辑会议结束
        /// </summary>
        public const uint CONF_OVER_SUCCESS = ERR_CONF_SERVICE + 4;

        /// <summary>
        /// 主席（非RM）结束永久会议
        /// </summary>
        public const uint CONF_NONRM_END_PERMANENTCONF = ERR_CONF_SERVICE + 5;

        /// <summary>
        /// 子会议不存在
        /// </summary>
        public const uint CONF_SUBCONF_ISNOT_EXIST = ERR_CONF_SERVICE + 6;

        /// <summary>
        /// 会议已结束
        /// </summary>
        public const uint CONF_IS_ALREADY_END = ERR_CONF_SERVICE + 7;

        /// <summary>
        /// 会议中的会场不存在
        /// </summary>
        public const uint CONF_SITE_ISNOT_EXIST = ERR_CONF_SERVICE + 8;

        /// <summary>
        /// 会议不处去活动状态
        /// </summary>
        public const uint CONF_ISNOT_ONGOING = ERR_CONF_SERVICE + 9;

        /// <summary>
        /// 会议不能被编辑
        /// </summary>
        public const uint CONF_CANNOT_EDIT = ERR_CONF_SERVICE + 0x0A;

        /// <summary>
        /// 正在编辑的会议不能被提交
        /// </summary>
        public const uint CONF_EDITING_CANNOT_COMMIT = ERR_CONF_SERVICE + 0x0B;

        /// <summary>
        /// 正在编辑的会议不存在
        /// </summary>
        public const uint CONF_EDITING_ISNOT_EXIST = ERR_CONF_SERVICE + 0x0C;

        /// <summary>
        /// 子会议中没有会场
        /// </summary>
        public const uint CONF_SUBCONF_NO_SITE = ERR_CONF_SERVICE + 0x0D;

        /// <summary>
        /// 主子会议中没有会场
        /// </summary>
        public const uint CONF_MASTERSUBCONF_NO_SITE = ERR_CONF_SERVICE + 0x0E;

        /// <summary>
        /// 会议类型错误
        /// </summary>
        public const uint CONF_TYPE_ERR = ERR_CONF_SERVICE + 0x0F;

        /// <summary>
        /// 会议召开失败
        /// </summary>
        public const uint CONF_START_FAILED = ERR_CONF_SERVICE + 0x10;

        /// <summary>
        /// 整个逻辑会议召开成功
        /// </summary>
        public const uint CONF_WHOLECONF_START = ERR_CONF_SERVICE + 0x11;

        /// <summary>
        /// 调度失败，这个错误码应该不需要，要使用调度模块返回结果
        /// </summary>
        public const uint CONF_SCHECONF_ERR = ERR_CONF_SERVICE + 0x12;

        /// <summary>
        /// 调度会议mcu为空
        /// </summary>
        public const uint CONF_SCHEMCU_IS_NULL = ERR_CONF_SERVICE + 0x13;

        /// <summary>
        /// 界面输入的ID错误，这个错误码应该不需要
        /// </summary>
        public const uint FIELD_ID_ERROR = ERR_CONF_SERVICE + 0x14;

        /// <summary>
        /// 删除会议中的会场失败
        /// </summary>
        public const uint CONF_REMOVE_SITE_FAIL = ERR_CONF_SERVICE + 0x15;

        /// <summary>
        /// 会场已存在
        /// </summary>
        public const uint CONF_SITE_EXIST = ERR_CONF_SERVICE + 0x16;

        /// <summary>
        /// 会议时长应在调度范围内
        /// </summary>
        public const uint CONF_SCHEDULE_DURATION_ERR = ERR_CONF_SERVICE + 0x17;

        /// <summary>
        /// 会场令牌类型错误
        /// </summary>
        public const uint CONF_SITE_TOKEN_ERROR = ERR_CONF_SERVICE + 0x18;

        /// <summary>
        /// 会场已经在线了，不能再呼入
        /// </summary>
        public const uint CONF_SITE_IS_ONLINE = ERR_CONF_SERVICE + 0x19;

        /// <summary>
        /// 调度的会议为null
        /// </summary>
        public const uint SCHEDULE_CONF_IS_NULL = ERR_CONF_SERVICE + 0x1A;

        /// <summary>
        /// 调度的会议会场参数为null
        /// </summary>
        public const uint SCHEDULE_CONFSITE_IS_NULL = ERR_CONF_SERVICE + 0x1B;

        /// <summary>
        /// 存在活动会议，不能编辑MCU
        /// </summary>
        public const uint CONF_EDITMCU_HAVECONF = ERR_CONF_SERVICE + 0x1C;

        /// <summary>
        /// 存在会议，不能删除MCU
        /// </summary>
        public const uint CONF_DELMCU_HAVECONF = ERR_CONF_SERVICE + 0x1D;

        /// <summary>
        /// 会场离会原因：mcu离线
        /// </summary>
        public const uint CONF_SITE_LEAVEREASON_MCUOFFLINE = ERR_CONF_SERVICE + 0x1E;

        /// <summary>
        /// 会议不能被取消
        /// </summary>
        public const uint CONF_APPROACH_START = ERR_CONF_SERVICE + 0x1F;

        /// <summary>
        /// 会议延长时间不在配置范围内
        /// </summary>
        public const uint CONF_PROLONG_TIME_ERR = ERR_CONF_SERVICE + 0x20;

        /// <summary>
        /// 会议个数超过周期会议支持的最大个数
        /// </summary>
        public const uint CONF_CYCLE_OVER_MAXCOUNT = ERR_CONF_SERVICE + 0x21;

        /// <summary>
        /// 会议已经召开，不允许编辑
        /// </summary>
        public const uint CONF_ACTIVE_NOT_ALLO_EDIT = ERR_CONF_SERVICE + 0x22;

        /// <summary>
        /// 会场离会
        /// </summary>
        public const uint CONF_SITE_LEAVE = ERR_CONF_SERVICE + 0X23;

        /// <summary>
        /// 会议调度不支持pstn、isdn会场
        /// </summary>
        public const uint CONF_NOT_SUPPORT_SITE = ERR_CONF_SERVICE + 0X24;

        /// <summary>
        /// 会议计费码为空
        /// </summary>
        public const uint CONF_BILLINGCODE_ERR = ERR_CONF_SERVICE + 0X25;

        /// <summary>
        /// 会议的计费码不在计费码列表中
        /// </summary>
        public const uint CONT_BILLINGCODE_NOT_FIND = ERR_CONF_SERVICE + 0X26;

        /// <summary>
        /// 结束会议失败（8系列MCU有结束会议失败的情况）
        /// </summary>
        public const uint FAILED_EDN_CONFERENCE = ERR_CONF_SERVICE + 0X27;

        /// <summary>
        /// 8系列MCU不支持设置一画面
        /// </summary>
        public const uint FAILED_SET_ONE_PANE = ERR_CONF_SERVICE + 0x28;

        /// <summary>
        /// 取消会议失败， 与当前会议的密码不一致
        /// </summary>
        public const uint FALIED_CANCLECONF_PASSWORD_NOTMATCH = ERR_CONF_SERVICE + 0x29;

        /// <summary>
        /// 主叫呼集添加会场时，不能添加TP会场辅屏
        /// </summary>
        public const uint CONF_ADD_SECONDARYSITE_ERR = ERR_CONF_SERVICE + 0X2A;

        /// <summary>
        /// 会议名称不能为空
        /// </summary>
        public const uint SCHEDULECONF_NAME_ERR = ERR_CONF_SERVICE + 0X2B;

        /// <summary>
        /// 会议密码长度过长
        /// </summary>
        public const uint SCHEDELECONF_PASSWORD_ERR = ERR_CONF_SERVICE + 0X2C;

        /// <summary>
        /// 会议中会场名称长度过长
        /// </summary>
        public const uint SCHEDELECONF_SITE_NAME_LENGTH_ERR = ERR_CONF_SERVICE + 0X2D;

        /// <summary>
        /// 会议中会场标识长度过长
        /// </summary>
        public const uint SCHEDELECONF_SITE_CODE_LENGTH_ERR = ERR_CONF_SERVICE + 0X2E;

        /// <summary>
        /// 会议计费码错误
        /// </summary>
        public const uint SCHEDELECONF_SITE_BILINGCODE_ERR = ERR_CONF_SERVICE + 0X2F;

        /// <summary>
        /// 周期会议至少有一个逻辑会议
        /// </summary>
        public const uint PERIOD_CONF_ATLEAST_ONE_ERR = ERR_CONF_SERVICE + 0X30;

        /// <summary>
        /// TP会场辅屏主叫呼集失败
        /// </summary>
        public const uint SCHEDULE_NOT_CALLSITE = ERR_CONF_SERVICE + 0X31;

        /// <summary>
        /// ad hoc会议子会议号不能为空
        /// </summary>
        public const uint SCHEDULE_CONF_PRECONFCODE_NULL = ERR_CONF_SERVICE + 0X32;

        /// <summary>
        /// 会场名称不能为空
        /// </summary>
        public const uint SCHEDELECONF_SITE_NAME_NULL = ERR_CONF_SERVICE + 0X33;

        /// <summary>
        /// 会场号码只能是数字
        /// </summary>
        public const uint SCHEDELECONF_SITE_CODEFORMAT_ERR = ERR_CONF_SERVICE + 0X34;

        /// <summary>
        /// 未启动第三方功能
        /// </summary>
        public const uint SCHEDULECONT_NOTSUPPORT_EXTERNAL = ERR_CONF_SERVICE + 0X35;

        /// <summary>
        /// 主叫呼集不能添加TP会场辅屏
        /// </summary>
        public const uint SITECALL_NOT_ADD_SECAND_TPSITE = ERR_CONF_SERVICE + 0X36;

        /// <summary>
        /// 会场离会原因：SMC服务重启
        /// </summary>
        public const uint CONF_SITE_LEAVEREASON_SMC_RESTART = ERR_CONF_SERVICE + 0x37;

        /// <summary>
        /// 主叫呼集不能含有TP辅屏会场
        /// </summary>
        public const uint SITECALL_DONTHAVE_SECAND_TPSITE = ERR_CONF_SERVICE + 0X38;

        /// <summary>
        /// 会议名称超过最大长度
        /// </summary>
        public const uint SCHEDULECONF_NAME_LENGTH_ERR = ERR_CONF_SERVICE + 0X39;

        /// <summary>
        /// 会议开始时间不能为已过去的时间
        /// </summary>
        public const uint CONF_SCHEDULE_TIME_ERR = ERR_CONF_SERVICE + 0x40;

        /// <summary>
        /// 会议处于等待召开的状态，不允许结束会议
        /// </summary>
        public const uint CONF_NOT_ALLOWEND_STARTCONF = ERR_CONF_SERVICE + 0x41;

        /// <summary>
        /// 会场入会(用于显示界面提示消息)
        /// </summary>
        public const uint CALL_SITE_SUCCESS = ERR_CONF_SERVICE + 0x42;

        /// <summary>
        /// 召集会议成功(用于显示界面提示消息)
        /// </summary>
        public const uint SCHEDULE_CONF_SUCCESS = ERR_CONF_SERVICE + 0x43;

        /// <summary>
        /// 会议被结束(用于显示界面提示消息)
        /// </summary>
        public const uint STOP_CONF_SUCCESS = ERR_CONF_SERVICE + 0x44;

        /// <summary>
        /// 会议被取消
        /// </summary>
        public const uint CANCEL_CONF_SUCCESS = ERR_CONF_SERVICE + 0x45;

        /// <summary>
        /// 会议被修改
        /// </summary>
        public const uint EDIT_SCHEDULE_CONF_SUCCESS = ERR_CONF_SERVICE + 0x46;

        /// <summary>
        /// 开始会议失败
        /// </summary>
        public const uint FALIED_START_CONF = ERR_CONF_SERVICE + 0x47;

        /// <summary>
        /// 会议因启动失败或超时重新召开
        /// </summary>
        public const uint CONF_RESTART = ERR_CONF_SERVICE + 0x48;

        /// ad hoc会议有会议密码必须有主席密码
        /// </summary>
        public const uint ADHOC_CONF_NEED_CHAIRMAN_PWD = ERR_CONF_SERVICE + 0x49;

        /// <summary>
        /// 系统加载nlog模块失败
        /// </summary>
        public const uint CONF_FAILED_LOAD_NLOGMODULE = ERR_CONF_SERVICE + 0x50;

        /// <summary>
        /// 周期会议允许子会议的时间出现冲突
        /// </summary>
        public const uint CYCLECONF_SCHEDULETIME_NOT_CONFIICT = ERR_CONF_SERVICE + 0x51;

        /// <summary>
        /// 会议中会场标识错误
        /// </summary>
        public const uint CONF_SITE_URI_ERR = ERR_CONF_SERVICE + 0x52;

        /// <summary>
        /// 会场号码与活动会议中会场号码重复
        /// </summary>
        public const uint CONF_SITECODE_IS_EXIST = ERR_CONF_SERVICE + 0x53;

        /// <summary>
        /// 会场名称与活动会议中会场名称重复
        /// </summary>
        public const uint CONF_SITENAME_IS_EXIST = ERR_CONF_SERVICE + 0x54;

        /// <summary>
        /// 预约会议成功(用于显示界面提示消息)
        /// </summary>
        public const uint BOOKING_CONF_SUCCESS = ERR_CONF_SERVICE + 0x55;

        /// <summary>
        /// 会场已经入会
        /// </summary>
        public const uint CONF_SITE_IS_ALREAD_ONLINE = ERR_CONF_SERVICE + 0x56;

        /// <summary>
        /// 存在活动会议，不能编辑录播服务器
        /// </summary>
        public const uint CONF_EDITNRS_HAVECONF = ERR_CONF_SERVICE + 0x57;

        /// <summary>
        /// 存在会议，不能删除录播服务器
        /// </summary>
        public const uint CONF_DELNRS_HAVECONF = ERR_CONF_SERVICE + 0x58;

        /// <summary>
        /// 录播服务器致命错误
        /// </summary>
        public const uint ERR_NRS_BADLY_LV = ERR_CONF_SERVICE + 0x59;

        /// <summary>
        /// 录播服务器严重错误
        /// </summary>
        public const uint ERR_NRS_ERR_LV = ERR_CONF_SERVICE + 0x5A;

        /// <summary>
        /// 录播服务器一般错误
        /// </summary>
        public const uint ERR_NRS_INFO_LV = ERR_CONF_SERVICE + 0x5B;

        /// <summary>
        /// 录播服务器系统错误
        /// </summary>
        public const uint ERR_NRS_SYS_LV = ERR_CONF_SERVICE + 0x5C;

        /// <summary>
        /// 设置会议{confname}{0}多画面失败，子画面{sitename}{0},失败原因：{err}{0}
        /// </summary>
        public const uint MCU_SET_MULTPICTS_FAILED = ERR_CONF_SERVICE + 0x5D;

        /// <summary>
        /// 录播会议不支持非H264协议
        /// </summary>
        public const uint ERR_NRS_NOT_SUPPORT_VIDEO_PROTOCAL = ERR_CONF_SERVICE + 0x5E;

        public const uint JOINCONFSITE_TYPE_ERR = ERR_CONF_SERVICE + 0x5F;

        /// <summary>
        /// 会场离会
        /// </summary>
        public const uint SITE_LEAVE_CONF = ERR_CONF_SERVICE + 0x60;

        /// <summary>
        /// 周期会议结束时间应早于开始时间
        /// </summary>
        public const uint PERIODCONF_TIME = ERR_CONF_SERVICE + 0x61;

        /// <summary>
        /// 服务区字冠备注信息最大长度不能超过255个字符
        /// </summary>
        public const uint ERR_MAX_DOMAINMEMO_CODE_LENGTH = ERR_CONF_SERVICE + 0x62;

        /// <summary>
        /// 会场MCU和会议主MCU是同一个服务区
        /// </summary>
        public const uint DOMIANIDS_SAME = ERR_CONF_SERVICE + 0x63;

        #endregion

        #region 会议模板
        //////////////////////////////////////////////////////////////////////////
        // 会议模板错误码0x50501000
        //////////////////////////////////////////////////////////////////////////

        /// <summary>
        /// 会议模板错误码的起始值
        /// </summary>
        public const uint ERR_CONFTEMPLATE_SERVICE = 0x50501000;

        /// <summary>
        /// 会议模板已存在
        /// </summary>
        public const uint CONFTEMPLATE_IS_ALREADY_EXIST = ERR_CONFTEMPLATE_SERVICE + 1;

        /// <summary>
        /// 会议模板不存在
        /// </summary>
        public const uint CONFTEMPLATE_ISNOT_EXIST = ERR_CONFTEMPLATE_SERVICE + 2;

        /// <summary>
        /// MCU能力不支持
        /// </summary>
        public const uint CONFTEMPLATE_MCU_ISNOT_SUPPORTED = ERR_CONFTEMPLATE_SERVICE + 3;

        /// <summary>
        /// 会议中会场速率不一致
        /// </summary>
        public const uint CONFTEMPLATE_SITERATE_IS_DIFFERENT = ERR_CONFTEMPLATE_SERVICE + 4;

        /// <summary>
        /// 辅流速率要小于会议总速率
        /// </summary>
        public const uint CONFTEMPLATE_AMCRATE_LESS_CONFRATE = ERR_CONFTEMPLATE_SERVICE + 5;

        /// <summary>
        /// 组播的会场不能锁定视频源
        /// </summary>
        public const uint CONFTEMPLATE_MULTICAST_CANNOT_LOCKED_SITE_VIEW = ERR_CONFTEMPLATE_SERVICE + 6;

        /// <summary>
        /// IP、E1、4E1多通道总数不能大于32个
        /// </summary>
        public const uint CONFTEMPLATE_MULTI_CHANNEL_OVER_MAXNUM = ERR_CONFTEMPLATE_SERVICE + 7;

        /// <summary>
        /// IP、E1、4E1多通道总数多通道不能小于1个
        /// </summary>
        public const uint CONFTEMPLATE_MULTI_CHANNEL_LESS_MINNUM = ERR_CONFTEMPLATE_SERVICE + 8;

        /// <summary>
        /// Adhoc会议不能复制
        /// </summary>
        public const uint CONFTEMPLATE_ADHOC_CANNOT_COPY = ERR_CONFTEMPLATE_SERVICE + 9;

        /// <summary>
        /// 会议的会场个数不能小于2
        /// </summary>
        public const uint CONFTEMPLATE_SITE_COUNT_LESS_MINNUM = ERR_CONFTEMPLATE_SERVICE + 0x0A;

        /// <summary>
        /// adhoc会议模板参数不合法
        /// </summary>
        public const uint CONFTEMPLATE_ADHOC_INVALID_PARAM = ERR_CONFTEMPLATE_SERVICE + 0x0B;

        /// <summary>
        /// 会议模板中已经存在该会场
        /// </summary>
        public const uint CONFSITE_IS_ALREADY_EXIST = ERR_CONFTEMPLATE_SERVICE + 0x0C;

        /// <summary>
        /// 主叫呼叫会议没有账号
        /// </summary>
        public const uint SITECALL_CONF_ACCOUNT_ERR = ERR_CONFTEMPLATE_SERVICE + 0x0D;

        /// <summary>
        /// 主叫呼集调度的持续时间错误
        /// </summary>
        public const uint SITECALL_CONF_TIMEDURATION_ERR = ERR_CONFTEMPLATE_SERVICE + 0x0E;

        /// <summary>
        /// 主叫呼集号码重复
        /// </summary>
        public const uint SITECALL_SITECODE_REPEAT = ERR_CONFTEMPLATE_SERVICE + 0x0F;

        /// <summary>
        /// 主叫呼集会场速率有错
        /// </summary>
        public const uint SITECALL_SITE_RATE_ERR = ERR_CONFTEMPLATE_SERVICE + 0x10;

        /// <summary>
        /// 主叫呼集会场号码错误
        /// </summary>
        public const uint SITECALL_SITE_CODE_ERR = ERR_CONFTEMPLATE_SERVICE + 0x11;

        /// <summary>
        /// 主叫呼集会议视频参数错误
        /// </summary>
        public const uint SITECALL_CONF_VIDEO_ERR = ERR_CONFTEMPLATE_SERVICE + 0x12;

        /// <summary>
        /// 主叫呼集会议id错误
        /// </summary>
        public const uint SITECALL_CONF_ID_ERR = ERR_CONFTEMPLATE_SERVICE + 0x13;

        /// <summary>
        /// 主叫呼集会议密码长度过长
        /// </summary>
        public const uint SITECALL_PWDLENGTH_EXTRA = ERR_CONFTEMPLATE_SERVICE + 0x14;

        /// <summary>
        /// 主叫呼集命令解析错误
        /// </summary>
        public const uint SITECALL_COMMANDTYPE_ERR = ERR_CONFTEMPLATE_SERVICE + 0x15;

        /// <summary>
        /// 主叫呼集参数错误
        /// </summary>
        public const uint SITECALL_PARAM_ERR = ERR_CONFTEMPLATE_SERVICE + 0x16;

        /// <summary>
        /// 主叫呼集索引超出范围
        /// </summary>
        public const uint SITECALL_INDEX_OVERSTEP_ERR = ERR_CONFTEMPLATE_SERVICE + 0x17;

        /// <summary>
        /// adhoc会议模板不能添加非IP的会场
        /// </summary>
        public const uint CONFTEMPLATE_ADHOC_ADDSITE_ERR = ERR_CONFTEMPLATE_SERVICE + 0x18;

        /// <summary>
        /// SIP用户认证错误
        /// </summary>
        public const uint SIP_USER_AUTH_ERR = ERR_CONFTEMPLATE_SERVICE + 0x19;

        /// <summary>
        /// SIP添加会场参数错误
        /// </summary>
        public const uint SIP_ADDSITE_PARAM_ERR = ERR_CONFTEMPLATE_SERVICE + 0x1A;

        /// <summary>
        /// 主叫呼集会场区号错误
        /// </summary>
        public const uint SITECALL_AREACODE_ERR = ERR_CONFTEMPLATE_SERVICE + 0x1B;

        /// <summary>
        /// ADHOC会议会场个数要不大于预留端口数
        /// </summary>
        public const uint ADHOCCONF_SITE_LESS_PORTRESCOURCE_COUNT = ERR_CONFTEMPLATE_SERVICE + 0x1C;

        /// <summary>
        /// 会议模板为null
        /// </summary>
        public const uint CONF_TEMPLATE_IS_NULL = ERR_CONFTEMPLATE_SERVICE + 0x1D;

        /// <summary>
        /// 会议模板中，可用的MCU个数为0
        /// </summary>
        public const uint CONF_TEMPLATE_MCU_IS_NULL = ERR_CONFTEMPLATE_SERVICE + 0x1E;

        /// <summary>
        /// 会议模板参数超出范围
        /// </summary>
        public const uint CONF_TEMPLATE_PARAM_EXCESS_RANGE = ERR_CONFTEMPLATE_SERVICE + 0x1F;

        /// <summary>
        /// check函数为null
        /// </summary>
        public const uint CONF_TEMPLATE_CHACK_NULL = ERR_CONFTEMPLATE_SERVICE + 0x20;

        /// <summary>
        /// 会议模板参数可选的范围超出界限
        /// </summary>
        public const uint CONF_TEMPLATE_PARAM_INDEX_ERR = ERR_CONFTEMPLATE_SERVICE + 0x21;

        /// <summary>
        /// 设置会议模板参数
        /// </summary>
        public const uint CONF_TEMPLATE_SELECTITEM_CHANGE_ERR = ERR_CONFTEMPLATE_SERVICE + 0x22;

        /// <summary>
        /// 会议模板的名称为空
        /// </summary>
        public const uint CONF_TEMPLATE_NAME_ERR = ERR_CONFTEMPLATE_SERVICE + 0x23;

        /// <summary>
        /// 会议模板密码为空
        /// </summary>
        public const uint CONF_TEMPLATE_PWD_ERR = ERR_CONFTEMPLATE_SERVICE + 0x24;

        /// <summary>
        /// 会议模板主席密码为空
        /// </summary>
        public const uint CONF_TEMPLATE_CHAIRMANPWD_ERR = ERR_CONFTEMPLATE_SERVICE + 0x25;

        /// <summary>
        /// 会议模板的视频协议不支持该视频格式
        /// </summary>
        public const uint CONF_TEMPLATE_VIDEOFORMAT_ERR = ERR_CONFTEMPLATE_SERVICE + 0x26;

        /// <summary>
        /// 接入号范围
        /// </summary>
        public const uint CONF_TEMPLATE_PREDEDFIX_ERR = ERR_CONFTEMPLATE_SERVICE + 0x27;

        /// <summary>
        /// 接入号子会议号超出最大范围
        /// </summary>
        public const uint CONF_TEMPLATE_PREDEFSUBCONFCODE = ERR_CONFTEMPLATE_SERVICE + 0x28;

        /// <summary>
        /// 会议模板的会场标识错误
        /// </summary>
        public const uint CONF_TEMPLATE_SITE_IDENTIFIER_ERR = ERR_CONFTEMPLATE_SERVICE + 0x29;

        /// <summary>
        /// 会议模板Email的格式错误
        /// </summary>
        public const uint CONF_TEMPLATE_EMAIL_FORMAT_ERR = ERR_CONFTEMPLATE_SERVICE + 0x2A;

        /// <summary>
        /// 会议模板时长超出范围
        /// </summary>
        public const uint CONF_TEMPLATE_TIME_ERR = ERR_CONFTEMPLATE_SERVICE + 0x2B;

        /// <summary>
        /// 子会议号超出预留范围
        /// </summary>
        public const uint CONF_TEMPLATE_SUBCONFCODE_EXCESS_RANGE = ERR_CONFTEMPLATE_SERVICE + 0x2C;

        /// <summary>
        /// 会议模板的所有者参数错误
        /// </summary>
        public const uint CONF_TEMPLATE_OWNER_PARAM_ERR = ERR_CONFTEMPLATE_SERVICE + 0x2D;

        /// <summary>
        /// 会议模板所有者有重复
        /// </summary>
        public const uint CONF_TEMPLATE_OWNER_REPEAT = ERR_CONFTEMPLATE_SERVICE + 0x2E;

        /// <summary>
        /// adhoc会议端口资源超过最大会场数
        /// </summary>
        public const uint CONF_TEMPLATE_PORTNUM_EXCESS_RANGE = ERR_CONFTEMPLATE_SERVICE + 0x2F;

        /// <summary>
        /// 会议模板中会场超过配置中的最大值
        /// </summary>
        public const uint CONF_TEMPLATE_SITE_EXCESS_MAXVALUE = ERR_CONFTEMPLATE_SERVICE + 0x30;

        /// <summary>
        /// 会议模板版本号冲突
        /// </summary>
        public const uint CONF_TEMPLATE_DATAVER_ERROR = ERR_CONFTEMPLATE_SERVICE + 0x31;

        /// <summary>
        /// 会议模板版本组播IP地址错误
        /// </summary>
        public const uint CONF_TEMPLATE_MULTICAST_IP_ERROR = ERR_CONFTEMPLATE_SERVICE + 0x32;

        /// <summary>
        /// adhoc会议不支持TP会场、CT会场
        /// </summary>
        public const uint ADHOC_CONF_TEMPLATE_NOT_SUPPORT_SITETYPE = ERR_CONFTEMPLATE_SERVICE + 0x33;

        /// <summary>
        /// ADHOC会议，有会议密码时必须有主席密码
        /// </summary>
        public const uint ADHOC_CONF_NEED_CHAIRMAN_PASSWORD = ERR_CONFTEMPLATE_SERVICE + 0x34;

        /// <summary>
        /// 视频方数{max}{0}不能低于视频与会者个数。
        /// </summary>
        public const uint CONF_VIDEOSITE_LESS_VIDEO_PORT = ERR_CONFTEMPLATE_SERVICE + 0x35;

        /// <summary>
        /// VoIP方数{max}{0}不能低于语音会场个数
        /// </summary>
        public const uint CONF_AUDIOSITE_LESS_AUUDIO_PORT = ERR_CONFTEMPLATE_SERVICE + 0x36;

        /// <summary>
        /// 普通会议总端口数不能小于2
        /// </summary>
        public const uint CONF_PORTRESOURCE_LESS_MINNUM = ERR_CONFTEMPLATE_SERVICE + 0x37;

        /// <summary>
        /// 会议速率不支持TP会场能
        /// </summary>
        public const uint CONF_NOT_SUPPORT_TPSITE_RATE = ERR_CONFTEMPLATE_SERVICE + 0x38;

        /// <summary>
        /// 会议模板Email有重复
        /// </summary>
        public const uint CONF_TEMPLATE_EMAIL_REPEAT = ERR_CONFTEMPLATE_SERVICE + 0x39;

        /// <summary>
        /// 主叫呼集会场名称错误
        /// </summary>
        public const uint SITECALL_SITE_NAME_ERR = ERR_CONFTEMPLATE_SERVICE + 0x3A;

        /// <summary>
        /// TP会场标识错误
        /// </summary>
        public const uint CONF_TEMPLATE_TPSITE_IDENTIFIER_ERR = ERR_CONFTEMPLATE_SERVICE + 0x3B;

        /// <summary>
        /// 计费码只能是数字
        /// </summary>
        public const uint CONF_BILINGCODE_FORMAT_ERR = ERR_CONFTEMPLATE_SERVICE + 0x3C;

        /// <summary>
        /// 会议模板名称不在长度范围内
        /// </summary>
        public const uint CONF_NAME_LEGHTH_ERR = ERR_CONFTEMPLATE_SERVICE + 0x3D;

        /// <summary>
        /// 会议模板密码只能为数字
        /// </summary>
        public const uint CONF_TEMPLATE_PWD_FORMAT_ERR = ERR_CONFTEMPLATE_SERVICE + 0x3E;

        /// <summary>
        /// 统一接入号格式错误（XXX@aaa.bbb,XXX只能是数字）
        /// </summary>
        public const uint UNIFIEDCODE_FORMAT_ERR = ERR_CONFTEMPLATE_SERVICE + 0x3F;

        /// <summary>
        /// 统一接入号长度过长
        /// </summary>
        public const uint UNIFIEDCODE_LENGTH_ERR = ERR_CONFTEMPLATE_SERVICE + 0x40;

        /// <summary>
        /// 统一接入号前缀格式错误
        /// </summary>
        public const uint UNIFIEDCODE_PREFIX_FORMAT_ERR = ERR_CONFTEMPLATE_SERVICE + 0x41;

        /// <summary>
        /// 接入号前缀长度超过最大值
        /// </summary>
        public const uint UNIFIEDCODE_PREFIX_LENGTH_ERR = ERR_CONFTEMPLATE_SERVICE + 0x42;

        /// <summary>
        /// 接入号的组成必须为数字
        /// </summary>
        public const uint CONF_TEMPLATE_PREDEDFIX_COMPOSITE_ERR = ERR_CONFTEMPLATE_SERVICE + 0x44;

        /// <summary>
        /// 主流、辅流端口不能重复。
        /// </summary>
        public const uint CONFTEMPLATE_AMCPORT_MAINPORT_ERR = ERR_CONFTEMPLATE_SERVICE + 0x46;

        /// <summary>
        /// 指定接入号不能为0
        /// </summary>
        public const uint CONFTEMPLATE_SUBCONFCODE_CANNOT_IS_ZERO = ERR_CONFTEMPLATE_SERVICE + 0x47;

        /// <summary>
        /// 会议所有者不是系统用户
        /// </summary>
        public const uint CONFTEMPLATE_OWNERS_NOT_EXIST_INSYS = ERR_CONFTEMPLATE_SERVICE + 0x48;

        /// <summary>
        /// 主流、音频端口不能重复。
        /// </summary>
        public const uint CONFTEMPLATE_MAINPORT_AUDIOPORT_ERR = ERR_CONFTEMPLATE_SERVICE + 0x49;

        /// <summary>
        /// 辅流、音频端口不能重复。
        /// </summary>
        public const uint CONFTEMPLATE_AMCPORT_AUDIOPORT_ERR = ERR_CONFTEMPLATE_SERVICE + 0x4A;

        /// <summary>
        /// 添加会场失败
        /// </summary>
        public const uint CONFTEMPLATE_ADDSITE_FAILED_ERR = ERR_CONFTEMPLATE_SERVICE + 0x4B;

        /// <summary>
        /// 批量添加会场
        /// </summary>
        public const uint CONFTEMPLATE_ADDMANYSITE_ERR = ERR_CONFTEMPLATE_SERVICE + 0x4C;

        /// <summary>
        /// Email个数不能超过200个
        /// </summary>
        public const uint EMAIL_NUM_ERR = ERR_CONFTEMPLATE_SERVICE + 0x4D;

        /// <summary>
        /// Email的长度超长
        /// </summary>
        public const uint EMAIL_LENGTH_ERR = ERR_CONFTEMPLATE_SERVICE + 0x4E;

        /// <summary>
        /// 音频端口不能与辅流端口重复
        /// </summary>
        public const uint CONFTEMPLATE_AUDIOPORT_AMCPORT_ERR = ERR_CONFTEMPLATE_SERVICE + 0x4F;

        /// <summary>
        /// 音频端口不能与主流端口重复
        /// </summary>
        public const uint CONFTEMPLATE_AUDIOPORT_MAINPORT_ERR = ERR_CONFTEMPLATE_SERVICE + 0x50;

        /// <summary>
        /// 主流端口不能与辅流端口重复
        /// </summary>
        public const uint CONFTEMPLATE_MAINPORT_AMCPORT_ERR = ERR_CONFTEMPLATE_SERVICE + 0x51;

        /// <summary>
        /// 会场号码重复
        /// </summary>
        public const uint CONFTEMPLATE_SITE_CODE_ALREADY_EXIST = ERR_CONFTEMPLATE_SERVICE + 0x52;

        /// <summary>
        /// 会场URI重复
        /// </summary>
        public const uint CONFTEMPLATE_SITE_URI_ALREADY_EXIST = ERR_CONFTEMPLATE_SERVICE + 0x53;

        /// <summary>
        /// 会场IP重复
        /// </summary>
        public const uint CONFTEMPLATE_SITE_IP_ALREADY_EXIST = ERR_CONFTEMPLATE_SERVICE + 0x54;

        /// <summary>
        /// 会场URI不能为空
        /// </summary>
        public const uint CONFTEMPLATE_SITE_URI_NOT_EMPTY = ERR_CONFTEMPLATE_SERVICE + 0x55;

        /// <summary>
        /// 组播地址不在有效范围内
        /// </summary>
        public const uint CONFTEMPLATE_MULTICAST_NOT_VALID = ERR_CONFTEMPLATE_SERVICE + 0X56;

        /// <summary>
        /// 会议速率不支持CT会场能力
        /// </summary>
        public const uint CONF_NOT_SUPPORT_CTSITE_RATE = ERR_CONFTEMPLATE_SERVICE + 0x57;

        /// <summary>
        /// 普通会议桌面终端个数、视频方和语音方总数不能小于2
        /// </summary>
        public const uint CONF_PORT_TOTAL_LESS_MINNUM = ERR_CONFTEMPLATE_SERVICE + 0x58;

        /// <summary>
        /// 当前会场不是TP会场
        /// </summary>
        public const uint CONF_ISNOT_TP_SITE = ERR_CONFTEMPLATE_SERVICE + 0x59;

        /// <summary>
        /// 端口参数必须为数字
        /// </summary>
        public const uint CONFTEMPLATE_PORT_MUSTBE_NUMBER = ERR_CONFTEMPLATE_SERVICE + 0x60;

        /// <summary>
        /// 与其他号码重复
        /// </summary>
        public const uint UNIFIEDCODE_SITEURI_REPEAT = ERR_CONFTEMPLATE_SERVICE + 0x61;

        /// <summary>
        /// 9610系列MCU不支持TP或者CT会场.请检查会场列表里是否有Tp或者CT会场，或者会议模板里是否勾选了支持多屏Tp会场选项
        /// </summary>
        public const uint MCU9610SERIAL_NOT_SUPPORT_TP = ERR_CONFTEMPLATE_SERVICE + 0x62;

        /// <summary>
        /// 当前MCU为9610，请注意不支持支持多屏Tp或者CT会场
        /// </summary>
        public const uint MCU9610SERIAL_NOT_SUPPORT_TP001 = ERR_CONFTEMPLATE_SERVICE + 0x63;

        /// <summary>
        /// 当前会场列表有Tp或者CT会场，该选项必须勾选
        /// </summary>
        public const uint MCU9610SERIAL_NOT_SUPPORT_TP002 = ERR_CONFTEMPLATE_SERVICE + 0x64;

        /// <summary>
        /// 当前指定的MCU不支持H264HP视频协议
        /// </summary>
        public const uint MCU_NOT_SUPPORT_H264HP = ERR_CONFTEMPLATE_SERVICE + 0x65;

        /// <summary>
        /// 当前指定的MCU不支持组播协议
        /// </summary>
        public const uint MCU_NOT_SUPPORT_MULTICAST = ERR_CONFTEMPLATE_SERVICE + 0x66;

        /// <summary>
        /// MCU不支持的音频协议，请检查会议参数或者参数中是否勾选当前MCU不支持的音频协议
        /// </summary>
        public const uint MCU_NOT_SUPPORT_AUDIO = ERR_CONFTEMPLATE_SERVICE + 0x67;

        /// <summary>
        /// 9系列MCU不支持录播会议
        /// </summary>
        public const uint MCU9610SERIAL_NOT_SUPPORT_RECORD_CONF = ERR_CONFTEMPLATE_SERVICE + 0x68;
        /// <summary>
        /// 9系列MCU不支持TP,CT\ISDN会场
        /// </summary>
        public const uint MCU9610SERIAL_NOT_SUPPORT_SITE = ERR_CONFTEMPLATE_SERVICE + 0x69;

        /// <summary>
        /// ISDN语音与会者个数不能大于ISDN语音方数
        /// </summary>
        public const uint CONF_ISDNAUDIOSITE_LESS_PORT = ERR_CONFTEMPLATE_SERVICE + 0x6A;

        /// <summary>
        /// 普通会议数据终端个数、视频方和语音方总数不能小于2
        /// </summary>
        public const uint CONF_PORT_DATA_TOTAL_LESS_MINNUM = ERR_CONFTEMPLATE_SERVICE + 0x6B;

        /// <summary>
        /// isdn会场不支持的会议速率
        /// </summary>
        public const uint ISDNSITE_NOT_SUPPORT_CONFRATE = ERR_CONFTEMPLATE_SERVICE + 0x6C;

        /// <summary>
        /// 9系列MCU不支持ISDN会场
        /// </summary>
        //public const uint MCU9610SERIAL_NOT_SUPPORT_ISDN_SITE = ERR_CONFTEMPLATE_SERVICE + 0x6D;

        /// <summary>
        /// 会场号码个数不小于XX;
        /// </summary>
        public const uint ISDN_MULTIB_CODE_NUM_ERR = ERR_CONFTEMPLATE_SERVICE + 0x6E;

        /// <summary>
        /// PSTN会场只能有一个号码
        /// </summary>
        public const uint PSTN_SITE_CODENUM_ERR = ERR_CONFTEMPLATE_SERVICE + 0x6F;

        /// <summary>
        /// ISDN会场速率无效
        /// </summary>
        public const uint ISDN_RATE_ERR = ERR_CONFTEMPLATE_SERVICE + 0x70;

        /// <summary>
        /// PSTN会场速率无效
        /// </summary>
        public const uint PSTN_RATE_ERR = ERR_CONFTEMPLATE_SERVICE + 0x71;

        public const uint ECONF_MAX_DATA_TOP_NUM = ERR_CONFTEMPLATE_SERVICE + 0x72;

        /// <summary>
        /// 会议模板中的组播视频格式不能超过会议的视频格式
        /// </summary>
        public const uint CONF_MULTICASE_VIDEO_FORMAT_ERR = ERR_CONFTEMPLATE_SERVICE + 0x73;

        /// <summary>
        /// 会议模板中的组播协议不能高于会议的视频参数
        /// </summary>
        public const uint CONF_MULTICASE_VIDEO_ERR = ERR_CONFTEMPLATE_SERVICE + 0x74;


        /// 外部会议ID只能为数字
        /// </summary>
        public const uint CONF_TEMPLATE_CHACK_CONFEXTERNALID_FORMAT_ERR = ERR_CONFTEMPLATE_SERVICE + 0x75;

        /// <summary>
        /// 外部会议ID不能为0
        /// </summary>
        public const uint CONFTEMPLATE_CONFEXTERNALID_CANNOT_IS_ZERO = ERR_CONFTEMPLATE_SERVICE + 0x76;

        /// <summary>
        /// Adhoc会议接入号不能为空
        /// </summary>
        public const uint ADHOC_CONFCODE_NOT_ALLOW_EMPTY = ERR_CONFTEMPLATE_SERVICE + 0x77;

        /// <summary>
        /// 该输入项必须输入正确的IPV6的组播地址，例如：“FF02:0:0:0:0:0:0:0”
        /// </summary>
        public const uint CONFTEMPLATE_MULTICASTIPV6_NOT_VALID = ERR_CONFTEMPLATE_SERVICE + 0X78;

        /// <summary>
        /// 会议模板需要支持动态添加CT或者TP会场。
        /// </summary>
        public const uint CONF_NEED_SUPPORT_CT = ERR_CONFTEMPLATE_SERVICE + 0x79;

        /// <summary>
        /// PSTN方数{max}{0}不能低于语音会场个数
        /// </summary>
        public const uint CONF_AUDIOSITE_LESS_PSTN_PORT = ERR_CONFTEMPLATE_SERVICE + 0x7A;

        /// <summary>
        /// 辅流会议速率不能低于64K
        /// </summary>
        public const uint CONF_AMC_NOT_LESS_THAN_BANKWITH = ERR_CONFTEMPLATE_SERVICE + 0x7B;
        #endregion

        #region 服务区和字冠
        //////////////////////////////////////////////////////////////////////////
        // 服务区和字冠错误码0x50502000
        //////////////////////////////////////////////////////////////////////////

        /// <summary>
        /// 服务区和字冠错误码起始值
        /// </summary>
        public const uint ERR_CONFDOMAIN_SERVICE = 0x50502000;

        /// <summary>
        /// 服务区参数错误
        /// </summary>
        public const uint CONF_DOMAIN_PARAM_ERR = ERR_CONFDOMAIN_SERVICE + 1;

        /// <summary>
        /// 服务区名称为空
        /// </summary>
        public const uint CONF_DOMAIN_NAME_IS_NULL = ERR_CONFDOMAIN_SERVICE + 2;

        /// <summary>
        /// 服务区已存在
        /// </summary>
        public const uint CONF_DOMAIN_IS_ALREADY_EXIT = ERR_CONFDOMAIN_SERVICE + 3;

        /// <summary>
        /// 服务区不存在
        /// </summary>
        public const uint CONF_DOMAIN_ISNOT_EXIT = ERR_CONFDOMAIN_SERVICE + 4;

        /// <summary>
        /// 服务区名称已存在
        /// </summary>
        public const uint CONF_DOMAIN_NAME_IS_ALREADY_EXIT = ERR_CONFDOMAIN_SERVICE + 5;

        /// <summary>
        /// 字冠参数错误
        /// </summary>
        public const uint CONF_DOMAINCODE_PARAM_ERR = ERR_CONFDOMAIN_SERVICE + 6;

        /// <summary>
        /// 字冠已存在
        /// </summary>
        public const uint CONF_DOMAINCODE_IS_ALREADY_EXIT = ERR_CONFDOMAIN_SERVICE + 7;

        /// <summary>
        /// 字冠不存在
        /// </summary>
        public const uint CONF_DOMAINCODE_ISNOT_EXIT = ERR_CONFDOMAIN_SERVICE + 8;

        /// <summary>
        /// 字冠名称为空
        /// </summary>
        public const uint CONF_DOMAINCODE_NAME_IS_NULL = ERR_CONFDOMAIN_SERVICE + 9;

        /// <summary>
        /// 字冠编号为空
        /// </summary>
        public const uint CONF_DOMAINCODE_CODE_IS_NULL = ERR_CONFDOMAIN_SERVICE + 0x0A;

        /// <summary>
        /// 字冠类型为空
        /// </summary>
        public const uint CONF_DOMAINCODE_TYPE_IS_NULL = ERR_CONFDOMAIN_SERVICE + 0x0B;

        /// <summary>
        /// 字冠名称已存在
        /// </summary>
        public const uint CONF_DOMAINCODE_NAME_IS_ALREADY_EXIT = ERR_CONFDOMAIN_SERVICE + 0x0C;

        /// <summary>
        /// 字冠编号已存在
        /// </summary>
        public const uint CONF_DOMAINCODE_CODE_IS_ALREADY_EXIT = ERR_CONFDOMAIN_SERVICE + 0x0D;

        /// <summary>
        /// 服务区数据库版本号不不一致
        /// </summary>
        public const uint CONF_DOMAIN_DATAVER_ERR = ERR_CONFDOMAIN_SERVICE + 0x0E;

        /// <summary>
        /// 字冠数据库版本号不不一致
        /// </summary>
        public const uint CONF_DOMAINCODE_DATAVER_ERR = ERR_CONFDOMAIN_SERVICE + 0x0F;

        /// <summary>
        /// ISDN前缀国家号、区号、电话号必填一项
        /// </summary>
        public const uint CONF_ISDNDOMAINCODE_ERR = ERR_CONFDOMAIN_SERVICE + 0x10;

        /// <summary>
        /// ISDN前缀只能是数字
        /// </summary>
        public const uint CONF_ISDNDOMAINCODE_FOMAT_ERR = ERR_CONFDOMAIN_SERVICE + 0x11;
        #endregion

        #endregion

        #region 电视墙错误码 0x50503000
        //////////////////////////////////////////////////////////////////////////
        // 电视墙错误码 0x50503000
        //////////////////////////////////////////////////////////////////////////

        /// <summary>
        /// 电视墙错误码起始值
        /// </summary>
        public const uint ERR_TVWALL_SERVICE = 0x50503000;

        /// <summary>
        /// 电视墙不存在
        /// </summary>
        public const uint CONF_TVWALL_ISNOT_EXIST = ERR_TVWALL_SERVICE + 1;

        /// <summary>
        /// 电视墙已存在
        /// </summary>
        public const uint CONF_TVWALL_IS_ALREADY_EXIST = ERR_TVWALL_SERVICE + 2;

        /// <summary>
        /// 会议中不包含指定电视墙
        /// </summary>
        public const uint CONF_NOT_CONTAINS_TVWALL = ERR_TVWALL_SERVICE + 3;

        /// <summary>
        /// 未设置电视墙轮询
        /// </summary>
        public const uint CONF_TVWALL_POLLSITE_IS_NOT_EXIST = ERR_TVWALL_SERVICE + 6;

        /// <summary>
        /// 设置视频源的端口数量与MCU支持的端口数量不一致
        /// </summary>
        public const uint CONF_TVWALL_PORT_ERR = ERR_TVWALL_SERVICE + 8;

        /// <summary>
        /// 获取不到视频源信息
        /// </summary>
        public const uint CONF_TVWALL_GETVIDEOSOURCE_FAILED = ERR_TVWALL_SERVICE + 9;

        /// <summary>
        /// MCU不支持电视墙
        /// </summary>
        public const uint CONF_TVWALL_QUERYMCU_IS_NOT_EXIST = ERR_TVWALL_SERVICE + 0x0A;
        #endregion

        #region Nlog
        //////////////////////////////////////////////////////////////////////////
        // Nlog错误码 0x50504000
        //////////////////////////////////////////////////////////////////////////

        /// <summary>
        /// Nlog错误码起始值
        /// </summary>
        public const uint ERR_NLOG_SERVICE = 0x50504000;

        /// <summary>
        /// Nlog的IP地址错误
        /// </summary>
        public const uint CONF_NLOG_IP_PARAM_ERR = ERR_NLOG_SERVICE + 1;

        /// <summary>
        /// Nlog端口错误
        /// </summary>
        public const uint CONF_NLOG_PORT_PARAM_ERR = ERR_NLOG_SERVICE + 2;

        /// <summary>
        /// Nlog参数错误
        /// </summary>
        public const uint CONF_NLOG_PARAM_ERR = ERR_NLOG_SERVICE + 3;

        /// <summary>
        /// Nlog异常
        /// </summary>
        public const uint CONF_NLOG_CATCH_EXCEPTION = ERR_NLOG_SERVICE + 4;
        #endregion

        #region  第三接口
        /// <summary>
        /// 第三方接口起始值
        /// </summary>
        public const uint ERR_CONF_EXTERNAL = 0x50505000;

        /// <summary>
        /// 第三方接口输入参数为空
        /// </summary>
        public const uint CONF_EXTERNAL_PARAM_NULL_ERR = ERR_CONF_EXTERNAL + 1;

        /// <summary>
        /// 会场列表为空
        /// </summary>
        public const uint CONF_EXTERNAL_SITELIST_NULL_ERR = ERR_CONF_EXTERNAL + 2;

        /// <summary>
        /// 会场为空
        /// </summary>
        public const uint CONF_EXTERNAL_SITE_NULL_ERR = ERR_CONF_EXTERNAL + 3;

        /// <summary>
        /// 会场不存在
        /// </summary>
        public const uint CONF_EXTERNAL_SITE_NOEXIST_ERR = ERR_CONF_EXTERNAL + 4;

        /// <summary>
        /// 会议名称为空
        /// </summary>
        public const uint CONF_EXTERNAL_CONFNAME_NULL_ERR = ERR_CONF_EXTERNAL + 5;

        /// <summary>
        /// 会议速率为空
        /// </summary>
        public const uint CONF_EXTERNAL_CONFRATE_NULL_ERR = ERR_CONF_EXTERNAL + 6;

        /// <summary>
        /// 会议速率格式错误
        /// </summary>
        public const uint CONF_EXTERNAL_CONFRATE_FORMAT_ERR = ERR_CONF_EXTERNAL + 7;

        /// <summary>
        /// 多画面资源数错误
        /// </summary>
        public const uint CONF_EXTERNAL_CONTINUOUSPRESENCERESOURCE_FORMAT_ERR = ERR_CONF_EXTERNAL + 8;

        /// <summary>
        /// 会议中不能存在多个会议室
        /// </summary>
        public const uint CONF_EXTERNAL_OVER_ONE_MEETINGROOM = ERR_CONF_EXTERNAL + 9;

        /// <summary>
        /// 无效的周期会议间隔
        /// </summary>
        public const uint CONF_EXTERNAL_INVALID_PERIODCONF_INTERVAL = ERR_CONF_EXTERNAL + 0x0A;
        #endregion

        #region IVR
        //////////////////////////////////////////////////////////////////////////
        //IVR错误码0x50506000
        //////////////////////////////////////////////////////////////////////////

        /// <summary>
        /// IVR错误码0x50506000
        /// </summary>
        public const uint ERR_IVR_SERVECE = 0x50506000;

        /// <summary>
        /// 请求IVR呼叫请求的会场号码为null
        /// </summary>
        public const uint IVR_SITEPARAM_NULL = ERR_IVR_SERVECE + 1;

        /// <summary>
        /// IVR对应的MCU不存在
        /// </summary>
        public const uint IVR_MCU_IS_NOT_EXIST = ERR_IVR_SERVECE + 2;

        /// <summary>
        /// MCU动态资源不足
        /// </summary>
        public const uint IVR_MCU_DYNAMIC_RESOURCE_NOT_ENOUGH = ERR_IVR_SERVECE + 3;

        /// <summary>
        /// 释放IVR资源失败
        /// </summary>
        public const uint IVR_IS_NOT_EXIST = ERR_IVR_SERVECE + 4;

        /// <summary>
        /// SIP呼叫，mcu非统一接入号指定的MCU
        /// </summary>
        public const uint IVR_MCU_UNIFELDCODE_ERR = ERR_IVR_SERVECE + 5;

        /// <summary>
        /// 统一接入号的预留端口资源不足
        /// </summary>
        public const uint IVR_UNIFELDCODE_PORT_NOT_ENOUGH = ERR_IVR_SERVECE + 6;

        /// <summary>
        /// 会场对应的IVR资源不存在
        /// </summary>
        public const uint IVR_SITE_IS_NOT_EXIST = ERR_IVR_SERVECE + 7;

        /// <summary>
        /// 统一接入号参数错误
        /// </summary>
        public const uint IVR_UNIFIEDCODE_PARAM_ERROR = ERR_IVR_SERVECE + 8;

        /// <summary>
        /// 在活动会议中没有找到会场
        /// </summary>
        public const uint SITE_IS_NOT_FIND_IN_ACTIVECONF = ERR_IVR_SERVECE + 9;

        /// <summary>
        /// 在adhoc会议模板中没有找到会场
        /// </summary>
        public const uint SITE_IS_NOT_FIND_IN_ADHOCCONFTEMPLATE = ERR_IVR_SERVECE + 0x0A;

        /// <summary>
        /// 没有找到adhoc会议模板
        /// </summary>
        public const uint ADHOC_CONF_NOT_FIND = ERR_IVR_SERVECE + 0x0B;

        /// <summary>
        /// adhoc会议已经达到最大个数
        /// </summary>
        public const uint ADHOC_CONF_ALREAD_REACHE_MAX_VALUE = ERR_IVR_SERVECE + 0x0C;

        /// <summary>
        /// adhoc会议接入号重复
        /// </summary>
        public const uint ADHOC_CONFCODE_REPEAT = ERR_IVR_SERVECE + 0x0D;

        /// <summary>
        /// 没有找到活动会议中adhoc会议
        /// </summary>
        public const uint ADHOC_SCHEDULECONF_NOT_FIND = ERR_IVR_SERVECE + 0x0E;

        /// <summary>
        /// IVR接入认证时，没有找到该会场，不能进行IVR接入
        /// </summary>
        public const uint IVR_NOT_FIND_SITE = ERR_IVR_SERVECE + 0x0F;

        /// <summary>
        /// 会场已经分配了IVR
        /// </summary>
        public const uint SITE_ALREADY_ASSIGN_IVR = ERR_IVR_SERVECE + 0x10;

        /// <summary>
        /// 没有IVR资源
        /// </summary>
        public const uint IVR_NOT_EXIST = ERR_IVR_SERVECE + 0x11;

        /// <summary>
        /// TP会场辅屏不能激活、加入会议
        /// </summary>
        public const uint SENCEND_TPSITE_NOT_JOINCONF = ERR_IVR_SERVECE + 0x12;

        /// <summary>
        /// 本版本不支持的接入号长度
        /// </summary>
        public const uint CREAT_ADHOC_CONF_CODE_LENGTH_ERR = ERR_IVR_SERVECE + 0x13;

        /// <summary>
        /// TP会场不能创建会议
        /// </summary>
        public const uint TPSITE_NOT_ACTIVECONF = ERR_IVR_SERVECE + 0x14;

        /// <summary>
        /// 呼叫类型与预定义会场类型不一致
        /// </summary>
        public const uint IVR_SITE_TYPE_ERR = ERR_IVR_SERVECE + 0x15;

        /// <summary>
        /// ISDN会场参数为空
        /// </summary>
        public const uint IVR_ISDN_SITE_NULL = ERR_IVR_SERVECE + 0x16;

        /// <summary>
        /// 终端无视频能力或带宽不足
        /// </summary>
        public const uint IVR_JOINCONFSITE_CAP_NULL = ERR_IVR_SERVECE + 0x17;

        /// <summary>
        /// 会议主MCU不是接入的MCU
        /// </summary>
        public const uint CONF_MAINMCU_ADN_CALLMCU_ERR = ERR_IVR_SERVECE + 0x18;

        /// <summary>
        /// 接入MCU不是会议的主MCU（用于ISDN加入会议时）
        /// </summary>
        public const uint ISDN_CALL_MCU_NOT_CONF_MAINMCU = ERR_IVR_SERVECE + 0x19;

        #endregion

        #region 录播
        /// <summary>
        /// 会议录播错误码起始值
        /// </summary>
        public const uint ERR_NRS_SERVICE = 0x50507000;

        /// <summary>
        /// 录播服务器不存在
        /// </summary>
        public const uint CONF_NRS_ISNOT_EXIST = ERR_NRS_SERVICE + 1;

        /// <summary>
        /// 录播服务器已经存在
        /// </summary>
        public const uint CONF_NRS_IS_ALREADY_EXIST = ERR_NRS_SERVICE + 2;

        /// <summary>
        /// 没有录播服务器与MCU处于同一服务区
        /// </summary>
        public const uint CONF_NRS_QUERYMCU_IS_NOT_EXIST = ERR_NRS_SERVICE + 3;

        /// <summary>
        /// 录制参数有误
        /// </summary>
        public const uint CONF_NRS_RECORD_PARAM_ERR = ERR_NRS_SERVICE + 4;

        /// <summary>
        /// 会议不支持录播
        /// </summary>
        public const uint CONF_NRS_NOT_SUPPORT_RECORDLIVE = ERR_NRS_SERVICE + 5;

        /// <summary>
        /// 未知会控命令
        /// </summary>
        public const uint CONF_NRS_UNKNOW_COMMAND = ERR_NRS_SERVICE + 6;

        /// <summary>
        /// 加载录播服务器失败
        /// </summary>
        public const uint CONF_NRS_LOAD_ERR = ERR_NRS_SERVICE + 7;

        /// <summary>
        /// 调度会议的录播服务器资源不足
        /// </summary>
        public const uint CONF_SCHENRS_IS_NULL = ERR_NRS_SERVICE + 8;

        /// <summary>
        /// 调度会议的录播服务器剩余空间不足
        /// </summary>
        public const uint CONF_SCHENRS_NOT_ENGOUHT_SPACE = ERR_NRS_SERVICE + 9;

        /// <summary>
        /// 会议模板中的录播视频格式不能超过会议的视频格式
        /// </summary>
        public const uint CONF_NRS_VIDEO_FORMAT_ERR = ERR_NRS_SERVICE + 0x0A;

        /// <summary>
        /// 会议模板中的录播带宽不能超过会议的带宽
        /// </summary>
        public const uint CONF_NRS_MEDIA_BANDWIDTH_ERR = ERR_NRS_SERVICE + 0x0B;

        /// <summary>
        /// 录播不支持该辅流视频格式
        /// </summary>
        public const uint CONF_NRS_AMC_VIDEO_FORMAT_ERR = ERR_NRS_SERVICE + 0x0C;

        /// <summary>
        /// 会议的视频格式不能低于录播视频格式
        /// </summary>
        public const uint CONF_VIDEO_LOWWER_THAN_NRS_ERR = ERR_NRS_SERVICE + 0x0D;
        #endregion

        #region 会议管理服务日志记录描述
        /// <summary>
        /// 会议服务日志记录描述
        /// </summary>
        public const uint ERR_CONF_SERVICE_LOG = 0x50508000;

        /// <summary>
        /// 召集会议 {confname}{0}
        /// </summary>
        public const uint ERR_CONF_SERVICE_STARTCONF = 0x50508001;

        /// <summary>
        /// 会议 {confname}{0}召集成功
        /// </summary>
        public const uint ERR_CONF_SERVICE_STARTCONF_SUCCESS = 0x50508002;

        /// <summary>
        /// 会议 {confname}{0}召集失败，原因:{err}{0}
        /// </summary>
        public const uint ERR_CONF_SERVICE_STARTCONF_FAILED = 0x50508003;

        /// <summary>
        /// 主叫呼集 终端{sitename}{0} 召集会议 {confname}{0}
        /// </summary>
        public const uint ERR_CONF_SERVICE_SITECALL_STARTCONF = 0x50508004;

        /// <summary>
        /// 第三方用户{sitename}{0}召集会议 {confname}{0}
        /// </summary>
        public const uint ERR_CONF_SERVICE_THIRDUSER_STARTCONF = 0x50508005;

        /// <summary>
        /// IVR{sitename}{0}召集会议 {confname}{0}
        /// </summary>
        public const uint ERR_CONF_SERVICE_IVRDUSER_STARTCONF = 0x50508006;

        /// <summary>
        /// 结束会议 {confname}{0}
        /// </summary>
        public const uint ERR_CONF_SERVICE_STOPCONF = 0x50508007;

        /// <summary>
        /// 会议{confname}{0}结束成功
        /// </summary>
        public const uint ERR_CONF_SERVICE_STOPCONF_SUCCESS = 0x50508008;

        /// <summary>
        /// 会议 {confname}{0}结束失败，原因:{err}{0}
        /// </summary>
        public const uint ERR_CONF_SERVICE_STOPCONF_FAILED = 0x50508009;

        /// <summary>
        /// 主叫呼集终端{sitename}{0} 结束会议 {confname}{0}
        /// </summary>
        public const uint ERR_CONF_SERVICE_SITECALL_STOPCONF = 0x5050800A;

        /// <summary>
        /// 第三方用户{sitename}{0}结束会议 {confname}{0}
        /// </summary>
        public const uint ERR_CONF_SERVICE_THIRDUSER_STOPCONF = 0x5050800B;


        /// <summary>
        /// 会议{confname}{0}进行声控切换
        /// </summary>
        public const uint ERR_CONF_SERVICE_AUDIOSWITCHCONF = 0x5050800C;

        /// <summary>
        /// 会议{confname}{0}进行声控切换成功
        /// </summary>
        public const uint ERR_CONF_SERVICE_AUDIOSWITCHCONF_SUCCESS = 0x5050800D;

        /// <summary>
        /// 会议{confname}{0}进行声控切换失败，原因：{err}{0}
        /// </summary>
        public const uint ERR_CONF_SERVICE_AUDIOSWITCHCONF_FAILED = 0x5050800E;

        /// <summary>
        /// 设置{confname}{0}的会议多画面
        /// </summary>
        public const uint ERR_CONF_SERVICE_SETCONFCP = 0x5050800F;

        /// <summary>
        /// 设置{confname}{0}的会议多画面成功
        /// </summary>
        public const uint ERR_CONF_SERVICE_SETCONFCP_SUCCESS = 0x50508010;

        /// <summary>
        /// 设置{confname}{0}的会议多画面失败，原因：{err}{0}
        /// </summary>
        public const uint ERR_CONF_SERVICE_SETCONFCP_FAILED = 0x50508011;

        /// <summary>
        /// 广播{confname}{0}多画面
        /// </summary>
        public const uint ERR_CONF_SERVICE_BROACASTCONFCP = 0x50508012;

        /// <summary>
        /// 广播{confname}{0}的会议多画面成功
        /// </summary>
        public const uint ERR_CONF_SERVICE_BROACASCONFCP_SUCCESS = 0x50508013;

        /// <summary>
        /// 广播{confname}{0}的会议多画面失败，原因：{err}{0}
        /// </summary>
        public const uint ERR_CONF_SERVICE_BROACASTCONFCP_FAILED = 0x50508014;

        /// <summary>
        /// 取消广播{confname}{0}的会议多画面
        /// </summary>
        public const uint ERR_CONF_SERVICE_CANCLE_BROACASTCONFCP = 0x50508015;

        /// <summary>
        /// 取消广播{confname}{0}的会议多画面成功
        /// </summary>
        public const uint ERR_CONF_SERVICE_CANCLE_BROACASCONFCP_SUCCESS = 0x50508016;

        /// <summary>
        /// 取消广播{confname}{0}的会议多画面失败，原因：{err}{0}
        /// </summary>
        public const uint ERR_CONF_SERVICE_CANCLE_BROACASTCONFCP_FAILED = 0x50508017;
        #endregion

        #region
        /// <summary>
        /// 呼叫会议 {confname}{0} 中的会场{sitename}{0}
        /// </summary>
        public const uint ERR_CONF_SERVICE_CALLCONFSITE = 0x50508018;

        /// <summary>
        /// 呼叫会议 {confname}{0} 中的会场{sitename}{0}成功
        /// </summary>
        public const uint ERR_CONF_SERVICE_CALLCONFSITE_SUCCESS = 0x50508019;

        /// <summary>
        /// 呼叫会议 {confname}{0} 中的会场{sitename}{0}失败，原因:{err}{0}
        /// </summary>
        public const uint ERR_CONF_SERVICE_CALLCONFSITE_FAILED = 0x5050801A;

        /// <summary>
        /// 挂断会议 {confname}{0} 中的会场{sitename}{0}
        /// </summary>
        public const uint ERR_CONF_SERVICE_HANGUPCONFSITE = 0x5050801B;

        /// <summary>
        /// 挂断会议 {confname}{0} 中的会场{sitename}{0}成功
        /// </summary>
        public const uint ERR_CONF_SERVICE_HANGUPCONFSITE_SUCCESS = 0x5050801C;

        /// <summary>
        /// 挂断 {confname}{0} 挂断会场{sitename}{0}失败，原因:{err}{0}
        /// </summary>
        public const uint ERR_CONF_SERVICE_HANGUPCONF_FAILED = 0x5050801D;

        /// <summary>
        /// 添加会议{confname}{0}中的会场{sitename}{0}
        /// </summary>
        public const uint ERR_CONF_SERVICE_ADDCONFSITE = 0x5050801E;

        /// <summary>
        /// 会议{confname}{0}中的会场{sitename}{0}添加成功
        /// </summary>
        public const uint ERR_CONF_SERVICE_ADDCONFSITE_SUCCESS = 0x5050801F;

        /// <summary>
        /// 会议{confname}{0}中的会场{sitename}{0}添加失败，原因:{err}{0}
        /// </summary>
        public const uint ERR_CONF_SERVICE_ADDCONF_FAILED = 0x50508020;

        /// <summary>
        /// 删除会议{confname}{0} 中的会场{sitename}{0}
        /// </summary>
        public const uint ERR_CONF_SERVICE_DELCONFSITE = 0x50508021;

        /// <summary>
        /// 会议{confname}{0} 中的会场{sitename}{0}删除成功
        /// </summary>
        public const uint ERR_CONF_SERVICE_DELCONFSITE_SUCCESS = 0x50508022;

        /// <summary>
        /// 会议 {confname}{0}中的会场 {sitename}{0}删除失败，原因:{err}{0}
        /// </summary>
        public const uint ERR_CONF_SERVICE_DELCONFSITE_FAILED = 0x50508023;

        /// <summary>
        /// 广播会议{confname}{0} 中的会场{sitename}{0}
        /// </summary>
        public const uint ERR_CONF_SERVICE_BROCASTCONFSITE = 0x50508024;

        /// <summary>
        /// 广播会议{confname}{0} 中的会场{sitename}{0}成功
        /// </summary>
        public const uint ERR_CONF_SERVICE_BROCASTCONFSITE_SUCCESS = 0x50508025;

        /// <summary>
        /// 广播会议 {confname}{0} 中会场 {sitename}{0}失败，原因:{err}{0}
        /// </summary>
        public const uint ERR_CONF_SERVICE_BROCASTCONFSITE_FAILED = 0x50508026;

        /// <summary>
        /// 取消广播会议{confname}{0} 中的会场{sitename}{0}
        /// </summary>
        public const uint ERR_CONF_SERVICE_CANCLE_BROCASTCONFSITE = 0x50508027;

        /// <summary>
        /// 取消广播会议{confname}{0} 中的会场{sitename}{0}成功
        /// </summary>
        public const uint ERR_CONF_SERVICE_CANCLE_BROCASTCONFSITE_SUCCESS = 0x50508028;

        /// <summary>
        /// 取消广播会议 {confname}{0} 中会场 {sitename}{0}失败，原因:{err}{0}
        /// </summary>
        public const uint ERR_CONF_SERVICE_CANCLE_BROCASTCONFSITE_FAILED = 0x50508029;

        /// <summary>
        /// 会议 {confname}{0}  会场{viewsitename}{0} 观看会场{sitename}{0}
        /// </summary>
        public const uint ERR_CONF_SERVICE_WATCHCONFSITE = 0x5050802A;

        /// <summary>
        /// 会议 {confname}{0}  会场{sitename}{0} 观看会场{sitename}{0}成功
        /// </summary>
        public const uint ERR_CONF_SERVICE_WATCHCONFSITE_SUCCESS = 0x5050802B;

        /// <summary>
        /// 会议 {confname}{0} 会场{sitename}{0} 观看会场{sitename}{0}失败，原因:{err}{0}
        /// </summary>
        public const uint ERR_CONF_SERVICE_WATCHCONFSITE_FAILED = 0x5050802C;

        /// <summary>
        /// 会议 {confname}{0} 中会场{sitename}{0}观看会议多画面
        /// </summary>
        public const uint ERR_CONF_SERVICE_WATCHCONFCP = 0x5050802D;

        /// <summary>
        /// 会议 {confname}{0} 中会场{sitename}{0}观看会议多画面成功
        /// </summary>
        public const uint ERR_CONF_SERVICE_WATCHCONFCP_SUCCESS = 0x5050802E;

        /// <summary>
        /// 会议 {confname}{0} 中会场{sitename}{0}观看会议多画面失败，原因:{err}{0}
        /// </summary>
        public const uint ERR_CONF_SERVICE_WATCHCONFCP_FAILED = 0x5050802F;

        /// <summary>
        /// 调节会议{confname}{0}中的会场{sitename}{0}音量
        /// </summary>
        public const uint ERR_CONF_SERVICE_ADJUSTCONFSITE = 0x50508030;

        /// <summary>
        /// 调节会议{confname}{0}中的会场{sitename}{0}音量成功
        /// </summary>
        public const uint ERR_CONF_SERVICE_ADJUSTCONFSITE_SUCCESS = 0x50508031;

        /// <summary>
        /// 调节会议{confname}{0}中的会场{sitename}{0}音量失败，原因：{err}{0}
        /// </summary>
        public const uint ERR_CONF_SERVICE_ADJUSTCONFSITE_FAILED = 0x50508032;

        /// <summary>
        /// 打开会议{confname}{0}中的会场{sitename}{0}的麦克风
        /// </summary>
        public const uint ERR_CONF_SERVICE_OPENCONFSITEMIC = 0x50508033;

        /// <summary>
        /// 打开会议{confname}{0}中的会场{sitename}{0}的麦克风成功
        /// </summary>
        public const uint ERR_CONF_SERVICE_OPENCONFSITEMIC_SUCCESS = 0x50508034;

        /// <summary>
        /// 打开会议{confname}{0}中的会场{sitename}{0}麦克风失败，原因：{err}{0}
        /// </summary>
        public const uint ERR_CONF_SERVICE_OPENCONFSITEMIC_FAILED = 0x50508035;

        /// <summary>
        /// 关闭会议{confname}{0}中的会场{sitename}{0}的麦克风
        /// </summary>
        public const uint ERR_CONF_SERVICE_CLOSECONFSITEMIC = 0x50508036;

        /// <summary>
        /// 关闭会议{confname}{0}中的会场{sitename}{0}的麦克风成功
        /// </summary>
        public const uint ERR_CONF_SERVICE_CLOSECONFSITEMIC_SUCCESS = 0x50508037;

        /// <summary>
        /// 关闭会议{confname}{0}中的会场{sitename}{0}麦克风失败，原因：{err}{0}
        /// </summary>
        public const uint ERR_CONF_SERVICE_CLOSECONFSITEMIC_FAILED = 0x50508038;

        /// <summary>
        /// 打开会议{confname}{0}中的会场{sitename}{0}的扬声器
        /// </summary>
        public const uint ERR_CONF_SERVICE_OPENCONFSITESPEAKER = 0x50508039;

        /// <summary>
        /// 打开会议{confname}{0}中的会场{sitename}{0}的扬声器成功
        /// </summary>
        public const uint ERR_CONF_SERVICE_OPENCONFSITESPEAKER_SUCCESS = 0x5050803A;

        /// <summary>
        /// 打开会议{confname}{0}中的会场{sitename}{0}扬声器失败，原因：{err}{0}
        /// </summary>
        public const uint ERR_CONF_SERVICE_OPENCONFSITESPEAKER_FAILED = 0x5050803B;

        /// <summary>
        /// 关闭会议{confname}{0}中的会场{sitename}{0}的扬声器
        /// </summary>
        public const uint ERR_CONF_SERVICE_CLOSECONFSITESPEAKER = 0x5050803C;

        /// <summary>
        /// 关闭会议{confname}{0}中的会场{sitename}{0}的扬声器成功
        /// </summary>
        public const uint ERR_CONF_SERVICE_CLOSECONFSITESPEAKER_SUCCESS = 0x5050803D;

        /// <summary>
        /// 关闭会议{confname}{0}中的会场{sitename}{0}扬声器失败，原因：{err}{0}
        /// </summary>
        public const uint ERR_CONF_SERVICE_CLOSECONFSITESPEAKER_FAILED = 0x5050803E;

        /// <summary>
        /// 打开会议{confname}{0}中的会场{sitename}{0}的视频图像
        /// </summary>
        public const uint ERR_CONF_SERVICE_OPENCONFSITEVIDEO = 0x5050803F;

        /// <summary>
        /// 打开会议{confname}{0}中的会场{sitename}{0}的视频图像成功
        /// </summary>
        public const uint ERR_CONF_SERVICE_OPENCONFSITEVIDEO_SUCCESS = 0x50508040;

        /// <summary>
        /// 打开会议{confname}{0}中的会场{sitename}{0}视频图像失败，原因：{err}{0}
        /// </summary>
        public const uint ERR_CONF_SERVICE_OPENCONFSITEVIDEO_FAILED = 0x50508041;

        /// <summary>
        /// 关闭会议{confname}{0}中的会场{sitename}{0}的视频图像
        /// </summary>
        public const uint ERR_CONF_SERVICE_CLOSECONFSITEVIDEO = 0x50508042;

        /// <summary>
        /// 关闭会议{confname}{0}中的会场{sitename}{0}的视频图像成功
        /// </summary>
        public const uint ERR_CONF_SERVICE_CLOSECONFSITEVIDEO_SUCCESS = 0x50508043;

        /// <summary>
        /// 关闭会议{confname}{0}中的会场{sitename}{0}视频图像失败，原因：{err}{0}
        /// </summary>
        public const uint ERR_CONF_SERVICE_CLOSECONFSITEVIDEO_FAILED = 0x50508044;

        /// <summary>
        ///设置会议"{confname}{0}"的会场"{sitename}{0}"的音频环回

        /// </summary>
        public const uint ERR_CONF_SERVICE_SETCONFSITELOOPBACK = 0x50508045;

        /// <summary>
        /// 设置会议"{confname}{0}"的会场"{sitename}{0}"的音频环回成功

        /// </summary>
        public const uint ERR_CONF_SERVICE_SETCONFSITELOOPBACK_SUCCESS = 0x50508046;

        /// <summary>
        /// 设置会议"{confname}{0}"的会场"{sitename}{0}"的音频环回失败，原因：{err}{0}

        /// </summary>
        public const uint ERR_CONF_SERVICE_SETCONFSITELOOPBACK_FAILED = 0x50508047;

        /// <summary>
        /// 取消会议"{confname}{0}"的会场"{sitename}{0}"的音频环回

        /// </summary>
        public const uint ERR_CONF_SERVICE_UNSETCONFSITELOOPBACK = 0x50508048;

        /// <summary>
        /// 取消会议"{confname}{0}"的会场"{sitename}{0}"的音频环回成功

        /// </summary>
        public const uint ERR_CONF_SERVICE_UNSETCONFSITELOOPBACK_SUCCESS = 0x50508049;

        /// <summary>
        /// 取消会议"{confname}{0}"的会场"{sitename}{0}"的音频环回失败，原因：{err}{0}

        /// </summary>
        public const uint ERR_CONF_SERVICE_UNSETCONFSITELOOPBACK_FAILED = 0x5050804A;

        /// <summary>
        /// 取消预约会议{confname}{0}成功
        /// </summary>
        public const uint ERR_CONF_SERVICE_CANCLECONF_SUCCESS = 0x5050804B;

        /// <summary>
        /// 取消预约会议{confname}{0}失败，原因:{err}{0}
        /// </summary>
        public const uint ERR_CONF_SERVICE_CANCLECONF_FAILED = 0x5050804C;

        /// <summary>
        /// 修改预约会议 {confname}{0}成功
        /// </summary>
        public const uint ERR_CONF_SERVICE_MODIFYCONF_SUCCESS = 0x5050804D;

        /// <summary>
        /// 修改预约会议 {confname}{0}失败，原因:{err}{0}
        /// </summary>
        public const uint ERR_CONF_SERVICE_MODIFYCONF_FAILED = 0x5050804E;

        /// <summary>
        /// 延长会议{confname}{0}成功
        /// </summary>
        public const uint ERR_CONF_SERVICE_PROLONGCONF_SUCCESS = 0x5050804F;

        /// <summary>
        /// 延长会议{confname}{0}失败，原因：{err}{0}
        /// </summary>
        public const uint ERR_CONF_SERVICE_PROLONGCONF_FAILED = 0x50508050;


        /// <summary>
        /// 添加会议模板{confname}{0}成功
        /// </summary>
        public const uint ERR_CONF_SERVICE_ADD_CONFTEMPLATE_SUCCESS = 0x50508051;

        /// <summary>
        /// 添加会议模板{confname}{0}失败，原因：{err}{0}
        /// </summary>
        public const uint ERR_CONF_SERVICE_ADD_CONFTEMPLATE_FAILED = 0x50508052;

        /// <summary>
        /// 删除会议模板{confname}{0}成功
        /// </summary>
        public const uint ERR_CONF_SERVICE_DEL_CONFTEMPLATE_SUCCESS = 0x50508053;

        /// <summary>
        /// 删除会议模板{confname}{0}失败，原因：{err}{0}
        /// </summary>
        public const uint ERR_CONF_SERVICE_DEL_CONFTEMPLATE_FAILED = 0x50508054;

        /// <summary>
        /// 修改会议模板{confname}{0}成功
        /// </summary>
        public const uint ERR_CONF_SERVICE_EDIT_CONFTEMPLATE_SUCCESS = 0x50508055;

        /// <summary>
        /// 修改会议模板{confname}{0}失败，原因：{err}{0}
        /// </summary>
        public const uint ERR_CONF_SERVICE_EDIT_CONFTEMPLATE_FAILED = 0x50508056;

        /// <summary>
        /// 复制会议模板{confname}{0}成功
        /// </summary>
        public const uint ERR_CONF_SERVICE_COPY_CONFTEMPLATE_SUCCESS = 0x50508057;

        /// <summary>
        /// 复制会议模板{confname}{0}失败，原因：{err}{0}
        /// </summary>
        public const uint ERR_CONF_SERVICE_COPY_CONFTEMPLATE_FAILED = 0x50508058;

        /// <summary>
        /// 录制会议{confname}{0}
        /// </summary>
        public const uint ERR_CONF_SERVICE_CTRLRECORD = 0x50508059;

        /// <summary>
        ///录制会议{confname}{0}成功
        /// </summary>
        public const uint ERR_CONF_SERVICE_CTRLRECORD_SUCCESS = 0x5050805A;

        /// <summary>
        /// 录制会议{confname}{0}失败，原因：{err}{0}
        /// </summary>
        public const uint ERR_CONF_SERVICE_CTRLRECORD_FAILED = 0x5050805B;

        /// <summary>
        /// 停止录制会议{confname}{0}
        /// </summary>
        public const uint ERR_CONF_SERVICE_STOPRECORD = 0x5050805C;

        /// <summary>
        ///停止录制会议{confname}{0}成功
        /// </summary>
        public const uint ERR_CONF_SERVICE_STOPRECORD_SUCCESS = 0x5050805D;

        /// <summary>
        /// 停止录制会议{confname}{0}失败，原因：{err}{0}
        /// </summary>
        public const uint ERR_CONF_SERVICE_STOPRECORD_FAILED = 0x5050805E;


        /// <summary>
        /// 直播会议{confname}{0}
        /// </summary>
        public const uint ERR_CONF_SERVICE_CTRLLIVE = 0x5050805F;

        /// <summary>
        ///直播会议{confname}{0}成功
        /// </summary>
        public const uint ERR_CONF_SERVICE_CTRLLIVE_SUCCESS = 0x50508060;

        /// <summary>
        /// 直播会议{confname}{0}失败，原因：{err}{0}
        /// </summary>
        public const uint ERR_CONF_SERVICE_CTRLLIVE_FAILED = 0x50508061;

        /// <summary>
        /// 停止直播会议{confname}{0}
        /// </summary>
        public const uint ERR_CONF_SERVICE_STOPLIVE = 0x50508062;

        /// <summary>
        ///停止直播会议{confname}{0}成功
        /// </summary>
        public const uint ERR_CONF_SERVICE_STOPLIVE_SUCCESS = 0x50508063;

        /// <summary>
        /// 停止直播会议{confname}{0}失败，原因：{err}{0}
        /// </summary>
        public const uint ERR_CONF_SERVICE_STOPLIVE_FAILED = 0x50508064;

        /// <summary>
        /// 设置会议{confname}{0}的录制视频源{sitename}{0}
        /// </summary>
        public const uint ERR_CONF_SERVICE_RECORDSOURCE = 0x50508065;

        /// <summary>
        /// 设置会议{confname}{0}的录制视频源{sitename}{0}成功
        /// </summary>
        public const uint ERR_CONF_SERVICE_RECORDSOURCE_SUCCESS = 0x50508066;

        /// <summary>
        ///  设置会议{confname}{0}的录制视频源{sitename}{0}失败，原因：{err}{0}
        /// </summary>
        public const uint ERR_CONF_SERVICE_RECORDSOURCE_FAILED = 0x50508067;

        /// <summary>
        /// 会议{confname}{0}的会场号码{sitecode}{1}名称{sitename}{2}入会。
        /// </summary>
        public const uint ERR_CONF_SERVICE_SITEINCONF = 0x50508068;

        /// <summary>
        /// 会议{confname}{0}的会场号码{sitecode}{1}名称{sitename}{2}离会。
        /// </summary>
        public const uint ERR_CONF_SERVICE_SITEOUTCONF = 0x50508069;

        /// <summary>
        /// 会议{confname}{0}的子会议召开失败，会议将被SMC强行结束。
        /// </summary>
        public const uint ERR_CONF_SERVICE_TIMINGRETRYSTARTPROC = 0x5050806A;

        /// <summary>
        /// 会议{confname}{0}的子会议结束失败，会议将被SMC强行结束。
        /// </summary>
        public const uint ERR_CONF_SERVICE_TIMINGRETRYENDPROC = 0x5050806B;

        /// <summary>
        /// 所有会场都离会，会议{confname}{0}结束会议。
        /// </summary>
        public const uint ERR_CONF_SERVICE_ALLSITEOFFLINEENDCONF = 0x5050806C;

        /// <summary>
        /// 所有会场都没有入会，会议{confname}{0}结束会议。
        /// </summary>
        public const uint ERR_CONF_SERVICE_ALLSITENOTJOINCONF = 0x5050806D;

        /// <summary>
        /// 会议{confname}{0}离线，原因MCU离线。
        /// </summary>
        public const uint ERR_CONF_SERVICE_CONFOFFLINE = 0x5050806E;

        /// <summary>
        /// 会议服务开启
        /// </summary>
        public const uint ERR_CONF_SERVICE_START = 0x5050806F;

        /// <summary>
        /// 会议服务停止
        /// </summary>
        public const uint ERR_CONF_SERVICE_STOP = 0x50508070;


        /// <summary>
        /// 主叫呼集会议{confname}{0}添加会场{sitename}{0}。
        /// </summary>
        public const uint ERR_CONF_SERVICE_ADDSITE = 0x50508071;
        /// <summary>
        /// 主叫呼集会议{confname}{0}添加会场{sitename}{0}成功。
        /// </summary>
        public const uint ERR_CONF_SERVICE_ADDSITE_SUCCESS = 0x50508072;
        /// <summary>
        /// 主叫呼集会议{confname}{0}添加会场{sitename}{0}失败，原因：{err}{0}。
        /// </summary>
        public const uint ERR_CONF_SERVICE_ADDSITE_FAILED = 0x50508073;

        /// <summary>
        /// 设置会议"{confname}{0}"的会场"{sitename}{0}"的视屏环回
        /// </summary>
        public const uint ERR_CONF_SERVICE_SETCONFSITEVIEWLOOPBACK = 0x50508074;

        /// <summary>
        /// 设置会议"{confname}{0}"的会场"{sitename}{0}"的视频环回成功
        /// </summary>
        public const uint ERR_CONF_SERVICE_SETCONFSITEVIEWLOOPBACK_SUCCESS = 0x50508075;

        /// <summary>
        /// 设置会议"{confname}{0}"的会场"{sitename}{0}"的视频环回失败，原因：{err}{0}
        /// </summary>
        public const uint ERR_CONF_SERVICE_SETCONFSITEVIEWLOOPBACK_FAILED = 0x50508076;

        /// <summary>
        /// 取消会议"{confname}{0}"的会场"{sitename}{0}"的视频环回
        /// </summary>
        public const uint ERR_CONF_SERVICE_UNSETCONFSITEVIEWLOOPBACK = 0x50508077;

        /// <summary>
        /// 取消会议"{confname}{0}"的会场"{sitename}{0}"的视频环回成功
        /// </summary>
        public const uint ERR_CONF_SERVICE_UNSETCONFSITEVIEWLOOPBACK_SUCCESS = 0x50508078;

        /// <summary>
        /// 取消会议"{confname}{0}"的会场"{sitename}{0}"的视频环回失败，原因：{err}{0}
        /// </summary>
        public const uint ERR_CONF_SERVICE_UNSETCONFSITEVIEWLOOPBACK_FAILED = 0x50508079;

        /// <summary>
        /// 会议"{confname}{0}"调度成功
        /// </summary>
        public const uint ERR_CONF_SERVICE_CONFRSCH_SUCCESS = 0x5050807A;
        /// <summary>
        /// 会议"{confname}{0}"调度失败，原因：{err}{0}
        /// </summary>
        public const uint ERR_CONF_SERVICE_CONFRSCH_FAILED = 0x5050807B;
        /// <summary>
        ///主席延长会议"{confname}{0}"成功
        /// </summary>
        public const uint ERR_CONF_SERVICE_CHAIRPROCONF_SUCCESS = 0x5050807C;
        /// <summary>
        /// 主席延长会议"{confname}{0}"失败
        /// </summary>
        public const uint ERR_CONF_SERVICE_CHAIRPROCONF_FAILED = 0x5050807D;
        /// <summary>
        /// 自动延长会议"{confname}{0}"成功
        /// </summary>
        public const uint ERR_CONF_SERVICE_AOUTPROCONF_SUCCESS = 0x5050807E;
        /// <summary>
        /// 自动延长会议"{confname}{0}"失败
        /// </summary>
        public const uint ERR_CONF_SERVICE_AOUTPROCONF_FAILED = 0x5050807F;

        /// <summary>
        /// 会议"{confname}{0}"重启
        /// </summary>
        public const uint ERR_CONF_SERVICE_CONFRESET = 0x50508080;

        /// <summary>
        /// 会议"{confname}{0}"不在MCU中，强制结束
        /// </summary>
        public const uint ERR_CONF_SERVICE_CONFNOTINMCU = 0x50508081;
        /// <summary>
        /// 会议"{confname}{0}"的会场"{sitename}{0}"不在MCU中，被删除
        /// </summary>
        public const uint ERR_CONF_SERVICE_CONFSITENTTINMCU = 0x50508082;

        /// <summary>
        /// 添加服务区"{domain}{0}"成功
        /// </summary>
        public const uint ERR_CONF_SERVICE_ADDAREACODE = 0x50508082;

        /// <summary>
        /// 添加服务区"{domain}{0}"失败，原因:{err}{0}
        /// </summary>
        public const uint ERR_CONF_SERVICE_ADDAREACODEFAILED = 0x50508083;

        /// <summary>
        /// 编辑服务区"{domain}{0}"成功
        /// </summary>
        public const uint ERR_CONF_SERVICE_EDITAREACODE = 0x50508084;

        /// <summary>
        /// 编辑服务区"{domain}{0}"失败，原因:{err}{0}
        /// </summary>
        public const uint ERR_CONF_SERVICE_EDITAREACODEFAILED = 0x50508085;

        /// <summary>
        /// 删除服务区"{domain}{0}"成功
        /// </summary>
        public const uint ERR_CONF_SERVICE_DELAREACODE = 0x50508086;

        /// <summary>
        /// 删除服务区"{domain}{0}"失败，原因:{err}{0}
        /// </summary>
        public const uint ERR_CONF_SERVICE_DELAREACODEFAILED = 0x50508087;

        #region 告警起始值
        /// <summary>
        /// 告警起始值
        /// </summary>
        public const uint ERR_CONF_ALARM_SERVICE = 0x50509000;

        ///// <summary>
        ///// ISDN资源使用率超过80%
        ///// </summary>
        //public const uint ISDN_RESOURCE_OVERFLOW_ALARM = ERR_CONF_ALARM_SERVICE + 0x01;

        ///// <summary>
        ///// IP资源使用率超过80%
        ///// </summary>
        //public const uint IP_RESOURCE_OVERFLOW_ALARM = ERR_CONF_ALARM_SERVICE + 0x02;

        ///// <summary>
        ///// 多画面编码资源使用率超过80%
        ///// </summary>
        //public const uint CPENCODE_RESOURCE_OVERFLOW_ALARM = ERR_CONF_ALARM_SERVICE + 0x03;

        ///// <summary>
        ///// 多画面解码资源使用率超过80%
        ///// </summary>
        //public const uint CPDECODE_RESOURCE_OVERFLOW_ALARM = ERR_CONF_ALARM_SERVICE + 0x04;

        ///// <summary>
        ///// 速率适配资源使用率超过80%
        ///// </summary>
        //public const uint RATE_RESOURCE_OVERFLOW_ALARM = ERR_CONF_ALARM_SERVICE + 0x05;

        ///// <summary>
        ///// 音频资源使用率超过80%
        ///// </summary>
        //public const uint AUDIO_RESOURCE_OVERFLOW_ALARM = ERR_CONF_ALARM_SERVICE + 0x06;

        ///// <summary>
        ///// 监控资源使用率超过80%
        ///// </summary>
        //public const uint MONITER_RESOURCE_OVERFLOW_ALARM = ERR_CONF_ALARM_SERVICE + 0x07;

        ///// <summary>
        ///// 加密资源使用率超过80%
        ///// </summary>
        //public const uint ENCRY_RESOURCE_OVERFLOW_ALARM = ERR_CONF_ALARM_SERVICE + 0x08;

        ///// <summary>
        ///// 端口资源使用率超过80%
        ///// </summary>
        //public const uint PORT_RESOURCE_OVERFLOW_ALARM = ERR_CONF_ALARM_SERVICE + 0x09;
        #endregion
        #endregion

        #region GKM服务的错误码0x50600000
        //////////////////////////////////////////////////////////////////////////
        // GKM服务的错误码0x50600000
        //////////////////////////////////////////////////////////////////////////

        /// <summary>
        /// GKM错误码
        /// </summary>
        public const uint ERR_GKM_SERVICE = 0x50600000;

        /// <summary>
        /// 系统共用错误码起始值
        /// </summary>
        public const uint ERR_GKM_COMMON_ERR_BASE = 0x50600100;

        /// <summary>
        /// 操作数据库部分错误码起始值
        /// </summary>
        public const uint GKM_DB_ERR_BASE = 0x50600200;

        /// <summary>
        /// 查询操作错误码起始值
        /// </summary>
        public const uint GKM_QUERY_OP_ERR_BASE = 0x50600300;

        /// <summary>
        /// adhoc/ivr部分错误码起始值
        /// </summary>
        public const uint GKM_SPECIAL_NUM_ERR_BASE = 0x50600400;

        /// <summary>
        /// WEB界面后台处理部分错误码起始值
        /// </summary>
        public const uint GKM_WEB_UI_ERR_BASE = 0x50600500;

        /// <summary>
        /// GK部分错误码起始值
        /// </summary>
        public const uint GKM_GK_ERR_BASE = 0x50600600;

        /// <summary>
        /// GKM节点部分错误码起始值
        /// </summary>
        public const uint GKM_EP_ERR_BASE = 0x50600700;

        /// <summary>
        /// GKM私网区域部分错误码起始值
        /// </summary>
        public const uint GKM_PRIVATE_ERR_BASE = 0x50600800;

        /// <summary>
        /// GKM路由部分错误码起始值
        /// </summary>
        public const uint GKM_ROUTE_ERR_BASE = 0x50600900;

        /// <summary>
        /// GKM呼叫部分
        /// </summary>
        public const uint GKM_CALL_ERR_BASE = 0x50600A00;

        /// <summary>
        /// GK返回错误码
        /// </summary>
        public const uint GKM_GK_RETURE_ERR_BASE = 0x50600B00;

        /// <summary>
        /// 终端网络地址本
        /// </summary>
        public const uint GKM_TER_ADDRBOOK_ERR_BASE = 0x50600C00;

        /// <summary>
        /// Gk的操作日志
        /// </summary>
        public const uint GKM_GK_OPERATION_LOG_ERR = 0x50600D00;

        /// <summary>
        /// Gk设备日志
        /// </summary>
        public const uint GKM_GK_DEVICE_LOG_ERR_BASE = 0x50600E00;

        #region 系统共用错误码从0x50600100开始
        /// <summary>
        /// 核心启动失败
        /// </summary>
        public const uint GKM_CORE_STARTUP_FAIL = ERR_GKM_COMMON_ERR_BASE + 1;

        /// <summary>
        /// 参数错误
        /// </summary>
        public const uint GKM_PARAM_ERROR = ERR_GKM_COMMON_ERR_BASE + 2;

        /// <summary>
        /// 申请内存失败
        /// </summary>
        public const uint GKM_ALLOCATE_MEMORY_FAIL = ERR_GKM_COMMON_ERR_BASE + 3;

        /// <summary>
        /// 数据转换失败
        /// </summary>
        public const uint GKM_DATATRANSLATE_ERROR = ERR_GKM_COMMON_ERR_BASE + 4;

        /// <summary>
        /// 访问互斥信号量超时
        /// </summary>
        public const uint GKM_ACCESS_LOCK_TIMEOUT = ERR_GKM_COMMON_ERR_BASE + 5;

        /// <summary>
        /// 等待响应超时
        /// </summary>
        public const uint GKM_WAIT_ACK_TIMEOUT = ERR_GKM_COMMON_ERR_BASE + 6;

        /// <summary>
        /// 配置GK数据等待回应超时
        /// </summary>
        public const uint GKM_WAIT_CFG_ACK_TIMEOUT = ERR_GKM_COMMON_ERR_BASE + 7;

        /// <summary>
        /// 操作失败
        /// </summary>
        public const uint GKM_OPERATION_ERR = ERR_GKM_COMMON_ERR_BASE + 8;

        /// <summary>
        /// 接收GK消息失败
        /// </summary>
        public const uint GKM_RECV_GK_MSG_FAIL = ERR_GKM_COMMON_ERR_BASE + 9;

        /// <summary>
        /// 远端GK断开连接
        /// </summary>
        public const uint GKM_REMOTE_GK_DISCONNECT = ERR_GKM_COMMON_ERR_BASE + 0x0A;

        /// <summary>
        /// 接收到非法的TPKT消息
        /// </summary>
        public const uint GKM_RCV_INVALID_TPKT_MSG = ERR_GKM_COMMON_ERR_BASE + 0x0B;

        /// <summary>
        /// 发送GK数据失败
        /// </summary>
        public const uint GKM_COM_SEND_GK_MSG_FAIL = ERR_GKM_COMMON_ERR_BASE + 0x0C;

        /// <summary>
        /// 解码GK消息时越界
        /// </summary>
        public const uint GKM_DECODING_GK_MSG_OVERFLOW = ERR_GKM_COMMON_ERR_BASE + 0x0D;

        /// <summary>
        /// 等待响应失败
        /// </summary>
        public const uint GKM_WAIT_ACK_FAIL = ERR_GKM_COMMON_ERR_BASE + 0x0E;

        /// <summary>
        /// 前缀长度超过最大值
        /// </summary>
        public const uint GKM_PREFIX_OVER_MAX_LENGTH = ERR_GKM_COMMON_ERR_BASE + 0x0F;

        /// <summary>
        /// 前缀不能为空
        /// </summary>
        public const uint GKM_PREFIX_EMPTY = ERR_GKM_COMMON_ERR_BASE + 0x10;

        /// <summary>
        /// 前缀不能为0
        /// </summary>
        public const uint GKM_PREFIX_ZERO = ERR_GKM_COMMON_ERR_BASE + 0x11;

        /// <summary>
        /// 前缀已存在
        /// </summary>
        public const uint GKM_PREFIX_ALREADY_EXIST = ERR_GKM_COMMON_ERR_BASE + 0x12;

        /// <summary>
        /// 前缀不存在
        /// </summary>
        public const uint GKM_PREFIX_NOT_EXIST = ERR_GKM_COMMON_ERR_BASE + 0x13;

        /// <summary>
        /// GKM核心未启动，不提供服务
        /// </summary>
        public const uint GKM_CORE_NOT_START = ERR_GKM_COMMON_ERR_BASE + 0x14;

        /// <summary>
        /// 暂不支持Ipv6
        /// </summary>
        public const uint GKM_IPV6_NOT_SUPPORT = ERR_GKM_COMMON_ERR_BASE + 0x15;

        /// <summary>
        /// 网络监听参数非法
        /// </summary>
        public const uint GKM_COM_LISTEN_PARAM_INVALID = ERR_GKM_COMMON_ERR_BASE + 0x16;

        /// <summary>
        /// 启动错误
        /// </summary>
        public const uint GKM_COM_START_LISTEN_ERROR = ERR_GKM_COMMON_ERR_BASE + 0x17;

        /// <summary>
        /// 过程错误
        /// </summary>
        public const uint GKM_COM_LISTEN_ERROR = ERR_GKM_COMMON_ERR_BASE + 0x18;

        /// <summary>
        /// GK接收线程错误
        /// </summary>
        public const uint GKM_COM_RCV_SELECT_ERROR = ERR_GKM_COMMON_ERR_BASE + 0x19;

        /// <summary>
        /// 非法Ip地址
        /// </summary>
        public const uint GKM_INVALID_IP_ADDR = ERR_GKM_COMMON_ERR_BASE + 0x1a;

        /// <summary>
        /// Ip地址为0
        /// </summary>
        public const uint GKM_IP_ADDR_EMPTY = ERR_GKM_COMMON_ERR_BASE + 0x1b;

        /// <summary>
        /// 端口号不合法
        /// </summary>
        public const uint GKM_INVALID_PORT = ERR_GKM_COMMON_ERR_BASE + 0x1c;

        /// <summary>
        /// 子网掩码不合法
        /// </summary>
        public const uint GKM_INVALID_IP_MASK = ERR_GKM_COMMON_ERR_BASE + 0x1d;

        /// <summary>
        /// 不合法的IP范围，正确的范围是1.0.0.0 ~ 223.255.255.255
        /// </summary>
        public const uint GKM_INVALID_IP_RANGE = ERR_GKM_COMMON_ERR_BASE + 0x1e;

        /// <summary>
        /// 数据转换错误
        /// </summary>
        public const uint GKM_DATA_CONVERT_ERROR = ERR_GKM_COMMON_ERR_BASE + 0x1f;

        /// <summary>
        /// 密码为空，请输入密码
        /// </summary>
        public const uint GKM_PASSWORD_EMPTY = ERR_GKM_COMMON_ERR_BASE + 0x20;

        /// <summary>
        /// E164别名错误，应全部为数字
        /// </summary>
        public const uint GKM_E164_ALIAS_IS_ALL_NUMBER = ERR_GKM_COMMON_ERR_BASE + 0x21;

        /// <summary>
        /// Uri别名格式错误
        /// </summary>
        public const uint GKM_URI_ALIAS_FORMAT_ERROR = ERR_GKM_COMMON_ERR_BASE + 0x22;

        /// <summary>
        /// Email别名格式错误
        /// </summary>
        public const uint GKM_EMAIL_ALIAS_ERROR = ERR_GKM_COMMON_ERR_BASE + 0x23;

        /// <summary>
        /// 后缀长度超过最大值
        /// </summary>
        public const uint GKM_SUFFIX_OVER_MAX_LENGTH = ERR_GKM_COMMON_ERR_BASE + 0x24;

        /// <summary>
        /// 后缀为空
        /// </summary>
        public const uint GKM_SUFFIX_EMPTY = ERR_GKM_COMMON_ERR_BASE + 0x25;

        /// <summary>
        /// 后缀格式错误
        /// </summary>
        public const uint GKM_INVALID_SUFFIX = ERR_GKM_COMMON_ERR_BASE + 0x26;

        /// <summary>
        /// 前缀必须全部是数字
        /// </summary>
        public const uint GKM_PREFIX_IS_ALL_NUMBER = ERR_GKM_COMMON_ERR_BASE + 0x27;

        /// <summary>
        /// 后缀已经存在
        /// </summary>
        public const uint GKM_SUFFIX_ALREADY_EXIST = ERR_GKM_COMMON_ERR_BASE + 0x28;

        /// <summary>
        /// 端口号必须全部是数字
        /// </summary>
        public const uint GKM_PORT_IS_ALL_NUMBER = ERR_GKM_COMMON_ERR_BASE + 0x29;

        /// <summary>
        /// 后缀不存在
        /// </summary>
        public const uint GKM_SUFFIX_NOT_EXIST = ERR_GKM_COMMON_ERR_BASE + 0x2A;

        /// <summary>
        /// 无
        /// </summary>
        public const uint GKM_REASON_NULL = ERR_GKM_COMMON_ERR_BASE + 0x2B;

        /// <summary>
        /// 与其他号码重复
        /// </summary>
        public const uint GKM_DUPLICATE_NUMBER = ERR_GKM_COMMON_ERR_BASE + 0x2C;

        /// <summary>
        /// 必须以@或者.结束
        /// </summary>
        public const uint GKM_STRING_MUST_END_WITH_DOT_OR_AT = ERR_GKM_COMMON_ERR_BASE + 0x2D;

        /// <summary>
        /// URI后缀不能为空。
        /// </summary>
        public const uint GKM_SUFFIX_SHOULD_NOT_BE_EMPTY = ERR_GKM_COMMON_ERR_BASE + 0x2E;

        /// <summary>
        /// 私网区域IP地址不能为空。
        /// </summary>
        public const uint GKM_PRIVATE_ZONE_IP_SHOULD_NOT_BE_EMPTY = ERR_GKM_COMMON_ERR_BASE + 0x2F;

        /// <summary>
        /// IP地址与子网掩码类型不匹配。
        /// </summary>
        public const uint GKM_IP_ADDR_MASK_TYPE_NOT_MATCH = ERR_GKM_COMMON_ERR_BASE + 0x30;
        #endregion

        #region 数据库操作部分从0x50600200开始
        /// <summary>
        /// 打开数据库失败
        /// </summary>
        public const uint GKM_DB_OPEN_FAIL = GKM_DB_ERR_BASE + 1;

        /// <summary>
        /// 操作数据库失败
        /// </summary>
        public const uint GKM_DB_OPERATION_FAIL = GKM_DB_ERR_BASE + 2;
        #endregion

        #region 查询操作部分从0x50600300开始
        /// <summary>
        /// 查询操作等待响应对象不存在
        /// </summary>
        public const uint GKM_QUERY_ACK_ENTITY_NOT_EXIST = GKM_QUERY_OP_ERR_BASE + 1;

        /// <summary>
        /// 系统忙
        /// </summary>
        public const uint GKM_SYSTERM_OVER_LOAD = GKM_QUERY_OP_ERR_BASE + 2;

        /// <summary>
        /// 查询操作序列号冲突
        /// </summary>
        public const uint GKM_QUERY_OPERATION_SEQ_CONFLINCT = GKM_QUERY_OP_ERR_BASE + 3;

        /// <summary>
        /// 操作正在进行
        /// </summary>
        public const uint GKM_OPERATION_PROCESSING = GKM_QUERY_OP_ERR_BASE + 4;

        /// <summary>
        /// 查询操作类型错误
        /// </summary>
        public const uint GKM_QUERY_TYPE_ERROR = GKM_QUERY_OP_ERR_BASE + 5;

        /// <summary>
        /// 申请界面序号失败
        /// </summary>
        public const uint GKM_APPLY_UISEQ_FAIL = GKM_QUERY_OP_ERR_BASE + 6;

        /// <summary>
        /// 操作被取消
        /// </summary>
        public const uint GKM_OPERATION_CANCLE = GKM_QUERY_OP_ERR_BASE + 7;

        /// <summary>
        /// 查询结果错误
        /// </summary>
        public const uint GKM_QUERY_RESULT_ERR = GKM_QUERY_OP_ERR_BASE + 8;
        #endregion

        #region adhoc/ivr部分从0x50600400开始
        /// <summary>
        /// adhoc/ivr号码不存在
        /// </summary>
        public const uint GKM_SPECIALNUM_NOT_EXIST = GKM_SPECIAL_NUM_ERR_BASE + 2;
        /// <summary>
        /// 接入号与会议特服号冲突
        /// </summary>
        public const uint GKM_SPECIALNUM_CONFICT_WITH_SSC = GKM_SPECIAL_NUM_ERR_BASE + 3;
        /// <summary>
        /// 接入号与其它adhoc/ivr号码冲突
        /// </summary>
        public const uint GKM_SPECIALNUM_CONFICT_WITH_SPECIAL_NUM = GKM_SPECIAL_NUM_ERR_BASE + 4;
        /// <summary>
        /// 接入号与GK前缀冲突
        /// </summary>
        public const uint GKM_SPECIALNUM_CONFICT_WITH_PREFIX = GKM_SPECIAL_NUM_ERR_BASE + 5;
        /// <summary>
        /// 接入号与节点号码冲突
        /// </summary>
        public const uint GKM_SPECIALNUM_CONFICT_WITH_EPNUM = GKM_SPECIAL_NUM_ERR_BASE + 6;
        /// <summary>
        /// 接入号与群组号冲突
        /// </summary>
        public const uint GKM_SPECIALNUM_CONFICT_WITH_SRVGRPCODE = GKM_SPECIAL_NUM_ERR_BASE + 7;
        #endregion

        #region WEB界面后台处理部分从0x50600500开始
        /// <summary>
        /// 临时操作对象不存在，用在查找GK、节点、路由、私网区域临时对象
        /// </summary>
        public const uint GKM_TEMP_OPERATION_OBJECT_NOT_EXIST = GKM_WEB_UI_ERR_BASE + 0x01;
        /// <summary>
        /// 开启骑墙，有效业务地址的个数必须要不小于2。
        /// </summary>
        public const uint GKM_BESTRIDE_SERVICE_IP_NUM_ERR = GKM_WEB_UI_ERR_BASE + 0X02;

        /// <summary>
        /// 不开启骑墙，有效的业务地址的个数只能为一个。
        /// </summary>
        public const uint GKM_UNBESTRIDE_SERVICE_IP_NUM_ERR = GKM_WEB_UI_ERR_BASE + 0X03;

        /// <summary>
        ///连接密码不能为空
        /// </summary>
        public const uint GKM_ERR_GK_AUTH_PWD_MIN_LENTH = GKM_WEB_UI_ERR_BASE + 0x04;

        /// <summary>
        /// 密码长度不能超过16位
        /// </summary>
        public const uint GKM_ERR_GK_AUTH_PWD_MAX_LENTH = GKM_WEB_UI_ERR_BASE + 0x05;

        /// <summary>
        /// Gk媒体IP地址不能为空
        /// </summary>
        public const uint GKM_GK_MEDIA_IP_NOT_EMPTY = GKM_WEB_UI_ERR_BASE + 0x06;

        /// <summary>
        /// 前缀/后缀没有在系统的GK中定义，请在GK上添加对应的前缀/后缀。
        /// </summary>
        public const uint GKM_GK_PRIFIX_SUFIX_NOT_EXIST = GKM_WEB_UI_ERR_BASE + 0x07;

        #endregion

        #region GK部分错误码从0x50600600开始
        /// <summary>
        /// GK不存在
        /// </summary>
        public const uint GKM_GK_NOT_EXIST = GKM_GK_ERR_BASE + 1;

        /// <summary>
        /// GK不在线
        /// </summary>
        public const uint GKM_GK_NO_ONLINE = GKM_GK_ERR_BASE + 2;

        /// <summary>
        /// GK名字长度超过最大长度
        /// </summary>
        public const uint GKM_GK_NAME_OVER_MAX_LEN = GKM_GK_ERR_BASE + 3;

        /// <summary>
        /// GK名字已经存在
        /// </summary>
        public const uint GKM_GK_NAME_ALREADY_EXIST = GKM_GK_ERR_BASE + 4;

        /// <summary>
        /// GK名字不能为空
        /// </summary>
        public const uint GKM_GK_NAME_EMPTY = GKM_GK_ERR_BASE + 5;

        /// <summary>
        /// GKID长度超过最大长度
        /// </summary>
        public const uint GKM_GK_ID_OVER_MAX_LEN = GKM_GK_ERR_BASE + 6;

        /// <summary>
        /// GK 主备机IP冲突
        /// </summary>
        public const uint GKM_GK_IPADDR_CONFLICT = GKM_GK_ERR_BASE + 7;

        /// <summary>
        /// GK 主机IP不能为空
        /// </summary>
        public const uint GKM_GK_IP_EMPTY = GKM_GK_ERR_BASE + 8;

        /// <summary>
        /// H460参数配置未使能
        /// </summary>
        public const uint GKM_H460_PARAM_CONFIG_DISABLE = GKM_GK_ERR_BASE + 9;

        /// <summary>
        /// H460服务器未使能
        /// </summary>
        public const uint GKM_H460_SERVER_DISABLE = GKM_GK_ERR_BASE + 0x0A;

        /// <summary>
        /// 与路由模式设置冲突
        /// </summary>
        public const uint GKM_ROUTE_MODE_CONFLICT = GKM_GK_ERR_BASE + 0x0B;

        /// <summary>
        /// GK已存在
        /// </summary>
        public const uint GKM_GK_ALREADY_EXIST = GKM_GK_ERR_BASE + 0x0C;

        /// <summary>
        /// GK网关前缀类型错误
        /// </summary>
        public const uint GKM_GK_PREFIX_TYPE_ERROR = GKM_GK_ERR_BASE + 0x0D;

        /// <summary>
        /// GK在线
        /// </summary>
        public const uint GKM_GK_ONLINE = GKM_GK_ERR_BASE + 0x0E;

        /// <summary>
        /// RTP、RTCP端口号范围错误（应该在1024~65535之间）
        /// </summary>
        public const uint GKM_PORT_RANGE_ERROR = GKM_GK_ERR_BASE + 0x0F;

        /// <summary>
        /// 被删除前缀/后缀存在匹配节点
        /// </summary>
        public const uint GKM_DEL_PREFIX_HAS_MATCH_EP = GKM_GK_ERR_BASE + 0x10;

        /// <summary>
        /// Media IP为0
        /// </summary>
        public const uint GKM_DEDIA_IP_EMPTY = GKM_GK_ERR_BASE + 0x11;

        /// <summary>
        /// GK状态变化。不允许编辑
        /// </summary>
        public const uint GKM_GK_STATUS_DIFFERENT = GKM_GK_ERR_BASE + 0x12;

        /// <summary>
        /// 启用了h460必须启用媒体IP
        /// </summary>
        public const uint GKM_ENABLE_MEDIA_IP_WHEN_ENABLE_H460 = GKM_GK_ERR_BASE + 0x13;

        /// <summary>
        /// GK ID为空
        /// </summary>
        public const uint GKM_GK_ID_EMPTY = GKM_GK_ERR_BASE + 0x14;

        /// <summary>
        /// GK 个数超过系统限制
        /// </summary>
        public const uint GKM_GK_NUM_OVER_MAX_NUM = GKM_GK_ERR_BASE + 0x15;

        /// <summary>
        /// 不存在该前缀的GK
        /// </summary>
        public const uint GKM_GK_MATCH_PREFIX_NOT_EXIST = GKM_GK_ERR_BASE + 0x16;

        /// <summary>
        /// 不能删除有预定义节点的GK
        /// </summary>
        public const uint GKM_CANNOT_DELETE_GK_WHO_HAS_EP = GKM_GK_ERR_BASE + 0x17;

        /// <summary>
        /// 告警值范围应在1~100之间
        /// </summary>
        public const uint GKM_INVALID_ALARM_RANGE = GKM_GK_ERR_BASE + 0x18;

        /// <summary>
        /// TTL握手周期范围应在10-3600
        /// </summary>
        public const uint GKM_INVALID_TTL_RANGE = GKM_GK_ERR_BASE + 0x19;

        /// <summary>
        /// IRR握手周期范围应在20-1800
        /// </summary>
        public const uint GKM_INVALID_IRR_RANGE = GKM_GK_ERR_BASE + 0x1A;

        /// <summary>
        /// 动态节点呼叫数范围应在20-1800
        /// </summary>
        public const uint GKM_INVALID_DYNAMIC_EP_CALL_NUMBER = GKM_GK_ERR_BASE + 0x1B;

        /// <summary>
        /// 动态节点带宽范围应在1-250
        /// </summary>
        public const uint GKM_INVALID_DYNAMIC_EP_BANDWIDTH_RANGE = GKM_GK_ERR_BASE + 0x1C;

        /// <summary>
        /// LRQ范围应在1-255
        /// </summary>
        public const uint GKM_INVALID_LRQ_RANGE = GKM_GK_ERR_BASE + 0x1D;

        /// <summary>
        /// Q931路由模式错误
        /// </summary>
        public const uint GKM_Q931_ROUTE_MODE_ERROR = GKM_GK_ERR_BASE + 0x1E;

        /// <summary>
        /// 配置H460参数错误
        /// </summary>
        public const uint GKM_CONFIG_H460_ERROR = GKM_GK_ERR_BASE + 0x1F;

        /// <summary>
        /// RTP端口号必须是1024-65534之间的偶数
        /// </summary>
        public const uint GKM_INVALID_RTP_PORT = GKM_GK_ERR_BASE + 0x20;

        /// <summary>
        /// 公私网穿越协议错误
        /// </summary>
        public const uint GKM_TRAVERSE_WAY_ERROR = GKM_GK_ERR_BASE + 0x21;

        /// <summary>
        /// GK前缀超过最大长度12
        /// </summary>
        public const uint GKM_GK_PREFIX_OVER_MAX_LENGTH = GKM_GK_ERR_BASE + 0x22;

        /// <summary>
        /// GK后缀超过最大长度125
        /// </summary>
        public const uint GKM_GK_SUFFIX_OVER_MAX_LENGTH = GKM_GK_ERR_BASE + 0x23;

        /// <summary>
        /// GK IP已存在
        /// </summary>
        public const uint GKM_GK_IP_ALREADY_EXIST = GKM_GK_ERR_BASE + 0x24;

        /// <summary>
        /// GK个数已达到License限制
        /// </summary>
        public const uint GKM_GK_OVER_LICENSE_LIMIT = GKM_GK_ERR_BASE + 0x25;

        /// <summary>
        /// 在线GK个数已经达到License最大限制
        /// </summary>
        public const uint GKM_GK_OFFLINE_BECAUSE_LICENSE_LIMIT = GKM_GK_ERR_BASE + 0x26;

        /// <summary>
        /// 包含{gkprefix}前缀的GK不存在，请在GK上添加对应前缀。(添加MCU时使用GK不存在的前缀提示信息)
        /// </summary>
        public const uint GKM_GK_ADDMCU_PREFIX_NOT_EXIST = GKM_GK_ERR_BASE + 0x27;

        /// <summary>
        /// 前缀没有在系统的Gk中定义，请在GK上添加对应前缀。
        /// </summary>
        public const uint GKM_GK_NOT_EXIST_BY_AREA_NUM = GKM_GK_ERR_BASE + 0x28;
        #endregion

        #region GKM节点部分从0x50600700
        /// <summary>
        /// 放号模式不允许
        /// </summary>
        public const uint GKM_ACCOUNT_MODE_DENY = GKM_EP_ERR_BASE + 1;

        /// <summary>
        /// 节点已存在
        /// </summary>
        public const uint GKM_EP_ALREADY_EXIST = GKM_EP_ERR_BASE + 2;

        /// <summary>
        /// 节点不存在
        /// </summary>
        public const uint GKM_EP_NOT_EXIST = GKM_EP_ERR_BASE + 3;

        /// <summary>
        /// 节点类型错误
        /// </summary>
        public const uint GKM_EP_TYPE_ERROR = GKM_EP_ERR_BASE + 4;

        /// <summary>
        /// 节点H323ID长度超过最大长度63
        /// </summary>
        public const uint GKM_EP_ALIAS_OVER_MAX_LEN = GKM_EP_ERR_BASE + 5;

        /// <summary>
        /// 节点别名已存在
        /// </summary>
        public const uint GKM_EP_ALIAS_ALREADY_EXIST = GKM_EP_ERR_BASE + 6;

        /// <summary>
        /// 节点别名不存在
        /// </summary>
        public const uint GKM_EP_ALIAS_NOT_EXIST = GKM_EP_ERR_BASE + 7;

        /// <summary>
        /// 节点别名不能为空
        /// </summary>
        public const uint GKM_EP_ALIAS_EMPTY = GKM_EP_ERR_BASE + 8;

        /// <summary>
        /// 节点认证方式错误
        /// </summary>
        public const uint GKM_EP_IDENTITY_TYPE_ERR = GKM_EP_ERR_BASE + 9;

        /// <summary>
        /// 节点认证密码超过最大长度
        /// </summary>
        public const uint GKM_EP_IDENTITY_PWD_OVER_MAX_LENGTH = GKM_EP_ERR_BASE + 0x0A;

        /// <summary>
        /// 节点别名类型错误
        /// </summary>
        public const uint GKM_EP_ALIAS_TYPE_ERR = GKM_EP_ERR_BASE + 0x0B;

        /// <summary>
        /// 节点状态错误
        /// </summary>
        public const uint GKM_EP_STATUS_ERROR = GKM_EP_ERR_BASE + 0x0C;

        /// <summary>
        /// 节点网关前缀类型错误
        /// </summary>
        public const uint GKM_EP_PREFIX_TYPE_ERROR = GKM_EP_ERR_BASE + 0x0D;

        /// <summary>
        /// 节点区号超过最大长度
        /// </summary>
        public const uint GKM_EP_AREA_OVER_MAX_LENGTH = GKM_EP_ERR_BASE + 0x0E;

        /// <summary>
        /// 节点E164号码非法
        /// </summary>
        public const uint GKM_EP_E164_INVALID = GKM_EP_ERR_BASE + 0x0F;

        /// <summary>
        /// 节点别名非法
        /// </summary>
        public const uint GKM_EP_ALIAS_INVALID = GKM_EP_ERR_BASE + 0x10;

        /// <summary>
        /// 终端节点带宽应在1-4000000
        /// </summary>
        public const uint GKM_INVALID_TERMINAL_BANDWIDTH = GKM_EP_ERR_BASE + 0x11;

        /// <summary>
        /// MCU节点带宽应在1-4000000
        /// </summary>
        public const uint GKM_INVALID_MCU_BANDWIDTH = GKM_EP_ERR_BASE + 0x12;

        /// <summary>
        /// 网关节点带宽应在1-8000000
        /// </summary>
        public const uint GKM_INVALID_GW_BANDWIDTH = GKM_EP_ERR_BASE + 0x13;

        /// <summary>
        /// 终端节点呼叫数应在1-100
        /// </summary>
        public const uint GKM_INVALID_TERMINAL_CALL_NUMBER = GKM_EP_ERR_BASE + 0x14;

        /// <summary>
        /// MCU节点呼叫数应在1-8000
        /// </summary>
        public const uint GKM_INVALID_MCU_CALL_NUMBER = GKM_EP_ERR_BASE + 0x15;

        /// <summary>
        /// 终端节点呼叫数应在1-8000
        /// </summary>
        public const uint GKM_INVALID_GW_CALL_NUMBER = GKM_EP_ERR_BASE + 0x16;

        /// <summary>
        /// 节点网关前缀超过最大限制32
        /// </summary>
        public const uint GKM_EP_PREFIX_OVER_MAX_LENGTH = GKM_EP_ERR_BASE + 0x17;

        /// <summary>
        /// 节点E164别名长度超过最大长度64
        /// </summary>
        public const uint GKM_EP_E164_ALIAS_OVER_MAX_LENGTH = GKM_EP_ERR_BASE + 0x18;

        /// <summary>
        /// 节点URI别名长度超过最大长度127
        /// </summary>
        public const uint GKM_EP_URI_ALIAS_OVER_MAX_LENGTH = GKM_EP_ERR_BASE + 0x19;

        /// <summary>
        /// 多个别名匹配到了不同的GK
        /// </summary>
        public const uint GKM_EP_MATCH_DIFFERENT_GK = GKM_EP_ERR_BASE + 0x1a;
        #endregion

        #region GK私网区域部分从0x50600800开始
        /// <summary>
        /// GKM私网区域名称超过最大长度
        /// </summary>
        public const uint GKM_PRIVATE_ZONE_NAME_OVER_MAX_LENGTH = GKM_PRIVATE_ERR_BASE + 1;

        /// <summary>
        /// GKM私网区域名称不能为空
        /// </summary>
        public const uint GKM_PRIVATE_ZONE_NAME_EMPTY = GKM_PRIVATE_ERR_BASE + 2;

        /// <summary>
        /// GKM私网区域规则类型错误
        /// </summary>
        public const uint GKM_PRIVATE_ITEM_TYPE_ERR = GKM_PRIVATE_ERR_BASE + 3;

        /// <summary>
        /// GKM私网区域规则URI后缀超过最大长度127
        /// </summary>
        public const uint GKM_PRIVATE_ITEM_URI_SUFFIX_OVER_MAX_LENGTH = GKM_PRIVATE_ERR_BASE + 4;

        /// <summary>
        /// GKM私网区域规则数字前缀超过最大长度12
        /// </summary>
        public const uint GKM_PRIVATE_ITEM_NUM_PREFIX_OVER_MAX_LENGTH = GKM_PRIVATE_ERR_BASE + 5;

        /// <summary>
        /// GKM添加私网区域冲突
        /// </summary>
        public const uint GKM_PRIVATE_ZONE_CONFLICT = GKM_PRIVATE_ERR_BASE + 6;

        /// <summary>
        /// GKM添加私网规则冲突
        /// </summary>
        public const uint GKM_PRIVATE_ITEM_CONFLICT = GKM_PRIVATE_ERR_BASE + 7;

        /// <summary>
        /// 本地私网区域已存在
        /// </summary>
        public const uint GKM_LOCAL_PRIVATE_ZONE_ALREADY_EXIST = GKM_PRIVATE_ERR_BASE + 8;

        /// <summary>
        /// 私网区域个数超过系统限制个数
        /// </summary>
        public const uint GKM_PRIVATE_ZONE_OVER_MAX_NUM = GKM_PRIVATE_ERR_BASE + 9;

        /// <summary>
        /// 私网规则个数超过系统限制个数
        /// </summary>
        public const uint GKM_PRIVATE_ITEM_OVER_MAX_NUM = GKM_PRIVATE_ERR_BASE + 0x0A;
        #endregion

        #region GKM路由部分从0x50600900开始
        /// <summary>
        /// GKM路由类型错误
        /// </summary>
        public const uint GKM_ROUTE_TYPE_ERR = GKM_ROUTE_ERR_BASE + 1;

        /// <summary>
        /// 数字前缀路由长度超过最大值
        /// </summary>
        public const uint GKM_ROUTE_NUM_PREFIX_OVER_MAX_LENGTH = GKM_ROUTE_ERR_BASE + 2;

        /// <summary>
        /// URI后缀路由长度超过最大值
        /// </summary>
        public const uint GKM_ROUTE_URI_SUFFIX_OVER_MAX_LENGTH = GKM_ROUTE_ERR_BASE + 3;

        /// <summary>
        /// H235密码长度超过最大值
        /// </summary>
        public const uint GKM_ROUTE_H235_PWD_OVER_MAX_LENGTH = GKM_ROUTE_ERR_BASE + 4;

        /// <summary>
        /// 路由目的端口号范围错误
        /// </summary>
        public const uint GKM_ROUTE_DEST_PORT_RANGE_ERR = GKM_ROUTE_ERR_BASE + 5;

        /// <summary>
        /// 路由描述超过最大长度
        /// </summary>
        public const uint GKM_ROUTE_DESC_OVER_MAX_LENGTH = GKM_ROUTE_ERR_BASE + 6;

        /// <summary>
        /// H235加密类型错误
        /// </summary>
        public const uint GKM_ROUTE_H235_SECURITY_TYPE_ERR = GKM_ROUTE_ERR_BASE + 7;

        /// <summary>
        /// 路由不存在
        /// </summary>
        public const uint GKM_ROUTE_NOT_EXIST = GKM_ROUTE_ERR_BASE + 8;

        /// <summary>
        /// 路由个数达到系统限制最大值
        /// </summary>
        public const uint GKM_ROUTE_OVER_MAX_NUM = GKM_ROUTE_ERR_BASE + 9;

        /// <summary>
        /// H235密码不能为空
        /// </summary>
        public const uint GKM_ROUTE_H235_PWD_EMPTY = GKM_ROUTE_ERR_BASE + 0x0A;

        /// <summary>
        /// 配置私网路由需先配置gk为H460服务端
        /// </summary>
        public const uint GKM_PRIVATE_ROUTE_MUST_ENABLE_H460_SERVER = GKM_ROUTE_ERR_BASE + 0x0B;

        /// <summary>
        /// 路由已存在
        /// </summary>
        public const uint GKM_ROUTE_HAS_EXIST = GKM_ROUTE_ERR_BASE + 0x0C;
        #endregion

        #region GKM呼叫部分从0x50600A00开始
        /// <summary>
        /// 指定的呼叫不存在
        /// </summary>
        public const uint GKM_CALL_NOT_EXIST = GKM_CALL_ERR_BASE + 1;
        #endregion

        #region GK返回错误码从0x50600B00开始
        /// <summary>
        /// 记录的后台索引已经存在
        /// </summary>
        public const uint GKM_GKCORE_CONF_INDEX_ALREADY_EXIST = GKM_GK_RETURE_ERR_BASE + 0x01;

        /// <summary>
        /// 记录的后台索引不存在
        /// </summary>
        public const uint GKM_GKCORE_CONF_INDEX_NOT_EXIST = GKM_GK_RETURE_ERR_BASE + 0x02;

        /// <summary>
        /// 解码业务中心消息失败
        /// </summary>
        public const uint GKM_GKCORE_CONF_DECODING_SMC_MSG_FAIL = GKM_GK_RETURE_ERR_BASE + 0x03;

        /// <summary>
        /// GKCORE的数据表已满
        /// </summary>
        public const uint GKM_GKCORE_CONF_DATA_TABLE_FULL = GKM_GK_RETURE_ERR_BASE + 0x04;

        /// <summary>
        /// GK的前台索引越界
        /// </summary>
        public const uint GKM_GKCORE_CONF_GK_INDEX_OVERFLOW = GKM_GK_RETURE_ERR_BASE + 0x05;

        /// <summary>
        /// GK的索引对照表错误
        /// </summary>
        public const uint GKM_GKCORE_CONF_INDEX_TABLE_ERROR = GKM_GK_RETURE_ERR_BASE + 0x06;

        /// <summary>
        /// GK节点有呼叫，无法删除
        /// </summary>
        public const uint GKM_GKCORE_EP_DEL_INCALL = GKM_GK_RETURE_ERR_BASE + 0x07;

        /// <summary>
        /// 节点有呼叫，无法注销
        /// </summary>
        public const uint GKM_GKCORE_EP_UNREG_INCALL = GKM_GK_RETURE_ERR_BASE + 0x08;

        /// <summary>
        /// 节点有呼叫，无法修改
        /// </summary>
        public const uint GKM_GKCORE_EP_MOD_INCALL = GKM_GK_RETURE_ERR_BASE + 0x09;

        /// <summary>
        /// 节点离线，无法注销
        /// </summary>
        public const uint GKM_GKCORE_EP_UNREG_OFFLINE = GKM_GK_RETURE_ERR_BASE + 0x0A;

        /// <summary>
        /// 节点不存在，无法修改
        /// </summary>
        public const uint GKM_GKCORE_EP_MOD_NOTEXIST = GKM_GK_RETURE_ERR_BASE + 0x0B;

        /// <summary>
        /// 节点不存在，无法删除
        /// </summary>
        public const uint GKM_GKCORE_EP_DEL_NOTEXIST = GKM_GK_RETURE_ERR_BASE + 0x0C;

        /// <summary>
        /// 呼叫不存在，无法删除
        /// </summary>
        public const uint GKM_GKCORE_CALL_NOTEXIST = GKM_GK_RETURE_ERR_BASE + 0x0D;

        /// <summary>
        /// 未知的错误
        /// </summary>
        public const uint GKM_GKCORE_CONF_UNKNOWN_ERROR = GKM_GK_RETURE_ERR_BASE + 0x0E;

        /// <summary>
        /// 节点表满
        /// </summary>
        public const uint GKM_GKCORE_EP_TABLE_FULL = GKM_GK_RETURE_ERR_BASE + 0x0F;

        /// <summary>
        /// 永久在线节点，无法注销
        /// </summary>
        public const uint GKM_GKCORE_EP_UNREG_ALWAYS_ONLINE = GKM_GK_RETURE_ERR_BASE + 0x10;

        /// <summary>
        /// 节点注销操作失败
        /// </summary>
        public const uint GKM_GKCORE_EP_UNREG_FAIL = GKM_GK_RETURE_ERR_BASE + 0x11;

        /// <summary>
        /// 节点名称已被在线节点占用
        /// </summary>
        public const uint GKM_GKCORE_EP_DUPLICATE_ALIAS_H323ID = GKM_GK_RETURE_ERR_BASE + 0x12;

        /// <summary>
        /// 节点号码已被在线节点占用
        /// </summary>
        public const uint GKM_GKCORE_EP_DUPLICATE_ALIAS_E164 = GKM_GK_RETURE_ERR_BASE + 0x13;

        /// <summary>
        /// 动态注册节点无法编辑信息
        /// </summary>
        public const uint GKM_GKCORE_EP_DYN_CANNOT_EDIT = GKM_GK_RETURE_ERR_BASE + 0x14;

        /// <summary>
        ///数据重复
        /// </summary>
        public const uint GKM_GKCORE_DUP_RULE_NODE = GKM_GK_RETURE_ERR_BASE + 0x15;

        /// <summary>
        /// GK返回失败
        /// </summary>
        public const uint GKM_GK_RETURN_ERROR = GKM_GK_RETURE_ERR_BASE + 0x16;
        /// <summary>
        /// 别名冲突
        /// </summary>
        public const uint GKM_GK_ALIAS_DUPLICATE = GKM_GK_RETURE_ERR_BASE + 0x17;
        /// <summary>
        /// 主键冲突
        /// </summary>
        public const uint GKM_GK_DB_KEYDUPLICATE = GKM_GK_RETURE_ERR_BASE + 0x18;

        /// <summary>
        /// 与有呼叫的节点的别名冲突.
        /// </summary>
        public const uint GKM_GK_CONFLICT_WITH_EP_DNY = GKM_GK_RETURE_ERR_BASE + 0x19;

        #endregion

        #region 终端网络地址本
        /// <summary>
        /// gk终端网络地址本个数超过系统最大限制
        /// </summary>
        public const uint GKM_TER_ADDRBOOK_OVER_MAX_NUM = GKM_TER_ADDRBOOK_ERR_BASE + 0x01;

        /// <summary>
        /// 产品标识不能为空
        /// </summary>
        public const uint GKM_TER_ADDRBOOK_PRODUCTID_EMPTY = GKM_TER_ADDRBOOK_ERR_BASE + 0x02;

        /// <summary>
        /// 产品标识长度过长
        /// </summary>
        public const uint GKM_TER_ADDRBOOK_PRODUCTID_MAX = GKM_TER_ADDRBOOK_ERR_BASE + 0x03;

        /// <summary>
        /// 产品版本不能为空
        /// </summary>
        public const uint GKM_TER_ADDRBOOK_PRODUCTVER_EMPTY = GKM_TER_ADDRBOOK_ERR_BASE + 0x04;

        /// <summary>
        /// 产品版本长度过长
        /// </summary>
        public const uint GKM_TER_ADDRBOOK_PRODUCTVER_MAX = GKM_TER_ADDRBOOK_ERR_BASE + 0x05;

        /// <summary>
        /// 版本描述长度过长
        /// </summary>
        public const uint GKM_TER_ADDRBOOK_PRODUCTDES_MAX = GKM_TER_ADDRBOOK_ERR_BASE + 0x06;

        /// <summary>
        /// FTP账号不能为空
        /// </summary>
        public const uint GKM_TER_ADDRBOOK_FTPUSER_EMPTY = GKM_TER_ADDRBOOK_ERR_BASE + 0x07;

        /// <summary>
        /// FTP账号长度过长
        /// </summary>
        public const uint GKM_TER_ADDRBOOK_FTPUSER_MAX = GKM_TER_ADDRBOOK_ERR_BASE + 0x08;

        /// <summary>
        /// FTP密码不能为空
        /// </summary>
        public const uint GKM_TER_ADDRBOOK_FTPPASSWORD_EMPTY = GKM_TER_ADDRBOOK_ERR_BASE + 0x09;

        /// <summary>
        /// FTP密码长度过长
        /// </summary>
        public const uint GKM_TER_ADDRBOOK_FTPPASSWORD_MAX = GKM_TER_ADDRBOOK_ERR_BASE + 0x10;

        /// <summary>
        /// FTP路径不能为空
        /// </summary>
        public const uint GKM_TER_ADDRBOOK_FTPPATH_EMPTY = GKM_TER_ADDRBOOK_ERR_BASE + 0x11;

        /// <summary>
        /// FTP路径长度过长
        /// </summary>
        public const uint GKM_TER_ADDRBOOK_FTPPATH_MAX = GKM_TER_ADDRBOOK_ERR_BASE + 0x12;

        /// <summary>
        /// FTP地址不能为空
        /// </summary>
        public const uint GKM_TER_ADDRBOOK_FTPADDR_EMPTY = GKM_TER_ADDRBOOK_ERR_BASE + 0x13;

        /// <summary>
        /// FTP地址过长
        /// </summary>
        public const uint GKM_TER_ADDRBOOK_FTPADDR_MAX = GKM_TER_ADDRBOOK_ERR_BASE + 0x14;

        /// <summary>
        /// 该输入项只能包含字符,数字和常见符号
        /// </summary>
        public const uint GKM_TER_ADDRBOOK_PRINT_ASC = GKM_TER_ADDRBOOK_ERR_BASE + 0x15;

        /// <summary>
        /// 产品标识冲突
        /// </summary>
        public const uint GKM_TER_PRODUCT_ID_CONFLICT = GKM_TER_ADDRBOOK_ERR_BASE + 0x16;
        #endregion

        #region GK操作日志错误码
        /// <summary>
        /// 开始添加语音网关失败
        /// </summary>
        public const uint GKM_BEGIN_ADD_VOICE_GW_SUCCEED = GKM_GK_OPERATION_LOG_ERR + 1;
        /// <summary>
        /// 开始添加语音网关失败
        /// </summary>
        public const uint GKM_BEGIN_ADD_VOICE_GW_FAILED = GKM_GK_OPERATION_LOG_ERR + 2;
        /// <summary>
        /// 开始修改语音网关成功
        /// </summary>
        public const uint GKM_BEGIN_EDIT_VOICE_GW_SUCCEED = GKM_GK_OPERATION_LOG_ERR + 3;
        /// <summary>
        /// 开始修改语音网关失败
        /// </summary>
        public const uint GKM_BEGIN_EDIT_VOICE_GW_FAILED = GKM_GK_OPERATION_LOG_ERR + 4;
        /// <summary>
        /// 检查语音网关参数成功
        /// </summary>
        public const uint GKM_CHECK_VOICE_GW_PARAM_SUCCEED = GKM_GK_OPERATION_LOG_ERR + 5;
        /// <summary>
        /// 检查语音网关参数失败
        /// </summary>
        public const uint GKM_CHECK_VOICE_GW_PARAM_FAILED = GKM_GK_OPERATION_LOG_ERR + 6;
        /// <summary>
        /// 提交语音网关操作成功
        /// </summary>
        public const uint GKM_COMMIT_OPERATING_VOICE_GW_SUCCEED = GKM_GK_OPERATION_LOG_ERR + 7;
        /// <summary>
        /// 提交语音网关操作失败
        /// </summary>
        public const uint GKM_COMMIT_OPERATING_VOICE_GW_FAILED = GKM_GK_OPERATION_LOG_ERR + 8;
        /// <summary>
        /// 取消语音网关操作成功
        /// </summary>
        public const uint GKM_CANCEL_OPERATING_VOICE_GW_SUCCEED = GKM_GK_OPERATION_LOG_ERR + 9;
        /// <summary>
        /// 取消语音网关操作失败
        /// </summary>
        public const uint GKM_CANCEL_OPERATING_VOICE_GW_FAILED = GKM_GK_OPERATION_LOG_ERR + 0x0A;
        /// <summary>
        /// 删除语音网关操作成功
        /// </summary>
        public const uint GKM_DELETE_VOICE_GW_SUCCEED = GKM_GK_OPERATION_LOG_ERR + 0x0B;
        /// <summary>
        /// 删除语音网关操作失败
        /// </summary>
        public const uint GKM_DELETE_VOICE_GW_FAILED = GKM_GK_OPERATION_LOG_ERR + 0x0C;
        /// <summary>
        /// 修改GK"{firstname}{0}"的邻居GK"{devicename}{1}"成功
        /// </summary>
        public const uint GKM_COMMIT_EDIT_ROUTE_SUCCEED = GKM_GK_OPERATION_LOG_ERR + 0x11;
        /// <summary>
        /// 修改GK"{firstname}{0}"的邻居GK"{devicename}{1}"失败，原因:{err}{0}
        /// </summary>
        public const uint GKM_COMMIT_EDIT_ROUTE_FAILED = GKM_GK_OPERATION_LOG_ERR + 0x12;
        /// <summary>
        /// 添加GK"{firstname}{0}"的邻居GK"{devicename}{1}"成功
        /// </summary>
        public const uint GKM_COMMIT_ADD_ROUTE_SUCCEED = GKM_GK_OPERATION_LOG_ERR + 0x13;
        /// <summary>
        /// 添加GK"{firstname}{0}"的邻居GK"{devicename}{1}"失败，原因:{err}{0}
        /// </summary>
        public const uint GKM_COMMIT_ADD_ROUTE_FAILED = GKM_GK_OPERATION_LOG_ERR + 0x14;
        /// <summary>
        /// 删除GK"{firstname}{0}"的邻居GK"{devicename}{1}"成功
        /// </summary>
        public const uint GKM_DELETE_ROUTE_SUCCEED = GKM_GK_OPERATION_LOG_ERR + 0x17;
        /// <summary>
        /// 删除GK"{firstname}{0}"的邻居GK"{devicename}{1}"失败，原因:{err}{0}
        /// </summary>
        public const uint GKM_DELETE_ROUTE_FAILED = GKM_GK_OPERATION_LOG_ERR + 0x18;
        /// <summary>
        /// 添加GK"{firstname}{0}"的私网区域"{devicename}{1}"成功
        /// </summary>
        public const uint GKM_COMMIT_ADD_PRIVATE_ZONE_SUCCEED = GKM_GK_OPERATION_LOG_ERR + 0x1D;
        /// <summary>
        /// 添加GK"{firstname}{0}"的私网区域"{devicename}{1}"失败，原因:{err}{0}
        /// </summary>
        public const uint GKM_COMMIT_ADD_PRIVATE_ZONE_FAILED = GKM_GK_OPERATION_LOG_ERR + 0x1E;
        /// <summary>
        /// 修改GK"{firstname}{0}"的私网区域"{devicename}{1}"成功
        /// </summary>
        public const uint GKM_COMMIT_EDIT_PRIVATE_ZONE_SUCCEED = GKM_GK_OPERATION_LOG_ERR + 0x1F;
        /// <summary>
        ///修改GK"{firstname}{0}"的私网区域"{devicename}{1}"失败，原因:{err}{0}
        /// </summary>
        public const uint GKM_COMMIT_EDIT_PRIVATE_ZONE_FAILED = GKM_GK_OPERATION_LOG_ERR + 0x20;
        /// <summary>
        /// 删除GK"{firstname}{0}"的私网区域"{devicename}{1}"成功
        /// </summary>
        public const uint GKM_DELETE_PRIVATE_ZONE_SUCCEED = GKM_GK_OPERATION_LOG_ERR + 0x23;
        /// <summary>
        /// 删除GK"{firstname}{0}"的私网区域"{devicename}{1}"失败，原因:{err}{0}
        /// </summary>
        public const uint GKM_DELETE_PRIVATE_ZONE_FAILED = GKM_GK_OPERATION_LOG_ERR + 0x24;
        /// <summary>
        /// 添加GK"{firstname}{0}"的私网区域规则"{devicename}{1}"成功
        /// </summary>
        public const uint GKM_COMMIT_ADD_PRIVATE_ITEM_SUCCEE = GKM_GK_OPERATION_LOG_ERR + 0x29;
        /// <summary>
        ///添加GK"{firstname}{0}"的私网区域规则"{devicename}{1}"失败，原因:{err}{0}
        /// </summary>
        public const uint GKM_COMMIT_ADD_PRIVATE_ITEM_FAILED = GKM_GK_OPERATION_LOG_ERR + 0x2A;
        /// <summary>
        /// 修改GK"{firstname}{0}"的私网区域规则"{devicename}{1}"成功
        /// </summary>
        public const uint GKM_COMMIT_EDIT_PRIVATE_ITEM_SUCCEE = GKM_GK_OPERATION_LOG_ERR + 0x2B;
        /// <summary>
        /// 修改GK"{firstname}{0}"的私网区域规则"{devicename}{1}"失败，原因:{err}{0}
        /// </summary>
        public const uint GKM_COMMIT_EDIT_PRIVATE_ITEM_FAILED = GKM_GK_OPERATION_LOG_ERR + 0x2C;
        /// <summary>
        /// 删除GK"{firstname}{0}"的私网区域规则"{devicename}{1}"成功
        /// </summary>
        public const uint GKM_DELETE_PRIVATE_ITEM_SUCCEED = GKM_GK_OPERATION_LOG_ERR + 0x2F;
        /// <summary>
        /// 删除GK"{firstname}{0}"的私网区域规则"{devicename}{1}"失败，原因:{err}{0}
        /// </summary>
        public const uint GKM_DELETE_PRIVATE_ITEM_FAILED = GKM_GK_OPERATION_LOG_ERR + 0x30;
        /// <summary>
        /// 添加GK"{firstname}{0}"的网络地址本FTP"{devicename}{1}"成功
        /// </summary>
        public const uint GKM_COMMIT_ADD_TERADDRBOOK_SUCCEE = GKM_GK_OPERATION_LOG_ERR + 0x35;
        /// <summary>
        /// 添加GK"{firstname}{0}"的网络地址本FTP"{devicename}{1}"失败，原因:{err}{0}
        /// </summary>
        public const uint GKM_COMMIT_ADD_TERADDRBOOK_FAILED = GKM_GK_OPERATION_LOG_ERR + 0x36;
        /// <summary>
        /// 修改GK"{firstname}{0}"的网络地址本FTP"{devicename}{1}"成功
        /// </summary>
        public const uint GKM_COMMIT_EDIT_TERADDRBOOK_SUCCEE = GKM_GK_OPERATION_LOG_ERR + 0x37;
        /// <summary>
        /// 修改GK"{firstname}{0}"的网络地址本FTP"{devicename}{1}"失败，原因:{err}{0}
        /// </summary>
        public const uint GKM_COMMIT_EDIT_TERADDRBOOK_FAILED = GKM_GK_OPERATION_LOG_ERR + 0x38;
        /// <summary>
        /// 删除GK"{firstname}{0}"的网络地址本FTP"{devicename}{1}"成功
        /// </summary>
        public const uint GKM_DELETE_TERADDRBOOK_SUCCEED = GKM_GK_OPERATION_LOG_ERR + 0x3B;
        /// <summary>
        ///删除GK"{firstname}{0}"的网络地址本FTP"{devicename}{1}"失败，原因:{err}{0}
        /// </summary>
        public const uint GKM_DELETE_TERADDRBOOK_FAILED = GKM_GK_OPERATION_LOG_ERR + 0x3C;
        /// <summary>
        /// 失效GK的License操作成功。
        /// </summary>
        public const uint GKM_REVOKE_TICKET_SUCCEED = GKM_GK_OPERATION_LOG_ERR + 0x3D;
        /// <summary>
        /// 失效GK的License操作失败。
        /// </summary>
        public const uint GKM_REVOKE_TICKET_FAILED = GKM_GK_OPERATION_LOG_ERR + 0x3E;
        /// <summary>
        /// 切换Mcu成功。
        /// </summary>
        public const uint GKM_SWITCH_MCU_SUCCEED = GKM_GK_OPERATION_LOG_ERR + 0x3F;
        /// <summary>
        /// 切换Mcu成功。
        /// </summary>
        public const uint GKM_SWITCH_MCU_FAILED = GKM_GK_OPERATION_LOG_ERR + 0x40;
        /// <summary>
        /// Gk（{gkip}）上线了。
        /// </summary>
        public const uint GKM_GK_IS_ONLINE = GKM_GK_OPERATION_LOG_ERR + 0x41;
        /// <summary>
        /// GK（{gkip}）下线了。
        /// </summary>
        public const uint GKM_GK_IS_OFFLINE = GKM_GK_OPERATION_LOG_ERR + 0x42;
        /// <summary>
        /// 通知GK"{firstname}{0}"的会场更改网络地址本FTP"{devicename}{1}"成功
        /// </summary>
        public const uint GKM_NOTIFY_UPDATE_ADDRESS_BOOK_SUCCEED = GKM_GK_OPERATION_LOG_ERR + 0x43;
        /// <summary>
        /// 通知GK"{firstname}{0}"的会场更改网络地址本FTP"{devicename}{1}"失败，原因:{err}{0}
        /// </summary>
        public const uint GKM_NOTIFY_UPDATE_ADDRESS_BOOK_FAILED = GKM_GK_OPERATION_LOG_ERR + 0x44;
        /// <summary>
        /// 用户{user}导出GK日志失败。
        /// </summary>
        public const uint GKM_EXPORT_GK_LOG_FAILED = GKM_GK_OPERATION_LOG_ERR + 0x45;
        /// <summary>
        /// 用户{user}导出GK日志成功。
        /// </summary>
        public const uint GKM_EXPORT_GK_LOG_SUCCEED = GKM_GK_OPERATION_LOG_ERR + 0x46;

        #endregion

        #region GK设备日志
        /// <summary>
        /// 另一个用户正在导出日志，请稍候再导出
        /// </summary>
        public const uint GKM_GK_DEVICE_LOADING_LOG_EXIST = GKM_GK_DEVICE_LOG_ERR_BASE + 0x01;

        /// <summary>
        /// 下载GK日志超时
        /// </summary>
        public const uint GKM_GK_DEVICE_LOG_LOAD_TIME_OUT = GKM_GK_DEVICE_LOG_ERR_BASE + 0x02;

        /// <summary>
        /// 下载Gk日志时写入文件失败
        /// </summary>
        public const uint GKM_GK_DEVICE_LOG_WRITE_FIEL_FAILED = GKM_GK_DEVICE_LOG_ERR_BASE + 0x03;

        #endregion

        #endregion

        #region 数据库服务的错误码0x50700000
        //////////////////////////////////////////////////////////////////////////
        // 数据库服务的错误码0x50700000
        //////////////////////////////////////////////////////////////////////////

        #region 系统公用错误码0x50700000
        /// <summary>
        /// 数据库服务管理错误码
        /// </summary>
        public const uint ERR_DB_SERVICE = 0x50700000;

        /// <summary>
        /// 数据库连接失败
        /// </summary>
        public const uint DB_CONN_OPEN_ERROR = ERR_DB_SERVICE + 1;

        /// <summary>
        /// 事务回滚失败
        /// </summary>
        public const uint DB_TRAN_ROLLBACK_ERROR = ERR_DB_SERVICE + 2;

        /// <summary>
        /// 执行SQL语句失败
        /// </summary>
        public const uint DB_SQL_EXEC_ERROR = ERR_DB_SERVICE + 3;

        /// <summary>
        /// 数据库数据版本不一致
        /// </summary>
        public const uint DB_DATA_DATAVER_ERROR = ERR_DB_SERVICE + 4;

        /// <summary>
        /// 请求数据不存在
        /// </summary>
        public const uint DB_DATA_NOTIN_ERROR = ERR_DB_SERVICE + 5;

        /// <summary>
        /// 事务提交异常
        /// </summary>
        public const uint DB_TRAN_COMMIT_ERROR = ERR_DB_SERVICE + 6;

        /// <summary>
        /// 创建事务失败
        /// </summary>
        public const uint DB_TRAN_CREATE_ERROR = ERR_DB_SERVICE + 7;

        /// <summary>
        /// 数据库连接为空
        /// </summary>
        public const uint DB_CONNECT_IS_NULL = ERR_DB_SERVICE + 8;

        /// <summary>
        /// 请求的事务不存在
        /// </summary>
        public const uint DB_TRANSACTION_NOT_EXIST = ERR_DB_SERVICE + 9;

        /// <summary>
        /// 数据库版本号错误
        /// </summary>
        public const uint DB_DATAVER_ERR = ERR_DB_SERVICE + 0x0A;

        /// <summary>
        /// 传入参数错误
        /// </summary>
        public const uint DB_CURRENTPAGE_ERR = ERR_DB_SERVICE + 0x0B;

        /// <summary>
        /// 话单数据写入失败
        /// </summary>
        public const uint DB_WRITECDR_ERR = ERR_DB_SERVICE + 0x0C;

        /// <summary>
        /// 设备个数已经达到最大值
        /// </summary>
        public const uint DB_DEVICECOUNT_REACH_MAX = ERR_DB_SERVICE + 0x0D;

        /// <summary>
        /// 初始化错误码失败
        /// </summary>
        public const uint DB_LOADSMCERR_FAILED = ERR_DB_SERVICE + 0x0E;

        /// <summary>
        /// 修改配置失败
        /// </summary>
        public const uint DB_MODIFYCONFIG_FAILED = ERR_DB_SERVICE + 0x0F;

        /// <summary>
        /// 报表设置参数错误
        /// </summary>
        public const uint REPORT_SETPARAMETER_FAILED = ERR_DB_SERVICE + 0x10;

        /// <summary>
        /// 数据库出错
        /// </summary>
        public const uint DB_ERROR = ERR_DB_SERVICE + 0x11;
        #endregion

        #region 配置项共用错误码0x50701000
        /// <summary>
        /// 配置项共用错误码0x50701000
        /// </summary>
        public const uint ERR_DB_COMMONCONFIG = 0x50701000;

        /// <summary>
        /// 配置列表为空
        /// </summary>
        public const uint DB_CONFIG_LIST_ISNULL = ERR_DB_COMMONCONFIG + 1;

        /// <summary>
        /// 正在编辑的配置列表不存在
        /// </summary>
        public const uint DB_CONFIG_EDIT_NOEXIST = ERR_DB_COMMONCONFIG + 2;

        /// <summary>
        /// 配置项不存在
        /// </summary>
        public const uint DB_CONFIG_NOEXIST = ERR_DB_COMMONCONFIG + 3;

        /// <summary>
        /// 提交保存的配置项为空
        /// </summary>
        public const uint DB_CONFIG_NOSUBMITITEM = ERR_DB_COMMONCONFIG + 4;

        /// <summary>
        /// 配置项的值必须为整型
        /// </summary>
        public const uint DB_CONFIG_INTEGER_ERROR = ERR_DB_COMMONCONFIG + 5;

        /// <summary>
        /// 配置项的值必须为布尔型
        /// </summary>
        public const uint DB_CONFIG_BOOLEAN_ERROR = ERR_DB_COMMONCONFIG + 6;

        /// <summary>
        /// 配置项的值必须为浮点型
        /// </summary>
        public const uint DB_CONFIG_DOUBLE_ERROR = ERR_DB_COMMONCONFIG + 7;

        /// <summary>
        /// 配置项为空
        /// </summary>
        public const uint DB_CONFIG_ISNULL_ERROR = ERR_DB_COMMONCONFIG + 8;

        /// <summary>
        /// 配置项的关联服务为空
        /// </summary>
        public const uint DB_CONFIG_SERVICELIST_ISNULL_ERROR = ERR_DB_COMMONCONFIG + 9;

        /// <summary>
        /// 配置项的关联服务不存在
        /// </summary>
        public const uint DB_CONFIG_SERVICE_NOEXIST_ERROR = ERR_DB_COMMONCONFIG + 0x0A;

        /// <summary>
        /// 获取配置项的值失败
        /// </summary>
        public const uint DB_CONFIG_GET_VALUE_ERROR = ERR_DB_COMMONCONFIG + 0x0B;

        /// <summary>
        /// 配置项值类型错误
        /// </summary>
        public const uint DB_CONFIG_TYPE_ERROR = ERR_DB_COMMONCONFIG + 0x0C;

        /// <summary>
        /// 配置项未定义
        /// </summary>
        public const uint DB_CONFIG_NODEFINE_ERROR = ERR_DB_COMMONCONFIG + 0x0D;

        /// <summary>
        /// 初始化配置数据为空
        /// </summary>
        public const uint DB_CONFIG_ITEM_INIT_NULL_ERROR = ERR_DB_COMMONCONFIG + 0xE;

        /// <summary>
        /// 没有数据被更新
        /// </summary>
        public const uint DB_UPDATE_NUM_NULL = ERR_DB_COMMONCONFIG + 0xF;
        #endregion

        #region 会议参数配置错误码0x50702000
        /// <summary>
        /// 会议参数配置起始错误码
        /// </summary>
        public const uint ERR_DB_CONFCONFIG = 0x50702000;

        /// <summary>
        /// 会议特服号和PSTN特服号不能重叠
        /// </summary>
        public const uint DB_CONFIG_SPECAILCODE_OVERLAP_ERROR = ERR_DB_CONFCONFIG + 1;

        /// <summary>
        /// 会议速率区间阈值不能小于最小值1
        /// </summary>
        public const uint DB_CONFIG_THRESHOLD_MINERR = ERR_DB_CONFCONFIG + 2;

        /// <summary>
        /// 单个周期会议的最大会议数必须在1～100之间的数字
        /// </summary>
        public const uint DB_DEFPERIODIC_CONF_NUM_MINLENGTH_ERR = ERR_DB_CONFCONFIG + 3;

        /// <summary>
        /// 呼叫间隔不能大于持续呼叫时间
        /// </summary>
        public const uint DB_CLLINTERVALMAXVALUE_ERR = ERR_DB_CONFCONFIG + 4;

        /// <summary>
        /// 主叫呼集默认时长不能小于最小值20,不能大于最大值360
        /// </summary>
        public const uint DB_SITECALLTIME_ERR = ERR_DB_CONFCONFIG + 5;

        /// <summary>
        /// 最小延长会议时长不能小于最小值10,不能大于最大值360
        /// </summary>
        public const uint DB_MINPROLONGTIMESPAN_ERR = ERR_DB_CONFCONFIG + 6;

        /// <summary>
        /// 最大延长会议时长不能小于最小值360,不能大于最大值99999
        /// </summary>
        public const uint DB_MAXPROLONGTIMESPAN_ERR = ERR_DB_CONFCONFIG + 7;

        /// <summary>
        /// 最小预约会议时长不能小于最小值20,不能大于最大值360
        /// </summary>
        public const uint DB_MINBOOKTIMESPAN_ERR = ERR_DB_CONFCONFIG + 8;

        /// <summary>
        /// 最大预约会议时长不能小于最小值360,不能大于最大值99999
        /// </summary>
        public const uint DB_MAXBOOKTIMESPAN_ERR = ERR_DB_CONFCONFIG + 9;

        /// <summary>
        /// 该输入项只能包含字母，数字和以下符号： @ . - _
        /// (可输入域名的错误提示)
        /// </summary>
        public const uint DB_DOMAIN_NAME_ERR = ERR_DB_CONFCONFIG + 0x0A;

        /// <summary>
        /// Ad hoc自动延长会议时长必须在最小和最大自动延长会议时长配置之间
        /// </summary>
        public const uint DB_ADHOCEXTENDLESSMINEXTEND_ERR = ERR_DB_CONFCONFIG + 0x0B;

        /// <summary>
        /// 统一接入号最大长度为13
        /// </summary>
        public const uint DB_CONIG_UNIFIEDCODE_MAXLENGTH_ERR = ERR_DB_CONFCONFIG + 0x0C;

        /// <summary>
        /// 结束会议重试次数不能小于最小值1,不能大于最大值30
        /// </summary>
        public const uint DB_ENDCONFRETRIES_ERR = ERR_DB_CONFCONFIG + 0x0D;

        /// <summary>
        /// 启动会议重试次数不能小于最小值1，不能大于最大值30
        /// </summary>
        public const uint DB_STARTCONFRETRIES_ERR = ERR_DB_CONFCONFIG + 0x0E;

        /// <summary>
        /// 会控超时时间不能小于最小值5，不能大于最大值120
        /// </summary>
        public const uint DB_CONFCTRLTIMEOUT_ERR = ERR_DB_CONFCONFIG + 0x0F;

        /// <summary>
        /// 主叫会场入会等待时间不能小于最小值60，不能大于最大值7200
        /// </summary>
        public const uint DB_CALLSITEWAITTIME_ERR = ERR_DB_CONFCONFIG + 0x10;

        /// <summary>
        /// 所有会场不入会等待时间不能小于最小值60，不能大于最大值7200
        /// </summary>
        public const uint DB_ALLSITEWAITTIME_ERR = ERR_DB_CONFCONFIG + 0x11;

        /// <summary>
        /// 初始化声控权限不能小于最小值1，不能大于最大值100
        /// </summary>
        public const uint DB_INITVOICEACTIVATION_ERR = ERR_DB_CONFCONFIG + 0x12;

        /// <summary>
        /// 会场离线时长不能大于最大值100
        /// </summary>
        public const uint DB_OFFLINETIME_ERR = ERR_DB_CONFCONFIG + 0x13;

        /// <summary>
        /// 离线重呼持续时长不能小于最小值1，不能大于最大值9999
        /// </summary>
        public const uint DB_RECALLHOLDTIME_ERR = ERR_DB_CONFCONFIG + 0x14;

        /// <summary>
        /// 该输入项的最大长度是256
        /// </summary>
        public const uint DB_SITECLOWSTR_MAX_LENGTH_ERR = ERR_DB_CONFCONFIG + 0x15;

        /// <summary>
        /// 该输入项的长度必须在1～16之间
        /// </summary>
        public const uint DB_GKMUICONNECTPWD_MAX_LENGTH_ERR = ERR_DB_CONFCONFIG + 0x16;

        /// <summary>
        /// SIP客户端连接数最小值为1，最大值为4
        /// </summary>
        public const uint DB_GKMMAXSIPCLIENT_MINORMAX_ERR = ERR_DB_CONFCONFIG + 0x17;

        /// <summary>
        /// SIP握手周期的最小值为20，最大值为120
        /// </summary>
        public const uint DB_SIPHEARTBEATTIME_MINORMAX_ERR = ERR_DB_CONFCONFIG + 0x18;

        /// <summary>
        /// 该输入项的最大长度是256
        /// </summary>
        public const uint DB_GKMLOGDIR_MAX_LENGTH_ERR = ERR_DB_CONFCONFIG + 0x19;

        /// <summary>
        /// 日志文件大小的最小值为64，最大值为1024
        /// </summary>
        public const uint DB_GKMLOGSIZE_MINORMAX_ERR = ERR_DB_CONFCONFIG + 0x1A;

        /// <summary>
        /// 输入的日志路径不存在
        /// </summary>
        public const uint DB_GKMLOGDIR_NOTEXISTS_ERR = ERR_DB_CONFCONFIG + 0x1B;

        /// <summary>
        /// Email服务器地址最大长度是256
        /// </summary>
        public const uint DB_SMTPSERVER_MAX_LENGTH_ERR = ERR_DB_CONFCONFIG + 0x1C;

        /// <summary>
        /// Email密码最大长度为32
        /// </summary>
        public const uint DB_EMAILPASSWORD_MAX_LENGTH_ERR = ERR_DB_CONFCONFIG + 0x1D;

        /// <summary>
        /// 名称的最大长度为64
        /// </summary>
        public const uint DB_NAME_MAX_LENGTH_ERR = ERR_DB_CONFCONFIG + 0x1E;

        /// <summary>
        /// LDAP密码最大长度为32
        /// </summary>
        public const uint DB_LDAPPASSWORD_MAX_LENGTH_ERR = ERR_DB_CONFCONFIG + 0x1F;

        /// <summary>
        /// PSTN特服号最大长度为5
        /// </summary>
        public const uint DB_PSTN_SPECIAL_CODE_MAX_LENGTH_ERR = ERR_DB_CONFCONFIG + 0x20;

        /// <summary>
        /// 会议特服号最大长度为12
        /// </summary>
        public const uint DB_CONF_SPECIAL_CODE_MAX_LENGTH_ERR = ERR_DB_CONFCONFIG + 0x21;

        /// <summary>
        /// 有接入MCU时预留端口数必须大于0
        /// </summary>
        public const uint DB_CONF_PORT_NUMVER_ERR = ERR_DB_CONFCONFIG + 0x22;

        /// <summary>
        /// 呼叫间隔必须在1～55之间的数字
        /// </summary>
        public const uint DB_CALL_INTERVAL_MAXVALUE_ERR = ERR_DB_CONFCONFIG + 0x23;

        /// <summary>
        /// 持续呼叫时间必须在1～9999之间的数字
        /// </summary>
        public const uint DB_CONTINUE_CALL_TIME_MAXVALUE_ERR = ERR_DB_CONFCONFIG + 0x24;

        /// <summary>
        /// 该输入项必须在0～100之间的数字
        /// (备注：NlogV2配置中门限最大值错误)
        /// </summary>
        public const uint DB_LOSSALARM_VIDELSEND_MAXVALUE_ERR = ERR_DB_CONFCONFIG + 0x25;

        /// <summary>
        /// 该输入项必须在0～1000之间的数字
        /// （备注：发送方向网络抖动门限和接受方向网络抖动门限）
        /// </summary>
        public const uint DB_LOSSALARM_NETDELAY_MAXVALUE_ERR = ERR_DB_CONFCONFIG + 0x26;

        /// <summary>
        /// 该输入项必须在1～60之间的数字
        /// </summary>
        public const uint DB_LOSSALARM_IFRAME_MAXVALUE_ERR = ERR_DB_CONFCONFIG + 0x27;

        /// <summary>
        /// 该输入项必须在0～6000之间的数字
        /// (备注：会场视频接收连续丢包数告警门限和会场视频发送连续丢包数告警门限)
        /// </summary>
        public const uint DB_LOSSALARM_VDRECSERLOSSRATENUM_MAXVALUE_ERR = ERR_DB_CONFCONFIG + 0x28;

        /// <summary>
        /// 计费码已经存在
        /// </summary>
        public const uint DB_BILLCODE_NOT_REPEAT_ERR = ERR_DB_CONFCONFIG + 0x29;

        /// <summary>
        /// 统一接入号为XXX或XXX@域名格式，总长度不能超过127，其中XXX为长度不超过13的数字字符串。
        /// </summary>
        public const uint DB_CONIG_CODE_DOMAIN_FORMAR_ERR = ERR_DB_CONFCONFIG + 0x2B;

        /// <summary>
        /// 所有会场离会后会议结束时间必须大于1
        /// </summary>
        public const uint DB_TERMINATECONFAFTERSITEDEFECTION_ERR = ERR_DB_CONFCONFIG + 0x2C;

        /// <summary>
        /// 该输入项必须在100～10240之间的数字
        /// (备注：Nlog存档文件最大长度)
        /// </summary>
        public const uint DB_ARCHIVEFILEMAXLEN_ERR = ERR_DB_CONFCONFIG + 0x2D;

        /// <summary>
        /// 该输入项必须在1～366之间的数字
        /// (备注：主文件记录最旧时间最大值)
        /// </summary>
        public const uint DB_MAINOLDESTIME_ERR = ERR_DB_CONFCONFIG + 0x2E;

        /// <summary>
        /// 默认会议名称最大长度为40
        /// </summary>
        public const uint DB_DEFAULT_CONF_NAME_MAX_LENGTH_ERR = ERR_DB_CONFCONFIG + 0x2F;

        /// <summary>
        /// EConf账号只能输入长度6－21位的字母、数字、下划线“_”
        /// </summary>
        public const uint DB_ECONF_ACCOUNT_ERR = ERR_DB_CONFCONFIG + 0x30;

        /// <summary>
        /// EConf企业标识只能输入长度6－20位的字母和数字
        /// </summary>
        public const uint DB_ECONF_ENTERPRISEID_ERR = ERR_DB_CONFCONFIG + 0x31;

        /// <summary>
        /// EConf AS地址格式必须为URL格式
        /// </summary>
        public const uint DB_ECONF_ASADDR_ERR = ERR_DB_CONFCONFIG + 0x32;

        /// <summary>
        /// 该输入项只能输入的数字和字母
        /// </summary>
        public const uint ERR_ONLY_NUMBERANDLETTER_ERROR = ERR_DB_CONFCONFIG + 0x33;

        /// <summary>
        /// 不能和会议特服号重复
        /// </summary>
        public const uint DB_CONIG_UNIFICATION_SPECAIL_SAME_ERR = ERR_DB_CONFCONFIG + 0x34;

        /// <summary>
        /// 不能和PSTN特服号重复
        /// </summary>
        public const uint DB_CONIG_UNIFICATION_PSTN_SAME_ERR = ERR_DB_CONFCONFIG + 0x35;

        /// <summary>
        /// 不能和统一接入号重复
        /// </summary>
        public const uint DB_CONIG_UNIFICATION_SAME_ERR = ERR_DB_CONFCONFIG + 0x36;

        /// <summary>
        /// 告警门限值必须小于严重告警门限值
        /// </summary>
        public const uint DB_CONIG_LOSS_ALARM_MIN_ERR = ERR_DB_CONFCONFIG + 0x37;

        /// <summary>
        /// 录播数据录入有误
        /// </summary>
        public const uint DB_NRS_ERR = ERR_DB_CONFCONFIG + 0x38;

        /// <summary>
        /// LDAP服务器地址不可为空
        /// </summary>
        public const uint DB_CONFIG_LDAPADDRESS_EMPTY_ERR = ERR_DB_CONFCONFIG + 0x39;

        /// <summary>
        /// LDAP服务器地址参考“ldap.smc20.com”,或者IP地址格式
        /// </summary>
        public const uint DB_CONFIG_LDAPADDRESS_ERR = ERR_DB_CONFCONFIG + 0x3A;

        /// <summary>
        /// 基准DN不可为空
        /// </summary>
        public const uint DB_CONFIG_LDAPBASEDN_EMPTY_ERR = ERR_DB_CONFCONFIG + 0x3B;

        /// <summary>
        /// 基准DN参考“CN=LdapT,DC=mcutest,DC=com”
        /// </summary>
        public const uint DB_CONFIG_LDAPBASEDN_ERR = ERR_DB_CONFCONFIG + 0x3C;

        /// <summary>
        /// LDAP用户名不可为空
        /// </summary>
        public const uint DB_CONFIG_LDAPUSERNAME_EMPTY_ERR = ERR_DB_CONFCONFIG + 0x3D;

        /// <summary>
        /// LDAP密码不可为空
        /// </summary>
        public const uint DB_CONFIG_LDAPPASSWORD_EMPTY_ERR = ERR_DB_CONFCONFIG + 0x3E;

        /// <summary>
        /// 配置项只能为英文字母
        /// </summary>
        public const uint DB_CONFIG_ENGLISH_ERR = ERR_DB_CONFCONFIG + 0x3F;

        /// <summary>
        /// 严重告警门限值必须大于告警门限值
        /// </summary>
        public const uint DB_CONIG_LOSS_ALARM_MAX_ERR = ERR_DB_CONFCONFIG + 0x40;

        /// <summary>
        /// 数据会议账号只能输入长度1－100位的字母、数字、下划线“_”
        /// </summary>
        public const uint DB_DATAECONF_ACCOUNT_ERR = ERR_DB_CONFCONFIG + 0x41;

        /// <summary>
        /// 数据会议企业标识只能输入长度1－100位的字母和数字
        /// </summary>
        public const uint DB_DATAECONF_ENTERPRISEID_ERR = ERR_DB_CONFCONFIG + 0x42;
        #endregion

        #region 分区策略和调度策略配置错误码0x50703000
        /// <summary>
        /// 分区策略和调度策略配置错误码0x50703000
        /// </summary>
        public const uint ERR_DB_TACTIC = 0x50703000;

        /// <summary>
        /// 会议速率区间阀值超出范围
        /// </summary>
        public const uint DB_CONFIG_THRESHOLD_OVERSTEP = ERR_DB_TACTIC + 1;

        /// <summary>
        /// 会议速率区间个数错误
        /// </summary>
        public const uint DB_CONFIG_CONFRATEAREA_COUNT_ERROR = ERR_DB_TACTIC + 2;

        /// <summary>
        /// 会议速率区间不存在
        /// </summary>
        public const uint DB_CONFIG_CONFRATEAREA_NOEXISTS_ERROR = ERR_DB_TACTIC + 3;

        /// <summary>
        /// 会议速率区间为空
        /// </summary>
        public const uint DB_CONFIG_CONFRATEAREA_NULL_ERROR = ERR_DB_TACTIC + 4;

        /// <summary>
        /// 子会议区间为空
        /// </summary>
        public const uint DB_CONFIG_SUBCONFAREA_NULL_ERROR = ERR_DB_TACTIC + 5;

        /// <summary>
        /// 子会议区间的值超出范围
        /// </summary>
        public const uint DB_CONFIG_SUBCONFAREA_VALUE_OVERSTEP_ERROR = ERR_DB_TACTIC + 6;

        /// <summary>
        /// 子会议区间的最大值不能小于最小值
        /// </summary>
        public const uint DB_CONFIG_SUBCONFAREA_MAXVALUE_LT_MINVALUE_ERROR = ERR_DB_TACTIC + 7;

        /// <summary>
        /// 子会议区间的最小值不能大于最大值
        /// </summary>
        public const uint DB_CONFIG_SUBCONFAREA_MINVALUE_GT_MAXNVALUE_ERROR = ERR_DB_TACTIC + 8;

        /// <summary>
        /// 子会议区间值重叠
        /// </summary>
        public const uint DB_CONFIG_SUBCONFAREA_VALUE_OVERLAP_ERROR = ERR_DB_TACTIC + 9;

        /// <summary>
        /// 子会议区间个数最大值为50
        /// </summary>
        public const uint DB_CONFIG_SUBCONFAREA_COUNT_ERROR = ERR_DB_TACTIC + 0x0A;

        /// <summary>
        /// 子会议区间不存在
        /// </summary>
        public const uint DB_CONFIG_SUBCONFAREA_NOEXISTS_ERROR = ERR_DB_TACTIC + 0x0B;

        /// <summary>
        /// 主辅流速率对应关系为空
        /// </summary>
        public const uint DB_CONFIG_MAINSTREAMAMCRATE_NULL_ERROR = ERR_DB_TACTIC + 0x0C;

        /// <summary>
        /// 编辑的主辅流速率对应关系不存在
        /// </summary>
        public const uint DB_CONFIG_MAINSTREAMAMCRATE_NOEXISTS_ERROR = ERR_DB_TACTIC + 0x0D;

        /// <summary>
        /// 主辅流视频对应关系为空
        /// </summary>
        public const uint DB_CONFIG_MAINSTREAMAMCVIDEO_NULL_ERROR = ERR_DB_TACTIC + 0x0E;

        /// <summary>
        /// 编辑的主辅流视频对应关系不存在
        /// </summary>
        public const uint DB_CONFIG_MAINSTREAMAMCVIDEO_NOEXISTS_ERROR = ERR_DB_TACTIC + 0x0F;

        /// <summary>
        /// 子会议区间的最小值配置错误
        /// </summary>
        public const uint DB_CONFIG_SUBCONFAREA_MINVALUE_SMALL_BEGINMAXVALUE_ERROR = ERR_DB_TACTIC + 0x10;
        #endregion
        #endregion

        #region email服务错误码0x50800000
        //////////////////////////////////////////////////////////////////////////
        //邮件服务管理错误码0x50800000
        //////////////////////////////////////////////////////////////////////////

        /// <summary>
        /// SMTP服务名错误
        /// </summary>
        public const uint ERR_EMAIL_SERVERNULL = 0x50800000;

        /// <summary>
        /// 载入Email_SMTP模块失败
        /// </summary>
        public const uint ERR_EMAIL_LOADSMTP = ERR_EMAIL_SERVERNULL + 1;

        /// <summary>
        /// 初始化SMTP模块失败
        /// </summary>
        public const uint ERR_EMAIL_INITSMTP = ERR_EMAIL_SERVERNULL + 2;

        /// <summary>
        /// 参数为空
        /// </summary>
        public const uint ERR_EMAIL_PARAMNULL = ERR_EMAIL_SERVERNULL + 3;

        /// <summary>
        /// 参数解析错误
        /// </summary>
        public const uint ERR_EMAIL_EXTERNPARAM = ERR_EMAIL_SERVERNULL + 4;

        /// <summary>
        /// 载入模板失败（警告）
        /// </summary>
        public const uint ERR_EMAIL_LOADTEMPLATE = ERR_EMAIL_SERVERNULL + 5;

        /// <summary>
        /// 邮箱错误
        /// </summary>
        public const uint ERR_EMAIL_EMAILBOX = ERR_EMAIL_SERVERNULL + 6;

        /// <summary>
        /// 从数据库中没有会场数据
        /// </summary>
        public const uint ERR_EMAIL_SITE_NOVALID = ERR_EMAIL_SERVERNULL + 7;

        /// <summary>
        /// 邮件发送失败
        /// </summary>
        public const uint ERR_EMAIL_UNABLERELAY = ERR_EMAIL_SERVERNULL + 8;
        //xianghaoxiang KF58774

        /// <summary>
        /// 载入Email.XslTransform模块失败
        /// </summary>
        public const uint ERR_EMAIL_LOADXSLTRANSFORM = ERR_EMAIL_SERVERNULL + 8; //Link_
        //xianghaoxiang KF58774
        /// <summary>
        /// 生效时间
        /// </summary>
        public const uint ERR_EMAIL_EFFICIENTTIME = ERR_EMAIL_SERVERNULL + 9;
        /// <summary>
        /// 周期会议天模式
        /// </summary>
        public const uint ERR_EMAIL_REPEATPERIODS_DAILY = ERR_EMAIL_SERVERNULL + 0x0A;

        /// <summary>
        /// 周期会议月模式
        /// </summary>
        public const uint ERR_EMAIL_REPEATPERIODS_MONTHLY = ERR_EMAIL_SERVERNULL + 0x0B;

        /// <summary>
        /// 周期会议月模式
        /// </summary>
        public const uint ERR_EMAIL_REPEATPERIODS_MONTHLY2_PLURAL = ERR_EMAIL_SERVERNULL + 0x0C;

        /// <summary>
        /// 周期会议月模式
        /// </summary>
        public const uint ERR_EMAIL_REPEATPERIODS_MONTHLY2_SINGLE = ERR_EMAIL_SERVERNULL + 0x0D;

        /// <summary>
        /// 周期会议周模式
        /// </summary>
        public const uint ERR_EMAIL_REPEATPERIODS_WEEKLY = ERR_EMAIL_SERVERNULL + 0x0E;

        /// <summary>
        /// 周期会议年模式
        /// </summary>
        public const uint ERR_EMAIL_REPEATPERIODS_YEARLY = ERR_EMAIL_SERVERNULL + 0x0F;

        /// <summary>
        /// 周期会议年模式
        /// </summary>
        public const uint ERR_EMAIL_REPEATPERIODS_YEARLY2 = ERR_EMAIL_SERVERNULL + 0x10;

        public const uint ERR_EMAIL_SUNDAY = ERR_EMAIL_SERVERNULL + 0x11;

        public const uint ERR_EMAIL_MONDAY = ERR_EMAIL_SERVERNULL + 0x12;

        public const uint ERR_EMAIL_TUESDAY = ERR_EMAIL_SERVERNULL + 0x13;

        public const uint ERR_EMAIL_WEDNESDAY = ERR_EMAIL_SERVERNULL + 0x14;

        public const uint ERR_EMAIL_THURSDAY = ERR_EMAIL_SERVERNULL + 0x15;

        public const uint ERR_EMAIL_FRIDAY = ERR_EMAIL_SERVERNULL + 0x16;

        public const uint ERR_EMAIL_SATURDAY = ERR_EMAIL_SERVERNULL + 0x17;

        /// <summary>
        /// 重复发送Email告警通知的周期只能输入1－360的数字
        /// </summary>
        public const uint ERR_EMAIL_RETIME = ERR_EMAIL_SERVERNULL + 0x18;

        /// <summary>
        /// 重复发送Email告警通知的次数只能输入0－10的数字
        /// </summary>
        public const uint ERR_EMAIL_RENUMBER = ERR_EMAIL_SERVERNULL + 0x19;

        /// <summary>
        /// 发送Email告警通知时，检查传入参数有误。
        /// </summary>
        public const uint ERR_EMAIL_DOWNPARAM = ERR_EMAIL_SERVERNULL + 0x1A;

        /// <summary>
        /// 发送Email时，会议email模板的标题为空。
        /// </summary>
        public const uint ERR_EMAIL_SUBJECT_IS_EMPTY = ERR_EMAIL_SERVERNULL + 0x1B;

        /// <summary>
        /// Email服务开启
        /// </summary>
        public const uint EMAIL_SERVICE_START = ERR_EMAIL_SERVERNULL + 0x1C;

        /// <summary>
        /// Email服务停止
        /// </summary>
        public const uint EMAIL_SERVICE_STOP = ERR_EMAIL_SERVERNULL + 0x1D;
        #endregion

        #region 前台服务错误码0x50900000
        /// <summary>
        /// 前台服务错误码0x50900000
        /// </summary>
        public const uint ERR_SESSION_SERVICE = 0x50900000;

        /// <summary>
        /// 获取会话管理中的用户服务失败
        /// </summary>
        public const uint SESSION_USERSERVICEERR = ERR_SESSION_SERVICE + 1;

        /// <summary>
        /// 添加子会话失败
        /// </summary>
        public const uint SESSION_SUBSESSION_ADDERR = ERR_SESSION_SERVICE + 2;

        /// <summary>
        /// 子会话不存在子会话集合中
        /// </summary>
        public const uint SESSION_SUBSESSION_NOTEXIST = ERR_SESSION_SERVICE + 3;

        /// <summary>
        /// 会话已存在会话集合中
        /// </summary>
        public const uint SESSION_GETSESSION_EXIST = ERR_SESSION_SERVICE + 4;

        /// <summary>
        /// IIS获取后台服务错误
        /// </summary>
        public const uint IISSERVICEERR = ERR_SESSION_SERVICE + 5;

        /// <summary>
        /// WebService 认证SoapHeader为空
        /// </summary>
        public const uint GUI_WEBSERVICE_SOAP_HEADER_ISNULL = ERR_SESSION_SERVICE + 6;

        /// <summary>
        /// WebService 认证用户不存在
        /// </summary>
        public const uint GUI_WEBSERVICE_USER_NOT_EXIST = ERR_SESSION_SERVICE + 7;

        /// <summary>
        /// WebService 认证异常
        /// </summary>
        public const uint GUI_WEBSERVICE_EXCEPTION = ERR_SESSION_SERVICE + 8;

        /// <summary>
        /// 登录请求不存在
        /// </summary>
        public const uint API_LOGINREQUEST_NOEXIST_ERROR = ERR_SESSION_SERVICE + 9;

        /// <summary>
        /// 登录请求为空
        /// </summary>
        public const uint API_LOGINREQUEST_NULL_ERROR = ERR_SESSION_SERVICE + 0x0A;

        /// <summary>
        /// 第三方登录认证失败
        /// </summary>
        public const uint API_LONGINAUTH_FAIL = ERR_SESSION_SERVICE + 0x0B;

        /// <summary>
        /// 第三方认证SOAP Header为空
        /// </summary>
        public const uint API_SOAPHEADER_NULL_ERROR = ERR_SESSION_SERVICE + 0x0C;

        /// <summary>
        /// 会话为空
        /// </summary>
        public const uint SESSION_NULL_ERROR = ERR_SESSION_SERVICE + 0x0D;

        /// <summary>
        /// 会话超时
        /// </summary>
        public const uint SESSION_TIMEOUT_ERROR = ERR_SESSION_SERVICE + 0x0E;

        /// <summary>
        /// 导出多点话单失败。
        /// </summary>
        public const uint EXPORT_POINTS_CDR_ERROR = ERR_SESSION_SERVICE + 0x1C;

        /// <summary>
        /// 导出当前多点话单成功。
        /// </summary>
        public const uint EXPORT_POINTS_CDR_CURRENT_LOG = ERR_SESSION_SERVICE + 0x1D;

        /// <summary>
        /// 导出所有多点话单成功。
        /// </summary>
        public const uint EXPORT_POINTS_CDR_ALL_LOG = ERR_SESSION_SERVICE + 0x1E;

        /// <summary>
        /// 导出点对点话单失败。
        /// </summary>
        public const uint EXPORT_PTP_CDR_ERROR = ERR_SESSION_SERVICE + 0x1F;

        /// <summary>
        /// 导出当前点对点话单成功。
        /// </summary>
        public const uint EXPORT_PTP_CDR_CURRENT_LOG = ERR_SESSION_SERVICE + 0x20;

        /// <summary>
        /// 导出所有点对点话单成功。
        /// </summary>
        public const uint EXPORT_PTP_CDR_ALL_LOG = ERR_SESSION_SERVICE + 0x21;
        #endregion

        #region EConf错误码0x50A00000
        //////////////////////////////////////////////////////////////////////////
        // EConf服务的错误码0x50A00000
        //////////////////////////////////////////////////////////////////////////

        /// <summary>
        /// EConf公用错误码
        /// </summary>
        public const uint ERR_ECONF_COMMON_ERR = 0x50A00100;

        #region EConf公用错误码0x50A00100
        /// <summary>
        /// EConf会议参数错误
        /// </summary>
        public const uint ECONF_PARAM_ERR = ERR_ECONF_COMMON_ERR + 0x01;

        /// <summary>
        /// 桌面终端终端个数错误(0-999)
        /// </summary>
        public const uint ECONF_MAX_DESK_TOP_NUM = ERR_ECONF_COMMON_ERR + 0x02;

        /// <summary>
        /// EConf操作失败
        /// </summary>
        public const uint ECONF_OPERATE_FAIL = ERR_ECONF_COMMON_ERR + 0x03;

        /// <summary>
        /// EConf会议不存在
        /// </summary>
        public const uint ECONF_NOT_EXIST = ERR_ECONF_COMMON_ERR + 0x04;

        /// <summary>
        /// EConf创建会议失败
        /// </summary>
        public const uint ECONF_CREATE_CONFID_NULL = ERR_ECONF_COMMON_ERR + 0x05;

        /// <summary>
        /// EConf返回的Web连接为空
        /// </summary>
        public const uint ECONF_RETURN_WEBURI_NULL = ERR_ECONF_COMMON_ERR + 0x06;

        /// <summary>
        /// 数据会议不支持周期会议
        /// </summary>
        public const uint ECONF_DATA_NOTSUPPORT_PERIOD = ERR_ECONF_COMMON_ERR + 0x08;

        /// <summary>
        /// 数据会议不支持永久会议
        /// </summary>
        public const uint ECONF_DATA_NOTSUPPORT_FOREVER = ERR_ECONF_COMMON_ERR + 0x09;
        #endregion
        #endregion

        #region License错误码
        /// <summary>
        /// License错误码
        /// </summary>
        public const uint LICENSE_ERR = 0x78000000;

        /// <summary>
        /// License格式错误
        /// </summary>
        public const uint LICENSE_FILEFORMAT_ERR = LICENSE_ERR + 0x51;

        /// <summary>
        /// 加载系统License失败
        /// </summary>
        public const uint LICENSE_LOAD_ERR = LICENSE_ERR + 0x13C;

        /// <summary>
        /// 导入License失败
        /// </summary>
        public const uint LICENSE_UPLOAD_ERR = LICENSE_ERR + 0x13D;

        /// <summary>
        /// 系统License失效
        /// </summary>
        public const uint LICENSE_UNAVAILABLE_ERR = LICENSE_ERR + 0x13E;

        /// <summary>
        /// 导入License成功
        /// </summary>
        public const uint LICENSE_UPLOAD_SUCCESS = LICENSE_ERR + 0x140;
        #endregion

        /// <summary>
        /// 呼叫会场失败
        /// </summary>
        public const uint ERR_CALL_SITE = 0x06000000;

        /// <summary>
        /// 呼叫会场失败原因
        /// </summary>
        public const uint STR_ADDSITE_RESULT = 0x25030000;

        /// <summary>
        /// 召集会议结果
        /// </summary>
        public const uint STR_CONFSTART_RESULT = 0x25020000;

        /// <summary>
        /// 会控结果
        /// </summary>
        public const uint STR_CONFCTROL_RESULT = 0x25021100;

        #region HDX通信模块错误码0x60200000
        public const uint ERR_HDX_COM_NO = 0x60200000;

        /// <summary>
        ///  参数为空
        /// </summary>
        public const uint ERR_HDX_PARAM_NULL = ERR_HDX_COM_NO + 0x01;

        /// <summary>
        ///  该状态不能通讯
        /// </summary>
        public const uint ERR_HDX_ENABLE_COMMUNICATION = ERR_HDX_COM_NO + 0x02;

        /// <summary>
        ///  消息类型转化失败
        /// </summary>
        public const uint ERR_HDX_MSG_CHANGE = ERR_HDX_COM_NO + 0x03;

        /// <summary>
        ///  发送指令时获取连接错误
        /// </summary>
        public const uint ERR_HDX_GET_CONNECTION = ERR_HDX_COM_NO + 0x04;

        /// <summary>
        ///  发送指令失败
        /// </summary>
        public const uint ERR_HDX_SEND_COMMAND = ERR_HDX_COM_NO + 0x05;

        /// <summary>
        ///  解析呼叫列表失败
        /// </summary>
        public const uint ERR_HDX_PARSE_CALLINFO = ERR_HDX_COM_NO + 0x06;

        /// <summary>
        ///  加载XML指令配置文件错误
        /// </summary>
        public const uint ERR_HDX_LOAD_XML = ERR_HDX_COM_NO + 0x07;

        /// <summary>
        ///  未知的设备型号
        /// </summary>
        public const uint ERR_HDX_UNKNOWN_MODEL = ERR_HDX_COM_NO + 0x08;

        /// <summary>
        ///  未知的通讯指令 
        /// </summary>
        public const uint ERR_HDX_WRONG_COMMAND = ERR_HDX_COM_NO + 0x09;

        /// <summary>
        ///  获取配置文件呼叫协议适配关系错误
        /// </summary>
        public const uint ERR_HDX_WRONG_IP_PROTOCOL_RELATION = ERR_HDX_COM_NO + 0x0a;
        #endregion

        #region 设备管理错误码0x60500000

        public const uint ERR_DEVICE_COMMON = 0x60500000;

        /// <summary>
        /// 该设备已预发放
        /// </summary>
        public const uint ERR_DEVICE_PREDEFINED = ERR_DEVICE_COMMON + 0x01;

        /// <summary>
        /// 不可管理设备的名称不能与其他设备的H323Id重复
        /// </summary>
        public const uint ERR_DEVICE_UNMANAGED_NAME_EXIST_IN_H323ID = ERR_DEVICE_COMMON + 0x02;

        /// <summary>
        /// 该设备名称已经存在
        /// </summary>
        public const uint ERR_DEVICE_NAME_ALREADY_EXIST = ERR_DEVICE_COMMON + 0x03;

        /// <summary>
        /// 设备的H323Uri或SipUri已经存在该号码
        /// </summary>
        public const uint ERR_DEVICE_CODE_ALREADY_EXIST = ERR_DEVICE_COMMON + 0x04;

        /// <summary>
        /// 设备的H323Uri或SipUri已经存在该URI
        /// </summary>
        public const uint ERR_DEVICE_URI_ALREADY_EXIST = ERR_DEVICE_COMMON + 0x05;

        /// <summary>
        /// 设备的H323Uri或SipUri已经存在该IP地址
        /// </summary>
        public const uint ERR_DEVICE_IPADD_ALREADY_EXIST = ERR_DEVICE_COMMON + 0x06;

        /// <summary>
        /// 设备的H323Id已经存在
        /// </summary>
        public const uint ERR_DEVICE_H323ID_ALREADY_EXIST = ERR_DEVICE_COMMON + 0x07;

        /// <summary>
        /// 设备IP重复
        /// </summary>
        public const uint ERR_DEVICE_IPADDR_ALREADY_EXIST = ERR_DEVICE_COMMON + 0x08;

        /// <summary>
        /// 设备名称为空
        /// </summary>
        public const uint ERR_DEVICE_NAME_EMPTY = ERR_DEVICE_COMMON + 0x09;

        /// <summary>
        /// 设备IP为空
        /// </summary>
        public const uint ERR_DEVICE_PARAM_IP_EMPTY = ERR_DEVICE_COMMON + 0x0a;

        /// <summary>
        /// 设备连接密码为空
        /// </summary>
        public const uint ERR_DEVICE_PARAM_CONNECTIONPWD_EMPTY = ERR_DEVICE_COMMON + 0x0b;

        /// <summary>
        /// 设备GK前缀为空
        /// </summary>
        public const uint ERR_DEVICE_PARAM_GKPREFIX_EMPTY = ERR_DEVICE_COMMON + 0x0c;

        /// <summary>
        /// 设备IP地址不可用
        /// </summary>
        public const uint IPADDRESS_NOT_AVAILABLE = ERR_DEVICE_COMMON + 0x0D;

        /// <summary>
        /// 用户选择设备类型与实际设备类型不匹配
        /// </summary>
        public const uint DEVICE_TYPE_NOMATCH = ERR_DEVICE_COMMON + 0x0E;

        /// <summary>
        /// 控制台类型不匹配
        /// </summary>
        public const uint BOARD_TYPE_NOMATCH = ERR_DEVICE_COMMON + 0x0F;

        /// <summary>
        /// 设备已经被其他系统连接
        /// </summary>
        public const uint CONNECT_MAX_NUM = ERR_DEVICE_COMMON + 0x10;

        /// <summary>
        /// 密码认证失败
        /// </summary>
        public const uint DEVICE_AUTHER_ERROR = ERR_DEVICE_COMMON + 0x11;

        #endregion

        #region MCU TCP通讯系列错误码 0x60700000

        public const uint MCU_8650_ERR_BASE = 0x60700000;

        /// <summary>
        /// mcu封包错误
        /// </summary>
        public const uint MCU_AGENT_TAPKETS_ERR = MCU_8650_ERR_BASE + 0x01;

        /// <summary>
        /// 打开连接错误
        /// </summary>
        public const uint MCU_OPEN_CONNECT_ERR = MCU_8650_ERR_BASE + 0x02;

        /// <summary>
        /// 初始化X224协议
        /// </summary>
        public const uint MCU_INIT_X224_ERR = MCU_8650_ERR_BASE + 0x03;

        /// <summary>
        /// mcu编码错误
        /// </summary>
        public const uint MCU_ENCODE_ERR = MCU_8650_ERR_BASE + 0x04;

        /// <summary>
        /// outmemery
        /// </summary>
        public const uint MCU_OUT_MEMEERY = MCU_8650_ERR_BASE + 0x05;

        /// <summary>
        /// X.224激活
        /// </summary>
        public const uint X224_ACTIVE_CONNECTION = MCU_8650_ERR_BASE + 0x06;

        /// <summary>
        /// X.224协议错误
        /// </summary>
        public const uint X224_CONNECTION_ERR = MCU_8650_ERR_BASE + 0x07;

        /// <summary>
        /// ClassType 错误
        /// </summary>
        public const uint X224_CLASS_TYPE_ERR = MCU_8650_ERR_BASE + 0x08;

        /// <summary>
        /// ClassType 错误
        /// </summary>
        public const uint X224_NO_X224DATA = MCU_8650_ERR_BASE + 0x09;

        /// <summary>
        /// 解包出错
        /// </summary>
        public const uint TCP_PACKET_DECODE = MCU_8650_ERR_BASE + 0x0A;

        /// <summary>
        /// 添加X224包出错
        /// </summary>
        public const uint TCP_ADD_X224_DATA = MCU_8650_ERR_BASE + 0x0B;

        /// <summary>
        /// 添加X224包出错
        /// </summary>
        public const uint TCP_RE = MCU_8650_ERR_BASE + 0x0C;

        /// <summary>
        /// 接收数据包出错
        /// </summary>
        public const uint TCP_RECEIVE_DATA = MCU_8650_ERR_BASE + 0x0D;

        /// <summary>
        /// 握手错误
        /// </summary>
        public const uint TCP_DRIVER_SHARKHAND = MCU_8650_ERR_BASE + 0x0E;

        #endregion

        #region RM呼叫会场失败原因
        #region 8系列上报原因
        ///// <summary>
        ///// 被叫拒绝接听
        ///// </summary>
        //public const uint SERIES8_SITE_CALL_REJECTD_BY_CALLEE = 0x00000073;

        ///// <summary>
        ///// 因启用免打扰而自动拒绝接听
        ///// </summary>
        //public const uint SERIES8_REJECTD_THE_CALLE_FOR_NOT_DISTURB = 0x00030001;
        //#endregion

        //#region 9系列上报原因
        ///// <summary>
        ///// 被叫拒绝接听
        ///// </summary>
        //public const uint SERIES9_SITE_CALL_REJECTD_BY_CALLEE = 0x00000073;

        ///// <summary>
        ///// 因启用免打扰而自动拒绝接听
        ///// </summary>
        //public const uint SERIES9_REJECTD_THE_CALLE_FOR_NOT_DISTURB = 0x00030001;
        #endregion
        #endregion

        #region RM呼叫会场失败原因
        #region 8系列上报原因

        /// <summary>
        /// 会场离会
        /// </summary>
        //public const uint SITE_LEAVE_CONF = 0x06000001;
        /// <summary>
        /// 呼叫会场失败
        /// </summary>
        //public const uint ERR_CALL_SITE = 0x06000000;
        //以下8系列原因均需要重呼
        //0x06000081             	 /* 被叫无人接听*/
        //0x06020005             	 /* Q931超时*/
        //0x06020009              	/* 网络不同对端无响应*/
        //0x0602000f              	/* H.245呼叫超时*/
        //0x06020011             	 /* 能力交换超时*/
        //0x06020013              	/* 主从决定失败*/
        //0x06020014             	/*本端未打开逻辑通道*/ 
        //0x06020015            	/* 对端未打开逻辑通道*/
        //0x06020016             	/* 对端和本端均未打开逻辑通道*/
        //0x06020017             	/* 对端重复打开逻辑通道*/
        //0x06020018             	/* 打开逻辑通道超时*/
        //0x0602001b            	 /* 发送计费认证超时*/
        //0x06020028             	/* 对端通道能力不正确 */
        //0x06030004             	/* 回路时延时超时*/
        //0x06030005            	 /* 没有收到视音频码流*/
        //0x06000071             	/* IRR超时*/
        //0x06030009             	/* 计费超时*/
        //0x06030011            	 /* 4E1线路异常*/
        //0x06030012            	 /* MCU重启挂机*/
        //0x06030013            	 /* 线路环回而离会*/
        //0x06030014            	//线路失步
        //0x06FF0002             	/* 呼叫失败未知原因*/
        //0x06FF0003             	/* 互通后挂断,原因未知*/
        //0x06FF0004             	/*互通后被叫挂断,原因未知*/
        //0x06FF0006             	/*会场不支持非对称多画面会议*/
        //0x06040058             	/*呼叫业务等待恢复。*/

        /// <summary>
        /// 被叫无人接听
        /// </summary>
        public const uint LEAVECAUSE_CALLEE_NORESPONSE_SERIES8 = 0x06000081;
        /// <summary>
        /// Q931超时
        /// </summary>
        public const uint LEAVECAUSE_Q931_TIMEOUT_SERIES8 = 0x06020008;
        /// <summary>
        /// 网络不通对端无响应
        /// </summary>
        public const uint LEAVECAUSE_NET_DISCONNECT_SERIES8 = 0x06020009;
        /// <summary>
        /// H.245呼叫超时
        /// </summary>
        public const uint LEAVECAUSE_H245_TIMEOUT_SERIES8 = 0x0602000F;
        /// <summary>
        /// 能力交换超时
        /// </summary>
        public const uint LEAVECAUSE_EXCHANG_TIMEOUT_SERIES8 = 0x06020011;

        /// <summary>
        /// 主从决定失败
        /// </summary>
        public const uint LEAVECAUSE_MASTER_SLAVE_FAIL_SERIES8 = 0x06020013;

        /// <summary>
        /// 本端未打开逻辑通道
        /// </summary>
        public const uint LEAVECAUSE_LOCAL_CLOSE_CHANNEL_SERIES8 = 0x06020014;

        /// <summary>
        /// 对端未打开逻辑通道
        /// </summary>
        public const uint LEAVECAUSE_REMOTE_CLOSE_CHANNEL_SERIES8 = 0x06020015;

        /// <summary>
        /// 对端和本端均未打开逻辑通道
        /// </summary>
        public const uint LEAVECAUSE_ALL_CLOSE_CHANNEL_SERIES8 = 0x06020016;

        /// <summary>
        /// 对端重复打开逻辑通道
        /// </summary>
        public const uint LEAVECAUSE_REMOTE_TWICE_CHANNEL_SERIES8 = 0x06020017;

        /// <summary>
        /// 打开逻辑通道超时
        /// </summary>
        public const uint LEAVECAUSE_OPEN_CHANNEL_TIMEOUT_SERIES8 = 0x06020018;
        /// <summary>
        /// 发送计费认证超时
        /// </summary>
        public const uint LEAVECAUSE_SEND_BILL_TIMEOUT_SERIES8 = 0x0602001b;

        /// <summary>
        /// 对端能力不完整
        /// </summary>
        public const uint LEAVECAUSE_CHAN_CAP_FULL_SERIES8 = 0x06020027;

        /// <summary>
        /// 对端通道能力不正确
        /// </summary>
        public const uint LEAVECAUSE_CHAN_CAP_INCORRECT_SERIES8 = 0x06020028;

        /// <summary>
        /// 回路时延时超时
        /// </summary>
        public const uint LEAVECAUSE_RETURN_PATH_TIMEOUT_SERIES8 = 0x06030004;

        /// <summary>
        /// 没有收到视音频码流
        /// </summary>
        public const uint LEAVECAUSE_NOT_RECIEVE_STREAM_SERIES8 = 0x06030005;

        /// <summary>
        /// IRR超时
        /// </summary>
        public const uint LEAVECAUSE_IRRTIMEOUT_SERIES8 = 0x06000071;


        /// <summary>
        /// 计费超时
        /// </summary>
        public const uint LEAVECAUSE_BILL_TIMEOUT_SERIES8 = 0x06030009;

        /// <summary>
        /// 4E1线路异常
        /// </summary>
        public const uint LEAVECAUSE_4E1_EXCEPTION_SERIES8 = 0x06030011;

        /// <summary>
        /// MCU重启挂机
        /// </summary>
        public const uint LEAVECAUSE_MCU_REBOOT_SERIES8 = 0x06030012;


        /// <summary>
        /// 线路环回而离会
        /// </summary>
        public const uint LEAVECAUSE_TERMINAL_LOOPBACK_SERIES8 = 0x06030013;


        /// <summary>
        /// 线路失步
        /// </summary>
        public const uint LEAVECAUSE_8660_ASYN_SERIES8 = 0x06030014;

        /// <summary>
        /// 呼叫失败未知原因
        /// </summary>
        public const uint LEAVECAUSE_UNKNOWN_8660_CALL_FAIL_SERIES8 = 0x06FF0002;
        /// <summary>
        /// 互通后挂断,原因未知
        /// </summary>
        public const uint LEAVECAUSE_UNKNOWN_8660_DISCONNECT_SERIES8 = 0x06FF0003;

        /// <summary>
        /// 互通后被叫挂断,原因未知
        /// </summary>
        public const uint LEAVECAUSE_UNKNOWN_8660_CALLEE_HANGUP_SERIES8 = 0x06FF0004;

        /// <summary>
        /// 会场不支持非对称多画面会议
        /// </summary>
        public const uint LEAVECAUSE_CALLING_NOASYMMETRIC_CAP = 0x06FF0006;

        /// <summary>
        /// 呼叫业务等待恢复。
        /// </summary>
        public const uint LEAVECAUSE_CALLING_OPERATION_WATING_RECOVER = 0x06040058;







        /// <summary>
        /// 被叫拒绝接听
        /// </summary>
        public const uint SERIES8_SITE_CALL_REJECTD_BY_CALLEE = 0x06000073;

        /// <summary>
        /// 被叫正忙
        /// </summary>
        public const uint SERIES8_THE_CALLE_FOR_BUSY = 0x06000075;

        /// <summary>
        /// 主叫正常挂断
        /// </summary>
        public const uint SERIES8_THE_CALLE_FOR_TRUNOFF = 0x0003000F;

        /// <summary>
        /// 主叫正常挂断
        /// </summary>
        public const uint SERIES8_THE_CALLE_FOR_TRUNOFF1 = 0x0603000F;

        /// <summary>
        /// 因启用免打扰而自动拒绝接听
        /// </summary>
        public const uint SERIES8_REJECTD_THE_CALLE_FOR_NOT_DISTURB = 0x06030001;
        #endregion




        #region 9系列上报原因
        //以下9系列上报原因均需要重呼
        //        129	被叫无人接听 
        //0x00020008	Q931连接超时
        //0x00020009	对端没有响应TCP连接或未应答
        //0x0002000F	H245连接超时
        //0x00020011	能力交换超时
        //0x00020013	主从决定失败
        //0x00020014	单通：本端未打开逻辑通道
        //0x00020015	单通挂断：对端未打开逻辑通道
        //0x00020016	单通挂断：对端和本端均没有打开通道
        //0x00020017	对端重复打开逻辑通道
        //0x00020018	打开通道超时
        //0x0002001B	MCU向业务发送的计费认证超时
        //0x00020028	 对端通道能力不正确
        //0x00030004	回路时延超时
        //0x00030005	没收到视音频码流
        //113	IRR超时挂断
        //0x00030009	计费超时挂断
        //0x00030011	4E1线路异常
        //0x00030012	MCU正常重启挂机
        //0x00030013	线路环回
        //0x00030014	线路失步
        //0x00ff0002	呼叫失败(原因未知)
        //0x00ff0003	呼通后的断开(原因未知)
        //0x00ff0004	互通后被叫挂断(原因未知)
        /// <summary>
        /// 被叫无人接听
        /// </summary>
        public const uint LEAVECAUSE_CALLEE_NORESPONSE_SERIES9 = 0x00000081;
        /// <summary>
        /// Q931超时
        /// </summary>
        public const uint LEAVECAUSE_Q931_TIMEOUT_SERIES9 = 0x00020008;
        /// <summary>
        /// 网络不通对端无响应
        /// </summary>
        public const uint LEAVECAUSE_NET_DISCONNECT_SERIES9 = 0x00020009;
        /// <summary>
        /// H.245呼叫超时
        /// </summary>
        public const uint LEAVECAUSE_H245_TIMEOUT_SERIES9 = 0x0002000F;
        /// <summary>
        /// 能力交换超时
        /// </summary>
        public const uint LEAVECAUSE_EXCHANG_TIMEOUT_SERIES9 = 0x00020011;

        /// <summary>
        /// 主从决定失败
        /// </summary>
        public const uint LEAVECAUSE_MASTER_SLAVE_FAIL_SERIES9 = 0x00020013;

        /// <summary>
        /// 本端未打开逻辑通道
        /// </summary>
        public const uint LEAVECAUSE_LOCAL_CLOSE_CHANNEL_SERIES9 = 0x00020014;

        /// <summary>
        /// 对端未打开逻辑通道
        /// </summary>
        public const uint LEAVECAUSE_REMOTE_CLOSE_CHANNEL_SERIES9 = 0x00020015;

        /// <summary>
        /// 对端和本端均未打开逻辑通道
        /// </summary>
        public const uint LEAVECAUSE_ALL_CLOSE_CHANNEL_SERIES9 = 0x00020016;

        /// <summary>
        /// 对端重复打开逻辑通道
        /// </summary>
        public const uint LEAVECAUSE_REMOTE_TWICE_CHANNEL_SERIES9 = 0x00020017;

        /// <summary>
        /// 打开逻辑通道超时
        /// </summary>
        public const uint LEAVECAUSE_OPEN_CHANNEL_TIMEOUT_SERIES9 = 0x00020018;
        /// <summary>
        /// 发送计费认证超时
        /// </summary>
        public const uint LEAVECAUSE_SEND_BILL_TIMEOUT_SERIES9 = 0x0002001b;

        /// <summary>
        /// 对端通道能力不正确
        /// </summary>
        public const uint LEAVECAUSE_CHAN_CAP_INCORRECT_SERIES9 = 0x00020028;

        /// <summary>
        /// 对端能力不完整
        /// </summary>
        public const uint LEAVECAUSE_CHAN_CAP_FULL_SERIES9 = 0x00020027;

        /// <summary>
        /// 回路时延时超时
        /// </summary>
        public const uint LEAVECAUSE_RETURN_PATH_TIMEOUT_SERIES9 = 0x00030004;

        /// <summary>
        /// 没有收到视音频码流
        /// </summary>
        public const uint LEAVECAUSE_NOT_RECIEVE_STREAM_SERIES9 = 0x00030005;

        /// <summary>
        /// IRR超时
        /// </summary>
        public const uint LEAVECAUSE_IRRTIMEOUT_SERIES9 = 0x00000071;


        /// <summary>
        /// 计费超时
        /// </summary>
        public const uint LEAVECAUSE_BILL_TIMEOUT_SERIES9 = 0x00030009;

        /// <summary>
        /// 4E1线路异常
        /// </summary>
        public const uint LEAVECAUSE_4E1_EXCEPTION_SERIES9 = 0x00030011;

        /// <summary>
        /// MCU重启挂机
        /// </summary>
        public const uint LEAVECAUSE_MCU_REBOOT_SERIES9 = 0x00030012;


        /// <summary>
        /// 线路环回而离会
        /// </summary>
        public const uint LEAVECAUSE_TERMINAL_LOOPBACK_SERIES9 = 0x00030013;


        /// <summary>
        /// 线路失步
        /// </summary>
        public const uint LEAVECAUSE_8660_ASYN_SERIES9 = 0x00030014;

        /// <summary>
        /// 呼叫失败未知原因
        /// </summary>
        public const uint LEAVECAUSE_UNKNOWN_8660_CALL_FAIL_SERIES9 = 0x00FF0002;
        /// <summary>
        /// 互通后挂断,原因未知
        /// </summary>
        public const uint LEAVECAUSE_UNKNOWN_8660_DISCONNECT_SERIES9 = 0x00FF0003;

        /// <summary>
        /// 互通后被叫挂断,原因未知
        /// </summary>
        public const uint LEAVECAUSE_UNKNOWN_8660_CALLEE_HANGUP_SERIES9 = 0x00FF0004;



        /// <summary>
        /// 被叫拒绝接听
        /// </summary>
        public const uint SERIES9_SITE_CALL_REJECTD_BY_CALLEE = 0x00000073;

        /// <summary>
        /// 被叫正忙
        /// </summary>
        public const uint SERIES9_THE_CALLE_FOR_BUSY = 0x00000075;

        /// <summary>
        /// 主叫正常挂断
        /// </summary>
        public const uint SERIE9_THE_CALLE_FOR_TRUNOFF = 0x0003000F;

        /// <summary>
        /// 因启用免打扰而自动拒绝接听
        /// </summary>
        public const uint SERIES9_REJECTD_THE_CALLE_FOR_NOT_DISTURB = 0x00030001;
        #endregion

        #endregion

        /// <summary>
        /// MCU上删除会场失败原因
        /// </summary>
        public const uint DEL_SITE_FAILED = 0x25021118;
    }
}