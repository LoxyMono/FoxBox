#include <IO/Filesystem.h>

static DWORD mTotalSectors = 0;
static DWORD mFatSize = 0;
static DWORD mRootDirSectors = 0;
static DWORD mFirstDataSector = 0;
static DWORD mFirstFatSector = 0;
static DWORD mDataSectors = 0;
static DWORD mTotalClusters = 0;

static BYTE *mClusterBuffer = NULL_PTR;
static DWORD *mFatTable = NULL_PTR;

static CHAR mFilenameBuffer[256 * 2]; /// Will hold 8.3 filenames and long filenames
static CHAR mCurrentPathBuffer[256];

static FILE_HANDLE *mOpenFiles = NULL_PTR;

static VOID ReadCluster(DWORD cluster, VOID *buffer);
static DWORD GetNextCluster(DWORD cluster);

BOOL FsInit()
{
    VgaPrintString("\t\tInitializing filesystem");

    FAT_BPB *bpb = (FAT_BPB *)0x7C00;

    mTotalSectors = (bpb->totalSectors16 == 0) ? bpb->totalSectors32 : bpb->totalSectors16;
    mFatSize = (bpb->sectorsPerFat16 == 0) ? bpb->sectorsPerFat32 : bpb->sectorsPerFat16;
    mRootDirSectors = ((bpb->rootEntryCount * 32) + (bpb->bytesPerSector - 1)) / bpb->bytesPerSector;
    mFirstDataSector = bpb->reservedSectorCount + (bpb->fatCount * mFatSize) + mRootDirSectors;
    mFirstFatSector = bpb->reservedSectorCount;

    mDataSectors = mTotalSectors - mFirstDataSector;
    mTotalClusters = mDataSectors / bpb->sectorsPerCluster;

    if (bpb->bytesPerSector == 0 || mTotalClusters < 65525)
    {
        VgaSetColor(VGA_COLOR_LIGHTRED, VGA_COLOR_BLACK);
        VgaPrintString("\rFAIL\r\n");
        VgaResetColor();

        return FALSE;
    }

    mClusterBuffer = (BYTE *)Allocate(bpb->bytesPerSector * bpb->sectorsPerCluster);
    mFatTable = (DWORD *)Allocate(bpb->bytesPerSector); /// We only need to read one sector of the FAT at a time
    mOpenFiles = (FILE_HANDLE *)Allocate(sizeof(FILE_HANDLE) * 16);

    StringSet(mFilenameBuffer, 0x00, 256 * 2);
    StringSet(mCurrentPathBuffer, 0x00, 256);
    StringSet(mOpenFiles, 0x00, sizeof(FILE_HANDLE) * 16);

    VgaPrintString("\rDONE\r\n");

    return TRUE;
}

VOID ReadCluster(DWORD cluster, VOID *buffer)
{
    FAT_BPB *bpb = (FAT_BPB *)0x7C00;

    DWORD firstSectorOfCluster = ((cluster - 2) * bpb->sectorsPerCluster) + mFirstDataSector;

    for (DWORD i = 0; i < bpb->sectorsPerCluster; i++)
    {
        DiskReadSector(bpb->driveNumber, firstSectorOfCluster + i, (BYTE *)buffer + (i * bpb->bytesPerSector));
    }
}

DWORD GetNextCluster(DWORD cluster)
{
    FAT_BPB *bpb = (FAT_BPB *)0x7C00;

    DWORD fatOffset = cluster * 4;
    DWORD fatSector = mFirstFatSector + (fatOffset / bpb->bytesPerSector);
    DWORD entOffset = fatOffset % bpb->bytesPerSector;

    DiskReadSector(bpb->driveNumber, fatSector, mFatTable);

    return mFatTable[entOffset / 4] & 0x0FFFFFFF;
}

FILE_HANDLE *FsOpenFile(const CHAR *path)
{
    FAT_BPB *bpb = (FAT_BPB *)0x7C00;

    DWORD currentCluster = bpb->rootCluster;

    _ClusterLoop_:
    while (currentCluster < 0x0FFFFFF8)
    {
        ReadCluster(currentCluster, mClusterBuffer);
        DWORD entriesPerCluster = (bpb->bytesPerSector * bpb->sectorsPerCluster) / 32;
        FAT_FILE_ENTRY *entries = (FAT_FILE_ENTRY *)mClusterBuffer;

        CopyStringUntil(mCurrentPathBuffer, path, '/');
        path += StringLength(mCurrentPathBuffer);

        for (DWORD i = 0; i < entriesPerCluster; i++)
        {
            FAT_FILE_ENTRY *entry = &entries[i];
            
            if (entry->name[0] == 0x00)
            {
                return NULL_PTR;
            }
            else if (entry->name[0] == 0xE5)
            {
                continue;
            }
            else if (entry->attributes & FAT_ATTRIBUTE_ARCHIVE)
            {
                DWORD charsInName = CharsUntilDelimiter(mFilenameBuffer, '\0');

                if (StringCompare(mFilenameBuffer, mCurrentPathBuffer, charsInName))
                {
                    FILE_HANDLE *fileHandle = NULL_PTR;
                    for (DWORD j = 0; j < 16; j++)
                    {
                        if (mOpenFiles[j].fileSize == 0)
                        {
                            fileHandle = &mOpenFiles[j];
                            break;
                        }
                    }

                    if (fileHandle == NULL_PTR)
                    {
                        VgaPrintString("No available file handles\r\n");
                        return NULL_PTR;
                    }

                    fileHandle->startCluster = ((DWORD)entry->firstClusterHigh << 16) | (DWORD)entry->firstClusterLow;
                    fileHandle->fileSize = entry->fileSize;

                    return fileHandle;
                }
                else
                {
                    StringSet(mFilenameBuffer, 0x00, 256 * 2);
                }
            }
            else if (entry->attributes & FAT_ATTRIBUTE_DIRECTORY)
            {
                DWORD charsInName = CharsUntilDelimiter(mFilenameBuffer, '\0');

                if (StringCompare(mFilenameBuffer, mCurrentPathBuffer, charsInName))
                {
                    if (*path == '/')
                    {
                        path++;
                        currentCluster = ((DWORD)entry->firstClusterHigh << 16) | (DWORD)entry->firstClusterLow;
                        StringSet(mFilenameBuffer, 0x00, 256 * 2);
                        StringSet(mCurrentPathBuffer, 0x00, 256);
                        goto _ClusterLoop_;
                    }
                    else if (*path != '\0')
                    {
                        VgaPrintString("Path not found\r\n");
                        return NULL_PTR;
                    }
                }
                else
                {
                    StringSet(mFilenameBuffer, 0x00, 256 * 2);
                }
            }
            else if (entry->attributes & FAT_ATTRIBUTE_LONGNAME)
            {
                FAT_LONG_FILE_ENTRY *longEntry = (FAT_LONG_FILE_ENTRY *)entry;
                DWORD longNameIndex = (longEntry->order & 0x1F) - 1;
                
                for (DWORD j = 0; j < 5; j++)
                {
                    mFilenameBuffer[longNameIndex * 13 + j] = (CHAR)longEntry->name1[j];
                }
                for (DWORD j = 0; j < 6; j++)
                {
                    mFilenameBuffer[longNameIndex * 13 + 5 + j] = (CHAR)longEntry->name2[j];
                }
                for (DWORD j = 0; j < 2; j++)
                {
                    mFilenameBuffer[longNameIndex * 13 + 11 + j] = (CHAR)longEntry->name3[j];
                }

                if (longEntry->order & 0x40)
                {
                    mFilenameBuffer[(longEntry->order & 0x1F) * 13] = 0x00;
                }
            }
        }

        currentCluster = GetNextCluster(currentCluster);
    }

    return NULL_PTR;
}

BOOL FsReadFile(FILE_HANDLE *handle, VOID *buffer, DWORD count)
{
    FAT_BPB *bpb = (FAT_BPB *)0x7C00;

    DWORD bytesPerCluster = bpb->bytesPerSector * bpb->sectorsPerCluster;
    DWORD bytesRead = 0;
    DWORD currentCluster = handle->startCluster;

    while (bytesRead < count && currentCluster < 0x0FFFFFF8)
    {
        ReadCluster(currentCluster, (BYTE *)buffer + bytesRead);

        if (count - bytesRead < bytesPerCluster)
        {
            bytesRead += (count - bytesRead);
        }
        else
        {
            bytesRead += bytesPerCluster;
        }

        currentCluster = GetNextCluster(currentCluster);
    }

    return TRUE;
}