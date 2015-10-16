using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;

namespace Cgw.SmcError
{
    /// <summary>
    /// IVS SDK错误码
    /// </summary>
    [DataContract]
    public class IvsSdkError : CgwError
    {
        /// <summary>
        /// 构造函数
        /// </summary>
        /// <param name="result">结果</param>
        public IvsSdkError(uint result)
            : base(result)
        {
        }

        /// <summary>
        /// 构造函数
        /// </summary>
        public IvsSdkError()
        {
        }

        /************************************************************
	    *   系统通用错误码定义
	    ***********************************************************/ 
	    public const uint IVS_SUCCEED                            = 0;           // 成功
        public const uint IVS_FAIL                               = 1;           // 失败
        public const uint IVS_PARA_INVALID                       = 2;           // 输入参数非法

	    /***********************************************************
	     *   系统启动、内存、数据库等通用错误码定义（模块编码：10）
	     ***********************************************************/ 
	    public const uint IVS_PASSWORD_ERROR                     = 109100000;   // 用户名密码错误，登录时输入的用户名或密码错误
        public const uint IVS_USER_NOT_EXIST                     = 109100001;   // 用户不存在，注册的用户ID已注销或不可用 
        public const uint IVS_NO_ENOUGH_RIGHT                    = 109100002;   // 权限不足，无此操作权限
        public const uint IVS_NOT_INIT                           = 109100003;   // 未初始化
        public const uint IVS_ALLOC_MEMORY_ERROR                 = 109100004;   // 分配内存失败
        public const uint IVS_OPERATE_MEMORY_ERROR               = 109100005;   // 操作内存失败
        public const uint IVS_NO_ENOUGH_BUF                      = 109100006;   // 缓冲区太小，接收数据的缓冲区或存放数据的缓冲区不足
        public const uint IVS_SYS_NO_SUPPORT                     = 109100007;   // 系统不支持
        public const uint IVS_INIT_DB_ERROR                      = 109100008;   // 初始化数据库失败
        public const uint IVS_CONNECT_DB_ERROR                   = 109100009;   // 连接数据库失败（用于获取数据库连接失败、数据库连接指针为NULL）
        public const uint IVS_OPERATE_DB_ERROR                   = 109100010;   // 数据库操作失败（用于各种数据库操作失败、数据库中内容不存在、数据库结果集指针为空等）
        public const uint IVS_DIR_ERROR                          = 109100011;   // 路径错误
        public const uint IVS_CREATE_DIR_ERROR                   = 109100012;   // 创建文件目录失败 	
        public const uint IVS_FILE_FORMAT_ERROR                  = 109100013;   // 文件格式不正确        
        public const uint IVS_CREATE_FILE_ERROR                  = 109100014;   // 创建文件失败
        public const uint IVS_OPEN_FILE_ERROR                    = 109100015;   // 打开文件失败
        public const uint IVS_WRITE_FILE_ERROR                   = 109100016;   // 写文件失败
        public const uint IVS_DISK_FULL                          = 109100017;   // 硬盘空间满 
        public const uint IVS_DISK_ERROR                         = 109100018;   // 硬盘出错
	    public const uint IVS_NET_CONNECT_ERROR                  = 109100019;   // 网络连接失败;服务器不在线或者网络原因引起连接超时等
        public const uint IVS_NET_OVER_MAXLINK                   = 109100020;   // 连接数超过最大限制
        public const uint IVS_NET_SEND_ERROR                     = 109100021;   // 网络发送数据失败
        public const uint IVS_NET_RECV_ERROR                     = 109100022;   // 网络接收数据失败
        public const uint IVS_NET_RECV_TIMEOUT                   = 109100023;   // 网络接收数据超时
        public const uint IVS_NET_DATA_ERROR                     = 109100024;   // 网络传送的数据有误
        public const uint IVS_JOIN_MULTICAST_ERROR               = 109100025;   // 加入多播组失败
        public const uint IVS_CREATE_THREAD_ERROR                = 109100026;   // 创建线程失败
        public const uint IVS_XML_INVALID                        = 109100027;   // XML消息体非法(用于XML消息体为NULL、XML解析失败)	
        public const uint IVS_XML_PACK_ERROR    		           = 109100028;   // XML组装失败
        public const uint IVS_XML_DEV_TEMPLATE_INVALID           = 109100029;   // 设备模板XML非法
        public const uint IVS_SECURITY_INIT_ERROR		           = 109100030;   // 安全操作类初始化失败
        public const uint IVS_BATCH_OPERATE_ALL_ERROR            = 109100031;   // 批量操作全部失败
        public const uint IVS_BATCH_OPERATE_PART_SUCCEED         = 109100032;   // 批量操作部分成功
        public const uint IVS_MU_OFF_LINE			               = 109100033;	  // MU不在线
        public const uint IVS_CMU_OFF_LINE	                   = 109100034;	  // CMU不在线
        public const uint IVS_DCG_OFF_LINE	                   = 109100035;	  // DCG不在线
        public const uint IVS_OMU_OFF_LINE	                   = 109100036;	  // OMU不在线
        public const uint IVS_SMU_OFF_LINE                       = 109100037;   // SMU不在线
	    public const uint IVS_SCU_OFF_LINE                       = 109100038;   // SCU不在线
        public const uint IVS_MULM_OFF_LINE	                   = 109100039;	  // MULM不在线 
        public const uint IVS_GET_LOCALIP_ERROR                  = 109100040;   // 获取本地IP失败       
        public const uint IVS_SET_LOCALIP_ERROR                  = 109100041;   // 设置本地IP失败
        public const uint IVS_SCRIPT_RUN_ERROR                   = 109100042;   // 脚本执行出错    
        // 参数校验类错误
        public const uint IVS_USERNAME_INVALID                   = 109100043;   // 用户名长度非法或包含非法字符
        public const uint IVS_PASSWORD_INVALID                   = 109100044;   // 用户密码长度非法或包含非法字符
        public const uint IVS_NAME_INVALID                       = 109100045;   // 名称长度非法或包含非法字符（用于设备名称，计划名称等）
        public const uint IVS_DESCRIBE_INVALID                   = 109100046;   // 描述长度非法或包含非法字符
        public const uint IVS_CODE_INVALID                       = 109100047;   // 编码长度非法或包含非法字符（用于设备编码，域编码等）
        public const uint IVS_IP_INVALID                         = 109100048;   // IP地址非法
        public const uint IVS_PORT_INVALID                       = 109100049;   // 端口非法
        public const uint IVS_TIME_INVALID                       = 109100050;   // 时间非法
        public const uint IVS_EMAIL_INVALID                      = 109100051;   // e-mail非法
        public const uint IVS_TELEPHONE_INVALID                  = 109100052;   // 电话非法
        public const uint IVS_DATA_INVALID                       = 109100053;   // 数值非法或超过取值范围
	    public const uint IVS_MAU_OFF_LINE                       = 109100054;   // MAU不在线
	    public const uint IVS_URL_INVALID                        = 109100055;   // URL非法
	    public const uint IVS_DOWNLOAD_ERROR                     = 109100056;   // 下载失败
	
	    /**************************************************************
	     *   SMU错误码定义（模块编码：11）
	     *************************************************************/ 
	    public const uint IVS_SMU_XML_INVALID                    = 119108001;   // XML数据不合法
        public const uint IVS_SMU_DEVICE_TIMEOUT                 = 119108002;   // 处理设备管理业务超时
	    public const uint IVS_SMU_GETURL_TIMEOUT                 = 119108003;   // 获取geturl超时
	    public const uint IVS_SMU_RECORDINQUIRY_TIMEOUT          = 119108004;   // 录像查询超时
        public const uint IVS_SMU_DEVICE_EXIST                   = 119108005;   // 设备已存在
	    public const uint IVS_SMU_DEVICE_NOTEXIST				   = 119108006;   // 设备不存在
	    public const uint IVS_SMU_DEVICE_UPDATEMAP_FAIL		   = 119108007;	  // 更新内存失败
        public const uint IVS_SMU_DEVICE_ADDMAP_FAIL             = 119108008;   // 添加内存数据失败
	    public const uint IVS_SMU_PTZ_CONTROL_TIMEOUT            = 119108009;   // 云镜控制超时
	    public const uint IVS_SMU_ROUTE_NVR_FAIL                 = 119108010;   // 路由nvr失败
        public const uint IVS_DB_RESULT_SET_NULL_POINTER		   = 119108011;   // 数据库结果集指针为空
        public const uint IVS_SMU_CMD_XML_IS_NULL                = 119108012;   // cmd中的xml指针为空
	    public const uint IVS_SMU_DR_RELATON_TIMEOUT             = 119108013;   // 容灾备份操作超时
        public const uint IVS_SMU_OPERATE_TIME_OUT               = 119108014;   // 操作超时
	    public const uint IVS_SMU_MD_DATA_TIME_OUT               = 119108015;   // 查询移动侦测数据超时
        public const uint IVS_SMU_LOAD_SQL_ERROR                 = 119108016;   //SMU加载数据库语句失败
	
        /* SMU:设备管理-【主设备+通用】错误码：119108XXX */
        public const uint IVS_SMU_DEV_TIMEOUT                    = 119108100;   // 处理设备管理业务超时
        public const uint IVS_SMU_DEV_CMD_INVALID                = 119108101;   // CMD不可用
        public const uint IVS_SMU_DEV_REQ_XML_INVALID            = 119108102;   // 所请求的XML非法
        public const uint IVS_SMU_DEV_REQ_XML_GROUPID_NOEXIST    = 119108103;   // 所请求的XML报文GroupId节点不存在
        public const uint IVS_SMU_DEV_REQ_XML_GROUPNAME_NOEXIST  = 119108104;   // 所请求的XML报文GroupName节点不存在
        public const uint IVS_SMU_DEV_DB_CONNECT_ERROR           = 119108105;   // 数据库连接错误
        public const uint IVS_SMU_DEV_DB_EXECUTE_ERROR           = 119108106;   // 数据库操作失败
        public const uint IVS_SMU_DEV_GROUPID_NOEXIST            = 119108107;   // 设备组ID不存在
        public const uint IVS_SMU_DEV_SAME_GROUPNAME_EXIST       = 119108108;   // 父设备组下存在同名子设置组
        public const uint IVS_SMU_DEV_ADD_ALL_FAIL               = 119108109;   // 添加主设备全部失败
        public const uint IVS_SMU_DEV_ADD_PART_SUCCEED           = 119108110;   // 添加主设备部分成功
	    public const uint IVS_SMU_DEV_TYPES_CHANGE_ERROR		   = 119108111;   // 设备类型强制转换错误
        public const uint IVS_SMU_DEV_XML_CHANNELNAME_NOEXIST    = 119108112;   // 所请求的XML报文ChannelName节点不存在
        public const uint IVS_SMU_DEV_XML_CHANNELTYPE_NOEXIST    = 119108113;   // 所请求的XML报文ChannelType节点不存在
        public const uint IVS_SMU_DEV_XML_CHANNELCODE_NOEXIST    = 119108114;   // 所请求的XML报文ChannelCode节点不存在
        public const uint IVS_SMU_DEV_CREATE_SUBDEVICE_FAIL      = 119108115;   // 创建子设备失败
        public const uint IVS_SMU_DEV_GET_ROUTING_ERROR          = 119108116;   // 获取NVR路由失败
        public const uint IVS_SMU_DEV_REQ_XML_TARGROUPID_NOEXIST = 119108117;   // 所请求的XML报文TargetGroup节点不存在
        public const uint IVS_SMU_DEV_DB_GROUPID_NOEXIST         = 119108118;   // 数据库GroupId无记录
        public const uint IVS_SMU_DEV_GROUP_LEVEL_EXCEED         = 119108119;   // Group Level超出最大值
	    public const uint IVS_SMU_DEV_ADD_TO_GROUP_ERROR		   = 119108120;	  // 添加设备组下设备失败
        public const uint IVS_SMU_DEV_NOEXIST                    = 119108121;   // 设备不存在
	    public const uint IVS_SMU_DEV_FROM_GRP_ID_ZERO		   = 119108122;	  // 父设备组已经为根组
	    public const uint IVS_SMU_DEV_PROTOCOL_TYPE_ERROR		   = 119108123;	  // 主设备协议类型错误
	    public const uint IVS_SMU_DEV_NVRCODE_CANNOT_MOD		   = 119108124;	  // 不可修改设备的nvrcode
        public const uint IVS_SMU_DEV_XML_LOGINID_NOEXIST        = 119108125;   // 所请求的XML报文LoginId节点不存在
	    public const uint IVS_SMU_DEV_CONNOT_DEL				   = 119108126;   // 该设备只在根组下，不能移除
        public const uint IVS_SMU_DEV_DELETE_FAIL                = 119108127;   // 删除设备失败
        public const uint IVS_SMU_DEV_NVR_ROUTING_OFFLINE        = 119108128;   // NVR不在线
        public const uint IVS_SMU_DEV_GROUP_TARGET_SAME_FAIL     = 119108129;   // 与目标设备组相同
        public const uint IVS_SMU_DEV_NVR_NOT_EXIST			   = 119108130;	  // nvrcode不存在	
        public const uint IVS_SMU_DEV_GROUP_TOTAL_EXCEED         = 119108131;   // 设备组总数达到最大值
        public const uint IVS_SMU_DEV_GROUP_CHILE_EXCEED         = 119108132;   // 子设备组达到最大值
        public const uint IVS_SMU_DEV_GROUP_EXIST_RECU           = 119108133;   // 设备组存在递归
        public const uint IVS_SMU_DEV_GROUPNAME_LENGTH_INVALID   = 119108134;   // 设备组长度无效
        public const uint IVS_SMU_DEV_GROUPNAME_STRING_INVALID   = 119108135;   // 设备组名称包含非法字符
        public const uint IVS_SMU_DEV_XML_PARENRGROUPID_NOEXIST  = 119108136;   // 所请求的PARENRGROUPID节点不存在
        public const uint IVS_SMU_DEV_GROUPREMARK_LENGTH_INVALID = 119108137;   // 设备组长度无效
        public const uint IVS_SMU_DEV_GROUPREMARK_STRING_INVALID = 119108138;   // 设备组名称包含非法字符
        public const uint IVS_SMU_DEV_BUILD_DEVICE_CODE_FAIL     = 119108139;   // 生成设备编码失败
	    public const uint IVS_SMU_DEV_PART_NOEXIST               = 119108140;   // 设备列表中部分设备不存在
        public const uint IVS_SMU_DEV_DR_TABLE_IS_EMPTY          = 119108141;   // 容灾关系表为空

        /* SMU:设备管理-【摄像机子设备】错误码：119109XXX */


        /* SMU:设备管理-【摄像机子设备】错误码：119109XXX */

	    /* SMU:用户登录Service错误码，取值范围：119108300 ~ 119108399 */ 
        public const uint IVS_SMU_USER_EXPIRED         	       = 119101302;   // 账号已经过期
        public const uint IVS_SMU_USER_STOPPED         	       = 119101303;   // 用户被停用
        public const uint IVS_SMU_USER_LOCKED          	       = 119101304;   // 用户被锁定
        public const uint IVS_SMU_USER_PWD_NEED_MOD     	       = 119101305;   // 用户密码已过期，需要修改
        public const uint IVS_SMU_EXCEED_SAME_USER_ONLINE_MAXNUM = 119101306;   // 超过同一账户同时在线的最大用户个数
        public const uint IVS_SMU_EXCEED_ONLINE_USER_MAXNUM      = 119101307;   // 同时在线总用户数超过系统支持的最大数量
        public const uint IVS_SMU_USER_FIRST_LOGIN     	       = 119101308;   // 用户首次登录需要修改密码
        public const uint IVS_SMU_USER_OFF_LINE        	       = 119101309;   // 用户已离线
        public const uint IVS_SMU_BASIC_LOGIN_NOT_NEED_DOMAIN_NAME = 119101310; // 基本身份登录不能带域名信息
        public const uint IVS_SMU_DOMAIN_LOGIN_NEED_DOMAIN_NAME  = 119101311;   // 域身份登录必须带域名信息
        public const uint IVS_SMU_NO_MATCH_ACC_RULE              = 119101312;   // 没找到账户匹配的角色规则信息
        public const uint IVS_SMU_NO_MATCH_OPER_RIGHT            = 119101313;   // 没找到账户匹配的操作权限信息
        public const uint IVS_SMU_LOGIN_WITH_EXIST_LINKID        = 119101314;   // 不能使用旧的TCP连接重复发送登录请求
        public const uint IVS_SMU_VERIFY_WITH_DIFFERENT_LINKID   = 119101315;   // 认证请求所使用的TCP连接与在线用户的TCP连接不同    
        public const uint IVS_SMU_PWD_IS_EMPTY                   = 119101316;   // 密码为空
        public const uint IVS_SMU_CU_IP_IS_ILLEGAL               = 119101317;   // 登录Xml的CU IP非法
        public const uint IVS_SMU_CU_PORT_IS_ILLEGAL             = 119101318;   // 登录Xml的CU端口非法
        public const uint IVS_SMU_LOGIN_TYPE_ILLEGAL             = 119101319;   // Login_Type非法
        public const uint IVS_SMU_LDAP_INIT_FAIL                 = 119101320;   // 域账户验证时，LDAP协议初始化失败
        public const uint IVS_SMU_LDAP_SEARCH_ACCOUNT_FAIL       = 119101321;   // 在域服务器上查找账户失败
        public const uint IVS_SMU_MACHINE_NAME_ILLEGAL           = 119101322;   // 机器名非法
        public const uint IVS_SMU_CLIENT_TYPE_ILLEGAL            = 119101323;   // 客户端类型非法
        public const uint IVS_SMU_SERVER_IP_IS_ILLEGAL           = 119101324;   // 登录Xml的服务器IP非法
        public const uint IVS_SMU_SERVER_PORT_IS_ILLEGAL         = 119101325;   // 登录Xml的服务器端口非法
        public const uint IVS_SMU_DOMAIN_LOGIN_SERVER_DISCONNECT = 119101326;   // 域账号服务器不在工作
        public const uint IVS_SMU_CAN_NOT_SIGNOUT_USER_SELF      = 119101327;   // 不能签退自己
        public const uint IVS_SMU_RIGHT_LEN_NO_MATCH             = 119101328;   // 权限长度不匹配
        public const uint IVS_SMU_ENCRYPT_FAIL                   = 119101329;   // 加密失败
        public const uint IVS_SMU_DECRYPT_FAIL                   = 119101331;   // 解密失败
        public const uint IVS_SMU_REQUEST_PUBLIC_KEY_FAIL        = 119101332;   // 请求公钥失败
        public const uint IVS_SMU_REQUEST_EXCHANGE_KEY_FAIL      = 119101333;   // 交换密钥失败
        public const uint IVS_SMU_SECURITY_CLIENT_INIT_FAIL      = 119101334;   // 安全Client初始化失败
        public const uint IVS_SMU_WAIT_ASYNC_RESPONSE            = 119101340;   // 等待异步响应
	    public const uint IVS_NULL_POINTER			 	       = 119101350;   // 服务器内部错误，空指针异常;
        public const uint IVS_SMU_LINKID_FORMAT_ERROR            = 119101351;   // LinkID格式错误
        public const uint IVS_SMU_IP_LOCKED          	           = 119101352;   // 用户IP被锁定
        public const uint IVS_SMU_HAS_RESTARED        	       = 119101353;   // SMU已重启，原在线用户已不存在，用于sdk重连smu  

        public const uint IVS_SMU_ADD_GROUP_ERROR                = 119105000;   // 新增用户组失败
        public const uint IVS_SMU_MODIFY_GROUP_ERROR             = 119105001;   // 修改用户组失败
        public const uint IVS_SMU_DELETE_GROUP_ERROR             = 119105002;   // 删除用户组失败
        public const uint IVS_SMU_MOVE_GROUP_ERROR               = 119105003;   // 移动用户组失败
        public const uint IVS_SMU_ADD_GROUP_HAVE_REPEAT_GROUP    = 119105004;   // 增加用户组时重复
        public const uint IVS_SMU_ADD_GROUP_PARENT_NOT_EXIST     = 119105005;   // 父节点不存在
        public const uint IVS_SMU_ADD_GROUP_PARENT_LEVEL_INVALID = 119105006;   // 父节点层级非法
        public const uint IVS_SMU_MOIDFY_GROUP_GROUP_NOT_EXIST   = 119105007;   // 被修改的用户组不存在
        public const uint IVS_SMU_MOIDFY_GROUP_HAVE_REPEAT_GROUP = 119105008;   // 修改用户组后重命名
        public const uint IVS_SMU_MOVE_GROUP_TARGET_NOT_EXIST    = 119105009;   // 移向的目标不存在
        public const uint IVS_SMU_MOVE_GROUP_TARGET_LEVEL_INVALID= 119105010;   // 移动后会超过3级
        public const uint IVS_SMU_MOVE_GROUP_BEMOVED_NOT_EXIST   = 119105011;   // 被移动的用户组不存在
        public const uint IVS_SMU_MOVE_GROUP_HAVE_REPEAT_GROUP   = 119105012;   // 移动后用重命名用户组
        public const uint IVS_SMU_DEL_GROUP_GROUP_NOT_EXIST      = 119105013;   // 要被删除的用户组不存在
        public const uint IVS_SMU_CAN_NOT_DELETE_ROOT_GROUP      = 119105016;   // 不能删除根组
        public const uint IVS_SMU_OVER_MAX_GROUP_NUM             = 119105017;   // 用户组个数超过最大限额
        public const uint IVS_SMU_CAN_NOT_MOVE_ROOT_GROUP        = 119105018;   // 根组不能移动
        public const uint IVS_SMU_CAN_NOT_MOVE_TO_SELF           = 119105019;   // 组不能移动到自己
        public const uint IVS_SMU_GROUP_NUM_OVER_MAX             = 119105020;   // 用户组个数达到上限，超过1000
        public const uint IVS_SMU_GROUP_CHILDREN_NUM_OVER_MAX    = 119105021;   // 子用户组个数超过99
        public const uint IVS_SMU_PARAM_INVALID_FOR_GROUP        = 119105022;   // 用户组参数错误
        public const uint IVS_SMU_RESET_PWD_SWITCH_OFF           = 119105023;   // 重置密码开关关闭

        public const uint IVS_SMU_ADD_ROLE_ERROR                 = 119105100;   // 新增角色失败
        public const uint IVS_SMU_MODIFY_ROLE_ERROR              = 119105101;   // 修改角色失败
        public const uint IVS_SMU_DELETE_ROLE_ERROR              = 119105102;   // 删除角色失败
        public const uint IVS_SMU_GET_ROLE_LIST_ERROR            = 119105103;   // 查询角色失败
        public const uint IVS_SMU_ACC_PARAM_ERROR                = 119105104;   // 帐号规则参数非法
        public const uint IVS_SMU_ADD_ROLE_HAVE_REPEAT_ROLE      = 119105105;   // 角色重命名
        public const uint IVS_SMU_DELETE_ROLE_ROLE_NOT_EXIST     = 119105106;   // 删除的角色不存在
        public const uint IVS_SMU_DELETE_DEFAULT_ROLE_ERROR      = 119105107;   // 不能删除默认角色
        public const uint IVS_SMU_MODIFY_ROLE_ROLE_NOT_EXIST     = 119105108;   // 修改的角色不存在
        public const uint IVS_SMU_GET_DEFAULT_RULE_ERROR         = 119105109;   // 获取默认帐号规则失败
        public const uint IVS_SMU_GET_ROLE_BY_ID_ERROR           = 119105110;   // 根据id获取角色失败
        public const uint IVS_SMU_GET_ROLE_BY_ID_ROLE_NOT_EXIST  = 119105111;   // 角色id对应的角色不存在
        public const uint IVS_SMU_GET_USER_ROLE_USER_NOT_HAVE_ROLE= 119105112;  // 用户没有分配角色
        public const uint IVS_SMU_GET_USER_ROLE_USER_NOT_EXIST   = 119105113;   // 用户没有分配角色
        public const uint IVS_SMU_GET_USER_ROLE_OP_ERROR         = 119105114;   // 获取角色操作权限失败
	    public const uint IVS_SMU_FILE_OP_ERROR                  = 119105116;   // 上传下载文件失败
        public const uint IVS_SMU_FILE_OVER_MAX                  = 119105117;   // 文件大小太大，超过20m
        public const uint IVS_SMU_FILE_BUILD_CMD_ERROR           = 119105118;   // 拼装cmd时错误
        public const uint IVS_SMU_PARAM_INVALID_FOR_ROLE         = 119105119;   // 角色参数错误
        public const uint IVS_SMU_ROLE_NUM_OVER_MAX              = 119105120;   // 角色个数达到上限，超过128

         /* SMU:用户管理错误码 */ 
        public const uint IVS_SMU_MODIFY_PWD_OLD_PWD_WRONG       = 119105200;   // 修改密码时旧密码错误
	    public const uint IVS_SMU_ADD_USER_ERROR                 = 119105201;   // 增加用户错误
	    public const uint IVS_SMU_DELETE_USER_ERROR              = 119105202;   // 删除用户错误
	    public const uint IVS_SMU_MODIFY_USER_ERROR              = 119105203;   // 修改用户错误
	    public const uint IVS_SMU_INQUIRY_USER_ERROR             = 119105204;   // 查询用户错误
	    public const uint IVS_SMU_INQUIR_ONLINE_USER_ERROR       = 119105205;   // 查询在线用户错误
	    public const uint IVS_SMU_INQUIR_DEVLIST_ERROR           = 119105206;   // 查询用户设备错误
	    public const uint IVS_SMU_USER_NOT_EXIST                 = 119105207;   // 用户不存在
        public const uint IVS_SMU_USER_EXIST                     = 119105208;   // 用户已存在
	    public const uint IVS_SMU_EXIST_SAME_ITEM                = 119105211;   // 数据库已存在相同的内容
        public const uint IVS_SMU_DB_NOT_EXIST_ITEM              = 119105212;   // 数据库不存在内容
        public const uint IVS_SMU_FROM_GREATERTHAN_INDEX_ERROR   = 119105214;   // 用户查询FROM索引大于TO索引
        public const uint IVS_SMU_XML_UNIFIEDQUERY_LIMIT_ERROR   = 119105215;   // 统一查询参数中的limit不正确
        public const uint IVS_SMU_XML_UNIFIEDQUERY_INDEX_ERROR   = 119105216;   // 统一查询参数中的Fromindex、Toindex不正确
        public const uint IVS_SMU_USER_ID_INVALID                = 119105218;   // 数据库ID不合法
        public const uint IVS_SMU_ID_GENERATE_ERROR              = 119105222;   // 数据库id生成错误
        public const uint IVS_SMU_ADD_USER_ROLE_RELATION_ERROR   = 119105223;   // 添加用户角色关联表错误
        public const uint IVS_SMU_ADD_USER_GROUP_RELATION_ERROR  = 119105224;   // 添加用户用户组关联表错误
        public const uint IVS_SMU_ROLE_NOT_EXIST                 = 119105225;   // 角色表中角色不存在
        public const uint IVS_SMU_GROUP_NOT_EXIST                = 119105226;   // 用户组不存在
        public const uint IVS_SMU_DEV_EXIST_SAME_ITEM            = 119105227;   // 设备已存在
        public const uint IVS_SMU_DELETE_USER_USERGROUP_ERROR    = 119105228;   // 删除用户用户组表错误
        public const uint IVS_SMU_DELETE_USER_ONLINE_ERROR       = 119105229;   // 删除在线用户错误
        public const uint IVS_SMU_DELETE_USER_ROLE_ERROR         = 119105230;   // 删除用户角色对应表错误
        public const uint IVS_SMU_DELETE_USER_DEVICE_ERROR       = 119105231;   // 删除用户设备对应表错误
        public const uint IVS_SMU_DELETE_USER_INFO_ERROR         = 119105232;   // 删除用户基本表错误
        public const uint IVS_SMU_MODIFY_USER_ROLE_ERROR         = 119105234;   // 修改用户角色对应表错误
        public const uint IVS_SMU_MODIFY_USER_USERGROUP_ERROR    = 119105235;   // 修改用户用户组表错误
        public const uint IVS_SMU_MODIFY_USER_USEINFO_ERROR      = 119105236;   // 用户修改基本信息表错误
        public const uint IVS_SMU_MODIFY_ADMIN_USEINFO_ERROR     = 119105237;   // 管理员修改基本信息表错误
        public const uint IVS_SMU_POINTER_IS_NULL                = 119105238;   // 指针为空
        public const uint IVS_SMU_GET_TIMEZONE_ERROR             = 119105239;   // 获取时区失败
        public const uint IVS_SMU_ADD_SBUSCRIBE_INFO_ERROR       = 119105240;   // 增加订阅告警信息错误
        public const uint IVS_SMU_PAGEINFO_INVALID               = 119105241;   // 分页信息不合法
        public const uint IVS_SMU_USER_TYPE_INVALID              = 119105246;   // 用户类型不合法
        public const uint IVS_SMU_DB_ID_INVALID                  = 119105247;   // 数据库id不合法
        public const uint IVS_SMU_USER_REGINSTER_DATE_INVALID    = 119105249;   // 用户注册时间不合法
        public const uint IVS_SMU_USER_STATUS_INVALID            = 119105250;   // 用户状态不合法
        public const uint IVS_SMU_USER_SEX_INVALID               = 119105253;   // 用户SEX不合法
        public const uint IVS_SMU_USER_ROLEID_INVALID            = 119105254;   // 用户角色ID不合法
        public const uint IVS_SMU_USER_GROUPID_INVALID           = 119105255;   // 用户组ID不合法
        public const uint IVS_SMU_GET_USER_ID_NAME_ERROR         = 119105256;   // 获取用户id和登录名错误
        public const uint IVS_SMU_SELECT_COUNT_ERROR             = 119105257;   // 获取查询总量错误
        public const uint IVS_SMU_OVER_MAX_USER_NUM              = 119105258;   // 超过用户最大限
        public const uint IVS_SMU_SEARCH_CODE_INVALID            = 119105259;   // 搜索条件信息不合法
        public const uint IVS_SMU_STATEMENT_FILE_OPEN_ERROR      = 119105262;   // 声明文件打开失败(文件不存在)
        public const uint IVS_SMU_STATEMENT_FILE_TOO_LARGE       = 119105263;   // 声明文件太大
        public const uint IVS_SMU_STATEMENT_FILE_INVALID         = 119105264;   // 声明文件不合法
        public const uint IVS_SMU_READ_STATEMENT_ERROR           = 119105265;   // 读取声明文件失败
        public const uint IVS_SMU_PARSE_STATEMENT_FILE_TOO_LARGE = 119105266;   // 解析声明文件失败
        public const uint IVS_SMU_DELETE_USERSELF_ERROR          = 119105267;   // 用户删除用户本身失败(自己删除自己失败)
        public const uint IVS_SMU_PARAMETER_INVALID              = 119105268;   // 分页信息中的用户参数不合法
        public const uint IVS_SMU_USERGROUP_NOT_EXIST            = 119105270;   // 用户组不存在
        public const uint IVS_SMU_USER_MODIFY_INFO_INVALID       = 119105271;   // 用户修改信息不合法
        public const uint IVS_SMU_ADD_USER_SUBSCRIBEINFO_ERROR   = 119105272;   // 增加用户订阅表信息失败
        public const uint IVS_SMU_DB_EXIST_SAME_ITEM             = 119105273;   // 数据库存在相同记录
        public const uint IVS_SMU_DELETE_SUBSCRIBE_LEVEL_ERROR   = 119105274;   // 删除订阅级别失败
        public const uint IVS_SMU_DELETE_SUBSCRIBE_TYPE_ERROR    = 119105275;   // 删除订阅类型失败
        public const uint IVS_SMU_SELF_LOCK_ERROR                = 119105276;   // 自己锁定自己不允许
        public const uint IVS_SMU_SELF_DELETE_ERROR              = 119105277;   // 自己删除自己不允许
        public const uint IVS_SMU_DEV_GROUP_DOMAIN_CODE_TOO_LONG = 119105284;   // 设备组域编码太长
        public const uint IVS_SMU_DELETE_SUPER_INVALID           = 119105285;   // 删除super不允许
        public const uint IVS_SMU_RESET_SUPER_INVALID            = 119105286;   // 重置super密码不允许
        public const uint IVS_SMU_ADD_USER_DEV_ERROR             = 119105287;   // 增加用户设备失败
        public const uint IVS_SMU_DEV_NOT_EXIST                  = 119105288;   // 设备不存在
        public const uint IVS_SMU_PCRE_PATTERN_TOO_LONG          = 119105289;   // PCRE规则太长
        public const uint IVS_SMU_PCRE_TEXT_TOO_LONG             = 119105290;   // 需要PCRE判断的文本太长
        public const uint IVS_SMU_PCRE_COMPILE_ERROR             = 119105291;   // PCRE编译出错
        public const uint IVS_SMU_PCRE_NOT_COMPELETLY_MATCH      = 119105292;   // PCRE规则不完全匹配
        public const uint IVS_SMU_PCRE_NOT_MATCH_OR_ERROR        = 119105293;   // PCRE完全不匹配或者出错
        public const uint IVS_SMU_PCRE_NOT_MATCH                 = 119105294;   // PCRE规则完全不匹配
        public const uint IVS_SMU_PCRE_ERROR                     = 119105295;   // PCRE执行出错
        public const uint IVS_SMU_PCRE_TEXT_IS_NULL              = 119105296;   // 需要PCRE判断的文本为空
        public const uint IVS_SMU_READ_UNICODE_TXT_ERROR         = 119105297;   // 读Unicode文件失败
        public const uint IVS_SMU_SEARCH_CONDITION_TOO_LONG      = 119105298;   // 搜索条件太长
        public const uint IVS_SMU_XML_NVR_CODE_INVALID           = 119105299;   // 设备编码错误
        public const uint IVS_SMU_PWD_PCRE_INVALID               = 119105300;   // 密码不符合PCRE规则
        public const uint IVS_SMU_PWD_PCRE_NO_CAPTIAL            = 119105301;   // 密码中没有包含大写
        public const uint IVS_SMU_PWD_PCRE_NO_LOWERCASE          = 119105302;   // 密码中没有包含小写
        public const uint IVS_SMU_PWD_PCRE_NO_FIGURE             = 119105303;   // 密码中没有包含数字
        public const uint IVS_SMU_PWD_PCRE_NO_SPECIAL_CHAR       = 119105304;   // 密码中没有包含特殊字符
        public const uint IVS_SMU_IS_TOTAL_GROUP_INVALID         = 119105305;   // IS_TOTAL_GROUP标记不合法
        public const uint IVS_SMU_GENERATE_PWD_ERROR             = 119105306;   // 生成密码失败
        public const uint IVS_SMU_NEW_OLD_PWD_DIFF_NUM_INVALID   = 119105307;   // 新旧密码不同字符数目不合法
        public const uint IVS_SMU_USER_PWD_RESET_ERROR           = 119105308;   // 重置密码错误
        public const uint IVS_SMU_PWD_REVERSE_ERROR              = 119105313;   // 密码反转失败
        public const uint IVS_SMU_PWD_LOGNINAME_REVERSE_SAME     = 119105314;   // 密码与登录名反转相同
        public const uint IVS_SMU_PWD_LOGNINAME_SAME             = 119105315;   // 密码与登录名相同
        public const uint IVS_SMU_OLD_PWD_DIFFERENT              = 119105316;   // 旧密码与真实密码不一致
        public const uint IVS_SMU_DECRYPT_PWD_ERROR              = 119105317;   // 用户密码解密失败
        public const uint IVS_SMU_ENCRYPT_PWD_ERROR              = 119105318;   // 用户密码加密失败
        public const uint IVS_SMU_DOMAIN_USER_MODIFY_PWD_ERROR   = 119105319;   // 域用户不可修改密码
        public const uint IVS_SMU_USER_MODIFY_LOGINNAME_INVALID  = 119105322;   // 用户登录名不能修改
        public const uint IVS_SMU_USER_MODIFY_DOMAINNAME_INVALID = 119105323;   // 用户域名不能修改
        public const uint IVS_SMU_USER_ID_TOO_LONG               = 119105333;   // ID长度不合法
        public const uint IVS_SMU_GROUP_ID_NOT_NUMBER            = 119105334;   // 组ID不是有效数字
        public const uint IVS_SMU_GROUP_ID_TOO_LONG              = 119105335;   // 组ID太长
        public const uint IVS_SMU_USER_ROLE_ID_NOT_NUMBER        = 119105336;   // 角色ID不是有效数字
        public const uint IVS_SMU_ROLE_ID_TOO_LONG               = 119105337;   // 角色ID太长
        public const uint IVS_SMU_DELETE_USER_SUBINFO_ERROR      = 119105345;   // 删除订阅信息错误
        public const uint IVS_SMU_INQUIRY_USER_GROUP_LIST_ERROR  = 119105346;   // 获取用户组列表失败
        public const uint IVS_SMU_NO_LANGUAGE_EXIST              = 119105347;   // 系统不支持该语言 
        public const uint IVS_SMU_STATEMENT_FILE_FSEEK_ERROR     = 119105348;   // 声明文件fseek操作失败
    

        /* SMU:告警管理错误码，取值范围：119120000~119120999 */ 
        public const uint IVS_SMU_DB_HAD_SAME_ALARM_LEVEL        = 119120000;    // 数据库中已经存在相同的告警级别了
        public const uint IVS_SMU_DB_MAX_ALARM_LEVEL_NUM         = 119120001;    // 数据库中告警级别的个数已经达到最大值
        public const uint IVS_SMU_DB_ALARM_NO_DATA               = 119120002;    // 在数据库中没有查到匹配查询条件的数据
        public const uint IVS_SMU_DB_HAD_SAME_ALARM_HELP         = 119120003;    // 数据库中已经存在相同的告警帮助了
        public const uint IVS_SMU_ALARM_DB_NO_AREA               = 119120004;    // 数据库中不存在指定的防区
        public const uint IVS_SMU_ALARM_AREA_HAS_GUARD_PLAN      = 119120005;    // 指定的防区已经有防区计划了
        public const uint IVS_SMU_ALARM_DB_NO_ALARM_LEVEL        = 119120006;    // 数据库中不存在指定的告警级别
	    public const uint IVS_SMU_ALARM_SUB_ALARM_TYPE_ERROR     = 119120007;    // 按告警类型订阅，告警类型不可为空
	    public const uint IVS_SMU_ALARM_SUB_ALARM_LEVEL_ERROR    = 119120008;    // 按告警级别订阅，告警级别大小错误
	    public const uint IVS_SMU_ALARM_SUB_OVER_MAX             = 119120009;    // 告警订阅请求数，超过1000
        public const uint IVS_SMU_ALARM_DB_NO_ALARM_TYPE         = 119120010;    // 在数据库中没有指定的告警类型
	    public const uint IVS_SMU_ALARM_CONFIRMED                = 119120011;    // 告警已确认
	    public const uint IVS_SMU_ALARM_REVOKED                  = 119120012;    // 告警已撤销
	    public const uint IVS_SMU_ALARM_RECOVERY                 = 119120013;    // 告警已恢复
	    public const uint IVS_SMU_MSGBLOCK_INVALID			   = 119120014;    // 请求CMD不合法
	    public const uint IVS_SMU_DEV_CODE_NOT_FOUND			   = 119120015;    // 设备已删除或不存在
	    public const uint IVS_SMU_ALARM_EVENT_ID_NOT_EXIST       = 119120016;    // 告警事件id不存在
        public const uint IVS_SMU_DB_HAD_SAME_ALARM_TYPE         = 119120017;    // 数据库中已经存在相同的告警类型了
	    public const uint IVS_SMU_DB_HAD_SAME_ALARM_TYPE_NAME    = 119120018;    // 数据库中已经存在相同的告警类型名称了
	    public const uint IVS_SMU_ALL_USERS_OF_ALARM_COMMISSION_ILLEGAL = 119120019; //告警授权用户全部不合法
	    public const uint IVS_SMU_ALARM_AREA_NOTEXIST			   = 119120020;    // 设备不存在
	    public const uint IVS_SMU_ALARM_AREA_GUARD_OVER_MAX      = 119120021;    // 防区超过了最大数(1000)
	    public const uint IVS_SMU_ALARM_AREA_GUARD_DEV_OVER_MAX  = 119120022;    // 防区设备超过了最大数(128)

	    /* SMU:录像模块错误码，取值范围：119125000 ~ 119125999 */
	    public const uint IVS_SMU_CAMERA_CODE_NOT_EXIST          = 119125001;    // 摄像机编码不存在
	    public const uint IVS_SMU_LINKID_NOT_EXIST               = 119125002;    // TCP连接不存在
	    public const uint IVS_SMU_RECORD_TIMEOUT                 = 119125003;    // 录像模块超时
	    public const uint IVS_SMU_RECORD_LABEL_TIMEOUT           = 119125004;    // 录像书签超时
	    public const uint IVS_SMU_RECORD_LOCK_TIMEOUT            = 119125005;    // 录像锁定模块超时
        public const uint IVS_SMU_MEDIA_SAFE_TIMEOUT             = 119125006;    // 媒体安全超时
        public const uint IVS_SMU_BACK_RECORD_PLAN_TIMEOUT       = 119125007;    // 备份录像计划超时
        public const uint IVS_SMU_GET_CLUSTER_RECORD_PLAN_FAIL   = 119125008;    // 集群下查询录像计划失败
        public const uint IVS_SMU_GET_RECORD_TASK_FAIL           = 119125009;    // 查询录像任务失败
        public const uint IVS_SMU_GET_CLUSTER_RECORD_POLICY_FAIL = 119125010;    // 集群下查询录像策略失败
        public const uint IVS_SMU_GET_CLUSTER_RECORD_STATE_FAIL  = 119125011;    // 集群下查询平台录像状态失败
        public const uint IVS_SMU_CLUSTER_START_RECORD_FAIL      = 119125012;    // 集群下手动启动录像失败
        public const uint IVS_SMU_CLUSTER_STOP_RECORD_FAIL       = 119125013;    // 集群下手动停止录像失败
        public const uint IVS_SMU_GET_BACK_MBU_TIMEOUT           = 119125014;    // 查询备份服务器信息超时
        public const uint IVS_SMU_GET_PRERECORD_CAMERA_LIST_TIMEOUT = 119125015; // 查询开启预录的摄像机列表超时
        public const uint IVS_SMU_GET_PRERECORD_CAMERA_LIST_FAIL = 119125016;    // 查询开启预录的摄像机列表全部失败
        public const uint IVS_SMU_GET_CAMERA_BACKUP_CFG_FAIL     = 119125017;    // 集群下查询摄像机备份参数失败
        public const uint IVS_SMU_BACKUP_SETTING_TIMEOUT         = 119125018;    // 备份录像参数超时
        public const uint IVS_SMU_GET_BACKUP_PLAN_FAIL           = 119125019;    // 集群下查询备份录像计划失败
        public const uint IVS_SMU_GET_MBU_CHANGE_HISTORY_FAIL    = 119125020;    // 集群下查询备份服务器变更记录失败
        public const uint IVS_SMU_TRANSPARENT_NOT_RUN            = 119125021;    // 透明通道没有启用
        public const uint IVS_SMU_GET_PRERECORD_CAMERA_LIST_PART_FAIL= 119125022;// 查询开启预录的摄像机列表部分失败
        public const uint IVS_SMU_GET_RECORD_TASK_PART_FAIL      = 119125023;    // 查询录像任务失败
        public const uint IVS_SMU_GET_RECORD_LIST_FAIL           = 119125024;    // 查询录像列表失败

        /* SMU:集群模块错误码，取值范围：119126000 ~ 119126999 */
        public const uint IVS_SMU_NVR_LIST_EMPTY                 = 119126000;    // NVR列表为空
        public const uint IVS_SMU_NVR_SERVER_CNT_IS_ZERO         = 119126001;    // server NVR个数为0
	    public const uint IVS_SMU_NOT_FOUND_LEADER               = 119126002;    // 没找到Leader
        public const uint IVS_SMU_GET_MEMBER_STATUS_TIME_OUT     = 119126003;    // 获取成员状态超时
        public const uint IVS_SMU_CREATE_CLUSTERING              = 119126004;    // 正在组建集群
        public const uint IVS_SMU_DISSOLVE_CLUSTERING            = 119126005;    // 正在解散集群
        public const uint IVS_SMU_CLUSTER_SYNCING                = 119126006;    // 正在集群同步，不允许集群操作
        public const uint IVS_SMU_CLEAR_BLACK_LIST_IS_NULL       = 119126007;    // ClearBlackList结点为空
        public const uint IVS_SMU_CREATE_CLUSTER_TIME_OUT        = 119126008;    // 组建集群超时
        public const uint IVS_SMU_NVR_LIST_NODE_NOT_EXIST        = 119126009;    // NvrList结点不存在
        public const uint IVS_SMU_NVR_INFO_NODE_NOT_EXIST        = 119126010;    // NvrInfo结点不存在
        public const uint IVS_SMU_NVR_CODE_OR_IP_INVALID         = 119126011;    // NvrCode或IP结点不合法
        public const uint IVS_SMU_NVR_ROLE_INVALID               = 119126012;    // Nvr角色结点不合法
        public const uint IVS_SMU_NVR_INFO_CONTENT_INVALID       = 119126013;    // NvrInfo结点内容不合法
        public const uint IVS_SMU_NODE_MAP_IS_EMPTY              = 119126014;    // NVR列表为空
        public const uint IVS_SMU_FIND_NVR_TRANS_ID_NULL         = 119126015;    // 查询NVR时事务ID为空
        public const uint IVS_SMU_FIND_NVR_CAMERA_LIST_EMPTY     = 119126016;    // 查询NVR时摄像机列表为空
        public const uint IVS_SMU_CLUSTER_OFFLINE                = 119126017;    // 集群不在线
        public const uint IVS_SMU_CLUSTER_ROLE_NOT_CLIENT        = 119126018;    // 角色不为Client
        public const uint IVS_SMU_CLUSTER_SIZE_IS_MIN            = 119126019;    // 集群个数已经是最小
        public const uint IVS_SMU_NVR_CODE_NOT_EXIST             = 119126020;    // 该NVR不存在
        public const uint IVS_SMU_SEND_CLUSTER_FAIL              = 119126021;    // 下发集群失败

        /* SMU:多域模块错误码，取值范围：119127000 ~ 119127999 */
        public const uint IVS_SMU_GET_CONFIG_PATH_ERROR          = 119127000;    // 获取配置路径失败
        public const uint IVS_SMU_LOAD_NET_XML_ERROR             = 119127001;    // 加载net.xml失败
        public const uint IVS_SMU_NET_ELEMENTS_NODE_NOT_EXIST    = 119127002;    // NetElement结点不存在
        public const uint IVS_SMU_SAVE_NET_XML_ERROR             = 119127003;    // 保存net.xml失败
        public const uint IVS_SMU_LOCAL_IP_IS_NULL               = 119127004;    // 本机信令IP为空
        public const uint IVS_SMU_MEDIA_IP_IS_NULL               = 119127005;    // 本机媒体IP为空

	    /* SMU:日志管理错误码，取值范围：119130000 ~ 119130999 */
	    public const uint IVS_SMU_SET_LOG_LEVEL_ERROR            = 119130000;    // 设置日志级别失败

	    public const uint IVS_SMU_NO_PCG_NODE_ERROR              = 119139000;    // 无PCG连接信息

	    /******************************************************************
	     *   SCU错误码定义（模块编码：12）
	     *****************************************************************/ 
        public const uint IVS_SCU_ALARM_XML_NODE_ILLEGAL         = 129123001;    // XML节点非法
	    public const uint IVS_SCU_LINKAGE_DEV_SIZE_ILLEGAL       = 129123002;    // 联动设备的个数非法(不能超过4个)
	    public const uint IVS_SCU_LINKAGE_POLICY_ID_NOT_EXIST    = 129123003;    // 联动策略ID不存在
	    public const uint IVS_SCU_LINKAGE_POLICY_ID_EXIST        = 129123004;    // 联动策略ID已经存在
	    public const uint IVS_SCU_LINKAGE_POLICY_EXIST           = 129123005;    // 联动策略已经存在
	    public const uint IVS_SCU_LINKAGE_POLICY_COUNT_OVER      = 129123006;    // 告警联动策略超过了最大数(500000)
	    public const uint IVS_SCU_LINKAGE_ACTION_ILLEGAL         = 129123007;    // 告警联动动作不能重复
        public const uint IVS_SCU_SEND_ALARM_CONFIRM_SMU_FAIL    = 129123008;    // 发送告警确认请求到SMU失败 
	    public const uint IVS_SCU_XML_DECODE_FAIL                = 129123009;    // XML解析失败
	    public const uint IVS_SCU_LINKAGE_DEV_COUNT_ERROR        = 129123010;    // 联动设备个数超过上限
	    public const uint IVS_SCU_LINKAGE_DEV_NOT_EXIST          = 129123011;    // 联动目标设备不存在或者已删除
	    public const uint IVS_SCU_LINKAGE_ACTION_NOT_EXIST       = 129123012;    // 联动动作不存在
	
	    public const uint IVS_SCU_ONLINE_USER_EXPIRE             = 129105001;    // 登录信息已失效，请重新登录
	
	    public const uint IVS_SCU_LOAD_SQL_ERROR                 = 129108000;    // 加载SQL语句失败
	    public const uint IVS_SCU_DEV_OFFLINE			           = 129108001;	   // 设备不在线
	    public const uint IVS_SCU_DEV_NOT_EXIST			       = 129108002;	   // 设备不存在或者已删除
	    public const uint IVS_SCU_DEV_INSERT_ERROR		       = 129108003;	   // 设备入库失败
	    public const uint IVS_SCU_DEV_MODULE_ERROR		       = 129108004;	   // 模板校验失败
	    public const uint IVS_SCU_DEV_PROTOCALTYPE_ERROR	       = 129108005;	   // 设备协议类型校验失败
	    public const uint IVS_SCU_DEV_VENTORTYPE_ERROR	       = 129108006;	   // 供应商类型校验失败
	    public const uint IVS_SCU_DEV_FORMTYPE_ERROR	           = 129108007;	   // 产品形态类型校验失败
	    public const uint IVS_SCU_DEV_MODELTYPE_ERROR	           = 129108008;	   // 设备型号类型校验失败
	    public const uint IVS_SCU_DEV_ALARMENABLE_ERROR	       = 129108009;	   // 告警使能校验失败
	    public const uint IVS_SCU_DEV_VIDEOFORMAT_ERROR	       = 129108010;	   // 视频制式校验失败
	    public const uint IVS_SCU_DEV_REGPASSWORD_ERROR	       = 129108011;	   // 注册密码为空
	    public const uint IVS_SCU_DEV_MAXDIRCONNECTNUM_ERROR	   = 129108012;	   // 最大直连数失败
	    public const uint IVS_SCU_DEV_NVRCODE_ERROR	           = 129108013;	   // NVR为不合法
	    public const uint IVS_SCU_DEV_DEVICECODE_ERROR	       = 129108014;	   // 设备编码不合法
	    public const uint IVS_SCU_DEV_DEVICENAME_ERROR	       = 129108015;	   // 设备名称不合法
	    public const uint IVS_SCU_XML_INVALID                    = 129108016;    // XML数据不合法
	    public const uint IVS_SCU_DCG_RESPONSE_FAIL              = 129108018;    // DCG响应失败	 
	    public const uint IVS_SCU_MAIN_DEV_XML_NODE_ILLEGAL      = 129108019;    // 主设备XML节点不合法
	    public const uint IVS_SCU_MAIN_DEV_GURAD_SET_FAIL        = 129108021;    // 主设备布撤防失败
	    public const uint IVS_SCU_DEV_NOT_IN_WHITE_LIST          = 129108022;    // 设备不属于当前NVR，可能已移除
        public const uint IVS_SCU_DEV_TEMPLATE_XML_INVALID       = 129108023;    // 设备模板XML非法
        public const uint IVS_SCU_VIDEO_STREAM_NUM_ERROR         = 129108024;    // 视频流数不正确
	    public const uint IVS_SCU_SENDTODCG_FAIL                 = 129108026;    // 下发DCG失败
	    public const uint IVS_SCU_ADD_AUDIO_PARA_FAIL            = 129108027;    // 增加音频参数失败
	    public const uint IVS_SCU_XMLBODY_IS_NULL                = 129108028;    // xm消息体为空
	    public const uint IVS_SCU_GET_LOGIN_ID_FAIL              = 129108029;    // 获取用户ID失败
	    public const uint IVS_SCU_MAINDEV_TO_XML_FAIL            = 129108030;    // 主设备转换为xml失败
        public const uint IVS_SCU_DEV_TEMPLATE_NONEXISTENT       = 129108031;    // 设备模板不存在
        public const uint IVS_SCU_DEV_IS_ONLINE    	           = 129108032;	   // 设备已在线
        public const uint IVS_SCU_SUB_DEV_INFO_NOT_EXIST         = 129108033;    // 子设备信息不存在
	    public const uint IVS_SCU_ADD_SUBDEVICE_FAIL             = 129108035;    // 添加子设备失败失败
	    public const uint IVS_SCU_SEND_MSG_TO_SMU_FAIL           = 129108036;    // 发送smu响应失败
	    public const uint IVS_SCU_ADD_ENVR_SUBDEVICE_FAIL        = 129108037;    // 添加ENVR设备失败
        public const uint IVS_SCU_ENVR_DEV_NOT_EXIST			   = 129108038;	   // eNVR设备不存在或者已删除
	    public const uint IVS_SCU_MAIN_DEV_NOT_EXIST			   = 129108039;	   // 主设备不存在或者已删除
        public const uint IVS_SCU_DEV_OFFLINE_CFG_NOT_EXIST	   = 129108040;	   // 设备离线配置不存在
	    public const uint IVS_SCU_DEV_TYPE_IS_NULL        	   = 129108041;	   // 设备设备形态不合法
        public const uint IVS_SCU_DEV_OSD_PARA_NOT_EXIST	       = 129108042;	   // 设备OSD参数不存在
	    public const uint IVS_SCU_DEL_ENVR_DEVICE_FAIL           = 129108043;    // 删除ENVR设备失败
	    public const uint IVS_SCU_SEL_SHARE_DEVICE_INVALID       = 129108044;    // 查询共享设备参数不合法
	    public const uint IVS_SCU_MODEL_TEMPLATE_IS_NULL         = 129108045;    // 设备模板数量为空
        public const uint IVS_SCU_DR_MAIN_DEV_NOT_EXIST          = 129108046;    // 容灾设备信息不存在
	    public const uint IVS_SCU_DEV_UPDATE_TEMPATE_MAIN_DEV    = 129108047;    // 设备模板更新主设备信息失败
        public const uint IVS_SCU_PRE_RECORD_DEV_NOT_EXIST       = 129108048;    // 开启预录设备不存在
        public const uint IVS_SCU_PU_NOT_SUPPORT_GET_PARAM       = 129108049;    // 前端不支持前端参数获取
	    public const uint IVS_SCU_MOD_ENVR_SUBDEVICE_FAIL        = 129108050;    // 修改ENVR设备失败
	    public const uint IVS_SCU_MOD_T28181_LENGTH_INVALID      = 129108051;    // T28181长度不合法
	    public const uint IVS_SCU_MAIN_DEV_IP_INVALID            = 129108052;    // 主设备Ip不合法
	    public const uint IVS_SCU_LOGIN_TYPE_INVALID             = 129108053;    // 认证类型不合法
	    public const uint IVS_SCU_BLANCE_SUPPORT_INVALID         = 129108054;    // 是否支持集群参数不合法
	    public const uint IVS_SCU_DIRECTCONNECT_FIRST_INVALID    = 129108055;    // 是否支持直连优先参数不合法
	    public const uint IVS_SCU_DECRYPT_PASSWORD_INVALID       = 129108056;    // smu发送密码加密失败
	    public const uint IVS_SCU_DECRYPT_REGPASSWORD_INVALID    = 129108057;    // smu发送注册密码加密失败
	    public const uint IVS_SCU_PASSWORD_LENGTH_INVALID        = 129108058;    // 密码解密长度超过规格
	    public const uint IVS_SCU_DECRYPT_XML_BODY_FAILED        = 129108059;    // xml解密失败
	    public const uint IVS_SCU_PARSE_CAMERA_TYPE_FAILED       = 129108060;    // 摄像机类型转换失败
	    public const uint IVS_SCU_MAIN_DEV_SEQUENCE_TYPE_INVALID = 129108061;    // 主设备序列号不合法
	    public const uint IVS_SCU_TAS_CODE_INVALID               = 129108062;    // TASCode不合法
	    public const uint IVS_SCU_DEV_SN_INVALID                 = 129108063;    // DevSN不合法
	    public const uint IVS_SCU_DEV_REG_PASSWORD_INVALID       = 129108064;    // 注册密码不合法
	    public const uint IVS_SCU_DEV_IP_TYPE_INVALID            = 129108065;    // 设备Ip类型不合法

        public const uint IVS_SCU_CAMERA_XML_NODE_ILLEGAL        = 129109001;    // 摄像机XML节点不合法
        public const uint IVS_SCU_CAMERA_DB_ERROR                = 129109002;    // 数据库错误
        public const uint IVS_SCU_CAMERA_NOT_EXIST               = 129109003;    // 摄像机子设备不存在或者已删除
	    public const uint IVS_SCU_MOTION_DETECTION_NOT_EXIST     = 129109004;    // 移动侦测信息不存在
	    public const uint IVS_SCU_VIDEO_HIDE_NOT_EXIST           = 129109005;    // 图像遮挡信息不存在
	    public const uint IVS_SCU_VIDEO_MASK_NOT_EXIST           = 129109006;    // 隐私保护信息不存在
	    public const uint IVS_SCU_CONF_PTZ_DB_ERROR              = 129109007;    // 配置云台参数成功但数据保存到数据库失败
	    public const uint IVS_SCU_BROADCAST_IP_PORT_EXIST        = 129109008;    // 组播IP端口已经存在
	
        public const uint IVS_SCU_CONF_VOICE_CHAN_TIMEOUT        = 129112001;    // 配置语音参数超时
	    public const uint IVS_SCU_CONF_VOICE_CHAN_DB_ERROR       = 129112002;    // 配置语音参数成功但数据保存到数据库失败
	
        public const uint IVS_SCU_GETURL_ERROR 			       = 129115001;	   // 获取媒体URL失败
        public const uint IVS_SCU_GETURL_TIMEOUT 		           = 129115002;	   // 获取媒体URL会话超时
        public const uint IVS_SCU_GETURL_SESSION_EXIST 	       = 129115003;	   // 获取媒体URL的请求已存在
	
	    public const uint IVS_SCU_PTZ_LOCKED                     = 129116001;    // 云台已锁定
	    public const uint IVS_SCU_PTZ_CONTROLED                  = 129116002;    // 云台被同优先级或者高优先级用户控制
	    public const uint IVS_SCU_PTZ_LEVEL_LOW_TO_UNLOCK        = 129116003;    // 云台被同优先级或者高优先级用户锁定
	    public const uint IVS_SCU_PTZ_LOCKED_BY_ALARM            = 129116004;    // 云台被告警联动中，不允许操作
	    public const uint IVS_SCU_PTZ_NOT_LOCKED                 = 129116005;    // 云台未锁定，或者已自动解锁
	    public const uint IVS_SCU_PTZ_GET_USER_INFO_FAIL         = 129116006;    // 获取用户云台信息失败
	    public const uint IVS_SCU_PTZ_MODE_SET_RUN               = 129116007;    // 同优先级或者高优先级用户正在录制PTZ轨迹
	    public const uint IVS_SCU_PTZ_LOCKE_NOT_SUPPORT          = 129116008;    // 外域不支持云台锁定操作
	    public const uint IVS_SCU_PTZ_MODE_SET_NOT_SUPPORT       = 129116009;    // 外域不支持模式录制操作
	    public const uint IVS_SCU_PTZ_MODE_CRUISE_NOT_EXIST      = 129116010;    // 模式轨迹索引不存在
	
	    public const uint IVS_SCU_PRESET_NUM_EXCEED_MAX          = 129117001;    // 预置位已经达到最大值
	    public const uint IVS_SCU_PRESET_NUM_MAY_EXCEED_MAX      = 129117002;    // 其他用户也在添加预置位，可能超过最大个数，请稍候
	    public const uint IVS_SCU_PRESET_IN_CRUISE_TRACK         = 129117003;    // 预置位在巡航轨迹中，无法删除
	    public const uint IVS_SCU_PRESET_NOT_EXIST               = 129117004;    // 预置位不存在
	    public const uint IVS_SCU_PRESET_NAME_REPEAT             = 129117005;    // 该摄像机已经存在相同的预置位名称
	    public const uint IVS_SCU_PRESET_IN_CRUISE_PLAN          = 129117006;    // 预置位在巡航计划中，不能删除
	    public const uint IVS_SCU_PRESET_IS_GUARD_POS            = 129117007;    // 预置位被设置成看守位，不能删除
	
	    public const uint IVS_SCU_GUARD_POS_NOT_EXIST            = 129117101;    // 看守位不存在
	
	    public const uint IVS_SCU_TRACK_NUM_EXCEED_MAX           = 129118001;    // 巡航轨迹个数已经达到最大值
	    public const uint IVS_SCU_TRACK_NUM_MAY_EXCEED_MAX       = 129118002;    // 其他用户也在添加巡航轨迹，可能超过最大个数，请稍候
	    public const uint IVS_SCU_TRACK_POINT_NUM_EXCEED_MAX     = 129118003;    // 轨迹中的预置位个数不能超过20个
	    public const uint IVS_SCU_TRACK_POINT_INVALID            = 129118004;    // 轨迹中的预置位不存在或者已经删除
        public const uint IVS_SCU_TRACK_NOT_EXIST                = 129118005;    // 巡航轨迹不存在
	    public const uint IVS_SCU_TRACK_NAME_REPEAT              = 129118006;    // 该摄像机已经存在相同的巡航轨迹名称
	    public const uint IVS_SCU_GUARD_PLAN_NOT_EXIST           = 129118007;    // 巡航计划不存在
	    public const uint IVS_SCU_TRACK_IS_RUNNING               = 129118008;    // 该巡航轨迹正在执行
	    public const uint IVS_SCU_TRACK_IS_IN_PLAN               = 129118009;    // 该轨迹在计划中，无法删除
	    public const uint IVS_SCU_MODE_TRACK_NUM_EXCEED_MAX      = 129118010;    // 模式巡航轨迹个数已经达到最大值
	    public const uint IVS_SCU_MODE_TRACK_NUM_MAY_EXCEED_MAX  = 129118011;    // 其他用户也在添加模式巡航轨迹，可能超过最大个数，请稍候
	
	    public const uint IVS_SCU_NO_NVR_NOT_EXIST               = 129124001;    // 存储信息不存在
        public const uint IVS_SCU_DEV_RECORD_PARA_NOT_EXIST      = 129124002;    // 设备录像参数信息不存在
	    public const uint IVS_SCU_STORAGE_NOT_ALLOC              = 129124003;    // 存储空间未分配
	    public const uint IVS_SCU_STORAGE_CANT_LESSEN            = 129124004;    // 存储空间不能缩小，只能扩大
	    public const uint IVS_SCU_STORAGE_IS_SCARCE              = 129124005;    // 存储空间不足	 
        public const uint IVS_SCU_CAMERA_BACKUP_CFG_NOT_EXIST    = 129124006;    // 摄像机备份参数记录不存在
        public const uint IVS_SCU_CAMERA_BACKUP_PLAN_NOT_EXIST   = 129124007;    // 摄像机备份计划不存在
        public const uint IVS_SCU_NEW_MBU_ADD_BACKUP_PLAN_FAIL   = 129124008;    // 向新MBU迁移备份计划失败
        public const uint IVS_SCU_RECORD_MBU_CONTRAIL_NOT_EXIST  = 129124009;    // MBU轨迹不存在
        public const uint IVS_SCU_BACKUP_RECORD_REGION_NOT_EXIST = 129124010;    // MBU录像备份记录的起止范围不存在

        /*容灾备份错误码 begin*/
        public const uint IVS_SCU_DEVICE_STRATEGY_MAP_NOT_EXIST  = 129124011;    // 设备录像策略映射表记录不存在
        public const uint IVS_SCU_NVR_HAS_THIS_CAMERA            = 129124012;    // NVR存在该摄像机
        public const uint IVS_SCU_OTHER_EXDOMAIN_HAS_THIS_CAMERA = 129124013;    // 其他外域已经在该NVR配置了该摄像机
        public const uint IVS_SCU_SEND_MSG_TO_MU_FAIL            = 129124014;    // 下发消息给MU失败
        public const uint IVS_SCU_CAMERA_IS_NOT_PR               = 129124016;    // 该设备不是容灾设备
        public const uint IVS_SCU_PR_CAMERA_IS_NOT_HAVE_PALN     = 129124017;    // 未制订录像策略或计划
        public const uint IVS_SCU_PR_CAMERA_CAN_NOT_MOD_RECORD   = 129124018;    // 已切换容的灾设备不能修改录像策略
        /*容灾备份错误码 end*/

        public const uint IVS_SCU_PU_MANUAL_RECORD_EXIST         = 129124015;    // 前端手动录像已存在

        public const uint IVS_SCU_RECORD_PLAN_NOT_EXIST          = 129125001;    // 录像计划不存在
        public const uint IVS_SCU_WAIT_DCG_RSP                   = 129125002;    // 等待DCG响应	

	    public const uint IVS_SCU_MU_OFF_LINE			           = 129134001;	   // MU不在线
	    public const uint IVS_SCU_CMU_OFF_LINE			       = 129134002;	   // CMU不在线
	    public const uint IVS_SCU_DCG_OFF_LINE				   = 129134003;	   // DCG不在线
        public const uint IVS_SCU_OMU_OFF_LINE				   = 129134004;	   // OMU不在线
	    public const uint IVS_SCU_SMU_OFF_LINE                   = 129134005;    // SMU不在线
        public const uint IVS_SCU_MULM_OFF_LINE			       = 129134006;	   // MULM不在线
	
	    /* SCU:集群模式错误码，取值范围：129134101 ~ 129134200 */
	    public const uint IVS_SCU_NOT_CLUSTER_MODE               = 129134101;    // 模式错误，非集群模式
	    public const uint IVS_SCU_REDIRECTION_ERROR              = 129134102;    // 错误，请重定向（集群下设备在黑名单时，需重定向）
	    public const uint IVS_SCU_SCRIPT_RUN_FAIL                = 129134103;    // 脚本执行出错
	    public const uint IVS_SCU_CLUSTER_SIZE_INVALID           = 129134104;    // 集群成员大小不合法
	
        /* SCU:配置模块错误码，取值范围：129127000 ~ 129127049 */
        public const uint IVS_SCU_GET_CONFIG_PATH_ERROR          = 129127000;    // 获取配置路径失败
        public const uint IVS_SCU_LOAD_NET_XML_ERROR             = 129127001;    // 加载net.xml失败
        public const uint IVS_SCU_NET_ELEMENTS_NODE_NOT_EXIST    = 129127002;    // NetElement结点不存在
        public const uint IVS_SCU_SAVE_NET_XML_ERROR             = 129127003;    // 保存net.xml失败
        public const uint IVS_SCU_LOCAL_IP_IS_NULL               = 129127004;    // 本机信令IP为空
        public const uint IVS_SCU_MEDIA_IP_IS_NULL               = 129127005;    // 本机媒体IP为空
        public const uint IVS_SCU_SET_LOCAL_IP_FAIL              = 129127006;    // 设置本机IP失败
	    public const uint IVS_SCU_SET_LOG_PARA_FAIL              = 129127007;    // 统一配置日志参数xml失败

        /* SCU:业务跟踪错误码，取值范围：129127050 ~ 129127100*/
        public const uint IVS_SCU_TRACE_NO_FIND                  = 129127050;    // 根据TraceID未找到业务信息
        public const uint IVS_SCU_TRACE_XML_NODE_NO_EXIST        = 129127051;    // XML节点信息不存在
        public const uint IVS_SCU_CREATE_TRACE_FAIL              = 129127052;    // 创建业务跟踪失败
        public const uint IVS_SCU_CANCLE_TRACE_FAIL              = 129127053;    // 取消业务跟踪失败
        public const uint IVS_SCU_TRACE_TIME_MORE_THAN_MAX       = 129127054;    // 业务跟踪时间超过最大值

        /* SCU:修改摄像机扩展参数接口错误码，取值范围：129147000 ~ 129147100*/
        public const uint IVS_SCU_WATERMARK_SECURITY_FAIL        = 129147000;    // DCG通知PU添加水印失败、MU切换媒体安全策略失败
        public const uint IVS_SCU_WATERMARK_FAIL                 = 129147001;    // DCG通知PU添加水印失败
        public const uint IVS_SCU_SECURITY_FAIL                  = 129147002;    // MU切换媒体安全策略失败

        /********************************************
	    *   SMU Portal错误码定义（模块编码：13）
	    *******************************************/ 
	 

	    /******************************************
	    *   CMU错误码定义（模块编码：14）
	    *******************************************/ 
	    public const uint IVS_CMU_FIND_LEADER_ERROR              = 149107001;    // 查找Leader失败

	    public const uint IVS_CMU_ADD_DEV_ERROR                  = 149113001;    // 添加设备失败
	    public const uint IVS_CMU_ADD_DEVS_SOME_ERROR            = 149113002;    // 添加一些设备失败
	    public const uint IVS_CMU_ADD_DEVS_ALL_ERROR             = 149113003;    // 添加所有设备失败	 


	     /******************************************
	     *   OMU错误码定义（模块编码：15）
	     *******************************************/
	    public const uint IVS_OMU_FILE_NOT_FOUND				   = 150000000;    // OMU 文件找不到
	    public const uint IVS_OMU_PIPE_CREATION_FAILED           = 150000001;    // OMU 管道创建失败
	    public const uint IVS_OMU_FORK_ERROR                     = 150000002;    // OMU 创建子进程失败
	    public const uint IVS_OMU_FILE_RENAME_ERROR	           = 150000003;	   // 文件重命名失败
	    //public const uint IVS_OMU_POINTER_ERROR	               = 150000004;	   // 空指针错误
	    //public const uint IVS_OMU_STRING_PARSE_ERROR	       = 150000005;	   // 字符串解析错误
	    public const uint IVS_OMU_FILE_EXIST_ERROR	           = 150000006;	   // 上传文件已存在
	    //// 公共:15+91+00(公共)+序号;
    //   public const uint IVS_OMU_CMD_ERROR                     = 159100000;    // 命令非法;
    //   public const uint IVS_OMU_CMD_NULL                      = 159100001;    // 命令为空;
    //   public const uint IVS_OMU_DECODE_ERROR                  = 159100002;    // 命令消息体解析错误;
    //   public const uint IVS_OMU_MSGBLOCK_NULL                 = 159100003;    // 命令消息体为空;
    //   public const uint IVS_OMU_XML_PARSE_ERROR               = 159100004;    // XML解析错误;
    //   public const uint IVS_OMU_XML_PARAM_ERROR               = 159100005;    // XML参数错误;
        public const uint IVS_OMU_ROUTE_MSG_ERROR                = 159100006;    // 消息路由失败;
	    //public const uint IVS_OMU_MSG_NOTIFY_FAIL              = 159100007;    // 通知其他模块失败    
	    public const uint IVS_OMU_SESSION_TIME_OUT               = 159100008;    // session超时;
	    //public const uint IVS_OMU_NOTICE_FAIL_ERROR            = 159100009;    // OMU通知其他模块失败;
        // 设备管理:15+91+01(设备管理)+序号;
        public const uint IVS_OMU_NODE_OFFLINE                   = 159101001;    // 节点离线; 
        public const uint IVS_OMU_MODULE_CFG_NOT_FOUND           = 159101005;    // OMU 模块配置不存在
        public const uint IVS_OMU_MODULE_CMD_NOT_FOUND           = 159101006;    // OMU 模块指令不存在
        public const uint IVS_OMU_MODULE_OPER_LIST_NOT_FOUND     = 159101007;    // OMU 元操作符为空
        public const uint IVS_OMU_NOT_DOWN_DOMAIN                = 159101008;    // 当前域包含子域，无法组建集群;
        public const uint IVS_OMU_HAS_NO_LICENSE		           = 159101009;    // 无授权许可，不可组建集群


	    public const uint IVS_OMU_MSG_TRACE_REPORT_FAIL		   = 159147001;    // 添加消息跟踪结果到上报队列失败;
	
	    public const uint IVS_OMU_REGISTER_PWD_INVALID           = 159103001;    // OMU 注册密码错误 
	    public const uint IVS_OMU_RE_REGISTER_INVALID            = 159103002;    // 不允许自己向自己注册
	    public const uint IVS_OMU_REGISTER_LOOP_ERR              = 159103003;    // 注册存在回路，不允许注册
	    public const uint IVS_OMU_REGISTER_RULE_ERR              = 159103004;    // 注册不符合规则
	    public const uint IVS_OMU_MAX_MPU_COUNT_ERR              = 159103006;    // MPU达到最大上限
	    public const uint IVS_OMU_MAX_VMU_COUNT_ERR              = 159103007;    // VMU达到最大上限
	    public const uint IVS_OMU_MAX_PAS_COUNT_ERR              = 159103008;    // PAS达到最大上限
        public const uint IVS_OMU_NO_ACCEPT_NEW_NODE             = 159103009;    // 不允许新设备注册 
	    public const uint IVS_OMU_CLUSTER_NOT_ALLOW_SUB_DOMAIN   = 159103010;    // 集群下不允许对接子域
	    public const uint IVS_OMU_REPORT_PWD_ERR			       = 159103011;    // 报表查询密码错误

        public const uint IVS_OMU_PORTAL_RESP_SUCCESS		       = 159102001;    // 登录成功
        public const uint IVS_OMU_PORTAL_PASSWD_WRONG            = 159102002;    // 用户名或密码输错
        public const uint IVS_OMU_PORTAL_RESP_DB_ERROR           = 159102003;    // 数据库错误
        public const uint IVS_OMU_PORTAL_RESP_XMLPRASE_ERROR     = 159102004;    // xml解析失败
        public const uint IVS_OMU_PORTAL_USER_LOCKED             = 159102005;    // 用户锁定
        public const uint IVS_OMU_PORTAL_RESP_SUCCESS_FIRST      = 159102006;    // 初始登录成功 
        public const uint IVS_OMU_CAN_NOT_ADD_NVR_ERROR          = 159102007;    // 不能新增NVR
        public const uint IVS_OMU_CAN_NOT_ADD_I_ERROR            = 159102008;    // 新增NVR不能是自己

        public const uint IVS_OMU_CAN_NOT_READ_CFG_ERROR         = 159102009;    // 驱动包配置文件读取失败

	    public const uint IVS_OMU_VERSION_EXIST_ERR			   = 159104001;    // 版本文件已存在错误

	    public const uint IVS_OMU_DIST_MONT_ISRUNNING		       = 159128001;    // 磁盘管理正在划分中
	 
	  
	    /**********************************************
	    *   MU错误码定义（模块编码：16）
	    ***********************************************/
	    /* MU:bsm返回错误码，取值范围：169100001 ~ 169100100;errcode = public const uint IVS_MU_ERR_CODE + BSM_ERR_CODE*/ 
	    public const uint IVS_MU_ERR_CODE_BEGIN                  = 169100001;    // BSM错误码起始值
	    public const uint IVS_MU_RESTOR_XFS_FAILED               = 169100045;    // 恢复xfs文件系统失败
	    public const uint IVS_MU_KEY_FILE_OPERATER_FAILED        = 169100046;    // 关键文件操作失败
	    public const uint IVS_MU_ERROR_BLOCK_STATUS              = 169100047;    // 块状态错误.
	    public const uint IVS_MU_ERROR_INVALID_BLOCK             = 169100048;    // 无效块号
	    public const uint IVS_MU_DEVICE_STATUS_ERROR             = 169100049;    // 存储设备状态不正确
	    public const uint IVS_MU_NEED_REBOOT_SYSTEM              = 169100050;    // 需要重启服务器
	    public const uint IVS_MU_DEVICE_EXISTS                   = 169100051;    // 设备已存在
	    public const uint IVS_MU_UNKNOWN_FILE_SYSTEM             = 169100052;
	    public const uint IVS_MU_MOUNT_ERROR                     = 169100053;    // bsm 挂载盘失败
	    public const uint IVS_MU_INVALID_PARAMETER               = 169100054;    // 无效的输入参数
	    public const uint IVS_MU_SCAN_SLOT_FAILED                = 169100055;    // BSM扫描槽位出错
	    public const uint IVS_MU_RAID_FAILED                     = 169100056;    // RAID模块未加载
	    public const uint IVS_MU_SLEEPWAKEUPFAILED               = 169100057;    // 休眠失败
	    public const uint IVS_MU_RAIDREBUILDING                  = 169100058;    // raid 重构中
	    public const uint IVS_MU_DB_DUP_ERROR                    = 169100059;    // 数据库主键冲突
	    public const uint IVS_MU_STREAM_PACKET_LEN_FAIL          = 169100060;    // 视频数据帧长度无效
	    public const uint IVS_MU_INVALID_DISKNUM                 = 169100061;    // 无效的盘号
	    public const uint IVS_MU_LOCK_OTHER_ERROR                = 169100062;    // 锁操作失败
	    public const uint IVS_MU_SAVE_INDEX_FAILED               = 169100063;    // 保存索引失败
	    public const uint IVS_MU_DISK_FAILED                     = 169100064;    // 磁盘错误(整盘失效)
	    public const uint IVS_MU_NO_ENOUGH_RESERVESPACE          = 169100065;    // 没有足够的预留空间
	    public const uint IVS_MU_INDEX_OUTDATED                  = 169100066;    // 已经循环覆盖的索引
	    public const uint IVS_MU_INDEX_UNFOUND                   = 169100067;    // save session查询索引失败
	    public const uint IVS_MU_DB_NO_DATA                      = 169100068;    // 数据库中没有相关数据
	    public const uint IVS_MU_DB_EXCEPTION                    = 169100069;    // 数据库操作异常
	    public const uint IVS_MU_INIT_DB_POLL_FAILED             = 169100070;    // 初始化数据连接池失败
	    public const uint IVS_MU_DELETE_DB_BLOCKINFO_ERROR       = 169100071;    // 删除数据中块信息失败
	    public const uint IVS_MU_OPERATING_RECLAIMED_IPC         = 169100072;    // 试图打开正在删除的摄像头
	    public const uint IVS_MU_LOCK_MULTI_LOCK                 = 169100073;    // 试图多次锁定非递归锁
	    public const uint IVS_MU_LOCK_RECURSIVE_EXCEED           = 169100074;    // 线程递归锁已经到达最大递归数限制
	    public const uint IVS_MU_LOCK_NOT_INIT_OK                = 169100075;    // 线程互斥锁没有初始化
	    public const uint IVS_MU_TOO_LARGE_DATA_SIZE             = 169100076;    // 图片尺寸超过1M; 超出BSM预留的1M
	    public const uint IVS_MU_INVALID_ID                      = 169100077;    // 无效摄像头ID
	    public const uint IVS_MU_CREATE_FOLDER_FAILED            = 169100078;    // 创建文件夹失败
	    public const uint IVS_MU_WRONG_SESSION_TYPE              = 169100079;    // 所要求的操作和当前会话类型不匹配
	    public const uint IVS_MU_INVALID_SESSION_ID              = 169100080;    // 无效的会话ID
	    public const uint IVS_MU_INVALID_SESSION_TYPE            = 169100081;    // 无效的会话类型
	    public const uint IVS_MU_INVALID_POINTER                 = 169100082;    // 无效指针
	    public const uint IVS_MU_NEW_FAILED                      = 169100083;    // new操作失败
	    public const uint IVS_MU_NO_BLOCK                        = 169100084;    // Spaces无法分配数据块
	    public const uint IVS_MU_NO_PHOTO_DATA                   = 169100085;    // 没有相关图片数据
	    public const uint IVS_MU_INVALID_TIME                    = 169100086;    // 无效时间
	    public const uint IVS_MU_PHOTO_RTTI_ERROR                = 169100087;    // 存储图片校验标志
	    public const uint IVS_MU_END_FLAG_ERROR                  = 169100088;    // 存储空间尾标志错误
	    public const uint IVS_MU_START_FLAG_ERROR                = 169100089;    // 存储空间头标志错误
	    public const uint IVS_MU_INVALID_SIZE                    = 169100090;    // 无效长度
	    public const uint IVS_MU_BUFFER_NOT_ENOUGH               = 169100091;    // 缓存空间不足
	    public const uint IVS_MU_UNKNOW_EXCEPTION                = 169100092;    // 未知异常
	    public const uint IVS_MU_ISTREAM_CREATE_FAILED           = 169100093;    // 输入流创建错误
	    public const uint IVS_MU_OSTREAM_CREATE_FAILED           = 169100094;    // 输出流创建错误
	    public const uint IVS_MU_SEEK_FAILED                     = 169100095;    // 文件定位错误
	    public const uint IVS_MU_OPEN_FAILED                     = 169100096;    // 打开存储文件错误
	    public const uint IVS_MU_READ_FAILED                     = 169100097;    // 读磁盘错误
	    public const uint IVS_MU_WRITE_FAILED                    = 169100098;    // 写磁盘错误

	    public const uint IVS_MU_ERR_CODE                        = 169100100;    // BSM错误码起始值

	    public const uint IVS_MU_RECORD_SEARCH_INDEX_ERROR	   = 169100200;    // 录像检索索引错误
	    public const uint IVS_MU_FROM_TO_TIME_ERROR			   = 169100201;	   // 录像检索开始结束时间错误
	    public const uint IVS_MU_TIME_FORMAT_ERROR			   = 169100202;	   // 录像检索时间格式错误
	    public const uint IVS_MU_RECORD_METHOD_ERROR			   = 169100203;	   // 录像检索方式错误
	    public const uint IVS_MU_CAMERA_CODE_ERROR			   = 169100204;	   // 录像检索摄像机编码错误
	    public const uint IVS_MU_RECORD_SEARCH_NSS_ERROR		   = 169100205;    // 录像检索NSS消息错误
	    public const uint IVS_MU_REPORT_DISK_RATE_ERROR		   = 169100206;    // 查询磁盘信息失败错误


	    /******************************************
	    *   SafeVideo错误码定义（模块编码：17）
	    *******************************************/

        /******************************************
        *   DCG错误码定义（模块编码：18）
        *******************************************/
        //DCG:录像计划配置 错误码 功能编码-25 
        public const uint IVS_DCG_SET_RECORD_ERROR              = 189125001;     // 设置录像计划错误
        public const uint IVS_DCG_GET_RECORD_ERROR              = 189125002;     // 获取录像计划错误
        public const uint IVS_DCG_SET_LAYER_CODE_ERROR          = 189125003;     // 分层编码设置错误
        public const uint IVS_DCG_RECORD_PLAN_NOT_EXIST         = 189125004;     // 录像计划为空
        public const uint IVS_DCG_CREATE_RECORD_STREAM_SMP_ERROR= 189125005;     // 创建录像流对象错误
        //DCG:云镜控制 错误码 功能编码-16
        public const uint IVS_DCG_CONTROL_PTZ_ERROR             = 189116001;     // 云台控制错误
        public const uint IVS_DCG_GET_PTZ_PARAM_ERROR           = 189116002;     // 获取云台参数错误
        public const uint IVS_DCG_SET_PTZ_PARAM_ERROR           = 189116003;     // 设置云台参数错误
        public const uint IVS_DCG_GET_PTZ_PROTOCOL_ERROR        = 189116004;     // 获取云台解码器协议错误
        public const uint IVS_DCG_GET_CRUISE_TRACK_ERROR        = 189116005;     // 获取巡航轨迹参数错误
        public const uint IVS_DCG_SET_CRUISE_TRACK_ERROR        = 189116006;     // 设置巡航轨迹参数错误
        public const uint IVS_DCG_CLEAL_CRUISE_TRACK_ERROR      = 189116007;     // 清空巡航列表错误
        public const uint IVS_DCG_TRACK_COMMAND_ERROR           = 189116008;     // 云台PTZ轨迹控制失败
        public const uint IVS_DCG_CRUISE_NUMBER_ERROR           = 189116009;     // 设置的巡航轨迹序号超过设备支持范围
        public const uint IVS_DCG_UNSUPPORT_PTZ_CMD             = 189116010;     // 不支持的云台控制命令
        public const uint IVS_DCG_TRACK_INDEX_ERROR             = 189116011;     // 设置的PTZ录制轨迹序号超过设备支持范围

        //DCG:预置位;看守位控制 错误码 功能编码-17
        public const uint IVS_DCG_SET_PRESET_ERROR              = 189117001;     // 设置预置位参数错误
        public const uint IVS_DCG_GET_PRESET_ERROR              = 189117002;     // 查询预置位参数错误
        public const uint IVS_DCG_DEL_PRESET_ERROR              = 189117003;     // 删除预置位参数错误
        public const uint IVS_DCG_SET_GUARD_ERROR               = 189117004;     // 设置看守位请求错误
        public const uint IVS_DCG_GET_GUARD_ERROR               = 189117005;     // 查询看守位请求错误

        //DCG:告警管理 错误码 功能编码-20
        public const uint IVS_DCG_SET_ALARM_GUARD_ERROR         = 189120001;     // 设置告警布撤防错误
        public const uint IVS_DCG_GET_ALARM_GUARD_ERROR         = 189120002;     // 获取镜头布撤防参数错误
        public const uint IVS_DCG_MAIN_DEV_ALARM_GUARD_ERROR    = 189120003;     // 主设备告警布防错误
        public const uint IVS_DCG_MAIN_DEV_ALARM_UNGUARD_ERROR  = 189120004;     // 主设备告警撤防错误
        public const uint IVS_DCG_MAIN_DEV_SET_ALARM_CALLBACK_ERROR = 189120005; // 主设备设置告警回调错误
        public const uint IVS_DCG_SET_VIDEO_MASK_ERROR          = 189120006;     // 设置隐私保护请求错误
        public const uint IVS_DCG_GET_VIDEO_MASK_ERROR          = 189120007;     // 查询隐私保护请求错误
        public const uint IVS_DCG_SET_MOTION_DETECTION_ERROR    = 189120008;     // 设置移动侦测请求错误
        public const uint IVS_DCG_GET_MOTION_DETECTION_ERROR    = 189120009;     // 查询移动侦测请求错误
        public const uint IVS_DCG_SET_MASK_PROTECT_ERROR        = 189120010;     // 设置视频遮盖请求错误
        public const uint IVS_DCG_GET_MASK_PROTECT_ERROR        = 189120011;     // 查询视频遮盖请求错误
        public const uint IVS_DCG_SET_IGT_COMMEM_PARA_ERROR     = 189120012;     // 设置智能分析公共参数错误
        public const uint IVS_DCG_SET_IGT_TRIPWIRE_PARA_ERROR   = 189120013;     // 设置智能分析绊线规则参数错误
        public const uint IVS_DCG_SET_IGT_INTRUSION_PARA_ERROR  = 189120014;     // 设置智能分析入侵检测参数错误
        public const uint IVS_DCG_SET_IGT_HOVER_PARA_ERROR      = 189120015;     // 设置智能分析徘徊检测参数错误
        public const uint IVS_DCG_SET_IGT_LEGACY_PARA_ERROR     = 189120016;     // 设置智能分析遗留检测参数错误
        public const uint IVS_DCG_SET_IGT_REMOVE_PARA_ERROR     = 189120017;     // 设置智能分析移走检测参数错误
        public const uint IVS_DCG_GET_IGT_COMMEM_PARA_ERROR     = 189120018;     // 获取智能分析公共参数错误
        public const uint IVS_DCG_GET_IGT_TRIPWIRE_PARA_ERROR   = 189120019;     // 获取智能分析绊线规则参数错误
        public const uint IVS_DCG_GET_IGT_INTRUSION_PARA_ERROR  = 189120020;     // 获取智能分析入侵检测参数错误
        public const uint IVS_DCG_GET_IGT_HOVER_PARA_ERROR      = 189120021;     // 获取智能分析徘徊检测参数错误
        public const uint IVS_DCG_GET_IGT_LEGACY_PARA_ERROR     = 189120022;     // 获取智能分析遗留检测参数错误
        public const uint IVS_DCG_GET_IGT_REMOVE_PARA_ERROR     = 189120023;     // 获取智能分析移走检测参数错误
        public const uint IVS_DCG_SET_ALARM_LINK_PARAM_ERROR    = 189120024;     // 设置告警联动策略参数错误
        public const uint IVS_DCG_ALARM_LINK_CANT_FIND_CHANNEL  = 189120025;     // 摄像头编码找不到对应的通道

        //DCG:设备管理 错误码 功能编码-08
        public const uint IVS_DCG_FIND_MAIN_DEV_EXIST           = 189108001;     // 查找主设备已经存在
        public const uint IVS_DCG_ADD_MAIN_DEV_ERROR            = 189108002;     // 添加主设备错误
        public const uint IVS_DCG_LOAD_PLUGIN_ERROR             = 189108003;     // 加载设备插件错误
        public const uint IVS_DCG_GET_DEV_PORT_ERROR            = 189108004;     // 获取设备端口错误
        public const uint IVS_DCG_FIND_MAIN_DEV_ERROR           = 189108005;     // 查找主设备不存在
        public const uint IVS_DCG_DEL_SUB_DEV_ERROR             = 189108006;     // 删除子设备错误
        public const uint IVS_DCG_DEL_MAIN_DEV_ERROR            = 189108007;     // 删除主设备错误
        public const uint IVS_DCG_REBOOT_MAIN_DEV_ERROR         = 189108008;     // 重启主设备错误
        public const uint IVS_DCG_OPEN_DLL_ERROR                = 189108009;     // Open动态库错误
        public const uint IVS_DCG_SYMBOL_DLL_ERROR              = 189108010;     // 动态库symbol错误
        public const uint IVS_DCG_CREATE_SDK_DLL_ERROR          = 189108011;     // 动态库CreateSdk错误
        public const uint IVS_DCG_SDK_INIT_ERROR                = 189108012;     // Sdk初始化错误
        public const uint IVS_DCG_DEV_LOGIN_ERROR               = 189108013;     // 主设备登录错误
        public const uint IVS_DCG_DEV_LOGIN_PASSWORD_ERROR      = 189108014;     // 主设备登录密码错误
        public const uint IVS_DCG_DEV_GET_ABILITY_ERROR         = 189108015;     // 主设备获取设备能力错误
        public const uint IVS_DCG_DEV_SDK_INIT_ERROR            = 189108016;     // 初始化设备SDK错误
        public const uint IVS_DCG_CREATE_LENS_ERROR             = 189108017;     // 创建镜头失败
        public const uint IVS_DCG_GET_TIME_PARAM_ERROR          = 189108018;     // 获取时间参数失败
        public const uint IVS_DCG_SET_TIME_PARAM_ERROR          = 189108019;     // 设置时间参数失败
        public const uint IVS_DCG_GET_NET_PARAM_ERROR           = 189108020;     // 获取网络参数失败
        public const uint IVS_DCG_SET_NET_PARAM_ERROR           = 189108021;     // 设置网络参数失败
        public const uint IVS_DCG_BIND_TRANSID_AND_SEARCHD_DEV_TASK_ERROR    = 189108022; //绑定事物ID与查找设备业务失败
        public const uint IVS_DCG_UNBIND_TRANSID_AND_SEARCHD_DEV_TASK_ERROR  = 189108023; //解除事物ID与查找设备业务绑定失败
        public const uint IVS_DCG_SEARCH_DEVICE_ERROR           = 189108024;     // 搜索设备失败
        public const uint IVS_DCG_CAN_NOT_FIND_SERACHDEVTASK    = 189108025;     // 查找不到搜索设备的事物
        public const uint IVS_DCG_STOP_SEARCH_DEVICE_ERROR      = 189108026;     // 停止搜索设备失败
        public const uint IVS_DCG_CREATE_VENDER_DEV_ERROR       = 189108027;     // 创建适配层设备失败
        public const uint IVS_DCG_UN_SUPPORT_PROTOCOL           = 189108028;     // 不支持的协议类型
        public const uint IVS_DCG_VERSION_NO_MATCH              = 189108029;     // SDK与设备版本不匹配(针对自研SDK)
        public const uint IVS_DCG_DEV_DISCOVERY_RUNNING         = 189108030;     // 设备自动发现服务已经在运行
        public const uint IVS_DCG_VENDOR_DEV_INVALID            = 189108031;     // 主设备句柄无效
        public const uint IVS_DCG_DEV_LOGIN_HANDLE_INVALID      = 189108032;     // 设备登录句柄无效
        public const uint IVS_DCG_DEV_UN_SUPPORT                = 189108033;     // 设备不支持
        public const uint IVS_DCG_DEV_UPDATE_ERROR              = 189108034;     // 设备升级失败
        public const uint IVS_DCG_UNAUTHORIZED                  = 189108035;     // 未鉴权
        public const uint IVS_DCG_HTTP_DIGEST_AUTHORIZE_ERROR   = 189108036;     // Http-Digest鉴权错误
        public const uint IVS_DCG_WSSE_AUTHORIZE_ERROR          = 189108037;     // WSSE鉴权错误
        public const uint IVS_DCG_ONVIF_SOAP_OBJECT_INVALID     = 189108038;     // SOAP对象无效
        public const uint IVS_DCG_ONVIF_SOAP_OPER_FAIL          = 189108039;     // SOAP请求失败
        public const uint IVS_DCG_REGIST_EVENT_CALL_BACK_ERROR  = 189108040;     // sdk注册前端异常回调错误
        public const uint IVS_DCG_ONVIF_APP_OBJECT_INVALID      = 189108041;     // ONVIF App对象无效
        public const uint IVS_DCG_FIND_SUB_DEV_EXIST            = 189108042;     // 查找子设备已经存在
        public const uint IVS_DCG_HTTP_NOT_IMPLEMENTED          = 189108043;     // HTTP访问501错误
        public const uint IVS_DCG_SET_CONFIG_FILE_ERROR         = 189108044;     // 设置配置文件错误
        public const uint IVS_DCG_GET_CONFIG_FILE_ERROR         = 189108045;     // 获取配置文件错误
        public const uint IVS_DCG_ONVIF_NO_CONF                 = 189108046;     // ONVIF无相关配置信息
        public const uint IVS_DCG_MAIN_DEV_REBOOTING              = 189108047;  //主设备重启中

        //DCG:实况 错误码 功能编码-15
        public const uint IVS_DCG_ADD_STREAM_PARAM_ERROR        = 189115001;     // 添加请求码流参数错误
        public const uint IVS_DCG_FIND_STREAM_PARAM_EXIST       = 189115002;     // 查找码流参数已经存在
        public const uint IVS_DCG_FIND_STREAM_PARAM_ERROR       = 189115003;     // 查找码流参数错误、不存在
        public const uint IVS_DCG_OPEN_SOCKET_ERROR             = 189115004;     // 初始化socket错误
        public const uint IVS_DCG_START_STREAM_ERROR            = 189115005;     // 开始取流错误
        public const uint IVS_DCG_TCP_OPEN_SOCKET_ERROR         = 189115006;     // TCP-socket打开错误
        public const uint IVS_DCG_TCP_SOCKET_CONNECT_ERROR      = 189115007;     // TCP-socket连接错误
        public const uint IVS_DCG_UDP_OPEN_SOCKET_ERROR         = 189115008;     // UDP-socket打开错误
        public const uint IVS_DCG_STOP_REAL_STREAM_ERROR        = 189115009;     // 停止流错误
        public const uint IVS_DCG_REQUEST_IFRAME_ERROR          = 189115010;     // 请求I帧错误
        public const uint IVS_DCG_STREAM_HANDLE_INVALID         = 189115011;     // 流句柄无效
	    public const uint IVS_DCG_SEND_SIP_INVITE_ERROR         = 189115012;     // 向千里眼设备发送sip invite错误
        public const uint IVS_DCG_SIP_INVITE_RESPONSE_ERROR     = 189115013;     // 千里眼设备回复sip invite响应错误
        public const uint IVS_DCG_MEDIA_BUF_WITH_RTP_INVALID    = 189115014;     // 带RTP头的海康、大华流需要使用该指针，为空时报错
        public const uint IVS_DCG_BIND_SERVICE_ID_AND_MU_LINK_ID_ERROR   = 189115015;   // 绑定ServiceID和MULinkID出错
        public const uint IVS_DCG_UNBIND_SERVICE_ID_AND_MU_LINK_ID_ERROR = 189115016;   // 解除绑定ServiceID和MULinkID出错
        public const uint IVS_DCG_BIND_SERVICE_ID_AND_MSGKEY_ERROR       = 189115017;   // 千里眼流请求的时候绑定ServiceID和MsgKey错误
        public const uint IVS_DCG_UNBIND_SERVICE_ID_AND_MSGKEY_ERROR     = 189115018;   // 千里眼流请求的时候解除绑定ServiceID和MsgKey错误
        public const uint IVS_DCG_BIND_SERVICE_ID_AND_SESSION_ID_ERROR   = 189115019;   // 千里眼流请求的时候绑定ServiceID和千里眼消息中的会话ID错误
        public const uint IVS_DCG_UNBIND_SERVICE_ID_AND_SESSION_ID_ERROR = 189115020;   // 千里眼流请求的时候解除绑定ServiceID和千里眼消息中的会话ID错误
        public const uint IVS_DCG_TRANSFER_PAYLOADTYPE_FAILED    = 189115021;     // payloadtype转换错误
        public const uint IVS_DCG_STREAM_UNSTARTED               = 189115022;     // 流未启动
        public const uint IVS_DCG_SET_WATERMARK_ERROR            = 189115023;     // 设置水印错误
        public const uint IVS_DCG_TCP_SOCKET_ACCEPT_ERROR        = 189115024;     // TCP-SOCKET-Accept错误
        public const uint IVS_DCG_TCP_SOCKET_ACCEPTOR_CLOSE_ERROR= 189115025;     // TCP-SOCKET-Acceptor close错误
        public const uint IVS_DCG_REV_STREAM_REQUEST_ERROR       = 189115026;     // 处理流请求错误，主要是千里眼设备发送Invt消息的时候错误
        public const uint IVS_DCG_UNSUPPORTED_ENCODE_MODE        = 189115028;     // 不支持的编码格式
        public const uint IVS_DCG_UNSUPPORTED_PROFILE            = 189115029;     // 不支持的Profile
        public const uint IVS_DCG_DECRYPT_SECURITY_ERROR         = 189115030;     // 解密失败
        public const uint IVS_DCG_STREAM_HAS_STOPED                     = 189115031;     // 实况流已停止

        //DCG:开关量输入子设备          10
        public const uint IVS_DCG_GET_ALARM_IN_PARAM_ERROR      = 189110001;     // 获取开关量子设备参数错误
        public const uint IVS_DCG_SET_ALARM_IN_PARAM_ERROR      = 189110002;     // 设置开关量子设备参数错误

        //DCG:设备管理-开关量输出子设备  11
        public const uint IVS_DCG_GET_ALARM_OUT_PARAM_ERROR     = 189111001;     // 获取开关量子设备参数错误
        public const uint IVS_DCG_SET_ALARM_OUT_PARAM_ERROR     = 189111002;     // 设置开关量子设备参数错误
        public const uint IVS_DCG_GET_ALARM_OUT_STATE_ERROR     = 189111003;     // 获取开关量子设备参数错误
        public const uint IVS_DCG_SET_ALARM_OUT_STATE_ERROR     = 189111004;     // 设置开关量子设备参数错误

        //DCG:摄像机子设备              09
        public const uint IVS_DCG_GET_CAMERA_STREAM_PARAM_ERROR = 189109001;     // 获取摄像机码流参数错误
        public const uint IVS_DCG_SET_CAMERA_STREAM_PARAM_ERROR = 189109002;     // 设置摄像机码流参数错误
        public const uint IVS_DCG_GET_IMAGING_PARAM_ERROR       = 189109003;     // 获取摄像机基本图像参数错误
        public const uint IVS_DCG_SET_IMAGING_PARAM_ERROR       = 189109004;     // 设置摄像机基本图像参数错误
        public const uint IVS_DCG_GET_VIDEO_PARAM_ERROR         = 189109005;     // 获取前端视频子设备参数错误
        public const uint IVS_DCG_SET_VIDEO_PARAM_ERROR         = 189109006;     // 设置前端视频子设备参数错误
        public const uint IVS_DCG_GET_VIDEO_EFFECT_PARAM_ERROR  = 189109007;     // 获取前端视频效果参数错误
        public const uint IVS_DCG_SET_VIDEO_EFFECT_PARAM_ERROR  = 189109008;     // 设置前端视频效果参数错误
        public const uint IVS_DCG_GET_VIDEO_ANALOG_FORMAT_ERROR = 189109009;     // 获取前端视频制式错误
        public const uint IVS_DCG_SET_VIDEO_ANALOG_FORMAT_ERROR = 189109010;     // 设置前端视频制式错误
        public const uint IVS_DCG_SET_OSD_ERROR                 = 189109011;     // 设置OSD参数错误
        public const uint IVS_DCG_GET_OSD_ERROR                 = 189109012;     // 获取OSD参数错误
        public const uint IVS_DCG_GET_EXTIMAGING_PARAM_ERROR    = 189109013;     // 获取摄像机扩展图像参数错误
        public const uint IVS_DCG_SET_EXTIMAGING_PARAM_ERROR    = 189109014;     // 设置摄像机扩展图像参数错误
        public const uint IVS_DCG_FIND_LENS_INVALID             = 189109015;     // 未查找到摄像机子设备
        public const uint IVS_DCG_VENDOR_LENS_INVALID           = 189109016;     // 摄像机子设备句柄无效

        //DCG:设备管理-语音子设备	       12
        public const uint IVS_DCG_GET_AUDIO_PARAM_ERROR         = 189112001;     // 获取音频子设备参数错误
        public const uint IVS_DCG_SET_AUDIO_PARAM_ERROR         = 189112002;     // 设置音频子设备参数错误

        //DCG:设备管理-串口子设备	       13
        public const uint IVS_DCG_GET_SERIAL_PARAM_ERROR        = 189113001;     // 获取串口子设备参数错误 
        public const uint IVS_DCG_SET_SERIAL_PARAM_ERROR        = 189113002;     // 设置串口子设备参数错误

        //DCG:设备管理-语音	       45
        public const uint IVS_DCG_START_VOICE_STREAM_ERROR      = 189145001;     // 开始音频对讲错误
        public const uint IVS_DCG_STOP_VOICE_STREAM_ERROR       = 189145002;     // 停止音频对讲错误
        public const uint IVS_DCG_START_REVERSE_AUDIO_THREAD_ERROR = 189145003;  // 开启接受CU客户端语音数据线程失败

        //DCG:日志管理                  30
        public const uint IVS_DCG_SET_LOG_CONFIG_ERROR          = 189130001;     // 设置日志开关参数错误
        public const uint IVS_DCG_GET_LOG_SFTP_CONF_ERROR       = 189130002;     // 获取日志统一配置错误
        public const uint IVS_DCG_SET_LOG_SFTP_CONF_ERROR       = 189130003;     // 设置日志统一配置错误
        public const uint IVS_DCG_GET_PU_LOG_ERROR              = 189130004;     // 获取前端日志失败

        //DCG：前端录像   40
        public const uint IVS_DCG_START_PU_RECORD_ERROR         = 189140001;     // 开启前端手动录像错误
        public const uint IVS_DCG_STOP_PU_RECORD_ERROR          = 189140002;     // 停止前端手动录像错误
        public const uint IVS_DCG_GET_PU_RECORD_STATE_ERROR     = 189140003;     // 获取前端录像状态错误
        public const uint IVS_DCG_GET_PU_RECORD_LIST_ERROR      = 189140004;     // 检索前端录像错误
        public const uint IVS_DCG_STOP_PLAYBACK_STREAM_ERROR    = 189140005;     // 停止前端录像回放错误
        public const uint IVS_DCG_START_PLAYBACK_STREAM_ERROR   = 189140006;     // 开启前端录像回放错误

        //DCG：录像回放             19
        public const uint IVS_DCG_PLAYBACK_BY_NAME_ERROR        = 189119001;     // 根据录像文件名错误
        public const uint IVS_DCG_SET_PLAYBACK_CALLBACK_ERROR   = 189119002;     // 设置录像回放回调函数错误
        public const uint IVS_DCG_PLAYBACK_START_PLAYBACK_ERROR = 189119003;     // 启动录像回调错误
        public const uint IVS_DCG_PLAYBACK_PLAYRESTART_ERROR    = 189119004;     // 回放恢复播放错误
        public const uint IVS_DCG_PLAYBACK_PLAYPAUSE_ERROR      = 189119005;     // 回放暂停错误
        public const uint IVS_DCG_PLAYBACK_PLAYFAST_ERROR       = 189119006;     // 回放快进错误
        public const uint IVS_DCG_PLAYBACK_PLAYSLOW_ERROR       = 189119007;     // 回放慢放错误
        public const uint IVS_DCG_PLAYBACK_PLAYSETTIME_ERROR    = 189119008;     // 回放设置播放时间错误
        public const uint IVS_DCG_PLAYBACK_LIST_IS_EMPTY_ERROR  = 189119009;     // 录像回放录像下标list为空错误
        public const uint IVS_DCG_PLAYBACK_INDEX_VALUE_ERROR    = 189119010;     // 从录像list拿出来的index错误
        public const uint IVS_DCG_STRTEAM_CONTROL_TIME_INVALID  = 189119011;     // 时间拖动，下发时间超过录像长度
	    public const uint IVS_DCG_PLEASE_RESEARCH_RECORD_LIST   = 189119012;     // 请重新检索录像
        //DCG：修改密码             26
        public const uint IVS_DCG_MOD_DEV_PASSWORD_ERROR        = 189126001;     // 修改前端密码错误

        //DCG：抓拍管理             46
        public const uint IVS_DCG_GET_SNAP_SHOT_ERROR           = 189146001;     // 获取前端抓拍错误
        public const uint IVS_DCG_PU_HAVE_NO_STORAGE_DEVICE     = 189146002;     // 前端设备没有存储设备


        //DCG:外部系统管理          39
        public const uint IVS_DCG_GET_EXTERN_DEV_LIST_ERROR     = 189139001;     // 获取外部系统设备列表错误
        public const uint IVS_DCG_SET_EXTERN_DEV_LIST_ERROR     = 189139002;     // 设置外部系统设备列表错误
        public const uint IVS_DCG_GET_SIPASS_UNIT_TABLE_ERROR   = 189139003;     // 获取西门子unit表错误
        public const uint IVS_DCG_GET_SIPASS_DEVICE_TABLE_ERROR = 189139004;     // 获取西门子deviec表错误
        public const uint IVS_DCG_GET_SIPASS_POINT_TABLE_ERROR  = 189139005;     // 获取西门子point表错误
        public const uint IVS_DCG_GET_SIPASS_FLN_TABLE_ERROR    = 189139006;     // 获取西门子FLN表错误
        public const uint IVS_DCG_FIND_EXT_DEV_ERROR            = 189139007;     // 获取map中，外部设备错误
        public const uint IVS_DCG_ADD_EXTSUBDEV_ECHO_ERROR      = 189139008;     // 添加外部设备时，设备已经存在
	    public const uint IVS_DCG_SIPASS_CON_SER_REG_ERROR      = 189139009;     // 连接服务器注册表错误
        public const uint IVS_DCG_SIPASS_OPERATE_REGEDIT_ERROR  = 189139010;     // 操作注册表错误
        public const uint IVS_DCG_SIPASS_GET_LO_REG_CLSID_ERROR = 189139011;     // 获取本地注册表，CLSID错误
        public const uint IVS_DCG_SIPASS_CREATE_INSTANCE_ERROR  = 189139012;     // 创建COM对象错误
        public const uint IVS_DCG_SIPASS_SET_ATL_ADVISE_ERROR   = 189139013;     // 设置告警上报接口错误
        public const uint IVS_DCG_EXT_SYS_IS_NOT_ONLINE_ERROR   = 189139014;     // 外部系统还未注册上线
        /******************************************
	    *   Transcode转码错误码定义（模块编码：24）
	    *******************************************/



	    /******************************************
	    *   HTTPS TS流分发错误码定义（模块编码：25）
	    *******************************************/
 	
	    /******************************************
	    *   RTSP分发错误码定义（模块编码：26）
	    *******************************************/

	    /******************************************
	    *   PCG错误码定义（模块编码：27）
	    *******************************************/
	    public const uint IVS_PCG_XML_DECODE_ERROR			   = 279139001; 	// xml解析错误
	    public const uint IVS_PCG_ERROR 						   = 279139000;		// 服务器内部错误
        public const uint IVS_PCG_PARA_ERROR 					   = 279140000; 	// 参数校验失败
	    public const uint IVS_PCG_CONN_CODE_ALREADY_EXIST 	   = 279139003; 	// 域编码已经存在
	    public const uint IVS_PCG_EXCEED_EXTERN_MAX_NUM 		   = 279139004; 	// 外域数量超过128
	    public const uint IVS_PCG_DEVICEID_REPEAT 			   = 279139005; 	// 设备互联编码重复
	    public const uint IVS_PCG_EXIST_EXTERN_DOMAIN_PROTOCOL   = 279139006; 	// 存在该协议类型的外域
	    /******************************************
	    *   MBU错误码定义（模块编码：28）
	    *******************************************/

	    /******************************************
	    *   MAUS错误码定义（模块编码：29）
	    *******************************************/
    //	 public const uint IVS_MAU_SUCCESS                       = 299133200;     // 响应成功
        public const uint IVS_MAU_FAILURE                        = 299133201;     // 响应失败
        public const uint IVS_MAU_SUCCESS_FIRST		           = 299133210;     // 首次登陆成功
        public const uint IVS_MAU_USER_LOCKED		               = 299133211;     // 用户锁定

        public const uint IVS_MAU_AUTHENTICATE_FAILURE           = 299133301;     // 登陆认证失败；
        public const uint IVS_MAU_TRANS_NOT_EXIST                = 299133501;     // 事务不存在，指定的事务不存在；
        public const uint IVS_MAU_FRONTDEV_NOT_EXIST             = 299133502;     // 主设备不存在，在查询主设备参数时，如果主设备不存在返回，其它错误情况，没有特别说明的，都返回
        public const uint IVS_MAU_LENS_NOT_EXIST                 = 299133503;     // 镜头设备不存在，在查询镜头设备参数时，如果镜头设备不存在返回，其它错误情况，没有特别说明的，都返回
        public const uint IVS_MAU_OTHER_ERROR                    = 299133601;     // 其他错误
        public const uint IVS_MAU_DB_ERROR                       = 299133602;     // 数据库操作异常
        public const uint IVS_MAU_PARAM_ERROR                    = 299133603;     // 参数异常
        public const uint IVS_MAU_RUNTIME_ERROR                  = 299133604;     // 运行时异常
        public const uint IVS_MAU_XMLPRASE_ERROR                 = 299133605;     // XML解析异常
        public const uint IVS_MAU_OVERLOAD_ERROR                 = 299133607;     // 过载异常
        public const uint IVS_MAU_RULEGROUP_NOTEXIST             = 299133608;     // 规则不存在
        public const uint IVS_MAU_PLANID_NOTEXIST                = 299133609;     // 计划信息不存在
        public const uint IVS_MAU_PLAN_TIME_CONFLICT             = 299133613;     // analyze Plan时间段重叠RESP_PLANANA_TSCONFL_ERROR
        public const uint IVS_MAU_PLAN_STARTORSTOPPING           = 299133614;     // 计划正在启停RESP_PLAN_STARTSTOPING
        public const uint IVS_MAU_REPEAT_OPRATION_ERROR          = 299133615;     // 重复操作

        public const uint IVS_MAU_FACEDBTYPE_NOT_EXIST           = 299133616;     // 人脸类型库不存在
        public const uint IVS_MAU_FACEDBTYPE_EXIST               = 299133617;     // 人脸库类型已经存在
        public const uint IVS_MAU_FACEDBNAME_EXIST               = 299133618;     // 人脸库类型名称已经存在
        public const uint IVS_MAU_FACE_NOT_DETECT                = 299133619;     // 未检测到人脸
        public const uint IVS_MAU_REQ_ENROLL2FACESRV_FAIL        = 299133620;     // 请求中转服务器注册人像模板失败
        public const uint IVS_MAU_REQ_ENROLL2FACESRV_EXCP        = 299133621;     // 请求中转服务器注册人像模板异常
        public const uint IVS_MAU_REQ_DELFROMFACESRV_EXCP        = 299133622;     // 请求节点服务器删除人像模板异常
        public const uint IVS_MAU_REQ_DELFROMFACESRV_FAIL        = 299133623;     // 请求节点服务器删除人像模板失败
        public const uint IVS_MAU_IMAGE_FORMAT_INVALIDATE        = 299133624;     // 图片格式不对
        public const uint IVS_MAU_REQ_UPDATE2FACESRV_EXCP        = 299133625;     // 请求中转服务器更新人像模板失败
        public const uint IVS_MAU_REQ_MEDIAANATYPE_ERR           = 299133626;     // 智能分析请求类型不正确
        public const uint IVS_MAU_NOT_FIND_MATCHD_FACE           = 299133628;     // 未找到相似的人脸
        public const uint IVS_MAU_FACEID_EXIST                   = 299133629;     // 人脸ID已存在
        public const uint IVS_MAU_FACEID_NOEXIST                 = 299133630;     // 人脸ID不存在


	    /******************************************
	    *   MAUQD错误码定义（模块编码：30）
	    *******************************************/

	    /******************************************
	    *   MAUBA错误码定义（模块编码：31）
	    *******************************************/


	    /******************************************
	    *   MAUFR错误码定义（模块编码：32）
	    *******************************************/


	     /******************************************
	     *   BP错误码定义（模块编码：97）
	     *******************************************/
	     public const uint IVS_BP_SUCCEED                       = 979199000;     // 成功;
	     public const uint IVS_BP_ERROR                         = 979199001;     // 失败;
	     public const uint IVS_BP_PARA_INVALID                  = 979199002;     // 输入参数非法

	
	     /******************************************
	     *   VWU错误码定义（模块编码：98）
	     *******************************************/



	    /******************************************
	    *   CU错误码定义（模块编码：99）
        *   错误码后三位编号：OCX 000～199 SDK 200～399 MP 400～599
	    *******************************************/
	    // OCX 000～199
	    //public const uint IVS_OCX_CU_XML_INVALID                 = 999115000;     // CU给OCX的查询XML非法;
	    //public const uint IVS_OCX_ALLOC_MEMORY_ERROR             = 999115001;     // CU内存申请失败;
	    //public const uint IVS_OCX_INTRINSIC_PTR_ERROR            = 999115002;     // CU内部指针错误;
	    public const uint IVS_OCX_SDK_INIT_FAIL                  = 999115003;     // OCX初始化SDK失败;
        //public const uint IVS_OCX_XML_NODE_VALUE_ERROR           = 999115004;     // XML节点值不正确
	    public const uint IVS_OCX_RET_SET_DEVNAME_ERROR          = 999115005;     // 设置设备名称失败
	    public const uint IVS_OCX_RET_VIDEOPANE_IS_PLAYING_ERROR = 999115006;     // 窗格正在播放视频错误
	    public const uint IVS_OCX_RET_GET_ACTIVE_WND_ID_ERROR    = 999115007;     // 获取活动窗格ID失败
	    //public const uint IVS_OCX_RET_GET_WND_POINTER_ERROR      = 999115008;     // 获取活动窗格指针失败
	    //public const uint IVS_OCX_RET_ALREADY_INIT_ERROR         = 999115009;     // 重复初始化
	    public const uint IVS_OCX_RET_RESET_PANE_INFO_ERROR      = 999115010;     // 复位窗格信息失败
	    public const uint IVS_OCX_RET_SET_WND_LAYOUT_ERROR       = 999115011;     // 设置窗格布局失败
	    //public const uint IVS_OCX_RET_NO_WRITE_AUTHORITY_ERROR   = 999115012;     // 没有写权限，创建文件夹或抓拍/录像文件失败
	    //public const uint IVS_OCX_RET_VIDEOPANEMGR_NULL          = 999115013;     // 窗格管理类为空
	    //public const uint IVS_OCX_RET_VIDEOPANE_NULL             = 999115014;     // 窗格为空
	    //public const uint IVS_OCX_RET_START_LOG_ERROR            = 999115015;     // 开启日志失败
	    //public const uint IVS_OCX_RET_SETOCX_CTRL_ERROR          = 999115016;     // 设置控件指针失败
	    //public const uint IVS_OCX_RET_INITIAL_VIDEOPANEMGR_ERROR = 999115017;     // 初始化窗格管理类失败
	    //public const uint IVS_OCX_RET_SETOCXLOG_ERROR            = 999115018;     // 设置日志参数失败
	    //public const uint IVS_OCX_RET_PLAYSOUND_WITH_VIDEO_ERROR = 999115019;     // 开启声音失败
	    //public const uint IVS_OCX_RET_STOPSOUND_WITHVIDEO_ERROR  = 999115020;     // 关闭声音失败
	    //public const uint IVS_OCX_RET_ADJUST_WAVE_AUDIO_ERROR    = 999115021;     // 调整音量失败
	    //public const uint IVS_OCX_RET_SET_WAVE_AUDIO_ERROR 	   = 999115022;     // 设置音量失败
	    //public const uint IVS_OCX_RET_SET_PAGENUMBER_ERROR 	   = 999115023;     // 设置页码失败
	    //public const uint IVS_OCX_RET_CAPTURE_HAS_STARTED_ERROR  = 999115024;     // 抓图已开始错误
	    public const uint IVS_OCX_RET_CAPTURE_CREATE_THREAD_ERROR = 999115025;    // 抓图创建线程错误
	    public const uint IVS_OCX_RET_SET_WARN_STATE_ERROR  	   = 999115026;     // 设置警告状态失败
	    //public const uint IVS_OCX_RET_VOICE_TALKBACK_HAS_PLAYED_ERROR     = 999115027;   //语音对讲已经开启
	    //public const uint IVS_OCX_RET_VOICE_TALKBACK_IS_NOT_PLAYING_ERROR = 999115028;   //语音对讲未开启
	    //public const uint IVS_OCX_RET_SET_DISPLAY_SCALE_ERROR       = 999115029;     // 设置窗口显示比例失败
	    public const uint IVS_OCX_RET_VIDEOPANE_ISNOT_PLAYING_ERROR = 999115030;     // 窗格没有播放视频
	    //public const uint IVS_OCX_RET_PARSE_XML_FAIL                = 999115031;     // 解析xml失败
	    //public const uint IVS_OCX_RET_VIDEOPANE_ISNOT_RECORD_ERROR  = 999115032;     // 窗格没有在录像
	    //public const uint IVS_OCX_RET_IS_ALARM_WINDOW_ERROR       = 999115033;    // 窗格为告警窗格
	    //public const uint IVS_OCX_RET_VIDEOPANE_ISNOT_SOUND_ERROR = 999115034;    // 窗格没有在打开声音
	    public const uint IVS_OCX_RET_VIDEOPANE_ISNOT_ALARM_ERROR = 999115035;    // 窗格不是告警窗格
	    //public const uint IVS_OCX_RET_PLAYBACK_FILENAME_ERROR    = 999115036;    // 回放文件名错误
	    //public const uint IVS_OCX_RET_PTZ_NO_RIGHT_EXCEPTION     = 999115037;    // 无云台权限异常
	    public const uint IVS_OCX_RET_GET_INFO_ERROR             = 999115038;    // 获取信息失败
	    //public const uint IVS_OCX_FILE_CREATE_ERROR              = 999115039;    // OCX本地文件创建失败
	    //public const uint IVS_OCX_FILE_NOT_EXIST                 = 999115040;    // OCX本地文件不存在
        public const uint IVS_OCX_RET_SKIN_INIT_ERROR            = 999115041;    // OCX初始化皮肤失败
        public const uint IVS_OCX_RET_FIND_PLAY_HANDLE_ERROR     = 999115042;    // OCX不存在该播放句柄
	    //public const uint IVS_OCX_RET_REALTIMEBAR_ERROR_SOUND    = 999115043;    // 实时声音错误
        public const uint IVS_OCX_RET_NO_PLAY_HANDLE             = 999115044;    // 没有播放句柄
	    //public const uint IVS_OCX_LOGO_XML_ERROR                 = 999115045;     // logoxml获取失败
	    //public const uint IVS_OCX_EVENT_NO_CATCH                 = 999115046;     // 未截获事件

	    // SDK 200～399
	    public const uint IVS_SDK_RET_INIT_RTSPCLIENT_ERROR      = 999115200;    // 初始化RtspClient库失败;
	    //public const uint IVS_SDK_RET_MAIN_SERVICE_INIT_FAIL     = 999115301;    // SDK调用BP创建服务失败;
	    //public const uint IVS_SDK_RET_INIT_THREA_FAIL            = 999115302;    // SDK初始化主线程失败;
	    //public const uint IVS_SDK_RET_INIT_LOG_FAIL              = 999115303;    // SDK初始化LOG服务失败;
	    //public const uint IVS_SDK_RET_SMU_SDK_XML_INVALD         = 999115304;    // SMU给SDK的响应XML非法;
	    public const uint IVS_SDK_RET_BP_CMD_REQ_INVALID         = 999115305;    // SDK调用BP创建命令非法;
	    public const uint IVS_SDK_RET_BP_CMD_RSP_INVALID         = 999115306;    // SDK调用BP应答命令非法;
	    //public const uint IVS_SDK_RET_BP_CMD_RSP_XML_ERROR       = 999115307;    // SDK调用BP应答命令XML非法;
	    //public const uint IVS_SDK_RET_INTRINSIC_PTR_ERROR        = 999115308;    // SDK内部指针错误;
	    //public const uint IVS_SDK_RET_ALLOC_MEMORY_ERROR         = 999115309;    // SDK内存申请失败;
	    //public const uint IVS_SDK_RET_MEMORY_CHECK_ERROR         = 999115310;    // SDK内存校验失败;
	    //public const uint IVS_SDK_RET_INIT_ERROR                 = 999115311;    // SDK初始化失败;
        public const uint IVS_SDK_RET_INVALID_SESSION_ID         = 999115312;    // 无效的SessionID;
	    public const uint IVS_SDK_ERR_FINDVIDEOHANDLE_FAILED     = 999115313;    // 获取视频句柄失败
	    public const uint IVS_SDK_ERR_ERR_MAX_USERNUM            = 999115314;    // 达到最大用用户
        //public const uint IVS_SDK_ERR_BUFFERSIZE_NOT_ENOUGH      = 999115315;    // 表示外部分配内存不足(可能是ocx也可能是第三方)
        //public const uint IVS_SDK_ERR_FINDXMLNODE_FAILED         = 999115316;    // 无法找到xml中相应相应节点;
	    //public const uint IVS_SDK_ERR_WRITEFILE_FAILED           = 999115317;    // 写文件时发生错误;
	    //public const uint IVS_SDK_ERR_OPENFILE_FAILED            = 999115318;    // 打开zip文件时发生错误;
	    public const uint IVS_SDK_ERR_CHEACKMD5_FAILED           = 999115319;    // 检测zip文件时发生错误;
	    public const uint IVS_SDK_ERR_DELETEZIPFILE_FAILED       = 999115320;    // 删除zip时发生错误;
	    public const uint IVS_SDK_ERR_CREATEZIPFILE_FAILED       = 999115321;    // 创建zip时发生错误;
        public const uint IVS_SDK_ERR_DOMAINCODE_NOT_EXIST       = 999115322;    // 域编码不存在
	    //public const uint IVS_SDK_ERR_GET_LOGO_FAILED            = 999115323;    // 获取logo失败
	    //public const uint IVS_SDK_ERR_FILE_NOT_EXIST             = 999115324;    // 获取logo失败
    
        //public const uint IVS_SDK_PLAYBACK_ERR_RSP_GETURL        = 999119200;    // 获取URL响应信息异常
        //public const uint IVS_SDK_PLAYBACK_ERR_STARTTIME_ZERO    = 999119201;    // 回放开始时间为0异常
        //public const uint IVS_SDK_PLAYBACK_ERR_PORT_NOTEXIST_FAIL_FORSTOP = 999119202;   // 停止回放port口不存在
        //public const uint IVS_SDK_PLAYBACK_ERR_PLAYHANDLE        = 999119203;    // 回放handle不合法
        //public const uint IVS_SDK_PLAYBACK_ERR_PLAT_SPEED_PARM   = 999119204;    // 平台回放设置速度参数不合法
        //public const uint IVS_SDK_PLAYBACK_ERR_PU_SPEED_PARM     = 999119205;    // 前端回放设置速度参数不合法
        //public const uint IVS_SDK_PLAYBACK_ERR_GET_REMOTE_STATUS = 999119206;    // 获取远端播放状态失败
        //public const uint IVS_SDK_PLAYBACK_ERR_GET_REMOTE_SPEED  = 999119207;    // 获取远端播放速度失败
        //public const uint IVS_SDK_PLAYBACK_ERR_GET_REMOTE_TIME   = 999119208;    // 获取远端播放时间失败
        //public const uint IVS_SDK_PLAYBACK_ERR_STATUS_DOUDLE_STOP   = 999119209;     // 重复设置暂停状态
        //public const uint IVS_SDK_PLAYBACK_ERR_STATUS_DOUDLE_RESUME = 999119210;     // 重复设置恢复状态
        public const uint IVS_SDK_PLAYBACK_ERR_GET_REMOTE_DATAINFO  = 999119211;     // 获取远端播放信息失败
        //public const uint IVS_SDK_PLAYBACK_ERR_TIMESPAN             = 999119212;     // 回放timespan不合法
        //public const uint IVS_SDK_PLAYBACK_ERR_GET_LOCAL_DATAINFO   = 999119213;     // 获取本地播放信息失败
        //public const uint IVS_SDK_PLAYBACK_ERR_LOCAL_RECORDING      = 999119214;     // 此窗格正在进行录制，开始本地录制失败
        //public const uint IVS_SDK_PLAYBACK_ERR_NOT_LOCAL_RECORDING  = 999119215;     // 没有窗格正在进行录制，停止本地录制失败
	    //public const uint IVS_SDK_RET_SMU_SERVER_NOT_RESPOND     = 999119216;     // SMU消息返回超时
	    //public const uint IVS_SDK_RET_SCU_SERVER_NOT_RESPOND     = 999119217;     // SCU消息返回超时
	    //public const uint IVS_SDK_RET_OMU_SERVER_NOT_RESPOND     = 999119218;     // OMU消息返回超时

        /*对讲错误码*/
        public const uint IVS_SDK_ERR_FINDAUDIOHANDLE_FAILED     = 999119219;       // 获取对讲句柄失败
        public const uint IVS_SDK_TALKBACK_ERR_ALREADY_OPENED    = 999119220;       // 已经开启一路对讲或广播
        public const uint IVS_SDK_TALKBACK_ERR_ALREADY_CLOSED    = 999119221;       // 尚未开启
        public const uint IVS_SDK_BROADCASE_ERR_NO_DEVICE        = 999119222;       // 设备组中没有设备
        public const uint IVS_SDK_BROADCASE_ERR_ALREADY_OPENED   = 999119223;       // 广播已经开启
        public const uint IVS_SDK_BROADCASE_ERR_NOT_OPENED       = 999119224;       // 广播没有开启
        public const uint IVS_SDK_VOICE_ERR_INVALID_USEROBJ      = 999119225;       // 用户对象为空，无法获取用户信息
        public const uint IVS_SDK_VOICE_ERR_INVALID_USERINFO     = 999119226;       // 用户信息为空
        public const uint IVS_SDK_VOICE_ERR_GET_URL_FAIL         = 999119227;       // 获取URL失败
        public const uint IVS_SDK_VOICE_ERR_SET_IP_FAIL          = 999119228;       // 设置接收IP失败
        public const uint IVS_SDK_VOICE_ERR_CALL_MLIB_FAIL       = 999119229;       // 调用Mlib层接口返回失败
        public const uint IVS_SDK_VOICE_ERR_DEV_ALREADY_IN       = 999119230;       // 设备已加入设备组
        public const uint IVS_SDK_VOICE_ERR_DEV_NOT_IN           = 999119231;       // 设备组中没有该设备
        public const uint IVS_SDK_VOICE_ERR_STOP_BROADCAST_FAIL  = 999119232;       // 删除设备时停止对应设备的广播失败
        public const uint IVS_SDK_VOICE_ERR_TOO_MUCH_DEVICES     = 999119233;       // 广播组中的设备数超过最大值256
        public const uint IVS_SDK_VOICE_ERR_GET_HANDLE_FAIL      = 999119234;       // 根据设备ID查找广播handle失败
        public const uint IVS_SDK_VOICE_ERR_FILE_NOT_EXIST       = 999119235;       // 文件不存在
        public const uint IVS_SDK_VOICE_ERR_FILE_NOT_RIGHT_TYPE  = 999119236;       // 不是wav格式的音频文件
        public const uint IVS_SDK_VOICE_ERR_CANNOT_REPORT_FAIL   = 999119237;       // 获取用户对象的回调函数失败
        public const uint IVS_SDK_VOICE_ERR_ALL_BROADCAST_FAIL   = 999119238;       // 广播开启\关闭全部失败
        public const uint IVS_SDK_VOICE_ERR_AUDIO_TYPE_CONFLICT  = 999119239;       // 音频格式和之前的镜头冲突

	    // MP 400～599
        public const uint IVS_PLAYER_RET_GET_CHANNEL_ERROR       = 999115400;     // 获取通道失败;
        public const uint IVS_PLAYER_RET_ALL_PORT_BUSY_ERROR     = 999115401;     // 所有端口均被占用;
        public const uint IVS_PLAYER_RET_WSA_START_ERROR         = 999115402;     // 启用SOCKET的DLL库错误;
        public const uint IVS_PLAYER_RET_CONNECT_SOCKET_ERROR    = 999115403;     // 建立socket连接失败;
	    public const uint IVS_PLAYER_RET_ALLOC_MEM_ERROR         = 999115404;     // 缓存分配失败;
	    public const uint IVS_PLAYER_RET_RECV_DATA_ERROR         = 999115405;     // 数据流错误;
        public const uint IVS_PLAYER_RET_RECV_DATA_TIMEOUT       = 999115406;     // 连接超时;
 	    public const uint IVS_PLAYER_RET_CREATE_THREAD_ERROR     = 999115407;     // 创建线程失败;
	    public const uint IVS_PLAYER_RET_CREATE_DECODER_ERROR    = 999115408;     // 创建解码库失败;
	    public const uint IVS_PLAYER_RET_ORDER_ERROR             = 999115409;     // 调用顺序不对;
	    public const uint IVS_PLAYER_RET_DECODER_ERROR           = 999115410;     // 解码失败;
	    public const uint IVS_PLAYER_RET_WATER_MARK_OK           = 999115411;     // 水印数据正常;
	    public const uint IVS_PLAYER_RET_WATER_MARK_ERROR        = 999115412;     // 水印数据被篡改;
	    public const uint IVS_PLAYER_RET_UNSUPPORT_OPERATION     = 999115413;     // 当前版本不支持此操作;
	    public const uint IVS_PLAYER_RET_DECODER_BUFFER_FULL     = 999115414;     // 缓冲区满;

        public const uint IVS_PLAYER_RET_RTSP_GET_HANDLE_FAIL    = 999115415;     // 获取rtsp的handle失败;
        public const uint IVS_PLAYER_RET_RTSP_SEND_SETUP_FAIL    = 999115416;     // 发送rtsp的Setup请求失败;
        public const uint IVS_PLAYER_RET_RTSP_SEND_PLAY_FAIL     = 999115417;     // 发送rtsp的Play请求失败;
        public const uint IVS_PLAYER_RET_RTSP_SEND_PAUSE_FAIL    = 999115418;     // 发送rtsp的Pause请求失败;
        public const uint IVS_PLAYER_RET_RTSP_SEND_TEARDOWN_FAIL = 999115419;     // 发送rtsp的Teardown请求失败;
        public const uint IVS_PLAYER_RET_RTSP_NORMAL_STOP        = 999115420;     // RTSP连接收到正常结束
        public const uint IVS_PLAYER_RET_RTSP_ABNORMAL_STOP      = 999115421;     // RTSP连接收到异常结束;
        public const uint IVS_PLAYER_RET_RTSP_DATA_ERROR         = 999115422;     // RTSP码流不符合要求;
	    public const uint IVS_PLAYER_RET_RTSP_DISCONNECT         = 999115423;     // RTSP码流断开;
        public const uint IVS_PLAYER_RET_NET_OPEN_STREAM_FAIL    = 999115424;     // 网络库打开网络流失败;
        public const uint IVS_PLAYER_RET_MP_OPEN_STREAM_FAIL     = 999115425;     // 播放库打开网络流失败;
        public const uint IVS_PLAYER_RET_MP_START_PLAY_FAIL      = 999115426;     // 播放库启动播放失败;
        public const uint IVS_PLAYER_RET_NET_START_RECEIVE_FAIL  = 999115427;     // 网络库启动收流失败;
        public const uint IVS_PLAYER_RET_INIT_NETSOURCE_ERROR    = 999115428;     // 初始化网络收流库失败;
        public const uint IVS_PLAYER_RET_INIT_PLAY_ERROR         = 999115429;     // 初始化播放收流库失败;
	    public const uint IVS_PLAYER_RET_FILE_ACCESS_ERROR       = 999115430;     // 访问文件或目录错误;
	    public const uint IVS_PLAYER_RET_DISK_SPACE_ERROR        = 999115431;     // 磁盘空间满错误;
        public const uint IVS_PLAYER_RET_MIXER_OPEN_STATE_ERROR  = 999115432;     // 混音器打开状态错误
        public const uint IVS_PLAYER_RET_MIXER_ERROR             = 999115433;     // 混音器操作错误
        public const uint IVS_PLAYER_RET_VOICE_TALKBACK_STATUS_ERROR = 999115434; // 语音对讲状态失败
        public const uint IVS_PLAYER_RET_BROADCAST_FILE_IS_END   = 999115435;     // 语音广播文件播放结束
        public const uint IVS_PLAYER_RET_BROADCAST_NOT_SUPPORTED_FORMAT = 999115436; // 语音广播传入的文件格式不支持
        public const uint IVS_PLAYER_RET_BROADCAST_OPEN_FILE_ERROR = 999115437;   // 语音广播读文件失败
        public const uint IVS_PLAYER_RET_GET_STREAM_OPEN_MODE_ERROR = 999115438;  // 获取数据流打开模式失败
        public const uint IVS_PLAYER_RET_GET_AUDIO_VOLUMN_ERROR  = 999115439;     // 获取音量失败
        public const uint IVS_PLAYER_RET_WAVE_OUT_ERROR          = 999115440;     // 打开音频输出失败
        public const uint IVS_PLAYER_RET_CAPTURE_OPEN_STATE_ERROR = 999115441;    // 音频收集状态错误
        public const uint IVS_PLAYER_RET_WAVE_IN_ERROR           =  999115442;    // 打开音频输入失败
        public const uint IVS_PLAYER_RET_AUDIO_DEVICE_NOT_FIND   =  999115443;    // 未找到音频设备
        public const uint IVS_PLAYER_RET_MEDIA_TYPE_UNSUPPORT    =  999115444;    // 不支持该媒体类型
        public const uint IVS_PLAYER_RET_VOICE_BROADCAST_STATUS_ERROR = 999115445;// 语音广播状态失败
        public const uint IVS_PLAYER_RET_CHANNEL_NOT_EXIST_ERROR = 999115446;     // 通道不存在

	    public const uint IVS_PLAYER_RET_RTSP_ERR_NOT_EXIST      = 999115447;     // 连接句柄不存在
	    public const uint IVS_PLAYER_RET_RTSP_ERR_DISCONNECT     = 999115448;     // 连接失败
	    public const uint IVS_PLAYER_RET_RTSP_ERR_RESPONSE       = 999115449;     // 响应消息错误
	    public const uint IVS_PLAYER_RET_RTSP_ERR_TIMEOUT        = 999115450;     // 响应超时
	    public const uint IVS_PLAYER_RET_RTSP_ERR_STAUTS_ABNORMAL= 999115451;     // 连接状态异常
	    public const uint IVS_PLAYER_RET_RTSP_ERR_SEND_FAIL      = 999115452;     // 发送出错
	    public const uint IVS_PLAYER_RET_RTSP_ERR_RECV_FAIL      = 999115453;     // 接收出错
	    public const uint IVS_PLAYER_RET_RTSP_ERR_PARAMETER      = 999115454;     // 参数检查出错
	    public const uint IVS_PLAYER_RET_RTSP_ERR_NORMAL_STOP    = 999115455;     // 正常结束，ANNOUNCE正常结束请求
	    public const uint IVS_PLAYER_RET_RTSP_ERR_ABNORMAL_STOP  = 999115456;     // 异常结束，ANNOUNCE异常结束请求
        public const uint IVS_PLAYER_RET_RTSP_ERR_RET_SYS        = 999115457;     // 通用系统错误

    }
}
