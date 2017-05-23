  include io.asm     ;подключение операций ввода-вывода

stack segment stack
      dw 128 dup (?)
stack ends

data  segment
; переменные и константы
data  ends

code segment
     assume ss:stack, ds:data, cs:code
; процедуры   
start:
    mov ax,data
    mov ds,ax
; команды программы
    finish
code ends
    end start 
