#include <Types.h>

#include <BIOS.h>

#include <IO/Disk.h>
#include <IO/Ports.h>

#include <Memory/MemoryMap.h>

#include <Utils/String.h>

#include <Video/VGA.h>

VOID Main()
{
    VgaInit();

    VgaSetColor(VGA_COLOR_ORANGE, VGA_COLOR_BLACK);
    VgaPrintString("FoxBox");
    VgaResetColor();
    VgaPrintString(" x86 BIOS\r\n");
    VgaPrintString("Compiled on " __DATE__ " at " __TIME__ "\r\n\r\n");

    VgaPrintString("\t\tInitializing loader");
    
    DWORD entryCount = 0;
    MEMORY_MAP_ENTRY *memoryMaps = MemoryMapLoad(&entryCount);

    for (DWORD i = 0; i < entryCount; i++)
    {
        MEMORY_MAP_ENTRY *entry = &memoryMaps[i];
        VgaPrintString("\r\nMemory Map Entry %x: Base=0x%8x_%8x Length=0x%8x_%8x Type=%x",
            i,
            entry->baseHigh,
            entry->baseLow,
            entry->lengthHigh,
            entry->lengthLow,
            entry->type);
    }
}