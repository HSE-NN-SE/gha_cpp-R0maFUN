
; You may customize this and other start-up templates; 
; The location of this template is c:\emu8086\inc\0_com_template.txt

CSEG segment
assume cs:CSEG, ds:CSEG, es:CSEG, ss:CSEG
org 100h


start:
     
     mov dx, offset fileName
     call OpenFile
     
     mov dx, offset fileNameOut
     mov bx, offset fileOutH
     call CreateFile
     
     xor dx,dx
     xor cx,cx
     
     mov bx,fileHandle
     lea di, buffer
     
    push dx
    push cx
    mov ax, 4202h
    int 21h
    mov AX1, ax
    mov DX1, dx
    ;jb EOF
    pop cx
    pop dx
     
while:
push dx
push cx
    mov ax, 4200h
    int 21h
   pop cx
   pop dx
    cmp dx,AX1
    jne ok2
    cmp cx,DX1
    je EOF
ok2:

    
    call ReadSymbol
    mov al,symbol
    mov [di], al
    inc bufSize
    
    cmp symbol, ' '
    je clrBuf
    
    cmp symbol, 10
    je clrBuf
    
    cmp symbol, '@'
    je compBuf
    ;call ReadString
    ;call OperateString
tailW:    
    inc di
    inc dx
    ;add dx,128
    ;cmp dx, 0FF80h
    cmp dx, 0FFFFh
    je IncCx
    ;jge IncCx
    
    jmp while
IncCx:
    inc cx
    sub dx,0FF80h
    jmp while
clrBuf:
    cmp flag,1
    jne Cont
    push dx
    push cx
    mov [di], 10
    mov [di+1], 13
    mov [di+2], '$'
    
    mov ah, 9
    lea dx, buffer
    int 21h
    
    mov ah,40h
    mov bx, fileOutH
    inc bufSize
    mov cx, bufSize
    int 21h
    
    pop cx
    pop dx
    mov flag,0
    
Cont:
    mov di, offset buffer-1
    mov bufSize, 0
    jmp tailW
compBuf:
    mov flag,1
    jmp tailW
    ;mov [di], symbol
;    inc di
;    inc dx
;    cmp dx, 0FFFFh
;    je IncCx
    
EOF:


ret

; ===== Procedures ======

OpenFile proc
   cmp fileHandle,0FFFFh
   jne Quit_open
   mov ax,3D00h
   int 21h
   mov fileHandle,ax
   ret 
Quit_open:
   stc
   ret     
   fileHandle dw 0FFFFh 
   
OpenFile endp

CreateFile proc
       mov AH, 3Ch
       mov CX, 0
       int 21h
       mov WORD PTR [BX], AX
    ret
CreateFile endp

ReadSymbol proc
    push cx
    push dx
    
    mov ah, 3Fh
    mov bx, fileHandle
    mov dx, offset symbol
    mov cx, 1
    
    int 21h
    
    pop dx
    pop cx
   ret
ReadSymbol endp

;ReadString proc
;    push cx
;    push dx
;    
;    mov ah, 3Fh
;    mov bx, fileHandle
;    mov dx, offset buffer
;    mov cx, 128
;    
;    int 21h
;    
;    mov buffer[129], '$'
;    
;    ;cmp ax, 0
;    ;je EOF
;    
;    pop dx
;    pop cx
;   ret
;ReadString endp

;OperateString proc
;    push cx
;    push dx
;    
;    lea si, buffer
; mov cx,0   
; for:
;    cmp [si], '@'
;    je Mail
;    inc si
;    inc cx
;    cmp si, si+128
;    jle for
;Mail:
;    push si
;    
; PrevChar:
;    
;    
;    
;    pop si
;    
;    cmp si, si+128
;    jle for
;        
;    pop dx
;    pop cx
;   ret
;OperateString endp

; ======== DATA =========

amount dd 0
AX1 dw ?
DX1 dw ?
fileNameOut db 'parsed.txt',0
fileOutH dw ?
fileName db 'data1.txt',0
symbol db ' '
flag db 0
bufSize dw 0
buffer equ $

CSEG ends
end start



