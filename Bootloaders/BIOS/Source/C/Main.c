#include <Types.h>

#include <BIOS.h>

#include <IO/Disk.h>
#include <IO/Ports.h>

#include <Utils/String.h>

#include <Video/VGA.h>

VOID Main()
{
    VgaInit();

    VgaSetColor(VGA_COLOR_ORANGE, VGA_COLOR_BLACK);
    VgaPrintString("FoxBox");
    VgaResetColor();
    VgaPrintString(" x86 BIOS\r\n");
    VgaPrintString("Compiled on " __DATE__ " at " __TIME__ "\r\n\r\n");
}