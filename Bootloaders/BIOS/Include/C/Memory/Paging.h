#pragma once

#include <Types.h>

#include <Memory/Allocate.h>

#include <Video/VGA.h>

typedef struct PAGE_TABLE_ENTRY
{
    DWORD present        : 1;
    DWORD readWrite      : 1;
    DWORD userSupervisor : 1;
    DWORD writeThrough   : 1;
    DWORD cacheDisabled  : 1;
    DWORD accessed       : 1;
    DWORD dirty          : 1;
    DWORD pageSize       : 1;
    DWORD global         : 1;
    DWORD available      : 3;
    DWORD frameAddress   : 20;
} __attribute__((packed)) PAGE_TABLE_ENTRY;

typedef struct PAGE_DIRECTORY
{
    PAGE_TABLE_ENTRY entries[1024];
} __attribute__((packed)) PAGE_DIRECTORY;

VOID *PagingInit();
VOID  PagingMapPage(PAGE_DIRECTORY *pageDirectory, DWORD virtualAddress, DWORD physicalAddress, BOOL isWritable, BOOL isUser);