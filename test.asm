BITS 64
section .text
global _start

print_int:
    push rax
    push rbx
    push rcx
    push rdx
    mov rcx, 10
    xor rbx, rbx
.loop:
    xor rdx, rdx
    div rcx
    push rdx
    inc rbx
    test rax, rax
    jnz .loop
.print:
    pop rdx
    add dl, '0'
    mov [buf], dl
    mov rax, 1
    mov rdi, 1
    mov rsi, buf
    mov rdx, 1
    syscall
    dec rbx
    jnz .print
    pop rdx
    pop rcx
    pop rbx
    pop rax
    ret

section .data
buf db 0

section .bss
; Variables del programa
var_x resq 1
var_y resq 1
var_z resq 1
var_resultado resq 1

section .text
_start:
    mov rax, 42
    mov [var_x], rax
    mov rax, [var_x]
    call print_int
    mov rax, 60
    xor rdi, rdi
    syscall
