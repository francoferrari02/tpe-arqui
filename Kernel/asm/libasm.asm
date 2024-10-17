GLOBAL cpuVendor
GLOBAL getSeconds
GLOBAL getMinutes
GLOBAL getHour
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

	mov byte [rdi + 13], 0

	mov rax, rdi

	pop rbx

	mov rsp, rbp
	pop rbp
	ret

; Función para convertir un entero a string
; Parámetros: 
; rdi - puntero al buffer donde se guardará la cadena
; rsi - número a convertir
getSeconds:
    push rbp
    mov rbp, rsp

    mov al, 0x00
    out 70h, al
    in al, 71h           ; Leer el valor de segundos

    ; Salida de la función
    mov rbp, rsp
    pop rbp
    ret

; Función para obtener los minutos desde el RTC
getMinutes:
    push rbp
    mov rbp, rsp

    mov al, 0x02
    out 70h, al
    in al, 71h           ; Leer el valor de minutos


    ; Salida de la función
    mov rbp, rsp
    pop rbp
    ret

; Función para obtener la hora desde el RTC
getHour:
    push rbp
    mov rbp, rsp

    mov al, 0x04
    out 70h, al
    in al, 71h           ; Leer el valor de horas

    ; Salida de la función
    mov rbp, rsp
    pop rbp
    ret

section .bss
buffer resb 3          ; Buffer para almacenar hasta 3 caracteres (máximo 255)
