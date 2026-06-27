#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "interpreter.h"

extern FILE* yyin;
extern int yyparse();
extern ASTNode* raiz;

// --- Modo REPL simple que acumula todo ---
void modo_repl() {
    printf("\n");
    printf("╔═══════════════════════════════════════════════════════╗\n");
    printf("║        Texting - Modo Interactivo (REPL)              ║\n");
    printf("╚═══════════════════════════════════════════════════════╝\n");
    printf("\n");
    printf("Escribe 'salir()' para terminar\n");
    printf("Escribe 'help()' para ayuda\n");
    printf("\n");
    printf("📝 Escribe línea por línea. Presiona Enter dos veces para ejecutar.\n");
    printf("   También puedes pegar bloques completos y presionar Enter dos veces.\n");
    printf("\n");
    
    char line[1024];
    char full_code[32768] = "";
    int empty_lines = 0;
    int line_count = 0;
    
    while(1) {
        if(line_count > 0) {
            printf("...> ");
        } else {
            printf("texting> ");
        }
        
        if(!fgets(line, sizeof(line), stdin)) break;
        line[strcspn(line, "\n")] = 0;
        
        // Comandos especiales
        if(strcmp(line, "salir()") == 0 || strcmp(line, "exit()") == 0) {
            if(strlen(full_code) > 0) {
                printf("Cancelando...\n");
                full_code[0] = '\0';
                line_count = 0;
                empty_lines = 0;
                continue;
            }
            break;
        }
        
        if(strcmp(line, "clear()") == 0) {
            printf("\033[2J\033[1;1H");
            continue;
        }
        
        if(strcmp(line, "help()") == 0) {
            printf("\n=== Ayuda de Texting ===\n");
            printf("Variables:    guardar 10 en x;\n");
            printf("Salida:       escribir en consola x;\n");
            printf("Entrada:      leer entrada en nombre;\n");
            printf("Condicional:  si (x > 5) { ... } sino { ... }\n");
            printf("Bucle repetir: repetir 5 veces { ... }\n");
            printf("Bucle mientras: mientras (i < 10) hacer { ... }\n");
            printf("Funciones:    definir sumar(a, b) { retornar a + b; }\n");
            printf("Listas:       [1, 2, 3]\n");
            printf("Concatenación: \"Hola\" & \" Mundo\"\n");
            printf("Comentarios:  # comentario  o  // comentario\n");
            printf("\n");
            printf("=== Comandos REPL ===\n");
            printf("salir()       - Salir del REPL\n");
            printf("clear()       - Limpiar pantalla\n");
            printf("help()        - Mostrar ayuda\n");
            continue;
        }
        
        // Línea vacía: ejecutar si hay código
        if(strlen(line) == 0) {
            empty_lines++;
            if(empty_lines >= 1 && strlen(full_code) > 0) {
                // Ejecutar el código acumulado
                if(strlen(full_code) > 0) {
                    FILE* temp = fopen("tmp/repl_code.tg", "w");
                    if(temp) {
                        fprintf(temp, "%s\n", full_code);
                        fclose(temp);
                        
                        yyin = fopen("tmp/repl_code.tg", "r");
                        if(yyin) {
                            init_interpreter();
                            yyparse();
                            if(raiz) {
                                ejecutar_programa(raiz);
                                liberar_ast(raiz);
                                liberar_variables();
                                raiz = NULL;
                            }
                            fclose(yyin);
                        }
                    }
                }
                full_code[0] = '\0';
                line_count = 0;
                empty_lines = 0;
            }
            continue;
        }
        
        // Reiniciar contador de líneas vacías
        empty_lines = 0;
        
        // Acumular código
        if(strlen(full_code) > 0) {
            strcat(full_code, "\n");
        }
        strcat(full_code, line);
        line_count++;
        
        // Si el código se hace muy largo, ejecutar automáticamente
        if(strlen(full_code) > 30000) {
            FILE* temp = fopen("tmp/repl_code.tg", "w");
            if(temp) {
                fprintf(temp, "%s\n", full_code);
                fclose(temp);
                
                yyin = fopen("tmp/repl_code.tg", "r");
                if(yyin) {
                    init_interpreter();
                    yyparse();
                    if(raiz) {
                        ejecutar_programa(raiz);
                        liberar_ast(raiz);
                        liberar_variables();
                        raiz = NULL;
                    }
                    fclose(yyin);
                }
            }
            full_code[0] = '\0';
            line_count = 0;
            empty_lines = 0;
        }
    }
    
    // Ejecutar código pendiente al salir
    if(strlen(full_code) > 0) {
        FILE* temp = fopen("tmp/repl_code.tg", "w");
        if(temp) {
            fprintf(temp, "%s\n", full_code);
            fclose(temp);
            
            yyin = fopen("tmp/repl_code.tg", "r");
            if(yyin) {
                init_interpreter();
                yyparse();
                if(raiz) {
                    ejecutar_programa(raiz);
                    liberar_ast(raiz);
                    liberar_variables();
                    raiz = NULL;
                }
                fclose(yyin);
            }
        }
    }
    
    printf("\n¡Hasta luego!\n");
}

// --- Ejecutar archivo ---
void modo_run(char* filename) {
    FILE* f = fopen(filename, "r");
    if(!f) {
        fprintf(stderr, "Error: No se puede abrir %s\n", filename);
        return;
    }
    yyin = f;
    init_interpreter();
    yyparse();
    if(raiz) {
        ejecutar_programa(raiz);
        liberar_ast(raiz);
        liberar_variables();
        raiz = NULL;
    }
    fclose(f);
}

// --- Compilar a binario ---
void modo_compile(char* filename, char* output) {
    FILE* f = fopen(filename, "r");
    if(!f) {
        fprintf(stderr, "Error: No se puede abrir %s\n", filename);
        return;
    }
    yyin = f;
    yyparse();
    fclose(f);
    if(raiz) {
        extern void compile_program(ASTNode* program, const char* output_file);
        compile_program(raiz, output);
        liberar_ast(raiz);
        raiz = NULL;
        printf("✅ Binario generado: %s\n", output);
    }
}

// --- Mostrar ayuda ---
void mostrar_ayuda() {
    printf("Uso: texting [OPCION] [ARCHIVO]\n\n");
    printf("Opciones:\n");
    printf("  -i, --interactive    Modo REPL interactivo\n");
    printf("  -r, --run ARCHIVO    Ejecutar archivo .tg (intérprete)\n");
    printf("  -c, --compile ARCHIVO [SALIDA]  Compilar a binario\n");
    printf("  -h, --help           Mostrar ayuda\n\n");
    printf("Ejemplos:\n");
    printf("  texting -i\n");
    printf("  texting -r programa.tg\n");
    printf("  texting -c programa.tg kernel.bin\n");
}

int main(int argc, char** argv) {
    if(argc < 2) {
        mostrar_ayuda();
        return 1;
    }
    if(strcmp(argv[1], "-i") == 0 || strcmp(argv[1], "--interactive") == 0) {
        modo_repl();
        return 0;
    }
    if(strcmp(argv[1], "-r") == 0 || strcmp(argv[1], "--run") == 0) {
        if(argc < 3) {
            fprintf(stderr, "Falta archivo\n");
            return 1;
        }
        modo_run(argv[2]);
        return 0;
    }
    if(strcmp(argv[1], "-c") == 0 || strcmp(argv[1], "--compile") == 0) {
        if(argc < 3) {
            fprintf(stderr, "Falta archivo\n");
            return 1;
        }
        char* output = (argc >= 4) ? argv[3] : "a.out";
        modo_compile(argv[2], output);
        return 0;
    }
    if(strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
        mostrar_ayuda();
        return 0;
    }
    fprintf(stderr, "Opción no reconocida: %s\n", argv[1]);
    mostrar_ayuda();
    return 1;
}
