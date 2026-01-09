#include <Types.h>

#include <BIOS.h>

#include <IO/Disk.h>
#include <IO/Filesystem.h>
#include <IO/Ports.h>

#include <Memory/Allocate.h>
#include <Memory/MemoryMap.h>
#include <Memory/Paging.h>

#include <Utils/CPUID.h>
#include <Utils/ElfLoader.h>
#include <Utils/Halt.h>
#include <Utils/String.h>

#include <Video/VGA.h>

extern VOID Init32(DWORD pagingTablesAddress, DWORD kernelEntryPoint);

VOID Main()
{
    VgaInit();

    VgaSetColor(VGA_COLOR_ORANGE, VGA_COLOR_BLACK);
    VgaPrintString("FoxBox");
    VgaResetColor();
    VgaPrintString(" x86 BIOS\r\n");
    VgaPrintString("Compiled on " __DATE__ " at " __TIME__ "\r\n");

    VgaSetColor(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
    VgaPrintString("\nBeginning loader initialization\r\n\n");
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

    BOOL fsStatus = FsInit();
    if (!fsStatus)
    {
        Halt(ERR_FILESYSTEM_INVALID_TYPE, "Unsupported or invalid filesystem type");
    }

    VgaSetColor(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
    VgaPrintString("\nChecking CpuID\r\n\n");
    VgaResetColor();

    I32 cpuIdAvailable = CpuIdAvailability();

    VgaPrintString("\t\tCpuId instruction available?  %s\r\n", cpuIdAvailable ? "YES" : "NO");

    DWORD eax, ebx, ecx, edx;
    CpuId(1, &eax, &ebx, &ecx, &edx);

    VgaPrintString("\t\tSSE support available?        %s\r\n", (edx & (1 << 25)) ? "YES" : "NO");
    VgaPrintString("\t\tLocal APIC available?         %s\r\n", (edx & (1 << 9)) ? "YES" : "NO");
    VgaPrintString("\t\tPhysical Address Extensions?  %s\r\n", (edx & (1 << 6)) ? "YES" : "NO");
    
    VgaSetColor(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
    VgaPrintString("\nLoading Kernel\r\n\n");
    VgaResetColor();

    FILE_HANDLE *kernelFile = FsOpenFile("Kernel/FoxKrnl");
    if (kernelFile == NULL_PTR)
    {
        Halt(ERR_FILESYSTEM_FILE_NOT_FOUND, "Kernel/FoxKrnl not found");
    }

    VgaPrintString("\t\tKernel size: %d bytes\r\n", kernelFile->fileSize);
    VOID *kernelBuffer = Allocate(kernelFile->fileSize);
    if (kernelBuffer == NULL_PTR)
    {
        Halt(ERR_CLASS_MEMORY, "Failed to allocate memory for kernel");
    }

    VgaPrintString("\t\tLoading kernel into memory at 0x%8x\r\n", (DWORD)kernelBuffer);
    BOOL readStatus = FsReadFile(kernelFile, kernelBuffer, kernelFile->fileSize);
    if (!readStatus)
    {
        Halt(ERR_DISK_READ_FAILURE, "Failed to read kernel file from disk");
    }

    VOID *kernelEntryPoint = ElfLoaderLoadFromMemory(kernelBuffer);
    if (kernelEntryPoint == NULL_PTR)
    {
        Halt(ERR_CLASS_GENERAL, "Failed to load kernel ELF");
    }

    VgaSetColor(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
    VgaPrintString("\nTransferring control to kernel at 0x%8x\r\n", (DWORD)kernelEntryPoint);
    VgaResetColor();

    PAGE_DIRECTORY *pagingTables = (PAGE_DIRECTORY *)PagingInit();

    /// Map kernel to 0xFF800000
    DWORD kernelVirtualAddress = 0xFF800000;
    DWORD kernelPhysicalAddress = ((DWORD)kernelEntryPoint) & 0xFFF00000;
    DWORD kernelSize = ((ELF_HEADER_32 *)kernelBuffer)->sectionHeaderEntryCount * ((ELF_HEADER_32 *)kernelBuffer)->sectionHeaderEntrySize;

    for (DWORD offset = 0; offset < kernelSize; offset += 0x1000)
    {
        PagingMapPage(pagingTables, kernelVirtualAddress + offset, kernelPhysicalAddress + offset, TRUE, FALSE);
    }

    Init32((DWORD)pagingTables, (DWORD)kernelEntryPoint);
}