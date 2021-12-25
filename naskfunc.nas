[FORMAT "WCOFF"]				
[INSTRSET "i486p"]
[BITS 32]				
[FILE "naskfunc.nas"]		
GLOBAL	_io_in
GLOBAL _io_out
		GLOBAL	_io_hlt, _io_cli, _io_sti, _io_stihlt
		GLOBAL	_io_in8,  _io_in16,  _io_in32
		GLOBAL	_io_out8, _io_out16, _io_out32
		GLOBAL	_io_load_eflags, _io_store_eflags
		GLOBAL	_load_gdtr, _load_idtr
		GLOBAL _RAND 
		GLOBAL _get_cpu
GLOBAL _ASM_call
GLOBAL _asm_inthandler20
EXTERN _inthandler20
EXTERN _inthandler40
GLOBAL _asm_inthandler40
GLOBAL _asm_inthandler21
GLOBAL _get_eax
GLOBAL _get_ebx
GLOBAL _get_ecx
GLOBAL _get_edx
GLOBAL _get_edi
GLOBAL _get_esi
GLOBAL _get_cs
GLOBAL _get_ebp
GLOBAL _get_eip
GLOBAL _get_esp
GLOBAL _pusha
GLOBAL _popa
GLOBAL _asm_func
GLOBAL _farjmp,_start_app
GLOBAL _load_cr0
EXTERN _inthandler21,_getch,_input_char_inSM
		GLOBAL	 _store_cr0
		GLOBAL	_load_tr,_memtest_sub,_farcall,_system_api_getch,_intreturn
[SECTION .text]
return_num: DD 0
_intreturn: 
	mov edx,[esp+4]
	mov [return_num],edx
	ret
_farcall:		; void farcall(int eip, int cs);
		CALL		FAR [ESP+4]     ; eip, cs
		RET
_memtest_sub:	; unsigned int memtest_sub(unsigned int start, unsigned int end)
		PUSH	EDI						; （由于还要使用EBX, ESI, EDI）
		PUSH	ESI
		PUSH	EBX
		MOV		ESI,0xaa55aa55			; pat0 = 0xaa55aa55;
		MOV		EDI,0x55aa55aa			; pat1 = 0x55aa55aa;
		MOV		EAX,[ESP+12+4]			; i = start;
mts_loop:
		MOV		EBX,EAX
		ADD		EBX,0xffc				; p = i + 0xffc;
		MOV		EDX,[EBX]				; old = *p;
		MOV		[EBX],ESI				; *p = pat0;
		XOR		DWORD [EBX],0xffffffff	; *p ^= 0xffffffff;
		CMP		EDI,[EBX]				; if (*p != pat1) goto fin;
		JNE		mts_fin
		XOR		DWORD [EBX],0xffffffff	; *p ^= 0xffffffff;
		CMP		ESI,[EBX]				; if (*p != pat0) goto fin;
		JNE		mts_fin
		MOV		[EBX],EDX				; *p = old;
		ADD		EAX,0x1000				; i += 0x1000;
		CMP		EAX,[ESP+12+8]			; if (i <= end) goto mts_loop;
		JBE		mts_loop
		POP		EBX
		POP		ESI
		POP		EDI
		RET
mts_fin:
		MOV		[EBX],EDX				; *p = old;
		POP		EBX
		POP		ESI
		POP		EDI
		RET

_load_cr0:		; int load_cr0(void);
		MOV		EAX,CR0
		RET

_store_cr0:		; void store_cr0(int cr0);
		MOV		EAX,[ESP+4]
		MOV		CR0,EAX
		RET

_load_tr:		; void load_tr(int tr);
		LTR		[ESP+4]			; tr
		RET
_system_api_getch:
	call _getch
	ret
_start_app:		; void start_app(int eip, int cs, int esp, int ds, int *tss_esp0);
		MOV EDI,ESP
		PUSHAD								; 将32位寄存器的值全部保存起来
		call FAR [edi+4]
		POPAD
		RET
; 应用程序结束后不会回到这里
_asm_func:
	ret
_farjmp:		; void farjmp(int eip, int cs);
		JMP    FAR [ESP+4]    ; eip, cs
		RET
_asm_inthandler40:
		STI
		PUSHAD	
		PUSHAD	
		CALL	_inthandler40
		ADD		ESP,32
		POPAD
		mov edx,[return_num]
		IRETD
_RAND:
MOV AX, 32767   ;产生从1到AX之间的随机数
MOV DX, 41H ;用端口41H，或者上面说的其他端口也行
OUT DX, AX  ;有这句话后，我发现就可以产生从1到AX之间的随机数了
IN AL, DX   ;产生的随机数AL
ret
_get_eax:
	ret
_get_ebx:
	mov eax,ebx
	ret
_get_ecx:
	mov eax,ecx
	ret
_get_edx:
	mov eax,edx
	ret
_get_esi:
	mov eax,esi
	ret
_get_edi:
	mov eax,edi
	ret
_get_cs:
	mov ax,cs
	ret
_get_esp:
	mov eax,esp
	ret
_get_ebp:
	mov eax,ebp
	ret
_get_eip:
	mov eax,$
	ret
_io_in:	; void _asmfunc(int addr, char data,int color);
		MOV		ECX,[ESP+4]		
		mov edx,ecx
		in ax,dx
		bswap eax
		xor ax,ax
		bswap eax
		mov ah,0
		RET
_io_out:	; void _asmfunc(int addr, char data,int color);
		MOV		ECX,[ESP+4]		
		mov edx,ecx
		mov ax,dx
		out 0x20,al
		RET
_io_hlt:
	hlt
	ret
_ASM_call:
mov dx,03d4h;03d4h是索引端口
mov al,0eh;内部的0eh位置存放着光标位置的高八位
out dx,al
inc dx;03d5h是数据端口用于读写数据
in al,dx;读取光标的高八位并且放入bh
mov bh,al
 
dec dx;这儿开始读取光标位置的低八位放入bl
mov al,0fh;0fh位置存放着光标位置的低八位
out  dx,al
inc dx
in al,dx
mov bl,al
 

mov word bx,[esp+4]   ;获取参数中的光标位置
 
mov  dx,03d4h;这段代码将改变后的光标位置写入端口内相应的地方以便下次访问
mov al,0eh;写入光标位置高八位
out dx,al
inc dx
mov al,bh
out dx,al
 
dec dx
mov al,0fh    ;写入光标位置低八位
out dx,al
inc dx
mov al,bl
out dx,al
ret
_io_cli:	; void io_cli(void);
		CLI
		RET

_io_sti:	; void io_sti(void);
		STI
		RET

_io_stihlt:	; void io_stihlt(void);
		STI
		HLT
		RET

_io_in8:	; int io_in8(int port);
		MOV		EDX,[ESP+4]		; port
		MOV		EAX,0
		IN		AL,DX
		RET

_io_in16:	; int io_in16(int port);
		MOV		EDX,[ESP+4]		; port
		MOV		EAX,0
		IN		AX,DX
		RET

_io_in32:	; int io_in32(int port);
		MOV		EDX,[ESP+4]		; port
		IN		EAX,DX
		RET

_io_out8:	; void io_out8(int port, int data);
		MOV		EDX,[ESP+4]		; port
		MOV		AL,[ESP+8]		; data
		OUT		DX,AL
		RET

_io_out16:	; void io_out16(int port, int data);
		MOV		EDX,[ESP+4]		; port
		MOV		EAX,[ESP+8]		; data
		OUT		DX,AX
		RET

_io_out32:	; void io_out32(int port, int data);
		MOV		EDX,[ESP+4]		; port
		MOV		EAX,[ESP+8]		; data
		OUT		DX,EAX
		RET

_io_load_eflags:	; int io_load_eflags(void);
		PUSHFD		; PUSH EFLAGS 
		POP		EAX
		RET

_io_store_eflags:	; void io_store_eflags(int eflags);
		MOV		EAX,[ESP+4]
		PUSH	EAX
		POPFD		; POP EFLAGS 
		RET

_load_gdtr:		; void load_gdtr(int limit, int addr);
		MOV		AX,[ESP+4]		; limit
		MOV		[ESP+6],AX
		LGDT	[ESP+6]
		RET

_load_idtr:		; void load_idtr(int limit, int addr);
		MOV		AX,[ESP+4]		; limit
		MOV		[ESP+6],AX
		LIDT	[ESP+6]
		RET
_asm_inthandler20:
        PUSH    ES
        PUSH    DS
        PUSHAD
        MOV        EAX,ESP
        PUSH    EAX
        MOV        AX,SS
        MOV        DS,AX
        MOV        ES,AX
        CALL    _inthandler20 ; 这里会调用void inthandler20(int *esp);函数
        POP        EAX
        POPAD
        POP        DS
        POP        ES
        IRETD
_asm_inthandler21:
        PUSH    ES
        PUSH    DS
        PUSHAD
        MOV        EAX,ESP
        PUSH    EAX
        MOV        AX,SS
        MOV        DS,AX
        MOV        ES,AX
        CALL    _inthandler21 ; 这里会调用void inthandler20(int *esp);函数
        POP        EAX
        POPAD
        POP        DS
        POP        ES
        IRETD

_get_cpu:
        mov eax, 0
        cpuid
	ret
_pusha:
pusha
	ret
_popa:
popa
	ret