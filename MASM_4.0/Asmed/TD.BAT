@echo off
set masmpath=\masm
set masmhost=t:
set temppath=c:\temp
set tdpath=t:\bc4\bin

%masmhost%
cd %masmpath%
if not exist %1.asm goto fileserr
if not exist masm.exe goto fileserr
masm %1.asm,%temppath%\temp.obj,%temppath%\temp.lst /ic:;
if errorlevel 1 goto errors
if not exist link.exe goto fileserr
link %temppath%\temp.obj+ioproc.obj,%temppath%\temp.exe;
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
