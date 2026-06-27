#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    char nombre[100];
    int valor;
} Variable;

Variable variables[100];
int num_variables = 0;

int buscar_variable(char* nombre) {
    for(int i = 0; i < num_variables; i++) {
        if(strcasecmp(variables[i].nombre, nombre) == 0) {
            return i;
        }
    }
    return -1;
}

void guardar_variable(char* nombre, int valor) {
    int idx = buscar_variable(nombre);
    if(idx == -1) {
        idx = num_variables;
        strcpy(variables[idx].nombre, nombre);
        num_variables++;
    }
    variables[idx].valor = valor;
    printf("DEBUG: Guardado %s = %d\n", nombre, valor);  // Debug
}

int es_numero(char* s) {
    if(!s || !*s) return 0;
    if(*s == '-') s++;
    while(*s) {
        if(!isdigit(*s)) return 0;
        s++;
    }
    return 1;
}

int evaluar_expresion(char* expr) {
    char limpio[100];
    int j = 0;
    for(int i = 0; expr[i]; i++) {
        if(expr[i] != ' ') limpio[j++] = expr[i];
    }
    limpio[j] = '\0';
    
    char* mas = strchr(limpio, '+');
    char* menos = strchr(limpio, '-');
    char* por = strchr(limpio, '*');
    char* div = strchr(limpio, '/');
    
    int izq_val, der_val;
    char izq[100], der[100];
    
    if(mas) {
        strncpy(izq, limpio, mas - limpio);
        izq[mas - limpio] = '\0';
        strcpy(der, mas + 1);
        
        if(es_numero(izq)) izq_val = atoi(izq);
        else {
            int idx = buscar_variable(izq);
            if(idx == -1) return 0;
            izq_val = variables[idx].valor;
        }
        
        if(es_numero(der)) der_val = atoi(der);
        else {
            int idx = buscar_variable(der);
            if(idx == -1) return 0;
            der_val = variables[idx].valor;
        }
        return izq_val + der_val;
    }
    
    if(por) {
        strncpy(izq, limpio, por - limpio);
        izq[por - limpio] = '\0';
        strcpy(der, por + 1);
        
        if(es_numero(izq)) izq_val = atoi(izq);
        else {
            int idx = buscar_variable(izq);
            if(idx == -1) return 0;
            izq_val = variables[idx].valor;
        }
        
        if(es_numero(der)) der_val = atoi(der);
        else {
            int idx = buscar_variable(der);
            if(idx == -1) return 0;
            der_val = variables[idx].valor;
        }
        return izq_val * der_val;
    }
    
    if(menos && menos != limpio) {
        strncpy(izq, limpio, menos - limpio);
        izq[menos - limpio] = '\0';
        strcpy(der, menos + 1);
        
        if(es_numero(izq)) izq_val = atoi(izq);
        else {
            int idx = buscar_variable(izq);
            if(idx == -1) return 0;
            izq_val = variables[idx].valor;
        }
        
        if(es_numero(der)) der_val = atoi(der);
        else {
            int idx = buscar_variable(der);
            if(idx == -1) return 0;
            der_val = variables[idx].valor;
        }
        return izq_val - der_val;
    }
    
    if(div) {
        strncpy(izq, limpio, div - limpio);
        izq[div - limpio] = '\0';
        strcpy(der, div + 1);
        
        if(es_numero(izq)) izq_val = atoi(izq);
        else {
            int idx = buscar_variable(izq);
            if(idx == -1) return 0;
            izq_val = variables[idx].valor;
        }
        
        if(es_numero(der)) der_val = atoi(der);
        else {
            int idx = buscar_variable(der);
            if(idx == -1) return 0;
            der_val = variables[idx].valor;
        }
        if(der_val == 0) return 0;
        return izq_val / der_val;
    }
    
    if(es_numero(limpio)) return atoi(limpio);
    
    int idx = buscar_variable(limpio);
    if(idx == -1) return 0;
    return variables[idx].valor;
}

int main(int argc, char** argv) {
    if(argc < 2) {
        printf("Uso: ./bin/texting archivo.tg\n");
        return 1;
    }
    
    FILE* f = fopen(argv[1], "r");
    if(!f) {
        printf("Error: No se puede abrir %s\n", argv[1]);
        return 1;
    }
    
    char linea[1024];
    
    while(fgets(linea, sizeof(linea), f)) {
        linea[strcspn(linea, "\n")] = 0;
        
        // Saltar líneas vacías y comentarios
        if(strlen(linea) == 0) continue;
        if(linea[0] == '#') continue;
        
        // Quitar punto y coma si existe
        char* pcoma = strchr(linea, ';');
        if(pcoma) *pcoma = 0;
        
        // Limpiar espacios al inicio
        char* cmd = linea;
        while(*cmd == ' ') cmd++;
        
        if(strlen(cmd) == 0) continue;
        
        // Comando: guardar
        if(strncasecmp(cmd, "guardar", 7) == 0 && (cmd[7] == ' ' || cmd[7] == '\0')) {
            char* resto = cmd + 7;
            while(*resto == ' ') resto++;
            
            char* en = strstr(resto, " en ");
            if(en) {
                *en = '\0';
                char* valor_str = resto;
                char* var = en + 4;
                while(*var == ' ') var++;
                
                int valor = evaluar_expresion(valor_str);
                guardar_variable(var, valor);
            }
        }
        
        // Comando: escribir en consola
        else if(strncasecmp(cmd, "escribir en consola", 19) == 0) {
            char* contenido = cmd + 19;
            while(*contenido == ' ') contenido++;
            
            // Caso 1: Entre comillas dobles
            if(contenido[0] == '"') {
                contenido++;
                char* fin = strchr(contenido, '"');
                if(fin) *fin = '\0';
                printf("%s\n", contenido);
            }
            // Caso 2: Entre comillas simples
            else if(contenido[0] == '\'') {
                contenido++;
                char* fin = strchr(contenido, '\'');
                if(fin) *fin = '\0';
                printf("%s\n", contenido);
            }
            // Caso 3: Expresión matemática
            else if(strchr(contenido, '+') || strchr(contenido, '-') || 
                    strchr(contenido, '*') || strchr(contenido, '/')) {
                int resultado = evaluar_expresion(contenido);
                printf("%d\n", resultado);
            }
            // Caso 4: Variable
            else {
                int idx = buscar_variable(contenido);
                if(idx >= 0) {
                    printf("%d\n", variables[idx].valor);
                } else {
                    printf("%s\n", contenido);
                }
            }
        }
        
        // Comando: leer entrada
        else if(strncasecmp(cmd, "leer entrada", 12) == 0) {
            char* resto = cmd + 12;
            while(*resto == ' ') resto++;
            
            char* en = strstr(resto, " en ");
            if(en) {
                char* var = en + 4;
                while(*var == ' ') var++;
                
                char entrada[100];
                printf("> ");
                fgets(entrada, sizeof(entrada), stdin);
                entrada[strcspn(entrada, "\n")] = 0;
                
                if(es_numero(entrada)) {
                    guardar_variable(var, atoi(entrada));
                } else {
                    printf("Error: se esperaba un número\n");
                }
            }
        }
        
        // Si es solo el nombre de una variable (mostrar valor)
        else {
            int idx = buscar_variable(cmd);
            if(idx >= 0) {
                printf("%d\n", variables[idx].valor);
            }
        }
    }
    
    fclose(f);
    return 0;
}
