GLOBAL cpuVendor
GLOBAL getSeconds
GLOBAL getMinutes
GLOBAL getHour

global waitForKeyPress  ; Hace la función visible para el código en C

section .data


    ; Mapa de códigos de escaneo a caracteres ASCII
    scancode_map db 0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', 8, 9
                db 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', 13, 0, 'a', 's'
                db 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '\\', 'z', 'x', 'c', 'v'
                db 'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' ', 0


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






waitForKeyPress:
    call getKeyPress

    ; Convertir el scan code al carácter ASCII usando el mapa
    movzx rax, al          ; Zero extend el scan code de 8 bits a 64 bits
    cmp al, 0x58           ; Chequea si es un código de scan válido (máx. 0x58 en esta tabla)
    ja invalid_scancode    ; Si es mayor, no es válido
    mov al, [scancode_map + rax]  ; Cargar el carácter correspondiente

invalid_scancode:
    mov [keyPressed], al   ; Almacenar el carácter o código de error en keyPressed
    lea rax, [keyPressed]  ; Cargar la dirección de keyPressed en rax
    ret

getKeyPress:
    .wait:
        in al, 0x64        ; Leer el estado del puerto 0x64
        test al, 0x01      ; Verificar si hay datos disponibles
        jz .wait           ; Si no, sigue esperando

    in al, 0x60            ; Leer el código de escaneo del puerto 0x60
    ret

section .bss

keyPressed resb 1

buffer resb 3          ; Buffer para almacenar hasta 3 caracteres (máximo 255)

