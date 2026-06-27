#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "interpreter.h"

// Buffer para almacenar comandos multilínea
char buffer_comando[1024];
int en_bloque = 0;
int lineas_bloque = 0;

int es_inicio_bloque(char* linea) {
    return (strstr(linea, "repetir") != NULL || 
            strstr(linea, "mientras") != NULL ||
            strstr(linea, "si") != NULL);
}

int es_fin_bloque(char* linea) {
    return (strstr(linea, "fin;") != NULL);
}

void ejecutar_linea(char* linea, int* salir) {
    // Verificar si es comando interno
    if(strcmp(linea, "salir") == 0 || strcmp(linea, "exit") == 0) {
        *salir = 1;
        return;
    }
    
    if(strcmp(linea, "help") == 0 || strcmp(linea, "ayuda") == 0) {
        printf("\n=== Comandos disponibles ===\n");
        printf("  Guardar NUM en VAR;              - Guardar numero\n");
        printf("  escribir en consola TEXTO;       - Mostrar texto\n");
        printf("  escribir en consola VAR;         - Mostrar variable\n");
        printf("  si X > Y entonces ... fin;       - Condicional\n");
        printf("  repetir N veces ... fin;         - Bucle fijo\n");
        printf("  mientras X > Y hacer ... fin;    - Bucle while\n");
        printf("  salir / exit                     - Salir del shell\n");
        printf("  help / ayuda                     - Esta ayuda\n");
        printf("================================\n");
        return;
    }
    
    if(strcmp(linea, "clear") == 0 || strcmp(linea, "limpiar") == 0) {
        printf("\033[2J\033[1;1H");  // Limpiar pantalla ANSI
        return;
    }
    
    if(strcmp(linea, "variables") == 0) {
        printf("\n=== Variables definidas ===\n");
        // Esto se implementaría accediendo a la tabla de variables
        printf("(funcion pendiente)\n");
        return;
    }
    
    // Agregar salto de linea y ejecutar
    char comando[1024];
    snprintf(comando, sizeof(comando), "%s\n", linea);
    
    // Usar yy_scan_string para analizar el comando
    // Por simplicidad, guardamos en archivo temporal
    FILE* temp = fopen("/tmp/texting_cmd.tg", "w");
    if(temp) {
        fprintf(temp, "%s", comando);
        fclose(temp);
        
        extern FILE* yyin;
        FILE* f = fopen("/tmp/texting_cmd.tg", "r");
        if(f) {
            yyin = f;
            yyparse();
            fclose(f);
        }
    }
}

int main(int argc, char** argv) {
    char linea[1024];
    int salir = 0;
    
    printf("\n");
    printf("╔═══════════════════════════════════════════╗\n");
    printf("║     Texting OS Shell - Interactive Mode   ║\n");
    printf("╚═══════════════════════════════════════════╝\n");
    printf("\n");
    printf("Escribe 'help' para ver los comandos disponibles\n");
    printf("Escribe 'salir' para terminar la sesion\n");
    printf("\n");
    
    while(!salir) {
        printf("texting> ");
        fflush(stdout);
        
        if(!fgets(linea, sizeof(linea), stdin)) {
            break;
        }
        
        // Quitar salto de linea
        linea[strcspn(linea, "\n")] = 0;
        
        if(strlen(linea) > 0) {
            ejecutar_linea(linea, &salir);
        }
    }
    
    printf("Adios!\n");
    return 0;
}
