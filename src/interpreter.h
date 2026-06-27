#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "ast.h"

typedef struct Variable {
    char* nombre;
    int tipo;
    union {
        int entero;
        double decimal;
        char* texto;
    } valor;
    struct Variable* siguiente;
} Variable;

// Estructura para funciones
typedef struct Funcion {
    char* nombre;
    char** parametros;
    int num_params;
    ASTNode* cuerpo;
    struct Funcion* siguiente;
} Funcion;

// Variables globales
extern Variable* variables;
extern Funcion* funciones;

// Funciones del intérprete
void init_interpreter();
void ejecutar_programa(ASTNode* programa);
void liberar_variables();
Variable* buscar_variable(char* nombre);
double obtener_valor_numerico(ASTNode* expr);
char* obtener_valor_texto(ASTNode* expr);
void guardar_variable_entero(char* nombre, int valor);
void guardar_variable_decimal(char* nombre, double valor);
void guardar_variable_texto(char* nombre, char* valor);
void guardar_variable_lista(char* nombre, void* lista);
void guardar_variable_archivo(char* nombre, void* archivo);

// Funciones para funciones
Funcion* buscar_funcion(char* nombre);
void ejecutar_funcion(char* nombre, ASTNode** args, int num_args);

#endif
