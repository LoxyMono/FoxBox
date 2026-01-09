#include <IO/Disk.h>

VOID DiskReadSector(BYTE drive, QWORD lba, VOID *buffer)
{
    REGS in = { 0 };

    DISK_PACKET packet = { 0 };
    packet.size = sizeof(DISK_PACKET);
    packet.sectorCount = 1;
    packet.offset = 0x1000; /// Hardcoded buffer that can be accessed by BIOS
    packet.segment = 0x0000;
    packet.lbaLow = (DWORD)(lba & 0xFFFFFFFF);
    packet.lbaHigh = (DWORD)((lba >> 32) & 0x0000FFFF);

    in.eax = 0x4200; /// Extended Read Sectors
    in.edx = (DWORD)drive;
    in.si = (DWORD)&packet;
    BIOSCall(0x13, in);

    StringCopy((CHAR *)buffer, (const CHAR *)0x1000, 512);
}