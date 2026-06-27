#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

// Funciones básicas
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

ASTNode* crear_nodo_lista(ASTNode** elementos, int cantidad) {
    ASTNode* nodo = malloc(sizeof(ASTNode));
    nodo->type = NODE_LISTA;
    nodo->lista.elementos = elementos;
    nodo->lista.cantidad = cantidad;
    nodo->next = NULL;
    return nodo;
}

ASTNode* crear_nodo_acceso_lista(char* nombre, ASTNode* indice) {
    ASTNode* nodo = malloc(sizeof(ASTNode));
    nodo->type = NODE_ACCESO_LISTA;
    nodo->acceso_lista.nombre = strdup(nombre);
    nodo->acceso_lista.indice = indice;
    nodo->next = NULL;
    return nodo;
}

ASTNode* crear_nodo_archivo_abrir(char* nombre, char* modo) {
    ASTNode* nodo = malloc(sizeof(ASTNode));
    nodo->type = NODE_ARCHIVO_ABRIR;
    nodo->archivo_abrir.nombre_archivo = strdup(nombre);
    nodo->archivo_abrir.modo = strdup(modo);
    nodo->next = NULL;
    return nodo;
}

ASTNode* crear_nodo_archivo_leer(char* variable, ASTNode* archivo) {
    ASTNode* nodo = malloc(sizeof(ASTNode));
    nodo->type = NODE_ARCHIVO_LEER;
    nodo->archivo_leer.variable = strdup(variable);
    nodo->archivo_leer.archivo = archivo;
    nodo->next = NULL;
    return nodo;
}

ASTNode* crear_nodo_archivo_escribir(ASTNode* archivo, ASTNode* dato) {
    ASTNode* nodo = malloc(sizeof(ASTNode));
    nodo->type = NODE_ARCHIVO_ESCRIBIR;
    nodo->archivo_escribir.archivo = archivo;
    nodo->archivo_escribir.dato = dato;
    nodo->next = NULL;
    return nodo;
}

ASTNode* crear_nodo_archivo_cerrar(ASTNode* archivo) {
    ASTNode* nodo = malloc(sizeof(ASTNode));
    nodo->type = NODE_ARCHIVO_CERRAR;
    nodo->archivo_cerrar.archivo = archivo;
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

// --- Funciones nuevas ---

ASTNode* crear_nodo_definir(char* nombre, char** parametros, int num_params, ASTNode* cuerpo) {
    ASTNode* nodo = malloc(sizeof(ASTNode));
    nodo->type = NODE_DEFINIR;
    nodo->definir.nombre = strdup(nombre);
    nodo->definir.parametros = parametros;
    nodo->definir.num_params = num_params;
    nodo->definir.cuerpo = cuerpo;
    nodo->next = NULL;
    return nodo;
}

ASTNode* crear_nodo_llamada(char* nombre, ASTNode** args, int num_args) {
    ASTNode* nodo = malloc(sizeof(ASTNode));
    nodo->type = NODE_LLAMADA;
    nodo->llamada.nombre = strdup(nombre);
    nodo->llamada.args = args;
    nodo->llamada.num_args = num_args;
    nodo->next = NULL;
    return nodo;
}

ASTNode* crear_nodo_retornar(ASTNode* expr) {
    ASTNode* nodo = malloc(sizeof(ASTNode));
    nodo->type = NODE_RETORNAR;
    nodo->retornar.expr = expr;
    nodo->next = NULL;
    return nodo;
}

void agregar_nodo_bloque(ASTNode** bloque, ASTNode* nuevo) {
    if(!*bloque) { *bloque = nuevo; return; }
    ASTNode* actual = *bloque;
    while(actual->next) actual = actual->next;
    actual->next = nuevo;
}

void liberar_ast(ASTNode* nodo) {
    if(!nodo) return;
    liberar_ast(nodo->next);
    free(nodo);
}
