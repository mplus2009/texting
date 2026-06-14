#!/bin/bash

echo "=== Compilando Texting para Sistema Operativo ==="

# Compilar el compilador
cd ~/texting
make clean
make

# Compilar kernel básico
echo "Compilando kernel..."
cat > kernel.tg << 'KERNEL'
# Kernel mínimo para Texting OS
escribir en consola "Texting OS v1.0";
escribir en consola "Iniciando kernel...";

# Inicializar memoria
reservar 65536 en memoria_kernel;

# Iniciar primer proceso
syscall 5("init");

# Bucle principal
repetir 10000 veces
    entrada_salida 0x60, tecla;
    si tecla == 27 entonces  # ESC para salir
        syscall 4();
    fin;
fin;
KERNEL

# Ejecutar el kernel
./bin/texting kernel.tg

echo "SO compilado!"
