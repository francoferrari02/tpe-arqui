GLOBAL cpuVendor
GLOBAL getSeconds
GLOBAL getMinutes
GLOBAL getHour
GLOBAL waitForKeyPress  
GLOBAL rtc_bd_to_binary

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

getSeconds:
    push rbp
    mov rbp, rsp

    call rtc_bd_to_binary

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

    call rtc_bd_to_binary

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

    call rtc_bd_to_binary

    mov al, 0x04
    out 70h, al
    in al, 71h           ; Leer el valor de horas

    ; Salida de la función
    mov rbp, rsp
    pop rbp
    ret

; Función para convertir el formato BCD a binario
rtc_bd_to_binary:
    push rbp
    mov rbp, rsp

    mov al, 0Bh           ; Seleccionar el registro de estado B
    out 70h, al
    in al, 71h            ; Leer el valor del registro de estado B

    or al, 00000010b      ; Activar el bit 2 (modo binario)
    
    mov bl, al            ; Guardar el valor de al en bl
    mov al, 0Bh           ; Seleccionar nuevamente el registro de estado B
    out 70h, al
    mov al, bl            ; Restaurar el valor modificado de al
    out 71h, al           ; Escribir el valor de vuelta al registro

    mov rsp, rbp 
    pop rbp
    ret


section .bss

keyPressed resb 1

buffer resb 3          ; Buffer para almacenar hasta 3 caracteres (máximo 255)

