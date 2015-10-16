//===================================================================
//	Copyright(C)2013-2020, Huawei Tech. Co., Ltd.
//
//	文件名	：IvsSdkClient.h
//	作  者	：lkf70422
//	版  本	：V100R002C03
//	完成日期：2013-04-7
//	说  明	：IVS SDK 客户端接口，实现口C++ 和C#之间转换
//	其  它	：
//	历史记录：
//===================================================================
#ifndef __IVSSDKCLENT_H__
#define __IVSSDKCLENT_H__
#pragma  once

#include "stdafx.h"
#include "IVS_SDK.h"

using namespace CgwMonitorManage::Common;
using namespace System;
using namespace System::Runtime::InteropServices;
using namespace System::Collections::Generic;

namespace CgwMonitorManage
{
	namespace Ivs
	{		
		public ref class IvsSdkClient
		{
		public:
			//===================================================================
			//	函数名称：IvsSdkClient
			//	功能描述：构造函数
			//	调用清单：NA
			//	被调清单：IvsVideoMonitor.cs
			//	输入参数：NA
			//	输出参数：NA
			//	  返回值：NA
			//	其他说明：
			//===================================================================
			IvsSdkClient();


			//===================================================================
			//	函数名称：SetId
			//	功能描述：设置本地ID，用于实况预览的时候，标识平台id
			//	调用清单：NA
			//	被调清单：IvsVideoMonitor.cs
			//	输入参数：id	平台id
			//	输出参数：NA
			//	  返回值：NA
			//	其他说明：
			//===================================================================
			void SetId(String^ id);

			//===================================================================
			//	函数名称：SetLogPath
			//	功能描述：初始化SDK
			//	调用清单：IVS_SDK
			//	被调清单：IvsVideoMonitor.cs
			//	输入参数：psLogPath 日志路径
			//	输出参数：NA
			//	  返回值：成功返回0，失败返回错误码
			//	其他说明：
			//===================================================================
			int SetLogPath(String^ psLogPath);

			//===================================================================
			//	函数名称：Init
			//	功能描述：初始化SDK
			//	调用清单：IVS_SDK
			//	被调清单：IvsVideoMonitor.cs
			//	输入参数：NA
			//	输出参数：NA
			//	  返回值：成功返回0，失败返回错误码
			//	其他说明：
			//===================================================================
			int Init();

			//===================================================================
			//	函数名称：Cleanup
			//	功能描述：释放SDK资源
			//	调用清单：IVS_SDK
			//	被调清单：IvsVideoMonitor.cs
			//	输入参数：NA
			//	输出参数：NA
			//	  返回值：成功返回0，失败返回错误码
			//	其他说明：
			//===================================================================
			int Cleanup();

			//===================================================================
			//	函数名称：Login
			//	功能描述：登陆服务器
			//	调用清单：IVS_SDK
			//	被调清单：IvsVideoMonitor.cs
			//	输入参数：ip		ip地址
			//			  port		端口
			//			  user		用户
			//			  password	密码
			//	输出参数：NA
			//	  返回值：成功返回0，失败返回错误码
			//	其他说明：
			//===================================================================
			int Login(String^ ip, int port, String^ user, String^ password);

			//===================================================================
			//	函数名称：ReLogin
			//	功能描述：根据上次调用Login接口使用的登录信息，重新登录服务器
			//	调用清单：IVS_SDK
			//	被调清单：IvsVideoMonitor.cs
			//	输入参数：NA
			//	输出参数：NA
			//	  返回值：成功返回0，失败返回错误码
			//	其他说明：
			//===================================================================
			int ReLogin();

			//===================================================================
			//	函数名称：Logout
			//	功能描述：注销登陆
			//	调用清单：IVS_SDK
			//	被调清单：IvsVideoMonitor.cs
			//	输入参数：NA
			//	输出参数：NA
			//	  返回值：成功返回0，失败返回错误码
			//	其他说明：
			//===================================================================
			int Logout();

			//===================================================================
			//	函数名称：GetDeviceList
			//	功能描述：根据索引获取设备列表，获取fromIndex和toIndex之间的记录
			//	调用清单：IVS_SDK
			//	被调清单：IvsVideoMonitor.cs
			//	输入参数：fromIndex			起始索引
			//			  toIndex			结束索引
			//	输出参数：totalCameraCount  记录总数
			//			  cameraList		摄像头列表
			//	  返回值：成功返回0，失败返回错误码
			//	其他说明：
			//===================================================================
			int GetDeviceList(int fromIndex,int toIndex,[Out]int% totalCameraCount,[Out]List<IvsCamera^>^% cameraList);

			//===================================================================
			//	函数名称：GetDomainRoute
			//	功能描述：获取域列表
			//	调用清单：IVS_SDK
			//	被调清单：IvsVideoMonitor.cs
			//	输入参数：NA
			//	输出参数：domainRouteList	域列表
			//	  返回值：成功返回0，失败返回错误码
			//	其他说明：
			//===================================================================
			int GetDomainRoute([Out]List<IvsDomainRoute^>^% domainRouteList);

			//===================================================================
			//	函数名称：GetDeviceGroupList
			//	功能描述：获取指定域，指定节点下的所有子设备组列表信息
			//	调用清单：IVS_SDK
			//	被调清单：IvsVideoMonitor.cs
			//	输入参数：domainCode			域编码
			//			  rootGroupNo			父节点（域的根节点为0，如果查询域下的所有分组，输入0即可） 
			//	输出参数：cameraGroupDic		分组列表，Dictionary类型，key为组编号，value为分组信息对象
			//	  返回值：成功返回0，失败返回错误码
			//	其他说明：
			//===================================================================
			int GetDeviceGroupList(String^ domainCode,String^ rootGroupNo,[Out]Dictionary<String^,IvsCameraGroup^>^% cameraGroupDic);

			//===================================================================
			//	函数名称：SetRealPlayCBRawCallBackFunc
			//	功能描述：设置码流回调函数
			//	调用清单：IVS_SDK
			//	被调清单：IvsVideoMonitor.cs
			//	输入参数：callbackFunc			码流回调函数IvsRealPlayCallBackRawFunc		  
			//	输出参数：NA
			//	  返回值：成功返回0，失败返回错误码
			//	其他说明：
			//===================================================================
			void SetRealPlayCBRawCallBackFunc(IvsRealPlayCallBackRawFunc^ callbackFunc);

			//===================================================================
			//	函数名称：SetEventCallBackFunc
			//	功能描述：设置码流回调函数
			//	调用清单：IVS_SDK
			//	被调清单：IvsVideoMonitor.cs
			//	输入参数：callbackFun 
			//	输出参数：NA
			//	  返回值：成功返回0，失败返回错误码
			//	其他说明：
			//===================================================================
			void SetEventCallBackFunc(IvsEventCallBackFunc^ callbackFunc);

			//===================================================================
			//	函数名称：StartRealPlayCBRaw
			//	功能描述：启动实况预览，开始接收码流
			//	调用清单：IVS_SDK
			//	被调清单：IvsVideoMonitor.cs
			//	输入参数：cameraNo			摄像头编号		  
			//	输出参数：handle			播放句柄
			//	  返回值：成功返回0，失败返回错误码
			//	其他说明：
			//===================================================================
			int StartRealPlayCBRaw(String^ cameraNo,[Out]long%  handle);

			//===================================================================
			//	函数名称：StopRealPlay
			//	功能描述：停止实况，停止接收码流
			//	调用清单：IVS_SDK
			//	被调清单：IvsVideoMonitor.cs
			//	输入参数：handle			播放句柄		  
			//	输出参数：NA
			//	  返回值：成功返回0，失败返回错误码
			//	其他说明：
			//===================================================================
			int StopRealPlay(long handle);

			//===================================================================
			//	函数名称：StartPtzControl
			//	功能描述：开始ptz控制
			//	调用清单：IVS_SDK
			//	被调清单：IvsVideoMonitor.cs
			//	输入参数：cameraNo			摄像头编号
			//			  controlCode		控制命令
			//			  param				命令参数（速度、倍数）
			//	输出参数：NA
			//	  返回值：成功返回0，失败返回错误码
			//	其他说明：
			//===================================================================
			int StartPtzControl(String^ cameraNo,int controlCode,String^ param);

			//===================================================================
			//	函数名称：StopPtzControl
			//	功能描述：停止ptz控制
			//	调用清单：IVS_SDK
			//	被调清单：IvsVideoMonitor.cs
			//	输入参数：cameraNo			摄像头编号
			//	输出参数：NA
			//	  返回值：成功返回0，失败返回错误码
			//	其他说明：
			//===================================================================
			int StopPtzControl(String^ cameraNo);

			//码流回调函数，IVS SDK接到码流后，调用这个回调函数，将码流发给融合网关
			static IvsRealPlayCallBackRawFunc^ cgwCallbackFunc;

			//事件回调函数(事件通知，异常回调等)
			static IvsEventCallBackFunc^ cgwEventCallBackFunc;

		private:			

			//登陆成功后session id
			IVS_INT32 sessionId;	
			String^ ip;
			int port;
			String^ user;
			String^ password;

		};

	}
}
#endif