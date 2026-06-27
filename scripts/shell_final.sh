#!/bin/bash

TMP_DIR="$HOME/texting/tmp"
mkdir -p "$TMP_DIR"

echo ""
echo "╔════════════════════════════════════════════╗"
echo "║          Texting OS Shell v3.0             ║"
echo "╚════════════════════════════════════════════╝"
echo ""
echo "📝 Reglas:"
echo "   guardar 42 en respuesta   -> guarda número"
echo "   respuesta                 -> muestra variable"
echo "   escribir en consola respuesta -> muestra variable"
echo "   escribir en consola \"texto\" -> muestra texto"
echo "   escribir en consola 5+3   -> muestra resultado"
echo "   salir                     -> terminar"
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
        cd "$HOME/texting"
        ./bin/texting "$TMP_DIR/cmd.tg" 2>&1 | grep -v "^DEBUG:"
    fi
done
