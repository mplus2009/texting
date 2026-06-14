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
    NODE_DESDE,
    NODE_DEFINIR,
    NODE_RETORNAR,
    NODE_LLAMADA,
    NODE_NUMERO,
    NODE_DECIMAL,
    NODE_STRING,
    NODE_IDENTIFICADOR,
    NODE_OPERACION,
    NODE_CONDICION,
    NODE_BLOQUE
} NodeType;

typedef struct ASTNode {
    NodeType type;
    struct ASTNode* next;
    union {
        struct {
            struct ASTNode* expr;
        } escritura;
        struct {
            char* nombre;
            struct ASTNode* valor;
        } asignacion;
        struct {
            char* nombre;
        } lectura;
        struct {
            struct ASTNode* condicion;
            struct ASTNode* bloque_si;
            struct ASTNode* bloque_sino;
        } si;
        struct {
            int veces;
            struct ASTNode* bloque;
        } repetir;
        struct {
            struct ASTNode* condicion;
            struct ASTNode* bloque;
        } mientras;
        struct {
            char* variable;
            struct ASTNode* inicio;
            struct ASTNode* fin;
            struct ASTNode* paso;
            struct ASTNode* bloque;
        } desde;
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
            double numero;
        } numero;
        struct {
            double decimal;
        } decimal;
        struct {
            char* texto;
        } string;
        struct {
            char* nombre;
        } identificador;
        struct {
            char operador;
            struct ASTNode* izquierda;
            struct ASTNode* derecha;
        } operacion;
        struct {
            char operador;
            struct ASTNode* izquierda;
            struct ASTNode* derecha;
        } condicion;
    };
} ASTNode;

ASTNode* crear_nodo_programa(ASTNode* primera);
ASTNode* crear_nodo_escritura(ASTNode* expr);
ASTNode* crear_nodo_asignacion(char* nombre, ASTNode* valor);
ASTNode* crear_nodo_lectura(char* nombre);
ASTNode* crear_nodo_si(ASTNode* cond, ASTNode* bloque_si, ASTNode* bloque_sino);
ASTNode* crear_nodo_repetir(int veces, ASTNode* bloque);
ASTNode* crear_nodo_mientras(ASTNode* cond, ASTNode* bloque);
ASTNode* crear_nodo_numero(double num);
ASTNode* crear_nodo_string(char* texto);
ASTNode* crear_nodo_identificador(char* nombre);
ASTNode* crear_nodo_operacion(char op, ASTNode* izq, ASTNode* der);
ASTNode* crear_nodo_condicion(char op, ASTNode* izq, ASTNode* der);
void agregar_nodo_bloque(ASTNode** bloque, ASTNode* nuevo);
void liberar_ast(ASTNode* nodo);

#endif
