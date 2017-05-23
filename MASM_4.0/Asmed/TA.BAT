@echo off
set tasmpath=\bc4\bin
set tasmhost=t:
set temppath=c:\temp
set tdpath=t:\bc4\bin

%tasmhost%
cd %tasmpath%
if not exist tasm.exe goto fileserr
if not exist %1.asm goto fileserr
tasm %1.asm,%temppath%\temp.obj,%temppath%\temp.lst /zi
if errorlevel 1 goto errors
if not exist tlink.exe goto fileserr
tlink %temppath%\temp.obj+ioproc.obj,%temppath%\temp.exe /v
del %temppath%\temp.obj
if errorlevel 1 goto errors

cls
%tdpath%\td %temppath%\temp.exe
del %temppath%\temp.exe
goto fin

:errors
pause
goto fin
:fileserr
@echo Один из необходимых файлов не был найден. Обратитесь к администратору сети.
pause
:fin
