include io.asm

stack segment stack
  DB 138 dup(?)
stack ends   

data segment
   x DB 100 dup(?),'$'
   y DB 100 dup(?)
   z DB 100 dup(?)
   b DB 'VVEDITE TEXT NE BOLEE 100 SYMBOLOV!','$'
   a DB 'VVEDENNIY TEXT!!!','$'
   AT DB 'REZULTAT!','$'
   c DB '1','$'
   k DB '2','$'
data ends

code segment
assume ss:stack,ds:data,cs:code

replacement PROC
 push AX
 o:mov AL,[BX]
 CMP AL,'1'
 JB net;<'1'
 CMP AL,'9'
 JA net;>'9'
 SUB AL,'0'
 ADD AL,'a'
 DEC AL
 MOV AH,AL
 mov [BX],AH;zamena
 net: 
 INC BX
 LOOP o
 pop AX
 RET
replacement ENDP


change  PROC
push CX
push AX
push DI
push DX
mov DI,0;
mov DX,0;
xr:mov AL,[BX]
cmp AL,'1'
JB da
cmp AL,'9'
JA da
mov [SI],AL
INC SI
jmp KON
da:
mov z[DI],AL
INC DI
INC DX
KON:INC BX
LOOP xr
mov CX,DX
mov DI,0
ffg:mov AL,z[DI]
mov[SI],AL
INC SI
INC DI
LOOP ffg
pop DX
pop CX
pop DI
pop AX
 RET
change  ENDP

out PROC
PUSH AX
S:MOV AL,[BX]
OUTCH AL
INC BX
LOOP S
NEWLINE
POP AX
RET
out ENDP



start:   mov AX,data
	 mov DS,AX
    LEA DX,b
    OUTSTR 
    newline
    mov CX,100
    mov BX,0
    mov DX,0
    m:inch AL
    CMP AL,'.'
    JE h
    mov x[BX],AL
    INC BX
    INC DX
    LOOP m
    h:
    mov CX,DX
    LEA DX,a
    OUTSTR
    newline
    mov DX,CX
    MOV CX,DX
    LEA BX,x
    call out
mov CX,DX
dec CX
mov SI,CX
mov AL,x[SI]
    cmp AL,'A'
    JB t;<
    CMP AL,'Z'
    JA t;>
    mov CX,DX
    dec CX
    mov BX,0
    U:cmp AL,x[BX]
    JE t
    INC BX 
    LOOP U
    OUTCH c
    newline
    mov CX,DX
    LEA BX,x
    call replacement
    mov CX,DX
    LEA DX,AT
    OUTSTR
    newline
    mov DX,CX
    MOV CX,DX    
    LEA BX,x
    call out
    JMP en
t:OUTCH k
    newline
    mov CX,DX
    mov BX,offset x
    mov SI,offset y
 call change
    mov CX,DX
    LEA DX,AT
    OUTSTR
    newline
    mov DX,CX
    MOV CX,DX
    LEA BX,y
    call out
 en: FINISH
code ends

	END start
end  