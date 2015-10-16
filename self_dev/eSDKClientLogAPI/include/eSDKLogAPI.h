#ifndef eSDK_LOG_API_H
#define eSDK_LOG_API_H
// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 LOGMANAGER_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// LOGMANAGER_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。

#ifdef WIN32
	#ifdef eSDK_LOGAPI_EXPORTS
		#define eSDK_LOG_API __declspec(dllexport)
	#else
		#define eSDK_LOG_API __declspec(dllimport)
	#endif
#else
	#define eSDK_LOG_API __attribute__((visibility("default")))
#endif

#ifdef WIN32
	#define _STD_CALL_  __stdcall
#else
	#define _STD_CALL_
#endif


#include "eSDKLogDataType.h"


#ifdef __cplusplus
extern "C"
{
#endif

	
	/**
	 *初始化
	 * 
	 *该函数用于初始化数据
	 *
	 *@param[in] 	product		使用日志模块的产品名字，同进程中的唯一标示
	 *@param[in] 	iniFile		日志配置文件路径（包括配置文件名，如：D:\eSDKClientLogCfg.ini）
	 *@param[in] 	logLevel	logLevel[0]接口日志级别, logLevel[1]操作日志级别, logLevel[2]运行日志级别，参考枚举LOGLEVEL
	 *@param[in] 	logPath		日志保存路径（如：D:\log\），必须是绝对路径，如要使用默认配置请用INVALID_LOG_LEVEL
	 *@return		0	成功
	 *@return		非0	失败（请参考错误返回码枚举类型RetCode）
	 *@attention	使用本API前先初始化
	 *@par			无
	**/
	eSDK_LOG_API int _STD_CALL_ LogInit(const char* product, const char* iniFile, unsigned int logLevel[LOG_CATEGORY], const char* logPath);


	/**
	 *去初始化
	 * 
	 *该函数用于进程不使用本API后，去初始化数据
	 *
	 *@param[in] 	product		使用日志模块的产品名字，同进程中的唯一标示
	 *@return		0	成功
	 *@return		非0	失败（请参考错误返回码枚举类型RetCode）
	 *@attention	结束进程前先调用本接口
	 *@par			无
	**/
	eSDK_LOG_API int _STD_CALL_ LogFini(const char* product);


	/**
	 *接口类日志info接口
	 * 
	 *该函数用于写interface下的info日志
	 *
	 *@param[in]	product			填写接口所属的产品，如UC的接口填写UC。包括UC、IVS、TP、FusionSphere、 Storage等
	 *@param[in]	interfaceType	接口类型，值为1和2：其中1标识为北向接口；2标识为南向接口
	 *@param[in]	protocolType	接口类型，值为SOAP（细分ParlayX）、Rest、COM、Native、HTTP+XML，SMPP
	 *@param[in]	interfaceName	接口名称
	 *@param[in]	sourceAddr		源端设备，客户端API类为空
	 *@param[in]	targetAddr		宿端设备，客户端API类为空
	 *@param[in]	TransactionID	唯一标识接口消息所属事务，不存在时为空
	 *@param[in]	reqTime			请求时间
	 *@param[in]	RespTime		应答时间
	 *@param[in]	resultCode		接口返回结果码
	 *@param[in]	params			请求和响应参数，格式为“paramname=value”等,关键字需要用*替换
	 *@param[in]	...				直接传入参数的变量名称
	 *@attention	调用前确保已经调用LogInit
	 *@par			无
	**/
	eSDK_LOG_API void _STD_CALL_ Log_Interface_Info(
		const char* product,
		const char* interfaceType,
		const char* protocolType,
		const char* interfaceName,
		const char* sourceAddr,
		const char* targetAddr,
		const char* TransactionID,
		const char* reqTime,
		const char* RespTime,
		const char* resultCode,
		const char* params,
		...
		);
	/**
	 *接口类日志error接口
	 * 
	 *该函数用于写interface下的error日志
	 *
	 *@param[in]	product			填写接口所属的产品，如UC的接口填写UC。包括UC、IVS、TP、FusionSphere、 Storage等
	 *@param[in]	interfaceType	接口类型，值为1和2：其中1标识为北向接口；2标识为南向接口
	 *@param[in]	protocolType	接口类型，值为SOAP（细分ParlayX）、Rest、COM、Native、HTTP+XML，SMPP
	 *@param[in]	interfaceName	接口名称
	 *@param[in]	sourceAddr		源端设备，客户端API类为空
	 *@param[in]	targetAddr		宿端设备，客户端API类为空
	 *@param[in]	TransactionID	唯一标识接口消息所属事务，不存在时为空
	 *@param[in]	reqTime			请求时间
	 *@param[in]	RespTime		应答时间
	 *@param[in]	resultCode		接口返回结果码
	 *@param[in]	params			请求和响应参数，格式为“paramname=%d”等,关键字需要用*替换
	 *@param[in]	...				直接传入参数的变量名称
	 *@attention	调用前确保已经调用LogInit
	 *@par			无
	 *@par			无
	**/
	eSDK_LOG_API void _STD_CALL_ Log_Interface_Error(
		const char* product,
		const char* interfaceType,
		const char* protocolType,
		const char* interfaceName,
		const char* sourceAddr,
		const char* targetAddr,
		const char* TransactionID,
		const char* reqTime,
		const char* RespTime,
		const char* resultCode,
		const char* params,
		...
		);

	/**
	 *操作类接口Debug接口
	 * 
	 *该函数用于写operate下的Debug日志
	 *
	 *@param[in] 	product		使用日志模块的产品名字，同进程中的唯一标示
	 *@param[in]	moduleName		内部模块名称，暂时分为：login、config、log、version。
	 *@param[in]	userName			操作用户名。
	 *@param[in]	clientFlag			操作客户端标识，一般为客户端IP。
	 *@param[in]	resultCode			操作结果码。
	 *@param[in]	keyInfo		关键描述信息：
	 *									查询类操作，需要包括查询对象标识、名称、相关属性名称和属性值。
	 *									设置类操作，需要包括设置对象标识、名称、相关属性名称和属性新值和旧值。
	 *									创建类操作，需要包括创建涉及对象标识、名称。
	 *									删除类操作，需要包括删除涉及对象标识、名称。
	 *@attention	调用前确保已经调用LogInit
	 *@par			无
	**/
	eSDK_LOG_API void _STD_CALL_ Log_Operate_Debug(	
		const char* product,
		const char* moduleName,
		const char* userName,
		const char* clientFlag,
		const char* resultCode,
		const char* keyInfo,
		const char* params,
		...
		);

	/**
	 *操作类接口Info接口
	 * 
	 *该函数用于写operate下的Info日志
	 *
	 *@param[in] 	product		使用日志模块的产品名字，同进程中的唯一标示
	 *@param[in]	moduleName		内部模块名称，暂时分为：login、config、log、version。
	 *@param[in]	userName			操作用户名。
	 *@param[in]	clientFlag			操作客户端标识，一般为客户端IP。
	 *@param[in]	resultCode			操作结果码。
	 *@param[in]	keyInfo		关键描述信息：
	 *									查询类操作，需要包括查询对象标识、名称、相关属性名称和属性值。
	 *									设置类操作，需要包括设置对象标识、名称、相关属性名称和属性新值和旧值。
	 *									创建类操作，需要包括创建涉及对象标识、名称。
	 *									删除类操作，需要包括删除涉及对象标识、名称。
	 *@attention	调用前先调用LogInit
	 *@par			无
	**/
	eSDK_LOG_API void _STD_CALL_ Log_Operate_Info(
		const char* product,
		const char* moduleName,
		const char* userName,
		const char* clientFlag,
		const char* resultCode,
		const char* keyInfo,
		const char* params,
		...
		);//操作类日志接口

	/**
	 *操作类接口Warn接口
	 * 
	 *该函数用于写operate下的Warn日志
	 *
	 *@param[in] 	product		使用日志模块的产品名字，同进程中的唯一标示
	 *@param[in]	moduleName		内部模块名称，暂时分为：login、config、log、version。
	 *@param[in]	userName			操作用户名。
	 *@param[in]	clientFlag			操作客户端标识，一般为客户端IP。
	 *@param[in]	resultCode			操作结果码。
	 *@param[in]	keyInfo		关键描述信息：
	 *									查询类操作，需要包括查询对象标识、名称、相关属性名称和属性值。
	 *									设置类操作，需要包括设置对象标识、名称、相关属性名称和属性新值和旧值。
	 *									创建类操作，需要包括创建涉及对象标识、名称。
	 *									删除类操作，需要包括删除涉及对象标识、名称。
	 *@attention	调用前确保已经调用LogInit
	 *@par			无
	**/
	eSDK_LOG_API void _STD_CALL_ Log_Operate_Warn(
		const char* product,
		const char* moduleName,
		const char* userName,
		const char* clientFlag,
		const char* resultCode,
		const char* keyInfo,
		const char* params,
		...
		);//操作类日志接口
	/**
	 *操作类接口Error接口
	 * 
	 *该函数用于写operate下的Error日志
	 *
	 *@param[in] 	product		使用日志模块的产品名字，同进程中的唯一标示
	 *@param[in]	moduleName		内部模块名称，暂时分为：login、config、log、version。
	 *@param[in]	userName			操作用户名。
	 *@param[in]	clientFlag			操作客户端标识，一般为客户端IP。
	 *@param[in]	resultCode			操作结果码。
	 *@param[in]	keyInfo		关键描述信息：
	 *									查询类操作，需要包括查询对象标识、名称、相关属性名称和属性值。
	 *									设置类操作，需要包括设置对象标识、名称、相关属性名称和属性新值和旧值。
	 *									创建类操作，需要包括创建涉及对象标识、名称。
	 *									删除类操作，需要包括删除涉及对象标识、名称。
	 *@attention	调用前先调用LogInit
	 *@par			无
	**/
	eSDK_LOG_API void _STD_CALL_ Log_Operate_Error(
		const char* product,
		const char* moduleName,
		const char* userName,
		const char* clientFlag,
		const char* resultCode,
		const char* keyInfo,
		const char* params,
		...
		);//操作类日志接口

	/**
	 *运行类Debug接口
	 * 
	 *该函数用于写run下的Debug日志
	 *
	 *@param[in] 	product		使用日志模块的产品名字，同进程中的唯一标示
	 *@param[in]	param 
	 *@attention	调用前先调用LogInit
	 *@par			无
	**/
	eSDK_LOG_API void _STD_CALL_ Log_Run_Debug(const char* product, const char* param);//运行类日志接口
		/**
	 *运行类Info接口
	 * 
	 *该函数用于写run下的Info日志
	 *
	 *@param[in] 	product		使用日志模块的产品名字，同进程中的唯一标示
	 *@param[in]	param 
	 *@attention	调用前先调用LogInit
	 *@par			无
	**/
	eSDK_LOG_API void _STD_CALL_ Log_Run_Info(const char* product, const char* param);//运行类日志接口
		/**
	 *运行类Warn接口
	 * 
	 *该函数用于写run下的Warn日志
	 *
	 *@param[in] 	product		使用日志模块的产品名字，同进程中的唯一标示
	 *@param[in]	param 
	 *@attention	调用前先调用init
	 *@par			无
	**/
	eSDK_LOG_API void _STD_CALL_ Log_Run_Warn(const char* product, const char* param);//运行类日志接口
		/**
	 *运行类Error接口
	 * 
	 *该函数用于写run下的Error日志
	 *
	 *@param[in] 	product		使用日志模块的产品名字，同进程中的唯一标示
	 *@param[in]	param 
	 *@attention	调用前先调用LogInit
	 *@par			无
	**/
	eSDK_LOG_API void _STD_CALL_ Log_Run_Error(const char* product, const char* param);//运行类日志接口


#ifdef __cplusplus
}
#endif 

#endif

