#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "ast.h"

typedef struct Variable {
    char* nombre;
    int tipo; // 0: entero, 1: texto, 2: decimal
    union {
        int entero;
        char* texto;
        double decimal;
    } valor;
    struct Variable* siguiente;
} Variable;

typedef struct Funcion {
    char* nombre;
    struct ASTNode* bloque;
    struct Funcion* siguiente;
} Funcion;

void init_interpreter(void);
Variable* buscar_variable(char* nombre);
void guardar_variable_entero(char* nombre, int valor);
void guardar_variable_decimal(char* nombre, double valor);
void guardar_variable_texto(char* nombre, char* valor);

void ejecutar_programa(ASTNode* programa);
void ejecutar_bloque(ASTNode* bloque);
void liberar_variables(void);

#endif
