echo －－－－－ 开始编译SIP －－－－－－－－－－－－－－－－

cd ..\SipSDK
call .\一键打包.bat

echo －－－－－ 编译SIP结束后进行拷贝DLL －－－－－－－－－－－－－－－－

cd ..\Cgw

xcopy /y /i /r /s "..\..\self_dev\eSDK_eLTE_SDK\release\eLTE_SDK\*"          "Cgw\Build\x86\Release\MonitorManageService\eLTeSDK" 

xcopy /y /i /r /s "..\..\self_dev\eSDK_eLTE_SDK\debug\eLTE_NativeService\*"          "Cgw\Build\x86\Release\MonitorManageService\eLTeSDK\eLTE_NativeService"

xcopy /y /i /r /s "..\..\platform\CGW\TiandySdk\*"           "Cgw\Build\x86\Release\MonitorManageService\TiandySdk"  

xcopy /y /i /r /s "..\..\platform\CGW\CGWDLL\*"           "Cgw\Build\x86\Release\MonitorManageService\IVSSDK"

xcopy /y /i /r /s "..\..\platform\CGW\CGWDLL\*"           "Cgw\Build\x86\Release"

xcopy /y /i /r /s "..\..\platform\CGW\SoftTerminal\Lib\Release\*"          "Cgw\Build\x86\Release"

xcopy /y /i /r /s "..\..\platform\CGW\SoftTerminal"          "Cgw\Cgw.SoftTerminal"

xcopy /y /i /r /s "..\..\platform\CGW\ExternalRef\*"           "Cgw\Build\x86\Release"

xcopy /y /i /r /s "..\..\platform\CGW\TiandySdk\*"           "CgwMonitorManageService\Cgw.Tiandy\TiandySdk"

xcopy /y /i /r /s "..\..\open_src\NLog.dll"      "Cgw\Build\x86\Release\MonitorManageService"

xcopy /y /i /r /s "..\..\open_src\NLog.dll"      "Cgw\Build\x86\Release\MonitorManageService\IVSSDK"

xcopy /y /i /r /s "..\..\open_src\NLog.dll"      "Cgw\Build\x86\Release"

xcopy /y /i /r /s "..\..\self_dev\Cgw\eSDK_IVS_API_V1.5.00_Windows\bin\*"            "Cgw\Build\x86\Release\MonitorManageService\IVSSDK"

xcopy /y /i /r /s "..\..\self_dev\Cgw\eSDK_IVS_API_V1.5.00_Windows\include\*"            "CgwMonitorManageService\Cgw.Ivs.SdkClient\Include"

xcopy /y /i /r /s "..\..\self_dev\Cgw\eSDK_IVS_API_V1.5.00_Windows\bin"            "CgwMonitorManageService\Cgw.Ivs.SdkClient\Lib"

xcopy /y /i /r /s "..\..\self_dev\Cgw\eSDK_IVS_API_V1.5.00_Windows\sdklib\*"            "CgwMonitorManageService\Cgw.Ivs.SdkClient\Lib"

xcopy /y /i /r /s "..\..\self_dev\Cgw\eSDK_IVS_API_V1.5.00_Windows\bin\config"            "CgwMonitorManageService\Cgw.Ivs\config"

xcopy /y /i /r /s "..\..\self_dev\Cgw\eSDK_IVS_API_V1.5.00_Windows\bin\defaultConfig"            "CgwMonitorManageService\Cgw.Ivs\defaultConfig"

xcopy /y /i /r /s "..\..\self_dev\Cgw\eSDK_IVS_API_V1.5.00_Windows\bin\lib"            "CgwMonitorManageService\Cgw.Ivs\lib"

xcopy /y /i /r /s "..\SipSDK\version\release\*"           "Cgw\Build\x86\Release\MonitorManageService\SipStack"

xcopy /y /i /r /s "..\..\open_src\log4cpp-1.1.1\log4cpp\msvc10\log4cpp\Release\log4cpp.dll"    "Cgw\Build\x86\Release\MonitorManageService\SipStack"

xcopy /y /i /r /s "..\..\self_dev\Cgw\eSDK_IVS_API_V1.5.00_Windows\bin\ACE.dll"   "Cgw\Build\x86\Release\MonitorManageService\SipStack"

xcopy /y /i /r /s "..\..\self_dev\Cgw\eSDK_IVS_API_V1.5.00_Windows\bin\bp_base.dll"   "Cgw\Build\x86\Release\MonitorManageService\SipStack"

xcopy /y /i /r /s "..\..\self_dev\Cgw\eSDK_IVS_API_V1.5.00_Windows\bin\IVS_NetSource.dll"   "Cgw\Build\x86\Release\MonitorManageService\SipStack"

xcopy /y /i /r /s "..\..\self_dev\Cgw\eSDK_IVS_API_V1.5.00_Windows\bin\bp_log.dll"   "Cgw\Build\x86\Release\MonitorManageService\SipStack"

xcopy /y /i /r /s "..\..\self_dev\Cgw\eSDK_IVS_API_V1.5.00_Windows\bin\ivs_security.dll"   "Cgw\Build\x86\Release\MonitorManageService\SipStack"

xcopy /y /i /r /s "..\..\self_dev\Cgw\eSDK_IVS_API_V1.5.00_Windows\bin\ivs_base.dll"   "Cgw\Build\x86\Release\MonitorManageService\SipStack"

xcopy /y /i /r /s "..\..\self_dev\Cgw\eSDK_IVS_API_V1.5.00_Windows\bin\ivs_compression.dll"   "Cgw\Build\x86\Release\MonitorManageService\SipStack"

xcopy /y /i /r /s "..\..\self_dev\Cgw\eSDK_IVS_API_V1.5.00_Windows\bin\ivs_sftp.dll"   "Cgw\Build\x86\Release\MonitorManageService\SipStack"

xcopy /y /i /r /s "..\..\self_dev\Cgw\eSDK_IVS_API_V1.5.00_Windows\bin\libeay32.dll"   "Cgw\Build\x86\Release\MonitorManageService\SipStack"

xcopy /y /i /r /s "..\..\self_dev\Cgw\eSDK_IVS_API_V1.5.00_Windows\bin\libmpg123.dll"   "Cgw\Build\x86\Release\MonitorManageService\SipStack"

xcopy /y /i /r /s "..\..\self_dev\Cgw\eSDK_IVS_API_V1.5.00_Windows\bin\libssh2.dll"   "Cgw\Build\x86\Release\MonitorManageService\SipStack"

xcopy /y /i /r /s "..\..\self_dev\Cgw\eSDK_IVS_API_V1.5.00_Windows\bin\zlib.dll"   "Cgw\Build\x86\Release\MonitorManageService\SipStack"

xcopy /y /i /r /s "..\SipSDK\version\release\*"           "CgwMonitorManageService\Cgw.T28181\SipStack"
pause