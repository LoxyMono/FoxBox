BITS 32
SECTION .init.rmtext

GLOBAL BIOSCall

EXTERN strucGdtr
EXTERN strucIdtReal

BIOSCall:
    push ebp
    mov ebp, esp

    cli

    mov eax, cr0
    mov [numCr0], eax
    mov [numEsp], esp
    push ebx
    push esi

    jmp 0x18:.initUnrealMode

    BITS 16
    .initUnrealMode:
        mov ax, 0x20
        mov ds, ax
        mov es, ax
        mov fs, ax
        mov gs, ax
        mov ss, ax

        lidt [strucIdtReal]

        mov eax, cr0
        and eax, 0xFFFFFFFE
        mov cr0, eax

        jmp 0x0000:.initRealMode

        .initRealMode:
            xor eax, eax
            mov ds, ax
            mov es, ax
            mov fs, ax
            mov gs, ax
            mov ss, ax

            sti

            mov eax, DWORD [ebp + 12]
            mov ebx, DWORD [ebp + 16]
            mov ecx, DWORD [ebp + 20]
            mov edx, DWORD [ebp + 24]

            push dx
            mov dx, WORD [ebp + 28]
            mov es, dx
            mov di, WORD [ebp + 30]
            mov dx, WORD [ebp + 32]
            mov ds, dx
            mov si, WORD [ebp + 34]
            pop dx

            ;; If I get sent to hell for this code, it was deserved
            push ax
            mov al, BYTE [ebp + 8]
            mov BYTE [.intCode], al
            pop ax

            db 0xCD ;; Opcode for INT immediate8
            .intCode: db 0

            cli

            mov DWORD [strucRegs], eax
            mov DWORD [strucRegs + 4], ebx
            mov DWORD [strucRegs + 8], ecx
            mov DWORD [strucRegs + 12], edx

            push dx
            mov dx, es
            mov WORD [strucRegs + 16], dx
            mov WORD [strucRegs + 18], di
            mov dx, ds
            mov WORD [strucRegs + 20], dx
            mov WORD [strucRegs + 22], si
            pop dx

            lgdt [strucGdtr]

            mov eax, [numCr0]
            mov cr0, eax

            jmp 0x08:.backToProtectedMode

            BITS 32
            .backToProtectedMode:
                mov ax, 0x10
                mov ds, ax
                mov es, ax
                mov fs, ax
                mov gs, ax
                mov ss, ax

                pop esi
                pop ebx

                mov esp, [numEsp]
                pop ebp

                mov eax, DWORD strucRegs

                ret

SECTION .init.data
ALIGN 4
strucRegs:
    dd 0, 0, 0, 0
    dw 0, 0, 0, 0

numCr0: dd 0
numEsp: dd 0