::设置各变量名
set	SVNROOT=%svn_path%
cd "D:\R2_File\eSpace_IVS_V100R002C01\IVS_COMMON"

@echo on
@echo . /noprogressui /nodlg /noui 
@echo －－－－－－－－－－－－－－开始清理目录－－－－－－－－－－－－－－－
"C:\Program Files\TortoiseSVN\bin\TortoiseProc.exe" /command:cleanup /path:"D:\R2_File\eSpace_IVS_V100R002C01\IVS_COMMON" /noui /noprogressui /nodlg 
@echo －－－－－－－－－－－－－－清理完成－－－－－－－－－－－－－－－
@echo . 
@echo －－－－－－－－－－－－－－开始更新－－－－－－－－－－－－－－－
"C:\Program Files\TortoiseSVN\bin\TortoiseProc.exe" /command:update /path:"D:\R2_File\eSpace_IVS_V100R002C01\IVS_COMMON" /closeonend:2  
@echo －－－－－－－－－－－－－－更新完成－－－－－－－－－－－－－－－