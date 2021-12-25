[BITS 32]
jmp start
str: db 'hello',0
start:
mov eax,0x09
mov ebx,str
int 0x40

RETF   