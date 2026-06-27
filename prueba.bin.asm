; Texting - Código máquina generado
BITS 16
ORG 0x1000

start:
    call print_num
    mov ax, 42
    mov [var_0], ax
    mov ax, [var_0]
    call print_num
    mov ax, 10
    mov [var_1], ax
    mov ax, 20
    mov [var_2], ax
    mov ax, [var_1]
    push ax
    mov ax, [var_2]
    pop bx
    add ax, bx
    mov [var_3], ax
    mov ax, [var_3]
    call print_num
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
var_0 resw 1  ; respuesta
var_1 resw 1  ; a
var_2 resw 1  ; b
var_3 resw 1  ; suma
