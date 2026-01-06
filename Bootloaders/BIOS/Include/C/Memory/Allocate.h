#pragma once

#include <Types.h>

#include <Memory/MemoryMap.h>

#include <Utils/String.h>

#include <Video/VGA.h>

#define PAGE_SIZE 4096

typedef struct PAGE
{
    BYTE data[4096];
} PAGE;

typedef struct ALLOCATION
{
    DWORD address;
    DWORD size;
} ALLOCATION;

VOID  AllocateInit(MEMORY_MAP_ENTRY *memoryMap);
VOID *Allocate(DWORD size);
VOID  Release(VOID *ptr);