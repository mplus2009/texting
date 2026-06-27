#!/bin/bash

# Texting OS Shell - Sin necesidad de punto y coma

TEXTING_HOME="$HOME/texting"
TMP_DIR="$TEXTING_HOME/tmp"
mkdir -p "$TMP_DIR"

echo ""
echo "========================================="
echo "     Texting OS Shell v2.0"
echo "========================================="
echo ""
echo "✨ Características:"
echo "  - No necesitas punto y coma (;)"
echo "  - Mayúsculas y minúsculas indistintas"
echo "  - Comandos multilínea"
echo ""
echo "Ejemplos:"
echo "  escribir en consola hola mundo"
echo "  guardar 15 en var"
echo "  escribir en consola var"
echo "  si 10 > 5 entonces escribir en consola mayor fin"
echo ""
echo "Escribe 'salir' para terminar"
echo ""

while true; do
    echo -n "texting> "
    read cmd
    
    if [ "$cmd" = "salir" ] || [ "$cmd" = "exit" ]; then
        echo "Adios!"
        break
    elif [ "$cmd" = "clear" ]; then
        clear
    elif [ -n "$cmd" ]; then
        echo "$cmd" > "$TMP_DIR/cmd.tg"
        cd "$TEXTING_HOME"
        ./bin/texting "$TMP_DIR/cmd.tg" 2>&1 | grep -v "^--- Texting ---$" | grep -v "^--- Fin ---$"
    fi
done
