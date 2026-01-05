#include <Memory/MemoryMap.h>

static MEMORY_MAP_ENTRY mMemoryMaps[32];

MEMORY_MAP_ENTRY *MemoryMapLoad(DWORD *entryCount)
{
    REGS in = { 0 };
    REGS *out = NULL_PTR;
    DWORD count = 0;

    in.eax = 0xE820;
    in.ebx = 0;
    in.ecx = 24;
    in.edx = 0x534D4150;
    in.es = 0;
    in.di = (WORD)&mMemoryMaps;
    out = BIOSCall(0x15, in);

    while (out->ebx != 0 && count < 32)
    {
        count++;

        in.eax = 0xE820;
        in.ebx = out->ebx;
        in.ecx = 24;
        in.edx = 0x534D4150;
        in.es = 0;
        in.di += 24;
        out = BIOSCall(0x15, in);
    }

    *entryCount = count;
    return mMemoryMaps;
}