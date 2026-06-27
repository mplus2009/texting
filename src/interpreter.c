#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "interpreter.h"

Variable* variables = NULL;
Funcion* funciones = NULL;

// Estructura para lista
typedef struct Lista {
    int capacidad;
    int cantidad;
    double* elementos;
} Lista;

// Estructura para archivo
typedef struct Archivo {
    FILE* fp;
    char* nombre;
} Archivo;

// ---- Variables ----
Variable* buscar_variable(char* nombre) {
    Variable* v = variables;
    while(v) {
        if(strcasecmp(v->nombre, nombre) == 0) return v;
        v = v->siguiente;
    }
    return NULL;
}

void guardar_variable_entero(char* nombre, int valor) {
    Variable* v = buscar_variable(nombre);
    if(!v) {
        v = malloc(sizeof(Variable));
        v->nombre = strdup(nombre);
        v->siguiente = variables;
        variables = v;
    }
    v->tipo = 0;
    v->valor.entero = valor;
}

void guardar_variable_decimal(char* nombre, double valor) {
    Variable* v = buscar_variable(nombre);
    if(!v) {
        v = malloc(sizeof(Variable));
        v->nombre = strdup(nombre);
        v->siguiente = variables;
        variables = v;
    }
    v->tipo = 2;
    v->valor.decimal = valor;
}

void guardar_variable_texto(char* nombre, char* valor) {
    Variable* v = buscar_variable(nombre);
    if(!v) {
        v = malloc(sizeof(Variable));
        v->nombre = strdup(nombre);
        v->siguiente = variables;
        variables = v;
    }
    v->tipo = 1;
    if(v->valor.texto) free(v->valor.texto);
    v->valor.texto = strdup(valor);
}

void guardar_variable_lista(char* nombre, void* lista) {
    Variable* v = buscar_variable(nombre);
    if(!v) {
        v = malloc(sizeof(Variable));
        v->nombre = strdup(nombre);
        v->siguiente = variables;
        variables = v;
    }
    v->tipo = 3;
    v->valor.texto = (char*)lista;
}

void guardar_variable_archivo(char* nombre, void* archivo) {
    Variable* v = buscar_variable(nombre);
    if(!v) {
        v = malloc(sizeof(Variable));
        v->nombre = strdup(nombre);
        v->siguiente = variables;
        variables = v;
    }
    v->tipo = 4;
    v->valor.texto = (char*)archivo;
}

// ---- Funciones ----
Funcion* buscar_funcion(char* nombre) {
    Funcion* f = funciones;
    while(f) {
        if(strcasecmp(f->nombre, nombre) == 0) return f;
        f = f->siguiente;
    }
    return NULL;
}

// ---- Evaluación ----
double obtener_valor_numerico(ASTNode* expr) {
    if(!expr) return 0;
    
    if(expr->type == NODE_NUMERO) return expr->numero.numero;
    if(expr->type == NODE_DECIMAL) return expr->decimal.decimal;
    
    if(expr->type == NODE_IDENTIFICADOR) {
        Variable* v = buscar_variable(expr->identificador.nombre);
        if(!v) return 0;
        if(v->tipo == 0) return v->valor.entero;
        if(v->tipo == 2) return v->valor.decimal;
        return 0;
    }
    
    if(expr->type == NODE_LLAMADA) {
        // Buscar la función
        Funcion* f = buscar_funcion(expr->llamada.nombre);
        if(!f) {
            fprintf(stderr, "Error: función '%s' no definida\n", expr->llamada.nombre);
            return 0;
        }
        
        // Guardar variables actuales
        Variable* vars_guardadas = variables;
        variables = NULL;
        
        // Asignar parámetros
        int num_params = f->num_params < expr->llamada.num_args ? f->num_params : expr->llamada.num_args;
        for(int i = 0; i < num_params; i++) {
            double val = obtener_valor_numerico(expr->llamada.args[i]);
            guardar_variable_entero(f->parametros[i], (int)val);
        }
        
        // Ejecutar cuerpo y capturar retorno
        double retorno = 0;
        ASTNode* actual = f->cuerpo;
        while(actual) {
            if(actual->type == NODE_RETORNAR) {
                retorno = obtener_valor_numerico(actual->retornar.expr);
                break;
            }
            // Ejecutar otras instrucciones
            if(actual->type == NODE_ESCRITURA) {
                ASTNode* expr2 = actual->escritura.expr;
                char* texto = obtener_valor_texto(expr2);
                printf("%s\n", texto);
            }
            else if(actual->type == NODE_ASIGNACION) {
                double val = obtener_valor_numerico(actual->asignacion.valor);
                guardar_variable_entero(actual->asignacion.nombre, (int)val);
            }
            actual = actual->next;
        }
        
        // Restaurar variables
        liberar_variables();
        variables = vars_guardadas;
        
        return retorno;
    }
    
    if(expr->type == NODE_CONDICION) {
        double izq = obtener_valor_numerico(expr->condicion.izquierda);
        double der = obtener_valor_numerico(expr->condicion.derecha);
        switch(expr->condicion.operador) {
            case '<': return izq < der;
            case '>': return izq > der;
            case 'L': return izq <= der;
            case 'G': return izq >= der;
            case '=': return izq == der;
            case '!': return izq != der;
            default: return 0;
        }
    }
    
    if(expr->type == NODE_OPERACION) {
        double izq = obtener_valor_numerico(expr->operacion.izquierda);
        double der = obtener_valor_numerico(expr->operacion.derecha);
        switch(expr->operacion.operador) {
            case '+': return izq + der;
            case '-': return izq - der;
            case '*': return izq * der;
            case '/': return der != 0 ? izq / der : 0;
            case '%': return (int)izq % (int)der;
            case '^': return pow(izq, der);
            case 'N': return -izq;
            case 'C': return 0;
            default: return 0;
        }
    }
    
    return 0;
}

char* obtener_valor_texto(ASTNode* expr) {
    static char buffer[2048];
    if(!expr) return "";
    
    if(expr->type == NODE_STRING) return expr->string.texto;
    
    if(expr->type == NODE_IDENTIFICADOR) {
        Variable* v = buscar_variable(expr->identificador.nombre);
        if(!v) return "";
        if(v->tipo == 1) return v->valor.texto;
        if(v->tipo == 0) {
            sprintf(buffer, "%d", v->valor.entero);
            return buffer;
        }
        if(v->tipo == 2) {
            sprintf(buffer, "%g", v->valor.decimal);
            return buffer;
        }
        if(v->tipo == 3) {
            Lista* l = (Lista*)v->valor.texto;
            sprintf(buffer, "[lista %d elementos]", l->cantidad);
            return buffer;
        }
        return "";
    }
    
    if(expr->type == NODE_OPERACION && expr->operacion.operador == 'C') {
        char* izq = obtener_valor_texto(expr->operacion.izquierda);
        char* der = obtener_valor_texto(expr->operacion.derecha);
        snprintf(buffer, sizeof(buffer), "%s%s", izq, der);
        return buffer;
    }
    
    double num = obtener_valor_numerico(expr);
    if(num == (int)num) {
        sprintf(buffer, "%d", (int)num);
    } else {
        sprintf(buffer, "%g", num);
    }
    return buffer;
}

int evaluar_condicion(ASTNode* cond) {
    if(!cond) return 0;
    if(cond->type == NODE_CONDICION) {
        double izq = obtener_valor_numerico(cond->condicion.izquierda);
        double der = obtener_valor_numerico(cond->condicion.derecha);
        switch(cond->condicion.operador) {
            case '<': return izq < der;
            case '>': return izq > der;
            case 'L': return izq <= der;
            case 'G': return izq >= der;
            case '=': return izq == der;
            case '!': return izq != der;
            default: return 0;
        }
    }
    return 0;
}

// ---- Ejecución de bloques ----
void ejecutar_bloque(ASTNode* nodo) {
    ASTNode* actual = nodo;
    while(actual) {
        if(actual->type == NODE_ESCRITURA) {
            ASTNode* expr = actual->escritura.expr;
            char* texto = obtener_valor_texto(expr);
            printf("%s\n", texto);
        }
        else if(actual->type == NODE_ASIGNACION) {
            double val = obtener_valor_numerico(actual->asignacion.valor);
            if(val == (int)val) {
                guardar_variable_entero(actual->asignacion.nombre, (int)val);
            } else {
                guardar_variable_decimal(actual->asignacion.nombre, val);
            }
        }
        else if(actual->type == NODE_LECTURA) {
            char entrada[100];
            printf("> ");
            fgets(entrada, 100, stdin);
            entrada[strcspn(entrada, "\n")] = 0;
            char* end;
            double num = strtod(entrada, &end);
            if(*end == '\0') {
                guardar_variable_entero(actual->lectura.nombre, (int)num);
            } else {
                guardar_variable_texto(actual->lectura.nombre, entrada);
            }
        }
        else if(actual->type == NODE_SI) {
            if(evaluar_condicion(actual->si.condicion)) {
                ejecutar_bloque(actual->si.bloque_si);
            } else if(actual->si.bloque_sino) {
                ejecutar_bloque(actual->si.bloque_sino);
            }
        }
        else if(actual->type == NODE_REPETIR) {
            for(int i = 0; i < actual->repetir.veces; i++) {
                ejecutar_bloque(actual->repetir.bloque);
            }
        }
        else if(actual->type == NODE_MIENTRAS) {
            while(evaluar_condicion(actual->mientras.condicion)) {
                ejecutar_bloque(actual->mientras.bloque);
            }
        }
        else if(actual->type == NODE_DEFINIR) {
            Funcion* f = malloc(sizeof(Funcion));
            f->nombre = strdup(actual->definir.nombre);
            f->parametros = actual->definir.parametros;
            f->num_params = actual->definir.num_params;
            f->cuerpo = actual->definir.cuerpo;
            f->siguiente = funciones;
            funciones = f;
        }
        else if(actual->type == NODE_LLAMADA) {
            // Llamada a función como instrucción (sin retorno)
            obtener_valor_numerico(actual);
        }
        else if(actual->type == NODE_RETORNAR) {
            // El retorno se maneja en obtener_valor_numerico para NODE_LLAMADA
            return;
        }
        else if(actual->type == NODE_ARCHIVO_ABRIR) {
            Archivo* a = malloc(sizeof(Archivo));
            a->nombre = strdup(actual->archivo_abrir.nombre_archivo);
            a->fp = fopen(actual->archivo_abrir.nombre_archivo, "r");
            if(!a->fp) {
                fprintf(stderr, "Error: No se puede abrir %s\n", actual->archivo_abrir.nombre_archivo);
                free(a);
            } else {
                guardar_variable_archivo(actual->archivo_abrir.nombre_archivo, a);
            }
        }
        else if(actual->type == NODE_ARCHIVO_LEER) {
            Variable* v = buscar_variable(actual->archivo_leer.archivo->identificador.nombre);
            if(v && v->tipo == 4) {
                Archivo* a = (Archivo*)v->valor.texto;
                char linea[1024];
                if(fgets(linea, sizeof(linea), a->fp)) {
                    linea[strcspn(linea, "\n")] = 0;
                    guardar_variable_texto(actual->archivo_leer.variable, linea);
                } else {
                    guardar_variable_texto(actual->archivo_leer.variable, "");
                }
            }
        }
        else if(actual->type == NODE_ARCHIVO_ESCRIBIR) {
            Variable* v = buscar_variable(actual->archivo_escribir.archivo->identificador.nombre);
            if(v && v->tipo == 4) {
                Archivo* a = (Archivo*)v->valor.texto;
                char* texto = obtener_valor_texto(actual->archivo_escribir.dato);
                fprintf(a->fp, "%s\n", texto);
                fflush(a->fp);
            }
        }
        else if(actual->type == NODE_ARCHIVO_CERRAR) {
            Variable* v = buscar_variable(actual->archivo_cerrar.archivo->identificador.nombre);
            if(v && v->tipo == 4) {
                Archivo* a = (Archivo*)v->valor.texto;
                fclose(a->fp);
                free(a->nombre);
                free(a);
                v->tipo = 0;
                v->valor.entero = 0;
            }
        }
        actual = actual->next;
    }
}

// ---- Funciones principales ----
void ejecutar_programa(ASTNode* programa) {
    ejecutar_bloque(programa);
}

void liberar_variables() {
    Variable* v = variables;
    while(v) {
        Variable* sig = v->siguiente;
        free(v->nombre);
        if(v->tipo == 1) free(v->valor.texto);
        if(v->tipo == 3) {
            Lista* l = (Lista*)v->valor.texto;
            free(l->elementos);
            free(l);
        }
        if(v->tipo == 4) {
            Archivo* a = (Archivo*)v->valor.texto;
            if(a->fp) fclose(a->fp);
            free(a->nombre);
            free(a);
        }
        free(v);
        v = sig;
    }
    variables = NULL;
}

void init_interpreter() {
    variables = NULL;
    funciones = NULL;
}
