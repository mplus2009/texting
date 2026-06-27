#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "ast.h"

FILE* out;
unsigned char* buffer;
int buffer_size;
int buffer_capacity;

void init_buffer() {
    buffer_capacity = 65536;
    buffer = malloc(buffer_capacity);
    buffer_size = 0;
}

void emit_byte(unsigned char byte) {
    if(buffer_size >= buffer_capacity) {
        buffer_capacity *= 2;
        buffer = realloc(buffer, buffer_capacity);
    }
    buffer[buffer_size++] = byte;
}

void emit_word(unsigned short word) {
    emit_byte(word & 0xFF);
    emit_byte((word >> 8) & 0xFF);
}

void emit_dword(unsigned int dword) {
    emit_byte(dword & 0xFF);
    emit_byte((dword >> 8) & 0xFF);
    emit_byte((dword >> 16) & 0xFF);
    emit_byte((dword >> 24) & 0xFF);
}

void emit(const unsigned char* bytes, int len) {
    for(int i = 0; i < len; i++) {
        emit_byte(bytes[i]);
    }
}

void emit_mov_ax_imm(int value) {
    emit_byte(0xB8);  // mov ax, imm16
    emit_word(value);
}

void emit_mov_bx_imm(int value) {
    emit_byte(0xBB);  // mov bx, imm16
    emit_word(value);
}

void emit_mov_cx_imm(int value) {
    emit_byte(0xB9);  // mov cx, imm16
    emit_word(value);
}

void emit_mov_dx_imm(int value) {
    emit_byte(0xBA);  // mov dx, imm16
    emit_word(value);
}

void emit_mov_si_imm(int value) {
    emit_byte(0xBE);  // mov si, imm16
    emit_word(value);
}

void emit_mov_di_imm(int value) {
    emit_byte(0xBF);  // mov di, imm16
    emit_word(value);
}

void emit_mov_ax_mem(int address) {
    emit_byte(0xA1);  // mov ax, [address]
    emit_word(address);
}

void emit_mov_mem_ax(int address) {
    emit_byte(0xA3);  // mov [address], ax
    emit_word(address);
}

void emit_add_ax_imm(int value) {
    emit_byte(0x05);  // add ax, imm16
    emit_word(value);
}

void emit_sub_ax_imm(int value) {
    emit_byte(0x2D);  // sub ax, imm16
    emit_word(value);
}

void emit_mul_bx() {
    emit_byte(0xF7);  // mul bx
    emit_byte(0xE3);
}

void emit_div_bx() {
    emit_byte(0xF7);  // div bx
    emit_byte(0xF3);
}

void emit_loop(int offset) {
    emit_byte(0xE2);  // loop
    emit_byte(offset);
}

void emit_int(int num) {
    emit_byte(0xCD);  // int
    emit_byte(num);
}

void emit_ret() {
void emit_inc_bx() { emit_byte(0x43); }
void emit_dec_bx() { emit_byte(0x4B); }
void emit_add_dl_imm(unsigned char val) { emit_byte(0x80); emit_byte(0xC2); emit_byte(val); }
void emit_mov_ah_imm(unsigned char val) { emit_byte(0xB4); emit_byte(val); }
    emit_byte(0xC3);
}

void emit_label(int* labels, int index) {
    // Las etiquetas se resuelven después
}

// Variables (espacio en memoria)
int next_var_addr = 0x1000;  // Las variables empiezan en 0x1000

int get_var_address(char* nombre) {
    // Por simplicidad, cada variable tiene un slot fijo
    static int var_counter = 0;
    static char var_names[100][50];
    static int var_addrs[100];
    
    for(int i = 0; i < var_counter; i++) {
        if(strcmp(var_names[i], nombre) == 0) {
            return var_addrs[i];
        }
    }
    
    strcpy(var_names[var_counter], nombre);
    var_addrs[var_counter] = next_var_addr;
    next_var_addr += 2;  // cada variable ocupa 2 bytes (word)
    var_counter++;
    
    return var_addrs[var_counter - 1];
}

void compile_expression(ASTNode* node) {
    if(!node) return;
    
    switch(node->type) {
        case NODE_NUMERO:
            emit_mov_ax_imm((int)node->numero.numero);
            break;
            
        case NODE_IDENTIFICADOR: {
            int addr = get_var_address(node->identificador.nombre);
            emit_mov_ax_mem(addr);
            break;
        }
        
        case NODE_OPERACION: {
            compile_expression(node->operacion.izquierda);
            emit("PUSH AX", 5);
            compile_expression(node->operacion.derecha);
            emit_mov_bx_imm(0);
            // pop bx
            emit_byte(0x5B);  // pop bx
            switch(node->operacion.operador) {
                case '+':
                    emit_byte(0x03);  // add ax, bx
                    emit_byte(0xC3);
                    break;
                case '-':
                    emit_byte(0x2B);  // sub ax, bx
                    emit_byte(0xC3);
                    break;
                case '*':
                    emit_mul_bx();
                    break;
                case '/':
                    emit_mov_dx_imm(0);
                    emit_div_bx();
                    break;
            }
            break;
        }
        default:
            break;
    }
}

void compile_instruction(ASTNode* node) {
    if(!node) return;
    
    switch(node->type) {
        case NODE_ASIGNACION: {
            int addr = get_var_address(node->asignacion.nombre);
            compile_expression(node->asignacion.valor);
            emit_mov_mem_ax(addr);
            break;
        }
        
        case NODE_ESCRITURA: {
            // Imprimir número en consola (usando BIOS)
            compile_expression(node->escritura.expr);
            emit("CALL print_num", 12);
            break;
        }
        
        case NODE_REPETIR: {
            emit_mov_cx_imm(node->repetir.veces);
            int loop_start = buffer_size;
            compile_instruction(node->repetir.bloque);
            emit_loop(loop_start - buffer_size - 1);
            break;
        }
        
        default:
            break;
    }
}

void compile_program(ASTNode* program, const char* output_file) {
    init_buffer();
    
    // Código de inicio
    emit_byte(0x00);  // lugar para variables
    
    // Función print_num (imprime AX en decimal)
    int print_num_start = buffer_size;
    emit("PUSH AX", 5);
    emit("PUSH BX", 5);
    emit("PUSH CX", 5);
    emit("PUSH DX", 5);
    emit_mov_cx_imm(10);
    emit_mov_bx_imm(0);
    // bucle de división
    int loop_div = buffer_size;
    emit_mov_dx_imm(0);
    emit_div_bx();
    emit("PUSH DX", 5);
    emit_inc_bx();
    emit("TEST AX, AX", 9);
    emit_byte(0x75);  // jnz
    emit_byte(loop_div - buffer_size - 1);
    // bucle de impresión
    int loop_print = buffer_size;
    emit_byte(0x5A);  // pop dx
    emit_add_dl_imm(0x30);
    emit_mov_ah_imm(0x0E);
    emit_int(0x10);
    emit_dec_bx();
    emit_byte(0x75);  // jnz
    emit_byte(loop_print - buffer_size - 1);
    emit("POP DX", 5);
    emit("POP CX", 5);
    emit("POP BX", 5);
    emit("POP AX", 5);
    emit_ret();
    
    // Programa principal
    ASTNode* node = program;
    while(node) {
        compile_instruction(node);
        node = node->next;
    }
    
    // Bucle infinito al final
    emit("JMP $", 4);
    
    // Guardar binario
    out = fopen(output_file, "wb");
    if(out) {
        fwrite(buffer, 1, buffer_size, out);
        fclose(out);
    }
    
    free(buffer);
}

int main(int argc, char** argv) {
    if(argc < 3) {
        fprintf(stderr, "Uso: %s entrada.tg salida.bin\n", argv[0]);
        return 1;
    }
    
    extern FILE* yyin;
    extern ASTNode* raiz;
    extern int yyparse();
    
    yyin = fopen(argv[1], "r");
    if(!yyin) {
        perror("No se puede abrir el archivo fuente");
        return 1;
    }
    
    if(yyparse() != 0) {
        fprintf(stderr, "Error de sintaxis\n");
        fclose(yyin);
        return 1;
    }
    
    compile_program(raiz, argv[2]);
    
    fclose(yyin);
    printf("Compilación exitosa: %s\n", argv[2]);
    return 0;
}
