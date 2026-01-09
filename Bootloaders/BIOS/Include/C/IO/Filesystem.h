#pragma once

#include <Types.h>

#include <IO/Disk.h>

#include <Memory/Allocate.h>

#include <Video/VGA.h>

#define FAT_ATTRIBUTE_READONLY    1 << 0
#define FAT_ATTRIBUTE_HIDDEN      1 << 1
#define FAT_ATTRIBUTE_SYSTEM      1 << 2
#define FAT_ATTRIBUTE_VOLUMEID    1 << 3
#define FAT_ATTRIBUTE_DIRECTORY   1 << 4
#define FAT_ATTRIBUTE_ARCHIVE     1 << 5
#define FAT_ATTRIBUTE_LONGNAME   (FAT_ATTRIBUTE_READONLY | FAT_ATTRIBUTE_HIDDEN | FAT_ATTRIBUTE_SYSTEM | FAT_ATTRIBUTE_VOLUMEID)

typedef struct FAT_BPB
{
    BYTE jmp[3];
    CHAR oemId[8];
    WORD bytesPerSector;
    BYTE sectorsPerCluster;
    WORD reservedSectorCount;
    BYTE fatCount;
    WORD rootEntryCount;
    WORD totalSectors16;
    BYTE mediaType;
    WORD sectorsPerFat16;
    WORD sectorsPerTrack;
    WORD headCount;
    DWORD hiddenSectors;
    DWORD totalSectors32;

    DWORD sectorsPerFat32;
    WORD  flags;
    WORD  version;
    DWORD rootCluster;
    WORD  fsInfoSector;
    WORD  backupBootSector;
    BYTE  reserved0[12];
    BYTE  driveNumber;
    BYTE  reserved1;
    BYTE  bootSignature;
    DWORD volumeId;
    CHAR  volumeLabel[11];
    CHAR  fsType[8];

} __attribute__((packed)) FAT_BPB;

typedef struct FAT_FILE_ENTRY
{
    CHAR     name[11];
    BYTE     attributes;
    BYTE     reserved;
    BYTE     creationTimeHundredths;
    WORD     creationTime;
    WORD     creationDate;
    WORD     lastAccessDate;
    WORD     firstClusterHigh;
    WORD     writeTime;
    WORD     writeDate;
    WORD     firstClusterLow;
    DWORD    fileSize;
} __attribute__((packed)) FAT_FILE_ENTRY;

typedef struct FAT_LONG_FILE_ENTRY
{
    BYTE     order;
    W_CHAR   name1[5];
    BYTE     attributes;
    BYTE     type;
    BYTE     checksum;
    W_CHAR   name2[6];
    WORD     zero;
    W_CHAR   name3[2];
} __attribute__((packed)) FAT_LONG_FILE_ENTRY;

typedef struct FILE_HANDLE
{
    DWORD startCluster;
    DWORD fileSize;
} __attribute__((packed)) FILE_HANDLE;

BOOL FsInit();
FILE_HANDLE *FsOpenFile(const CHAR *path);
BOOL FsReadFile(FILE_HANDLE *handle, VOID *buffer, DWORD count);
VOID FsCloseFile(FILE_HANDLE *handle);