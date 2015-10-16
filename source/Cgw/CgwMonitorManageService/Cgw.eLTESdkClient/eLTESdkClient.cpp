//===================================================================
//	Copyright(C)2013-2020, Huawei Tech. Co., Ltd.
//
//	文件名	：OperationProxy.cpp
//	作  者	：w00206574
//	完成日期：2014-04-08
//	说  明	：实现eLTE接口C++ 和C#之间转换
//	其  它	：
//	历史记录：
//===================================================================
#include "StdAfx.h"
#include <tchar.h>
#include <string>
#include <iostream>
#include <fstream>
#include <windows.h>

#include "eLTESdkClient.h"
#include "eLTE_SDK.h"
#include "process_xml.h"
#include "securec.h"

#define STR_LENGTH_SIZE  20
#define INT_FORMAT_D	 10
#define WAIT_OBJECT_TIME 5000 

using namespace std;

namespace CgwMonitorManage
{
	namespace eLTE
	{	
		//服务器IP
		char cServerIP[50] = {0};
		//用户名
		char cUserName[50] = {0};
		//sip端口
		char cSipPort[50] = {0};
		//本地IP 
		char cLocalIP[50] = {0};
		//用户密码
		char cPassWord[50] = {0};

		//===================================================================
		//	函数名称：	
		//	功能描述：	实现wchar_t和std::string互转
		//	调用清单：
		//	被调清单：
		//	输入参数：wchar_t str	--	[in] wchar_t
		//	输出参数：  
		//	  返回值： std::string   -- [out]  std::string字符串
		//	其他说明：
		//===================================================================
		string WSToString(const std::wstring& ws)
		{
			std::string curLocale = setlocale(LC_ALL, NULL);
			setlocale(LC_ALL, "chs");
			const wchar_t* _Source = ws.c_str();
			size_t _Dsize = 2 * ws.size() + 1;
			char *_Dest = new char[_Dsize];
			memset_s(_Dest,_Dsize,0,_Dsize);
			wcstombs(_Dest,_Source,_Dsize);
			string result = _Dest;
			delete []_Dest;
			setlocale(LC_ALL, curLocale.c_str());
			return result;
		}

		// int 转换为 std::string
		std::string Int2String(int iVar)
		{
			char buf[STR_LENGTH_SIZE] = {0};
			(void)_itoa_s(iVar, buf, INT_FORMAT_D);
			return std::string(buf);
		}

		string ANSIToUnicode(const std::string& str)
		{
			string strResult("");

			int textlen = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);
			if (0 >= textlen)
			{
				// MultiByteToWideChar failed.
				return strResult;
			}

			size_t bufsize = (size_t)(textlen+1);
			wchar_t* pBuf = new wchar_t[bufsize];

			memset_s(pBuf,sizeof(wchar_t)*bufsize,0,sizeof(wchar_t)*bufsize);
			MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, (LPWSTR)pBuf, textlen);//lint !e713

			wstring  rt = pBuf;
			//实现wchar_t和std::string互转
			strResult= WSToString(rt);
			delete[] pBuf;

			return strResult;
		}

		//===================================================================
		//	函数名称：	
		//	功能描述：	实现UTF-8编码和Unicode编码互转
		//	调用清单：
		//	被调清单：
		//	输入参数：	str	--	[in] utf_8
		//	输出参数：  
		//	  返回值：  wstring  -- [out]  Unicode类型字符串
		//	其他说明：
		//===================================================================
		string UTF8ToUnicode(string str)
		{
			int  ilen = str.length();
			int  iUnicodeLen = ::MultiByteToWideChar( CP_UTF8,
				0,
				str.c_str(),
				-1,
				NULL,
				0 ); 
			wchar_t*  pUnicode = new  wchar_t[iUnicodeLen+1];

			memset_s(pUnicode,(iUnicodeLen+1)*sizeof(wchar_t),0,(iUnicodeLen+1)*sizeof(wchar_t));
			MultiByteToWideChar( CP_UTF8,
				0,
				str.c_str(),
				-1,
				(LPWSTR)pUnicode,
				iUnicodeLen ); 

			wstring  rt = ( wchar_t* )pUnicode;
		   //实现wchar_t和std::string互转
			string strRet= WSToString(rt);

			if(pUnicode)
			{
				delete[] pUnicode;
				pUnicode=NULL;
			}
			
			return  strRet; 
		}

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
 
			memset_s(wBuf,(wlen+1)*sizeof(wchar_t),0,(wlen+1)*sizeof(wchar_t));
			MultiByteToWideChar(CP_ACP, 
				0, 
				(const char*)buffer, 
				int(len), 
				wBuf, 
				int(wlen));
			return wBuf;
		} 

		//===================================================================
		//	函数名称：	
		//	功能描述：	将managed string转换成native ansi string
		//	调用清单：
		//	被调清单：
		//	输入参数：	nSrcString	--	[in]	 Managed String
		//	输出参数：  mDestString -- [out]   Native char类型字符串
		//	  返回值：
		//	其他说明：不检查目标字符串缓冲区会越界
		//===================================================================
		bool operator << (OUT char* nDestString, IN String^ mSrcString)
		{
			if (NULL == nDestString || nullptr == mSrcString)
			{
				return false;
			}
			IntPtr ip = System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(mSrcString);
			char* tempBuf = static_cast<char *>(ip.ToPointer());

			//便于使用方便，nDestString长度由调用者保证，这里传的长度保证能完整拷贝源字符串；
			strcpy_s(nDestString,strlen(tempBuf)+1,tempBuf);

			System::Runtime::InteropServices::Marshal::FreeHGlobal(ip);
			return true;
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
			mDestString = System::Runtime::InteropServices::Marshal::PtrToStringAnsi(static_cast<IntPtr>(nSrcString));
			return true;
		}

		ELTE_VOID __SDK_CALL EventCallBackFunc(ELTE_INT32 iEventType, ELTE_VOID* pEventBuf, ELTE_UINT32 uiBufSize, ELTE_VOID* pUserData)
		{
			if(NULL != pUserData)
			{
				TDDisplayMnager* displayMnager = (TDDisplayMnager*)pUserData;
				string strEventBuf = "";
				if (NULL != pEventBuf)
				{
					ELTE_CHAR* pBuf = NULL;
					SAFE_NEW_A(pBuf, ELTE_CHAR, uiBufSize + 1);
					
					memset_s(pBuf,uiBufSize+1,0x00,uiBufSize+1);
					
					memcpy_s(pBuf,uiBufSize+1,pEventBuf,uiBufSize);
					strEventBuf = pBuf;
					SAFE_DELETE_A(pBuf)
				}
				switch(iEventType)
				{
				case ELTE_Event_NotifyProvisionAllResync:
					{
						displayMnager->notifyProvisionAllResync();
					}
					break;	
				case ELTE_Event_NotifyResourceStatus:
					{						
						displayMnager->notifyResourceStatus(strEventBuf);
					}
					break;
				case ELTE_Event_NotifyUserStatus:
					{						
						displayMnager->notifyUserStatus(strEventBuf);
					}
					break;
				case ELTE_Event_NotifyProvisionChange:
					{						
						displayMnager->notifyProvisionChange(strEventBuf);
					}
					break;
				case ELTE_Event_NotifyModuleStatus:
					{						
						displayMnager->notifyModuleStatus(strEventBuf);
					}
					break;
				case ELTE_Event_NotifyP2pvideocallStatus:
					{						
						displayMnager->notifyP2pvideocallStatus(strEventBuf);
					}
					break;
				case ELTE_Event_NotifyP2pcallStatus:
					{						
						displayMnager->notifyP2pcallStatus(strEventBuf);
					}
					break;
				case ELTE_Event_NotifyGroupStatus:
					{						
						displayMnager->notifyGroupStatus(strEventBuf);
					}
					break;
				default:
					{
					}
				}
			}
		}

		//===================================================================
		//	函数名称：eLTESdkClient
		//	功能描述：构造函数
		//	调用清单：NA
		//	被调清单：OperationProxy.cpp
		//	输入参数：NA
		//	输出参数：NA
		//	  返回值：NA
		//	其他说明：
		//===================================================================
		eLTESdkClient::eLTESdkClient():bypassMedia(true)
		{
	
		}

		eLTESdkClient::~eLTESdkClient()
		{
			try
			{
				GC::Collect();
			}
			catch (...)
			{
			}	
		}

		//===================================================================
		//	函数名称：	
		//	功能描述：	获取一个用户的详细信息函数的函数名、参数、描述、返回值以及相关操作
		//	调用清单：
		//	被调清单：
		//	输入参数：	userid	--	[in]	用户ID
		//	输出参数：  userInfo  -- [out]   用户信息结构体
		//	  返回值：
		//	其他说明：	
		//===================================================================
		ELTE_INT32 eLTESdkClient:: GetUserInfo(int userid,[System::Runtime::InteropServices::Out]eLTEUserInfo^% userInfo)
		{
			userInfo = gcnew eLTEUserInfo();

			//接口调用
			string strUserId = Int2String(userid);
			ELTE_CHAR* pUserInfo = NULL;
			ELTE_INT32 iRet = ELTE_SDK_GetUserInfo(strUserId.c_str(), &pUserInfo);
			if(RET_SUCCESS != iRet)
			{
				return iRet;
			}

			//xml 解析
			CXml rspXml;
			if(!rspXml.Parse(pUserInfo))
			{
				return RET_PARSE_XML_FAILURE;
			}

			if(!rspXml.FindElemEx("Content/UserInfo"))
			{
				return RET_PARSE_XML_FAILURE;
			}
			if(!rspXml.FindElem("UserID"))
			{
				return RET_PARSE_XML_FAILURE;
			}
			userInfo->userid = atoi(rspXml.GetElemValue());
			if(!rspXml.FindElem("UserName"))
			{
				return RET_PARSE_XML_FAILURE;
			}
			userInfo->username = String(ANSIToUnicode(rspXml.GetElemValue()).c_str()).ToString();
			if(!rspXml.FindElem("UserCategory"))
			{
				return RET_PARSE_XML_FAILURE;
			}
			userInfo->usercategory = (eLTEUser_Category_t)(atoi(rspXml.GetElemValue()));

			//释放内存
			iRet = ELTE_SDK_ReleaseBuffer(pUserInfo);
			if(RET_SUCCESS != iRet)
			{
				return iRet;
			}
			return RET_SUCCESS;
		}

		//===================================================================
		//	函数名称：	
		//	功能描述：	获得某个组里的所有User，返回值为一份克隆的数据，由调用者负责操纵并最终销毁
		//	调用清单：
		//	被调清单：
		//	输入参数：	grpid	--	[in]	用户分组ID
		//	输出参数：  groupUserList  -- [out]   用户列表
		//	  返回值：
		//	其他说明：	
		//===================================================================
		ELTE_INT32 eLTESdkClient:: GetGroupUsers(int grpid,[System::Runtime::InteropServices::Out]List<eLTEGrpUserInfo^>^% groupUserList)
		{
			groupUserList = gcnew List<eLTEGrpUserInfo^>();

			//接口调用
			string strGrpId = Int2String(grpid);
			ELTE_CHAR* pGroupUsers = NULL;
			ELTE_INT32 iRet = ELTE_SDK_GetGroupUsers(strGrpId.c_str(), &pGroupUsers);
			if(RET_SUCCESS != iRet)
			{
				return iRet;
			}

			//xml 解析
			CXml rspXml;
			if(!rspXml.Parse(pGroupUsers))
			{
				return RET_PARSE_XML_FAILURE;
			}

			if(!rspXml.FindElemEx("Content/GroupUserInfoList"))
			{
				return RET_PARSE_XML_FAILURE;
			}

			if(!rspXml.FindElem("GroupUserInfo"))
			{
				return RET_PARSE_XML_FAILURE;
			}

			do 
			{
				if(rspXml.IntoElem())
				{
					//C#数据结构，将C++的数据结构转成C#的数据结构
					eLTEGrpUserInfo^ groupUser = gcnew eLTEGrpUserInfo();
					if(!rspXml.FindElem("GroupID"))
					{
						return RET_PARSE_XML_FAILURE;
					}
					groupUser->grpid = atoi(rspXml.GetElemValue());
					if(!rspXml.FindElem("MemberType"))
					{
						return RET_PARSE_XML_FAILURE;
					}
					groupUser->memberType = (eLTEGroup_Member_t)(atoi(rspXml.GetElemValue()));
					if(!rspXml.FindElem("UserID"))
					{
						return RET_PARSE_XML_FAILURE;
					}
					groupUser->userid = atoi(rspXml.GetElemValue());

					groupUserList->Add(groupUser);
					(void)rspXml.OutOfElem();
				}
			} while (rspXml.NextElem());

			//释放内存
			iRet = ELTE_SDK_ReleaseBuffer(pGroupUsers);
			if(RET_SUCCESS != iRet)
			{
				return iRet;
			}

			return RET_SUCCESS;
		}
		
		//===================================================================
		//	函数名称：	
		//	功能描述：	获得本DC管理的所有User(包括fixeduser类型)，返回值为一份克隆的数据，由调用者负责操纵并最终销毁
		//	调用清单：
		//	被调清单：
		//	输入参数：	proviId	--	[in]	调度台的用户号
		//	输出参数：  groupList  -- [out]   获取调度台作为成员的群组的列表。若无，则返回NULL
		//	  返回值：
		//	其他说明：	
		//===================================================================
		ELTE_INT32 eLTESdkClient:: GetAllGroups(int proviId,[System::Runtime::InteropServices::Out]List<eLTEGroupInfo^>^% groupList)
		{
			groupList = gcnew List<eLTEGroupInfo^>();

			//接口调用
			string strProviId = Int2String(proviId);
			ELTE_CHAR* pAllGroups = NULL;
			ELTE_INT32 iRet = ELTE_SDK_GetDcGroups(strProviId.c_str(), &pAllGroups);
			if(RET_SUCCESS != iRet)
			{
				return iRet;
			}

			//xml 解析
			CXml rspXml;
			if(!rspXml.Parse(pAllGroups))
			{
				return RET_PARSE_XML_FAILURE;
			}

			if(!rspXml.FindElemEx("Content/GroupInfoList"))
			{
				return RET_PARSE_XML_FAILURE;
			}

			if(!rspXml.FindElem("GroupInfo"))
			{
				return RET_PARSE_XML_FAILURE;
			}

			do 
			{
				if(rspXml.IntoElem())
				{
					//C#数据结构，将C++的数据结构转成C#的数据结构
					eLTEGroupInfo^ groupInfo = gcnew eLTEGroupInfo();
					if(!rspXml.FindElem("GroupID"))
					{
						return RET_PARSE_XML_FAILURE;
					}
					groupInfo->grpid = atoi(rspXml.GetElemValue());
					if(!rspXml.FindElem("GroupName"))
					{
						return RET_PARSE_XML_FAILURE;
					}
					groupInfo->grpname = String(ANSIToUnicode(rspXml.GetElemValue()).c_str()).ToString();

					groupList->Add(groupInfo);
					(void)rspXml.OutOfElem();
				}
			} while (rspXml.NextElem());

			//释放内存
			iRet = ELTE_SDK_ReleaseBuffer(pAllGroups);
			if(RET_SUCCESS != iRet)
			{
				return iRet;
			}
			return RET_SUCCESS;
		}

		//===================================================================
		//	函数名称：	
		//	功能描述：	获得本DC管理的所有User (包括fixeduser 类型)， 返回值为一份克隆的数据，由调用者负责操纵并最终销毁
		//	调用清单：
		//	被调清单：
		//	输入参数：	grpid	--	[in]	本调度台的用户号; 如果 Dcid==0，则返回全网所有的User
		//	输出参数：  groupUserList  -- [out]   用户列表
		//	  返回值：
		//	其他说明：	
		//===================================================================
		ELTE_INT32 eLTESdkClient:: GetAllDcUsers(int grpid,[System::Runtime::InteropServices::Out]List<eLTEUserInfo^>^% dcUsersList)
		{
			dcUsersList = gcnew List<eLTEUserInfo^>();

			//接口调用
			string strGrpId = Int2String(grpid);
			ELTE_CHAR* pAllDcUsers = NULL;
			ELTE_INT32 iRet = ELTE_SDK_GetDcUsers(strGrpId.c_str(), &pAllDcUsers);
			if(RET_SUCCESS != iRet)
			{
				return iRet;
			}

			//xml 解析
			CXml rspXml;
			if(!rspXml.Parse(pAllDcUsers))
			{
				return RET_PARSE_XML_FAILURE;
			}

			if(!rspXml.FindElemEx("Content/UserInfoList"))
			{
				return RET_PARSE_XML_FAILURE;
			}

			if(!rspXml.FindElem("UserInfo"))
			{
				return RET_PARSE_XML_FAILURE;
			}

			do 
			{
				if(rspXml.IntoElem())
				{
					//C#数据结构，将C++的数据结构转成C#的数据结构
					eLTEUserInfo^ userInfo = gcnew eLTEUserInfo();
				
					if(!rspXml.FindElem("UserID"))
					{
						return RET_PARSE_XML_FAILURE;
					}
					userInfo->userid = atoi(rspXml.GetElemValue());
					if(!rspXml.FindElem("UserName"))
					{
						return RET_PARSE_XML_FAILURE;
					}
					userInfo->username = String(ANSIToUnicode(rspXml.GetElemValue()).c_str()).ToString();
					if(!rspXml.FindElem("UserCategory"))
					{
						return RET_PARSE_XML_FAILURE;
					}
					userInfo->usercategory = (eLTEUser_Category_t)(atoi(rspXml.GetElemValue()));

					dcUsersList->Add(userInfo);
					(void)rspXml.OutOfElem();
				}
			} while (rspXml.NextElem());

			//释放内存
			iRet = ELTE_SDK_ReleaseBuffer(pAllDcUsers);
			if(RET_SUCCESS != iRet)
			{
				return iRet;
			}
			return RET_SUCCESS;
		}

		//===================================================================
		//	函数名称：	Set_Login_Info
		//	功能描述：	设置登录服务器IP、本地IP、用户名、密码、sip端口等信息
		//	调用清单：
		//	被调清单：
		//	输入参数：	服务器IP、本地IP、用户名、密码、sip端口
		//	输出参数：  无
		//	  返回值：
		//	其他说明：	
		//===================================================================
		ELTE_INT32 eLTESdkClient:: Set_Login_Info(String^ CSserverIP,String^ CSlocalIP,String^ CSuserName,String^ CSpassWord,String^ CSsipPort,int CSDCID)
		{
			try
			{	
				this->serverIP=CSserverIP;
				this->localIP=CSlocalIP;
				this->userName=CSuserName;
				this->passWord=CSpassWord;
				this->sipPort=CSsipPort;
				this->DCID=CSDCID;

				cServerIP<<this->serverIP;
				cUserName<<this->userName;
				cPassWord<<this->passWord;
				cSipPort<<this->sipPort;
				cLocalIP<<this->localIP;
			}
			catch (...)
			{
				return RET_FAILURE;
			}
			return RET_SUCCESS;
		}

		//===================================================================
		//	函数名称：	App_Login
		//	功能描述：	登录、注销SDK 
		//	调用清单：
		//	被调清单：
		//	输入参数：	bLogin：true表实注销
		//	输出参数：  无
		//	  返回值：
		//	其他说明：	
		//===================================================================
		ELTE_INT32 eLTESdkClient::App_Login(bool bLogin)
		{
			if(bLogin)
			{
				ELTE_INT32 iRet = ELTE_SDK_Login(cUserName, cPassWord, cServerIP, cLocalIP, atoi(cSipPort));
				if(RET_SUCCESS != iRet)
				{
					return iRet;
				}
				else
				{
					TDDisplayMnager::Instance().SetResID(cUserName);
					return RET_SUCCESS;
				}
			}
			else
			{
				ELTE_INT32 iRet = ELTE_SDK_Logout(cUserName);
				if(RET_SUCCESS != iRet)
				{
					return iRet;
				}
				else
				{
					return RET_SUCCESS;
				}
			}
		}

		//===================================================================
		//	函数名称：	SDK_START
		//	功能描述：	初始化上下文，启动SDK 
		//	调用清单：
		//	被调清单：
		//	输入参数：	
		//	输出参数：  无
		//	  返回值：
		//	其他说明：	
		//==================================================================
		ELTE_INT32 eLTESdkClient::SDK_START ()
		{
			//获取当前工作目录
			wchar_t szCurrentDir[MAX_PATH] = {0};
			GetCurrentDirectory(MAX_PATH, szCurrentDir);
			//获取启动路径
			TCHAR szModuleFilePath[MAX_PATH];    
			GetModuleFileName(NULL, szModuleFilePath, MAX_PATH );

			string  strModuleFilePath = TCHAR2STRING(szModuleFilePath);
			strModuleFilePath = strModuleFilePath.substr(0,strModuleFilePath.find_last_of("\\"));

			TCHAR* szBasePath =CHAR2WCHAR(strModuleFilePath.c_str());

			TCHAR szSDKPath[MAX_PATH] = L"\\eLTeSDK\\";    
			TCHAR szAllPath[MAX_PATH];    
			_stprintf(szAllPath,_T("%s%s"),szBasePath, szSDKPath); 

			delete[] szBasePath;
			szBasePath=NULL;

			//设置DLL路径
			SetCurrentDirectory(szAllPath);
			SetCurrentDirectory(szCurrentDir);

			
			static bool InitSucc=false;
			if(InitSucc)
			{
				ELTE_SDK_Cleanup();
				Sleep(3000);
				InitSucc=false;
			}

			ELTE_INT32 iRet = ELTE_SDK_SetLogPath("eLTeSDK\\log");
			if(RET_SUCCESS != iRet)
			{
				return iRet;
			}
			
			iRet = ELTE_SDK_Init();
			if(RET_SUCCESS != iRet)
			{
				return iRet;
			}
			InitSucc=true;
			
			iRet = ELTE_SDK_SetEventCallBack(EventCallBackFunc, &TDDisplayMnager::Instance());
			if(RET_SUCCESS != iRet)
			{
				return iRet;
			}

			// 登录
			iRet = App_Login(true);
			if(RET_SUCCESS != iRet)
			{
				return iRet;
			}
			::ResetEvent(TDDisplayMnager::Instance().GetLoginEventHandle());
			iRet = (ELTE_INT32)::WaitForSingleObject(TDDisplayMnager::Instance().GetLoginEventHandle(), (ELTE_INT32)WAIT_OBJECT_TIME);
			if (RET_SUCCESS != iRet)
			{
				return iRet;
			}

			/*	iRet = ELTE_SDK_ProvisionManagerInit(cServerIP, cUserName);
			if(RET_SUCCESS != iRet)
			{
			return iRet;
			}*/
			return RET_SUCCESS;
		}

		//===================================================================
		//	函数名称：	TriggerStatusReport
		//	功能描述：	触发状态上报 
		//	调用清单：
		//	被调清单：
		//	输入参数：	True：开始状态上报、False：关闭状态上报
		//	输出参数：  无
		//	  返回值：
		//	其他说明：	
		//===================================================================
		ELTE_INT32 eLTESdkClient::TriggerStatusReport(bool bEnableStatusReport)
		{
			ELTE_INT32 iRet = ELTE_SDK_TriggerStatusReport(bEnableStatusReport);
			if(RET_SUCCESS != iRet)
			{
				return iRet;
			}
			return RET_SUCCESS;
		}

		//===================================================================
		//	函数名称：	SDK_STOP
		//	功能描述：	通知SDK停止处理业务 
		//	调用清单：
		//	被调清单：
		//	输入参数：	
		//	输出参数：  无
		//	  返回值：
		//	其他说明：	
		//===================================================================
		ELTE_INT32 eLTESdkClient:: SDK_STOP()
		{
			ELTE_INT32 iRet = ELTE_SDK_SetEventCallBack(NULL, NULL);
			if (RET_SUCCESS != iRet)
			{   
				return iRet;
			}

			iRet = ELTE_SDK_ProvisionManagerExit();
			if (RET_SUCCESS != iRet)
			{   
				return iRet;
			}

			iRet = App_Login(false);
			if (RET_SUCCESS != iRet)
			{   
				return iRet;
			}

			iRet = ELTE_SDK_Cleanup();
			if (RET_SUCCESS != iRet)
			{   
				return iRet;
			}
			return RET_SUCCESS;
		}

		//===================================================================
		//	函数名称：	StartP2PVideoMonitor
		//	功能描述：	调度台发起视频回传 
		//	调用清单：
		//	被调清单：  mobileid
		//	输入参数：	
		//	输出参数：  无
		//	  返回值：
		//	其他说明：	
		//===================================================================
		ELTE_INT32 eLTESdkClient:: StartP2PVideoMonitor(int mobileid,const eLTEVideoParameter^ videoPara)
		{
			//接口调用
			string strMobileId = Int2String(mobileid);
			string strVideoParam;
			strVideoParam.append("<Content><VideoParam><VideoFormat>");
			strVideoParam.append((char*)(void*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(videoPara->fmtvalue));
			strVideoParam.append("</VideoFormat>");
			strVideoParam.append("<CameraType>");
			strVideoParam.append((char*)(void*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(videoPara->cameratype));
			strVideoParam.append("</CameraType>");
			strVideoParam.append("<UserConfirmType>");
			strVideoParam.append((char*)(void*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(videoPara->user_confirm_type));
			strVideoParam.append("</UserConfirmType>");
			strVideoParam.append("<MuteType>");
			strVideoParam.append((char*)(void*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(videoPara->mutetype));
			strVideoParam.append("</MuteType>");
			strVideoParam.append("</VideoParam></Content>");

			ELTE_INT32 iRet = ELTE_SDK_StartRealPlay(strMobileId.c_str(), strVideoParam.c_str());
			if(RET_SUCCESS != iRet)
			{
				return iRet;
			}
			return RET_SUCCESS;
		}

		//===================================================================
		//	函数名称：	PTZControl
		//	功能描述：	开始云台控制 
		//	调用清单：
		//	被调清单：
		//	输入参数：	移动终端编码、云台控制命令、控制参数
		//	输出参数：  无
		//	  返回值：
		//	其他说明：	
		//===================================================================
		ELTE_INT32 eLTESdkClient:: PTZControl(int mobileid,unsigned int ptz_control_code,unsigned int ptz_control_value)
		{
			//接口调用
			string strMobileId = Int2String(mobileid);
			ELTE_INT32 iRet = ELTE_SDK_PTZControl(strMobileId.c_str(), ptz_control_code, ptz_control_value);
			if (RET_SUCCESS != iRet)
			{
				return iRet;
			}
			return RET_SUCCESS;
		}

		//===================================================================
		//	函数名称：	StopP2PVideo
		//	功能描述：	调度台挂断视频点呼或视频回传 
		//	调用清单：
		//	被调清单：
		//	输入参数：	
		//	输出参数：  无
		//	  返回值：
		//	其他说明：	
		//===================================================================
		ELTE_INT32 eLTESdkClient:: StopP2PVideo(int mobileid)
		{
			//接口调用
			string strMobileId = Int2String(mobileid);
			ELTE_INT32 iRet =  ELTE_SDK_StopRealPlay(strMobileId.c_str());
			if(RET_SUCCESS != iRet)
			{
				return iRet;
			}
			return RET_SUCCESS;
		}
	 }
 }