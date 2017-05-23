@echo off
cls
@echo Подождите, пока идет подготовка к началу работы.
set AsmedPath=T:\masm\Admed
c:
cd \temp
if exist asmed goto folderex
md asmed
:folderex
cd asmed
copy %asmedpath%\*.* c:\Temp > null
@echo Подготовка завешена.
start asm_ed.exe
