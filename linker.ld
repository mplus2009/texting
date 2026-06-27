/* Linker script para Texting OS - Sin dependencias de libc */
OUTPUT_FORMAT(binary)
ENTRY(_start)

SECTIONS
{
    . = 0x1000;
    
    .text : {
        *(.text)
        *(.text.*)
    }
    
    .data : {
        *(.data)
        *(.data.*)
    }
    
    .rodata : {
        *(.rodata)
        *(.rodata.*)
    }
    
    .bss : {
        *(.bss)
        *(COMMON)
    }
    
    /* Stack */
    . = ALIGN(4096);
    stack_top = .;
    . += 4096;
    stack_bottom = .;
}
