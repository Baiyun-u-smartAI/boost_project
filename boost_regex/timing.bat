if "%1"=="hide" goto CmdBegin
start mshta vbscript:createobject("wscript.shell").run("""%~0"" hide",0)(window.close)&&exit
:CmdBegin
@echo off
REM @choice /t 60 /d y /n >nul

:run
@choice /t 2400 /d y /n >nul
start warning.bat
goto run