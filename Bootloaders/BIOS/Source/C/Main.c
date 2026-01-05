#include <Types.h>

#include <BIOS.h>

#include <IO/Ports.h>

#include <Utils/String.h>

#include <Video/VGA.h>

VOID Main()
{
    VgaInit();

    VgaPrintString("Hello from FoxBox!\r\n");
}