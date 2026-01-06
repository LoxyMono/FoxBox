#include <Memory/Allocate.h>

extern PtrLoaderEnd;

static VOID *AllocatePages(DWORD count);
static VOID ReleasePages(VOID *ptr, DWORD count);

DWORD mPageCount = 0;
DWORD mAllocationMapSize = 0;

PAGE *mPages;

ALLOCATION *mAllocationMap = NULL_PTR;

VOID AllocateInit(MEMORY_MAP_ENTRY *memoryMap)
{
    VgaPrintString("\t\tInitializing physical memory allocator");

    if (memoryMap == NULL_PTR)
    {
        VgaSetColor(VGA_COLOR_LIGHTRED, VGA_COLOR_BLACK);
        VgaPrintString("\rERR\r\n");
        VgaResetColor();
        VgaPrintString("\t\tNo valid memory map provided\r\n");
        return;
    }

    DWORD currentAddress = memoryMap->baseLow;
    mPages = currentAddress;

    while (currentAddress + 0x1000 < memoryMap->baseLow + memoryMap->lengthLow)
    {
        StringCopy("FREE", (CHAR *)currentAddress, 4);
        currentAddress += 0x1000;
        mPageCount++;
    }

    mAllocationMap = AllocatePages(4);
    mAllocationMapSize = (4 * 4096) / sizeof(ALLOCATION);

    VgaPrintString("\rDONE\r\n");

    VgaPrintString("\t\t%uMB of available memory\r\n", (mPageCount * 4096) / (1024 * 1024));
}

VOID *Allocate(DWORD size)
{
    for (DWORD i = 0; i < mAllocationMapSize; i++)
    {
        ALLOCATION *alloc = &mAllocationMap[i];

        if (alloc->address == 0x00000000)
        {
            DWORD pageCount = 0;
            I32 sizeLeft = size;

            while (sizeLeft > 0)
            {
                sizeLeft -= 4096;
                pageCount++;
            }

            VOID *allocTemp = AllocatePages(pageCount);
            alloc->address = (DWORD)allocTemp;
            alloc->size = size;

            return (VOID *)alloc->address;
        }
    }

    return NULL_PTR;
}

VOID Release(VOID *ptr)
{
    for (DWORD i = 0; i < mAllocationMapSize; i++)
    {
        ALLOCATION *alloc = &mAllocationMap[i];
        
        if (alloc->address == ptr)
        {
            DWORD pageCount = 0;
            I32 sizeLeft = alloc->size;

            while (sizeLeft > 0)
            {
                sizeLeft -= 4096;
                pageCount++;
            }
            
            ReleasePages((VOID *)alloc->address, pageCount);

            alloc->address = 0x00000000;
            alloc->size = 0;

            return;
        }
    }

    VgaPrintString("\t\tNo such allocation at 0x%8x\r\n", ptr);
}

VOID *AllocatePages(DWORD count)
{
    DWORD pagesLeft = count;

    for (DWORD i = 0; i < mPageCount; i++)
    {
        if (StringCompare("FREE", mPages[i].data, 4))
        {
            pagesLeft--;
        }
        else
        {
            pagesLeft = count;
            continue;
        }

        if (pagesLeft == 0)
        {
            StringSet(mPages[i - count + 1].data, 0x00, count * 4096);

            return &mPages[i - count + 1];
        }
    }

    VgaSetColor(VGA_COLOR_LIGHTRED, VGA_COLOR_BLACK);
    VgaPrintString("\r\n\t\tAllocate: Out of free pages\r\n");
    VgaResetColor();

    return NULL_PTR;
}

VOID ReleasePages(VOID *ptr, DWORD count)
{
    for (DWORD i = 0; i < count; i++)
    {
        PAGE *p = ptr + (i * sizeof(PAGE));

        StringSet(p, 0x00, 4096);
        StringCopy("FREE", p->data, 4);
    }
}