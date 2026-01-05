ENTRY(FoxBoxMain)
OUTPUT_FORMAT(binary)
SECTIONS
{
    . = 0xF0000000;

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