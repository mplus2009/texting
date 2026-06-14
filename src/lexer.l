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
"entonces"              { return TOKEN_ENTONCES; }
"sino"                  { return TOKEN_SINO; }
"fin"                   { return TOKEN_FIN; }
"repetir"               { return TOKEN_REPETIR; }
"veces"                 { return TOKEN_VECES; }
"mientras"              { return TOKEN_MIENTRAS; }
"hacer"                 { return TOKEN_HACER; }
[0-9]+                  { yylval.numero = atoi(yytext); return TOKEN_NUMERO; }
[0-9]+\.[0-9]+          { yylval.decimal = atof(yytext); return TOKEN_DECIMAL; }
[a-zA-Z_][a-zA-Z0-9_]*  { yylval.texto = strdup(yytext); return TOKEN_IDENTIFICADOR; }
\"[^\"]*\"              { yytext[strlen(yytext)-1] = '\0'; yylval.texto = strdup(yytext+1); return TOKEN_STRING; }
"+"                     { return TOKEN_MAS; }
"*"                     { return TOKEN_MULTIPLICAR; }
"/"                     { return TOKEN_DIVIDIR; }
"-"                     { return TOKEN_MENOS; }
"<"                     { return TOKEN_MENOR; }
">"                     { return TOKEN_MAYOR; }
"("                     { return '('; }
")"                     { return ')'; }
";"                     { return ';'; }
"#"                     { while(input() != '\n'); }
"\n"                    { /* ignorar */ }
[ \t\r]                 { /* ignorar */ }
.                       { fprintf(stderr, "Caracter no reconocido: %s\n", yytext); }

%%
