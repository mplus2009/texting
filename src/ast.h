#ifndef AST_H
#define AST_H

typedef enum {
    NODE_PROGRAMA,
    NODE_ESCRITURA,
    NODE_ASIGNACION,
    NODE_LECTURA,
    NODE_SI,
    NODE_REPETIR,
    NODE_MIENTRAS,
    NODE_LISTA,
    NODE_ACCESO_LISTA,
    NODE_ARCHIVO_ABRIR,
    NODE_ARCHIVO_LEER,
    NODE_ARCHIVO_ESCRIBIR,
    NODE_ARCHIVO_CERRAR,
    NODE_NUMERO,
    NODE_DECIMAL,
    NODE_STRING,
    NODE_IDENTIFICADOR,
    NODE_OPERACION,
    NODE_CONDICION,
    NODE_BLOQUE,
    // Nuevos nodos para funciones
    NODE_DEFINIR,
    NODE_LLAMADA,
    NODE_RETORNAR
} NodeType;

typedef struct ASTNode {
    NodeType type;
    struct ASTNode* next;
    union {
        struct { struct ASTNode* expr; } escritura;
        struct { char* nombre; struct ASTNode* valor; } asignacion;
        struct { char* nombre; } lectura;
        struct { struct ASTNode* condicion; struct ASTNode* bloque_si; struct ASTNode* bloque_sino; } si;
        struct { int veces; struct ASTNode* bloque; } repetir;
        struct { struct ASTNode* condicion; struct ASTNode* bloque; } mientras;
        struct { struct ASTNode** elementos; int cantidad; } lista;
        struct { char* nombre; struct ASTNode* indice; } acceso_lista;
        struct { char* nombre_archivo; char* modo; } archivo_abrir;
        struct { char* variable; struct ASTNode* archivo; } archivo_leer;
        struct { struct ASTNode* archivo; struct ASTNode* dato; } archivo_escribir;
        struct { struct ASTNode* archivo; } archivo_cerrar;
        struct { double numero; } numero;
        struct { double decimal; } decimal;
        struct { char* texto; } string;
        struct { char* nombre; } identificador;
        struct { char operador; struct ASTNode* izquierda; struct ASTNode* derecha; } operacion;
        struct { char operador; struct ASTNode* izquierda; struct ASTNode* derecha; } condicion;
        // Funciones
        struct { 
            char* nombre; 
            char** parametros; 
            int num_params; 
            struct ASTNode* cuerpo; 
        } definir;
        struct { 
            char* nombre; 
            struct ASTNode** args; 
            int num_args; 
        } llamada;
        struct { 
            struct ASTNode* expr; 
        } retornar;
    };
} ASTNode;

ASTNode* crear_nodo_programa(ASTNode* primera);
ASTNode* crear_nodo_escritura(ASTNode* expr);
ASTNode* crear_nodo_asignacion(char* nombre, ASTNode* valor);
ASTNode* crear_nodo_lectura(char* nombre);
ASTNode* crear_nodo_si(ASTNode* cond, ASTNode* bloque_si, ASTNode* bloque_sino);
ASTNode* crear_nodo_repetir(int veces, ASTNode* bloque);
ASTNode* crear_nodo_mientras(ASTNode* cond, ASTNode* bloque);
ASTNode* crear_nodo_lista(ASTNode** elementos, int cantidad);
ASTNode* crear_nodo_acceso_lista(char* nombre, ASTNode* indice);
ASTNode* crear_nodo_archivo_abrir(char* nombre, char* modo);
ASTNode* crear_nodo_archivo_leer(char* variable, ASTNode* archivo);
ASTNode* crear_nodo_archivo_escribir(ASTNode* archivo, ASTNode* dato);
ASTNode* crear_nodo_archivo_cerrar(ASTNode* archivo);
ASTNode* crear_nodo_numero(double num);
ASTNode* crear_nodo_string(char* texto);
ASTNode* crear_nodo_identificador(char* nombre);
ASTNode* crear_nodo_operacion(char op, ASTNode* izq, ASTNode* der);
ASTNode* crear_nodo_condicion(char op, ASTNode* izq, ASTNode* der);
// Funciones
ASTNode* crear_nodo_definir(char* nombre, char** parametros, int num_params, ASTNode* cuerpo);
ASTNode* crear_nodo_llamada(char* nombre, ASTNode** args, int num_args);
ASTNode* crear_nodo_retornar(ASTNode* expr);
void agregar_nodo_bloque(ASTNode** bloque, ASTNode* nuevo);
void liberar_ast(ASTNode* nodo);

#endif
