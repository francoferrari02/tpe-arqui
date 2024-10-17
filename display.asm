section .text
global _start

_start:
    ; Mapeo de la memoria de video en modo texto
    mov edx, 0xB8000  ; Dirección de la memoria de video
    mov ecx, 'A'      ; Carácter a escribir
    mov ebx, 0x0A00   ; Atributo: fondo negro (0) y texto verde (0A)

    ; Escribir "Arquitectura de Computadoras"
    mov edi, mensaje
    mov esi, 0
siguiente:
    lodsb               ; Cargar el siguiente byte de 'mensaje' en AL
    cmp al, 0          ; Verificar si es el final del string
    je fin             ; Si es cero, saltar a fin
    mov [edx + esi * 2], al ; Escribir el carácter en la memoria de video
    mov [edx + esi * 2 + 1], bl ; Escribir el atributo
    inc esi            ; Incrementar el índice
    jmp siguiente

fin:
    hlt                 ; Detener la ejecución

mensaje db 'Arquitectura de Computadoras', 0 ; Mensaje a mostrar

