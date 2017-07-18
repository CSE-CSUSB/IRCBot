@echo off
if "%VCINSTALLDIR%"=="" (
echo Please run this script from a Visual Studio developer command prompt.
pause
goto :eof
)
@echo on
cl /I socketstream main.cc irc_bot.cc irc_message.cc /link Ws2_32.lib

