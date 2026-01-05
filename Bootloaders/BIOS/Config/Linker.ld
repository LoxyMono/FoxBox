ENTRY(_Startup)
OUTPUT_FORMAT(binary)
SECTIONS
{
    . = 0x7000;
    .init.bss (NOLOAD) :
    {
        *(.init.bss)
    }
    . = 0xA000;

    PtrLoaderStart = .;
    .init :
    {
        *(.init.text)
        *(.init.rmtext)
        *(.init.rodata)
        *(.init.data)
    }
    .text :
    {
        *(.text)
    }
    .rodata :
    {
        *(.rodata)
    }
    .data :
    {
        *(.data)
    }

    .bss (NOLOAD) : ALIGN(16)
    {
        *(.bss)
        *(COMMON)
    }

    PtrLoaderEnd = .;

    /DISCARD/ :
    {
        *(.eh_frame)
        *(.comment)
        *(.note.GNU-stack)
        *(.gnu.warning)
    }
}