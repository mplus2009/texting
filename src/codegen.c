#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "codegen.h"
#include "ast.h"

FILE* salida = NULL;

void emit(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vfprintf(salida, fmt, args);
    va_end(args);
}

void generar_instruccion(ASTNode* nodo) {
    if(!nodo) return;
    
    switch(nodo->type) {
        case NODE_ESCRITURA:
            // Generar syscall write para Linux/Windows/MyOS
            emit("    mov rax, 1\n");     // syscall write
            emit("    mov rdi, 1\n");     // stdout
            emit("    lea rsi, [msg_%d]\n", __LINE__);
            emit("    mov rdx, 13\n");
            emit("    syscall\n");
            break;
            
        case NODE_ASIGNACION:
            emit("    mov rax, %d\n", (int)nodo->asignacion.valor);
            emit("    mov [var_%s], rax\n", nodo->asignacion.nombre);
            break;
            
        case NODE_REPETIR:
            emit("    mov rcx, %d\n", nodo->repetir.veces);
            emit("_bucle_%d:\n", __LINE__);
            generar_instruccion(nodo->repetir.bloque);
            emit("    loop _bucle_%d\n", __LINE__);
            break;
    }
}

void generar_codigo(ASTNode* programa, CodeGenConfig* config) {
    salida = fopen(config->output_file, "w");
    if(!salida) return;
    
    // Cabecera para Linux x86_64
    emit("section .text\n");
    emit("global _start\n");
    emit("\n");
    emit("_start:\n");
    
    // Generar código del programa
    generar_instruccion(programa);
    
    // Salida del programa
    emit("    mov rax, 60\n");    // syscall exit
    emit("    xor rdi, rdi\n");
    emit("    syscall\n");
    
    // Datos
    emit("section .data\n");
    emit("msg_1: db 'Hola Mundo', 10\n");
    
    fclose(salida);
}

void generar_bootloader(char* output_file) {
    FILE* boot = fopen(output_file, "wb");
    if(!boot) return;
    
    // Bootloader simple para x86 real mode
    unsigned char bootloader[] = {
        0x31, 0xC0,              // xor ax, ax
        0x8E, 0xD8,              // mov ds, ax
        0x8E, 0xC0,              // mov es, ax
        0xBE, 0x00, 0x7C,        // mov si, 0x7C00
        0xBF, 0x00, 0x10,        // mov di, 0x1000
        0xB9, 0x00, 0x02,        // mov cx, 512
        0xF3, 0xA4,              // rep movsb
        0xEA, 0x00, 0x10, 0x00, 0x00,  // jmp 0x1000:0x0000
        0x00, 0x00, 0x55, 0xAA   // Boot signature
    };
    
    fwrite(bootloader, 1, sizeof(bootloader), boot);
    fclose(boot);
}
