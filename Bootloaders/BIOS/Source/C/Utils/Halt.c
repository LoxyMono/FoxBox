#include <Utils/Halt.h>

VOID Halt(DWORD code, const CHAR *message)
{
    VgaSetColor(VGA_COLOR_LIGHTRED, VGA_COLOR_BLACK);
    VgaPrintString("\r\nBoot error 0x%8x: %s\r\n", code, message);
    VgaSetColor(VGA_COLOR_DARKGREY, VGA_COLOR_BLACK);
    VgaPrintString("System halted\r\n");

    asm volatile ("1: cli; hlt; jmp 1b");
}