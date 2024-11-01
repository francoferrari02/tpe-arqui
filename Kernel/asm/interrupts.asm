GLOBAL _cli
GLOBAL _sti
GLOBAL picMasterMask
GLOBAL picSlaveMask
GLOBAL haltcpu
GLOBAL _hlt

GLOBAL _irq00Handler
GLOBAL _irq01Handler


GLOBAL exception_divideByZero
GLOBAL exception_invalidOpCode


GLOBAL interrupt_syscall

GLOBAL exception_register_dump
GLOBAL register_info
GLOBAL has_register_info

EXTERN timer_handler
EXTERN keyboard_handler
EXTERN syscall_handler
EXTERN exception_handler
EXTERN dv_newline

SECTION .text

%macro pushState 0
	push rax
	push rbx
	push rcx
	push rdx
	push rbp
	push rdi
	push rsi
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15
%endmacro

%macro popState 0
	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop rsi
	pop rdi
	pop rbp
	pop rdx
	pop rcx
	pop rbx
	pop rax
%endmacro

%macro endOfHardwareInterrupt 0 
    mov al, 20h
    out 20h, al
%endmacro


%macro saveRegistersException 0 ; Guarda los registros en exception_register_dump
	mov [exception_register_dump+0], rax ;0
	mov [exception_register_dump+8], rbx ;1
	mov [exception_register_dump+16], rcx ;2
	mov [exception_register_dump+24], rdx ;3
	mov [exception_register_dump+32], rsi ;4
	mov [exception_register_dump+40], rdi ;5
	mov [exception_register_dump+48], rbp ;6
	mov [exception_register_dump+64], r8  ;8
	mov [exception_register_dump+72], r9  ;9
	mov [exception_register_dump+80], r10 ;10
	mov [exception_register_dump+88], r11	;11
	mov [exception_register_dump+96], r12 ;12
	mov [exception_register_dump+104], r13 ;13
	mov [exception_register_dump+112], r14 ;14
	mov [exception_register_dump+120], r15 ;15

	mov rax, rsp 
	add rax, 40
	mov [exception_register_dump+56], rax ;7

	mov rax, [rsp] 
	mov [exception_register_dump+128], rax ;16

	mov rax, [rsp+8] 
	mov [exception_register_dump+136], rax ;17
%endmacro

_hlt:
	sti
	hlt
	ret

_cli:
	cli
	ret


_sti:
	sti
	ret

picMasterMask:
	push rbp
    mov rbp, rsp
    mov ax, di
    out	21h,al
    pop rbp
    retn

picSlaveMask:
	push    rbp
    mov     rbp, rsp
    mov     ax, di  ; ax = mascara de 16 bits
    out	0A1h,al
    pop     rbp
    retn




;Interrupciones
_irq00Handler: ; Timer tick
	pushState

	call timer_handler

	endOfHardwareInterrupt
	popState
	iretq



_irq01Handler: ; Keyboard
	pushState
	
	xor rax, rax
	in al, 60h 		; 60 es el puerto del teclado AL : 8 bits
	mov rdi, rax 	; recepcion del primer parametro

	cmp al, 0x2A 	;left shift pressed
	jne .continue1
	mov byte [left_shift], 1

.continue1: 
	cmp al, 0xAA 	
	jne .continue2
	mov byte [left_shift], 0

	;si estan apretados SHIFT+S se copian los registros en el vector regdataDump
	;mover RSP a donde estaba antes de ser llamada la excepcion
.continue2: 
	cmp byte [left_shift], 1 	; 's' pressed
	jne .continue3
	cmp al, 0x1F
	jne .continue3

	mov [register_info+2*8], rbx 
	mov [register_info+3*8], rcx
	mov [register_info+4*8], rdx
	mov [register_info+5*8], rsi
	mov [register_info+6*8], rdi
	mov [register_info+7*8], rbp
	mov [register_info+9*8], r8
	mov [register_info+10*8], r9
	mov [register_info+11*8], r10
	mov [register_info+12*8], r11
	mov [register_info+13*8], r12
	mov [register_info+14*8], r13
	mov [register_info+15*8], r14
	mov [register_info+16*8], r15

	mov rax, rsp
	add rax, 160 ;120 del popstate 
	mov [register_info+8*8], rax ;RSP

	mov rax, [rsp+15*8]
	mov [register_info], rax ;RIP
	
	mov rax, [rsp+14*8]
	mov [register_info+1*8], rax ;RAX

	mov byte [has_register_info], 1

.continue3:
    call keyboard_handler

    endOfHardwareInterrupt
    popState
	iretq




;Excepciones
exception_divideByZero:
	saveRegistersException

	mov rdi, 00h
	mov rsi, exception_register_dump
	call exception_handler

	iret


exception_invalidOpCode:
	saveRegistersException

	mov rdi, 06h
	mov rsi, exception_register_dump
	call exception_handler



;Syscall

; syscalls params:	RDI	RSI	RDX	R10	R8	R9
; C 	params   :	RDI RSI RDX RCX R8  R9
interrupt_syscall:
	mov rcx, r10
	mov r9, rax
	call syscall_handler
	iretq




haltcpu:
	cli
	hlt
	ret



SECTION .bss
	aux resq 1
	exception_register_dump resq 18	; reserva 18 bytes para guardar los registros para excepciones
	register_info	resq 17	; reserve space for 17 qwords (one for each register we want to show on inforeg).
	has_register_info resb 1 	; reserve 1 byte for a boolean on whether a regdump has already occurred.
	left_shift resb 1   ; shift presionado