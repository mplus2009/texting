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
%token TOKEN_SI TOKEN_ENTONCES TOKEN_SINO TOKEN_FIN
%token TOKEN_REPETIR TOKEN_VECES
%token TOKEN_MIENTRAS TOKEN_HACER
%token TOKEN_NUMERO TOKEN_DECIMAL TOKEN_IDENTIFICADOR TOKEN_STRING
%token TOKEN_MAS TOKEN_MENOS TOKEN_MULTIPLICAR TOKEN_DIVIDIR
%token TOKEN_MENOR TOKEN_MAYOR TOKEN_MENOR_IGUAL TOKEN_MAYOR_IGUAL TOKEN_IGUALDAD
%token TOKEN_NEWLINE

%type <nodo> programa instruccion escritura asignacion lectura
%type <nodo> condicional bucle_repetir bucle_mientras
%type <nodo> expresion condicion bloque opcional_sino
%type <numero> TOKEN_NUMERO
%type <decimal> TOKEN_DECIMAL
%type <texto> TOKEN_IDENTIFICADOR TOKEN_STRING

%left TOKEN_MAS TOKEN_MENOS
%left TOKEN_MULTIPLICAR TOKEN_DIVIDIR
%nonassoc UMINUS

%start programa

%%

programa:
    { raiz = NULL; }
    | programa linea
    ;

linea:
    instruccion { if(!raiz) raiz = $1; else { ASTNode* n = raiz; while(n->next) n = n->next; n->next = $1; } }
    | TOKEN_NEWLINE
    ;

instruccion:
    escritura ';' { $$ = $1; }
    | asignacion ';' { $$ = $1; }
    | lectura ';' { $$ = $1; }
    | condicional { $$ = $1; }
    | bucle_repetir { $$ = $1; }
    | bucle_mientras { $$ = $1; }
    ;

escritura:
    TOKEN_ESCRIBIR TOKEN_EN TOKEN_IDENTIFICADOR expresion { $$ = crear_nodo_escritura($4); }
    | TOKEN_ESCRIBIR TOKEN_EN TOKEN_IDENTIFICADOR TOKEN_STRING { $$ = crear_nodo_string($4); }
    ;

asignacion:
    TOKEN_GUARDAR expresion TOKEN_EN TOKEN_IDENTIFICADOR { $$ = crear_nodo_asignacion($4, $2); }
    ;

lectura:
    TOKEN_LEER TOKEN_IDENTIFICADOR TOKEN_EN TOKEN_IDENTIFICADOR { $$ = crear_nodo_lectura($4); }
    ;

condicion:
    expresion TOKEN_MENOR expresion { $$ = crear_nodo_condicion('<', $1, $3); }
    | expresion TOKEN_MAYOR expresion { $$ = crear_nodo_condicion('>', $1, $3); }
    | expresion TOKEN_MENOR_IGUAL expresion { $$ = crear_nodo_condicion('L', $1, $3); }
    | expresion TOKEN_MAYOR_IGUAL expresion { $$ = crear_nodo_condicion('G', $1, $3); }
    | expresion TOKEN_IGUALDAD expresion { $$ = crear_nodo_condicion('=', $1, $3); }
    ;

condicional:
    TOKEN_SI condicion TOKEN_ENTONCES bloque opcional_sino TOKEN_FIN { $$ = crear_nodo_si($2, $4, $5); }
    ;

opcional_sino:
    /* vacío */ { $$ = NULL; }
    | TOKEN_SINO bloque { $$ = $2; }
    ;

bloque:
    instruccion { $$ = $1; }
    | bloque instruccion { agregar_nodo_bloque(&$1, $2); $$ = $1; }
    | bloque TOKEN_NEWLINE { $$ = $1; }
    ;

bucle_repetir:
    TOKEN_REPETIR TOKEN_NUMERO TOKEN_VECES bloque TOKEN_FIN { $$ = crear_nodo_repetir($2, $4); }
    ;

bucle_mientras:
    TOKEN_MIENTRAS condicion TOKEN_HACER bloque TOKEN_FIN { $$ = crear_nodo_mientras($2, $4); }
    ;

expresion:
    TOKEN_NUMERO { $$ = crear_nodo_numero($1); }
    | TOKEN_DECIMAL { $$ = crear_nodo_numero($1); }
    | TOKEN_IDENTIFICADOR { $$ = crear_nodo_identificador($1); }
    | TOKEN_MENOS expresion %prec UMINUS { $$ = crear_nodo_operacion('N', NULL, $2); }
    | expresion TOKEN_MAS expresion { $$ = crear_nodo_operacion('+', $1, $3); }
    | expresion TOKEN_MENOS expresion { $$ = crear_nodo_operacion('-', $1, $3); }
    | expresion TOKEN_MULTIPLICAR expresion { $$ = crear_nodo_operacion('*', $1, $3); }
    | expresion TOKEN_DIVIDIR expresion { $$ = crear_nodo_operacion('/', $1, $3); }
    | '(' expresion ')' { $$ = $2; }
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Error linea %d: %s cerca de '%s'\n", yylineno, s, yytext);
}
