#!/bin/bash
set -e

cd ~/texting
rm -f src/*.o bin/texting

echo "=== Generando parser y lexer ==="
bison -d src/parser.y -o src/parser.tab.c
flex -o src/lex.yy.c src/lexer.l

echo "=== Compilando objetos ==="
gcc -c src/ast.c -o src/ast.o
gcc -c src/interpreter.c -o src/interpreter.o
gcc -c src/lex.yy.c -o src/lex.yy.o
gcc -c src/parser.tab.c -o src/parser.tab.o
gcc -c src/compiler.c -o src/compiler.o
gcc -c src/texting.c -o src/texting.o

echo "=== Enlazando ==="
mkdir -p bin
gcc -o bin/texting src/ast.o src/interpreter.o src/lex.yy.o src/parser.tab.o src/compiler.o src/texting.o -lm

echo ""
echo "✅ Texting compilado exitosamente"
echo "Ubicación: bin/texting"
echo ""
echo "=== Modos disponibles ==="
./bin/texting -h
