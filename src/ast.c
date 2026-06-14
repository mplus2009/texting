#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

ASTNode* crear_nodo_programa(ASTNode* primera) {
    ASTNode* nodo = malloc(sizeof(ASTNode));
    nodo->type = NODE_PROGRAMA;
    nodo->next = primera;
    return nodo;
}

ASTNode* crear_nodo_escritura(ASTNode* expr) {
    ASTNode* nodo = malloc(sizeof(ASTNode));
    nodo->type = NODE_ESCRITURA;
    nodo->escritura.expr = expr;
    nodo->next = NULL;
    return nodo;
}

ASTNode* crear_nodo_asignacion(char* nombre, ASTNode* valor) {
    ASTNode* nodo = malloc(sizeof(ASTNode));
    nodo->type = NODE_ASIGNACION;
    nodo->asignacion.nombre = strdup(nombre);
    nodo->asignacion.valor = valor;
    nodo->next = NULL;
    return nodo;
}

ASTNode* crear_nodo_lectura(char* nombre) {
    ASTNode* nodo = malloc(sizeof(ASTNode));
    nodo->type = NODE_LECTURA;
    nodo->lectura.nombre = strdup(nombre);
    nodo->next = NULL;
    return nodo;
}

ASTNode* crear_nodo_si(ASTNode* cond, ASTNode* bloque_si, ASTNode* bloque_sino) {
    ASTNode* nodo = malloc(sizeof(ASTNode));
    nodo->type = NODE_SI;
    nodo->si.condicion = cond;
    nodo->si.bloque_si = bloque_si;
    nodo->si.bloque_sino = bloque_sino;
    nodo->next = NULL;
    return nodo;
}

ASTNode* crear_nodo_repetir(int veces, ASTNode* bloque) {
    ASTNode* nodo = malloc(sizeof(ASTNode));
    nodo->type = NODE_REPETIR;
    nodo->repetir.veces = veces;
    nodo->repetir.bloque = bloque;
    nodo->next = NULL;
    return nodo;
}

ASTNode* crear_nodo_mientras(ASTNode* cond, ASTNode* bloque) {
    ASTNode* nodo = malloc(sizeof(ASTNode));
    nodo->type = NODE_MIENTRAS;
    nodo->mientras.condicion = cond;
    nodo->mientras.bloque = bloque;
    nodo->next = NULL;
    return nodo;
}

ASTNode* crear_nodo_numero(double num) {
    ASTNode* nodo = malloc(sizeof(ASTNode));
    nodo->type = NODE_NUMERO;
    nodo->numero.numero = num;
    return nodo;
}

ASTNode* crear_nodo_string(char* texto) {
    ASTNode* nodo = malloc(sizeof(ASTNode));
    nodo->type = NODE_STRING;
    nodo->string.texto = strdup(texto);
    return nodo;
}

ASTNode* crear_nodo_identificador(char* nombre) {
    ASTNode* nodo = malloc(sizeof(ASTNode));
    nodo->type = NODE_IDENTIFICADOR;
    nodo->identificador.nombre = strdup(nombre);
    return nodo;
}

ASTNode* crear_nodo_operacion(char op, ASTNode* izq, ASTNode* der) {
    ASTNode* nodo = malloc(sizeof(ASTNode));
    nodo->type = NODE_OPERACION;
    nodo->operacion.operador = op;
    nodo->operacion.izquierda = izq;
    nodo->operacion.derecha = der;
    return nodo;
}

ASTNode* crear_nodo_condicion(char op, ASTNode* izq, ASTNode* der) {
    ASTNode* nodo = malloc(sizeof(ASTNode));
    nodo->type = NODE_CONDICION;
    nodo->condicion.operador = op;
    nodo->condicion.izquierda = izq;
    nodo->condicion.derecha = der;
    return nodo;
}

void agregar_nodo_bloque(ASTNode** bloque, ASTNode* nuevo) {
    if(*bloque == NULL) {
        *bloque = nuevo;
    } else {
        ASTNode* actual = *bloque;
        while(actual->next) actual = actual->next;
        actual->next = nuevo;
    }
}

void liberar_ast(ASTNode* nodo) {
    if(!nodo) return;
    liberar_ast(nodo->next);
    free(nodo);
}
