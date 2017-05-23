include io.asm

stack segment stack
  DB 138 dup(?)
stack ends   

data segment
   x DB 100 dup(?),'$'
   y DB 100 dup(?),'$'
   b DB 'VVEDITE TEXT NE BOLEE 100 SYMBOLOV!','$'
   a DB 'VVEDENNIY TEXT!!!','$'
data ends

code segment
assume ss:stack,ds:data,cs:code


out PROC
push AX
push DX
LEA DX,a
OUTSTR
newline
q:mov AL,[BX]
outch AL
INC BX
LOOP q
pop DX
pop AX
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
    h:MOV x[BX],'$'
    DEC DX
    mov CX,DX
    mov BX,offset x
    call out
FINISH
code ends
END start
end