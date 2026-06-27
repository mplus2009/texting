; Texting - Código máquina generado
BITS 16
ORG 0x1000

start:
    mov ax, 1
    mov [var_0], ax
    mov ax, [var_0]
    call print_num
    mov ax, [var_0]
    push ax
    mov ax, 1
    pop bx
    add ax, bx
    mov [var_0], ax
    jmp $

print_num:
    push ax
    push bx
    push cx
    push dx
    mov cx, 10
    xor bx, bx
.div_loop:
    xor dx, dx
    div cx
    push dx
    inc bx
    test ax, ax
    jnz .div_loop
.print_loop:
    pop dx
    add dl, '0'
    mov ah, 0x0E
    int 0x10
    dec bx
    jnz .print_loop
    pop dx
    pop cx
    pop bx
    pop ax
    ret

section .bss
var_0 resw 1  ; i
