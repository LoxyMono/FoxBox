#pragma once

#include <Types.h>

static inline VOID OutByte(WORD port, BYTE value)
{
    asm volatile ("outb %0, %1" : : "a"(value), "Nd"(port));
}

static inline BYTE InByte(WORD port)
{
    BYTE ret;
    asm volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

static inline VOID OutWord(WORD port, WORD value)
{
    asm volatile ("outw %0, %1" : : "a"(value), "Nd"(port));
}

static inline WORD InWord(WORD port)
{
    WORD ret;
    asm volatile ("inw %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

static inline VOID OutDword(WORD port, unsigned int value)
{
    asm volatile ("outl %0, %1" : : "a"(value), "Nd"(port));
}

static inline unsigned int InDword(WORD port)
{
    unsigned int ret;
    asm volatile ("inl %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}