@echo off
cls
@echo ��������, ���� ���� �����⮢�� � ��砫� ࠡ���.
set AsmedPath=T:\masm\Admed
c:
cd \temp
if exist asmed goto folderex
md asmed
:folderex
cd asmed
copy %asmedpath%\*.* c:\Temp > null
@echo �����⮢�� ����襭�.
start asm_ed.exe
