include io.asm

stack segment stack
  DB 138 dup(?)
stack ends   

data segment
   x DB 100 dup(?),'$'
   y DB 100 dup(?),'$'
   b DB 'VVEDITE TEXT NE BOLEE 100 SYMBOLOV!','$'
   a DB 'VVEDENNIY TEXT!!!','$'
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


delete PROC
 push AX
 mov AL,[BX]
 mov [SI],AL
 INC SI
 f:INC BX
 CMP [BX],AL
 JE ravno;
 mov AH,[BX]
 mov [SI],AH
 INC SI 
 ravno:LOOP f
 pop AX
 RET
delete ENDP

out PROC
PUSH AX
push DX
LEA DX,a
OUTSTR
newline
S:MOV AL,[BX]
OUTCH AL
INC BX
LOOP S
NEWLINE
pop DX
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
    h:MOV CX,DX
    LEA BX,x
    call out
    mov CX,DX
    mov BX,0
    r:mov AL,x[BX]
    cmp AL,'a'
    JB t;<
    CMP AL,'z'
    JA t;>
    JMP u

    t:CMP AL,'A' 
    JB n;<
    cmp AL,'Z'
    JA n;>
    JMP u

    n:CMP AL,'0'
    JB g
    CMP AL,'9'
    JA g
    u:INC BX
    LOOP r
    OUTCH c
    newline
    mov CX,DX
    LEA BX,x
    call replacement
    MOV CX,DX    
    LEA BX,x
    call out
    JMP en

    g:OUTCH k
    newline
    mov CX,DX
    mov BX,offset x
    mov SI,offset y
    call delete
    MOV CX,DX
    LEA BX,y
    call out
    