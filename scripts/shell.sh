#!/bin/bash

# Texting OS Shell - Con soporte para comandos multilínea

# Colores
if [ -t 1 ]; then
    GREEN='\033[0;32m'
    YELLOW='\033[1;33m'
    BLUE='\033[0;34m'
    NC='\033[0m'
else
    GREEN=''; YELLOW=''; BLUE=''; NC=''
fi

# Directorios
TEXTING_HOME="$HOME/texting"
TMP_DIR="$TEXTING_HOME/tmp"
mkdir -p "$TMP_DIR"
HISTORY_FILE="$TEXTING_HOME/.texting_history"
touch "$HISTORY_FILE"

# Variables
ultimo_comando=""
en_bloque=0
bloque=""

# Función para ejecutar un bloque de código Texting
ejecutar_bloque() {
    local codigo="$1"
    local tmp_file="$TMP_DIR/cmd_$$.tg"
    echo "$codigo" > "$tmp_file"
    cd "$TEXTING_HOME"
    ./bin/texting "$tmp_file" 2>&1 | grep -v "^--- Texting ---$" | grep -v "^--- Fin ---$"
    rm -f "$tmp_file"
}

# Función para mostrar ayuda
mostrar_ayuda() {
    echo ""
    echo -e "${BLUE}=== Comandos Texting ===${NC}"
    echo "  Guardar NUM en VAR;              - Guardar numero"
    echo "  escribir en consola TEXTO;       - Mostrar texto"
    echo "  escribir en consola VAR;         - Mostrar variable"
    echo "  si COND entonces ... fin;        - Condicional"
    echo "  repetir N veces ... fin;         - Bucle fijo"
    echo "  mientras COND hacer ... fin;     - Bucle while"
    echo ""
    echo -e "${BLUE}=== Comandos del shell ===${NC}"
    echo "  help / ayuda        - Mostrar ayuda"
    echo "  clear / limpiar     - Limpiar pantalla"
    echo "  history             - Ver historial"
    echo "  salir / exit        - Salir"
    echo ""
}

# Limpiar pantalla
limpiar_pantalla() {
    printf "\033[2J\033[1;1H"
}

# Inicio
limpiar_pantalla
echo ""
echo -e "${GREEN}╔════════════════════════════════════════════╗${NC}"
echo -e "${GREEN}║     Texting OS Shell - Modo Interactivo    ║${NC}"
echo -e "${GREEN}╚════════════════════════════════════════════╝${NC}"
echo ""
echo -e "${YELLOW}Escribe 'help' para ayuda. Los comandos multilínea se ejecutan al escribir 'fin'${NC}"
echo ""

while true; do
    if [ $en_bloque -eq 0 ]; then
        echo -n "${GREEN}texting> ${NC}"
    else
        echo -n "${GREEN}  ...> ${NC}"
    fi
    
    read linea
    
    # Guardar en historial si no es parte de un bloque multilínea
    if [ $en_bloque -eq 0 ] && [ -n "$linea" ]; then
        echo "$linea" >> "$HISTORY_FILE"
        ultimo_comando="$linea"
    fi
    
    # Procesar comandos especiales del shell (solo fuera de bloques)
    if [ $en_bloque -eq 0 ]; then
        case "$linea" in
            "salir"|"exit")
                echo -e "${YELLOW}Saliendo...${NC}"
                break
                ;;
            "help"|"ayuda")
                mostrar_ayuda
                continue
                ;;
            "clear"|"limpiar")
                limpiar_pantalla
                continue
                ;;
            "history")
                echo -e "${BLUE}=== Historial ===${NC}"
                tail -20 "$HISTORY_FILE"
                continue
                ;;
            "!!")
                if [ -n "$ultimo_comando" ]; then
                    echo -e "${YELLOW}Ejecutando: $ultimo_comando${NC}"
                    ejecutar_bloque "$ultimo_comando"
                else
                    echo -e "${YELLOW}No hay comando anterior${NC}"
                fi
                continue
                ;;
            "")
                continue
                ;;
        esac
    fi
    
    # Manejo de comandos multilínea
    if [[ "$linea" =~ ^(si|repetir|mientras) ]] || [ $en_bloque -eq 1 ]; then
        if [ $en_bloque -eq 0 ]; then
            en_bloque=1
            bloque="$linea"
        else
            bloque="$bloque"$'\n'"$linea"
        fi
        
        # Verificar si el bloque terminó
        if [[ "$linea" =~ ^[[:space:]]*fin[[:space:]]*\;?[[:space:]]*$ ]] || [[ "$linea" == "fin;" ]]; then
            en_bloque=0
            ejecutar_bloque "$bloque"
            bloque=""
        fi
    else
        # Comando de una sola línea
        if [ -n "$linea" ]; then
            ejecutar_bloque "$linea"
        fi
    fi
done

echo -e "${GREEN}¡Hasta luego!${NC}"
