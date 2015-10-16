@echo off

set filepath = "%VS100COMNTOOLS%..\..\Common7\IDE"

cd /d %filepath%

rem _buildMode can be "Release|Win32" or "Debug|Win32"
start devenv.exe "..\source\OutlookPlugin\TPConfAddin\TPConfAddin.sln" /Rebuild release /out "log.txt"

echo d|xcopy "..\self_dev\VersionUpdate" "..\source\OutlookPlugin\TPConfAddin\bin\Release\VersionUpdate" /e

exit