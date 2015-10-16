//===================================================================
//	Copyright(C)2013-2020, Huawei Tech. Co., Ltd.
//
//	文件名	：IvsSdkClient.h
//	作  者	：lkf70422
//	版  本	：V100R002C03
//	完成日期：2013-04-7
//	说  明	：IVS SDK 客户端接口实现，调用SDK接口，实现口C++ 和C#之间转换
//	其  它	：
//	历史记录：
//===================================================================
#include "hwsdk.h"
#include "IvsSdkClient.h"
#include "ivs_error.h"
#include <string>
#include "securec.h"

using namespace std;

namespace CgwMonitorManage
{

	namespace Ivs
	{
		//===================================================================
		//	函数名称：	
		//	功能描述：	实现string编码和TCHAR编码互转
		//	调用清单：
		//	被调清单：
		//	输入参数：	str	--	[in] TCHAR
		//	输出参数：  
		//	  返回值：  wstring  -- [out]  TCHAR类型字符串
		//	其他说明：
		//===================================================================
		string TCHAR2STRING(TCHAR* szStr)
		{
			int iLen = WideCharToMultiByte(CP_ACP, 
				0,
				szStr, 
				-1,
				NULL, 
				0, 
				NULL, 
				NULL); 
			char* chRtn =new char[iLen*sizeof(char)]; 
			WideCharToMultiByte(CP_ACP, 
				0, 
				szStr,
				-1,
				chRtn, 
				iLen, 
				NULL, 
				NULL); 

			std::string str(chRtn);
			delete[] chRtn;
			chRtn =NULL;
			return str;
		}

		//===================================================================
		//	函数名称：	
		//	功能描述：	实现TCHAR编码和char编码互转
		//	调用清单：
		//	被调清单：
		//	输入参数：	const char *	--	[in] utf_8
		//	输出参数：  
		//	  返回值：  TCHAR  -- [out]  TCHAR类型字符串
		//	其他说明：
		//===================================================================
		TCHAR* CHAR2WCHAR(const char *buffer)
		{
			size_t len = strlen(buffer);
			size_t wlen = MultiByteToWideChar(CP_ACP, 
				0, 
				(const char*)buffer, 
				int(len), 
				NULL, 
				0);
			TCHAR* wBuf = new TCHAR[wlen + 1];

			memset_s(wBuf,wlen+1,0,(wlen+1)*sizeof(wchar_t));
			MultiByteToWideChar(CP_ACP, 
				0, 
				(const char*)buffer, 
				int(len), 
				wBuf, 
				int(wlen));
			return wBuf;
		} 

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
		IvsSdkClient::IvsSdkClient()
		{
			sessionId = -1;
		}

		//===================================================================
		//	函数名称：	
		//	功能描述：	将managed string转换成native ansi string
		//	调用清单：
		//	被调清单：
		//	输入参数：	nSrcString	--	[in]	 Managed String
		//	输出参数：  mDestString  -- [out]   Native char类型字符串
		//	  返回值：
		//	其他说明：不检查目标字符串缓冲区会越界
		//===================================================================
		bool operator << (OUT IVS_CHAR* nDestString, IN String^ mSrcString)
		{
			if (NULL == nDestString || nullptr == mSrcString)
			{
				return false;
			}
			IntPtr ip = Marshal::StringToHGlobalAnsi(mSrcString);
			IVS_CHAR* tempBuf = static_cast<IVS_CHAR *>(ip.ToPointer());

			//便于使用方便，nDestString长度由调用者保证，这里传的长度保证能完整拷贝源字符串；
			strcpy_s(nDestString,strlen(tempBuf)+1,tempBuf);

			Marshal::FreeHGlobal(ip);
			return true;
		}

		//全局变量，监控平台ID，默认值cgw
		//CString* localId=new CString("cgw");
		IVS_CHAR*  localId = new IVS_CHAR[IVS_DEV_CODE_LEN];
		void IvsSdkClient::SetId(String^ id)
		{
			memset_s(localId,sizeof(localId),0,IVS_DEV_CODE_LEN);
			localId<<id;
		}

		//===================================================================
		//	函数名称：	
		//	功能描述：	将native ansi string转换成managed string
		//	调用清单：
		//	被调清单：
		//	输入参数：	nSrcString	--	[in]	Native Char类型字符串
		//	输出参数：  mDestString  -- [out]   Managed String
		//	  返回值：
		//	其他说明：	
		//===================================================================
		bool operator << (OUT String^% mDestString, IN char* nSrcString)
		{			
			if (NULL == nSrcString)
			{
				return false;
			}
			mDestString = Marshal::PtrToStringAnsi(static_cast<IntPtr>(nSrcString));
			return true;
		}

		//===================================================================
		//	函数名称：CharToString
		//	功能描述：将IVS_CHAR拷贝成C#字符串，在原指针后面加0，防止因原数据不以0结束，导致数据错误
		//	调用清单：
		//	被调清单：
		//	输入参数：NA
		//	输出参数：NA
		//	  返回值：复制后的字符串
		//	其他说明：
		//===================================================================
		String^ CharToString(IVS_CHAR* nSrcString,int lenght)
		{			
			IVS_CHAR* charTemp=(IVS_CHAR*) malloc(lenght+1);

			memset_s(charTemp,lenght+1,0,lenght+1);

			memcpy_s(charTemp,lenght+1,nSrcString,lenght);
			String^ destString =  String(charTemp).ToString();

			delete charTemp;
			charTemp = NULL;

			return destString;
		}

		//===================================================================
		//	函数名称：SetLogPath
		//	功能描述：设置日志根目录
		//	调用清单：IVS_SDK
		//	被调清单：IvsVideoMonitor.cs
		//	输入参数：NA
		//	输出参数：NA
		//	  返回值：成功返回0，失败返回错误码
		//	其他说明：
		//===================================================================
		int IvsSdkClient::SetLogPath(String^ strLogPath)
		{
			IVS_CHAR psLogPath[IVS_FILE_NAME_LEN+1] = {0};

			psLogPath<<strLogPath;
			int result = IVS_SDK_SetLogPath(psLogPath);
			return result;
		}

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
		int IvsSdkClient::Init()
		{
			//int result = IVS_SDK_Cleanup();
			//获取启动路径
			wchar_t szCurrentDir[MAX_PATH] = {0};
			GetCurrentDirectory(MAX_PATH, szCurrentDir);
			TCHAR szModuleFilePath[MAX_PATH];    
			GetModuleFileName(NULL, szModuleFilePath, MAX_PATH );

			string  strModuleFilePath = TCHAR2STRING(szModuleFilePath);
			strModuleFilePath = strModuleFilePath.substr(0,strModuleFilePath.find_last_of("\\"));

			TCHAR* szBasePath =CHAR2WCHAR(strModuleFilePath.c_str());

			TCHAR szSDKPath[MAX_PATH] = L"\\IVSSDK\\";    
			TCHAR szAllPath[MAX_PATH];    
			_stprintf(szAllPath,_T("%s%s"),szBasePath, szSDKPath); 

			//设置DLL路径
			SetCurrentDirectory(szAllPath);
			SetCurrentDirectory(szCurrentDir);

			delete[] szBasePath;
			szBasePath=NULL;

			int result = IVS_SDK_Init();	
			return result;
		}

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
		int IvsSdkClient::Cleanup()
		{
			int result = IVS_SDK_Cleanup();	
			return result;
		}

		//===================================================================
		//	函数名称：EventCallBack
		//	功能描述：事件回调函数(事件通知，异常回调等)
		//	调用清单：IVS_SDK
		//	被调清单：IvsVideoMonitor.cs
		//	输入参数：iEventType			 事件类型		  
		//	输入参数：pEventBuf			pEventBuf
		//	输入参数：uiBufSize			buf长度
		//	输入参数：pUserData			用户数据，自定义，相应回调函数被调用时返回该参数
		//	  返回值：
		//	其他说明：
		//===================================================================
		IVS_VOID __SDK_CALL  EventCallBackFun(IVS_INT32  iEventType,IVS_VOID*  pEventBuf, IVS_UINT32  uiBufSize, IVS_VOID*   pUserData)
		{
			if (IVS_EVENT_USER_OFFLINE == iEventType||IVS_EVENT_LOGIN_FAILED ==iEventType)//用户下线通知、保活失败
			{
				IVS_CHAR*   sdkId = (IVS_CHAR*  )pUserData;
				IvsUserOfflineOff^ ivsUserOfflineOff = gcnew IvsUserOfflineOff();

				IvsSdkClient::cgwEventCallBackFunc(iEventType,ivsUserOfflineOff,uiBufSize, String(sdkId).ToString());
			}
		}

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
		int IvsSdkClient::Login(String^ ip, int port, String^ user, String^ password)
		{
			IVS_LOGIN_INFO* loginInfo = new IVS_LOGIN_INFO();

			loginInfo->stIP.cIP<<ip;
			loginInfo->stIP.uiIPType = IP_V4;
			loginInfo->uiPort=port;

			loginInfo->cUserName<<user;
			loginInfo->pPWD<<password;
			loginInfo->cMachineName<<System::Environment::MachineName;

			loginInfo->uiClientType = CLIENT_PC;
			loginInfo->uiLoginType = LOGIN_BASIC;
			IVS_INT32 sessionIdTemp;

			int result = IVS_SDK_Login(loginInfo, &sessionIdTemp);
			this->sessionId=sessionIdTemp;

			//将用户密码保存下来，用于下次重连
			this->ip=ip;
			this->password=password;
			this->port=port;
			this->user=user;

			delete loginInfo;
			loginInfo = NULL;

			EventCallBack fEventCallBack = &EventCallBackFun;
			IVS_SDK_SetEventCallBack(sessionIdTemp, fEventCallBack, NULL);

			return result;
		}

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
		int IvsSdkClient::ReLogin()
		{
			int result = Login(this->ip,this->port,this->user,this->password);
			return result;
		}

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
		int IvsSdkClient::Logout()
		{
			int result = IVS_SDK_Logout(sessionId);	
			return result;
		}

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
		int IvsSdkClient::GetDeviceList(int fromIndex,int toIndex,[Out]int% totalCameraCount,[Out]List<IvsCamera^>^% cameraList)
		{
			cameraList = gcnew List<IvsCamera^>();

			//分配内存空间
			IVS_UINT32 uiReqNum = (toIndex - fromIndex) + 1; 
			IVS_UINT32 uiRspBufferSize = 0;
			uiRspBufferSize = sizeof(IVS_CAMERA_BRIEF_INFO_LIST) + (uiReqNum-1)*sizeof(IVS_CAMERA_BRIEF_INFO);
			IVS_VOID *pRspBuffer = new IVS_CHAR[uiRspBufferSize]; 
			
			memset_s(pRspBuffer,uiRspBufferSize,0,uiRspBufferSize);

			IVS_INDEX_RANGE stRange;
			stRange.uiToIndex = toIndex;
			stRange.uiFromIndex = fromIndex;

			int result = IVS_SDK_GetDeviceList(sessionId,DEVICE_TYPE_CAMERA,&stRange,pRspBuffer,uiRspBufferSize);
			//如果错误为“无效的会话ID”或“登录信息已失效，请重新登录”，则重新登录后再次执行，断线重连
			if (result==IVS_MU_INVALID_SESSION_ID||result==IVS_SCU_ONLINE_USER_EXPIRE||result==IVS_SMU_USER_OFF_LINE||result==IVS_SDK_RET_INVALID_SESSION_ID)
			{
				//重新登录
				ReLogin();				
				
				memset_s(pRspBuffer,uiRspBufferSize,0,uiRspBufferSize);
				result = IVS_SDK_GetDeviceList(sessionId,DEVICE_TYPE_CAMERA,&stRange,pRspBuffer,uiRspBufferSize);
			}

			if (result != IVS_SUCCEED)
			{
				delete[] pRspBuffer;
				pRspBuffer = NULL;
				return result;
			}
			IVS_CAMERA_BRIEF_INFO_LIST *pRspDeviceList = (IVS_CAMERA_BRIEF_INFO_LIST*)pRspBuffer; 
			IVS_UINT32 iNum = (pRspDeviceList->stIndexRange.uiToIndex - pRspDeviceList->stIndexRange.uiFromIndex) + 1;
			if (0 == pRspDeviceList->uiTotal)
			{
				iNum = 0;
			}
			//实际记录数小于分页参数查询的记录数的时候，只遍历实际的记录数
			else if((pRspDeviceList->uiTotal - pRspDeviceList->stIndexRange.uiFromIndex + 1)<iNum ) 
			{				
				iNum = (pRspDeviceList->uiTotal - pRspDeviceList->stIndexRange.uiFromIndex) + 1;
			}
			totalCameraCount=pRspDeviceList->uiTotal;


			IVS_UINT32 i = 0;

			while (i < iNum)
			{
				IVS_CAMERA_BRIEF_INFO &devInfo = pRspDeviceList->stCameraBriefInfo[i];

				//C#数据结构，将C++的数据结构转成C#的数据结构
				IvsCamera^ ivsCamera = gcnew IvsCamera();

				ivsCamera->Name = CharToString(devInfo.cName,IVS_CAMERA_NAME_LEN);
				ivsCamera->No = CharToString(devInfo.cCode,IVS_DEV_CODE_LEN);
				int typeTemp = devInfo.uiType;
				ivsCamera->Type = (IvsCameraType)typeTemp;
				ivsCamera->GroupNo = CharToString(devInfo.cDevGroupCode,IVS_DEVICE_GROUP_LEN);
				ivsCamera->ParentNo =  CharToString(devInfo.cParentCode,IVS_DEV_CODE_LEN);
				ivsCamera->DomainCode =CharToString(devInfo.cDomainCode,IVS_DOMAIN_CODE_LEN);
				if (DEV_STATUS_ONLINE == devInfo.uiStatus)
				{
					ivsCamera->Status = CameraStatus::Online;
				}else
				{
					ivsCamera->Status = CameraStatus::Offline;
				}

				cameraList->Add(ivsCamera);
				i++;
			}

			delete[] pRspBuffer;
			pRspBuffer = NULL;

			return result;
		}

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
		int IvsSdkClient::GetDomainRoute([Out]List<IvsDomainRoute^>^% domainRouteList)
		{
			domainRouteList = gcnew List<IvsDomainRoute^>();

			//目前最大支持128个
			IVS_UINT32 uiReqNum = IVS_MAX_DOMAIN_ROUTE_NUM; 
			IVS_UINT32 uiBufferSize = sizeof(IVS_DOMAIN_ROUTE_LIST) + (uiReqNum-1)*sizeof(IVS_DOMAIN_ROUTE);
			IVS_DOMAIN_ROUTE_LIST *routeList =  (IVS_DOMAIN_ROUTE_LIST*)malloc(uiBufferSize);
			
			memset_s(routeList,uiBufferSize,0,uiBufferSize);

			int result = IVS_SDK_GetDomainRoute(sessionId,routeList,uiBufferSize);

			//如果错误为“无效的会话ID”或“登录信息已失效，请重新登录”，则重新登录后再次执行，断线重连
			if (result==IVS_MU_INVALID_SESSION_ID||result==IVS_SCU_ONLINE_USER_EXPIRE||result==IVS_SMU_USER_OFF_LINE||result==IVS_SDK_RET_INVALID_SESSION_ID)
			{
				//重新登录
				ReLogin();
				
				memset_s(routeList,uiBufferSize,0,uiBufferSize);
				result = IVS_SDK_GetDomainRoute(sessionId,routeList,uiBufferSize);
			}

			if (result!= IVS_SUCCEED)
			{
				//delete routeList;
				free(routeList);
				routeList = NULL;
				return result;
			}

			IVS_UINT32 i = 0;			
			while (i < routeList->uiTotal)
			{
				IVS_DOMAIN_ROUTE &domainRoute = routeList->stDomainRoute[i];

				IvsDomainRoute^ ivsDomainRoute = gcnew IvsDomainRoute();
				ivsDomainRoute->Ip = CharToString(domainRoute.stIP.cIP,IVS_IP_LEN);
				ivsDomainRoute->Port = domainRoute.uiPort;
				ivsDomainRoute->ParentDomain = CharToString(domainRoute.cSuperDomain,IVS_DOMAIN_CODE_LEN);
				ivsDomainRoute->DomainCode = CharToString(domainRoute.cDomainCode,IVS_DOMAIN_CODE_LEN);
				ivsDomainRoute->DomainName = CharToString(domainRoute.cDomainName,IVS_NAME_LEN);

				domainRouteList->Add(ivsDomainRoute);
				i++;
			}

			delete routeList;
			routeList = NULL;

			return result;
		}

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
		int IvsSdkClient::GetDeviceGroupList(String^ domainCode,String^ rootGroupNo,[Out]Dictionary<String^,IvsCameraGroup^>^% cameraGroupDic)
		{
			cameraGroupDic = gcnew Dictionary<String^,IvsCameraGroup^>();
			IVS_UINT32 uiReqNum = IVS_MAX_DEV_GROUP_NUM; 
			IVS_UINT32 uiBufferSize = 0;
			uiBufferSize = sizeof(IVS_DEVICE_GROUP_LIST) + (uiReqNum-1)*sizeof(IVS_DEVICE_GROUP);
			IVS_DEVICE_GROUP_LIST *groupList = (IVS_DEVICE_GROUP_LIST*)malloc(uiBufferSize);
			
			memset_s(groupList,uiBufferSize,0,uiBufferSize);

			//IVS返回的domainCode长度为32位，最后一位不为0，因此调用时将长度+1
			IVS_CHAR domain[IVS_DOMAIN_CODE_LEN+1];
			
			memset_s(domain,IVS_DOMAIN_CODE_LEN+1,0,IVS_DOMAIN_CODE_LEN+1);
			domain<<domainCode;

			IVS_CHAR rootGroupCode[IVS_DEVICE_GROUP_LEN+1];
			
			memset_s(rootGroupCode,IVS_DEVICE_GROUP_LEN+1,0,IVS_DEVICE_GROUP_LEN+1);
			rootGroupCode<<rootGroupNo;

			int result = IVS_SDK_GetDeviceGroup(sessionId,domain,rootGroupCode,groupList,uiBufferSize);

			//如果错误为“无效的会话ID”或“登录信息已失效，请重新登录”，则重新登录后再次执行，断线重连
			if (result==IVS_MU_INVALID_SESSION_ID||result==IVS_SCU_ONLINE_USER_EXPIRE||result==IVS_SMU_USER_OFF_LINE||result==IVS_SDK_RET_INVALID_SESSION_ID)
			{
				//重新登录
				ReLogin();
				//memset(groupList,0,uiBufferSize);
				memset_s(groupList,uiBufferSize,0,uiBufferSize);
				result = IVS_SDK_GetDeviceGroup(sessionId,domain,rootGroupCode,groupList,uiBufferSize);
			}

			if (result!= IVS_SUCCEED)
			{
				delete groupList;
				groupList = NULL;
				return result;
			}

			IVS_UINT32 i = 0;
			while (i < groupList->uiTotal)
			{
				IVS_DEVICE_GROUP &group = groupList->stDeviceGroup[i];
				IvsCameraGroup^ ivsCameraGroup = gcnew IvsCameraGroup();
				ivsCameraGroup->GroupNo = CharToString(group.cGroupCode,IVS_DEVICE_GROUP_LEN);
				ivsCameraGroup->GroupName = CharToString(group.cGroupName,IVS_NAME_LEN);
				ivsCameraGroup->DomainCode = domainCode;
				ivsCameraGroup->ParentGroupNo = CharToString(group.cParentGroupCode,IVS_DEVICE_GROUP_LEN);
				//解决问题单DTS2013080201001，规避因IVS服务器存在摄像头重复的bug导致融合网关异常的问题
				if (!cameraGroupDic->ContainsKey(ivsCameraGroup->GroupNo))
				{
					cameraGroupDic->Add(ivsCameraGroup->GroupNo,ivsCameraGroup);
				}
				i++;
			}

			delete groupList;
			groupList = NULL;

			return result;
		}

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
		void IvsSdkClient::SetRealPlayCBRawCallBackFunc(IvsRealPlayCallBackRawFunc^ callbackFunc)
		{			
			cgwCallbackFunc=callbackFunc;
		}

		array<Byte>^ NativeArr2ManagedArrByte(BYTE* nByArr, unsigned int nArrLen)
		{
			array<Byte>^ mByArr = gcnew array<Byte>(nArrLen);
			if(NULL == nByArr)
			{
				return mByArr;
			}
			for(unsigned int i = 0; i < nArrLen; i++)
			{
				mByArr[i] = nByArr[i];
			}
			return mByArr;
		}

		//===================================================================
		//	函数名称：IvsRealPlayCallBackRaw
		//	功能描述：码流回调函数，接收SDK的码流
		//	调用清单：IvsVideoMonitor.cs
		//	被调清单：IVS_SDK
		//	输入参数：callbackFunc			码流回调函数RealPlayCallBackRaw		  
		//	输出参数：NA
		//	  返回值：成功返回0，失败返回错误码
		//	其他说明：
		//===================================================================
		IVS_VOID __SDK_CALL IvsRealPlayCallBackRaw(IVS_ULONG ulHandle, IVS_RAW_FRAME_INFO* pRawFrameInfo, IVS_VOID* pBuf, IVS_UINT32 uiBufSize, IVS_VOID* pUserData)
		{			
			IVS_CHAR*   sdkId = (IVS_CHAR*  )pUserData;
			if (0 != strcmp(localId,sdkId))
			{
				//不是该平台的回调，舍弃
				return;
			}

			IVS_CHAR*  dataBuffer = (IVS_CHAR* )pBuf;
			array<Byte>^ buf = NativeArr2ManagedArrByte((BYTE*)dataBuffer,uiBufSize);

			IvsRawFrameInfo^ ivsRawFrameInfo = gcnew IvsRawFrameInfo();

			ivsRawFrameInfo->StreamType = pRawFrameInfo->uiStreamType;
			ivsRawFrameInfo->FrameType = pRawFrameInfo->uiFrameType;
			ivsRawFrameInfo->GopSequence = pRawFrameInfo->uiGopSequence;
			ivsRawFrameInfo->Height = pRawFrameInfo->uiHeight;
			ivsRawFrameInfo->TimeStamp = pRawFrameInfo->uiTimeStamp;
			ivsRawFrameInfo->WaterMarkValue = pRawFrameInfo->uiWaterMarkValue;
			ivsRawFrameInfo->Width = pRawFrameInfo->uiWidth;
			ivsRawFrameInfo->TimeTick = pRawFrameInfo->ullTimeTick;

			IvsSdkClient::cgwCallbackFunc(ulHandle,ivsRawFrameInfo,buf,uiBufSize, String(sdkId).ToString());

		}

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
		int IvsSdkClient::StartRealPlayCBRaw(String^ cameraNo,[Out]long%  handle)
		{

			IVS_CHAR pCameraCode[IVS_DEV_CODE_LEN];
			
			memset_s(pCameraCode,IVS_DEV_CODE_LEN,0,IVS_DEV_CODE_LEN);
			pCameraCode<<cameraNo;

			////2015/3/3 获取设备的网络配置参数，如：TCP或UDP
			//IntPtr ip = Marshal::StringToHGlobalAnsi(cameraNo);
			//IVS_CHAR* tempBuf = static_cast<IVS_CHAR *>(ip.ToPointer());

			//IVS_CAMERA_STREAM_CFG deviceInfo={0};
			//IVS_SDK_GetDeviceConfig(sessionId,tempBuf,CONFIG_CAMERA_STREAM_CFG,&deviceInfo,sizeof(deviceInfo));

			IVS_REALPLAY_PARAM  pRealplayPara;
			pRealplayPara.bDirectFirst = MEDIA_TRANS;                           //中转，枚举IVS_MEDIA_TRANS_MODE    
			pRealplayPara.bMultiCast = BROADCAST_UNICAST;						//单播，枚举IVS_BROADCAST_TYPE
			pRealplayPara.uiProtocolType = PROTOCOL_RTP_OVER_UDP;				//UDP协议，枚举IVS_PROTOCOL_TYPE
			pRealplayPara.uiStreamType = STREAM_TYPE_MAIN;						//主码流，枚举IVS_STREAM_TYPE

			/*	for(int i=0;i< deviceInfo.uiStreamInfoNum;i++)
			{
			if(deviceInfo.stStreamInfo[i].uiStreamType==STREAM_TYPE_MAIN)
			{
			pRealplayPara.uiProtocolType=deviceInfo.stStreamInfo[i].uiTransProtocol;
			break;
			}
			}*/

			RealPlayCallBackRaw ivsCallbackFunc = &IvsRealPlayCallBackRaw;
			IVS_ULONG iPlayHandle = 0;

			int result = IVS_SDK_StartRealPlayCBRaw(sessionId, &pRealplayPara, pCameraCode,ivsCallbackFunc, localId,&iPlayHandle);

			//如果错误为“无效的会话ID”或“登录信息已失效，请重新登录”，则重新登录后再次执行，断线重连
			if (result==IVS_MU_INVALID_SESSION_ID||result==IVS_SCU_ONLINE_USER_EXPIRE||result==IVS_SMU_USER_OFF_LINE||result==IVS_SDK_RET_INVALID_SESSION_ID)
			{
				//重新登录
				ReLogin();
				result = IVS_SDK_StartRealPlayCBRaw(sessionId, &pRealplayPara, pCameraCode,ivsCallbackFunc, localId,&iPlayHandle);
			}

			handle = iPlayHandle;

			return result;
		}

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
		int IvsSdkClient::StopRealPlay(long handle)
		{
			int result = IVS_SDK_StopRealPlay(sessionId,handle);

			//如果错误为“无效的会话ID”或“登录信息已失效，请重新登录”，则重新登录后再次执行，断线重连
			if (result==IVS_MU_INVALID_SESSION_ID||result==IVS_SCU_ONLINE_USER_EXPIRE||result==IVS_SMU_USER_OFF_LINE||result==IVS_SDK_RET_INVALID_SESSION_ID)
			{
				//重新登录
				ReLogin();
				result = IVS_SDK_StopRealPlay(sessionId,handle);
			}
			return result;
		}

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
		int IvsSdkClient::StartPtzControl(String^ cameraNo,int controlCode,String^ param)
		{
			IVS_CHAR pCameraCode[IVS_DEV_CODE_LEN];
			
			memset_s(pCameraCode,IVS_DEV_CODE_LEN,0,IVS_DEV_CODE_LEN);
			pCameraCode<<cameraNo;

			IVS_CHAR pControlPara2[IVS_DEV_CODE_LEN];
			
			memset_s(pControlPara2,IVS_DEV_CODE_LEN,0,IVS_DEV_CODE_LEN);
			pControlPara2<<param;

			//云台锁定状态：0-解锁，1-锁定
			IVS_UINT32 pLockStatus = 0;
			//使用连续模式，第四个参数2表示连续模式
			int result = IVS_SDK_PtzControl(sessionId,pCameraCode, controlCode,"2",pControlPara2, &pLockStatus);

			//如果错误为“无效的会话ID”或“登录信息已失效，请重新登录”，则重新登录后再次执行，断线重连
			if (result==IVS_MU_INVALID_SESSION_ID||result==IVS_SCU_ONLINE_USER_EXPIRE||result==IVS_SMU_USER_OFF_LINE||result==IVS_SDK_RET_INVALID_SESSION_ID)
			{
				//重新登录
				ReLogin();
				result = IVS_SDK_PtzControl(sessionId,pCameraCode, controlCode,"2",pControlPara2, &pLockStatus);
			}

			return result;
		}

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
		int IvsSdkClient::StopPtzControl(String^ cameraNo)
		{
			IVS_CHAR pCameraCode[IVS_DEV_CODE_LEN];
			
			memset_s(pCameraCode,IVS_DEV_CODE_LEN,0,IVS_DEV_CODE_LEN);
			pCameraCode<<cameraNo;

			//云台锁定状态：0-解锁，1-锁定
			IVS_UINT32 pLockStatus = 0;

			//PTZ_STOP来自枚举IVS_PTZ_CODE,IVS侧要求，当是停止命令PTZ_STOP时，pControlPara1，pControlPara2随意传,但不能为NULL
			int result = IVS_SDK_PtzControl(sessionId,pCameraCode,PTZ_STOP,"","", &pLockStatus);

			//如果错误为“无效的会话ID”或“登录信息已失效，请重新登录”，则重新登录后再次执行，断线重连
			if (result==IVS_MU_INVALID_SESSION_ID||result==IVS_SCU_ONLINE_USER_EXPIRE||result==IVS_SMU_USER_OFF_LINE||result==IVS_SDK_RET_INVALID_SESSION_ID)
			{
				//重新登录
				ReLogin();
				result = IVS_SDK_PtzControl(sessionId,pCameraCode,PTZ_STOP,"","", &pLockStatus);
			}

			return result;
		}

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
		void IvsSdkClient::SetEventCallBackFunc(IvsEventCallBackFunc^ callbackFunc)
		{			
			cgwEventCallBackFunc=callbackFunc;
		}
	}
}