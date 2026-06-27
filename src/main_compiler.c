#include <stdio.h>
#include <stdlib.h>
#include "ast.h"

extern FILE* yyin;
extern int yyparse();
extern ASTNode* raiz;

void compile_program(ASTNode* program, const char* output_file);

int main(int argc, char **argv) {
    if(argc < 3) {
        fprintf(stderr, "Uso: %s entrada.tg salida.asm\n", argv[0]);
        return 1;
    }
    
    yyin = fopen(argv[1], "r");
    if(!yyin) {
        fprintf(stderr, "No se puede abrir %s\n", argv[1]);
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
