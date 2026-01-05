#include <Utils/StringFormatting.h>

static CHAR FormatBuffer[64] = { 0 };
static const CHAR HexDigits[] = "0123456789ABCDEF";

CHAR *DwordToHexString(DWORD value, BYTE count)
{
    I32 i = 0;

    if (count > 8)
    {
        count = 8;
    }
    if (count < 0)
    {
        count = 0;
    }

    if (count <= 0)
    {
        DWORD tmp = value;
        count = 0;

        do
        {
            count++;
            tmp >>= 4;
        } while (tmp);
    }

    for (i = count - 1; i >= 0; i--)
    {
        FormatBuffer[i] = HexDigits[value & 0xF];
        value >>= 4;
    }

    FormatBuffer[count] = '\0';
    return FormatBuffer;
}