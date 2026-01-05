BITS 16

SECTION .init.bss

ptrStack:
    db 0 dup(8192)
    .top:

SECTION .init.text

GLOBAL _Startup
_Startup:
    mov esp, ptrStack.top
    mov ebp, esp

    in al, 0x92
    or al, 2
    out 0x92, al

    cli

    lgdt [strucGdtr]

    mov eax, cr0
    or eax, 1
    mov cr0, eax
    jmp 0x08:lblStartProtectedMode

    .x:
        cli
        hlt
        jmp .x

SECTION .init.rodata

GLOBAL strucGdtr
GLOBAL strucIdtReal

strucGdt:
    .nullDescriptor:
        dw 0x0000, 0x0000
        db 0x00, 0b00000000, 0b00000000, 0x00
    .code32:
        dw 0xFFFF, 0x0000
        db 0x00, 0b10011010, 0b11001111, 0x00
    .data32:
        dw 0xFFFF, 0x0000
        db 0x00, 0b10010010, 0b11001111, 0x00
    .code16:
        dw 0xFFFF, 0x0000
        db 0x00, 0b10011010, 0b00000000, 0x00
    .data16:
        dw 0xFFFF, 0x0000
        db 0x00, 0b10010010, 0b11001111, 0x00
    .end:

strucGdtr:
    .limit: dw strucGdt.end - strucGdt - 1
    .base:  dd strucGdt

strucIdtReal:
    dw 0x03FF
    dd 0

SECTION .text

EXTERN Main

BITS 32
lblStartProtectedMode:
    mov eax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    xor eax, eax

    call Main

.x:
    cli
    hlt
    jmp .x