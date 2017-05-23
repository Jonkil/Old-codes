@echo off
C:\E\MSU\PRACT\ASM\MASM_4.0\masm %1.asm,%1.obj,%1.lst /iC:\WebDEN\MSU\Asembler\MASM_4.0;
if errorlevel 1 goto fin
C:\E\MSU\PRACT\ASM\MASM_4.0\link %1.obj+E:\MSU\PRACT\ASM\MASM_4.0\ioproc.obj,%1.exe;
if errorlevel 1 goto fin
del %1.obj
%1.exe
:fin
