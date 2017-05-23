@echo off
set masmpath=\masm
set masmhost=t:
set temppath=c:\temp
%masmhost%
cd %masmpath%
if not exist %1.asm goto fileserr
if not exist masm.exe goto fileserr
masm %1.asm,%temppath%\temp.obj,%temppath%\temp.lst /ic:;
if errorlevel 1 goto fin
if not exist link.exe goto fileserr
link %temppath%\temp.obj+ioproc.obj,%temppath%\temp.exe;
if errorlevel 1 goto fin
del %temppath%\temp.obj
cls
%temppath%\temp.exe
del %temppath%\temp.exe
goto fin
:fileserr
@echo Один из необходимых файлов не был найден. Обратитесь к администратору сети.
:fin
pause
