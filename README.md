# 🚀 Texting - Lenguaje de Programación en Español

**Texting** es un lenguaje de programación diseñado para crear **sistemas operativos desde cero**. Sintaxis 100% en español.

## 📋 Características

- ✅ Variables numéricas y texto
- ✅ Números negativos y decimales
- ✅ Operaciones: + - * /
- ✅ Entrada: `leer entrada en variable`
- ✅ Salida: `escribir en consola`
- ✅ Condicionales: `si` / `sino` / `fin`
- ✅ Bucles: `repetir N veces` y `mientras`
- ✅ Comentarios con `#`

## 📦 Instalación en Termux

```bash
pkg update
pkg install bison flex clang make

texting/
├── bin/texting      # Compilador
├── src/
│   ├── ast.h/.c     # Árbol sintáctico
│   ├── interpreter.h/.c
│   ├── lexer.l      # Analizador léxico
│   └── parser.y     # Analizador sintáctico
├── ejemplos/
└── Makefile

cd ~/texting
make clean
make

sintaxis 

variables
Guardar 10 en edad;
Guardar -5 en negativo;
Guardar "Juan" en nombre;

escribir en consola "Hola";
escribir en consola edad;
leer entrada en nombre;



si edad > 17 entonces
    escribir en consola "Mayor";
sino
    escribir en consola "Menor";
fin;

repetir 5 veces
    escribir en consola "Hola";
fin;

Guardar 3 en i;
mientras i > 0 hacer
    escribir en consola i;
    Guardar i - 1 en i;
fin;



Guardar 5 + 3 en suma;
Guardar 10 - 4 en resta;
Guardar 7 * 6 en multi;
Guardar 20 / 4 en divi;
Guardar -10 en negativo;


Guardar 5 en a;
Guardar 3 en b;
Guardar a + b en c;
escribir en consola c;


Guardar 1 en i;
mientras i <= 10 hacer
    Guardar 5 * i en r;
    escribir en consola r;
    Guardar i + 1 en i;
fin;


escribir en consola "Como te llamas?";
leer entrada en nombre;
escribir en consola "Hola ";
escribir en consola nombre;escribir en consola "Como te llamas?";
leer entrada en nombre;
escribir en consola "Hola ";
escribir en consola nombre;





# Compilar
cd ~/texting && make

# Ejecutar programa
./bin/texting ejemplos/mi_programa.tg

# Crear y ejecutar rápido
echo 'escribir en consola "Hola Mundo";' > test.tg
./bin/texting test.tg



# Operaciones
cat > ejemplos/op.tg << 'EOF'
Guardar 10 en a;
Guardar 3 en b;
Guardar a + b en s;
escribir en consola s;
Guardar a - b en r;
escribir en consola r;
Guardar a * b en m;
escribir en consola m;
Guardar a / b en d;
escribir en consola d;
