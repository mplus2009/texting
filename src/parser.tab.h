/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_SRC_PARSER_TAB_H_INCLUDED
# define YY_YY_SRC_PARSER_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    TOKEN_ESCRIBIR = 258,          /* TOKEN_ESCRIBIR  */
    TOKEN_GUARDAR = 259,           /* TOKEN_GUARDAR  */
    TOKEN_EN = 260,                /* TOKEN_EN  */
    TOKEN_LEER = 261,              /* TOKEN_LEER  */
    TOKEN_SI = 262,                /* TOKEN_SI  */
    TOKEN_SINO = 263,              /* TOKEN_SINO  */
    TOKEN_REPETIR = 264,           /* TOKEN_REPETIR  */
    TOKEN_VECES = 265,             /* TOKEN_VECES  */
    TOKEN_MIENTRAS = 266,          /* TOKEN_MIENTRAS  */
    TOKEN_HACER = 267,             /* TOKEN_HACER  */
    TOKEN_LISTA = 268,             /* TOKEN_LISTA  */
    TOKEN_ABRIR = 269,             /* TOKEN_ABRIR  */
    TOKEN_LEER_ARCHIVO = 270,      /* TOKEN_LEER_ARCHIVO  */
    TOKEN_ESCRIBIR_ARCHIVO = 271,  /* TOKEN_ESCRIBIR_ARCHIVO  */
    TOKEN_CERRAR_ARCHIVO = 272,    /* TOKEN_CERRAR_ARCHIVO  */
    TOKEN_NUMERO = 273,            /* TOKEN_NUMERO  */
    TOKEN_DECIMAL = 274,           /* TOKEN_DECIMAL  */
    TOKEN_IDENTIFICADOR = 275,     /* TOKEN_IDENTIFICADOR  */
    TOKEN_STRING = 276,            /* TOKEN_STRING  */
    TOKEN_MAS = 277,               /* TOKEN_MAS  */
    TOKEN_MENOS = 278,             /* TOKEN_MENOS  */
    TOKEN_MULTIPLICAR = 279,       /* TOKEN_MULTIPLICAR  */
    TOKEN_DIVIDIR = 280,           /* TOKEN_DIVIDIR  */
    TOKEN_MODULO = 281,            /* TOKEN_MODULO  */
    TOKEN_POTENCIA = 282,          /* TOKEN_POTENCIA  */
    TOKEN_MENOR = 283,             /* TOKEN_MENOR  */
    TOKEN_MAYOR = 284,             /* TOKEN_MAYOR  */
    TOKEN_MENOR_IGUAL = 285,       /* TOKEN_MENOR_IGUAL  */
    TOKEN_MAYOR_IGUAL = 286,       /* TOKEN_MAYOR_IGUAL  */
    TOKEN_IGUALDAD = 287,          /* TOKEN_IGUALDAD  */
    TOKEN_DIFERENTE = 288,         /* TOKEN_DIFERENTE  */
    TOKEN_CONCAT = 289,            /* TOKEN_CONCAT  */
    TOKEN_NEWLINE = 290,           /* TOKEN_NEWLINE  */
    TOKEN_DEFINIR = 291,           /* TOKEN_DEFINIR  */
    TOKEN_RETORNAR = 292,          /* TOKEN_RETORNAR  */
    UMINUS = 293                   /* UMINUS  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 15 "src/parser.y"

    int numero;
    double decimal;
    char* texto;
    struct ASTNode* nodo;

#line 109 "src/parser.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_SRC_PARSER_TAB_H_INCLUDED  */
