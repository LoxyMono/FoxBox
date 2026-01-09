ENTRY(FoxBoxMain)
OUTPUT_FORMAT(elf32-i386)
SECTIONS
{
    . = 0xFF800000;

    .text BLOCK(4K) : ALIGN(4K)
    {
        *(.text)
    }
    .rodata BLOCK(4K) : ALIGN(4K)
    {
        *(.rodata)
    }
    .data BLOCK(4K) : ALIGN(4K)
    {
        *(.data)
    }
    .bss (NOLOAD) : ALIGN(4K)
    {
        *(.bss)
        *(COMMON)
    }

    /DISCARD/ :
    {
        *(.eh_frame)
        *(.comment)
        *(.note.GNU-stack)
        *(.gnu.warning)
    }
}