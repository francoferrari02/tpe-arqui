GLOBAL cpuVendor
GLOBAL bd_to_binary
GLOBAL secondsGetter
GLOBAL hoursGetter
GLOBAL minutesGetter
GLOBAL speakerOut
GLOBAL speakerIn
section .text
	

cpuVendor:
	push rbp
	mov rbp, rsp

	push rbx

	mov rax, 0
	cpuid


	mov [rdi], ebx
	mov [rdi + 4], edx
	mov [rdi + 8], ecx

	mov byte [rdi+13], 0

	mov rax, rdi

	pop rbx

	mov rsp, rbp
	pop rbp
	ret

	bd_to_binary:
	push rbp
	mov rbp, rsp

	mov al, 0Bh 
	out 70h, al  
	in al, 71h
	
	mov bl, 4h
	or al, bl
	
	mov bl, al
	mov al, 0Bh
	
	out 70h, al
	
	mov al, bl
	
	out 71h, al

	mov rsp, rbp
	pop rbp
	ret


	
secondsGetter:
	push rbp
	mov rbp, rsp

	call bd_to_binary


    mov al, 0x00
    out 70h, al
    in al, 71h

	mov rsp, rbp
	pop rbp
    ret

minutesGetter:
	push rbp
	mov rbp, rsp

	call bd_to_binary

    mov al, 0x02
    out 70h, al
    in al, 71h

	mov rsp, rbp
	pop rbp
    ret

hoursGetter:
	push rbp
	mov rbp, rsp

	call bd_to_binary

    mov al, 0x04
    out 70h, al
    in al, 71h

	mov rsp, rbp
	pop rbp
    ret		


speakerIn:
	push rbp
	mov rbp, rsp

	mov rdx, rdi
	in al, dx

	mov rsp, rbp
	pop rbp
	ret

speakerOut:
	push rbp
	mov rbp, rsp 

	mov rax, rsi 
	mov rdx, rdi 
	out dx, al 

	mov rsp, rbp
	pop rbp
	ret