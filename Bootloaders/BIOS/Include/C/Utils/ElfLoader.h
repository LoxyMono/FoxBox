#pragma once

#include <Types.h>

#include <IO/Disk.h>
#include <IO/Filesystem.h>

#include <Memory/Allocate.h>

#include <Video/VGA.h>

typedef struct ELF_HEADER_32
{
    CHAR magic[4];
    BYTE arch;
    BYTE endianness;
    BYTE headerVersion;
    BYTE osAbi;
    BYTE unused[8];
    WORD type;
    WORD instructionSet;
    DWORD elfVersion;
    DWORD entryPointOffset;
    DWORD programHeaderTableOffset;
    DWORD sectionHeaderTableOffset;
    DWORD flags;
    WORD headerSize;
    WORD programHeaderEntrySize;
    WORD programHeaderEntryCount;
    WORD sectionHeaderEntrySize;
    WORD sectionHeaderEntryCount;
    WORD sectionHeaderStringTableIndex;
} __attribute__((packed)) ELF_HEADER_32;

typedef struct ELF_PROGRAM_HEADER_32
{
    DWORD type;
    DWORD offset;
    DWORD virtualAddress;
    DWORD physicalAddress;
    DWORD fileSize;
    DWORD memorySize;
    DWORD flags;
    DWORD alignment;
} __attribute__((packed)) ELF_PROGRAM_HEADER_32;

VOID *ElfLoaderLoadFromMemory(VOID *elfData);