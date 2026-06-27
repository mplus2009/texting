#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "interpreter.h"

Variable* variables = NULL;

void init_interpreter(void) {
    // Inicialización del entorno de ejecución
}

Variable* buscar_variable(char* nombre) {
    Variable* v = variables;
    while(v) {
        if(strcmp(v->nombre, nombre) == 0) return v;
        v = v->siguiente;
    }
    return NULL;
}

void guardar_variable_entero(char* nombre, int valor) {
    Variable* v = buscar_variable(nombre);
    if(!v) {
        v = malloc(sizeof(Variable));
        v->nombre = strdup(nombre);
        v->siguiente = variables;
        variables = v;
    }
    v->tipo = 0;
    v->valor.entero = valor;
}

void guardar_variable_decimal(char* nombre, double valor) {
    Variable* v = buscar_variable(nombre);
    if(!v) {
        v = malloc(sizeof(Variable));
        v->nombre = strdup(nombre);
        v->siguiente = variables;
        variables = v;
    }
    v->tipo = 2;
    v->valor.decimal = valor;
}

void guardar_variable_texto(char* nombre, char* valor) {
    Variable* v = buscar_variable(nombre);
    if(!v) {
        v = malloc(sizeof(Variable));
        v->nombre = strdup(nombre);
        v->siguiente = variables;
        variables = v;
    }
    v->tipo = 1;
    v->valor.texto = strdup(valor);
}

int evaluar_condicion(ASTNode* cond) {
    if(!cond) return 0;
    
    // Evaluación simple de expresiones izquierda y derecha
    int izq = 0, der = 0;
    if(cond->condicion.izquierda->type == NODE_NUMERO) izq = cond->condicion.izquierda->numero.numero;
    else if(cond->condicion.izquierda->type == NODE_IDENTIFICADOR) {
        Variable* v = buscar_variable(cond->condicion.izquierda->identificador.nombre);
        if(v && v->tipo == 0) izq = v->valor.entero;
    }
    
    if(cond->condicion.derecha->type == NODE_NUMERO) der = cond->condicion.derecha->numero.numero;
    else if(cond->condicion.derecha->type == NODE_IDENTIFICADOR) {
        Variable* v = buscar_variable(cond->condicion.derecha->identificador.nombre);
        if(v && v->tipo == 0) der = v->valor.entero;
    }

    switch(cond->condicion.operador) {
        case '<': return izq < der;
        case '>': return izq > der;
        case '=': return izq == der;
        case 'L': return izq <= der;
        case 'G': return izq >= der;
        default: return 0;
    }
}

void ejecutar_bloque(ASTNode* bloque) {
    ASTNode* actual = bloque;
    while(actual) {
        if(actual->type == NODE_ASIGNACION) {
            if(actual->asignacion.expresion->type == NODE_NUMERO) {
                guardar_variable_entero(actual->asignacion.nombre, actual->asignacion.expresion->numero.numero);
            }
        }
        else if(actual->type == NODE_ESCRIBIR) {
            if(actual->escritura.expresion->type == NODE_NUMERO) {
                printf("%d\n", (int)actual->escritura.expresion->numero.numero);
            } 
            else if(actual->escritura.expresion->type == NODE_IDENTIFICADOR) {
                Variable* v = buscar_variable(actual->escritura.expresion->identificador.nombre);
                if(v) {
                    if(v->tipo == 0) printf("%d\n", v->valor.entero);
                    else if(v->tipo == 1) printf("%s\n", v->valor.texto);
                } else {
                    printf("Error: Variable '%s' no definida.\n", actual->escritura.expresion->identificador.nombre);
                }
            }
        }
        else if(actual->type == NODE_STRING) {
            printf("%s\n", actual->string_node.texto);
        }
        else if(actual->type == NODE_SI) {
            if(evaluar_condicion(actual->si.condicion)) {
                ejecutar_bloque(actual->si.bloque_si);
            } else if(actual->si.bloque_sino) {
                ejecutar_bloque(actual->si.bloque_sino);
            }
        }
        actual = actual->next;
    }
}

void ejecutar_programa(ASTNode* programa) {
    ejecutar_bloque(programa);
}

void liberar_variables(void) {
    Variable* v = variables;
    while(v) {
        Variable* aux = v->siguiente;
        free(v->nombre);
        if(v->tipo == 1) free(v->valor.texto);
        free(v);
        v = aux;
    }
    variables = NULL;
}
