BITS 32

SECTION .text
GLOBAL Init32 ;; IN = Address of paging tables, Kernel entry point

Init32:
    push ebp
    mov ebp, esp

    mov eax, [ebp + 8] ;; Paging tables address
    mov cr3, eax

    mov ebx, [ebp + 12] ;; Kernel entry point address

    mov eax, cr0
    or eax, 0x80000001 ;; Enable paging and protected mode
    mov cr0, eax

    jmp ebx ;; Jump to kernel entry point