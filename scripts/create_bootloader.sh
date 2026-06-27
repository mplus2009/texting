#!/bin/bash

echo "=== Creando Bootloader para Texting OS ==="

# Bootloader en ensamblador
cat > bootloader.asm << 'ASM'
; Bootloader para Texting OS
; Modo real x86

BITS 16
ORG 0x7C00

start:
    ; Configurar segmentos
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00
    
    ; Limpiar pantalla
    mov ax, 0x0003
    int 0x10
    
    ; Mostrar mensaje
    mov si, msg_booting
    call print
    
    ; Cargar kernel desde disco
    mov ah, 0x02        ; leer sectores
    mov al, 16          ; 16 sectores (8KB)
    mov ch, 0           ; cilindro 0
    mov cl, 2           ; sector 2
    mov dh, 0           ; cabeza 0
    mov bx, 0x1000      ; dirección de carga
    mov es, bx
    xor bx, bx
    int 0x13
    jc error
    
    ; Saltar al kernel
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

error:
    mov si, msg_error
    call print
    jmp $

msg_booting db 'Texting OS - Booting...', 13, 10, 0
msg_error   db 'Error loading kernel!', 0

times 510-($-$$) db 0
dw 0xAA55
ASM

# Compilar bootloader
echo "Compilando bootloader..."
nasm -f bin bootloader.asm -o bootloader.bin 2>/dev/null

if [ $? -eq 0 ]; then
    echo "✅ Bootloader creado: bootloader.bin"
else
    echo "❌ Error: nasm no instalado. Instalar con: pkg install nasm"
    exit 1
fi

# Crear imagen de disco
echo "Creando imagen de disco..."
dd if=/dev/zero of=texting.img bs=512 count=2880 2>/dev/null
dd if=bootloader.bin of=texting.img conv=notrunc 2>/dev/null

echo "✅ Imagen de disco: texting.img"
echo ""
echo "Para probar en QEMU:"
echo "qemu-system-x86_64 -drive format=raw,file=texting.img"
