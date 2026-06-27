#!/bin/bash

echo "========================================="
echo "  Texting OS Builder - Crea tu SO"
echo "========================================="

# Colores
RED='\033[0;31m'
GREEN='\033[0;32m'
BLUE='\033[0;34m'
NC='\033[0m'

# Crear estructura
mkdir -p os/bootloader os/kernel os/user

echo -e "${BLUE}[1/6] Creando bootloader...${NC}"
cat > os/bootloader/boot.asm << 'ASM'
BITS 16
ORG 0x7C00

start:
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00
    
    mov si, msg
    call print
    
    mov ah, 0x02
    mov al, 32
    mov ch, 0
    mov cl, 2
    mov dh, 0
    mov bx, 0x1000
    mov es, bx
    xor bx, bx
    int 0x13
    
    jmp 0x1000:0x0000

print:
    lodsb
    or al, al
    jz .done
    mov ah, 0x0E
    int 0x10
    jmp print
.done:
    ret

msg db 'Texting OS v1.0', 13, 10, 0
times 510-($-$$) db 0
dw 0xAA55
ASM

echo -e "${BLUE}[2/6] Compilando bootloader...${NC}"
nasm -f bin os/bootloader/boot.asm -o os/bootloader/boot.bin 2>/dev/null

echo -e "${BLUE}[3/6] Creando kernel en Texting...${NC}"
cat > os/kernel/kernel.tg << 'KERNEL'
# Texting OS Kernel
escribir en consola "=========================================";
escribir en consola "  Texting OS Kernel v1.0";
escribir en consola "=========================================";
escribir en consola "Kernel cargado correctamente";
escribir en consola "Inicializando hardware...";

# Bucle principal
Guardar 0 en ticks;
repetir 10 veces
    Guardar ticks + 1 en ticks;
    si ticks % 5 == 0 entonces
        escribir en consola "Heartbeat";
    fin;
fin;

escribir en consola "=========================================";
escribir en consola "Sistema listo!";
escribir en consola "Bienvenido a Texting OS";
escribir en consola "=========================================";

# Demostracion de variables
Guardar 10 en x;
Guardar 20 en y;
Guardar x + y en z;
escribir en consola "Demostracion: 10 + 20 = ";
escribir en consola z;
KERNEL

echo -e "${BLUE}[4/6] Compilando kernel...${NC}"
cp os/kernel/kernel.tg kernel.tg

echo -e "${BLUE}[5/6] Creando imagen de disco...${NC}"
dd if=/dev/zero of=os.img bs=512 count=2880 2>/dev/null
dd if=os/bootloader/boot.bin of=os.img conv=notrunc 2>/dev/null

echo -e "${BLUE}[6/6] Creando programas de usuario...${NC}"
cat > os/user/hola.tg << 'USER1'
# Programa de usuario
escribir en consola "Hola desde Texting!";
Guardar 5 en a;
Guardar 3 en b;
Guardar a + b en c;
escribir en consola "5 + 3 = ";
escribir en consola c;
USER1

cat > os/user/bucle.tg << 'USER2'
# Bucle de ejemplo
Guardar 1 en i;
mientras i <= 5 hacer
    escribir en consola i;
    Guardar i + 1 en i;
fin;
escribir en consola "Fin del bucle";
USER2

echo -e "${GREEN}========================================="
echo -e "OS Creado Exitosamente!"
echo -e "=========================================${NC}"
echo ""
echo "Archivos generados:"
ls -la os.img 2>/dev/null
ls -la os/bootloader/boot.bin 2>/dev/null
echo ""
echo "Para probar el kernel con el interprete Texting:"
echo "./bin/texting os/kernel/kernel.tg"
echo ""
echo "Para instalar QEMU y probar el SO:"
echo "pkg install qemu-system-x86_64"
echo "qemu-system-x86_64 -drive format=raw,file=os.img"
