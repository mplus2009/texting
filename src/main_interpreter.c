#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "interpreter.h"

extern FILE* yyin;
extern int yyparse();
extern ASTNode* raiz;

int main(int argc, char **argv) {
    if(argc < 2) {
        fprintf(stderr, "Uso: %s archivo.tg\n", argv[0]);
        return 1;
    }
    
    FILE *f = fopen(argv[1], "r");
    if(!f) {
        fprintf(stderr, "No se puede abrir %s\n", argv[1]);
        return 1;
    }
    yyin = f;
    
    printf("--- Texting ---\n");
    init_interpreter();
    yyparse();
    
    if(raiz) {
        ejecutar_programa(raiz);
        liberar_ast(raiz);
        liberar_variables();
    }
    
    printf("--- Fin ---\n");
    fclose(f);
    return 0;
}
