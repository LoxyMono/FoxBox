#include <Utils/ElfLoader.h>

VOID *ElfLoaderLoadFromMemory(VOID *elfData)
{
    ELF_HEADER_32 *elfHeader = (ELF_HEADER_32 *)elfData;

    if (elfHeader->magic[0] != 0x7F || elfHeader->magic[1] != 'E' ||
        elfHeader->magic[2] != 'L' || elfHeader->magic[3] != 'F')
    {
        VgaSetColor(VGA_COLOR_LIGHTRED, VGA_COLOR_BLACK);
        VgaPrintString("\t\tInvalid ELF magic number\r\n");
        VgaResetColor();
        return NULL_PTR;
    }

    ELF_PROGRAM_HEADER_32 *programHeaders = (ELF_PROGRAM_HEADER_32 *)((DWORD)elfData + elfHeader->programHeaderTableOffset);

    for (WORD i = 0; i < elfHeader->programHeaderEntryCount; i++)
    {
        ELF_PROGRAM_HEADER_32 *programHeader = &programHeaders[i];

        if (programHeader->type != 1) /// Only load PT_LOAD segments
        {
            continue;
        }

        VOID *segmentDestination = (VOID *)programHeader->physicalAddress;
        VOID *segmentSource = (VOID *)((DWORD)elfData + programHeader->offset);

        StringCopy((CHAR *)segmentDestination, (const CHAR *)segmentSource, programHeader->fileSize);

        /// Zero out the remaining memory if memorySize > fileSize
        if (programHeader->memorySize > programHeader->fileSize)
        {
            StringSet((BYTE *)segmentDestination + programHeader->fileSize, 0x00, programHeader->memorySize - programHeader->fileSize);
        }
    }

    return (VOID *)elfHeader->entryPointOffset;
}