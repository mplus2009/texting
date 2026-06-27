%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

void yyerror(const char *s);
extern int yylex();
extern int yylineno;
extern char* yytext;

ASTNode* raiz = NULL;
%}

%union {
    int numero;
    double decimal;
    char* texto;
    struct ASTNode* nodo;
}

%token TOKEN_ESCRIBIR TOKEN_GUARDAR TOKEN_EN TOKEN_LEER
%token TOKEN_SI TOKEN_SINO
%token TOKEN_REPETIR TOKEN_VECES
%token TOKEN_MIENTRAS TOKEN_HACER
%token TOKEN_LISTA
%token TOKEN_ABRIR TOKEN_LEER_ARCHIVO TOKEN_ESCRIBIR_ARCHIVO TOKEN_CERRAR_ARCHIVO
%token TOKEN_NUMERO TOKEN_DECIMAL TOKEN_IDENTIFICADOR TOKEN_STRING
%token TOKEN_MAS TOKEN_MENOS TOKEN_MULTIPLICAR TOKEN_DIVIDIR
%token TOKEN_MODULO TOKEN_POTENCIA
%token TOKEN_MENOR TOKEN_MAYOR TOKEN_MENOR_IGUAL TOKEN_MAYOR_IGUAL
%token TOKEN_IGUALDAD TOKEN_DIFERENTE
%token TOKEN_CONCAT
%token TOKEN_NEWLINE
%token TOKEN_DEFINIR TOKEN_RETORNAR

%type <nodo> programa instruccion escritura asignacion lectura
%type <nodo> condicional bucle_repetir bucle_mientras
%type <nodo> expresion
%type <nodo> lista elementos_lista
%type <nodo> archivo_operacion
%type <nodo> bloque
%type <nodo> opcional_sino
%type <nodo> instrucciones
%type <nodo> definicion
%type <nodo> retorno
%type <nodo> lista_parametros
%type <nodo> lista_argumentos
%type <nodo> llamada_funcion
%type <numero> TOKEN_NUMERO
%type <decimal> TOKEN_DECIMAL
%type <texto> TOKEN_IDENTIFICADOR TOKEN_STRING

%right TOKEN_CONCAT
%left TOKEN_POTENCIA
%left TOKEN_MULTIPLICAR TOKEN_DIVIDIR TOKEN_MODULO
%left TOKEN_MAS TOKEN_MENOS
%left TOKEN_MENOR TOKEN_MAYOR TOKEN_MENOR_IGUAL TOKEN_MAYOR_IGUAL TOKEN_IGUALDAD TOKEN_DIFERENTE
%nonassoc UMINUS

%start programa

%%

programa:
    { raiz = NULL; }
    | programa definicion { agregar_nodo_bloque(&raiz, $2); }
    | programa instruccion ';' { agregar_nodo_bloque(&raiz, $2); }
    | programa instruccion { agregar_nodo_bloque(&raiz, $2); }
    | programa TOKEN_NEWLINE
    | programa error TOKEN_NEWLINE { yyerrok; }
    ;

definicion:
    TOKEN_DEFINIR TOKEN_IDENTIFICADOR '(' lista_parametros ')' bloque {
        char** params = NULL;
        int num_params = 0;
        ASTNode* p = $4;
        if(p) {
            ASTNode* temp = p;
            while(temp) {
                num_params++;
                temp = temp->next;
            }
            params = malloc(num_params * sizeof(char*));
            temp = p;
            int i = 0;
            while(temp) {
                params[i++] = strdup(temp->identificador.nombre);
                temp = temp->next;
            }
        }
        $$ = crear_nodo_definir($2, params, num_params, $6);
    }
    ;

lista_parametros:
    { $$ = NULL; }
    | TOKEN_IDENTIFICADOR { $$ = crear_nodo_identificador($1); }
    | lista_parametros ',' TOKEN_IDENTIFICADOR {
        agregar_nodo_bloque(&$1, crear_nodo_identificador($3));
        $$ = $1;
    }
    ;

instruccion:
    escritura { $$ = $1; }
    | asignacion { $$ = $1; }
    | lectura { $$ = $1; }
    | condicional { $$ = $1; }
    | bucle_repetir { $$ = $1; }
    | bucle_mientras { $$ = $1; }
    | archivo_operacion { $$ = $1; }
    | retorno { $$ = $1; }
    | llamada_funcion { $$ = $1; }   // <--- Ahora llamada_funcion es también instrucción
    ;

escritura:
    TOKEN_ESCRIBIR TOKEN_STRING { $$ = crear_nodo_escritura(crear_nodo_string($2)); }
    | TOKEN_ESCRIBIR expresion { $$ = crear_nodo_escritura($2); }
    ;

asignacion:
    TOKEN_GUARDAR expresion TOKEN_EN TOKEN_IDENTIFICADOR { $$ = crear_nodo_asignacion($4, $2); }
    ;

lectura:
    TOKEN_LEER TOKEN_EN TOKEN_IDENTIFICADOR { $$ = crear_nodo_lectura($3); }
    ;

condicional:
    TOKEN_SI '(' expresion ')' bloque opcional_sino { $$ = crear_nodo_si($3, $5, $6); }
    ;

opcional_sino:
    { $$ = NULL; }
    | TOKEN_SINO bloque { $$ = $2; }
    ;

bloque:
    '{' instrucciones '}' { $$ = $2; }
    ;

instrucciones:
    { $$ = NULL; }
    | instrucciones instruccion ';' { agregar_nodo_bloque(&$1, $2); $$ = $1; }
    | instrucciones instruccion { agregar_nodo_bloque(&$1, $2); $$ = $1; }
    | instrucciones TOKEN_NEWLINE { $$ = $1; }
    ;

bucle_repetir:
    TOKEN_REPETIR TOKEN_NUMERO TOKEN_VECES bloque { $$ = crear_nodo_repetir($2, $4); }
    ;

bucle_mientras:
    TOKEN_MIENTRAS '(' expresion ')' TOKEN_HACER bloque { $$ = crear_nodo_mientras($3, $6); }
    ;

retorno:
    TOKEN_RETORNAR expresion { $$ = crear_nodo_retornar($2); }
    ;

lista_argumentos:
    { $$ = NULL; }
    | expresion { $$ = $1; }
    | lista_argumentos ',' expresion {
        agregar_nodo_bloque(&$1, $3);
        $$ = $1;
    }
    ;

llamada_funcion:
    TOKEN_IDENTIFICADOR '(' lista_argumentos ')' {
        ASTNode** args = NULL;
        int num_args = 0;
        ASTNode* p = $3;
        if(p) {
            ASTNode* temp = p;
            while(temp) {
                num_args++;
                temp = temp->next;
            }
            args = malloc(num_args * sizeof(ASTNode*));
            temp = p;
            int i = 0;
            while(temp) {
                args[i++] = temp;
                temp = temp->next;
            }
        }
        $$ = crear_nodo_llamada($1, args, num_args);
    }
    ;

lista:
    '[' elementos_lista ']' { $$ = $2; }
    | '[' ']' { $$ = crear_nodo_lista(NULL, 0); }
    ;

elementos_lista:
    expresion { 
        ASTNode** arr = malloc(sizeof(ASTNode*));
        arr[0] = $1;
        $$ = crear_nodo_lista(arr, 1);
    }
    | elementos_lista ',' expresion {
        int n = $1->lista.cantidad + 1;
        ASTNode** arr = realloc($1->lista.elementos, n * sizeof(ASTNode*));
        arr[n-1] = $3;
        $1->lista.elementos = arr;
        $1->lista.cantidad = n;
        $$ = $1;
    }
    ;

expresion:
    TOKEN_NUMERO { $$ = crear_nodo_numero($1); }
    | TOKEN_DECIMAL { $$ = crear_nodo_numero($1); }
    | TOKEN_IDENTIFICADOR { $$ = crear_nodo_identificador($1); }
    | TOKEN_IDENTIFICADOR '[' expresion ']' { $$ = crear_nodo_acceso_lista($1, $3); }
    | lista { $$ = $1; }
    | TOKEN_STRING { $$ = crear_nodo_string($1); }
    | llamada_funcion { $$ = $1; }  // <--- Llamada a función como expresión
    | expresion TOKEN_CONCAT expresion { $$ = crear_nodo_operacion('C', $1, $3); }
    | TOKEN_MENOS expresion %prec UMINUS { $$ = crear_nodo_operacion('N', NULL, $2); }
    | expresion TOKEN_MAS expresion { $$ = crear_nodo_operacion('+', $1, $3); }
    | expresion TOKEN_MENOS expresion { $$ = crear_nodo_operacion('-', $1, $3); }
    | expresion TOKEN_MULTIPLICAR expresion { $$ = crear_nodo_operacion('*', $1, $3); }
    | expresion TOKEN_DIVIDIR expresion { $$ = crear_nodo_operacion('/', $1, $3); }
    | expresion TOKEN_MODULO expresion { $$ = crear_nodo_operacion('%', $1, $3); }
    | expresion TOKEN_POTENCIA expresion { $$ = crear_nodo_operacion('^', $1, $3); }
    | expresion TOKEN_MENOR expresion { $$ = crear_nodo_condicion('<', $1, $3); }
    | expresion TOKEN_MAYOR expresion { $$ = crear_nodo_condicion('>', $1, $3); }
    | expresion TOKEN_MENOR_IGUAL expresion { $$ = crear_nodo_condicion('L', $1, $3); }
    | expresion TOKEN_MAYOR_IGUAL expresion { $$ = crear_nodo_condicion('G', $1, $3); }
    | expresion TOKEN_IGUALDAD expresion { $$ = crear_nodo_condicion('=', $1, $3); }
    | expresion TOKEN_DIFERENTE expresion { $$ = crear_nodo_condicion('!', $1, $3); }
    | '(' expresion ')' { $$ = $2; }
    ;

archivo_operacion:
    TOKEN_ABRIR TOKEN_STRING TOKEN_EN TOKEN_IDENTIFICADOR {
        $$ = crear_nodo_archivo_abrir($2, "r");
    }
    | TOKEN_LEER_ARCHIVO TOKEN_IDENTIFICADOR TOKEN_EN TOKEN_IDENTIFICADOR {
        $$ = crear_nodo_archivo_leer($4, crear_nodo_identificador($2));
    }
    | TOKEN_ESCRIBIR_ARCHIVO TOKEN_IDENTIFICADOR ',' expresion {
        $$ = crear_nodo_archivo_escribir(crear_nodo_identificador($2), $4);
    }
    | TOKEN_CERRAR_ARCHIVO TOKEN_IDENTIFICADOR {
        $$ = crear_nodo_archivo_cerrar(crear_nodo_identificador($2));
    }
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Error linea %d: %s cerca de '%s'\n", yylineno, s, yytext);
}
