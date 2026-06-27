#!/bin/bash

echo "========================================="
echo "  Iniciando Texting OS Shell"
echo "========================================="
echo ""

# Usar directorio home para archivos temporales
TMP_DIR="$HOME/tmp"
mkdir -p "$TMP_DIR"
TMP_FILE="$TMP_DIR/texting_shell.tg"

# Bucle infinito del shell
while true; do
    echo -n "texting> "
    read comando
    
    # Comandos internos del shell bash
    if [ "$comando" = "salir" ] || [ "$comando" = "exit" ]; then
        echo "Saliendo..."
        break
    elif [ "$comando" = "help" ] || [ "$comando" = "ayuda" ]; then
        echo ""
        echo "=== Comandos disponibles ==="
        echo "  Guardar NUM en VAR;              - Guardar numero"
        echo "  escribir en consola TEXTO;       - Mostrar texto"
        echo "  escribir en consola VAR;         - Mostrar variable"
        echo "  si X > Y entonces ... fin;       - Condicional"
        echo "  repetir N veces ... fin;         - Bucle fijo"
        echo "  mientras X > Y hacer ... fin;    - Bucle while"
        echo "  clear / limpiar                  - Limpiar pantalla"
        echo "  help / ayuda                     - Esta ayuda"
        echo "  salir / exit                     - Salir del shell"
        echo "=================================="
        echo ""
    elif [ "$comando" = "clear" ] || [ "$comando" = "limpiar" ]; then
        clear
    elif [ -z "$comando" ]; then
        # Línea vacía, no hacer nada
        continue
    else
        # Ejecutar como comando Texting
        echo "$comando" > "$TMP_FILE"
        ./bin/texting "$TMP_FILE" 2>&1 | sed '/^--- Texting ---$/d' | sed '/^--- Fin ---$/d'
    fi
done

rm -f "$TMP_FILE"
echo "Shell terminado"
