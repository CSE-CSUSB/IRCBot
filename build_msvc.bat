@echo off
if "%VCINSTALLDIR%"=="" (
echo Please run this script from a Visual Studio developer command prompt.
pause
goto :eof
)
@echo on
cl /EHsc /W4 main.cc irc.cc /link Ws2_32.lib

