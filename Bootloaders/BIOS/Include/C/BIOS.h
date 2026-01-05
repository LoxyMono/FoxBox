#pragma once

#include <Types.h>

typedef struct REGS
{
    DWORD eax, ebx, ecx, edx;
    WORD es, di;
    WORD ds, si;
} __attribute__((packed)) REGS;

extern REGS *BIOSCall(BYTE function, REGS regs);