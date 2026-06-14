%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "interpreter.h"

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
%token TOKEN_MENOR TOKEN_MAYOR

%type <numero> TOKEN_NUMERO
%type <decimal> TOKEN_DECIMAL
%type <texto> TOKEN_IDENTIFICADOR TOKEN_STRING
%type <nodo> programa instruccion escritura asignacion lectura
%type <nodo> condicional bucle_repetir bucle_mientras
%type <nodo> expresion condicion
%type <nodo> bloque_si opcional_sino

%left TOKEN_MAS TOKEN_MENOS
%left TOKEN_MULTIPLICAR TOKEN_DIVIDIR
%nonassoc UMINUS

%start programa

%%

programa:
    { raiz = NULL; }
    | programa instruccion { agregar_nodo_bloque(&raiz, $2); }
    ;

instruccion:
    escritura { $$ = $1; }
    | asignacion { $$ = $1; }
    | lectura { $$ = $1; }
    | condicional { $$ = $1; }
    | bucle_repetir { $$ = $1; }
    | bucle_mientras { $$ = $1; }
    ;

escritura:
    TOKEN_ESCRIBIR expresion ';' {
        $$ = crear_nodo_escritura($2);
    }
    | TOKEN_ESCRIBIR TOKEN_STRING ';' {
        $$ = crear_nodo_escritura(crear_nodo_string($2));
    }
    ;

asignacion:
    TOKEN_GUARDAR expresion TOKEN_EN TOKEN_IDENTIFICADOR ';' {
        $$ = crear_nodo_asignacion($4, $2);
    }
    ;

lectura:
    TOKEN_LEER TOKEN_EN TOKEN_IDENTIFICADOR ';' {
        $$ = crear_nodo_lectura($3);
    }
    ;

condicion:
    expresion TOKEN_MENOR expresion { $$ = crear_nodo_condicion('<', $1, $3); }
    | expresion TOKEN_MAYOR expresion { $$ = crear_nodo_condicion('>', $1, $3); }
    ;

condicional:
    TOKEN_SI condicion TOKEN_ENTONCES bloque_si opcional_sino TOKEN_FIN ';' {
        $$ = crear_nodo_si($2, $4, $5);
    }
    ;

opcional_sino:
    { $$ = NULL; }
    | TOKEN_SINO bloque_si { $$ = $2; }
    ;

bloque_si:
    instruccion { $$ = $1; }
    | bloque_si instruccion { agregar_nodo_bloque(&$1, $2); $$ = $1; }
    ;

bucle_repetir:
    TOKEN_REPETIR TOKEN_NUMERO TOKEN_VECES bloque_si TOKEN_FIN ';' {
        $$ = crear_nodo_repetir($2, $4);
    }
    ;

bucle_mientras:
    TOKEN_MIENTRAS condicion TOKEN_HACER bloque_si TOKEN_FIN ';' {
        $$ = crear_nodo_mientras($2, $4);
    }
    ;

expresion:
    TOKEN_NUMERO { $$ = crear_nodo_numero($1); }
    | TOKEN_DECIMAL { $$ = crear_nodo_numero($1); }
    | TOKEN_IDENTIFICADOR { $$ = crear_nodo_identificador($1); }
    | TOKEN_MENOS expresion %prec UMINUS { 
        $$ = crear_nodo_operacion('N', NULL, $2); 
    }
    | expresion TOKEN_MAS expresion { $$ = crear_nodo_operacion('+', $1, $3); }
    | expresion TOKEN_MENOS expresion { $$ = crear_nodo_operacion('-', $1, $3); }
    | expresion TOKEN_MULTIPLICAR expresion { $$ = crear_nodo_operacion('*', $1, $3); }
    | expresion TOKEN_DIVIDIR expresion { $$ = crear_nodo_operacion('/', $1, $3); }
    | '(' expresion ')' { $$ = $2; }
    ;

%%

int main(int argc, char **argv) {
    if(argc > 1) {
        FILE *f = fopen(argv[1], "r");
        if(!f) {
            fprintf(stderr, "No se puede abrir %s\n", argv[1]);
            return 1;
        }
        extern FILE *yyin;
        yyin = f;
    }
    
    printf("--- Texting ---\n");
    init_interpreter();
    yyparse();
    
    if(raiz) {
        ejecutar_programa(raiz);
        liberar_ast(raiz);
        liberar_variables();
    }
    
    printf("--- Fin ---\n");
    return 0;
}

void yyerror(const char *s) {
    fprintf(stderr, "Error linea %d: %s cerca de '%s'\n", yylineno, s, yytext);
}
