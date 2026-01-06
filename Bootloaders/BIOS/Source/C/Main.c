#include <Types.h>

#include <BIOS.h>

#include <IO/Disk.h>
#include <IO/Ports.h>

#include <Memory/Allocate.h>
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

    VgaSetColor(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
    VgaPrintString("Beginning loader initialization\r\n\n");
    VgaResetColor();
    
    DWORD entryCount = 0;
    MEMORY_MAP_ENTRY *memoryMaps = MemoryMapLoad(&entryCount);

    DWORD bestMap = 0;

    for (DWORD i = 0; i < entryCount; i++)
    {
        MEMORY_MAP_ENTRY *entry = &memoryMaps[i];
        if (entry->type == MEMORY_MAP_TYPE_AVAILABLE &&
            (entry->lengthHigh > memoryMaps[bestMap].lengthHigh ||
            (entry->lengthHigh == memoryMaps[bestMap].lengthHigh &&
             entry->lengthLow > memoryMaps[bestMap].lengthLow)) &&
            (entry->lengthHigh != 0 || entry->lengthLow >= 0x100000))
        {
            bestMap = i;
        }
    }

    AllocateInit(&memoryMaps[bestMap]);
}