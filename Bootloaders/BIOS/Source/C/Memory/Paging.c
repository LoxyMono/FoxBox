#include <Memory/Paging.h>

PAGE_DIRECTORY *mPageDirectory = NULL_PTR;

VOID *PagingInit()
{
    VgaPrintString("\t\tInitializing paging");

    mPageDirectory = (PAGE_DIRECTORY *)Allocate(sizeof(PAGE_DIRECTORY));
    StringSet((BYTE *)mPageDirectory, 0x00, sizeof(PAGE_DIRECTORY));

    /// Identity map the first 1MB of memory
    for (DWORD addr = 0; addr < 0x100000; addr += 0x1000)
    {
        PagingMapPage(mPageDirectory, addr, addr, TRUE, FALSE);
    }

    /// Set the last PDE entry to itself to allow for easy access to page tables
    mPageDirectory->entries[1023].present = 1;
    mPageDirectory->entries[1023].readWrite = 1;
    mPageDirectory->entries[1023].frameAddress = (DWORD)mPageDirectory >> 12;

    return mPageDirectory;
}

VOID PagingMapPage(PAGE_DIRECTORY *pageDirectory, DWORD virtualAddress, DWORD physicalAddress, BOOL isWritable, BOOL isUser)
{
    DWORD pdIndex = (virtualAddress >> 22) & 0x3FF;
    DWORD ptIndex = (virtualAddress >> 12) & 0x3FF;

    PAGE_TABLE_ENTRY *pageTable;

    if (pageDirectory->entries[pdIndex].present == 0)
    {
        pageTable = (PAGE_TABLE_ENTRY *)Allocate(sizeof(PAGE_TABLE_ENTRY));
        StringSet((BYTE *)pageTable, 0x00, sizeof(PAGE_TABLE_ENTRY) * 1024);

        pageDirectory->entries[pdIndex].present = 1;
        pageDirectory->entries[pdIndex].readWrite = 1;
        pageDirectory->entries[pdIndex].userSupervisor = 1;
        pageDirectory->entries[pdIndex].frameAddress = (DWORD)pageTable >> 12;
    }
    else
    {
        pageTable = (PAGE_TABLE_ENTRY *)(pageDirectory->entries[pdIndex].frameAddress << 12);
    }

    pageTable[ptIndex].present = 1;
    pageTable[ptIndex].readWrite = isWritable ? 1 : 0;
    pageTable[ptIndex].userSupervisor = isUser ? 1 : 0;
    pageTable[ptIndex].frameAddress = physicalAddress >> 12;
}