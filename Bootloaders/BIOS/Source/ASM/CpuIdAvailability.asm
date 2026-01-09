BITS 32

SECTION .text
GLOBAL CpuIdAvailability

;; Function: I32 CpuIdAvailability()
CpuIdAvailability:
    push ebp
    mov ebp, esp ;; We dont really need this, but it just makes the code look a little nicer

    pushfd
    pushfd
    xor DWORD [esp], 0x00200000 ;; Try to invert the ID bit in EFLAGS
    popfd
    pushfd
    pop eax
    xor eax, [esp] ;; Check if the ID bit changed
    popfd
    and eax, 0x00200000 ;; If the ID bit is set, CPUID is available

    pop ebp
    ret