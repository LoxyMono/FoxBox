#pragma once

#include <Types.h>

#include <BIOS.h>

#define MEMORY_MAP_TYPE_AVAILABLE        1
#define MEMORY_MAP_TYPE_RESERVED         2
#define MEMORY_MAP_TYPE_ACPI_RECLAIMABLE 3
#define MEMORY_MAP_TYPE_ACPI_NVS         4
#define MEMORY_MAP_TYPE_BAD_MEMORY       5

typedef struct MEMORY_MAP_ENTRY
{
    DWORD baseLow;
    DWORD baseHigh;
    DWORD lengthLow;
    DWORD lengthHigh;
    DWORD type;
    DWORD acpiAttributes;
} __attribute__((packed)) MEMORY_MAP_ENTRY;

MEMORY_MAP_ENTRY *MemoryMapLoad(DWORD *entryCount);