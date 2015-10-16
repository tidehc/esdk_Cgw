/********************************************************************
filename    :    ivs_error.h
author      :    z00189721
created     :    2012/11/13
description :    ivs 错误码定义
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2012/11/13 初始版本
*********************************************************************/

#ifndef _IVS_ERROR_H_
#define _IVS_ERROR_H_

/************************************************************
*               IVS平台错误码总体定义规则
*  模块编码（2位）+ 91 + 功能编码（2位）+ 错误编码（3位）
*************************************************************
*  其中模块编码详见错误码枚举定义中的模块注释。
*  功能编码定义如下：
*      功能                       功能编码
*  1、登录	                        01
*  2、主界面	                    02
*  3、组织管理	                    03
*  4、角色管理	                    04
*  5、用户管理	                    05
*  6、用户管理-例外设备	            06
*  7、业务节点管理	                07
*  8、设备管理-主设备	            08
*  9、摄像机子设备	                09
*  10、开关量输入子设备	            10
*  11、设备管理-开关量输出子设备	11
*  12、设备管理-语音子设备	        12
*  13、设备管理-串口子设备	        13
*  14、设备划归	                    14
*  15、实况	                        15
*  16、云镜控制	                    16
*  17、实况-预置位	                17
*  18、实况-巡航轨迹	            18
*  19、录像回放	                    19
*  20、告警管理	                    20
*  21、外部告警管理	                21
*  22、巡航设置	                    22
*  23、告警设置	                    23
*  24、录像空间设置	                24
*  25、录像计划配置	                25
*  26、修改密码	                    26
*  27、域管理	                    27
*  28、存储服务器管理	            28
*  29、设备更新	                    29
*  30、日志管理	                    30
*  31、系统维护	                    31
*  32、License管理	                32
*  33、智能分析计划/规则管理	    33
*  34、NVR管理	                    34
*  35、防区管理	                    35
*  36、防区计划管理	                36
*  37、设备预注册	                37
*  38、DCG管理	                    38
*  39、外部系统管理	                39
*  40、前端录像	                    43
*  41、前端录像计划	                44
*  42、语音广播	                    45
*  43、抓拍管理	                    46
*
*  99、BP通用功能	                99
*****************************************************/

// ivs 错误码定义
typedef enum enIvsError
{
	/************************************************************
	 *   系统通用错误码定义
	 ***********************************************************/
	IVS_SUCCEED                            = 0,           // 成功
    IVS_FAIL                               = 1,           // 失败
    IVS_PARA_INVALID                       = 2,           // 输入参数非法

	/***********************************************************
	 *   系统启动、内存、数据库等通用错误码定义（模块编码：10）
	 ***********************************************************/
	IVS_PASSWORD_ERROR                     = 109100000,   // 用户名密码错误，登录时输入的用户名或密码错误
    IVS_USER_NOT_EXIST                     = 109100001,   // 用户不存在，注册的用户ID已注销或不可用
    IVS_NO_ENOUGH_RIGHT                    = 109100002,   // 权限不足，无此操作权限
    IVS_NOT_INIT                           = 109100003,   // 未初始化
    IVS_ALLOC_MEMORY_ERROR                 = 109100004,   // 分配内存失败
    IVS_OPERATE_MEMORY_ERROR               = 109100005,   // 操作内存失败
    IVS_NO_ENOUGH_BUF                      = 109100006,   // 缓冲区太小，接收数据的缓冲区或存放数据的缓冲区不足
    IVS_SYS_NO_SUPPORT                     = 109100007,   // 系统不支持
    IVS_INIT_DB_ERROR                      = 109100008,   // 初始化数据库失败
    IVS_CONNECT_DB_ERROR                   = 109100009,   // 连接数据库失败（用于获取数据库连接失败、数据库连接指针为NULL）
    IVS_OPERATE_DB_ERROR                   = 109100010,   // 数据库操作失败（用于各种数据库操作失败、数据库中内容不存在、数据库结果集指针为空等）
    IVS_DIR_ERROR                          = 109100011,   // 路径错误
    IVS_CREATE_DIR_ERROR                   = 109100012,   // 创建文件目录失败
    IVS_FILE_FORMAT_ERROR                  = 109100013,   // 文件格式不正确
    IVS_CREATE_FILE_ERROR                  = 109100014,   // 创建文件失败
    IVS_OPEN_FILE_ERROR                    = 109100015,   // 打开文件失败
    IVS_WRITE_FILE_ERROR                   = 109100016,   // 写文件失败
    IVS_DISK_FULL                          = 109100017,   // 硬盘空间满
    IVS_DISK_ERROR                         = 109100018,   // 硬盘出错
	IVS_NET_CONNECT_ERROR                  = 109100019,   // 网络连接失败,服务器不在线或者网络原因引起连接超时等
    IVS_NET_OVER_MAXLINK                   = 109100020,   // 连接数超过最大限制
    IVS_NET_SEND_ERROR                     = 109100021,   // 网络发送数据失败
    IVS_NET_RECV_ERROR                     = 109100022,   // 网络接收数据失败
    IVS_NET_RECV_TIMEOUT                   = 109100023,   // 网络接收数据超时
    IVS_NET_DATA_ERROR                     = 109100024,   // 网络传送的数据有误
    IVS_JOIN_MULTICAST_ERROR               = 109100025,   // 加入多播组失败
    IVS_CREATE_THREAD_ERROR                = 109100026,   // 创建线程失败
    IVS_XML_INVALID                        = 109100027,   // XML消息体非法(用于XML消息体为NULL、XML解析失败)
    IVS_XML_PACK_ERROR    		           = 109100028,   // XML组装失败
    IVS_XML_DEV_TEMPLATE_INVALID           = 109100029,   // 设备模板XML非法
    IVS_SECURITY_INIT_ERROR		           = 109100030,   // 安全操作类初始化失败
    IVS_BATCH_OPERATE_ALL_ERROR            = 109100031,   // 批量操作全部失败
    IVS_BATCH_OPERATE_PART_SUCCEED         = 109100032,   // 批量操作部分成功
    IVS_MU_OFF_LINE			               = 109100033,	  // MU不在线
    IVS_CMU_OFF_LINE	                   = 109100034,	  // CMU不在线
    IVS_DCG_OFF_LINE	                   = 109100035,	  // DCG不在线
    IVS_OMU_OFF_LINE	                   = 109100036,	  // OMU不在线
    IVS_SMU_OFF_LINE                       = 109100037,   // SMU不在线
	IVS_SCU_OFF_LINE                       = 109100038,   // SCU不在线
    IVS_MULM_OFF_LINE	                   = 109100039,	  // MULM不在线
    IVS_GET_LOCALIP_ERROR                  = 109100040,   // 获取本地IP失败
    IVS_SET_LOCALIP_ERROR                  = 109100041,   // 设置本地IP失败
    IVS_SCRIPT_RUN_ERROR                   = 109100042,   // 脚本执行出错
    // 参数校验类错误
    IVS_USERNAME_INVALID                   = 109100043,   // 用户名长度非法或包含非法字符
    IVS_PASSWORD_INVALID                   = 109100044,   // 用户密码长度非法或包含非法字符
    IVS_NAME_INVALID                       = 109100045,   // 名称长度非法或包含非法字符（用于设备名称，计划名称等）
    IVS_DESCRIBE_INVALID                   = 109100046,   // 描述长度非法或包含非法字符
    IVS_CODE_INVALID                       = 109100047,   // 编码长度非法或包含非法字符（用于设备编码，域编码等）
    IVS_IP_INVALID                         = 109100048,   // IP地址非法
    IVS_PORT_INVALID                       = 109100049,   // 端口非法
    IVS_TIME_INVALID                       = 109100050,   // 时间非法
    IVS_EMAIL_INVALID                      = 109100051,   // e-mail非法
    IVS_TELEPHONE_INVALID                  = 109100052,   // 电话非法
    IVS_DATA_INVALID                       = 109100053,   // 数值非法或超过取值范围
	IVS_MAU_OFF_LINE                       = 109100054,   // MAU不在线
	IVS_URL_INVALID                        = 109100055,   // URL非法
	IVS_DOWNLOAD_ERROR                     = 109100056,   // 下载失败
    IVS_DOMAIN_TYPE_ERROR                  = 109100057,   // 域模式错误
	IVS_DATA_ENCRYPT_ERROR                 = 109100058,   // 数据加密失败
	IVS_DATA_DECRYPT_ERROR                 = 109100059,   // 数据解密失败
    IVS_IP_INVALID_SAMEAS_PHYSICAL_IP      = 109100060,   // IP地址非法:浮动ip和物理ip相同
    IVS_IP_INVALID_BEING_USED              = 109100061,   // IP地址非法:浮动ip地址冲突
    IVS_IP_INVALID_NETWORK_SEGMENT         = 109100062,   // IP地址非法:浮动ip非同网段IP
    IVS_NOT_SUPPORT_MSG                    = 109100063,   // 不支持的消息类型
    IVS_GET_RECORD_LIST_TIMEOUT            = 109100064,   // 查询录像超时，请缩小时间范围 
	IVS_LINK_NOT_AUTH                      = 109100065,   // 连接为鉴权
    IVS_PASSWORD_WEAK                      = 109100066,   // 输入的密码属于弱口令，请重新输入
	
	/**************************************************************
	 *   SMU错误码定义（模块编码：11）
	 *************************************************************/
	IVS_SMU_XML_INVALID                    = 119108001,   // XML数据不合法
    IVS_SMU_DEVICE_TIMEOUT                 = 119108002,   // 处理设备管理业务超时
	IVS_SMU_GETURL_TIMEOUT                 = 119108003,   // 获取geturl超时
	IVS_SMU_RECORDINQUIRY_TIMEOUT          = 119108004,   // 录像查询超时
    IVS_SMU_DEVICE_EXIST                   = 119108005,   // 设备已存在
	IVS_SMU_DEVICE_NOTEXIST				   = 119108006,   // 设备不存在
	IVS_SMU_DEVICE_UPDATEMAP_FAIL		   = 119108007,	  // 更新内存失败
    IVS_SMU_DEVICE_ADDMAP_FAIL             = 119108008,   // 添加内存数据失败
	IVS_SMU_PTZ_CONTROL_TIMEOUT            = 119108009,   // 云镜控制超时
	IVS_SMU_ROUTE_NVR_FAIL                 = 119108010,   // 路由nvr失败
    IVS_DB_RESULT_SET_NULL_POINTER		   = 119108011,   // 数据库结果集指针为空
    IVS_SMU_CMD_XML_IS_NULL                = 119108012,   // cmd中的xml指针为空
	IVS_SMU_DR_RELATON_TIMEOUT             = 119108013,   // 容灾备份操作超时
    IVS_SMU_OPERATE_TIME_OUT               = 119108014,   // 操作超时
	IVS_SMU_MD_DATA_TIME_OUT               = 119108015,   // 查询移动侦测数据超时
    IVS_SMU_LOAD_SQL_ERROR                 = 119108016,   //SMU加载数据库语句失败
	IVS_SMU_SQL_DATA_NULL                  = 119108017,   //SMU查询数据库为空

    /* SMU:设备管理-【主设备+通用】错误码：119108XXX */
    IVS_SMU_DEV_TIMEOUT                    = 119108100,   // 处理设备管理业务超时
    IVS_SMU_DEV_CMD_INVALID                = 119108101,   // CMD不可用
    IVS_SMU_DEV_REQ_XML_INVALID            = 119108102,   // 所请求的XML非法
    IVS_SMU_DEV_REQ_XML_GROUPID_NOEXIST    = 119108103,   // 所请求的XML报文GroupId节点不存在
    IVS_SMU_DEV_REQ_XML_GROUPNAME_NOEXIST  = 119108104,   // 所请求的XML报文GroupName节点不存在
    IVS_SMU_DEV_DB_CONNECT_ERROR           = 119108105,   // 数据库连接错误
    IVS_SMU_DEV_DB_EXECUTE_ERROR           = 119108106,   // 数据库操作失败
    IVS_SMU_DEV_GROUPID_NOEXIST            = 119108107,   // 设备组ID不存在
    IVS_SMU_DEV_SAME_GROUPNAME_EXIST       = 119108108,   // 父设备组下存在同名子设置组
    IVS_SMU_DEV_ADD_ALL_FAIL               = 119108109,   // 添加主设备全部失败
    IVS_SMU_DEV_ADD_PART_SUCCEED           = 119108110,   // 添加主设备部分成功
	IVS_SMU_DEV_TYPES_CHANGE_ERROR		   = 119108111,   // 设备类型强制转换错误
    IVS_SMU_DEV_XML_CHANNELNAME_NOEXIST    = 119108112,   // 所请求的XML报文ChannelName节点不存在
    IVS_SMU_DEV_XML_CHANNELTYPE_NOEXIST    = 119108113,   // 所请求的XML报文ChannelType节点不存在
    IVS_SMU_DEV_XML_CHANNELCODE_NOEXIST    = 119108114,   // 所请求的XML报文ChannelCode节点不存在
    IVS_SMU_DEV_CREATE_SUBDEVICE_FAIL      = 119108115,   // 创建子设备失败
    IVS_SMU_DEV_GET_ROUTING_ERROR          = 119108116,   // 获取NVR路由失败
    IVS_SMU_DEV_REQ_XML_TARGROUPID_NOEXIST = 119108117,   // 所请求的XML报文TargetGroup节点不存在
    IVS_SMU_DEV_DB_GROUPID_NOEXIST         = 119108118,   // 数据库GroupId无记录
    IVS_SMU_DEV_GROUP_LEVEL_EXCEED         = 119108119,   // Group Level超出最大值
	IVS_SMU_DEV_ADD_TO_GROUP_ERROR		   = 119108120,	  // 添加设备组下设备失败
    IVS_SMU_DEV_NOEXIST                    = 119108121,   // 设备不存在
	IVS_SMU_DEV_FROM_GRP_ID_ZERO		   = 119108122,	  // 父设备组已经为根组
	IVS_SMU_DEV_PROTOCOL_TYPE_ERROR		   = 119108123,	  // 主设备协议类型错误
	IVS_SMU_DEV_NVRCODE_CANNOT_MOD		   = 119108124,	  // 不可修改设备的nvrcode
    IVS_SMU_DEV_XML_LOGINID_NOEXIST        = 119108125,   // 所请求的XML报文LoginId节点不存在
	IVS_SMU_DEV_CONNOT_DEL				   = 119108126,   // 该设备只在根组下，不能移除
    IVS_SMU_DEV_DELETE_FAIL                = 119108127,   // 删除设备失败
    IVS_SMU_DEV_NVR_ROUTING_OFFLINE        = 119108128,   // NVR不在线
    IVS_SMU_DEV_GROUP_TARGET_SAME_FAIL     = 119108129,   // 与目标设备组相同
    IVS_SMU_DEV_NVR_NOT_EXIST			   = 119108130,	  // nvrcode不存在
    IVS_SMU_DEV_GROUP_TOTAL_EXCEED         = 119108131,   // 设备组总数达到最大值
    IVS_SMU_DEV_GROUP_CHILE_EXCEED         = 119108132,   // 子设备组达到最大值
    IVS_SMU_DEV_GROUP_EXIST_RECU           = 119108133,   // 设备组存在递归
    IVS_SMU_DEV_GROUPNAME_LENGTH_INVALID   = 119108134,   // 设备组长度无效
    IVS_SMU_DEV_GROUPNAME_STRING_INVALID   = 119108135,   // 设备组名称包含非法字符
    IVS_SMU_DEV_XML_PARENRGROUPID_NOEXIST  = 119108136,   // 所请求的PARENRGROUPID节点不存在
    IVS_SMU_DEV_GROUPREMARK_LENGTH_INVALID = 119108137,   // 设备组长度无效
    IVS_SMU_DEV_GROUPREMARK_STRING_INVALID = 119108138,   // 设备组名称包含非法字符
    IVS_SMU_DEV_BUILD_DEVICE_CODE_FAIL     = 119108139,   // 生成设备编码失败
	IVS_SMU_DEV_PART_NOEXIST               = 119108140,   // 设备列表中部分设备不存在
    IVS_SMU_DEV_DR_TABLE_IS_EMPTY          = 119108141,   // 容灾关系表为空
	IVS_SMU_DEVICE_IP_EXIST                = 119108142,   // 设备IP已存在
    IVS_SMU_ONLY_SUPPORT_LOCAL_DOAMIN      = 119108143,   // 只支持本域设备
    IVS_SMU_QD_NO_LICENSE                  = 119108144,   // 视频质量诊断无license
    IVS_SMU_QD_NO_SUPPORT_SYSTEM           = 119108145,   // 视频质量诊断不支持的系统(VCN500不支持)

    /* SMU:设备管理-【摄像机子设备】错误码：119109XXX */


    /* SMU:设备管理-【摄像机子设备】错误码：119109XXX */

	/* SMU:用户登录Service错误码，取值范围：119108300 ~ 119108399 */
    IVS_SMU_USER_EXPIRED         	       = 119101302,   // 账号已经过期
    IVS_SMU_USER_STOPPED         	       = 119101303,   // 用户被停用
    IVS_SMU_USER_LOCKED          	       = 119101304,   // 用户被锁定
    IVS_SMU_USER_PWD_NEED_MOD     	       = 119101305,   // 用户密码已过期，需要修改
    IVS_SMU_EXCEED_SAME_USER_ONLINE_MAXNUM = 119101306,   // 超过同一账户同时在线的最大用户个数
    IVS_SMU_EXCEED_ONLINE_USER_MAXNUM      = 119101307,   // 同时在线总用户数超过系统支持的最大数量
    IVS_SMU_USER_FIRST_LOGIN     	       = 119101308,   // 用户首次登录需要修改密码
    IVS_SMU_USER_OFF_LINE        	       = 119101309,   // 用户已离线
    IVS_SMU_BASIC_LOGIN_NOT_NEED_DOMAIN_NAME = 119101310, // 基本身份登录不能带域名信息
    IVS_SMU_DOMAIN_LOGIN_NEED_DOMAIN_NAME  = 119101311,   // 域身份登录必须带域名信息
    IVS_SMU_NO_MATCH_ACC_RULE              = 119101312,   // 没找到账户匹配的角色规则信息
    IVS_SMU_NO_MATCH_OPER_RIGHT            = 119101313,   // 没找到账户匹配的操作权限信息
    IVS_SMU_LOGIN_WITH_EXIST_LINKID        = 119101314,   // 不能使用旧的TCP连接重复发送登录请求
    IVS_SMU_VERIFY_WITH_DIFFERENT_LINKID   = 119101315,   // 认证请求所使用的TCP连接与在线用户的TCP连接不同
    IVS_SMU_PWD_IS_EMPTY                   = 119101316,   // 密码为空
    IVS_SMU_CU_IP_IS_ILLEGAL               = 119101317,   // 登录Xml的CU IP非法
    IVS_SMU_CU_PORT_IS_ILLEGAL             = 119101318,   // 登录Xml的CU端口非法
    IVS_SMU_LOGIN_TYPE_ILLEGAL             = 119101319,   // Login_Type非法
    IVS_SMU_LDAP_INIT_FAIL                 = 119101320,   // 域账户验证时，LDAP协议初始化失败
    IVS_SMU_LDAP_SEARCH_ACCOUNT_FAIL       = 119101321,   // 在域服务器上查找账户失败
    IVS_SMU_MACHINE_NAME_ILLEGAL           = 119101322,   // 机器名非法
    IVS_SMU_CLIENT_TYPE_ILLEGAL            = 119101323,   // 客户端类型非法
    IVS_SMU_SERVER_IP_IS_ILLEGAL           = 119101324,   // 登录Xml的服务器IP非法
    IVS_SMU_SERVER_PORT_IS_ILLEGAL         = 119101325,   // 登录Xml的服务器端口非法
    IVS_SMU_DOMAIN_LOGIN_SERVER_DISCONNECT = 119101326,   // 域账号服务器不在工作
    IVS_SMU_CAN_NOT_SIGNOUT_USER_SELF      = 119101327,   // 不能签退自己
    IVS_SMU_RIGHT_LEN_NO_MATCH             = 119101328,   // 权限长度不匹配
    IVS_SMU_ENCRYPT_FAIL                   = 119101329,   // 加密失败
    IVS_SMU_DECRYPT_FAIL                   = 119101331,   // 解密失败
    IVS_SMU_REQUEST_PUBLIC_KEY_FAIL        = 119101332,   // 请求公钥失败
    IVS_SMU_REQUEST_EXCHANGE_KEY_FAIL      = 119101333,   // 交换密钥失败
    IVS_SMU_SECURITY_CLIENT_INIT_FAIL      = 119101334,   // 安全Client初始化失败
    IVS_SMU_WAIT_ASYNC_RESPONSE            = 119101340,   // 等待异步响应
	IVS_NULL_POINTER			 	       = 119101350,   // 服务器内部错误，空指针异常;
    IVS_SMU_LINKID_FORMAT_ERROR            = 119101351,   // LinkID格式错误
    IVS_SMU_IP_LOCKED          	           = 119101352,   // 用户IP被锁定
    IVS_SMU_HAS_RESTARED        	       = 119101353,   // SMU已重启，原在线用户已不存在，用于sdk重连smu
    IVS_SMU_DNSCONFIG_NOT_SET              = 119101354,   //域服务器未配置
    IVS_SMU_USER_NOT_IN_VALID_TIME         = 119101355,   //用户不在有效期中
    IVS_SMU_DOMAIN_LOGIN_NOT_SUPPORT       = 119101356,   //不支持域登录，当前VCN500不支持

    IVS_SMU_ADD_GROUP_ERROR                = 119105000,   // 新增用户组失败
    IVS_SMU_MODIFY_GROUP_ERROR             = 119105001,   // 修改用户组失败
    IVS_SMU_DELETE_GROUP_ERROR             = 119105002,   // 删除用户组失败
    IVS_SMU_MOVE_GROUP_ERROR               = 119105003,   // 移动用户组失败
    IVS_SMU_ADD_GROUP_HAVE_REPEAT_GROUP    = 119105004,   // 增加用户组时重复
    IVS_SMU_ADD_GROUP_PARENT_NOT_EXIST     = 119105005,   // 父节点不存在
    IVS_SMU_ADD_GROUP_PARENT_LEVEL_INVALID = 119105006,   // 父节点层级非法
    IVS_SMU_MOIDFY_GROUP_GROUP_NOT_EXIST   = 119105007,   // 被修改的用户组不存在
    IVS_SMU_MOIDFY_GROUP_HAVE_REPEAT_GROUP = 119105008,   // 修改用户组后重命名
    IVS_SMU_MOVE_GROUP_TARGET_NOT_EXIST    = 119105009,   // 移向的目标不存在
    IVS_SMU_MOVE_GROUP_TARGET_LEVEL_INVALID= 119105010,   // 移动后会超过3级
    IVS_SMU_MOVE_GROUP_BEMOVED_NOT_EXIST   = 119105011,   // 被移动的用户组不存在
    IVS_SMU_MOVE_GROUP_HAVE_REPEAT_GROUP   = 119105012,   // 移动后用重命名用户组
    IVS_SMU_DEL_GROUP_GROUP_NOT_EXIST      = 119105013,   // 要被删除的用户组不存在
    IVS_SMU_CAN_NOT_DELETE_ROOT_GROUP      = 119105016,   // 不能删除根组
    IVS_SMU_OVER_MAX_GROUP_NUM             = 119105017,   // 用户组个数超过最大限额
    IVS_SMU_CAN_NOT_MOVE_ROOT_GROUP        = 119105018,   // 根组不能移动
    IVS_SMU_CAN_NOT_MOVE_TO_SELF           = 119105019,   // 组不能移动到自己
    IVS_SMU_GROUP_NUM_OVER_MAX             = 119105020,   // 用户组个数达到上限,超过1000
    IVS_SMU_GROUP_CHILDREN_NUM_OVER_MAX    = 119105021,   // 子用户组个数超过99
    IVS_SMU_PARAM_INVALID_FOR_GROUP        = 119105022,   // 用户组参数错误
    IVS_SMU_RESET_PWD_SWITCH_OFF           = 119105023,   // 重置密码开关关闭
	IVS_SMU_CAN_NOT_DEL_USER_IN_SELFGRP    = 119105024,   // 不能删除自己所在的用户组
    IVS_SMU_CAN_NOT_DEL_ROLE_IN_SELF       = 119105025,   // 不能删除自己的角色
    IVS_SMU_USER_GROUP_DESC_LENGTH_INVALID = 119105026,   // 用户组备注长度非法

    IVS_SMU_ADD_ROLE_ERROR                 = 119105100,   // 新增角色失败
    IVS_SMU_MODIFY_ROLE_ERROR              = 119105101,   // 修改角色失败
    IVS_SMU_DELETE_ROLE_ERROR              = 119105102,   // 删除角色失败
    IVS_SMU_GET_ROLE_LIST_ERROR            = 119105103,   // 查询角色失败
    IVS_SMU_ACC_PARAM_ERROR                = 119105104,   // 帐号规则参数非法
    IVS_SMU_ADD_ROLE_HAVE_REPEAT_ROLE      = 119105105,   // 角色重命名
    IVS_SMU_DELETE_ROLE_ROLE_NOT_EXIST     = 119105106,   // 删除的角色不存在
    IVS_SMU_DELETE_DEFAULT_ROLE_ERROR      = 119105107,   // 不能删除默认角色
    IVS_SMU_MODIFY_ROLE_ROLE_NOT_EXIST     = 119105108,   // 修改的角色不存在
    IVS_SMU_GET_DEFAULT_RULE_ERROR         = 119105109,   // 获取默认帐号规则失败
    IVS_SMU_GET_ROLE_BY_ID_ERROR           = 119105110,   // 根据id获取角色失败
    IVS_SMU_GET_ROLE_BY_ID_ROLE_NOT_EXIST  = 119105111,   // 角色id对应的角色不存在
    IVS_SMU_GET_USER_ROLE_USER_NOT_HAVE_ROLE= 119105112,  // 用户没有分配角色
    IVS_SMU_GET_USER_ROLE_USER_NOT_EXIST   = 119105113,   // 用户没有分配角色
    IVS_SMU_GET_USER_ROLE_OP_ERROR         = 119105114,   // 获取角色操作权限失败
	IVS_SMU_FILE_OP_ERROR                  = 119105116,   // 上传下载文件失败
    IVS_SMU_FILE_OVER_MAX                  = 119105117,   // 文件大小太大，超过20m
    IVS_SMU_FILE_BUILD_CMD_ERROR           = 119105118,   // 拼装cmd时错误
    IVS_SMU_PARAM_INVALID_FOR_ROLE         = 119105119,   // 角色参数错误
    IVS_SMU_ROLE_NUM_OVER_MAX              = 119105120,    // 角色个数达到上限,超过128
	IVS_SMU_GROUP_NUM_OVER_MAX_VCN500      = 119105121,    // VCN500用户组个数达到上限，超过16
	IVS_SMU_ROLE_NUM_OVER_MAX_VCN500       = 119105122,    // VCN500角色个数达到上限,超过32

     /* SMU:用户管理错误码 */
    IVS_SMU_MODIFY_PWD_OLD_PWD_WRONG       = 119105200,   // 修改密码时旧密码错误
	IVS_SMU_ADD_USER_ERROR                 = 119105201,   // 增加用户错误
	IVS_SMU_DELETE_USER_ERROR              = 119105202,   // 删除用户错误
	IVS_SMU_MODIFY_USER_ERROR              = 119105203,   // 修改用户错误
	IVS_SMU_INQUIRY_USER_ERROR             = 119105204,   // 查询用户错误
	IVS_SMU_INQUIR_ONLINE_USER_ERROR       = 119105205,   // 查询在线用户错误
	IVS_SMU_INQUIR_DEVLIST_ERROR           = 119105206,   // 查询用户设备错误
	IVS_SMU_USER_NOT_EXIST                 = 119105207,   // 用户不存在
    IVS_SMU_USER_EXIST                     = 119105208,   // 用户已存在
	IVS_SMU_EXIST_SAME_ITEM                = 119105211,   // 数据库已存在相同的内容
    IVS_SMU_DB_NOT_EXIST_ITEM              = 119105212,   // 数据库不存在内容
    IVS_SMU_FROM_GREATERTHAN_INDEX_ERROR   = 119105214,   // 用户查询FROM索引大于TO索引
    IVS_SMU_XML_UNIFIEDQUERY_LIMIT_ERROR   = 119105215,   // 统一查询参数中的limit不正确
    IVS_SMU_XML_UNIFIEDQUERY_INDEX_ERROR   = 119105216,   // 统一查询参数中的Fromindex、Toindex不正确
    IVS_SMU_USER_ID_INVALID                = 119105218,   // 数据库ID不合法
    IVS_SMU_ID_GENERATE_ERROR              = 119105222,   // 数据库id生成错误
    IVS_SMU_ADD_USER_ROLE_RELATION_ERROR   = 119105223,   // 添加用户角色关联表错误
    IVS_SMU_ADD_USER_GROUP_RELATION_ERROR  = 119105224,   // 添加用户用户组关联表错误
    IVS_SMU_ROLE_NOT_EXIST                 = 119105225,   // 角色表中角色不存在
    IVS_SMU_GROUP_NOT_EXIST                = 119105226,   // 用户组不存在
    IVS_SMU_DEV_EXIST_SAME_ITEM            = 119105227,   // 设备已存在
    IVS_SMU_DELETE_USER_USERGROUP_ERROR    = 119105228,   // 删除用户用户组表错误
    IVS_SMU_DELETE_USER_ONLINE_ERROR       = 119105229,   // 删除在线用户错误
    IVS_SMU_DELETE_USER_ROLE_ERROR         = 119105230,   // 删除用户角色对应表错误
    IVS_SMU_DELETE_USER_DEVICE_ERROR       = 119105231,   // 删除用户设备对应表错误
    IVS_SMU_DELETE_USER_INFO_ERROR         = 119105232,   // 删除用户基本表错误
    IVS_SMU_MODIFY_USER_ROLE_ERROR         = 119105234,   // 修改用户角色对应表错误
    IVS_SMU_MODIFY_USER_USERGROUP_ERROR    = 119105235,   // 修改用户用户组表错误
    IVS_SMU_MODIFY_USER_USEINFO_ERROR      = 119105236,   // 用户修改基本信息表错误
    IVS_SMU_MODIFY_ADMIN_USEINFO_ERROR     = 119105237,   // 管理员修改基本信息表错误
    IVS_SMU_POINTER_IS_NULL                = 119105238,   // 指针为空
    IVS_SMU_GET_TIMEZONE_ERROR             = 119105239,   // 获取时区失败
    IVS_SMU_ADD_SBUSCRIBE_INFO_ERROR       = 119105240,   // 增加订阅告警信息错误
    IVS_SMU_PAGEINFO_INVALID               = 119105241,   // 分页信息不合法
    IVS_SMU_USER_TYPE_INVALID              = 119105246,   // 用户类型不合法
    IVS_SMU_DB_ID_INVALID                  = 119105247,   // 数据库id不合法
    IVS_SMU_USER_REGINSTER_DATE_INVALID    = 119105249,   // 用户注册时间不合法
    IVS_SMU_USER_STATUS_INVALID            = 119105250,   // 用户状态不合法
    IVS_SMU_USER_SEX_INVALID               = 119105253,   // 用户SEX不合法
    IVS_SMU_USER_ROLEID_INVALID            = 119105254,   // 用户角色ID不合法
    IVS_SMU_USER_GROUPID_INVALID           = 119105255,   // 用户组ID不合法
    IVS_SMU_GET_USER_ID_NAME_ERROR         = 119105256,   // 获取用户id和登录名错误
    IVS_SMU_SELECT_COUNT_ERROR             = 119105257,   // 获取查询总量错误
    IVS_SMU_OVER_MAX_USER_NUM              = 119105258,   // 超过用户最大限
    IVS_SMU_SEARCH_CODE_INVALID            = 119105259,   // 搜索条件信息不合法
    IVS_SMU_OVER_MAX_USER_NUM_VCN500       = 119105260,   // 超过用户最大限(VCN500)
    IVS_SMU_STATEMENT_FILE_OPEN_ERROR      = 119105262,   // 声明文件打开失败(文件不存在)
    IVS_SMU_STATEMENT_FILE_TOO_LARGE       = 119105263,   // 声明文件太大
    IVS_SMU_STATEMENT_FILE_INVALID         = 119105264,   // 声明文件不合法
    IVS_SMU_READ_STATEMENT_ERROR           = 119105265,   // 读取声明文件失败
    IVS_SMU_PARSE_STATEMENT_FILE_TOO_LARGE = 119105266,   // 解析声明文件失败
    IVS_SMU_DELETE_USERSELF_ERROR          = 119105267,   // 用户删除用户本身失败(自己删除自己失败)
    IVS_SMU_PARAMETER_INVALID              = 119105268,   // 分页信息中的用户参数不合法
    IVS_SMU_USERGROUP_NOT_EXIST            = 119105270,   // 用户组不存在
    IVS_SMU_USER_MODIFY_INFO_INVALID       = 119105271,   // 用户修改信息不合法
    IVS_SMU_ADD_USER_SUBSCRIBEINFO_ERROR   = 119105272,   // 增加用户订阅表信息失败
    IVS_SMU_DB_EXIST_SAME_ITEM             = 119105273,   // 数据库存在相同记录
    IVS_SMU_DELETE_SUBSCRIBE_LEVEL_ERROR   = 119105274,   // 删除订阅级别失败
    IVS_SMU_DELETE_SUBSCRIBE_TYPE_ERROR    = 119105275,   // 删除订阅类型失败
    IVS_SMU_SELF_LOCK_ERROR                = 119105276,   // 自己锁定自己不允许
    IVS_SMU_SELF_DELETE_ERROR              = 119105277,   // 自己删除自己不允许
    IVS_SMU_DEV_GROUP_DOMAIN_CODE_TOO_LONG = 119105284,   // 设备组域编码太长
    IVS_SMU_DELETE_SUPER_INVALID           = 119105285,   // 删除super不允许
    IVS_SMU_RESET_SUPER_INVALID            = 119105286,   // 重置super密码不允许
    IVS_SMU_ADD_USER_DEV_ERROR             = 119105287,   // 增加用户设备失败
    IVS_SMU_DEV_NOT_EXIST                  = 119105288,   // 设备不存在
    IVS_SMU_PCRE_PATTERN_TOO_LONG          = 119105289,   // PCRE规则太长
    IVS_SMU_PCRE_TEXT_TOO_LONG             = 119105290,   // 需要PCRE判断的文本太长
    IVS_SMU_PCRE_COMPILE_ERROR             = 119105291,   // PCRE编译出错
    IVS_SMU_PCRE_NOT_COMPELETLY_MATCH      = 119105292,   // PCRE规则不完全匹配
    IVS_SMU_PCRE_NOT_MATCH_OR_ERROR        = 119105293,   // PCRE完全不匹配或者出错
    IVS_SMU_PCRE_NOT_MATCH                 = 119105294,   // PCRE规则完全不匹配
    IVS_SMU_PCRE_ERROR                     = 119105295,   // PCRE执行出错
    IVS_SMU_PCRE_TEXT_IS_NULL              = 119105296,   // 需要PCRE判断的文本为空
    IVS_SMU_READ_UNICODE_TXT_ERROR         = 119105297,   // 读Unicode文件失败
    IVS_SMU_SEARCH_CONDITION_TOO_LONG      = 119105298,   // 搜索条件太长
    IVS_SMU_XML_NVR_CODE_INVALID           = 119105299,   // 设备编码错误
    IVS_SMU_PWD_PCRE_INVALID               = 119105300,   // 密码不符合PCRE规则
    IVS_SMU_PWD_PCRE_NO_CAPTIAL            = 119105301,   // 密码中没有包含大写
    IVS_SMU_PWD_PCRE_NO_LOWERCASE          = 119105302,   // 密码中没有包含小写
    IVS_SMU_PWD_PCRE_NO_FIGURE             = 119105303,   // 密码中没有包含数字
    IVS_SMU_PWD_PCRE_NO_SPECIAL_CHAR       = 119105304,   // 密码中没有包含特殊字符
    IVS_SMU_IS_TOTAL_GROUP_INVALID         = 119105305,   // IS_TOTAL_GROUP标记不合法
    IVS_SMU_GENERATE_PWD_ERROR             = 119105306,   // 生成密码失败
    IVS_SMU_NEW_OLD_PWD_DIFF_NUM_INVALID   = 119105307,   // 新旧密码不同字符数目不合法
    IVS_SMU_USER_PWD_RESET_ERROR           = 119105308,   // 重置密码错误
    IVS_SMU_PWD_REVERSE_ERROR              = 119105313,   // 密码反转失败
    IVS_SMU_PWD_LOGNINAME_REVERSE_SAME     = 119105314,   // 密码与登录名反转相同
    IVS_SMU_PWD_LOGNINAME_SAME             = 119105315,   // 密码与登录名相同
    IVS_SMU_OLD_PWD_DIFFERENT              = 119105316,   // 旧密码与真实密码不一致
    IVS_SMU_DECRYPT_PWD_ERROR              = 119105317,   // 用户密码解密失败
    IVS_SMU_ENCRYPT_PWD_ERROR              = 119105318,   // 用户密码加密失败
    IVS_SMU_DOMAIN_USER_MODIFY_PWD_ERROR   = 119105319,   // 域用户不可修改密码
    IVS_SMU_USER_MODIFY_LOGINNAME_INVALID  = 119105322,   // 用户登录名不能修改
    IVS_SMU_USER_MODIFY_DOMAINNAME_INVALID = 119105323,   // 用户域名不能修改
    IVS_SMU_USER_ID_TOO_LONG               = 119105333,   // ID长度不合法
    IVS_SMU_GROUP_ID_NOT_NUMBER            = 119105334,   // 组ID不是有效数字
    IVS_SMU_GROUP_ID_TOO_LONG              = 119105335,   // 组ID太长
    IVS_SMU_USER_ROLE_ID_NOT_NUMBER        = 119105336,   // 角色ID不是有效数字
    IVS_SMU_ROLE_ID_TOO_LONG               = 119105337,   // 角色ID太长
    IVS_SMU_DELETE_USER_SUBINFO_ERROR      = 119105345,   // 删除订阅信息错误
    IVS_SMU_INQUIRY_USER_GROUP_LIST_ERROR  = 119105346,   // 获取用户组列表失败
    IVS_SMU_NO_LANGUAGE_EXIST              = 119105347,   // 系统不支持该语言
    IVS_SMU_STATEMENT_FILE_FSEEK_ERROR     = 119105348,   // 声明文件fseek操作失败
    IVS_SMU_RESET_SELF_INVALID             = 119105349,   // 重置自身密码不允许
    IVS_SMU_ROLE_BEYOND_LIMITED            = 119105350,   // 权限不足



    /* SMU:告警管理错误码，取值范围：119120000~119120999 */
    IVS_SMU_DB_HAD_SAME_ALARM_LEVEL        = 119120000,    // 数据库中已经存在相同的告警级别了
    IVS_SMU_DB_MAX_ALARM_LEVEL_NUM         = 119120001,    // 数据库中告警级别的个数已经达到最大值
    IVS_SMU_DB_ALARM_NO_DATA               = 119120002,    // 在数据库中没有查到匹配查询条件的数据
    IVS_SMU_DB_HAD_SAME_ALARM_HELP         = 119120003,    // 数据库中已经存在相同的告警帮助了
    IVS_SMU_ALARM_DB_NO_AREA               = 119120004,    // 数据库中不存在指定的防区
    IVS_SMU_ALARM_AREA_HAS_GUARD_PLAN      = 119120005,    // 指定的防区已经有防区计划了
    IVS_SMU_ALARM_DB_NO_ALARM_LEVEL        = 119120006,    // 数据库中不存在指定的告警级别
	IVS_SMU_ALARM_SUB_ALARM_TYPE_ERROR     = 119120007,    // 按告警类型订阅，告警类型不可为空
	IVS_SMU_ALARM_SUB_ALARM_LEVEL_ERROR    = 119120008,    // 按告警级别订阅，告警级别大小错误
	IVS_SMU_ALARM_SUB_OVER_MAX             = 119120009,    // 告警订阅请求数，超过1000
    IVS_SMU_ALARM_DB_NO_ALARM_TYPE         = 119120010,    // 在数据库中没有指定的告警类型
	IVS_SMU_ALARM_CONFIRMED                = 119120011,    // 告警已确认
	IVS_SMU_ALARM_REVOKED                  = 119120012,    // 告警已撤销
	IVS_SMU_ALARM_RECOVERY                 = 119120013,    // 告警已恢复
	IVS_SMU_MSGBLOCK_INVALID			   = 119120014,    // 请求CMD不合法
	IVS_SMU_DEV_CODE_NOT_FOUND			   = 119120015,    // 设备已删除或不存在
	IVS_SMU_ALARM_EVENT_ID_NOT_EXIST       = 119120016,    // 告警事件id不存在
    IVS_SMU_DB_HAD_SAME_ALARM_TYPE         = 119120017,    // 数据库中已经存在相同的告警类型了
	IVS_SMU_DB_HAD_SAME_ALARM_TYPE_NAME    = 119120018,    // 数据库中已经存在相同的告警类型名称了
	IVS_SMU_ALL_USERS_OF_ALARM_COMMISSION_ILLEGAL = 119120019, //告警授权用户全部不合法
	IVS_SMU_ALARM_AREA_NOTEXIST			   = 119120020,    // 设备不存在
	IVS_SMU_ALARM_AREA_GUARD_OVER_MAX      = 119120021,    // 防区超过了最大数(1000)
	IVS_SMU_ALARM_AREA_GUARD_DEV_OVER_MAX  = 119120022,    // 防区设备超过了最大数(128)
	IVS_SMU_ALARM_AREA_NAME_GUARD_EXIST    = 119120023,    // 防区名称已经存在
	IVS_SMU_ALARM_SUB_STYLE_OVER_MAX       = 119120024,    // 告警订阅类型超过了最大数(5000)
	IVS_SMU_ALARM_INFO_NOT_FOUND		   = 119120025,    // 未查找到告警信息
	IVS_SMU_LINKAGE_ACTION_NOT_EXIST       = 119120026,    // 联动动作不存在
	IVS_SMU_AREA_ID_NOT_EXIST              = 119120027,    // 提示该防区已被删除
	IVS_SMU_AREA_NO_DEV                    = 119120028,    // 指定防区里没有设备
	IVS_SMU_PART_USERS_OF_ALARM_COMMISSION_ILLEGAL = 119120029, //告警授权用户部分不合法
	IVS_SMU_ALARM_AREA_DEVICE_INVALID      = 119120030,    // 防区中不能添加外域设备


	/* SMU:录像模块错误码，取值范围：119125000 ~ 119125999 */
	IVS_SMU_CAMERA_CODE_NOT_EXIST          = 119125001,    // 摄像机编码不存在
	IVS_SMU_LINKID_NOT_EXIST               = 119125002,    // TCP连接不存在
	IVS_SMU_RECORD_TIMEOUT                 = 119125003,    // 录像模块超时
	IVS_SMU_RECORD_LABEL_TIMEOUT           = 119125004,    // 录像书签超时
	IVS_SMU_RECORD_LOCK_TIMEOUT            = 119125005,    // 录像锁定模块超时
    IVS_SMU_MEDIA_SAFE_TIMEOUT             = 119125006,    // 媒体安全超时
    IVS_SMU_BACK_RECORD_PLAN_TIMEOUT       = 119125007,    // 备份录像计划超时
    IVS_SMU_GET_CLUSTER_RECORD_PLAN_FAIL   = 119125008,    // 集群下查询录像计划失败
    IVS_SMU_GET_RECORD_TASK_FAIL           = 119125009,    // 查询录像任务失败
    IVS_SMU_GET_CLUSTER_RECORD_POLICY_FAIL = 119125010,    // 集群下查询录像策略失败
    IVS_SMU_GET_CLUSTER_RECORD_STATE_FAIL  = 119125011,    // 集群下查询平台录像状态失败
    IVS_SMU_CLUSTER_START_RECORD_FAIL      = 119125012,    // 集群下手动启动录像失败
    IVS_SMU_CLUSTER_STOP_RECORD_FAIL       = 119125013,    // 集群下手动停止录像失败
    IVS_SMU_GET_BACK_MBU_TIMEOUT           = 119125014,    // 查询备份服务器信息超时
    IVS_SMU_GET_PRERECORD_CAMERA_LIST_TIMEOUT = 119125015, // 查询开启预录的摄像机列表超时
    IVS_SMU_GET_PRERECORD_CAMERA_LIST_FAIL = 119125016,    // 查询开启预录的摄像机列表全部失败
    IVS_SMU_GET_CAMERA_BACKUP_CFG_FAIL     = 119125017,    // 集群下查询摄像机备份参数失败
    IVS_SMU_BACKUP_SETTING_TIMEOUT         = 119125018,    // 备份录像参数超时
    IVS_SMU_GET_BACKUP_PLAN_FAIL           = 119125019,    // 集群下查询备份录像计划失败
    IVS_SMU_GET_MBU_CHANGE_HISTORY_FAIL    = 119125020,    // 集群下查询备份服务器变更记录失败
    IVS_SMU_TRANSPARENT_NOT_RUN            = 119125021,    // 透明通道没有启用
    IVS_SMU_GET_PRERECORD_CAMERA_LIST_PART_FAIL= 119125022,// 查询开启预录的摄像机列表部分失败
    IVS_SMU_GET_RECORD_TASK_PART_FAIL      = 119125023,    // 查询录像任务失败
    IVS_SMU_GET_RECORD_LIST_FAIL           = 119125024,    // 查询录像列表失败

    /* SMU:集群模块错误码，取值范围：119126000 ~ 119126999 */
    IVS_SMU_NVR_LIST_EMPTY                 = 119126000,    // NVR列表为空
    IVS_SMU_NVR_SERVER_CNT_IS_ZERO         = 119126001,    // server NVR个数为0
	IVS_SMU_NOT_FOUND_LEADER               = 119126002,    // 没找到Leader
    IVS_SMU_GET_MEMBER_STATUS_TIME_OUT     = 119126003,    // 获取成员状态超时
    IVS_SMU_BE_IN_CREATE_CLUSTER           = 119126004,    // 正在组建集群
    IVS_SMU_BE_IN_DISSOLVE_CLUSTER         = 119126005,    // 正在解散集群
    IVS_SMU_CLUSTER_SYNCING                = 119126006,    // 正在集群同步，不允许集群操作
    IVS_SMU_CLEAR_BLACK_LIST_IS_NULL       = 119126007,    // ClearBlackList结点为空
    IVS_SMU_CREATE_CLUSTER_TIME_OUT        = 119126008,    // 组建集群超时
    IVS_SMU_NVR_LIST_NODE_NOT_EXIST        = 119126009,    // NvrList结点不存在
    IVS_SMU_NVR_INFO_NODE_NOT_EXIST        = 119126010,    // NvrInfo结点不存在
    IVS_SMU_NVR_CODE_OR_IP_INVALID         = 119126011,    // NvrCode或IP结点不合法
    IVS_SMU_NVR_ROLE_INVALID               = 119126012,    // Nvr角色结点不合法
    IVS_SMU_NVR_INFO_CONTENT_INVALID       = 119126013,    // NvrInfo结点内容不合法
    IVS_SMU_NODE_MAP_IS_EMPTY              = 119126014,    // NVR列表为空
    IVS_SMU_FIND_NVR_TRANS_ID_NULL         = 119126015,    // 查询NVR时事务ID为空
    IVS_SMU_FIND_NVR_CAMERA_LIST_EMPTY     = 119126016,    // 查询NVR时摄像机列表为空
    IVS_SMU_CLUSTER_OFFLINE                = 119126017,    // 集群不在线
    IVS_SMU_CLUSTER_ROLE_NOT_CLIENT        = 119126018,    // 角色不为Client
    IVS_SMU_CLUSTER_SIZE_IS_MIN            = 119126019,    // 集群个数已经是最小
    IVS_SMU_NVR_CODE_NOT_EXIST             = 119126020,    // 该NVR不存在
    IVS_SMU_SEND_CLUSTER_FAIL              = 119126021,    // 下发集群失败
    IVS_SMU_CLUSTER_CODE_HAS_EXIST         = 119126022,    // 该集群编码已经存在
    IVS_SMU_NVR_NOT_CLEAR                  = 119126023,    // 该NVR未清理干净
    IVS_SMU_CLUSTER_NOT_EXIST              = 119126024,    // 该集群不存在
    IVS_SMU_BE_IN_ADD_CLIENT               = 119126025,    // 正在添加client
    IVS_SMU_BE_IN_DEL_CLIENT               = 119126026,    // 正在删除client
	IVS_SMU_CLUSTER_SCHEDULING             = 119126027,    // 集群正在调度或者删除成员，请稍后再试

    /* SMU:多域模块错误码，取值范围：119127000 ~ 119127999 */
    IVS_SMU_GET_CONFIG_PATH_ERROR          = 119127000,    // 获取配置路径失败
    IVS_SMU_LOAD_NET_XML_ERROR             = 119127001,    // 加载net.xml失败
    IVS_SMU_NET_ELEMENTS_NODE_NOT_EXIST    = 119127002,    // NetElement结点不存在
    IVS_SMU_SAVE_NET_XML_ERROR             = 119127003,    // 保存net.xml失败
    IVS_SMU_LOCAL_IP_IS_NULL               = 119127004,    // 本机信令IP为空
    IVS_SMU_MEDIA_IP_IS_NULL               = 119127005,    // 本机媒体IP为空
	IVS_SMU_DOMAIN_IS_NOT_ONLIE            = 119127006,    // 域不在线

	/* SMU:日志管理错误码，取值范围：119130000 ~ 119130999 */
	IVS_SMU_SET_LOG_LEVEL_ERROR            = 119130000,    // 设置日志级别失败

	IVS_SMU_NO_PCG_NODE_ERROR              = 119139000,    // 无PCG连接信息

    /* SMU License管理， 取值范围：119132000 ~ 119132999 */
    IVS_SMU_LICENSE_FILE_NOEXIST = 119132001, // license文件不存在
    IVS_SMU_LICENSE_ACCESS_LIMITED = 119132002, // License接入入数受限
    IVS_SMU_LICENSE_ESN_MISMATCH = 119132003, // License导入ESN不匹配
    IVS_SMU_LICENSE_ACTIVATION_FAIL =119132004, // License激活失败
    IVS_SMU_DEV_NOT_EXIST_OR_LICENSE_ACCESS_LIMITED = 119132005, //设备不存在或者License受限
	IVS_SMU_DEVICE_NUM_LIMITED = 119132006, //设备接入数受限
	IVS_SMU_DEVICE_NUM_MAY_LIMITED = 119132007, //其他用户正在添加设备，可能超过最大接入能力，请稍后


	/******************************************************************
	 *   SCU错误码定义（模块编码：12）
	 *****************************************************************/
	IVS_SCU_LINKAGE_DEV_SIZE_ILLEGAL       = 129123002,    // 联动设备的个数非法(不能超过4个)
	IVS_SCU_LINKAGE_POLICY_ID_NOT_EXIST    = 129123003,    // 联动策略ID不存在
	IVS_SCU_LINKAGE_POLICY_ID_EXIST        = 129123004,    // 联动策略ID已经存在
	IVS_SCU_LINKAGE_POLICY_EXIST           = 129123005,    // 联动策略已经存在
	IVS_SCU_LINKAGE_POLICY_COUNT_OVER      = 129123006,    // 告警联动策略超过了最大数(3000)
	IVS_SCU_LINKAGE_ACTION_ILLEGAL         = 129123007,    // 告警联动动作不能重复
    IVS_SCU_SEND_ALARM_CONFIRM_SMU_FAIL    = 129123008,    // 发送告警确认请求到SMU失败
	IVS_SCU_LINKAGE_DEV_COUNT_ERROR        = 129123010,    // 联动设备个数超过上限
	IVS_SCU_LINKAGE_DEV_NOT_EXIST          = 129123011,    // 联动目标设备不存在或者已删除
	IVS_SCU_LINKAGE_ACTION_NOT_EXIST       = 129123012,    // 联动动作不存在

	IVS_SCU_ONLINE_USER_EXPIRE             = 129105001,    // 登录信息已失效，请重新登录

	IVS_SCU_DEV_OFFLINE			           = 129108001,	   // 设备不在线
	IVS_SCU_DEV_NOT_EXIST			       = 129108002,	   // 设备不存在或者已删除
	IVS_SCU_DEV_NOT_EXIST_CLUSTER          = 129108003,	   // (集群使用)设备不存在当前NVR，可能已迁移
	IVS_SCU_DEV_MODULE_ERROR		       = 129108004,	   // 模板校验失败
	IVS_SCU_DEV_PROTOCALTYPE_ERROR	       = 129108005,	   // 设备协议类型校验失败
	IVS_SCU_DEV_VENDORTYPE_ERROR	       = 129108006,	   // 供应商类型校验失败
	IVS_SCU_DEV_FORMTYPE_ERROR	           = 129108007,	   // 产品形态类型校验失败
	IVS_SCU_DEV_MODELTYPE_ERROR	           = 129108008,	   // 设备型号类型校验失败
	IVS_SCU_DEV_ALARMENABLE_ERROR	       = 129108009,	   // 告警使能校验失败
	IVS_SCU_DEV_VIDEOFORMAT_ERROR	       = 129108010,	   // 视频制式校验失败
	IVS_SCU_DEV_REGPASSWORD_ERROR	       = 129108011,	   // 注册密码为空
	IVS_SCU_DEV_MAXDIRCONNECTNUM_ERROR	   = 129108012,	   // 最大直连数失败
	IVS_SCU_DEV_NVRCODE_ERROR	           = 129108013,	   // NVR为不合法
	IVS_SCU_DCG_RESPONSE_FAIL              = 129108018,    // DCG响应失败
	IVS_SCU_MAIN_DEV_GURAD_SET_FAIL        = 129108021,    // 主设备布撤防失败
    IVS_SCU_DEV_NOT_IN_WHITE_LIST_CLUSTER  = 129108022,    // 设备不属于当前NVR，可能已移除,集群使用
    IVS_SCU_DEV_NOT_IN_WHITE_LIST          = 129108023,    // 设备不属于当前NVR，可能已移除,非集群使用
    IVS_SCU_VIDEO_STREAM_NUM_ERROR         = 129108024,    // 视频流数不正确
	IVS_SCU_SENDTODCG_FAIL                 = 129108026,    // 下发DCG失败
	IVS_SCU_ADD_AUDIO_PARA_FAIL            = 129108027,    // 增加音频参数失败
	IVS_SCU_GET_LOGIN_ID_FAIL              = 129108029,    // 获取用户ID失败
    IVS_SCU_DEV_TEMPLATE_NONEXISTENT       = 129108031,    // 设备模板不存在
    IVS_SCU_DEV_IS_ONLINE    	           = 129108032,	   // 设备已在线
    IVS_SCU_SUB_DEV_INFO_NOT_EXIST         = 129108033,    // 子设备参数信息不存在
	IVS_SCU_ADD_SUBDEVICE_FAIL             = 129108035,    // 添加子设备失败失败
	IVS_SCU_SEND_MSG_TO_SMU_FAIL           = 129108036,    // 发送smu响应失败
	IVS_SCU_ADD_ENVR_SUBDEVICE_FAIL        = 129108037,    // 添加ENVR设备失败
    IVS_SCU_ENVR_DEV_NOT_EXIST			   = 129108038,	   // eNVR设备不存在或者已删除
	IVS_SCU_MAIN_DEV_NOT_EXIST			   = 129108039,	   // 主设备不存在或者已删除
    IVS_SCU_DEV_OFFLINE_CFG_NOT_EXIST	   = 129108040,	   // 设备离线配置不存在
	IVS_SCU_DEV_TYPE_IS_NULL        	   = 129108041,	   // 设备设备形态不合法
    IVS_SCU_DEV_OSD_PARA_NOT_EXIST	       = 129108042,	   // 设备OSD参数不存在
	IVS_SCU_DEL_ENVR_DEVICE_FAIL           = 129108043,    // 删除ENVR设备失败
	IVS_SCU_SEL_SHARE_DEVICE_INVALID       = 129108044,    // 查询共享设备参数不合法
	IVS_SCU_MODEL_TEMPLATE_IS_NULL         = 129108045,    // 设备模板数量为空
    IVS_SCU_DR_MAIN_DEV_NOT_EXIST          = 129108046,    // 容灾设备信息不存在
	IVS_SCU_DEV_UPDATE_TEMPATE_MAIN_DEV    = 129108047,    // 设备模板更新主设备信息失败
    IVS_SCU_PRE_RECORD_DEV_NOT_EXIST       = 129108048,    // 开启预录设备不存在
    IVS_SCU_PU_NOT_SUPPORT_GET_PARAM       = 129108049,    // 前端不支持前端参数获取
	IVS_SCU_MOD_ENVR_SUBDEVICE_FAIL        = 129108050,    // 修改ENVR设备失败
	IVS_SCU_MOD_T28181_LENGTH_INVALID      = 129108051,    // T28181长度不合法
	IVS_SCU_LOGIN_TYPE_INVALID             = 129108053,    // 认证类型不合法
	IVS_SCU_BLANCE_SUPPORT_INVALID         = 129108054,    // 是否支持集群参数不合法
	IVS_SCU_DIRECTCONNECT_FIRST_INVALID    = 129108055,    // 是否支持直连优先参数不合法
	IVS_SCU_DECRYPT_PASSWORD_INVALID       = 129108056,    // smu发送密码加密失败
	IVS_SCU_DECRYPT_REGPASSWORD_INVALID    = 129108057,    // smu发送注册密码加密失败
	IVS_SCU_PASSWORD_LENGTH_INVALID        = 129108058,    // 密码长度或者复杂度不满足要求
	IVS_SCU_PARSE_CAMERA_TYPE_FAILED       = 129108060,    // 摄像机类型转换失败
	IVS_SCU_MAIN_DEV_SEQUENCE_TYPE_INVALID = 129108061,    // 主设备序列号不合法
	IVS_SCU_TAS_CODE_INVALID               = 129108062,    // TASCode不合法
	IVS_SCU_DEV_SN_INVALID                 = 129108063,    // DevSN不合法
	IVS_SCU_DEV_REG_PASSWORD_INVALID       = 129108064,    // 注册密码不合法
	IVS_SCU_DEV_IP_TYPE_INVALID            = 129108065,    // 设备Ip类型不合法
	IVS_SCU_GET_DEV_LIST_FAILED            = 129108066,    // 获取主设备列表失败
	IVS_SCU_PATCH_PARSE_MAIN_DEV_FAILED    = 129108067,    // 批量解析主设备全部失败
	IVS_SCU_PWD_STRENGTH_WEAK              = 129108068,    // 密码强度不够
	IVS_SCU_DEVICE_IP_EXIST                = 129108069,    // 设备IP已存在
	IVS_SCU_DEV_REG_PWD_INVALID            = 129108070,    // 密码复杂度不满足要求，如需关闭密码复杂度校验，请查看配置指南

    IVS_SCU_CAMERA_NOT_EXIST               = 129109003,    // 摄像机子设备不存在或者已删除
	IVS_SCU_MOTION_DETECTION_NOT_EXIST     = 129109004,    // 移动侦测信息不存在
	IVS_SCU_VIDEO_HIDE_NOT_EXIST           = 129109005,    // 图像遮挡信息不存在
	IVS_SCU_VIDEO_MASK_NOT_EXIST           = 129109006,    // 隐私保护信息不存在
	IVS_SCU_CONF_PTZ_DB_ERROR              = 129109007,    // 配置云台参数成功但数据保存到数据库失败
	IVS_SCU_BROADCAST_IP_PORT_EXIST        = 129109008,    // 组播IP端口已存在或已被占用

    IVS_SCU_CONF_VOICE_CHAN_TIMEOUT        = 129112001,    // 配置语音参数超时
	IVS_SCU_CONF_VOICE_CHAN_DB_ERROR       = 129112002,    // 配置语音参数成功但数据保存到数据库失败

    IVS_SCU_GETURL_ERROR 			       = 129115001,	   // 获取媒体URL失败
    IVS_SCU_GETURL_TIMEOUT 		           = 129115002,	   // 获取媒体URL会话超时
    IVS_SCU_GETURL_SESSION_EXIST 	       = 129115003,	   // 获取媒体URL的请求已存在

	IVS_SCU_PTZ_LOCKED                     = 129116001,    // 云台已锁定
	IVS_SCU_PTZ_CONTROLED                  = 129116002,    // 云台被同优先级或者高优先级用户控制
	IVS_SCU_PTZ_LEVEL_LOW_TO_UNLOCK        = 129116003,    // 云台被同优先级或者高优先级用户锁定
	IVS_SCU_PTZ_LOCKED_BY_ALARM            = 129116004,    // 云台被告警联动中，不允许操作
	IVS_SCU_PTZ_NOT_LOCKED                 = 129116005,    // 云台未锁定，或者已自动解锁
	IVS_SCU_PTZ_GET_USER_INFO_FAIL         = 129116006,    // 获取用户云台信息失败
	IVS_SCU_PTZ_MODE_SET_RUN               = 129116007,    // 同优先级或者高优先级用户正在录制PTZ轨迹
	IVS_SCU_PTZ_LOCKE_NOT_SUPPORT          = 129116008,    // 外域不支持云台锁定操作
	IVS_SCU_PTZ_MODE_SET_NOT_SUPPORT       = 129116009,    // 外域不支持模式录制操作
	IVS_SCU_PTZ_MODE_CRUISE_NOT_EXIST      = 129116010,    // 模式轨迹索引不存在

	IVS_SCU_PRESET_NUM_EXCEED_MAX          = 129117001,    // 预置位已经达到最大值
	IVS_SCU_PRESET_NUM_MAY_EXCEED_MAX      = 129117002,    // 其他用户也在添加预置位，可能超过最大个数，请稍候
	IVS_SCU_PRESET_IN_CRUISE_TRACK         = 129117003,    // 预置位在巡航轨迹中，无法删除
	IVS_SCU_PRESET_NOT_EXIST               = 129117004,    // 预置位不存在
	IVS_SCU_PRESET_NAME_REPEAT             = 129117005,    // 该摄像机已经存在相同的预置位名称
	IVS_SCU_PRESET_IN_CRUISE_PLAN          = 129117006,    // 预置位在巡航计划中，不能删除
	IVS_SCU_PRESET_IS_GUARD_POS            = 129117007,    // 预置位被设置成启用的看守位，不能删除

	IVS_SCU_GUARD_POS_NOT_EXIST            = 129117101,    // 看守位不存在

	IVS_SCU_TRACK_NUM_EXCEED_MAX           = 129118001,    // 巡航轨迹个数已经达到最大值
	IVS_SCU_TRACK_NUM_MAY_EXCEED_MAX       = 129118002,    // 其他用户也在添加巡航轨迹，可能超过最大个数，请稍候
	IVS_SCU_TRACK_POINT_NUM_EXCEED_MAX     = 129118003,    // 轨迹中的预置位个数不能超过20个
	IVS_SCU_TRACK_POINT_INVALID            = 129118004,    // 轨迹中的预置位不存在或者已经删除
    IVS_SCU_TRACK_NOT_EXIST                = 129118005,    // 巡航轨迹不存在
	IVS_SCU_TRACK_NAME_REPEAT              = 129118006,    // 该摄像机已经存在相同的巡航轨迹名称
	IVS_SCU_GUARD_PLAN_NOT_EXIST           = 129118007,    // 巡航计划不存在
	IVS_SCU_TRACK_IS_RUNNING               = 129118008,    // 该巡航轨迹正在执行
	IVS_SCU_TRACK_IS_IN_PLAN               = 129118009,    // 该轨迹在计划中，无法删除
	IVS_SCU_MODE_TRACK_NUM_EXCEED_MAX      = 129118010,    // 模式巡航轨迹个数已经达到最大值
	IVS_SCU_MODE_TRACK_NUM_MAY_EXCEED_MAX  = 129118011,    // 其他用户也在添加模式巡航轨迹，可能超过最大个数，请稍候

	IVS_SCU_NO_NVR_NOT_EXIST               = 129124001,    // 存储信息不存在
    IVS_SCU_DEV_RECORD_PARA_NOT_EXIST      = 129124002,    // 设备录像参数信息不存在
	IVS_SCU_STORAGE_NOT_ALLOC              = 129124003,    // 存储空间未分配
	IVS_SCU_STORAGE_CANT_LESSEN            = 129124004,    // 存储空间不能缩小，只能扩大
	IVS_SCU_STORAGE_IS_SCARCE              = 129124005,    // 存储空间不足
    IVS_SCU_CAMERA_BACKUP_CFG_NOT_EXIST    = 129124006,    // 摄像机备份参数记录不存在
    IVS_SCU_CAMERA_BACKUP_PLAN_NOT_EXIST   = 129124007,    // 摄像机备份计划不存在
    IVS_SCU_NEW_MBU_ADD_BACKUP_PLAN_FAIL   = 129124008,    // 向新MBU迁移备份计划失败
    IVS_SCU_RECORD_MBU_CONTRAIL_NOT_EXIST  = 129124009,    // MBU轨迹不存在
    IVS_SCU_BACKUP_RECORD_REGION_NOT_EXIST = 129124010,    // MBU录像备份记录的起止范围不存在
    IVS_SCU_ENCODEMODE_NOT_H264            = 129124011,    // 码流编码格式不是H264

    /*容灾备份错误码 begin*/
    IVS_SCU_DEVICE_STRATEGY_MAP_NOT_EXIST  = 129124011,    // 设备录像策略映射表记录不存在
    IVS_SCU_NVR_HAS_THIS_CAMERA            = 129124012,    // NVR存在该摄像机
    IVS_SCU_OTHER_EXDOMAIN_HAS_THIS_CAMERA = 129124013,    // 其他外域已经在该NVR配置了该摄像机
    IVS_SCU_SEND_MSG_TO_MU_FAIL            = 129124014,    // 下发消息给MU失败
    IVS_SCU_CAMERA_IS_NOT_PR               = 129124016,    // 该设备不是容灾设备
    IVS_SCU_PR_CAMERA_IS_NOT_HAVE_PALN     = 129124017,    // 未制订录像策略或计划
    IVS_SCU_PR_CAMERA_CAN_NOT_MOD_RECORD   = 129124018,    // 灾设备不能修改录像策略
    IVS_SCU_DR_CAMERA                      = 129124019,    // 容灾设备
    IVS_SCU_MBU_HAS_THIS_CAMERA            = 129124020,    // 不同域的摄像机编码冲突
    /*容灾备份错误码 end*/

    IVS_SCU_PU_MANUAL_RECORD_EXIST         = 129124015,    // 前端手动录像已存在

    IVS_SCU_RECORD_PLAN_NOT_EXIST          = 129125001,    // 录像计划不存在
    IVS_SCU_WAIT_DCG_RSP                   = 129125002,    // 等待DCG响应

	/* SCU:集群模式错误码，取值范围：129134101 ~ 129134200 */
	IVS_SCU_NOT_CLUSTER_MODE               = 129134101,    // 模式错误，非集群模式
	IVS_SCU_REDIRECTION_ERROR              = 129134102,    // 错误，请重定向（集群下设备在黑名单时，需重定向）
	IVS_SCU_SCRIPT_RUN_FAIL                = 129134103,    // 脚本执行出错
	IVS_SCU_CLUSTER_SIZE_INVALID           = 129134104,    // 集群成员大小不合法
	IVS_SCU_DEVLIST_VERSION_NOTSAME        = 129134105,    // 本地DB主设备版本号与中心DB不一致
	IVS_SCU_MODE_NOT_SYNC			       = 129134106,    // 身份未同步
	IVS_SCU_NOT_DB_NODE			       = 129134107,    // 不是中心数据库节点

    /* SCU:业务跟踪错误码，取值范围：129127050 ~ 129127100*/
    IVS_SCU_TRACE_NO_FIND                  = 129127050,    // 根据TraceID未找到业务信息
    IVS_SCU_CREATE_TRACE_FAIL              = 129127052,    // 创建业务跟踪失败
    IVS_SCU_CANCLE_TRACE_FAIL              = 129127053,    // 取消业务跟踪失败
    IVS_SCU_TRACE_TIME_MORE_THAN_MAX       = 129127054,    // 业务跟踪时间超过最大值

	/* SCU:外部告警系统错误码，取值范围：129128000 ~ 129128100*/
	IVS_SCU_UNICODE_EXISTS                 = 129128000,    // 标识符已存在
	IVS_SCU_UNICODE_INVALID                = 129128001,    // 标识符长度非法或包含非法字符
	IVS_SCU_LOCATION_INVALID               = 129128002,    // 安装位置长度非法或包含非法字符
	IVS_SCU_EXDEV_NUM_EXCEED               = 129128003,    // 外部告警设备数达到最大值
	IVS_SCU_EXDEV_CODE_GEN_FAIL            = 129128004,    // 外部告警设备编码生成失败
	IVS_SCU_EXSYS_TYPE_INVALID             = 129128005,    // 系统类型长度非法或包含非法字符
	IVS_SCU_EXDEV_TYPE_INVALID             = 129128006,    // 设备类型长度非法或包含非法字符
	IVS_SCU_EXDEV_TYPE_ERROR               = 129128007,    // 设备类型非法

    /* SCU:修改摄像机扩展参数接口错误码，取值范围：129147000 ~ 129147100*/
    IVS_SCU_WATERMARK_SECURITY_FAIL        = 129147000,    // DCG通知PU添加水印失败、MU切换媒体安全策略失败
    IVS_SCU_WATERMARK_FAIL                 = 129147001,    // DCG通知PU添加水印失败
    IVS_SCU_SECURITY_FAIL                  = 129147002,    // MU切换媒体安全策略失败

    /********************************************
	*   SMU Portal错误码定义（模块编码：13）
	*******************************************/


	/******************************************
	*   CMU错误码定义（模块编码：14）
	*******************************************/
	IVS_CMU_FIND_LEADER_ERROR              = 149107001,    // 查找Leader失败
	IVS_CMU_ZNODE_NOT_EXIST                = 149107002,    // 集群节点不存在

    IVS_CMU_PARAMETER_ERROR                = 149112016,    // 参数错误
    IVS_CMU_SCHEDULE_GET_IPC_LIST_ERROR    = 149112017,    // 调度时获取IPC列表失败
    IVS_CMU_SCHEDULE_LENS_MANAGE_ERROR     = 149112018,    // 调度镜头动作失败
    IVS_CMU_SCHEDULE_SET_IPC_NVR_ERROR     = 149112019,    // 调度成功，设置IPC和NVR绑定关系失败
    IVS_CMU_SCHEDULE_SET_IPC_TRACK_ERROR   = 149112020,    // 调度成功，设置IPC历史轨迹失败
    IVS_CMU_SCHEDULE_ARITHMETIC_ERROR      = 149112021,    // 调度算法失败
    IVS_CMU_SEND_SYNC_MSG_PART_ERROR       = 149112022,    // 发送同步消息失败
    IVS_CMU_GET_NON_NVR_STUATUS_ERROR      = 149112023,    // 未查找到任何NVR 的状态
    IVS_CMU_SET_CLUSTER_MEMBER_ERROR       = 149112024,    // 设置集群成员失败
    IVS_CMU_SET_RIGHT_ERROR                = 149112025,    // 集群设置操作无权限（用于向非Leader成员发送集群设置消息返回错误）
    IVS_CMU_SET_CLUSTER_INFO_ERROR         = 149112026,    // 设置集群信息失败
    IVS_CMU_GET_USABLE_NVR_INFO_ERROR      = 149112027,    // 可用 NVR 列表为空
    IVS_CMU_SEND_CLEAN_MSG_ERROR           = 149112028,    // 发送摄像机历史轨迹消息失败
    IVS_CMU_GET_DFS_LOCK_ERROR             = 149112029,    // 获取分布式文件锁失败
    IVS_CMU_GET_DFS_LOCK_TIMEOUT_ERROR     = 149112030,    // 获取分布式文件锁超时失败
	IVS_CMU_PLUGIN_NOT_FOUND_ERROR         = 149112031,    // 找不到对应的插件
	IVS_CMU_NO_AVAILABLE_CONTAINER_ERROR   = 149112032,    // 调度时没有可用容器
	IVS_CMU_DISCONNECT_WITH_ZK_ERROR       = 149112033,    // 与ZK断开连接
	IVS_CMU_TASK_SHIFT_IN_ERROR            = 149112034,    // 任务迁入失败（框架中）

    //用于集群添加主设备失败的错误码(功能编码~13)
	IVS_CMU_ADD_MAIN_DEV_OUT_OF_CONTENT    = 149113001,    // 超出集群负载能力部分，设备添加失败
	IVS_CMU_ADD_MAIN_DEV_WRITE_ZK_ERROR    = 149113002,    // 保存数据出错，设备添加失败
	IVS_CMU_ADD_MAIN_DEV_SCHEDULE_ERROR    = 149113003,    // 调度设备出错，设备添加失败
	IVS_CMU_ADD_MAIN_DEV_READ_ZK_ERROR     = 149113004,    // 读取数据出错，设备添加失败
	IVS_CMU_ADD_MAIN_DEV_DUPLICATE         = 149113005,    // 设备已存在，设备添加失败
	IVS_CMU_ADD_DEV_ERROR                  = 149113006,    // 添加设备失败
	IVS_CMU_ADD_DEVS_ALL_ERROR             = 149113007,    // 添加所有设备失败
    IVS_CMU_DEVICE_IP_EXIST                = 149113008,    // 设备IP已存在，添加失败
	IVS_CMU_DEVICE_IP_DUPLICATE            = 149113009,    // 重复添加IP相同的设备
	IVS_CMU_DEVICE_CODE_DUPLICATE          = 149113010,    // 重复添加相同编码的设备，请确认需要添加的设备

    //用于定位摄像机（即SMU异常时，实况的重定向）失败的错误码(功能编码~14)
    IVS_CMU_IPC_CODE_NOT_FOUND_ERROR       = 149114001,    // 摄像机已删除或不存在
    IVS_CMU_GET_NVR_ID_ERROR               = 149114002,    // 读取数据出错，定位摄像机失败
    IVS_CMU_GET_NVR_DATA_ERROR             = 149114003,    // 读取数据出错，定位摄像机失败

    //用于OMU统一配置（查询和修改配置项，包括日志参数项）失败时的错误码(功能编码~15)
    IVS_CMU_CONFIG_TYPE_NOT_FOUND_ERROR    = 149115001,    // 配置项类型不存在
    IVS_CMU_CONFIG_NAME_NOT_FOUND_ERROR    = 149115002,    // 配置项名称不存在
    IVS_CMU_CONFIG_VALUE_INVALID_ERROR     = 149115003,    // 配置项参数无效
    IVS_CMU_READ_LOG_SFTP_CONF_ERROR       = 149115004,    // 获取日志参数项失败
    IVS_CMU_WRITE_LOG_SFTP_CONF_ERROR      = 149115005,    // 修改日志参数项失败

    //用于定位摄像机历史轨迹（录像检索）失败的错误码（功能编码~16）
    IVS_CMU_CAMERA_CODE_NOT_FOUND_ERROR    = 149116001,    // 摄像机已删除或不存在
    IVS_CMU_GET_CAMERA_TRACE_ERROR         = 149116002,    // 获取摄像机轨迹信息失败

	//用于告警联动的错误码（功能编码~20）
	IVS_CMU_LINKAGE_ACTION_ILLEGAL         = 149120001,    // 告警联动动作不能重复
	IVS_CMU_LINKAGE_POLICY_ID_EXIST        = 149120002,    // 联动策略ID已经存在
	IVS_CMU_LINKAGE_POLICY_EXIST           = 149120003,    // 联动策略已经存在

    IVS_CMU_PTR_NULL_ERROR                 = 149131001,    // 入参指针为空
    IVS_CMU_OBJ_TO_XML_ERROR               = 149131002,    // 消息类转换xml失败
    IVS_CMU_XML_TO_OBJ_ERROR               = 149131003,    // xml转换消息类失败
    IVS_CMU_CAN_NOT_FIND_LINKID_ERROR      = 149131004,    // 找不到LinkID
    IVS_CMU_NETELEMENT_TYPE_NULL_ERROR     = 149131005,    // 网元类型为空
    IVS_CMU_NSS_ENCODE_ERROR               = 149131006,    // NSS消息编码失败
    IVS_CMU_NSS_DECODE_ERROR               = 149131007,    // NSS消息解码失败
    IVS_CMU_SEND_SYNC_MSG_ERROR            = 149131008,    // 发送同步消息失败
    IVS_CMU_SEND_MSG_ERROR                 = 149131009,    // 发送异步消息失败
    IVS_CMU_XML_PARSE_STREAM_ERROR         = 149131010,    // XML Parse流失败
    IVS_CMU_DELETE_NET_LINK_ERROR          = 149131011,    // 删除网元link失败
    IVS_CMU_CREATE_LINK_ERROR              = 149131012,    // 动态创建网元连接失败
    IVS_CMU_DELETE_TRANSID_ERROR           = 149131013,    // 发送给自己的消息删除 TransID失败
    IVS_CMU_SET_SCHEDUE_MODE_FAIL          = 149131014,    // 禁制调度失败消息返回码
    IVS_CMU_CREATE_SESSION_ERROR           = 149131015,    // 创建会话失败
    IVS_CMU_REMOVE_WAIT_IMPORT_DATA_FLAG_ERROR = 149131016,// 删除待导回标志文件失败;
    IVS_CMU_IMPORT_NVR_DATA_ERROR          = 149131018,    // 保留数据组建集群失败
    IVS_CMU_NOT_LEADER_ERROR               = 149131019,    // 当Leader的业务发送给非leader节点的时候

	 /******************************************
	 *   OMU错误码定义（模块编码：15）
	 *******************************************/
	IVS_OMU_FILE_NOT_FOUND				   = 159100000,    // OMU 文件找不到
	IVS_OMU_PIPE_CREATION_FAILED           = 159100001,    // OMU 管道创建失败
	IVS_OMU_FORK_ERROR                     = 159100002,    // OMU 创建子进程失败
	IVS_OMU_FILE_RENAME_ERROR	           = 159100003,	   // 文件重命名失败
	//IVS_OMU_POINTER_ERROR	               = 159100004,	   // 空指针错误
	//IVS_OMU_STRING_PARSE_ERROR	       = 159100005,	   // 字符串解析错误
	IVS_OMU_FILE_EXIST_ERROR	           = 159100016,	   // 上传文件已存在
	//// 公共:15+91+00(公共)+序号;
//   IVS_OMU_CMD_ERROR                     = 159100000,    // 命令非法;
//   IVS_OMU_CMD_NULL                      = 159100001,    // 命令为空;
//   IVS_OMU_DECODE_ERROR                  = 159100002,    // 命令消息体解析错误;
//   IVS_OMU_MSGBLOCK_NULL                 = 159100003,    // 命令消息体为空;
//   IVS_OMU_XML_PARSE_ERROR               = 159100004,    // XML解析错误;
//   IVS_OMU_XML_PARAM_ERROR               = 159100005,    // XML参数错误;
    IVS_OMU_ROUTE_MSG_ERROR                = 159100006,    // 消息路由失败;
	//IVS_OMU_MSG_NOTIFY_FAIL              = 159100007,    // 通知其他模块失败
	IVS_OMU_SESSION_TIME_OUT               = 159100008,    // session超时;
	//IVS_OMU_NOTICE_FAIL_ERROR            = 159100009,    // OMU通知其他模块失败;
    // 设备管理:15+91+01(设备管理)+序号;
    IVS_OMU_NODE_OFFLINE                   = 159101001,    // 节点离线;
    IVS_OMU_MODULE_CFG_NOT_FOUND           = 159101005,    // OMU 模块配置不存在
    IVS_OMU_MODULE_CMD_NOT_FOUND           = 159101006,    // OMU 模块指令不存在
    IVS_OMU_MODULE_OPER_LIST_NOT_FOUND     = 159101007,    // OMU 元操作符为空
    IVS_OMU_NOT_DOWN_DOMAIN                = 159101008,    // 当前域包含子域，无法组建集群;
    IVS_OMU_HAS_NO_LICENSE		           = 159101009,    // 无授权许可，不可组建集群

	IVS_OMU_NOT_SINGLE_MODE                = 159101010,    // 不是单机模式，无法切换

	IVS_OMU_MSG_TRACE_REPORT_FAIL		   = 159147001,    // 添加消息跟踪结果到上报队列失败;

	IVS_OMU_REGISTER_PWD_INVALID           = 159103001,    // OMU 注册密码错误
	IVS_OMU_RE_REGISTER_INVALID            = 159103002,    // 不允许自己向自己注册
	IVS_OMU_REGISTER_LOOP_ERR              = 159103003,    // 注册存在回路，不允许注册
	IVS_OMU_REGISTER_RULE_ERR              = 159103004,    // 注册不符合规则
	IVS_OMU_MAX_MPU_COUNT_ERR              = 159103006,    // MPU达到最大上限
	IVS_OMU_MAX_VMU_COUNT_ERR              = 159103007,    // VMU达到最大上限
	IVS_OMU_MAX_PAS_COUNT_ERR              = 159103008,    // PAS达到最大上限
    IVS_OMU_NO_ACCEPT_NEW_NODE             = 159103009,    // 不允许新设备注册
	IVS_OMU_CLUSTER_NOT_ALLOW_SUB_DOMAIN   = 159103010,    // 集群下不允许对接子域
	IVS_OMU_REPORT_PWD_ERR			       = 159103011,    // 报表查询密码错误
    IVS_OMU_CLUSTER_SERVER_MUMBER_ERR      = 159103012,    // 组建集群server数量不足
    IVS_OMU_CLUSTER_MUMBER_ERR             = 159103013,   // 组建集群的NVR数量错误

    IVS_OMU_PORTAL_RESP_SUCCESS		       = 159102001,    // 登录成功
    IVS_OMU_PORTAL_PASSWD_WRONG            = 159102002,    // 用户名或密码输错
    IVS_OMU_PORTAL_RESP_DB_ERROR           = 159102003,    // 数据库错误
    IVS_OMU_PORTAL_RESP_XMLPRASE_ERROR     = 159102004,    // xml解析失败
    IVS_OMU_PORTAL_USER_LOCKED             = 159102005,    // 用户锁定
    IVS_OMU_PORTAL_RESP_SUCCESS_FIRST      = 159102006,    // 初始登录成功
    IVS_OMU_CAN_NOT_ADD_NVR_ERROR          = 159102007,    // 不能新增NVR
    IVS_OMU_CAN_NOT_ADD_I_ERROR            = 159102008,    // 新增NVR不能是自己

    IVS_OMU_CAN_NOT_READ_CFG_ERROR         = 159102009,    // 驱动包配置文件读取失败

    IVS_OMU_PORTAL_PASS_TIMEOUT            = 159102010,    // 强制清除密码

	IVS_OMU_VERSION_EXIST_ERR			   = 159104001,    // 版本文件已存在错误

	IVS_OMU_DIST_MONT_ISRUNNING		       = 159128001,    // 磁盘管理正在划分中

	IVS_OMU_CHANGE_NODE_TYPE_ERR           = 159129001,    // 存在上级或下级OMU，不允许改变节点类型

	IVS_OMU_PCG_OFF_LINE_ERR               = 159130001,    // PCG离线
	IVS_OMU_MAUS_OFF_LINE_ERR              = 159130002,    // MAUS离线
	IVS_OMU_HTTPS_OFF_LINE_ERR             = 159130003,    // HTTPS离线

	IVS_OMU_IS_SUBDOMAIN_ERR               = 159140001,    // 是域从，不允许改变IP
	IVS_OMU_IP_CONFIGING_ERR               = 159140002,    // IP地址修改中，不允许再次修改

    IVS_OMU_FAULT_RUNNING                  = 159140003,     //故障收集正在运行
    IVS_OMU_NOT_SUPPORT_CLUSTER            = 159140004,     //不支持集群
    IVS_OMU_QD_HAVE_LICENCE                = 159140005,     //QD有licence
    IVS_OMU_QD_HAVE_NO_LICENCE             = 159140006,     //QD没有licence
    IVS_OMU_FAIL_FOR_WITH_SLAVE            = 159140007,     //有域从不允许切单机
    IVS_OMU_WEAK_PASSWD                    = 159140008,     //弱口令
    IVS_OMU_CLOSE_DOMAIN_IN_NTP            = 159140009,     //先清除全网ntp
    IVS_OMU_CLOSE_DOMAIN_BT_NTP            = 159140010,     //先清除全网ntp
	/**********************************************
	*   MU错误码定义（模块编码：16）
	***********************************************/
	/* MU:bsm返回错误码，取值范围：169100001 ~ 169100100,errcode = IVS_MU_ERR_CODE + BSM_ERR_CODE*/
	IVS_MU_ERR_CODE_BEGIN                  = 169100001,    // BSM错误码起始值
    IVS_MU_EXIST_AUDIO                     = 169100044,    // 音频通道已被使用
	IVS_MU_RESTOR_XFS_FAILED               = 169100045,    // 恢复xfs文件系统失败
	IVS_MU_KEY_FILE_OPERATER_FAILED        = 169100046,    // 关键文件操作失败
	IVS_MU_ERROR_BLOCK_STATUS              = 169100047,    // 块状态错误.
	IVS_MU_ERROR_INVALID_BLOCK             = 169100048,    // 无效块号
	IVS_MU_DEVICE_STATUS_ERROR             = 169100049,    // 存储设备状态不正确
	IVS_MU_NEED_REBOOT_SYSTEM              = 169100050,    // 需要重启服务器
	IVS_MU_DEVICE_EXISTS                   = 169100051,    // 设备已存在
	IVS_MU_UNKNOWN_FILE_SYSTEM             = 169100052,
	IVS_MU_MOUNT_ERROR                     = 169100053,    // bsm 挂载盘失败
	IVS_MU_INVALID_PARAMETER               = 169100054,    // 无效的输入参数
	IVS_MU_SCAN_SLOT_FAILED                = 169100055,    // BSM扫描槽位出错
	IVS_MU_RAID_FAILED                     = 169100056,    // RAID模块未加载
	IVS_MU_SLEEPWAKEUPFAILED               = 169100057,    // 休眠失败
	IVS_MU_RAIDREBUILDING                  = 169100058,    // raid 重构中
	IVS_MU_DB_DUP_ERROR                    = 169100059,    // 数据库主键冲突
	IVS_MU_STREAM_PACKET_LEN_FAIL          = 169100060,    // 视频数据帧长度无效
	IVS_MU_INVALID_DISKNUM                 = 169100061,    // 无效的盘号
	IVS_MU_LOCK_OTHER_ERROR                = 169100062,    // 锁操作失败
	IVS_MU_SAVE_INDEX_FAILED               = 169100063,    // 保存索引失败
	IVS_MU_DISK_FAILED                     = 169100064,    // 磁盘错误(整盘失效)
	IVS_MU_NO_ENOUGH_RESERVESPACE          = 169100065,    // 没有足够的预留空间
	IVS_MU_INDEX_OUTDATED                  = 169100066,    // 已经循环覆盖的索引
	IVS_MU_INDEX_UNFOUND                   = 169100067,    // save session查询索引失败
	IVS_MU_DB_NO_DATA                      = 169100068,    // 数据库中没有相关数据
	IVS_MU_DB_EXCEPTION                    = 169100069,    // 数据库操作异常
	IVS_MU_INIT_DB_POLL_FAILED             = 169100070,    // 初始化数据连接池失败
	IVS_MU_DELETE_DB_BLOCKINFO_ERROR       = 169100071,    // 删除数据中块信息失败
	IVS_MU_OPERATING_RECLAIMED_IPC         = 169100072,    // 试图打开正在删除的摄像头
	IVS_MU_LOCK_MULTI_LOCK                 = 169100073,    // 试图多次锁定非递归锁
	IVS_MU_LOCK_RECURSIVE_EXCEED           = 169100074,    // 线程递归锁已经到达最大递归数限制
	IVS_MU_LOCK_NOT_INIT_OK                = 169100075,    // 线程互斥锁没有初始化
	IVS_MU_TOO_LARGE_DATA_SIZE             = 169100076,    // 图片尺寸超过1M, 超出BSM预留的1M
	IVS_MU_INVALID_ID                      = 169100077,    // 无效摄像头ID
	IVS_MU_CREATE_FOLDER_FAILED            = 169100078,    // 创建文件夹失败
	IVS_MU_WRONG_SESSION_TYPE              = 169100079,    // 所要求的操作和当前会话类型不匹配
	IVS_MU_INVALID_SESSION_ID              = 169100080,    // 无效的会话ID
	IVS_MU_INVALID_SESSION_TYPE            = 169100081,    // 无效的会话类型
	IVS_MU_INVALID_POINTER                 = 169100082,    // 无效指针
	IVS_MU_NEW_FAILED                      = 169100083,    // new操作失败
	IVS_MU_NO_BLOCK                        = 169100084,    // Spaces无法分配数据块
	IVS_MU_NO_PHOTO_DATA                   = 169100085,    // 没有相关图片数据
	IVS_MU_INVALID_TIME                    = 169100086,    // 无效时间
	IVS_MU_PHOTO_RTTI_ERROR                = 169100087,    // 存储图片校验标志
	IVS_MU_END_FLAG_ERROR                  = 169100088,    // 存储空间尾标志错误
	IVS_MU_START_FLAG_ERROR                = 169100089,    // 存储空间头标志错误
	IVS_MU_INVALID_SIZE                    = 169100090,    // 无效长度
	IVS_MU_BUFFER_NOT_ENOUGH               = 169100091,    // 缓存空间不足
	IVS_MU_UNKNOW_EXCEPTION                = 169100092,    // 未知异常
	IVS_MU_ISTREAM_CREATE_FAILED           = 169100093,    // 输入流创建错误
	IVS_MU_OSTREAM_CREATE_FAILED           = 169100094,    // 输出流创建错误
	IVS_MU_SEEK_FAILED                     = 169100095,    // 文件定位错误
	IVS_MU_OPEN_FAILED                     = 169100096,    // 打开存储文件错误
	IVS_MU_READ_FAILED                     = 169100097,    // 读磁盘错误
	IVS_MU_WRITE_FAILED                    = 169100098,    // 写磁盘错误

	IVS_MU_ERR_CODE                        = 169100100,    // BSM错误码起始值

	IVS_MU_RECORD_SEARCH_INDEX_ERROR	   = 169100200,    // 录像检索索引错误
	IVS_MU_FROM_TO_TIME_ERROR			   = 169100201,	   // 录像检索开始结束时间错误
	IVS_MU_TIME_FORMAT_ERROR			   = 169100202,	   // 录像检索时间格式错误
	IVS_MU_RECORD_METHOD_ERROR			   = 169100203,	   // 录像检索方式错误
	IVS_MU_CAMERA_CODE_ERROR			   = 169100204,	   // 录像检索摄像机编码错误
	IVS_MU_RECORD_SEARCH_NSS_ERROR		   = 169100205,    // 录像检索NSS消息错误
	IVS_MU_REPORT_DISK_RATE_ERROR		   = 169100206,    // 查询磁盘信息失败错误

    IVS_MU_MBU_OVERLOAD                    = 169100300,    // mbu已经过载


	/******************************************
	*   SafeVideo错误码定义（模块编码：17）
	*******************************************/

    /******************************************
    *   DCG错误码定义（模块编码：18）
    *******************************************/
    //DCG:录像计划配置 错误码 功能编码-25
    IVS_DCG_SET_RECORD_ERROR              = 189125001,     // 设置录像计划错误
    IVS_DCG_GET_RECORD_ERROR              = 189125002,     // 获取录像计划错误
    IVS_DCG_SET_LAYER_CODE_ERROR          = 189125003,     // 分层编码设置错误
    IVS_DCG_RECORD_PLAN_NOT_EXIST         = 189125004,     // 录像计划为空
    IVS_DCG_CREATE_RECORD_STREAM_SMP_ERROR= 189125005,     // 创建录像流对象错误
    //DCG:云镜控制 错误码 功能编码-16
    IVS_DCG_CONTROL_PTZ_ERROR             = 189116001,     // 云台控制错误
    IVS_DCG_GET_PTZ_PARAM_ERROR           = 189116002,     // 获取云台参数错误
    IVS_DCG_SET_PTZ_PARAM_ERROR           = 189116003,     // 设置云台参数错误
    IVS_DCG_GET_PTZ_PROTOCOL_ERROR        = 189116004,     // 获取云台解码器协议错误
    IVS_DCG_GET_CRUISE_TRACK_ERROR        = 189116005,     // 获取巡航轨迹参数错误
    IVS_DCG_SET_CRUISE_TRACK_ERROR        = 189116006,     // 设置巡航轨迹参数错误
    IVS_DCG_CLEAL_CRUISE_TRACK_ERROR      = 189116007,     // 清空巡航列表错误
    IVS_DCG_TRACK_COMMAND_ERROR           = 189116008,     // 云台PTZ轨迹控制失败
    IVS_DCG_CRUISE_NUMBER_ERROR           = 189116009,     // 设置的巡航轨迹序号超过设备支持范围
    IVS_DCG_UNSUPPORT_PTZ_CMD             = 189116010,     // 不支持的云台控制命令
    IVS_DCG_TRACK_INDEX_ERROR             = 189116011,     // 设置的PTZ录制轨迹序号超过设备支持范围
    IVS_DCG_GET_ABS_POSITION_ERROR        = 189116012,     // 获取云台坐标参数

    //DCG:预置位,看守位控制 错误码 功能编码-17
    IVS_DCG_SET_PRESET_ERROR              = 189117001,     // 设置预置位参数错误
    IVS_DCG_GET_PRESET_ERROR              = 189117002,     // 查询预置位参数错误
    IVS_DCG_DEL_PRESET_ERROR              = 189117003,     // 删除预置位参数错误
    IVS_DCG_SET_GUARD_ERROR               = 189117004,     // 设置看守位请求错误
    IVS_DCG_GET_GUARD_ERROR               = 189117005,     // 查询看守位请求错误
    IVS_DCG_CANNOT_OVERITE_HOME_POSITION  = 189117006,     // 当前位置可能为保留预置位，不能设置成看守位
    IVS_DCG_CANNOT_SET_HOME_POS_WHILE_PTZ_MOVING  = 189117007,     // ptz正在转动,不能设置看守位

    //DCG:告警管理 错误码 功能编码-20
    IVS_DCG_SET_ALARM_GUARD_ERROR         = 189120001,     // 设置告警布撤防错误
    IVS_DCG_GET_ALARM_GUARD_ERROR         = 189120002,     // 获取镜头布撤防参数错误
    IVS_DCG_MAIN_DEV_ALARM_GUARD_ERROR    = 189120003,     // 主设备告警布防错误
    IVS_DCG_MAIN_DEV_ALARM_UNGUARD_ERROR  = 189120004,     // 主设备告警撤防错误
    IVS_DCG_MAIN_DEV_SET_ALARM_CALLBACK_ERROR = 189120005, // 主设备设置告警回调错误
    IVS_DCG_SET_VIDEO_MASK_ERROR          = 189120006,     // 设置隐私保护请求错误
    IVS_DCG_GET_VIDEO_MASK_ERROR          = 189120007,     // 查询隐私保护请求错误
    IVS_DCG_SET_MOTION_DETECTION_ERROR    = 189120008,     // 设置移动侦测请求错误
    IVS_DCG_GET_MOTION_DETECTION_ERROR    = 189120009,     // 查询移动侦测请求错误
    IVS_DCG_SET_MASK_PROTECT_ERROR        = 189120010,     // 设置视频遮盖请求错误
    IVS_DCG_GET_MASK_PROTECT_ERROR        = 189120011,     // 查询视频遮盖请求错误
    IVS_DCG_SET_IGT_COMMEM_PARA_ERROR     = 189120012,     // 设置智能分析公共参数错误
    IVS_DCG_SET_IGT_TRIPWIRE_PARA_ERROR   = 189120013,     // 设置智能分析绊线规则参数错误
    IVS_DCG_SET_IGT_INTRUSION_PARA_ERROR  = 189120014,     // 设置智能分析入侵检测参数错误
    IVS_DCG_SET_IGT_HOVER_PARA_ERROR      = 189120015,     // 设置智能分析徘徊检测参数错误
    IVS_DCG_SET_IGT_LEGACY_PARA_ERROR     = 189120016,     // 设置智能分析遗留检测参数错误
    IVS_DCG_SET_IGT_REMOVE_PARA_ERROR     = 189120017,     // 设置智能分析移走检测参数错误
    IVS_DCG_GET_IGT_COMMEM_PARA_ERROR     = 189120018,     // 获取智能分析公共参数错误
    IVS_DCG_GET_IGT_TRIPWIRE_PARA_ERROR   = 189120019,     // 获取智能分析绊线规则参数错误
    IVS_DCG_GET_IGT_INTRUSION_PARA_ERROR  = 189120020,     // 获取智能分析入侵检测参数错误
    IVS_DCG_GET_IGT_HOVER_PARA_ERROR      = 189120021,     // 获取智能分析徘徊检测参数错误
    IVS_DCG_GET_IGT_LEGACY_PARA_ERROR     = 189120022,     // 获取智能分析遗留检测参数错误
    IVS_DCG_GET_IGT_REMOVE_PARA_ERROR     = 189120023,     // 获取智能分析移走检测参数错误
    IVS_DCG_SET_ALARM_LINK_PARAM_ERROR    = 189120024,     // 设置告警联动策略参数错误
    IVS_DCG_ALARM_LINK_CANT_FIND_CHANNEL  = 189120025,     // 摄像头编码找不到对应的通道
    IVS_DCG_SET_ANALYTICS_ERROR   = 189120026,     // 设置智能分析错误
    IVS_DCG_ANALYTICS_UNSUPPORT  = 189120027,     // 该设备智能分析不支持平台设置，请到WEB页面设置

    //DCG:设备管理 错误码 功能编码-08
    IVS_DCG_FIND_MAIN_DEV_EXIST           = 189108001,     // 查找主设备已经存在
    IVS_DCG_ADD_MAIN_DEV_ERROR            = 189108002,     // 添加主设备错误
    IVS_DCG_LOAD_PLUGIN_ERROR             = 189108003,     // 加载设备插件错误
    IVS_DCG_GET_DEV_PORT_ERROR            = 189108004,     // 获取设备端口错误
    IVS_DCG_FIND_MAIN_DEV_ERROR           = 189108005,     // 查找主设备不存在
    IVS_DCG_DEL_SUB_DEV_ERROR             = 189108006,     // 删除子设备错误
    IVS_DCG_DEL_MAIN_DEV_ERROR            = 189108007,     // 删除主设备错误
    IVS_DCG_REBOOT_MAIN_DEV_ERROR         = 189108008,     // 重启主设备错误
    IVS_DCG_OPEN_DLL_ERROR                = 189108009,     // Open动态库错误
    IVS_DCG_SYMBOL_DLL_ERROR              = 189108010,     // 动态库symbol错误
    IVS_DCG_CREATE_SDK_DLL_ERROR          = 189108011,     // 动态库CreateSdk错误
    IVS_DCG_SDK_INIT_ERROR                = 189108012,     // Sdk初始化错误
    IVS_DCG_DEV_LOGIN_ERROR               = 189108013,     // 主设备登录错误
    IVS_DCG_DEV_LOGIN_PASSWORD_ERROR      = 189108014,     // 主设备登录密码错误
    IVS_DCG_DEV_GET_ABILITY_ERROR         = 189108015,     // 主设备获取设备能力错误
    IVS_DCG_DEV_SDK_INIT_ERROR            = 189108016,     // 初始化设备SDK错误
    IVS_DCG_CREATE_LENS_ERROR             = 189108017,     // 创建镜头失败
    IVS_DCG_GET_TIME_PARAM_ERROR          = 189108018,     // 获取时间参数失败
    IVS_DCG_SET_TIME_PARAM_ERROR          = 189108019,     // 设置时间参数失败
    IVS_DCG_GET_NET_PARAM_ERROR           = 189108020,     // 获取网络参数失败
    IVS_DCG_SET_NET_PARAM_ERROR           = 189108021,     // 设置网络参数失败
    IVS_DCG_BIND_TRANSID_AND_SEARCHD_DEV_TASK_ERROR    = 189108022, //绑定事物ID与查找设备业务失败
    IVS_DCG_UNBIND_TRANSID_AND_SEARCHD_DEV_TASK_ERROR  = 189108023, //解除事物ID与查找设备业务绑定失败
    IVS_DCG_SEARCH_DEVICE_ERROR           = 189108024,     // 搜索设备失败
    IVS_DCG_CAN_NOT_FIND_SERACHDEVTASK    = 189108025,     // 查找不到搜索设备的事物
    IVS_DCG_STOP_SEARCH_DEVICE_ERROR      = 189108026,     // 停止搜索设备失败
    IVS_DCG_CREATE_VENDER_DEV_ERROR       = 189108027,     // 创建适配层设备失败
    IVS_DCG_UN_SUPPORT_PROTOCOL           = 189108028,     // 不支持的协议类型
    IVS_DCG_VERSION_NO_MATCH              = 189108029,     // SDK与设备版本不匹配(针对自研SDK)
    IVS_DCG_DEV_DISCOVERY_RUNNING         = 189108030,     // 设备自动发现服务已经在运行
    IVS_DCG_VENDOR_DEV_INVALID            = 189108031,     // 主设备句柄无效
    IVS_DCG_DEV_LOGIN_HANDLE_INVALID      = 189108032,     // 设备登录句柄无效
    IVS_DCG_DEV_UN_SUPPORT                = 189108033,     // 设备不支持
    IVS_DCG_DEV_UPDATE_ERROR              = 189108034,     // 设备升级失败
    IVS_DCG_UNAUTHORIZED                  = 189108035,     // 未鉴权
    IVS_DCG_HTTP_DIGEST_AUTHORIZE_ERROR   = 189108036,     // Http-Digest鉴权错误
    IVS_DCG_WSSE_AUTHORIZE_ERROR          = 189108037,     // WSSE鉴权错误
    IVS_DCG_ONVIF_SOAP_OBJECT_INVALID     = 189108038,     // SOAP对象无效
    IVS_DCG_ONVIF_SOAP_OPER_FAIL          = 189108039,     // SOAP请求失败
    IVS_DCG_REGIST_EVENT_CALL_BACK_ERROR  = 189108040,     // sdk注册前端异常回调错误
    IVS_DCG_ONVIF_APP_OBJECT_INVALID      = 189108041,     // ONVIF App对象无效
    IVS_DCG_FIND_SUB_DEV_EXIST            = 189108042,     // 查找子设备已经存在
    IVS_DCG_HTTP_NOT_IMPLEMENTED          = 189108043,     // HTTP访问501错误
    IVS_DCG_SET_CONFIG_FILE_ERROR         = 189108044,     // 设置配置文件错误
    IVS_DCG_GET_CONFIG_FILE_ERROR         = 189108045,     // 获取配置文件错误
    IVS_DCG_ONVIF_NO_CONF                 = 189108046,     // ONVIF无相关配置信息
    IVS_DCG_MAIN_DEV_REBOOTING            = 189108047,     //主设备重启中
    IVS_DCG_DEV_LOGIN_USER_LOCKED         = 189108048,     //主设备登录锁定
    IVS_DCG_TIMEZONE_SHIFT_INVALID              = 189108049, //时区偏移量不合法
    IVS_DCG_FRAMERATE_INVALID                      = 189108050, //帧率不合法
    IVS_DCG_BITRATE_INVALID                             = 189108051, //码率不合法
    IVS_DCG_RESOLUTION_INVALID                      = 189108052, //分辨率不合法
    IVS_DCG_PROFILE_INVALID                             = 189108053, //profile不合法
    IVS_DCG_IFRAMEINTERVAL_INVALID               = 189108054, //I帧间隔不合法
    IVS_DCG_PU_LOG_NOT_FOUND               = 189108055, //前端日志未找到
    IVS_DCG_PU_SET_VEDIO_FORMAT_ERROR       = 189108056, //设置前端PN制式失败

    //DCG:实况 错误码 功能编码-15
    IVS_DCG_ADD_STREAM_PARAM_ERROR        = 189115001,     // 添加请求码流参数错误
    IVS_DCG_FIND_STREAM_PARAM_EXIST       = 189115002,     // 查找码流参数已经存在
    IVS_DCG_FIND_STREAM_PARAM_ERROR       = 189115003,     // 查找码流参数错误、不存在
    IVS_DCG_OPEN_SOCKET_ERROR             = 189115004,     // 初始化socket错误
    IVS_DCG_START_STREAM_ERROR            = 189115005,     // 开始取流错误
    IVS_DCG_TCP_OPEN_SOCKET_ERROR         = 189115006,     // TCP-socket打开错误
    IVS_DCG_TCP_SOCKET_CONNECT_ERROR      = 189115007,     // TCP-socket连接错误
    IVS_DCG_UDP_OPEN_SOCKET_ERROR         = 189115008,     // UDP-socket打开错误
    IVS_DCG_STOP_REAL_STREAM_ERROR        = 189115009,     // 停止流错误
    IVS_DCG_REQUEST_IFRAME_ERROR          = 189115010,     // 请求I帧错误
    IVS_DCG_STREAM_HANDLE_INVALID         = 189115011,     // 流句柄无效
	IVS_DCG_SEND_SIP_INVITE_ERROR         = 189115012,     // 向千里眼设备发送sip invite错误
    IVS_DCG_SIP_INVITE_RESPONSE_ERROR     = 189115013,     // 千里眼设备回复sip invite响应错误
    IVS_DCG_MEDIA_BUF_WITH_RTP_INVALID    = 189115014,     // 带RTP头的海康、大华流需要使用该指针，为空时报错
    IVS_DCG_BIND_SERVICE_ID_AND_MU_LINK_ID_ERROR   = 189115015,   // 绑定ServiceID和MULinkID出错
    IVS_DCG_UNBIND_SERVICE_ID_AND_MU_LINK_ID_ERROR = 189115016,   // 解除绑定ServiceID和MULinkID出错
    IVS_DCG_BIND_SERVICE_ID_AND_MSGKEY_ERROR       = 189115017,   // 千里眼流请求的时候绑定ServiceID和MsgKey错误
    IVS_DCG_UNBIND_SERVICE_ID_AND_MSGKEY_ERROR     = 189115018,   // 千里眼流请求的时候解除绑定ServiceID和MsgKey错误
    IVS_DCG_BIND_SERVICE_ID_AND_SESSION_ID_ERROR   = 189115019,   // 千里眼流请求的时候绑定ServiceID和千里眼消息中的会话ID错误
    IVS_DCG_UNBIND_SERVICE_ID_AND_SESSION_ID_ERROR = 189115020,   // 千里眼流请求的时候解除绑定ServiceID和千里眼消息中的会话ID错误
    IVS_DCG_TRANSFER_PAYLOADTYPE_FAILED    = 189115021,     // payloadtype转换错误
    IVS_DCG_STREAM_UNSTARTED               = 189115022,     // 流未启动
    IVS_DCG_SET_WATERMARK_ERROR            = 189115023,     // 设置水印错误
    IVS_DCG_TCP_SOCKET_ACCEPT_ERROR        = 189115024,     // TCP-SOCKET-Accept错误
    IVS_DCG_TCP_SOCKET_ACCEPTOR_CLOSE_ERROR= 189115025,     // TCP-SOCKET-Acceptor close错误
    IVS_DCG_REV_STREAM_REQUEST_ERROR       = 189115026,     // 处理流请求错误，主要是千里眼设备发送Invt消息的时候错误
    IVS_DCG_UNSUPPORTED_ENCODE_MODE        = 189115028,     // 不支持的编码格式
    IVS_DCG_UNSUPPORTED_PROFILE            = 189115029,     // 不支持的Profile
    IVS_DCG_DECRYPT_SECURITY_ERROR         = 189115030,     // 解密失败
    IVS_DCG_STREAM_HAS_STOPED                     = 189115031,     // 实况流已停止
    IVS_DCG_WRONG_STOPPING_STREAM_WITH_DEV = 189115032,     //设备停流失败
    IVS_DCG_IS_IN_START                    = 189115033,     //此流业务还处于流请求和ACK中
    IVS_DCG_IS_IN_STOP                     = 189115034,     //此流业务还处于流停止中

    //DCG:开关量输入子设备          10
    IVS_DCG_GET_ALARM_IN_PARAM_ERROR      = 189110001,     // 获取开关量子设备参数错误
    IVS_DCG_SET_ALARM_IN_PARAM_ERROR      = 189110002,     // 设置开关量子设备参数错误

    //DCG:设备管理-开关量输出子设备  11
    IVS_DCG_GET_ALARM_OUT_PARAM_ERROR     = 189111001,     // 获取开关量子设备参数错误
    IVS_DCG_SET_ALARM_OUT_PARAM_ERROR     = 189111002,     // 设置开关量子设备参数错误
    IVS_DCG_GET_ALARM_OUT_STATE_ERROR     = 189111003,     // 获取开关量子设备参数错误
    IVS_DCG_SET_ALARM_OUT_STATE_ERROR     = 189111004,     // 设置开关量子设备参数错误

    //DCG:摄像机子设备              09
    IVS_DCG_GET_CAMERA_STREAM_PARAM_ERROR = 189109001,     // 获取摄像机码流参数错误
    IVS_DCG_SET_CAMERA_STREAM_PARAM_ERROR = 189109002,     // 设置摄像机码流参数错误
    IVS_DCG_GET_IMAGING_PARAM_ERROR       = 189109003,     // 获取摄像机基本图像参数错误
    IVS_DCG_SET_IMAGING_PARAM_ERROR       = 189109004,     // 设置摄像机基本图像参数错误
    IVS_DCG_GET_VIDEO_PARAM_ERROR         = 189109005,     // 获取前端视频子设备参数错误
    IVS_DCG_SET_VIDEO_PARAM_ERROR         = 189109006,     // 设置前端视频子设备参数错误
    IVS_DCG_GET_VIDEO_EFFECT_PARAM_ERROR  = 189109007,     // 获取前端视频效果参数错误
    IVS_DCG_SET_VIDEO_EFFECT_PARAM_ERROR  = 189109008,     // 设置前端视频效果参数错误
    IVS_DCG_GET_VIDEO_ANALOG_FORMAT_ERROR = 189109009,     // 获取前端视频制式错误
    IVS_DCG_SET_VIDEO_ANALOG_FORMAT_ERROR = 189109010,     // 设置前端视频制式错误
    IVS_DCG_SET_OSD_ERROR                 = 189109011,     // 设置OSD参数错误
    IVS_DCG_GET_OSD_ERROR                 = 189109012,     // 获取OSD参数错误
    IVS_DCG_GET_EXTIMAGING_PARAM_ERROR    = 189109013,     // 获取摄像机扩展图像参数错误
    IVS_DCG_SET_EXTIMAGING_PARAM_ERROR    = 189109014,     // 设置摄像机扩展图像参数错误
    IVS_DCG_FIND_LENS_INVALID             = 189109015,     // 未查找到摄像机子设备
    IVS_DCG_VENDOR_LENS_INVALID           = 189109016,     // 摄像机子设备句柄无效
    IVS_DCG_SET_FEC_ERROR                 = 189109017,     // 设置FEC参数错误
    IVS_DCG_GET_FEC_ERROR                 = 189109018,     // 获取FEC参数错误
    IVS_DCG_SET_VIDEO_ENCODE_FORMAT_ERROR                 = 189109019,     // 设置视频编码格式错误

    //DCG:设备管理-语音子设备	       12
    IVS_DCG_GET_AUDIO_PARAM_ERROR         = 189112001,     // 获取音频子设备参数错误
    IVS_DCG_SET_AUDIO_PARAM_ERROR         = 189112002,     // 设置音频子设备参数错误

    //DCG:设备管理-串口子设备	       13
    IVS_DCG_GET_SERIAL_PARAM_ERROR        = 189113001,     // 获取串口子设备参数错误
    IVS_DCG_SET_SERIAL_PARAM_ERROR        = 189113002,     // 设置串口子设备参数错误

    //DCG:设备管理-语音	       45
    IVS_DCG_START_VOICE_STREAM_ERROR      = 189145001,     // 开始音频对讲错误
    IVS_DCG_STOP_VOICE_STREAM_ERROR       = 189145002,     // 停止音频对讲错误
    IVS_DCG_START_REVERSE_AUDIO_THREAD_ERROR = 189145003,  // 开启接受CU客户端语音数据线程失败

    //DCG:日志管理                  30
    IVS_DCG_SET_LOG_CONFIG_ERROR          = 189130001,     // 设置日志开关参数错误
    IVS_DCG_GET_LOG_SFTP_CONF_ERROR       = 189130002,     // 获取日志统一配置错误
    IVS_DCG_SET_LOG_SFTP_CONF_ERROR       = 189130003,     // 设置日志统一配置错误
    IVS_DCG_GET_PU_LOG_ERROR              = 189130004,     // 获取前端日志失败

    //DCG：前端录像   40
    IVS_DCG_START_PU_RECORD_ERROR         = 189140001,     // 开启前端手动录像错误
    IVS_DCG_STOP_PU_RECORD_ERROR          = 189140002,     // 停止前端手动录像错误
    IVS_DCG_GET_PU_RECORD_STATE_ERROR     = 189140003,     // 获取前端录像状态错误
    IVS_DCG_GET_PU_RECORD_LIST_ERROR      = 189140004,     // 检索前端录像错误
    IVS_DCG_STOP_PLAYBACK_STREAM_ERROR    = 189140005,     // 停止前端录像回放错误
    IVS_DCG_START_PLAYBACK_STREAM_ERROR   = 189140006,     // 开启前端录像回放错误
    IVS_DCG_SET_RECORD_STREAM_ERROR       = 189140007,     // 设置前端录像码流类型错误

    //DCG：录像回放             19
    IVS_DCG_PLAYBACK_BY_NAME_ERROR        = 189119001,     // 根据录像文件名错误
    IVS_DCG_SET_PLAYBACK_CALLBACK_ERROR   = 189119002,     // 设置录像回放回调函数错误
    IVS_DCG_PLAYBACK_START_PLAYBACK_ERROR = 189119003,     // 启动录像回调错误
    IVS_DCG_PLAYBACK_PLAYRESTART_ERROR    = 189119004,     // 回放恢复播放错误
    IVS_DCG_PLAYBACK_PLAYPAUSE_ERROR      = 189119005,     // 回放暂停错误
    IVS_DCG_PLAYBACK_PLAYFAST_ERROR       = 189119006,     // 回放快进错误
    IVS_DCG_PLAYBACK_PLAYSLOW_ERROR       = 189119007,     // 回放慢放错误
    IVS_DCG_PLAYBACK_PLAYSETTIME_ERROR    = 189119008,     // 回放设置播放时间错误
    IVS_DCG_PLAYBACK_LIST_IS_EMPTY_ERROR  = 189119009,     // 录像回放录像下标list为空错误
    IVS_DCG_PLAYBACK_INDEX_VALUE_ERROR    = 189119010,     // 从录像list拿出来的index错误
    IVS_DCG_STRTEAM_CONTROL_TIME_INVALID  = 189119011,     // 时间拖动，下发时间超过录像长度
	IVS_DCG_PLEASE_RESEARCH_RECORD_LIST   = 189119012,     // 请重新检索录像
    //DCG：修改密码             26
    IVS_DCG_MOD_DEV_PASSWORD_ERROR        = 189126001,     // 修改前端密码错误

    //DCG：抓拍管理             46
    IVS_DCG_GET_SNAP_SHOT_ERROR           = 189146001,     // 平台抓拍失败
    IVS_DCG_PU_HAVE_NO_STORAGE_DEVICE     = 189146002,     // 前端设备没有存储设备

    //DCG: DCG管理              38
    IVS_DCG_DEAL_SESSION_TIME_OUT         = 189138001,     //处理消息超时

    //DCG:外部系统管理          39
    IVS_DCG_GET_EXTERN_DEV_LIST_ERROR     = 189139001,     // 获取外部系统设备列表错误
    IVS_DCG_SET_EXTERN_DEV_LIST_ERROR     = 189139002,     // 设置外部系统设备列表错误
    IVS_DCG_GET_SIPASS_UNIT_TABLE_ERROR   = 189139003,     // 获取西门子unit表错误
    IVS_DCG_GET_SIPASS_DEVICE_TABLE_ERROR = 189139004,     // 获取西门子deviec表错误
    IVS_DCG_GET_SIPASS_POINT_TABLE_ERROR  = 189139005,     // 获取西门子point表错误
    IVS_DCG_GET_SIPASS_FLN_TABLE_ERROR    = 189139006,     // 获取西门子FLN表错误
    IVS_DCG_FIND_EXT_DEV_ERROR            = 189139007,     // 获取map中，外部设备错误
    IVS_DCG_ADD_EXTSUBDEV_ECHO_ERROR      = 189139008,     // 添加外部设备时，设备已经存在
	IVS_DCG_SIPASS_CON_SER_REG_ERROR      = 189139009,     // 连接服务器注册表错误
    IVS_DCG_SIPASS_OPERATE_REGEDIT_ERROR  = 189139010,     // 操作注册表错误
    IVS_DCG_SIPASS_GET_LO_REG_CLSID_ERROR = 189139011,     // 获取本地注册表，CLSID错误
    IVS_DCG_SIPASS_CREATE_INSTANCE_ERROR  = 189139012,     // 创建COM对象错误
    IVS_DCG_SIPASS_SET_ATL_ADVISE_ERROR   = 189139013,     // 设置告警上报接口错误
    IVS_DCG_EXT_SYS_IS_NOT_ONLINE_ERROR   = 189139014,     // 外部系统还未注册上线
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
	IVS_PCG_XML_DECODE_ERROR			   = 279139001, 	// xml解析错误
	IVS_PCG_ERROR 						   = 279139000,		// 服务器内部错误
    IVS_PCG_PARA_ERROR 					   = 279140000, 	// 参数校验失败
	IVS_PCG_CONN_CODE_ALREADY_EXIST 	   = 279139003, 	// 域编码已经存在
	IVS_PCG_EXCEED_EXTERN_MAX_NUM 		   = 279139004, 	// 外域数量超过128
	IVS_PCG_DEVICEID_REPEAT 			   = 279139005, 	// 设备互联编码重复
	IVS_PCG_EXIST_EXTERN_DOMAIN_PROTOCOL   = 279139006, 	// 存在该协议类型的外域
	IVS_PCG_SHARE_RES_ITSELF_FAIL          = 279139012,     // 外域设备不能再次共享给外域本身
	IVS_PCG_EXDOMAIN_PWD_VALIDATE          = 279139013,     // 外域密码不符合密码设置规则，校验失败
	IVS_PCG_NOT_SUPPORT_REQUEST_FUNCTION   = 279139014,     // 外域设备不支持请求的功能
	IVS_PCG_SC_BUSY_HERE   = 279142002,     // 外域带宽超出最大限制

	/******************************************
	*   MBU错误码定义（模块编码：28）
	*******************************************/

	/******************************************
	*   MAUS错误码定义（模块编码：29）
	*******************************************/
    IVS_MAU_ERR_CODE                       = 299133200,     // MAU通用错误起始值
    IVS_MAU_FAILURE                        = 299133201,     // 响应失败
    IVS_MAU_SUCCESS_FIRST		           = 299133210,     // 首次登陆成功
    IVS_MAU_USER_LOCKED		               = 299133211,     // 用户锁定
    IVS_MAU_NO_ANALYSIS_UNIT               = 299133212,     // MAU未连接分析单元

    IVS_MAU_AUTHENTICATE_FAILURE           = 299133301,     // 登陆认证失败；
    IVS_MAU_TRANS_NOT_EXIST                = 299133501,     // 事务不存在，指定的事务不存在；
    IVS_MAU_FRONTDEV_NOT_EXIST             = 299133502,     // 主设备不存在，在查询主设备参数时，如果主设备不存在返回，其它错误情况，没有特别说明的，都返回
    IVS_MAU_LENS_NOT_EXIST                 = 299133503,     // 镜头设备不存在，在查询镜头设备参数时，如果镜头设备不存在返回，其它错误情况，没有特别说明的，都返回
    IVS_MAU_OTHER_ERROR                    = 299133601,     // 其他错误
    IVS_MAU_DB_ERROR                       = 299133602,     // 数据库操作异常
    IVS_MAU_PARAM_ERROR                    = 299133603,     // 参数异常
    IVS_MAU_RUNTIME_ERROR                  = 299133604,     // 运行时异常
    IVS_MAU_XMLPRASE_ERROR                 = 299133605,     // XML解析异常
    IVS_MAU_OVERLOAD_ERROR                 = 299133607,     // 过载异常
    IVS_MAU_RULEGROUP_NOTEXIST             = 299133608,     // 规则不存在
    IVS_MAU_PLANID_NOTEXIST                = 299133609,     // 计划信息不存在
    IVS_MAU_PLAN_TIME_CONFLICT             = 299133613,     // analyze Plan时间段重叠RESP_PLANANA_TSCONFL_ERROR
    IVS_MAU_PLAN_STARTORSTOPPING           = 299133614,     // 计划正在启停RESP_PLAN_STARTSTOPING
    IVS_MAU_REPEAT_OPRATION_ERROR          = 299133615,     // 重复操作

    IVS_MAU_FACEDBTYPE_NOT_EXIST           = 299133616,     // 人脸类型库不存在
    IVS_MAU_FACEDBTYPE_EXIST               = 299133617,     // 人脸库类型已经存在
    IVS_MAU_FACEDBNAME_EXIST               = 299133618,     // 人脸库类型名称已经存在
    IVS_MAU_FACE_NOT_DETECT                = 299133619,     // 未检测到人脸
    IVS_MAU_REQ_ENROLL2FACESRV_FAIL        = 299133620,     // 请求中转服务器注册人像模板失败
    IVS_MAU_REQ_ENROLL2FACESRV_EXCP        = 299133621,     // 请求中转服务器注册人像模板异常
    IVS_MAU_REQ_DELFROMFACESRV_EXCP        = 299133622,     // 请求节点服务器删除人像模板异常
    IVS_MAU_REQ_DELFROMFACESRV_FAIL        = 299133623,     // 请求节点服务器删除人像模板失败
    IVS_MAU_IMAGE_FORMAT_INVALIDATE        = 299133624,     // 图片格式不对
    IVS_MAU_REQ_UPDATE2FACESRV_EXCP        = 299133625,     // 请求中转服务器更新人像模板失败
    IVS_MAU_REQ_MEDIAANATYPE_ERR           = 299133626,     // 智能分析请求类型不正确
    IVS_MAU_NOT_FIND_MATCHD_FACE           = 299133628,     // 未找到相似的人脸
    IVS_MAU_FACEID_EXIST                   = 299133629,     // 人脸ID已存在
    IVS_MAU_FACEID_NOEXIST                 = 299133630,     // 人脸ID不存在


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
	 IVS_BP_SUCCEED                       = 979199000,     // 成功;
	 IVS_BP_ERROR                         = 979199001,     // 失败;
	 IVS_BP_PARA_INVALID                  = 979199002,     // 输入参数非法
	 IVS_BP_THREAD_BUSY                   = 979199003,     // 线程繁忙
	 IVS_BP_NOT_SUPPORT					  = 979199004,     // 消息类型不支持


	 /******************************************
	 *   VWU错误码定义（模块编码：98）
	 *******************************************/



	/******************************************
	*   CU错误码定义（模块编码：99）
    *   错误码后三位编号：OCX 000～199 SDK 200～399 MP 400～599
	*******************************************/
	// OCX 000～199
	//IVS_OCX_CU_XML_INVALID                 = 999115000,     // CU给OCX的查询XML非法;
	//IVS_OCX_ALLOC_MEMORY_ERROR             = 999115001,     // CU内存申请失败;
	//IVS_OCX_INTRINSIC_PTR_ERROR            = 999115002,     // CU内部指针错误;
	IVS_OCX_SDK_INIT_FAIL                  = 999115003,     // OCX初始化SDK失败;
    //IVS_OCX_XML_NODE_VALUE_ERROR           = 999115004,     // XML节点值不正确
	IVS_OCX_RET_SET_DEVNAME_ERROR          = 999115005,     // 设置设备名称失败
	IVS_OCX_RET_VIDEOPANE_IS_PLAYING_ERROR = 999115006,     // 窗格正在播放视频错误
	IVS_OCX_RET_GET_ACTIVE_WND_ID_ERROR    = 999115007,     // 获取活动窗格ID失败
	//IVS_OCX_RET_GET_WND_POINTER_ERROR      = 999115008,     // 获取活动窗格指针失败
	//IVS_OCX_RET_ALREADY_INIT_ERROR         = 999115009,     // 重复初始化
	IVS_OCX_RET_RESET_PANE_INFO_ERROR      = 999115010,     // 复位窗格信息失败
	IVS_OCX_RET_SET_WND_LAYOUT_ERROR       = 999115011,     // 设置窗格布局失败
	//IVS_OCX_RET_NO_WRITE_AUTHORITY_ERROR   = 999115012,     // 没有写权限，创建文件夹或抓拍/录像文件失败
	//IVS_OCX_RET_VIDEOPANEMGR_NULL          = 999115013,     // 窗格管理类为空
	//IVS_OCX_RET_VIDEOPANE_NULL             = 999115014,     // 窗格为空
	//IVS_OCX_RET_START_LOG_ERROR            = 999115015,     // 开启日志失败
	//IVS_OCX_RET_SETOCX_CTRL_ERROR          = 999115016,     // 设置控件指针失败
	//IVS_OCX_RET_INITIAL_VIDEOPANEMGR_ERROR = 999115017,     // 初始化窗格管理类失败
	//IVS_OCX_RET_SETOCXLOG_ERROR            = 999115018,     // 设置日志参数失败
	//IVS_OCX_RET_PLAYSOUND_WITH_VIDEO_ERROR = 999115019,     // 开启声音失败
	//IVS_OCX_RET_STOPSOUND_WITHVIDEO_ERROR  = 999115020,     // 关闭声音失败
	//IVS_OCX_RET_ADJUST_WAVE_AUDIO_ERROR    = 999115021,     // 调整音量失败
	//IVS_OCX_RET_SET_WAVE_AUDIO_ERROR 	   = 999115022,     // 设置音量失败
	//IVS_OCX_RET_SET_PAGENUMBER_ERROR 	   = 999115023,     // 设置页码失败
	//IVS_OCX_RET_CAPTURE_HAS_STARTED_ERROR  = 999115024,     // 抓图已开始错误
	IVS_OCX_RET_CAPTURE_CREATE_THREAD_ERROR = 999115025,    // 抓图创建线程错误
	IVS_OCX_RET_SET_WARN_STATE_ERROR  	   = 999115026,     // 设置警告状态失败
	//IVS_OCX_RET_VOICE_TALKBACK_HAS_PLAYED_ERROR     = 999115027,   //语音对讲已经开启
	//IVS_OCX_RET_VOICE_TALKBACK_IS_NOT_PLAYING_ERROR = 999115028,   //语音对讲未开启
	//IVS_OCX_RET_SET_DISPLAY_SCALE_ERROR       = 999115029,     // 设置窗口显示比例失败
	IVS_OCX_RET_VIDEOPANE_ISNOT_PLAYING_ERROR = 999115030,     // 窗格没有播放视频
	//IVS_OCX_RET_PARSE_XML_FAIL                = 999115031,     // 解析xml失败
	//IVS_OCX_RET_VIDEOPANE_ISNOT_RECORD_ERROR  = 999115032,     // 窗格没有在录像
	//IVS_OCX_RET_IS_ALARM_WINDOW_ERROR       = 999115033,    // 窗格为告警窗格
	//IVS_OCX_RET_VIDEOPANE_ISNOT_SOUND_ERROR = 999115034,    // 窗格没有在打开声音
	IVS_OCX_RET_VIDEOPANE_ISNOT_ALARM_ERROR = 999115035,    // 窗格不是告警窗格
	//IVS_OCX_RET_PLAYBACK_FILENAME_ERROR    = 999115036,    // 回放文件名错误
	//IVS_OCX_RET_PTZ_NO_RIGHT_EXCEPTION     = 999115037,    // 无云台权限异常
	IVS_OCX_RET_GET_INFO_ERROR             = 999115038,    // 获取信息失败
	//IVS_OCX_FILE_CREATE_ERROR              = 999115039,    // OCX本地文件创建失败
	//IVS_OCX_FILE_NOT_EXIST                 = 999115040,    // OCX本地文件不存在
    IVS_OCX_RET_SKIN_INIT_ERROR            = 999115041,    // OCX初始化皮肤失败
    IVS_OCX_RET_FIND_PLAY_HANDLE_ERROR     = 999115042,    // OCX不存在该播放句柄
	//IVS_OCX_RET_REALTIMEBAR_ERROR_SOUND    = 999115043,    // 实时声音错误
    IVS_OCX_RET_NO_PLAY_HANDLE             = 999115044,    // 没有播放句柄
	//IVS_OCX_LOGO_XML_ERROR                 = 999115045,     // logoxml获取失败
	//IVS_OCX_EVENT_NO_CATCH                 = 999115046,     // 未截获事件
	IVS_OCX_RET_POINT_OUT_CLIENT             = 999115047,    // 鼠标点在OCX以外

	// SDK 200～399
	IVS_SDK_RET_INIT_RTSPCLIENT_ERROR        = 999115200,    // 初始化RtspClient库失败;
	IVS_SDK_RET_SET_LOG_LEVEL_ERROR          = 999115201,    // 设置日志级别失败
	//IVS_SDK_RET_MAIN_SERVICE_INIT_FAIL     = 999115301,    // SDK调用BP创建服务失败;
	//IVS_SDK_RET_INIT_THREA_FAIL            = 999115302,    // SDK初始化主线程失败;
	//IVS_SDK_RET_INIT_LOG_FAIL              = 999115303,    // SDK初始化LOG服务失败;
	//IVS_SDK_RET_SMU_SDK_XML_INVALD         = 999115304,    // SMU给SDK的响应XML非法;
	IVS_SDK_RET_BP_CMD_REQ_INVALID         = 999115305,    // SDK调用BP创建命令非法;
	IVS_SDK_RET_BP_CMD_RSP_INVALID         = 999115306,    // SDK调用BP应答命令非法;
	//IVS_SDK_RET_BP_CMD_RSP_XML_ERROR       = 999115307,    // SDK调用BP应答命令XML非法;
	//IVS_SDK_RET_INTRINSIC_PTR_ERROR        = 999115308,    // SDK内部指针错误;
	//IVS_SDK_RET_ALLOC_MEMORY_ERROR         = 999115309,    // SDK内存申请失败;
	//IVS_SDK_RET_MEMORY_CHECK_ERROR         = 999115310,    // SDK内存校验失败;
	//IVS_SDK_RET_INIT_ERROR                 = 999115311,    // SDK初始化失败;
    IVS_SDK_RET_INVALID_SESSION_ID         = 999115312,    // 无效的SessionID;
	IVS_SDK_ERR_FINDVIDEOHANDLE_FAILED     = 999115313,    // 获取视频句柄失败
	IVS_SDK_ERR_ERR_MAX_USERNUM            = 999115314,    // 达到最大用用户
    //IVS_SDK_ERR_BUFFERSIZE_NOT_ENOUGH      = 999115315,    // 表示外部分配内存不足(可能是ocx也可能是第三方)
    //IVS_SDK_ERR_FINDXMLNODE_FAILED         = 999115316,    // 无法找到xml中相应相应节点;
	//IVS_SDK_ERR_WRITEFILE_FAILED           = 999115317,    // 写文件时发生错误;
	//IVS_SDK_ERR_OPENFILE_FAILED            = 999115318,    // 打开zip文件时发生错误;
	IVS_SDK_ERR_CHEACKMD5_FAILED           = 999115319,    // 检测zip文件时发生错误;
	IVS_SDK_ERR_DELETEZIPFILE_FAILED       = 999115320,    // 删除zip时发生错误;
	IVS_SDK_ERR_CREATEZIPFILE_FAILED       = 999115321,    // 创建zip时发生错误;
    IVS_SDK_ERR_DOMAINCODE_NOT_EXIST       = 999115322,    // 域编码不存在
	//IVS_SDK_ERR_GET_LOGO_FAILED            = 999115323,    // 获取logo失败
	//IVS_SDK_ERR_FILE_NOT_EXIST             = 999115324,    // 获取logo失败

    //IVS_SDK_PLAYBACK_ERR_RSP_GETURL        = 999119200,    // 获取URL响应信息异常
    //IVS_SDK_PLAYBACK_ERR_STARTTIME_ZERO    = 999119201,    // 回放开始时间为0异常
    //IVS_SDK_PLAYBACK_ERR_PORT_NOTEXIST_FAIL_FORSTOP = 999119202,   // 停止回放port口不存在
    //IVS_SDK_PLAYBACK_ERR_PLAYHANDLE        = 999119203,    // 回放handle不合法
    //IVS_SDK_PLAYBACK_ERR_PLAT_SPEED_PARM   = 999119204,    // 平台回放设置速度参数不合法
    //IVS_SDK_PLAYBACK_ERR_PU_SPEED_PARM     = 999119205,    // 前端回放设置速度参数不合法
    //IVS_SDK_PLAYBACK_ERR_GET_REMOTE_STATUS = 999119206,    // 获取远端播放状态失败
    //IVS_SDK_PLAYBACK_ERR_GET_REMOTE_SPEED  = 999119207,    // 获取远端播放速度失败
    //IVS_SDK_PLAYBACK_ERR_GET_REMOTE_TIME   = 999119208,    // 获取远端播放时间失败
    //IVS_SDK_PLAYBACK_ERR_STATUS_DOUDLE_STOP   = 999119209,     // 重复设置暂停状态
    //IVS_SDK_PLAYBACK_ERR_STATUS_DOUDLE_RESUME = 999119210,     // 重复设置恢复状态
    IVS_SDK_PLAYBACK_ERR_GET_REMOTE_DATAINFO  = 999119211,     // 获取远端播放信息失败
    //IVS_SDK_PLAYBACK_ERR_TIMESPAN             = 999119212,     // 回放timespan不合法
    //IVS_SDK_PLAYBACK_ERR_GET_LOCAL_DATAINFO   = 999119213,     // 获取本地播放信息失败
    //IVS_SDK_PLAYBACK_ERR_LOCAL_RECORDING      = 999119214,     // 此窗格正在进行录制，开始本地录制失败
    //IVS_SDK_PLAYBACK_ERR_NOT_LOCAL_RECORDING  = 999119215,     // 没有窗格正在进行录制，停止本地录制失败
	//IVS_SDK_RET_SMU_SERVER_NOT_RESPOND     = 999119216,     // SMU消息返回超时
	//IVS_SDK_RET_SCU_SERVER_NOT_RESPOND     = 999119217,     // SCU消息返回超时
	//IVS_SDK_RET_OMU_SERVER_NOT_RESPOND     = 999119218,     // OMU消息返回超时
	IVS_SDK_SET_CLUSTER_MSG_FAILED           = 999119218,     // 下发集群失败

    /*对讲错误码*/
    IVS_SDK_ERR_FINDAUDIOHANDLE_FAILED     = 999119219,       // 获取对讲句柄失败
    IVS_SDK_TALKBACK_ERR_ALREADY_OPENED    = 999119220,       // 已经开启一路对讲或广播
//     IVS_SDK_TALKBACK_ERR_ALREADY_CLOSED    = 999119221,       // 尚未开启
    IVS_SDK_BROADCASE_ERR_NO_DEVICE        = 999119222,       // 设备组中没有设备
    IVS_SDK_BROADCASE_ERR_ALREADY_OPENED   = 999119223,       // 广播已经开启
    IVS_SDK_BROADCASE_ERR_NOT_OPENED       = 999119224,       // 广播没有开启
    IVS_SDK_VOICE_ERR_INVALID_USEROBJ      = 999119225,       // 用户对象为空，无法获取用户信息
    IVS_SDK_VOICE_ERR_INVALID_USERINFO     = 999119226,       // 用户信息为空
    IVS_SDK_VOICE_ERR_GET_URL_FAIL         = 999119227,       // 获取URL失败
    IVS_SDK_VOICE_ERR_SET_IP_FAIL          = 999119228,       // 设置接收IP失败
    IVS_SDK_VOICE_ERR_CALL_MLIB_FAIL       = 999119229,       // 调用Mlib层接口返回失败
    IVS_SDK_VOICE_ERR_DEV_ALREADY_IN       = 999119230,       // 设备已加入设备组
    IVS_SDK_VOICE_ERR_DEV_NOT_IN           = 999119231,       // 设备组中没有该设备
    IVS_SDK_VOICE_ERR_STOP_BROADCAST_FAIL  = 999119232,       // 删除设备时停止对应设备的广播失败
    IVS_SDK_VOICE_ERR_TOO_MUCH_DEVICES     = 999119233,       // 广播组中的设备数超过最大值256
    IVS_SDK_VOICE_ERR_GET_HANDLE_FAIL      = 999119234,       // 根据设备ID查找广播handle失败
    IVS_SDK_VOICE_ERR_FILE_NOT_EXIST       = 999119235,       // 文件不存在
    IVS_SDK_VOICE_ERR_FILE_NOT_RIGHT_TYPE  = 999119236,       // 不是wav格式的音频文件
    IVS_SDK_VOICE_ERR_CANNOT_REPORT_FAIL   = 999119237,       // 获取用户对象的回调函数失败
    IVS_SDK_VOICE_ERR_ALL_BROADCAST_FAIL   = 999119238,       // 广播开启\关闭全部失败
    IVS_SDK_VOICE_ERR_AUDIO_TYPE_CONFLICT  = 999119239,       // 音频格式和之前的镜头冲突
	IVS_SDK_EXCHANGE_KEY_ERR_GET_KEY_FAIL  = 999119240,       // 密钥协商交换失败、获取密钥失败
	IVS_SDK_LOCALRECORD_UNSUPPORTED_MJPEG = 999119241,       // 本地录像不支持MJPEG格式
	IVS_SDK_LOCALRECORD_UNSUPPORTED_VIDEODECTYPE  = 999119242,       // 本地录像不支持该视频编码格式
	IVS_SDK_NOT_CLUSTER_MODE               = 999119243,       // 服务器异常或模式错误
    IVS_SDK_NOT_PLAY_TVWALL                = 999119244,       // 没有电视上墙(在关闭电视上墙的时候返回)

	// MP 400～599
    IVS_PLAYER_RET_GET_CHANNEL_ERROR       = 999115400,     // 获取通道失败;
    IVS_PLAYER_RET_ALL_PORT_BUSY_ERROR     = 999115401,     // 所有端口均被占用;
    IVS_PLAYER_RET_WSA_START_ERROR         = 999115402,     // 启用SOCKET的DLL库错误;
    IVS_PLAYER_RET_CONNECT_SOCKET_ERROR    = 999115403,     // 建立socket连接失败;
	IVS_PLAYER_RET_ALLOC_MEM_ERROR         = 999115404,     // 缓存分配失败;
	IVS_PLAYER_RET_RECV_DATA_ERROR         = 999115405,     // 数据流错误;
    IVS_PLAYER_RET_RECV_DATA_TIMEOUT       = 999115406,     // 连接超时;
 	IVS_PLAYER_RET_CREATE_THREAD_ERROR     = 999115407,     // 创建线程失败;
	IVS_PLAYER_RET_CREATE_DECODER_ERROR    = 999115408,     // 创建解码库失败;
	IVS_PLAYER_RET_ORDER_ERROR             = 999115409,     // 调用顺序不对;
	IVS_PLAYER_RET_DECODER_ERROR           = 999115410,     // 解码失败;
	IVS_PLAYER_RET_WATER_MARK_OK           = 999115411,     // 水印数据正常;
	IVS_PLAYER_RET_WATER_MARK_ERROR        = 999115412,     // 水印数据被篡改;
	IVS_PLAYER_RET_UNSUPPORT_OPERATION     = 999115413,     // 当前版本不支持此操作;
	IVS_PLAYER_RET_DECODER_BUFFER_FULL     = 999115414,     // 缓冲区满;

    IVS_PLAYER_RET_RTSP_GET_HANDLE_FAIL    = 999115415,     // 获取rtsp的handle失败;
    IVS_PLAYER_RET_RTSP_SEND_SETUP_FAIL    = 999115416,     // 发送rtsp的Setup请求失败;
    IVS_PLAYER_RET_RTSP_SEND_PLAY_FAIL     = 999115417,     // 发送rtsp的Play请求失败;
    IVS_PLAYER_RET_RTSP_SEND_PAUSE_FAIL    = 999115418,     // 发送rtsp的Pause请求失败;
    IVS_PLAYER_RET_RTSP_SEND_TEARDOWN_FAIL = 999115419,     // 发送rtsp的Teardown请求失败;
    IVS_PLAYER_RET_RTSP_NORMAL_STOP        = 999115420,     // RTSP连接收到正常结束
    IVS_PLAYER_RET_RTSP_ABNORMAL_STOP      = 999115421,     // RTSP连接收到异常结束;
    IVS_PLAYER_RET_RTSP_DATA_ERROR         = 999115422,     // RTSP码流不符合要求;
	IVS_PLAYER_RET_RTSP_DISCONNECT         = 999115423,     // RTSP码流断开;
    IVS_PLAYER_RET_NET_OPEN_STREAM_FAIL    = 999115424,     // 网络库打开网络流失败;
    IVS_PLAYER_RET_MP_OPEN_STREAM_FAIL     = 999115425,     // 播放库打开网络流失败;
    IVS_PLAYER_RET_MP_START_PLAY_FAIL      = 999115426,     // 播放库启动播放失败;
    IVS_PLAYER_RET_NET_START_RECEIVE_FAIL  = 999115427,     // 网络库启动收流失败;
    IVS_PLAYER_RET_INIT_NETSOURCE_ERROR    = 999115428,     // 初始化网络收流库失败;
    IVS_PLAYER_RET_INIT_PLAY_ERROR         = 999115429,     // 初始化播放收流库失败;
	IVS_PLAYER_RET_FILE_ACCESS_ERROR       = 999115430,     // 访问文件或目录错误;
	IVS_PLAYER_RET_DISK_SPACE_ERROR        = 999115431,     // 磁盘空间满错误;
    IVS_PLAYER_RET_MIXER_OPEN_STATE_ERROR  = 999115432,     // 混音器打开状态错误
    IVS_PLAYER_RET_MIXER_ERROR             = 999115433,     // 混音器操作错误
    IVS_PLAYER_RET_VOICE_TALKBACK_STATUS_ERROR = 999115434, // 语音对讲状态失败
    IVS_PLAYER_RET_BROADCAST_FILE_IS_END   = 999115435,     // 语音广播文件播放结束
    IVS_PLAYER_RET_BROADCAST_NOT_SUPPORTED_FORMAT = 999115436, // 语音广播传入的文件格式不支持
    IVS_PLAYER_RET_BROADCAST_OPEN_FILE_ERROR = 999115437,   // 语音广播读文件失败
    IVS_PLAYER_RET_GET_STREAM_OPEN_MODE_ERROR = 999115438,  // 获取数据流打开模式失败
    IVS_PLAYER_RET_GET_AUDIO_VOLUMN_ERROR  = 999115439,     // 获取音量失败
    IVS_PLAYER_RET_WAVE_OUT_ERROR          = 999115440,     // 打开音频输出失败
    IVS_PLAYER_RET_CAPTURE_OPEN_STATE_ERROR = 999115441,    // 音频收集状态错误
    IVS_PLAYER_RET_WAVE_IN_ERROR           =  999115442,    // 打开音频输入失败
    IVS_PLAYER_RET_AUDIO_DEVICE_NOT_FIND   =  999115443,    // 未找到音频设备
    IVS_PLAYER_RET_MEDIA_TYPE_UNSUPPORT    =  999115444,    // 不支持该媒体类型
    IVS_PLAYER_RET_VOICE_BROADCAST_STATUS_ERROR = 999115445,// 语音广播状态失败
    IVS_PLAYER_RET_CHANNEL_NOT_EXIST_ERROR = 999115446,     // 通道不存在

	IVS_PLAYER_RET_RTSP_ERR_NOT_EXIST      = 999115447,     // 连接句柄不存在
	IVS_PLAYER_RET_RTSP_ERR_DISCONNECT     = 999115448,     // 连接失败
	IVS_PLAYER_RET_RTSP_ERR_RESPONSE       = 999115449,     // 响应消息错误
	IVS_PLAYER_RET_RTSP_ERR_TIMEOUT        = 999115450,     // 响应超时
	IVS_PLAYER_RET_RTSP_ERR_STAUTS_ABNORMAL= 999115451,     // 连接状态异常
	IVS_PLAYER_RET_RTSP_ERR_SEND_FAIL      = 999115452,     // 发送出错
	IVS_PLAYER_RET_RTSP_ERR_RECV_FAIL      = 999115453,     // 接收出错
	IVS_PLAYER_RET_RTSP_ERR_PARAMETER      = 999115454,     // 参数检查出错
	IVS_PLAYER_RET_RTSP_ERR_NORMAL_STOP    = 999115455,     // 正常结束，ANNOUNCE正常结束请求
	IVS_PLAYER_RET_RTSP_ERR_ABNORMAL_STOP  = 999115456,     // 异常结束，ANNOUNCE异常结束请求
	IVS_PLAYER_RET_RTSP_ERR_RET_SYS        = 999115457,      // 通用系统错误
    IVS_PLAYER_RET_NET_BIND_PORT_FAIL	   = 999115458,      // 绑定端口失败;
	IVS_PLAYER_RET_RTSP_ERR_SERVICE_500_RSP= 999115459,		 // 服务器500响应;
	IVS_PLAYER_RET_RTSP_ERR_SERVICE_UNAVAILABLE_RSP=999115460,// 服务不可用;
	IVS_PLAYER_RET_DISK_WRITE_ERROR=999115461,// 录像文件写入失败，请检查文件夹权限设置;
	IVS_PLAYER_RET_ERR_FILE_NOT_EXIST       = 999115462,       // 录像文件不存在
    IVS_PLAYER_RET_TALKBACK_OPENED_ERROR = 999115463,     // 语音对讲已经开启
	IVS_PLAYER_RET_NOTENOUGH_BANDWIDTH   = 999115464,     // 媒体服务器带宽不足

} IVS_ERROR_E;


#endif //_IVS_ERROR_H_
