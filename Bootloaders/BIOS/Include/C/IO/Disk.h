#pragma once

#include <Types.h>

#include <BIOS.h>

typedef struct DISK_PACKET
{
    BYTE  size;
    BYTE  reserved;
    WORD  sectorCount;
    WORD  offset;
    WORD  segment;
    DWORD lbaLow;
    DWORD lbaHigh;
} __attribute__((packed)) DISK_PACKET;

VOID DiskReadSector(BYTE drive, QWORD lba, VOID *buffer);