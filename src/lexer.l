%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.tab.h"
extern int yylineno;
%}

%option yylineno
%option noyywrap
%option caseless

%%

"escribir en consola"   { return TOKEN_ESCRIBIR; }
"guardar"               { return TOKEN_GUARDAR; }
"en"                    { return TOKEN_EN; }
"leer entrada"          { return TOKEN_LEER; }
"si"                    { return TOKEN_SI; }
"sino"                  { return TOKEN_SINO; }
"repetir"               { return TOKEN_REPETIR; }
"veces"                 { return TOKEN_VECES; }
"mientras"              { return TOKEN_MIENTRAS; }
"hacer"                 { return TOKEN_HACER; }
"lista"                 { return TOKEN_LISTA; }
"abrir"                 { return TOKEN_ABRIR; }
"leer_archivo"          { return TOKEN_LEER_ARCHIVO; }
"escribir_archivo"      { return TOKEN_ESCRIBIR_ARCHIVO; }
"cerrar_archivo"        { return TOKEN_CERRAR_ARCHIVO; }
"mayor"                 { return TOKEN_MAYOR; }
"menor"                 { return TOKEN_MENOR; }
"igual"                 { return TOKEN_IGUALDAD; }
"diferente"             { return TOKEN_DIFERENTE; }
"mayor_igual"           { return TOKEN_MAYOR_IGUAL; }
"menor_igual"           { return TOKEN_MENOR_IGUAL; }
"definir"               { return TOKEN_DEFINIR; }
"retornar"              { return TOKEN_RETORNAR; }
"es"                    { /* ignorar */ }
"que"                   { /* ignorar */ }
[0-9]+                  { yylval.numero = atoi(yytext); return TOKEN_NUMERO; }
[0-9]+\.[0-9]+          { yylval.decimal = atof(yytext); return TOKEN_DECIMAL; }
[a-zA-Z_][a-zA-Z0-9_]*  { yylval.texto = strdup(yytext); return TOKEN_IDENTIFICADOR; }
\"[^\"]*\"              { yytext[strlen(yytext)-1] = '\0'; yylval.texto = strdup(yytext+1); return TOKEN_STRING; }
"."                     { 
    char c = input();
    if(c == 'y' || c == 'Y') {
        return TOKEN_CONCAT;
    } else {
        unput(c);
        return '.';
    }
}
"&"                     { return TOKEN_CONCAT; }
"+"                     { return TOKEN_MAS; }
"-"                     { return TOKEN_MENOS; }
"*"                     { return TOKEN_MULTIPLICAR; }
"/"                     { return TOKEN_DIVIDIR; }
"%"                     { return TOKEN_MODULO; }
"^"                     { return TOKEN_POTENCIA; }
"=="                    { return TOKEN_IGUALDAD; }
"!="                    { return TOKEN_DIFERENTE; }
"<="                    { return TOKEN_MENOR_IGUAL; }
">="                    { return TOKEN_MAYOR_IGUAL; }
"<"                     { return TOKEN_MENOR; }
">"                     { return TOKEN_MAYOR; }
"["                     { return '['; }
"]"                     { return ']'; }
","                     { return ','; }
";"                     { return ';'; }
"{"                     { return '{'; }
"}"                     { return '}'; }
"("                     { return '('; }
")"                     { return ')'; }
"#"                     { while(input() != '\n'); }
"//"                    { while(input() != '\n'); }
[ \t]+                  { /* ignorar espacios */ }
"\n"                    { return TOKEN_NEWLINE; }
.                       { fprintf(stderr, "Caracter no reconocido: %s\n", yytext); }

%%
