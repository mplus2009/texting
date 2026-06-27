#ifndef CODEGEN_H
#define CODEGEN_H

#include "ast.h"

typedef enum {
    TARGET_LINUX,
    TARGET_WINDOWS,
    TARGET_MYOS
} Target;

typedef struct {
    Target target;
    char* output_file;
    int bits;  // 32 o 64
} CodeGenConfig;

void generar_codigo(ASTNode* programa, CodeGenConfig* config);
void generar_bootloader(char* output_file);

#endif
