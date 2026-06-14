#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "ast.h"

typedef struct Variable Variable;
struct Variable {
    char* nombre;
    int tipo;
    union {
        int entero;
        double decimal;
        char* texto;
    } valor;
    Variable* siguiente;
};

typedef struct Funcion Funcion;
struct Funcion {
    char* nombre;
    char** parametros;
    int num_params;
    ASTNode* cuerpo;
    Funcion* siguiente;
};

void init_interpreter();
void ejecutar_programa(ASTNode* programa);
void liberar_variables();

#endif
