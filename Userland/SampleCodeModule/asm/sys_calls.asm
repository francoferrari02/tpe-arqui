GLOBAL sys_read
GLOBAL sys_write
GLOBAL sys_clear
GLOBAL sys_getMinutes
GLOBAL sys_getHours
GLOBAL sys_getSeconds
GLOBAL sys_scrHeight
GLOBAL sys_scrWidth
GLOBAL sys_fillRect
GLOBAL sys_wait
GLOBAL sys_inforeg
GLOBAL sys_printmem
GLOBAL sys_pixelMinus
GLOBAL sys_pixelPlus
GLOBAL sys_playSound
GLOBAL sys_mute
section .text



sys_read:
    mov rax, 0x00
    int 80h
    ret

sys_write:
    mov rax, 0x01
    int 80h
    ret

sys_clear:
    mov rax, 0x02
    int 80h
    ret

sys_getHours:
    mov rax, 0x03
    int 80h
    ret

sys_getMinutes:
    mov rax, 0x04
    int 80h
    ret

sys_getSeconds:
    mov rax, 0x05
    int 80h
    ret

sys_scrHeight:
    mov rax, 0x06
    int 80h
    ret

sys_scrWidth:
    mov rax, 0x07
    int 80h
    ret

sys_fillRect:
    mov rax, 0x08
    mov r10, rcx        ;4to parametro de syscall es R10
    int 80h
    ret

sys_wait:
    mov rax, 0x09
    int 80h
    ret

sys_inforeg:
    mov rax, 0x0A
    int 80h
    ret

sys_printmem: 
    mov rax, 0x0B
    int 80h
    ret

sys_pixelPlus: 
    mov rax, 0x0C
    int 80h
    ret

sys_pixelMinus: 
    mov rax, 0x0D
    int 80h
    ret

sys_playSound: 
    mov rax, 0x0E
    int 80h
    ret

sys_stopSound: 
    mov rax, 0x0F
    int 80h
    ret


