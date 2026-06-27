#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "interpreter.h"

extern int yyparse(void);
extern void yy_scan_string(const char* str);
extern ASTNode* raiz;

void ejecutar_linea(char* linea, int* salir) {
    if(strcmp(linea, "salir") == 0 || strcmp(linea, "exit") == 0) {
        *salir = 1;
        return;
    }
    
    if(strcmp(linea, "limpiar") == 0 || strcmp(linea, "clear") == 0) {
        printf("\033[H\033[2J");
        return;
    }

    if(strcmp(linea, "ayuda") == 0 || strcmp(linea, "help") == 0) {
        printf("\n=== Comandos de la Shell de Texting ===\n");
        printf("  Guardar [Num] en [Var];      -> Asignación\n");
        printf("  escribir en consola [Var];   -> Mostrar variable\n");
        printf("  limpiar / salir              -> Comandos del sistema\n\n");
        return;
    }

    // Agregar salto de línea reglamentario
    char comando[1024];
    snprintf(comando, sizeof(comando), "%s\n", linea);
    
    // Simular entrada para el parser
    FILE* temp = fopen(".shell_temp.tg", "w");
    if(temp) {
        fprintf(temp, "%s", comando);
        fclose(temp);
        
        extern FILE* yyin;
        yyin = fopen(".shell_temp.tg", "r");
        if(yyin) {
            raiz = NULL;
            yyparse();
            fclose(yyin);
            if(raiz) {
                ejecutar_programa(raiz);
                liberar_ast(raiz);
            }
        }
    }
}

int main(int argc, char** argv) {
    char linea[1024];
    int salir = 0;
    
    printf("\n");
    printf(" ╔═══════════════════════════════════════════╗\n");
    printf(" ║     Texting OS Shell - Modo Interactivo   ║\n");
    printf(" ╚═══════════════════════════════════════════╝\n\n");
    printf("Escribe 'ayuda' para ver comandos o 'salir' para terminar.\n\n");
    
    init_interpreter();
    
    while(!salir) {
        printf("texting>>> ");
        fflush(stdout);
        if(!fgets(linea, sizeof(linea), stdin)) break;
        
        // Quitar salto de línea de la lectura
        linea[strcspn(linea, "\n")] = 0;
        
        if(strlen(linea) > 0) {
            ejecutar_linea(linea, &salir);
        }
    }
    
    liberar_variables();
    return 0;
}
