BITS 16
ORG 0x7C00

jmp SHORT lblSkipBPB
nop

bpbOemIdentifier:         db 8 dup(0)
bpbBytesPerSector:        dw 512
bpbSectorsPerCluster:     db 0
bpbReservedSectors:       dw 0
bpbNumberOfFATS:          db 2
bpbRootDirectoryEntries:  dw 0
bpbSectors16:             dw 0
bpbMediaDescriptor:       db 0
bpbSectorsPerFAT:         dw 0 ;; Unused in FAT32
bpbSectorsPerTrack:       dw 0
bpbNumberOfHeads:         dw 0
bpbNumberOfHiddenSectors: dd 0
bpbSectors32:             dd 0

ebpbSectorsPerFAT:        dd 0
ebpbFlags:                dw 0
ebpbFATVersion:           dw 0
ebpbRootDirClusterNumber: dd 2
ebpbFSInfoSector:         dw 0
ebpbBackupBootSector:     dw 0
                          db 12 dup(0) ;; Reserved
ebpbDriveNumber:          db 0
                          db 0 ;; Also reserved
ebpbSignature:            db 0
ebpbVolumeID:             dd 0x4C6F7879
ebpbVolumeLabel:          db "FoxBox     "
ebpbSystemIdentifier:     db "FAT32   "

lblSkipBPB:
    jmp 0x0000:lblInit
lblInit:
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov sp, 0x7C00

    mov [ebpbDriveNumber], dl
    
    mov ah, 0x41
    mov bx, 0x55AA
    int 0x13

    jnc .continue

    mov si, strNoLBA
    call fnPrintString
    jmp lblDiskError.x

.continue:
    mov eax, [bpbNumberOfHiddenSectors]
    push eax
    add eax, 2
    mov [strucDiskPacket.lbaLow], eax

    mov [strucDiskPacket.address], WORD 0x7E00
    mov [strucDiskPacket.count], WORD 3
    mov ah, 0x42
    mov dl, [ebpbDriveNumber]
    mov si, strucDiskPacket
    int 0x13
    jc lblDiskError

    pop ebx
    movzx eax, WORD [bpbReservedSectors]
    add eax, ebx
    mov [numFATStart], eax

    mov eax, [ebpbSectorsPerFAT]
    shl eax, 1
    add eax, [numFATStart]
    mov [numDataStart], eax

    mov eax, 2

    mov di, 0xA000
    call fnReadCluster
    call fnReadCluster

    mov si, strOk
    call fnPrintString

    jmp lblContinueInit

    cli
    hlt
    jmp $

lblDiskError:
    mov si, strDiskError
    call fnPrintString

.x:
    cli
    hlt
    jmp .x

%include "Source/ASM/Print.asm"

;;; DATA ;;;
strNoLBA:     db "E: BIOS or drive does not support LBA", 0
strDiskError: db "E: Disk read error", 0
strOk:        db "INIT", 13, 10, 0
strNo:        db "No "
strStartFile: db "STARTUP BIN", 0

numFATStart:  dd 0
numDataStart: dd 0
numPartition: dw 0

ALIGN 4
strucDiskPacket:
    db 16
    db 0
    .count:   dw 1
    .address: dw 0x0000
    .segment: dw 0x0000
    .lbaLow:  dd 0
    .lbaHigh: dd 0

TIMES 510 - ($ - $$) db 0
dw 0xAA55

lblContinueInit:

    mov eax, [ebpbRootDirClusterNumber]
    mov ebp, 0x1400
    push eax
    movzx eax, BYTE [bpbSectorsPerCluster]
    mul WORD [bpbBytesPerSector]
    add ebp, eax
    pop eax

    mov edi, 0x1400
    push edi
    call fnReadCluster
    pop edi

    mov esi, strStartFile

    .searchRootDir:
        push esi
        mov ecx, 11
        a32 repe cmpsb
        pop esi

        jz .foundFile

        cmp edi, ebp
        jae .loadNextRootDirCluster
        jmp .searchRootDir

        .loadNextRootDirCluster:
            push eax
            call fnCalculateFATSector
            mov eax, ebx
            call fnReadFATSector
            pop eax
            
            mov esi, edx
            add esi, FAT_BUF
            mov eax, DWORD [esi]
            and eax, 0x0FFFFFFF
            cmp eax, 0x0FFFFFF8
            jae .noFile

            mov edi, 0x1400
            push edi
            call fnReadCluster
            pop edi

            mov esi, strStartFile
            
            jmp .searchRootDir

        .foundFile:
            add edi, 19
            movzx eax, WORD [edi]
            shl eax, 16
            sub edi, 4
            mov ax, WORD [edi]
            and eax, 0x0FFFFFFF

            push es
            push ax
            
            mov di, 0xA000

            .readLoop:
                push eax
                call fnCalculateFATSector
                mov eax, ebx
                call fnReadFATSector
                pop eax

                call fnReadCluster
                
                mov si, dx
                add si, FAT_BUF
                mov eax, DWORD [si]
                and eax, 0x0FFFFFFF
                cmp eax, 0x0FFFFFF8
                jae .doneReading
                jmp .readLoop

            .doneReading:
                pop es
                jmp 0x0000:0xA000
            
        .noFile:
            mov si, strNo
            call fnPrintString
            jmp $
        

DATA_BUF EQU 0x1000
FAT_BUF  EQU 0x1200

; IN: EAX = LBA
fnReadSector:
    push eax
    push si
    push dx
    push di

    mov [strucDiskPacket.lbaLow], eax
    mov [strucDiskPacket.address], WORD DATA_BUF
    mov [strucDiskPacket.count], WORD 1
    mov ah, 0x42
    mov dl, [ebpbDriveNumber]
    mov si, strucDiskPacket
    int 0x13

    jc lblDiskError

    pop di
    pop dx
    pop si
    pop eax
    ret

; Same as fnReadSector, but takes numFATStart into account automatically
fnReadFATSector:
    push eax
    push si
    push dx
    push di

    add eax, [numFATStart]

    mov [strucDiskPacket.lbaLow], eax
    mov [strucDiskPacket.address], WORD FAT_BUF
    mov [strucDiskPacket.count], WORD 1
    mov ah, 0x42
    mov dl, [ebpbDriveNumber]
    mov si, strucDiskPacket
    int 0x13

    jc lblDiskError

    pop di
    pop dx
    pop si
    pop eax
    ret

; IN: EAX = Cluster #
;     EDI = PAddress
fnReadCluster:
    push eax
    push ecx
    push esi

    sub eax, 2 ;; Correct for the true root cluster starting at cluster #2
    mul BYTE [bpbSectorsPerCluster]
    add eax, [numDataStart]

    movzx ecx, BYTE [bpbSectorsPerCluster]

    .readLoop:
        call fnReadSector

        dec ecx
        inc eax

        mov esi, DATA_BUF
        push ecx
        mov ecx, 512
        a32 rep movsb
        pop ecx

        jecxz .exit
        jmp .readLoop

        .exit:
            pop esi
            pop ecx
            pop eax
            ret

; IN: EAX = Cluster #
; OUT: EAX: FAT offset
;      EBX: FAT sector
;      EDX: Entry offset in sector
fnCalculateFATSector:
    ;; FAT offset = cluster * 4
    shl eax, 2

    xor ebx, ebx
    mov edx, ebx

    ;; FAT sector = (FAT offset / bytes per sector) + first FAT sector
    push eax
    shr eax, 9
    mov ebx, eax
    pop eax

    ;; FAT entry offset = FAT offset % bytes per sector
    mov edx, eax
    and edx, 512 - 1

    ret

TIMES 2046 - ($ - $$) nop
dw 0x55AA ;; NOTE: only put here for debugging purposes, this isnt required to boot