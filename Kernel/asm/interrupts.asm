
GLOBAL _cli
GLOBAL _sti
GLOBAL picMasterMask
GLOBAL picSlaveMask
GLOBAL haltcpu
GLOBAL _hlt

GLOBAL _irq00Handler
GLOBAL _irq01Handler
GLOBAL _irq02Handler
GLOBAL _irq03Handler
GLOBAL _irq04Handler
GLOBAL _irq05Handler

GLOBAL _exception0Handler

GLOBAL excRegData
GLOBAL registerInfo
GLOBAL hasregisterInfo

EXTERN keyboard_handler

EXTERN dv_newLine

EXTERN irqDispatcher
EXTERN exceptionDispatcher

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

%macro saveRegistersException 0
	mov [excRegData+0], rax
	mov [excRegData+8], rbx
	mov [excRegData+16], rcx
	mov [excRegData+24], rdx
	mov [excRegData+32], rsi
	mov [excRegData+40], rdi
	mov [excRegData+48], rbp
	mov [excRegData+64], r8
	mov [excRegData+72], r9
	mov [excRegData+80], r10
	mov [excRegData+88], r11
	mov [excRegData+96], r12
	mov [excRegData+104], r13
	mov [excRegData+112], r14
	mov [excRegData+120], r15

	mov rax, rsp
	add rax, 40
	mov [excRegData+56], rax

	mov rax, [rsp]
	mov [excRegData+128], rax

	mov rax, [rsp+8]
	mov [excRegData+136], rax
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

%macro irqHandlerMaster 1
	pushState

	mov rdi, %1 ; pasaje de parametro
	call irqDispatcher

	; signal pic EOI (End of Interrupt)
	mov al, 20h
	out 20h, al

	popState
	iretq
%endmacro



%macro exceptionHandler 1
	pushState

	mov rdi, %1 ; pasaje de parametro
	call exceptionDispatcher

	popState
	iretq
%endmacro






;8254 Timer (Timer Tick)
_irq00Handler:
	irqHandlerMaster 0

;Keyboard
_irq01Handler:
	interrupt_keyboardHandler:
	pushState

    xor rax, rax
    in al, 60h
    mov rdi, rax

    cmp al, 0x2A  ; Check if left shift is pressed
    jne .check_left_shift_release
    mov byte [shiftKey], 1

.check_left_shift_release:
    cmp al, 0xAA  ; Check if left shift is released
    jne .check_shift_s_pressed
    mov byte [shiftKey], 0

.check_shift_s_pressed:
    cmp byte [shiftKey], 1
    jne .handle_keyboard_interrupt
    cmp al, 0x1F  ; Check if 's' key is pressed
    jne .handle_keyboard_interrupt

    ; Save register values
    mov [registerInfo+2*8], rbx
    mov [registerInfo+3*8], rcx
    mov [registerInfo+4*8], rdx
    mov [registerInfo+5*8], rsi
    mov [registerInfo+6*8], rdi
    mov [registerInfo+7*8], rbp
    mov [registerInfo+9*8], r8
    mov [registerInfo+10*8], r9
    mov [registerInfo+11*8], r10
    mov [registerInfo+12*8], r11
    mov [registerInfo+13*8], r12
    mov [registerInfo+14*8], r13
    mov [registerInfo+15*8], r14
    mov [registerInfo+16*8], r15

    mov rax, rsp
    add rax, 160
    mov [registerInfo+8*8], rax

    mov rax, [rsp+15*8]
    mov [registerInfo], rax
    
    mov rax, [rsp+14*8]
    mov [registerInfo+1*8], rax

    mov byte [hasregisterInfo], 1

.handle_keyboard_interrupt:
    call keyboard_handler

    endOfHardwareInterrupt
    popState
    iretq

;Cascade pic never called
_irq02Handler:
	irqHandlerMaster 2

;Serial Port 2 and 4
_irq03Handler:
	irqHandlerMaster 3

;Serial Port 1 and 3
_irq04Handler:
	irqHandlerMaster 4

;USB
_irq05Handler:
	irqHandlerMaster 5


;Zero Division Exception
_exception0Handler:
	exceptionHandler 0

haltcpu:
	cli
	hlt
	ret



SECTION .bss
	aux resq 1
	excRegData		resq	18
	registerInfo	resq	17
	hasregisterInfo 		resb 1
	shiftKey  	resb 1