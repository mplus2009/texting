#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "interpreter.h"

Variable* variables = NULL;
Funcion* funciones = NULL;

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
    if(v->valor.texto) free(v->valor.texto);
    v->valor.texto = strdup(valor);
}

double obtener_valor_numerico(ASTNode* expr) {
    if(!expr) return 0;
    
    if(expr->type == NODE_NUMERO) return expr->numero.numero;
    if(expr->type == NODE_DECIMAL) return expr->decimal.decimal;
    if(expr->type == NODE_IDENTIFICADOR) {
        Variable* v = buscar_variable(expr->identificador.nombre);
        if(!v) {
            fprintf(stderr, "Error: variable '%s' no definida\n", expr->identificador.nombre);
            return 0;
        }
        if(v->tipo == 0) return v->valor.entero;
        if(v->tipo == 2) return v->valor.decimal;
        return 0;
    }
    if(expr->type == NODE_OPERACION) {
        double izq = obtener_valor_numerico(expr->operacion.izquierda);
        double der = obtener_valor_numerico(expr->operacion.derecha);
        switch(expr->operacion.operador) {
            case '+': return izq + der;
            case '-': return izq - der;
            case '*': return izq * der;
            case '/': 
                if(der == 0) { fprintf(stderr, "Error: division por cero\n"); return 0; }
                return izq / der;
            case 'N': return -izq;
            default: return 0;
        }
    }
    return 0;
}

char* obtener_valor_texto(ASTNode* expr) {
    static char buffer[100];
    if(!expr) return "";
    
    if(expr->type == NODE_STRING) return expr->string.texto;
    if(expr->type == NODE_IDENTIFICADOR) {
        Variable* v = buscar_variable(expr->identificador.nombre);
        if(!v) return "";
        if(v->tipo == 1) return v->valor.texto;
        if(v->tipo == 0) {
            sprintf(buffer, "%d", v->valor.entero);
            return buffer;
        }
        if(v->tipo == 2) {
            sprintf(buffer, "%g", v->valor.decimal);
            return buffer;
        }
    }
    double num = obtener_valor_numerico(expr);
    sprintf(buffer, "%g", num);
    return buffer;
}

int evaluar_condicion(ASTNode* cond) {
    if(!cond) return 0;
    
    if(cond->type == NODE_CONDICION) {
        double izq = obtener_valor_numerico(cond->condicion.izquierda);
        double der = obtener_valor_numerico(cond->condicion.derecha);
        switch(cond->condicion.operador) {
            case '<': return izq < der;
            case '>': return izq > der;
            default: return 0;
        }
    }
    return 0;
}

void ejecutar_bloque(ASTNode* nodo) {
    ASTNode* actual = nodo;
    while(actual) {
        if(actual->type == NODE_ESCRITURA) {
            ASTNode* expr = actual->escritura.expr;
            if(expr->type == NODE_STRING) {
                printf("%s\n", expr->string.texto);
            } else {
                printf("%g\n", obtener_valor_numerico(expr));
            }
        }
        else if(actual->type == NODE_ASIGNACION) {
            char* nombre = actual->asignacion.nombre;
            ASTNode* valor = actual->asignacion.valor;
            double resultado = obtener_valor_numerico(valor);
            if(resultado == (int)resultado) {
                guardar_variable_entero(nombre, (int)resultado);
            } else {
                guardar_variable_decimal(nombre, resultado);
            }
        }
        else if(actual->type == NODE_LECTURA) {
            char entrada[100];
            printf("> ");
            fgets(entrada, 100, stdin);
            entrada[strlen(entrada)-1] = '\0';
            char* end;
            double num = strtod(entrada, &end);
            if(*end == '\0') {
                guardar_variable_entero(actual->lectura.nombre, (int)num);
            } else {
                guardar_variable_texto(actual->lectura.nombre, entrada);
            }
        }
        else if(actual->type == NODE_SI) {
            int cond = evaluar_condicion(actual->si.condicion);
            if(cond) {
                ejecutar_bloque(actual->si.bloque_si);
            } else if(actual->si.bloque_sino) {
                ejecutar_bloque(actual->si.bloque_sino);
            }
        }
        else if(actual->type == NODE_REPETIR) {
            for(int i = 0; i < actual->repetir.veces; i++) {
                ejecutar_bloque(actual->repetir.bloque);
            }
        }
        else if(actual->type == NODE_MIENTRAS) {
            while(evaluar_condicion(actual->mientras.condicion)) {
                ejecutar_bloque(actual->mientras.bloque);
            }
        }
        actual = actual->next;
    }
}

void ejecutar_programa(ASTNode* programa) {
    ejecutar_bloque(programa);
}

void liberar_variables() {
    Variable* v = variables;
    while(v) {
        Variable* sig = v->siguiente;
        free(v->nombre);
        if(v->tipo == 1) free(v->valor.texto);
        free(v);
        v = sig;
    }
    variables = NULL;
}

void init_interpreter() {
    variables = NULL;
    funciones = NULL;
}
